/*
   (c) NXP B.V. 2018, 2020, 2022. All rights reserved.

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
#include "ncs10131/ncs10131_crc.h"



uint8_t u8Crc_Calculate(tCrcParameters crc_calc_param, uint32_t u32_cmd_msg)
{
	uint16_t u16_index;
	/* Calculate mask value */
	uint64_t u64_mask = ((~(uint64_t)0) >> (32 - crc_calc_param.u8DataLength)) - ((uint64_t)(((uint64_t)1) << crc_calc_param.u8CrcLength) - 1);
	/* Update the command message using CRC params */
	u32_cmd_msg = (u32_cmd_msg >> crc_calc_param.u8DataBitStart) & ((uint32_t)(((uint64_t)1) << crc_calc_param.u8DataLength) - 1);
	/* Calculate remainder and XOR value */
	uint64_t u64_remainder = (uint64_t)(((((uint64_t)u32_cmd_msg)) | (((uint64_t)crc_calc_param.u8InitialValue) << (crc_calc_param.u8DataLength))) & u64_mask);
	 uint64_t u64_xor = ((uint64_t)crc_calc_param.u16Polynomial) << (crc_calc_param.u8DataLength - 1) ;
	/* Calculate remainder by shifting xor value over message
	 and CRC length */
	for (u16_index = (uint16_t) (((uint16_t)crc_calc_param.u8DataLength) + ((uint16_t)crc_calc_param.u8CrcLength)); u16_index > 0; u16_index--)
	{
		 if (((u64_remainder >> (u16_index - 1)) & 0x01) != 0)
		 {
			 u64_remainder ^= u64_xor;
		 }

		 	 u64_xor >>= 1;

		 if ((u64_remainder & u64_mask) == 0)
		 {
			 break;
		 }
	}

	/* Return 8-bit CRC calculated */
	return (uint8_t)(u64_remainder & (uint8_t)((uint64_t)(((uint64_t)1) << crc_calc_param.u8CrcLength) - 1));
}

bool bCrc_Check(uint8_t u8_crc, tCrcParameters crc_calc_param, uint32_t u32_response)
{
	/* Check whether calculated 8-bit CRC on response message
	 matches with u8_crc */
	return u8_crc == u8Crc_Calculate(crc_calc_param, u32_response);
}


