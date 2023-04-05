/*
   (c) NXP B.V. 2021-2022. All rights reserved.

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

#include "device_registers.h"	/* include peripheral declarations S32K144 */
#include "utils/nxp_console_adapter.h"
#include "utils/nxp_console.h"


#define CHECKSUM_MATCH				(0)
#define CHECKSUM_DIFFER				(1)

/* In the buffer received via SPI, TPMS data starts at buffer[9] */
#define RF_TPMS_DATA_START_INDEX	(9)

/* Code violation byte is included in the RF data received */
#define LENGTH_CODE_VIOLATION		(1) // One-byte long

/* TPMS DATA starting at RF_Buffer[RF_TPMS_DATA_START_INDEX]
 * PAYLOAD_LENGTH	ID_0 	ID_1 	ID_2	ID_3	CODE_F	CODE_H	P_MSB	P_LSB	Z_MSB	Z_LSB
 * 	X_MSB	X_LSB	Volt	Temp	Status_Read	 Status_Comp	Cnt_MSB		Cnt_LSB		CRC_MSB		CRC_LSB
 * Note: the value in PAYLOAD_LENGTH covers bytes from ID_0 to Cnt_LSB, so total number of TPMS data bytes is
 * equal to (PAYLOAD_LENGTH + LENGTH_PAYLOAD_LENGTH + LENGTH_CRC)
 */
#define LENGTH_PAYLOAD_LENGTH		(1) // Payload length is one-byte long
#define LENGTH_CRC					(2) // CRC is two-byte long

#define TPMS_DATA_LENGTH_IND		(RF_TPMS_DATA_START_INDEX + 0)
#define TPMS_DATA_ID_IND			(RF_TPMS_DATA_START_INDEX + 1)
#define TPMS_DATA_CODEF_IND			(RF_TPMS_DATA_START_INDEX + 5)
#define TPMS_DATA_CODEH_IND			(RF_TPMS_DATA_START_INDEX + 6)
#define TPMS_DATA_P_IND				(RF_TPMS_DATA_START_INDEX + 7)
#define TPMS_DATA_Z_IND				(RF_TPMS_DATA_START_INDEX + 9)
#define TPMS_DATA_X_IND				(RF_TPMS_DATA_START_INDEX + 11)
#define TPMS_DATA_V_IND				(RF_TPMS_DATA_START_INDEX + 13)
#define TPMS_DATA_T_IND				(RF_TPMS_DATA_START_INDEX + 14)
#define TPMS_DATA_STATUS_R_IND		(RF_TPMS_DATA_START_INDEX + 15)
#define TPMS_DATA_STATUS_C_IND		(RF_TPMS_DATA_START_INDEX + 16)
#define TPMS_DATA_CNT_IND			(RF_TPMS_DATA_START_INDEX + 17)
#define TPMS_DATA_CHECKSUM_IND		(RF_TPMS_DATA_START_INDEX + 19)


static uint8_t RF_CheckTpmsChecksum (uint8_t *pau8RfFrame, uint8_t u8NbBytesForChecksum, uint16_t u16ChecksumValue);
static uint16_t RF_ComputeCrc(uint16_t crc, uint8_t dataByte, uint16_t polynomial);


/************************************************************************************
 * Description: checks if the data included in the SPI frame is from the TPMS, and
 * 				if so, verifies that the checksum is correct before displaying the
 * 				content of the data
 * Input: pau8RfFrame, pointer to the SPI data received
 * Output: void
 ************************************************************************************/
void RF_ParseTpmsData (uint8_t *pau8RfFrame)
{
	uint8_t u8PayloadLength;
	uint16_t u16Cheksum;
	uint8_t u8FrameFormatOk;
	uint8_t u8NbSpiBytesReceived;

	/* Assume not ok and update later */
	u8FrameFormatOk = 0;

	u8NbSpiBytesReceived = pau8RfFrame[0];

	/* Check if the frame format is the expected one for a TPMS frame */
	if (u8NbSpiBytesReceived > TPMS_DATA_LENGTH_IND)
	{
		u8PayloadLength = pau8RfFrame[TPMS_DATA_LENGTH_IND];
		if (u8NbSpiBytesReceived == u8PayloadLength + LENGTH_PAYLOAD_LENGTH + LENGTH_CRC + LENGTH_CODE_VIOLATION + TPMS_DATA_LENGTH_IND)
		{
			u16Cheksum = (uint16_t)((256 * pau8RfFrame[TPMS_DATA_CHECKSUM_IND]) + pau8RfFrame[TPMS_DATA_CHECKSUM_IND+1]);
			if (CHECKSUM_MATCH == RF_CheckTpmsChecksum(&pau8RfFrame[RF_TPMS_DATA_START_INDEX], u8PayloadLength+1, u16Cheksum))
			{
				u8FrameFormatOk = 1;

				PRINTF("\n\rTPMS data checksum:OK ");
				/* Extract content */
				PRINTF("UniqueID=0x%02X%02X%02X%02X ",pau8RfFrame[TPMS_DATA_ID_IND], pau8RfFrame[TPMS_DATA_ID_IND+1],
						pau8RfFrame[TPMS_DATA_ID_IND+2], pau8RfFrame[TPMS_DATA_ID_IND+3]);
				PRINTF("CODEF=0x%02X ",pau8RfFrame[TPMS_DATA_CODEF_IND]);
				PRINTF("CODEH=0x%02X ",pau8RfFrame[TPMS_DATA_CODEH_IND]);
				PRINTF("P=0x%02X%02X ",pau8RfFrame[TPMS_DATA_P_IND], pau8RfFrame[TPMS_DATA_P_IND+1]);
				PRINTF("Z=0x%02X%02X ",pau8RfFrame[TPMS_DATA_Z_IND], pau8RfFrame[TPMS_DATA_Z_IND+1]);
				PRINTF("X=0x%02X%02X ",pau8RfFrame[TPMS_DATA_X_IND], pau8RfFrame[TPMS_DATA_X_IND+1]);
				PRINTF("V=0x%02X ",pau8RfFrame[TPMS_DATA_V_IND]);
				PRINTF("T=0x%02X ",pau8RfFrame[TPMS_DATA_T_IND]);
				PRINTF("StatusRead=0x%02X ",pau8RfFrame[TPMS_DATA_STATUS_R_IND]);
				PRINTF("StatusComp=0x%02X ",pau8RfFrame[TPMS_DATA_STATUS_C_IND]);
				PRINTF("Counter=0x%02X%02X \n\r",pau8RfFrame[TPMS_DATA_CNT_IND], pau8RfFrame[TPMS_DATA_CNT_IND+1]);
			}
		}
	}

	/* If format is not correct, inform the user */
	if (0 == u8FrameFormatOk)
	{
		PRINTF("\n\rData is not from the TPMS, or checksum of TPMS data is not correct \n\r");
	}

}

/************************************************************************************
 * Description: verifies if the CRC is correct or not
 * Input: pau8TpmsData, pointer to the TPMS data received
 * 		  u8NbBytesForChecksum, number of bytes to be included in the checksum calculation
 * 		  u16ChecksumValue, checksum value included in the frame received
 * Output: CHECKSUM_MATCH is the checksum is correct, CHECKSUM_DIFFER otherwise
 ************************************************************************************/
static uint8_t RF_CheckTpmsChecksum (uint8_t *pau8TpmsData, uint8_t u8NbBytesForChecksum, uint16_t u16ChecksumValue)
{
	uint8_t u8Match;
	uint8_t i;
	uint16_t u16Crc;
	uint16_t u16Polynomial;

	/* Calculate checksum */
	u16Polynomial = 0x1021;
    u16Crc = 0x1D0F;
	for(i = 0; i < u8NbBytesForChecksum; i++)
	{
		u16Crc = RF_ComputeCrc(u16Crc, pau8TpmsData[i], u16Polynomial);
	}

	u16Crc = (uint16_t)(~u16Crc);

	/* Compare with received value */
	if (u16Crc == u16ChecksumValue)
		u8Match = CHECKSUM_MATCH;
	else
		u8Match = CHECKSUM_DIFFER;

	return u8Match;
}

/************************************************************************************
 * Description: computes the CRC
 * Input: u16Crc, current CRC to be updated
 * 		  u8DataByte, byte to include in the CRC calculation
 * 		  u16Polynomial, polynomial to use for the CRC calculation
 * Output: the 16-bit updated CRC
 ************************************************************************************/
static uint16_t RF_ComputeCrc(uint16_t u16Crc, uint8_t u8DataByte, uint16_t u16Polynomial)
{
	uint8_t i;

    for(i = 0; i < 8; i++)
    {
      if((((u16Crc & 0x8000) >> 8) ^ (u8DataByte & 0x80)) != 0)
      {
    	  u16Crc <<= 1; // shift left once
    	  u16Crc ^= u16Polynomial; // XOR with polynomial
      }
    else
    {
    	u16Crc <<= 1; // shift left once
    }

      u8DataByte <<= 1; // Next data bit
  }
  return u16Crc;
}


