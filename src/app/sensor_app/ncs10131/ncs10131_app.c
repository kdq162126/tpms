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
#include "ncs10131/ncs10131_drv.h"
#include "utils/nxp_console_adapter.h"
#include "utils/nxp_console.h"


/* Temperature */
#define TEMP_25DEG			(23040)
#define TEMP_SENS			(256)

/* ARC */
#define ARC_SENS_250DPS		(131)

/* Acceleration
 * Acc0 is X, Acc1 is Y and Acc2 is Z */
#define ACC_SENS_6G			(5458)

/* Variable keeping track of whether initialization was successful */
bool bNCS10131_init_ok = false;


void NCS10131_Init (void)
{
	uint8_t u8Status;

	bNCS10131_init_ok = false;

	NCS10131_DRV_Init();

	PRINTF("\n\r");

	u8Status = NCS10131_DRV_Reset_device();
	if (STATUS_OK != u8Status)
	{
		PRINTF("NCS10131 reset failed, configuration not performed. \n\r");
		PRINTF("\n\r");
		return;
	}

	u8Status = NCS10131_DRV_Cfg();
	if (STATUS_OK != u8Status)
	{
		PRINTF("NCS10131 configuration failed. \n\r");
		PRINTF("\n\r");
		return;
	}

	bNCS10131_init_ok = true;

	u8Status = NCS10131_DRV_Lock_config();
	if (STATUS_OK == u8Status)
	{
		PRINTF("NCS10131 successful configured <ARC range: +/-250dps, ACC range: +/-6g> and Locked. \n\r");
	}
	else
	{
		PRINTF("NCS10131 successful configured <ARC range: +/-250dps, ACC range: +/-6g>, but Lock failed. \n\r");
	}
	PRINTF("\n\r");

	return;
}

void NCS10131_Read_data (void)
{
	uint8_t u8Status;
	uint8_t u8Cnt;
	uint8_t u8Devstat;
	uint8_t u8Devstat1;
	uint8_t u8Devstat2;
	uint8_t u8Devstat3;
	uint8_t u8SensStatus;
	int16_t i16Data;
	int16_t i16DataConverted;
	float fDataConverted;

	if (true == bNCS10131_init_ok)
	{
		PRINTF("NCS10131: reading data \n\r");

		/* Read status */
		u8Status = NCS10131_DRV_Read_COUNT_DEVSTAT(&u8Cnt, &u8Devstat, &u8Devstat1, &u8Devstat2, &u8Devstat3);

		if (STATUS_OK == u8Status)
		{
			PRINTF("DEVSTAT = 0x%02X \n\r", u8Devstat);
			//PRINTF("DEVSTAT1 = 0x%02X \n\r", u8Devstat1);
			//PRINTF("DEVSTAT2 = 0x%02X \n\r", u8Devstat2);
			//PRINTF("DEVSTAT3 = 0x%02X \n\r", u8Devstat3);
		}
		else
		{
			PRINTF("Error while reading DEVSTAT. \n\r");
		}


		/* Temperature */
		u8Status = NCS10131_DRV_Read_TEMPERATURE(&i16Data);

		if (STATUS_OK == u8Status)
		{
			/* Convert in degC */
			i16DataConverted = ((i16Data - TEMP_25DEG) / TEMP_SENS) + 25;
			PRINTF("Temperature = %d LSB <=> %d degC \n\r",i16Data, i16DataConverted);
		}
		else
		{
			PRINTF("Error while reading Temperature. \n\r");
		}

		/* Gyroscope */
		u8Status = NCS10131_DRV_Read_sens(ARC_0_SID, &i16Data, &u8SensStatus);

		if (STATUS_OK == u8Status)
		{
			/* Convert in DPS */
			i16DataConverted = (i16Data) / ARC_SENS_250DPS;
			PRINTF("ARC = %d LSB <=> %d dps. Status S[1:0] = 0x%02X. \n\r",i16Data, i16DataConverted, u8SensStatus);
		}
		else if (STATUS_NO_SENSOR_DATA == u8Status)
		{
			PRINTF("ARC data not available. \n\r");
		}
		else
		{
			PRINTF("Error while reading ARC. \n\r");
		}


		/* X-accel */
		u8Status = NCS10131_DRV_Read_sens(ACC0_0_SID, &i16Data, &u8SensStatus);

		if (STATUS_OK == u8Status)
		{
			/* Convert in g */
			fDataConverted = (float)(i16Data) / (float)(ACC_SENS_6G);
			PRINTF("ACC X = %d LSB <=> %f g. Status S[1:0] = 0x%02X. \n\r",i16Data, fDataConverted, u8SensStatus);
			/* Note: to print fDataConverted with only two decimals we could use %.2f, but this does not work for some
			 * values. When fDataConverted = 0.99xxxx then the value printed is 0.00
			 * So, we display all decimals to avoid any issue */
		}
		else if (STATUS_NO_SENSOR_DATA == u8Status)
		{
			PRINTF("ACC X data not available. \n\r");
		}
		else
		{
			PRINTF("Error while reading ACC X. \n\r");
		}

		/* Y-accel */
		u8Status = NCS10131_DRV_Read_sens(ACC1_0_SID, &i16Data, &u8SensStatus);

		if (STATUS_OK == u8Status)
		{
			/* Convert in g */
			fDataConverted = (float)(i16Data) / (float)(ACC_SENS_6G);
			PRINTF("ACC Y = %d LSB <=> %f g. Status S[1:0] = 0x%02X. \n\r", i16Data, fDataConverted, u8SensStatus);
		}
		else if (STATUS_NO_SENSOR_DATA == u8Status)
		{
			PRINTF("ACC Y data not available. \n\r");
		}
		else
		{
			PRINTF("Error while reading ACC Y. \n\r");
		}

		/* Z-accel */
		u8Status = NCS10131_DRV_Read_sens(ACC2_0_SID, &i16Data, &u8SensStatus);

		if (STATUS_OK == u8Status)
		{
			/* Convert in g */
			fDataConverted = (float)(i16Data) / (float)(ACC_SENS_6G);
			PRINTF("ACC Z = %d LSB <=> %f g. Status S[1:0] = 0x%02X. \n\r", i16Data, fDataConverted, u8SensStatus);
		}
		else if (STATUS_NO_SENSOR_DATA == u8Status)
		{
			PRINTF("ACC Z data not available. \n\r");
		}
		else
		{
			PRINTF("Error while reading ACC Z. \n\r");
		}

		PRINTF("\n\r");
	}
	else
	{
		PRINTF("NCS10131 configuration was not performed. \n\r");
		PRINTF("\n\r");
	}
}

