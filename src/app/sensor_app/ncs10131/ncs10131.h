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

#ifndef NCS10131_NCS10131_H_
#define NCS10131_NCS10131_H_

/**
 **
 **  NCS10131 Sensor Internal Registers
 */
enum {
	NCS10131_COUNT	                = 0x00, // R
	NCS10131_DEVSTAT	            = 0x01, // R
	NCS10131_DEVSTAT1               = 0x02, // R
	NCS10131_DEVSTAT2               = 0x03, // R
	NCS10131_DEVSTAT3               = 0x04, // R

	NCS10131_TEMPERATURE_L          = 0x0E, // R
	NCS10131_TEMPERATURE_H          = 0x0F, // R
	NCS10131_DEVLOCK                = 0x10, // RW

	NCS10131_UF_REGION_W            = 0x14, // RW
	NCS10131_UF_REGION              = 0x15, // R

	NCS10131_SID_ARC_0              = 0x1A, // RW
	NCS10131_SID_ARC_1              = 0x1B, // RW

	NCS10131_SID_ACC0_0             = 0x20, // RW
	NCS10131_SID_ACC0_1             = 0x21, // RW
	NCS10131_SID_ACC1_0             = 0x22, // RW
	NCS10131_SID_ACC1_1             = 0x23, // RW
	NCS10131_SID_ACC2_0             = 0x24, // RW
	NCS10131_SID_ACC2_1             = 0x25, // RW

	NCS10131_SPI_SADDR              = 0x3E, // RW

	NCS10131_ARC0_CFG_U1            = 0x40, // RW

	NCS10131_ARC0_CFG_U3            = 0x42, // RW
	NCS10131_ARC0_CFG_U4            = 0x43, // RW
	NCS10131_ARC0_CFG_U5            = 0x44, // RW
	NCS10131_ARC0_STADJ             = 0x45, // RW
	NCS10131_ARC0_RATEADJ_L         = 0x46, // RW
	NCS10131_ARC0_RATEADJ_H         = 0x47, // RW

	NCS10131_ACC0_CFG_U1            = 0x58, // RW
	NCS10131_ACC0_CFG_U2            = 0x59, // RW
	NCS10131_ACC0_CFG_U3            = 0x5A, // RW
	NCS10131_ACC0_CFG_U4            = 0x5B, // RW
	NCS10131_ACC0_CFG_U5            = 0x5C, // RW

	NCS10131_ACC0_U_OFF_L           = 0x5E, // RW
	NCS10131_ACC0_U_OFF_H           = 0x5F, // RW
	NCS10131_ACC1_CFG_U1            = 0x60, // RW
	NCS10131_ACC1_CFG_U2            = 0x61, // RW
	NCS10131_ACC1_CFG_U3            = 0x62, // RW
	NCS10131_ACC1_CFG_U4            = 0x63, // RW
	NCS10131_ACC1_CFG_U5            = 0x64, // RW

	NCS10131_ACC1_U_OFF_L           = 0x66, // RW
	NCS10131_ACC1_U_OFF_H           = 0x67, // RW
	NCS10131_ACC2_CFG_U1            = 0x68, // RW
	NCS10131_ACC2_CFG_U2            = 0x69, // RW
	NCS10131_ACC2_CFG_U3            = 0x6A, // RW
	NCS10131_ACC2_CFG_U4            = 0x6B, // RW
	NCS10131_ACC2_CFG_U5            = 0x6C, // RW

	NCS10131_ACC2_U_OFF_L           = 0x6E, // RW
	NCS10131_ACC2_U_OFF_H           = 0x6F, // RW
	NCS10131_ARC0_STAT              = 0x70, // R
	NCS10131_DEVSTAT_COPY           = 0x71, // R
	NCS10131_ARC0_SNSDATA0_L        = 0x72, // R
	NCS10131_ARC0_SNSDATA0_H        = 0x73, // R
	NCS10131_ARC0_SNSDATA1_L        = 0x74, // R
	NCS10131_ARC0_SNSDATA1_H        = 0x75, // R
	NCS10131_ARC0_ST_L              = 0x76, // R
	NCS10131_ARC0_ST_H              = 0x77, // R

	NCS10131_ACC0_STAT              = 0x88, // R
	NCS10131_DEVSTAT_COPY1          = 0x89, // R
	NCS10131_ACC0_SNSDATA0_L        = 0x8A, // R
	NCS10131_ACC0_SNSDATA0_H        = 0x8B, // R
	NCS10131_ACC0_SNSDATA1_L        = 0x8C, // R
	NCS10131_ACC0_SNSDATA1_H        = 0x8D, // R

	NCS10131_ACC1_STAT              = 0x90, // R
	NCS10131_DEVSTAT_COPY2          = 0x91, // R
	NCS10131_ACC1_SNSDATA0_L        = 0x92, // R
	NCS10131_ACC1_SNSDATA0_H        = 0x93, // R
	NCS10131_ACC1_SNSDATA1_L        = 0x94, // R
	NCS10131_ACC1_SNSDATA1_H        = 0x95, // R

	NCS10131_ACC2_STAT              = 0x98, // R
	NCS10131_DEVSTAT_COPY3          = 0x99, // R
	NCS10131_ACC2_SNSDATA0_L        = 0x9A, // R
	NCS10131_ACC2_SNSDATA0_H        = 0x9B, // R
	NCS10131_ACC2_SNSDATA1_L        = 0x9C, // R
	NCS10131_ACC2_SNSDATA1_H        = 0x9D, // R

	NCS10131_SN0	                = 0xA1, // F, R
	NCS10131_SN1	                = 0xA2, // F, R
	NCS10131_SN2	                = 0xA3, // F, R
	NCS10131_SN3	                = 0xA4, // F, R
	NCS10131_SN4	                = 0xA5, // F, R
	NCS10131_ACC0_STH_P_L           = 0xA6, // F, R
	NCS10131_ACC0_STH_P_H           = 0xA7, // F, R
	NCS10131_ACC0_STL_N_L           = 0xA8, // F, R
	NCS10131_ACC0_STL_N_H           = 0xA9, // F, R
	NCS10131_ACC1_STH_P_L           = 0xAA, // F, R
	NCS10131_ACC1_STH_P_H           = 0xAB, // F, R
	NCS10131_ACC1_STL_N_L           = 0xAC, // F, R
	NCS10131_ACC1_STL_N_H           = 0xAD, // F, R
	NCS10131_ACC2_STH_P_L           = 0xAE, // F, R
	NCS10131_ACC2_STH_P_H           = 0xAF, // F, R
	NCS10131_ACC2_STL_N_L           = 0xB0, // F, R
	NCS10131_ACC2_STL_N_H           = 0xB1, // F, R
	NCS10131_ARC_ST_DELTA_L         = 0xB2, // F, R
	NCS10131_ARC_ST_DELTA_H         = 0xB3, // F, R
	NCS10131_PN0                    = 0xB4, // F, R
	NCS10131_PN1                    = 0xB5, // F, R
};

/* Bit fields used in the application */
#define DEVSTAT_DEVRES_SET			(0x04)
#define SID_ENABLE_SET				(0x80)
#define DEVLOCK_ENDINIT_SET			(0x80)

/* Commands to trigger a reset */
#define NCS10131_RST_CMD_1		(0x20010005)
#define NCS10131_RST_CMD_2		(0x20010034)
#define NCS10131_RST_CMD_3		(0x20010013)


/* SPI FORMAT */

/*  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
 *
 * Read Reg Cmd
 * | SA  |1  1  0  0 |0  0  0  0  0  0 | RA[7:1]            |x |0  0  0  0  0  0  0  0  0 | CRC    |
 *
 * Write Reg Cmd
 * | SA  |1  0  0  0 |0  0  0  0  0  0 | RA[7:0]               | RD[7:0]               |0 | CRC    |
 *
 * Read Sensor Cmd
 * | SA  |SID[3:1]|1 |0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 | CRC    |
 *
 *  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
 *
 * Read Reg Resp
 * |0 | SA  |1  1  0  0 |0  0  0  0  0 | RD[15:8]              | RD[7:0] 0  0  0  0  0 |1 | CRC    |
 *
 * Write Reg Resp
 * |0 | SA  |1  0  0  0 |0  0  0  0  0 | RD[15:8]              | RD[7:0] 0  0  0  0  0 |1 | CRC    |
 *
 * Read Sensor Resp
 * |D | SA  |SID[3:1]|1 |0  0  0  0  S1| SD [15:0]                                     |S0| CRC    |
 * D = 1 when sensor data is included.
 * S1 S0 detailed in the datasheet.
 */

#define CMD_SA_ADDR_SHIFT		(30)
#define CMD_SA_ADDR_MASK		(0xC0000000)

#define CMD_RW_REG_CMD_SHIFT	(26)
#define CMD_RW_REG_CMD_MASK		(0x3C000000)

#define CMD_RW_REG_RA_SHIFT		(12)
#define CMD_RW_REG_RA_MASK		(0x000FF000)

#define CMD_W_REG_RD_SHIFT		(4)
#define CMD_W_REG_RD_MASK		(0x00000FF0)

#define CMD_RS_SID_SHIFT		(27)
#define CMD_RS_SID_MASK			(0x38000000)

#define RSP_SA_ADDR_SHIFT		(29)
#define RSP_SA_ADDR_MASK		(0x60000000)

#define RSP_RW_REG_CMD_SHIFT	(25)
#define RSP_RW_REG_CMD_MASK		(0x1E000000)

#define RSP_RW_REG_RD_H_SHIFT	(12)
#define RSP_RW_REG_RD_H_MASK	(0x000FF000)

#define RSP_RW_REG_RD_L_SHIFT	(4)
#define RSP_RW_REG_RD_L_MASK	(0x00000FF0)

#define RSP_RS_SID_SHIFT		(26)
#define RSP_RS_SID_MASK			(0x1C000000)

#define RSP_RS_SD_SHIFT			(4)
#define RSP_RS_SD_MASK			(0x000FFFF0)

#define CRC_SHIFT				(0)
#define CRC_MASK				(0x00000007)

#define READ_REG_CMD			(0x0C)
#define WRITE_REG_CMD			(0x08)

#define RSP_RS_HAS_DATA			(1)
#define RSP_RS_HAS_DATA_SHIFT	(31)

#define RSP_RS_S0_SHIFT			(3)
#define RSP_RS_S1_SHIFT			(20)


#endif /* NCS10131_NCS10131_H_ */
