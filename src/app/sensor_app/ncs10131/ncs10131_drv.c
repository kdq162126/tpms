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
#include "ncs10131/ncs10131.h"
#include "ncs10131/ncs10131_crc.h"
#include "ncs10131/ncs10131_drv.h"
#include "ncs10131/SPI_ncs10131.h"
#include "aml/common_aml.h"
#include "aml/wait_aml/wait_aml.h"


#define NCS10131_SPI_ADDR		(0x00)

/* Declare CRC parameter instance and initialize.
 For the out-of-frame format a 3bit CRC with the polynomial 0x5 (x3 + x1 + x0) is used with a
 start value of 101b and a target value of 000b.
*/
tCrcParameters crc_out_of_frame_param =
{
	 .eType = eSeedType_NonDirect,
	 .u16Polynomial = 0x000B,
	 .u8InitialValue = 0x05,
	 .u8CrcLength = 0x03,
	 .u8DataBitStart = 0,
	 .u8DataLength = 32
};

/*************************************************************************
 * Private functions
 *************************************************************************/
uint8_t NCS10131_DRV_Read_register (uint8_t u8RegAddr, uint8_t *pu8ValHigh, uint8_t *pu8ValLow);
uint32_t NCS10131_DRV_Prepare_read_register_request (uint8_t u8RegAddr);
uint8_t NCS10131_DRV_Check_read_register_response (uint32_t u32Response, uint8_t *pu8RegDataHigh, uint8_t *pu8RegDataLow);

uint8_t NCS10131_DRV_Write_register (uint8_t u8RegAddr, uint8_t u8Val);
uint32_t NCS10131_DRV_Prepare_write_register_request (uint8_t u8RegAddr, uint8_t u8Val);
uint8_t NCS10131_DRV_Check_write_register_response (uint32_t u32Response, uint8_t *pu8ValHigh, uint8_t *pu8ValLow);

uint8_t NCS10131_DRV_Read_sensor_data (uint8_t u8SensorId, int16_t *pi16Data, uint8_t *pu8HasData, uint8_t *pu8SenStatus);
uint32_t NCS10131_DRV_Prepare_read_sensor_request (uint8_t u8SensorId);
uint8_t NCS10131_DRV_Check_read_sensor_response (uint32_t u32Response, uint8_t *pu8SensorId3bit, uint8_t *pu8HasData, int16_t *pi16Data, uint8_t *pu8SensStatus);




void NCS10131_DRV_Init (void)
{
	/* Note that pins are initialized in PINS_DRV_Init() */

	SPI_NCS10131_Init();
}

uint8_t NCS10131_DRV_Reset_device (void)
{
	status_t spi_status;
	uint8_t u8Status;
	uint8_t u8COUNT;
	uint8_t u8DEVSTAT = 0;

	/* Send the three commands consecutively, with a short delay between them to let time to
	 * the register to be written */
	spi_status = SPI_NCS10131_tx_request(NCS10131_RST_CMD_1);
	spi_status |= SPI_NCS10131_tx_request(NCS10131_RST_CMD_2);
	spi_status |= SPI_NCS10131_tx_request(NCS10131_RST_CMD_3);

	if (kStatus_Success != spi_status)
	{
		return STATUS_SPI_ERROR;
	}

	/* Wait for SPI to be ready again */
	WAIT_AML_WaitMs(3);

	/* Check that the Reset bit is set */
	u8Status = NCS10131_DRV_Read_register(NCS10131_COUNT, &u8DEVSTAT, &u8COUNT);
	if ((STATUS_OK == u8Status) && (DEVSTAT_DEVRES_SET == (u8DEVSTAT & DEVSTAT_DEVRES_SET)))
	{
		u8Status = STATUS_OK;
	}
	else
	{
		u8Status = STATUS_ERROR;
	}

	return u8Status;
}

uint8_t NCS10131_DRV_Cfg (void)
{
	uint8_t u8Status;

	/* Configure ARC 250 dps */
	u8Status = NCS10131_DRV_Write_register(NCS10131_ARC0_CFG_U3, 0x24);

	/* Configure all three accelerometer axes to +/-6g range.
	 * ACC_CFG_U1: SNS_SHIFT = 0x1 (shift factor 0.5).
	 * ACC_CFG_U2: SNS_MULT = 0xFF.
	 * ACC_CFG_U3: DATATYPE = 0b001 (no offset cancellation).
	 * Keep default sample time (64us) and LPF0. */
	u8Status |= NCS10131_DRV_Write_register(NCS10131_ACC0_CFG_U1, 0x01);
	u8Status |= NCS10131_DRV_Write_register(NCS10131_ACC1_CFG_U1, 0x01);
	u8Status |= NCS10131_DRV_Write_register(NCS10131_ACC2_CFG_U1, 0x01);
	u8Status |= NCS10131_DRV_Write_register(NCS10131_ACC0_CFG_U2, 0xFF);
	u8Status |= NCS10131_DRV_Write_register(NCS10131_ACC1_CFG_U2, 0xFF);
	u8Status |= NCS10131_DRV_Write_register(NCS10131_ACC2_CFG_U2, 0xFF);
	u8Status |= NCS10131_DRV_Write_register(NCS10131_ACC0_CFG_U3, 0x24);
	u8Status |= NCS10131_DRV_Write_register(NCS10131_ACC1_CFG_U3, 0x24);
	u8Status |= NCS10131_DRV_Write_register(NCS10131_ACC2_CFG_U3, 0x24);

	/* Configure and enable the SID */
	u8Status |= NCS10131_DRV_Write_register(NCS10131_SID_ARC_0, (ARC_0_SID | SID_ENABLE_SET));
	u8Status |= NCS10131_DRV_Write_register(NCS10131_SID_ACC0_0, (ACC0_0_SID | SID_ENABLE_SET));
	u8Status |= NCS10131_DRV_Write_register(NCS10131_SID_ACC1_0, (ACC1_0_SID | SID_ENABLE_SET));
	u8Status |= NCS10131_DRV_Write_register(NCS10131_SID_ACC2_0, (ACC2_0_SID | SID_ENABLE_SET));

	return u8Status;
}

uint8_t NCS10131_DRV_Lock_config (void)
{
	uint8_t u8Status;

	/* Note that the only other field in DEVLOCK register is RESET[1:0], and writing to these bits once has no effect */
	u8Status = NCS10131_DRV_Write_register(NCS10131_DEVLOCK, DEVLOCK_ENDINIT_SET);

	return u8Status;
}

uint8_t NCS10131_DRV_Read_COUNT_DEVSTAT (uint8_t *pu8Cnt, uint8_t *pu8Devstat, uint8_t *pu8Devstat1, uint8_t *pu8Devstat2, uint8_t *pu8Devstat3)
{
	uint8_t u8Status;
	uint8_t u8Dummy;

	if ((NULL == pu8Cnt) || (NULL == pu8Devstat) || (NULL == pu8Devstat1) || (NULL == pu8Devstat2) || (NULL == pu8Devstat3))
	{
		return STATUS_PARAM_ERROR;
	}

	u8Status = NCS10131_DRV_Read_register(NCS10131_COUNT, pu8Devstat, pu8Cnt);
	u8Status |= NCS10131_DRV_Read_register(NCS10131_DEVSTAT1, pu8Devstat2, pu8Devstat1);
	u8Status |= NCS10131_DRV_Read_register(NCS10131_DEVSTAT3, &u8Dummy, pu8Devstat3);

	return u8Status;
}

uint8_t NCS10131_DRV_Read_TEMPERATURE (int16_t *pi16Temp)
{
	uint8_t u8Status;
	uint8_t u8TempHigh;
	uint8_t u8TempLow;

	if (NULL == pi16Temp)
	{
		return STATUS_PARAM_ERROR;
	}

	u8Status = NCS10131_DRV_Read_register(NCS10131_TEMPERATURE_L, &u8TempHigh, &u8TempLow);

	if (0 == u8Status)
	{
		*pi16Temp = (int16_t)((u8TempHigh*256) + u8TempLow);
	}

	return u8Status;
}

uint8_t NCS10131_DRV_Read_sens (uint8_t u8Sid, int16_t *pi16Data, uint8_t *pu8SensStatus)
{
	uint8_t u8Status;
	uint8_t u8HasData;

	if ((NULL == pi16Data) || (NULL == pu8SensStatus))
	{
		return STATUS_PARAM_ERROR;
	}

	u8Status = NCS10131_DRV_Read_sensor_data(u8Sid, pi16Data, &u8HasData, pu8SensStatus);

	if (0 != u8Status)
	{
		return STATUS_ERROR;
	}

	if (u8HasData != RSP_RS_HAS_DATA)
	{
		return STATUS_NO_SENSOR_DATA;
	}

	return STATUS_OK;
}

/*** Read Register ****************************************************************************/

uint8_t NCS10131_DRV_Read_register (uint8_t u8RegAddr, uint8_t *pu8ValHigh, uint8_t *pu8ValLow)
{
	status_t spi_status;
	uint8_t u8Status;
	uint32_t u32ReadCmd;
	uint32_t u32ReadRsp;

	if ((NULL == pu8ValHigh) || (NULL == pu8ValLow))
	{
		return STATUS_PARAM_ERROR;
	}

	u32ReadCmd = NCS10131_DRV_Prepare_read_register_request(u8RegAddr);
	spi_status = SPI_NCS10131_tx_request_rx_response(u32ReadCmd, &u32ReadRsp);

	if (kStatus_Success != spi_status)
	{
		return STATUS_SPI_ERROR;
	}

	u8Status = NCS10131_DRV_Check_read_register_response(u32ReadRsp, pu8ValHigh, pu8ValLow);

	return u8Status;
}

/*  Read Reg Cmd
 *  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
 *
 * | SA  |1  1  0  0 |0  0  0  0  0  0 | RA[7:1]            |x |0  0  0  0  0  0  0  0  0 | CRC    |
 */
uint32_t NCS10131_DRV_Prepare_read_register_request (uint8_t u8RegAddr)
{
	uint32_t u32TxData = 0;

	/* Insert "SPI address" value into the tx frame */
	u32TxData |= ((NCS10131_SPI_ADDR << CMD_SA_ADDR_SHIFT) & CMD_SA_ADDR_MASK);
	/* Insert "Command" value into the tx frame */
	u32TxData |= ((READ_REG_CMD << CMD_RW_REG_CMD_SHIFT) & CMD_RW_REG_CMD_MASK);
	/* Insert "Register Address" value into the tx frame */
	u32TxData |= ((u8RegAddr << CMD_RW_REG_RA_SHIFT) & CMD_RW_REG_RA_MASK);

	/* Calculate the CRC and add it to the frame */
	u32TxData |= ((u8Crc_Calculate(crc_out_of_frame_param, u32TxData)) & CRC_MASK);

	return u32TxData;
}

/* Read Reg Resp
 *  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
 *
 * |0 | SA  |1  1  0  0 |0  0  0  0  0 | RD[15:8]              | RD[7:0] 0  0  0  0  0 |1 | CRC    |
 */
uint8_t NCS10131_DRV_Check_read_register_response (uint32_t u32Response, uint8_t *pu8RegDataHigh, uint8_t *pu8RegDataLow)
{
	uint8_t u8Val;
	bool bCrcOk;

	/* Check that parameters are ok first */
	if ((NULL == pu8RegDataHigh) || (NULL == pu8RegDataLow))
	{
		return STATUS_PARAM_ERROR;
	}

	/* Check that Sensor Address has the expected value */
	u8Val = (uint8_t)((u32Response >> RSP_SA_ADDR_SHIFT) & 0x03);
	if (NCS10131_SPI_ADDR != u8Val)
	{
		return STATUS_ERROR;
	}

	/* Check that Command has the expected value */
	u8Val = (uint8_t)((u32Response >> RSP_RW_REG_CMD_SHIFT) & 0x0F);
	if (READ_REG_CMD != u8Val)
	{
		return STATUS_ERROR;
	}

	/* Check that CRC is correct */
	bCrcOk = bCrc_Check((u32Response & CRC_MASK), crc_out_of_frame_param, (u32Response & ~CRC_MASK));
	if (false == bCrcOk)
	{
		return STATUS_ERROR;
	}

	/* Finally, if all is correct, extract data */
	*pu8RegDataHigh = (uint8_t)(u32Response >> RSP_RW_REG_RD_H_SHIFT);
	*pu8RegDataLow = (uint8_t)(u32Response >> RSP_RW_REG_RD_L_SHIFT);

	return STATUS_OK;
}

/*** Write Register ****************************************************************************/

uint8_t NCS10131_DRV_Write_register (uint8_t u8RegAddr, uint8_t u8Val)
{
	status_t spi_status;
	uint8_t u8Status;
	uint32_t u32WriteCmd;
	uint32_t u32ReadRsp;
	uint8_t u8ValHigh;
	uint8_t u8ValLow;

	u32WriteCmd = NCS10131_DRV_Prepare_write_register_request(u8RegAddr, u8Val);
	spi_status = SPI_NCS10131_tx_request(u32WriteCmd);
	/* Delay a bit before reading the response, to let time to the register to be written */
	spi_status |= SPI_NCS10131_rx_response(&u32ReadRsp);

	if (kStatus_Success != spi_status)
	{
		return STATUS_SPI_ERROR;
	}

	u8Status = NCS10131_DRV_Check_write_register_response(u32ReadRsp, &u8ValHigh, &u8ValLow);

	/* Check that the value was correctly written.
	 * If address is even, then we wrote at lower address. If odd, at higher address. */
	if (((STATUS_OK == u8Status) && (0 == u8RegAddr % 2) && (u8ValLow == u8Val))
			|| ((STATUS_OK == u8Status) && (0 != u8RegAddr % 2) && (u8ValHigh == u8Val)))
	{
		u8Status = STATUS_OK;
	}
	else
	{
		u8Status = STATUS_ERROR;
	}

	return u8Status;
}

/*  Write Reg Cmd
 *  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
 *
 * | SA  |1  0  0  0 |0  0  0  0  0  0 | RA[7:0]               | RD[7:0]               |0 | CRC    |
 */
uint32_t NCS10131_DRV_Prepare_write_register_request (uint8_t u8RegAddr, uint8_t u8Val)
{
	uint32_t u32TxData = 0;

	/* Insert "SPI address" value into the tx frame */
	u32TxData |= ((NCS10131_SPI_ADDR << CMD_SA_ADDR_SHIFT) & CMD_SA_ADDR_MASK);
	/* Insert "Command" value into the tx frame */
	u32TxData |= ((WRITE_REG_CMD << CMD_RW_REG_CMD_SHIFT) & CMD_RW_REG_CMD_MASK);
	/* Insert "Register Address" value into the tx frame */
	u32TxData |= ((u8RegAddr << CMD_RW_REG_RA_SHIFT) & CMD_RW_REG_RA_MASK);
	/* Insert "Register Value" value into the tx frame */
	u32TxData |= ((u8Val << CMD_W_REG_RD_SHIFT) & CMD_W_REG_RD_MASK);

	/* Calculate the CRC and add it to the frame */
	u32TxData |= ((u8Crc_Calculate(crc_out_of_frame_param, u32TxData)) & CRC_MASK);

	return u32TxData;
}

/* Write Reg Resp
 *  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
 *
 * |0 | SA  |1  0  0  0 |0  0  0  0  0 | RD[15:8]              | RD[7:0]               |1 | CRC    |
 */
uint8_t NCS10131_DRV_Check_write_register_response (uint32_t u32Response, uint8_t *pu8ValHigh, uint8_t *pu8ValLow)
{
	uint8_t u8Val;
	bool bCrcOk;

	/* Check that parameters are ok first */
	if ((NULL == pu8ValHigh) || (NULL == pu8ValLow))
	{
		return STATUS_PARAM_ERROR;
	}

	/* Check that Sensor Address has the expected value */
	u8Val = (uint8_t)((u32Response >> RSP_SA_ADDR_SHIFT) & 0x03);
	if (NCS10131_SPI_ADDR != u8Val)
	{
		return STATUS_ERROR;
	}

	/* Check that Command has the expected value */
	u8Val = (uint8_t)((u32Response >> RSP_RW_REG_CMD_SHIFT) & 0x0F);
	if (WRITE_REG_CMD != u8Val)
	{
		return STATUS_ERROR;
	}

	/* Check that CRC is correct */
	bCrcOk = bCrc_Check((u32Response & CRC_MASK), crc_out_of_frame_param, (u32Response & ~CRC_MASK));
	if (false == bCrcOk)
	{
		return STATUS_ERROR;
	}

	/* Finally, if all is correct, extract data */
	*pu8ValHigh = (uint8_t)(u32Response >> RSP_RW_REG_RD_H_SHIFT);
	*pu8ValLow = (uint8_t)(u32Response >> RSP_RW_REG_RD_L_SHIFT);

	return STATUS_OK;
}

/*** Read Register ****************************************************************************/

uint8_t NCS10131_DRV_Read_sensor_data (uint8_t u8SensorId, int16_t *pi16Data, uint8_t *pu8HasData, uint8_t *pu8SenStatus)
{
	status_t spi_status;
	uint8_t u8Status;
	uint32_t u32ReadCmd;
	uint32_t u32ReadRsp;
	uint8_t u8SensId3bitReceived;

	if ((NULL == pi16Data) || (NULL == pu8HasData) || (NULL == pu8SenStatus))
	{
		return STATUS_PARAM_ERROR;
	}

	u32ReadCmd = NCS10131_DRV_Prepare_read_sensor_request(u8SensorId);
	spi_status = SPI_NCS10131_tx_request_rx_response(u32ReadCmd, &u32ReadRsp);

	if (kStatus_Success != spi_status)
	{
		return STATUS_SPI_ERROR;
	}

	u8Status = NCS10131_DRV_Check_read_sensor_response(u32ReadRsp, &u8SensId3bitReceived, pu8HasData, pi16Data, pu8SenStatus);

	/* u8SensId3bitReceived was written if status is OK */
	if ((STATUS_OK == u8Status) && (u8SensId3bitReceived != (u8SensorId & 0x07)))
	{
		return STATUS_ERROR;
	}

	return u8Status;
}

/*  Read Sensor Cmd
 *  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
 *
 * | SA  |SID[3:1]|1 |0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 | CRC    |
 */
uint32_t NCS10131_DRV_Prepare_read_sensor_request (uint8_t u8SensorId)
{
	uint32_t u32TxData = 0;
	uint8_t u8SensorId3bit;

	/* Insert "SPI address" value into the tx frame */
	u32TxData |= ((NCS10131_SPI_ADDR << CMD_SA_ADDR_SHIFT) & CMD_SA_ADDR_MASK);
	/* Insert 3-bit "Sensor ID"  (SOURCEID[2:0]) value into the tx frame */
	u8SensorId3bit = u8SensorId & 0x07;
	u32TxData |= ((u8SensorId3bit << CMD_RS_SID_SHIFT) & CMD_RS_SID_MASK);
	/* Set BIT 26 */
	u32TxData |= 0x04000000;

	/* Calculate the CRC and add it to the frame */
	u32TxData |= ((u8Crc_Calculate(crc_out_of_frame_param, u32TxData)) & CRC_MASK);

	return u32TxData;
}

/* Read Sensor Resp
 *  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
 *
 * |D | SA  |SID[3:1]|1 |0  0  0  0  S1| SD [15:0]                                     |S0| CRC    |
 * D = 1 when sensor data is included.
 * S1 S0 detailed in the datasheet.
 */
uint8_t NCS10131_DRV_Check_read_sensor_response (uint32_t u32Response, uint8_t *pu8SensorId3bit, uint8_t *pu8HasData, int16_t *pi16Data, uint8_t *pu8SensStatus)
{
	uint8_t u8Val;
	bool bCrcOk;

	/* Check that parameters are ok first */
	if ((NULL == pu8SensorId3bit) || (NULL == pu8HasData) || (NULL == pi16Data) || (NULL == pu8SensStatus))
	{
		return STATUS_PARAM_ERROR;
	}

	/* Check that Sensor Address has the expected value */
	u8Val = (uint8_t)((u32Response >> RSP_SA_ADDR_SHIFT) & 0x03);
	if (NCS10131_SPI_ADDR != u8Val)
	{
		return STATUS_ERROR;
	}

	/* Check that CRC is correct */
	bCrcOk = bCrc_Check((u32Response & CRC_MASK), crc_out_of_frame_param, (u32Response & ~CRC_MASK));
	if (false == bCrcOk)
	{
		return STATUS_ERROR;
	}

	/* Finally, if all is correct, check if data is available and extract it */
	*pu8SensorId3bit = (uint8_t)(u32Response >> RSP_RS_SID_SHIFT) & 0x07;
	*pu8SensStatus = ((uint8_t)(u32Response >> (RSP_RS_S1_SHIFT - 1))) & 0x02;
	*pu8SensStatus |= ((uint8_t)(u32Response >> RSP_RS_S0_SHIFT)) & 0x01;
	*pu8HasData = (uint8_t)(u32Response >> RSP_RS_HAS_DATA_SHIFT) & 0x01;

	if (RSP_RS_HAS_DATA == *pu8HasData)
	{
		*pi16Data = (int16_t)(u32Response >> RSP_RS_SD_SHIFT);
	}

	return STATUS_OK;
}


