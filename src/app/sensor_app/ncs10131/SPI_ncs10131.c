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
#include "aml/spi_aml/spi_aml.h"


/*********************************************************
 * Private macros
 *********************************************************/
#define NCS10131_SPI_INSTANCE		(1)

/* In our case, we perform max two transfers in a row */
#define NCS10131_SPI_MAX_NB_BYTES	(2*4)


/*********************************************************
 * Private declarations
 *********************************************************/
spi_sdk_master_config_t	ncs10131_spi_cfg = {
		.bitcount = 32,
		.bitsPerSec = 1000000, // 1MHz
		.isPcsContinuous = false,
		.transferType = LPSPI_USING_INTERRUPTS,
		.callback = NULL,
		.callbackParam = NULL,
		.clkPhase = LPSPI_CLOCK_PHASE_1ST_EDGE,
		.clkPolarity = LPSPI_SCK_ACTIVE_HIGH,
		.pcsPolarity = LPSPI_ACTIVE_LOW,
		.lpspiSrcClk = 8000000U,
		.lsbFirst = false,
		.whichPcs = LPSPI_PCS3
};

/* SPI buffers */
uint8_t gau8SpiRxBuffer[NCS10131_SPI_MAX_NB_BYTES] = {0};
uint8_t gau8SpiTxBuffer[NCS10131_SPI_MAX_NB_BYTES] = {0};

/* SPI transfer structure, will be filled at runtime */
spi_aml_transfer_t amlSpi1Data = {
		gau8SpiTxBuffer,
		gau8SpiRxBuffer,
        0U,
        0U
};

/*********************************************************
 * Extern functions
 *********************************************************/
//extern _EWL_IMP_EXP_C void * _EWL_CDECL memset(void * s, int c, size_t n) _EWL_CANT_THROW;




void SPI_NCS10131_Init (void)
{
	SPI_AML_MasterInit(NCS10131_SPI_INSTANCE, &ncs10131_spi_cfg, ncs10131_spi_cfg.lpspiSrcClk);
	LPSPI1->TCR |= LPSPI_TCR_BYSW_MASK; // Enable Byte Swap to account for the endianness
}

status_t SPI_NCS10131_tx_request_rx_response (uint32_t u32Cmd, uint32_t *pu32Response)
{
	status_t status;

	/* First transfer: send the command */
	gau8SpiTxBuffer[0] = (uint8_t)(u32Cmd >> 24);
	gau8SpiTxBuffer[1] = (uint8_t)(u32Cmd >> 16);
	gau8SpiTxBuffer[2] = (uint8_t)(u32Cmd >> 8);
	gau8SpiTxBuffer[3] = (uint8_t)(u32Cmd);
	/* Second transfer: read the response */
	gau8SpiTxBuffer[4] = 0x00;
	gau8SpiTxBuffer[5] = 0x00;
	gau8SpiTxBuffer[6] = 0x00;
	gau8SpiTxBuffer[7] = 0x00;

	amlSpi1Data.dataSize = 8; // Two 4-byte transfers

	/* Clear reception buffer */
	memset(gau8SpiRxBuffer, 0, NCS10131_SPI_MAX_NB_BYTES*sizeof(uint8_t));

	status = SPI_AML_MasterTransfer(NCS10131_SPI_INSTANCE, &amlSpi1Data);

	if (kStatus_Success == status)
	{
		*pu32Response = (uint32_t)(gau8SpiRxBuffer[4] << 24) & 0xFF000000;
		*pu32Response |= (uint32_t)(gau8SpiRxBuffer[5] << 16) & 0x00FF0000;
		*pu32Response |= (uint32_t)(gau8SpiRxBuffer[6] << 8) & 0x0000FF00;
		*pu32Response |= (uint32_t)(gau8SpiRxBuffer[7]);
	}

	return status;
}

status_t SPI_NCS10131_tx_request (uint32_t u32Cmd)
{
	status_t status;

	/* Send the command */
	gau8SpiTxBuffer[0] = (uint8_t)(u32Cmd >> 24);
	gau8SpiTxBuffer[1] = (uint8_t)(u32Cmd >> 16);
	gau8SpiTxBuffer[2] = (uint8_t)(u32Cmd >> 8);
	gau8SpiTxBuffer[3] = (uint8_t)(u32Cmd);

	amlSpi1Data.dataSize = 4; // One 4-byte transfers

	status = SPI_AML_MasterTransfer(NCS10131_SPI_INSTANCE, &amlSpi1Data);

	return status;
}

status_t SPI_NCS10131_rx_response (uint32_t *pu32Response)
{
	status_t status;

	/* Send dummy data */
	gau8SpiTxBuffer[0] = 0x00;
	gau8SpiTxBuffer[1] = 0x00;
	gau8SpiTxBuffer[2] = 0x00;
	gau8SpiTxBuffer[3] = 0x00;

	amlSpi1Data.dataSize = 4; // One 4-byte transfers

	/* Clear reception buffer */
	memset(gau8SpiRxBuffer, 0, NCS10131_SPI_MAX_NB_BYTES*sizeof(uint8_t));

	status = SPI_AML_MasterTransfer(NCS10131_SPI_INSTANCE, &amlSpi1Data);

	if (kStatus_Success == status)
	{
		*pu32Response = (uint32_t)(gau8SpiRxBuffer[0] << 24) & 0xFF000000;
		*pu32Response |= (uint32_t)(gau8SpiRxBuffer[1] << 16) & 0x00FF0000;
		*pu32Response |= (uint32_t)(gau8SpiRxBuffer[2] << 8) & 0x0000FF00;
		*pu32Response |= (uint32_t)(gau8SpiRxBuffer[3]);
	}

	return status;
}

