/*
   (c) NXP B.V. 2009-2018, 2021-2022. All rights reserved.

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


/* ###################################################################
**     Filename    : main.c
**     Processor   : S32K14x
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */


/* Including necessary module. Cpu.h contains other modules needed for compiling.*/
#include "Cpu.h"

#include "utils/nxp_console_adapter.h"
#include "utils/nxp_console.h"
#include "pin_mux.h"
#include "gpio_int.h"
#include "RF.h"
#include "lf/LF.h"
#include "handle_cmd.h"
#include "sensor_selection.h"
#ifdef USE_MPL3115
#include "mpl3115/mpl3115_app.h"
#endif
#ifdef USE_NCS10131
#include "ncs10131/ncs10131_app.h"
#endif
#include "lpuart1.h"
#include "LED_LZ.h"

uint32_t x = 0;

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
	/* Clock initialization */
	CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                 g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Pins initialization.
     * Init pins for LEDs, buttons, LF and communication with sensors (if enabled).
     * See pin_mux.h for more details.
     * Note that pins for Lizard are initialized later, in RF_LZ_init(). */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    /* GPIO install interrupts: install interrupts for the switches (buttons), and LZ INT signal */
    GPIO_install_int();

    /* Init LPUART1 - must be done before Lizard and sensors init
	 * Set SDK_DEBUGCONSOLE to 1U to enable debug prints, set it to 0 to disable */
    (void)DbgConsole_Init(BOARD_DEBUG_UART_BASEADDR, BOARD_DEBUG_UART_TYPE, &lpuart1_State,&lpuart1_InitConfig0);

    /* Lizard driver initialization */
    RF_LZ_init();
    /* Lizard configuration for reception of TPMS frames */
    RF_LZ_config();

#ifdef USE_MPL3115
    /* Sensor: MPL3115 Init */
#if (MPL3115_MODE == MODE_INTERRUPT)
    /* Configure the sensor, enable interrupts. Set mode to Active.
     * New sensor data is read when ready.
     * Change mode (Active <-> Standby) by pushing SW1 button. */
    MPL3115_Cfg_interrupt();
    MPL3115_Start();
#elif (MPL3115_MODE == MODE_POLLING)
    /* Configure the sensor, do not enable interrupts. Set mode to Active.
     * Check if new sensor data is ready, and read it, by pushing SW1 button. */
    MPL3115_Cfg_no_interrupt();
    MPL3115_Start();
#elif (MPL3115_MODE == MODE_ONE_SHOT)
    /* Configure the sensor, do not enable interrupts. Keep sensor in Standby.
     * Trigger new sensor data measurement by pushing SW1 button. */
    //MPL3115_Cfg_no_interrupt();
#endif
#endif

#ifdef USE_NCS10131
    /* Sensor: NCS10131 Init
     * Sensor data is read when SW2 button is pushed */
    NCS10131_Init();
#endif

    /* FTM0 and FTM2 initialization for LF transmission */
    LF_init();

    /* UART: install the callback for rx events */
    LPUART_DRV_InstallRxCallback(INST_LPUART1, UART_rxCallback, NULL);
    /* Enable command reception */
    UART_EnableRxInterrupt();
    //QueueHandle_t xQueue = NULL;
        //xQueue = xQueueCreate(1, sizeof(unsigned long));

	while(true)
	{
		x++;
		/* Check if we have received an RF frame and display it */
		RF_LZ_check_for_message_received();
		PRINTF("\n\r-----------------%d     \n\r", x);
		OSIF_TimeDelay(2000);
		/* Check if we have received a command via UART and execute it */
		if (1 == gu8CmdComplete)
		{
			UART_HandleCmd();
		    UART_EnableRxInterrupt();
		}

#ifdef USE_MPL3115
#if (MPL3115_MODE == MODE_INTERRUPT)

		/* Check MPL3115 INT2 */
		if (gu8MPL3115_INT2)
		{
			gu8MPL3115_INT2 = 0;
			MPL3115_Check_flag_read_data();
		}

#endif
#endif

		/* Handle buttons */
		if (1 == u8Sw1_LZ_pressed)
		{
			u8Sw1_LZ_pressed = 0;
			LED3_Toggle();

#ifdef USE_MPL3115
#if (MPL3115_MODE == MODE_POLLING)
			/* Check if new sensor data is ready, and if so, read it */
			MPL3115_Check_flag_read_data();
#elif (MPL3115_MODE == MODE_INTERRUPT)
			/* Change MPL3115 mode: if currently active, set to Standby, and vice-versa */
			MPL3115_Change_mode();
#elif (MPL3115_MODE == MODE_ONE_SHOT)
			/* Trigger data acquisition once */
			MPL3115_One_shot_measurement();
#endif
#endif
		}

		if (1 == u8Sw2_LZ_pressed)
		{
			u8Sw2_LZ_pressed = 0;
			LED2_Toggle();

#ifdef USE_NCS10131
			/* Read NCS10131 data once */
	    	NCS10131_Read_data();
#endif
		}

		if (1 == u8Sw3_S32_pressed)
		{
			u8Sw3_S32_pressed = 0;
			LED4_Toggle();
		}

		if (1 == u8Sw2_S32_pressed)
		{
			u8Sw2_S32_pressed = 0;
			LED5_Toggle();
		}

	} /* end while(true) */

} /*** End of main routine ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/
