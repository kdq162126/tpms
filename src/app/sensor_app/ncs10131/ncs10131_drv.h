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

#ifndef NCS10131_NCS10131_DRV_H_
#define NCS10131_NCS10131_DRV_H_


/* Status returned by the functions */
#define STATUS_OK						(0x00)
#define STATUS_ERROR					(0x01)
#define STATUS_NO_SENSOR_DATA			(0x02)
#define STATUS_PARAM_ERROR				(0x03)
#define STATUS_SPI_ERROR				(0x04)

/* Sensor IDs */
#define ARC_0_SID						(0x01)
#define ACC0_0_SID						(0x02)
#define ACC1_0_SID						(0x03)
#define ACC2_0_SID						(0x04)


void NCS10131_DRV_Init (void);
uint8_t NCS10131_DRV_Reset_device(void);
uint8_t NCS10131_DRV_Cfg (void);
uint8_t NCS10131_DRV_Lock_config (void);

uint8_t NCS10131_DRV_Read_COUNT_DEVSTAT (uint8_t *pu8Cnt, uint8_t *pu8Devstat, uint8_t *pu8Devstat1, uint8_t *pu8Devstat2, uint8_t *pu8Devstat3);
uint8_t NCS10131_DRV_Read_TEMPERATURE (int16_t *pi16Temp);
uint8_t NCS10131_DRV_Read_sens (uint8_t u8Sid, int16_t *pi16Data, uint8_t *pu8SensStatus);

#endif /* NCS10131_NCS10131_DRV_H_ */
