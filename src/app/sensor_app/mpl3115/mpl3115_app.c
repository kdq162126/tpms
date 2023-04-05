/*
   (c) NXP B.V. 2022. All rights reserved.

   Disclaimer
   1. The NXP Software/Source Code is provided to Licensee "AS IS" without any
      warranties of any kind. NXP makes no warranties to Licensee and shall not
      indemnify Licensee or hold it harmless for any reason related to the NXP
      Software/Source Code or otherwise be liable to the NXP customer. The NXP
      customer acknowledges and agrees that the NXP Software/Source Code is
      provided AS-IS and accepts all risks of utilizing the NXP Software under
      the conditions set forth according to this disclaimer.

   2. NXP EXPRESSLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING,
      BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
      FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT OF INTELLECTUAL PROPERTY
      RIGHTS. NXP SHALL HAVE NO LIABILITY TO THE NXP CUSTOMER, OR ITS
      SUBSIDIARIES, AFFILIATES, OR ANY OTHER THIRD PARTY FOR ANY DAMAGES,
      INCLUDING WITHOUT LIMITATION, DAMAGES RESULTING OR ALLEGED TO HAVE
      RESULTED FROM ANY DEFECT, ERROR OR OMISSION IN THE NXP SOFTWARE/SOURCE
      CODE, THIRD PARTY APPLICATION SOFTWARE AND/OR DOCUMENTATION, OR AS A
      RESULT OF ANY INFRINGEMENT OF ANY INTELLECTUAL PROPERTY RIGHT OF ANY
      THIRD PARTY. IN NO EVENT SHALL NXP BE LIABLE FOR ANY INCIDENTAL,
      INDIRECT, SPECIAL, EXEMPLARY, PUNITIVE, OR CONSEQUENTIAL DAMAGES
      (INCLUDING LOST PROFITS) SUFFERED BY NXP CUSTOMER OR ITS SUBSIDIARIES,
      AFFILIATES, OR ANY OTHER THIRD PARTY ARISING OUT OF OR RELATED TO THE NXP
      SOFTWARE/SOURCE CODE EVEN IF NXP HAS BEEN ADVISED OF THE POSSIBILITY OF
      SUCH DAMAGES.

   3. NXP reserves the right to make changes to the NXP Software/Sourcecode any
      time, also without informing customer.

   4. Licensee agrees to indemnify and hold harmless NXP and its affiliated
      companies from and against any claims, suits, losses, damages,
      liabilities, costs and expenses (including reasonable attorney's fees)
      resulting from Licensee's and/or Licensee customer's/licensee's use of the
      NXP Software/Source Code.
*/

#include "Cpu.h"
#include "mpl3115/mpl3115_app.h"
//#include "mpl3115/mpl3115_drv.h"
#include "utils/nxp_console_adapter.h"
#include "utils/nxp_console.h"
#include "status.h"
#include "aml/gpio_aml.h"



#define MPL3115_PRESSURE_CONV_FACTOR 	(64)     /* Will give Pascals */
#define MPL3115_TEMPERATURE_CONV_FACTOR (256) 	 /* Will give degC */

#if (MPL3115_MODE == MODE_INTERRUPT)
#define MPL3115_INT2_PORT_IRQN			PORTB_IRQn
#define MPL3115_INT2_INSTANCE			instanceB
#define MPL3115_INT2_PIN_NB				(uint8_t)(8)
#endif

/*********************************************************
 * Global variables
 *********************************************************/
status_t guInitStatus;
bool bIsMPL3115_active = false;
#if (MPL3115_MODE == MODE_INTERRUPT)
uint8_t gu8MPL3115_INT2 = 0;
#endif

/*********************************************************
 * Local definitions
 *********************************************************/
void MPL3115_Read_data (void);
#if (MPL3115_MODE == MODE_INTERRUPT)
void MPL3115_Install_INT2_interrupt (void);
void vPort_B_ISRHandler (void);
void MPL3115_Check_INT2_interrupt (void);
#endif

/* Status **************************************************************************************

    STATUS_SUCCESS						   = 0x000U,
    STATUS_BUSY                            = 0x002U,    !< Generic operation busy status
    STATUS_TIMEOUT                         = 0x003U,    !< Generic operation timeout status
   I2C specific error codes
    STATUS_I2C_RECEIVED_NACK               = 0x200U,  !< NACK signal received
    STATUS_I2C_TX_UNDERRUN                 = 0x201U,  !< TX underrun error
    STATUS_I2C_RX_OVERRUN                  = 0x202U,  !< RX overrun error
    STATUS_I2C_ARBITRATION_LOST            = 0x203U,  !< Arbitration lost
    STATUS_I2C_ABORTED                     = 0x204U,  !< A transfer was aborted
    STATUS_I2C_BUS_BUSY                    = 0x205U,  !< I2C bus is busy, cannot start transfer
 */


#if (MPL3115_MODE == MODE_INTERRUPT)
/* Configure the sensor without starting it. Interrupts are used */
void MPL3115_Cfg_interrupt (void)
{
	/* Init I2C */
	guInitStatus = MPL3115_DRV_Init();

	if (STATUS_SUCCESS == guInitStatus)
	{
		guInitStatus = 	MPL3115_DRV_Cfg_interrupt_no_fifo();
		MPL3115_Install_INT2_interrupt();
	}

	PRINTF("\n\r");

	if (STATUS_SUCCESS == guInitStatus)
	{
		PRINTF("MPL3115 successfully configured (interrupt, no FIFO mode). \n\r");
	}
	else
	{
		PRINTF("MPL3115 configuration failed. \n\r");
		PRINTF("\n\r");
		return;
	}

	/* In case we are here after S32K reset but the MPL3115 did not reset, maybe the application
	 * missed an INT2 interrupt. Just check if an INT2 interrupt is pending */
	MPL3115_Check_INT2_interrupt();

	return;
}

#else

/* Configure the sensor without starting it. Interrupts are not used */
void MPL3115_Cfg_no_interrupt (void)
{
	/* Init I2C */
	guInitStatus = MPL3115_DRV_Init();

	if (STATUS_SUCCESS == guInitStatus)
	{
		/* Configure sensor as Barometer, interrupts and FIFO not used */
		guInitStatus = 	MPL3115_DRV_Cfg_polling_no_fifo();
	}

	PRINTF("\n\r");

	if (STATUS_SUCCESS == guInitStatus)
	{
#if (MPL3115_MODE == MODE_POLLING)
		PRINTF("MPL3115 successfully configured (polling, no FIFO mode). \n\r");
#else
		PRINTF("MPL3115 successfully configured (one-shot, no FIFO mode). \n\r");
#endif
	}
	else
	{
		PRINTF("MPL3115 configuration failed. \n\r");
		PRINTF("\n\r");
		return;
	}

	return;
}
#endif

void MPL3115_Read_data (void)
{
	status_t status;
	uint32_t u32RawPressure;
	int16_t i16RawTemperature;
	uint32_t u32Pressure_Pascals;
	int16_t i16Temperature_degC;

	/* Make sure sensor has been initialized */
	if (STATUS_SUCCESS == guInitStatus)
	{
		/* Read data */
		status = MPL3115_DRV_Read_P_T(&u32RawPressure, &i16RawTemperature);

		if (STATUS_SUCCESS == status)
		{
			u32Pressure_Pascals = u32RawPressure / MPL3115_PRESSURE_CONV_FACTOR;
			i16Temperature_degC = i16RawTemperature / MPL3115_TEMPERATURE_CONV_FACTOR;
			PRINTF("32-bit Pressure = 0x%08X <=> P = %d Pascals. \n\r", u32RawPressure, u32Pressure_Pascals);
			PRINTF("16-bit Temperature = 0x%04X  <=> T = %d degC. \n\r", i16RawTemperature, i16Temperature_degC);
			PRINTF("\n\r");
		}
		else
		{
			PRINTF("Error while reading MPL3115 data. \n\r");
		}
	}
	else
	{
		PRINTF("MPL3115 has not been configured. \n\r");
	}

	return;
}

#if (MPL3115_MODE == MODE_ONE_SHOT)

void MPL3115_One_shot_measurement (void)
{
	status_t status;
	bool bIsOstClear;

	/* Make sure sensor has been initialized */
	if (STATUS_SUCCESS == guInitStatus)
	{
		/* Trigger one-shot measurement */
		status = MPL3115_DRV_Set_OST();

		if (STATUS_SUCCESS == status)
		{
			PRINTF("MPL3115 One-shot measurement triggered. \n\r");

			/* Wait for OST to be clear before reading the data */
			do
			{
				MPL3115_DRV_Check_OST_bit(&bIsOstClear);
			} while (false == bIsOstClear);

			/* OST is clear, read the data */
			MPL3115_Read_data();
		}
		else
		{
			PRINTF("MPL3115 One-shot measurement could not be triggered. \n\r");
			PRINTF("\n\r");
		}
	}

	return;
}

#else

void MPL3115_Start (void)
{
	status_t status;
	bool bIsInStandby;

	/* Make sure sensor is configured */
	if (STATUS_SUCCESS == guInitStatus)
	{
		status = MPL3115_DRV_Set_active_mode();

		if (STATUS_SUCCESS != status)
		{
			PRINTF("Error while trying to put MPL3115 in Active mode. \n\r");
			return;
		}

		/* Check sensor mode */
		status = MPL3115_DRV_Check_mode(&bIsInStandby);

		if ((STATUS_SUCCESS == status) && (false == bIsInStandby))
		{
			bIsMPL3115_active = true;
			PRINTF("MPL3115 successfully put in Active mode. \n\r");
			PRINTF("\n\r");
		}
		else if ((STATUS_SUCCESS == status) && (true == bIsInStandby))
		{
			PRINTF("MPL3115 could not be put in Active mode. \n\r");
			PRINTF("\n\r");
		}
		else
		{
			PRINTF("Error while checking MPL3115 mode. \n\r");
		}
	}

	return;
}

void MPL3115_Stop (void)
{
	status_t status;
	bool bIsInStandby;

	/* Make sure sensor is configured */
	if (STATUS_SUCCESS == guInitStatus)
	{
		status = MPL3115_DRV_Set_standby_mode();

		if (STATUS_SUCCESS != status)
		{
			PRINTF("Error while trying to put MPL3115 in Standby mode. \n\r");
			return;
		}

		/* Check sensor mode */
		status = MPL3115_DRV_Check_mode(&bIsInStandby);

		if ((STATUS_SUCCESS == status) && (true == bIsInStandby))
		{
			bIsMPL3115_active = false;
			PRINTF("MPL3115 successfully put in Standby mode. \n\r");
			PRINTF("\n\r");
		}
		else if ((STATUS_SUCCESS == status) && (false == bIsInStandby))
		{
			PRINTF("MPL3115 could not be put in Standby mode. \n\r");
			PRINTF("\n\r");
		}
		else
		{
			PRINTF("Error while checking MPL3115 mode. \n\r");
		}

#if (MPL3115_MODE == MODE_INTERRUPT)
		/* The MPL3115 may have triggered an interrupt while we were handling the request
		 * to put the sensor in Standby mode.
		 * Make sure there is no pending interrupt. */
		MPL3115_Check_INT2_interrupt();
#endif
	}

	return;
}

void MPL3115_Change_mode (void)
{
	/* If current mode is Active mode, change to Standby */
	if (true == bIsMPL3115_active)
	{
		MPL3115_Stop();
	}
	else
	{
		MPL3115_Start();
	}
}

void MPL3115_Check_flag_read_data (void)
{
	status_t status;
	bool bIsDataReady;

	/* Make sure sensor has been initialized */
	if (STATUS_SUCCESS == guInitStatus)
	{
#if (MPL3115_MODE == MODE_POLLING)
		status = MPL3115_DRV_Check_data_ready(&bIsDataReady);
#elif (MPL3115_MODE == MODE_INTERRUPT)
		status = MPL3115_DRV_Check_interrupt_source(&bIsDataReady);
#endif

		if ((STATUS_SUCCESS == status) && (true == bIsDataReady))
		{
			PRINTF("MPL3115 Data ready. \n\r");

			MPL3115_Read_data();
		}
		else if ((STATUS_SUCCESS == status) && (false == bIsDataReady))
		{
			PRINTF("MPL3115 data not ready yet. \n\r");
			PRINTF("\n\r");
		}
		else
		{
			PRINTF("Error while reading MPL3115 status or interrupt source. \n\r");
		}
	}
	else
	{
		PRINTF("MPL3115 has not been configured. \n\r");
	}

	return;
}

#endif

#if (MPL3115_MODE == MODE_INTERRUPT)

void MPL3115_Install_INT2_interrupt (void)
{
	/* Install INT2 interrupt handler */
    INT_SYS_InstallHandler(MPL3115_INT2_PORT_IRQN, vPort_B_ISRHandler, (isr_t *)NULL);
    /* Enable INT2 interrupt handler */
    INT_SYS_EnableIRQ(MPL3115_INT2_PORT_IRQN);
}

void vPort_B_ISRHandler (void)
{
	uint32_t u32Flags;

	u32Flags = GPIO_AML_GetInterruptFlags(MPL3115_INT2_INSTANCE);

	/* Check if interrupt comes from INT2 */
	if (u32Flags & (1 << MPL3115_INT2_PIN_NB))
	{
		gu8MPL3115_INT2 = 1;
		GPIO_AML_ClearInterruptFlags(MPL3115_INT2_INSTANCE, MPL3115_INT2_PIN_NB);
	}
}

void MPL3115_Check_INT2_interrupt (void)
{
	/* When the S32K resets (for example due to the user pressing the RST button) then it can happen that
	 * the application misses the INT2 interrupt. In that case the INT2 pin remains at low state.
	 * Here we check if INT2 pin is at low state and the application missed the interrupt. */
	if ((0 == gu8MPL3115_INT2) && (0 == GPIO_AML_ReadInput(MPL3115_INT2_INSTANCE, MPL3115_INT2_PIN_NB)))
	{
		/* INT2 pin is at low state, we missed the interrupt but MPL3115 is still waiting.
		 * So, read the data to clear the flag */
		MPL3115_Check_flag_read_data();
	}
}

#endif

