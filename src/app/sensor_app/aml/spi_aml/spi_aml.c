/*
   (c) NXP B.V. 2009-2018, 2022. All rights reserved.

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

/*
 * File: spi_aml.c
 *
 *  SPI, DSPI and LPSPI driver intended for S32 SDK and KSDK.
 *  This driver creates abstraction layer for SPI, DSPI and LPSPI peripherals.
 */

/*******************************************************************************
* Includes
 ******************************************************************************/
#include "spi_aml.h"

/*******************************************************************************
 * Global variables
 ******************************************************************************/

#if (SDK_VERSION == SDK_KSDK)
#if FSL_FEATURE_SOC_SPI_COUNT
static SPI_Type *const g_spiBases[] = SPI_BASE_PTRS;  /*!< Base address pointers. */
#elif FSL_FEATURE_SOC_DSPI_COUNT
static SPI_Type *const g_dspiBases[] = SPI_BASE_PTRS; /*!< Base address pointers. */
#endif
#elif (SDK_VERSION == SDK_S32)
lpspi_state_t g_lpspiState[LPSPI_INSTANCE_COUNT];     /*!< S32 LPSPI state structures. */
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_AML_MasterInit
 * Description   : Initializes the SPI as master.
 *
 *END**************************************************************************/
void SPI_AML_MasterInit(aml_instance_t instance,
        const spi_sdk_master_config_t *spiSdkMasterConfig,
        uint32_t sourceClockHz)
{
    AML_ASSERT(instance < SPI_AML_DEV_CNT);
    AML_ASSERT(spiSdkMasterConfig != NULL);

    #if (SDK_VERSION == SDK_KSDK)
    #if FSL_FEATURE_SOC_SPI_COUNT
    SPI_MasterInit(g_spiBases[instance], spiSdkMasterConfig, sourceClockHz);
    #elif FSL_FEATURE_SOC_DSPI_COUNT
    DSPI_MasterInit(g_dspiBases[instance], spiSdkMasterConfig, sourceClockHz);
    #endif
    #elif (SDK_VERSION == SDK_S32)
    LPSPI_DRV_MasterInit(instance, &(g_lpspiState[instance]), spiSdkMasterConfig);
    AML_UNUSED(sourceClockHz);
    #endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_AML_SlaveInit
 * Description   : Initializes the SPI as slave.
 *
 *END**************************************************************************/
void SPI_AML_SlaveInit(aml_instance_t instance,
        const spi_sdk_slave_config_t *spiSdkSlaveConfig)
{
    AML_ASSERT(instance < SPI_AML_DEV_CNT);
    AML_ASSERT(spiSdkSlaveConfig != NULL);

    #if (SDK_VERSION == SDK_KSDK)
    #if FSL_FEATURE_SOC_SPI_COUNT
    SPI_SlaveInit(g_spiBases[instance], spiSdkSlaveConfig);
    #elif FSL_FEATURE_SOC_DSPI_COUNT
    DSPI_SlaveInit(g_dspiBases[instance], spiSdkSlaveConfig);
    #endif
    #elif (SDK_VERSION == SDK_S32)
    LPSPI_DRV_SlaveInit(instance, &(g_lpspiState[instance]), spiSdkSlaveConfig);
    #endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_AML_MasterDeinit
 * Description   : Deinitializes the SPI master.
 *
 *END**************************************************************************/
void SPI_AML_MasterDeinit(aml_instance_t instance)
{
    AML_ASSERT(instance < SPI_AML_DEV_CNT);

    #if (SDK_VERSION == SDK_KSDK)
    #if FSL_FEATURE_SOC_SPI_COUNT
    SPI_Deinit(g_spiBases[instance]);
    #elif FSL_FEATURE_SOC_DSPI_COUNT
    DSPI_Deinit(g_dspiBases[instance]);
    #endif
    #elif (SDK_VERSION == SDK_S32)
    LPSPI_DRV_MasterDeinit(instance);
    #endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_AML_SlaveDeinit
 * Description   : Deinitializes the SPI slave.
 *
 *END**************************************************************************/
void SPI_AML_SlaveDeinit(aml_instance_t instance)
{
    AML_ASSERT(instance < SPI_AML_DEV_CNT);

    #if (SDK_VERSION == SDK_KSDK)
    #if FSL_FEATURE_SOC_SPI_COUNT
    SPI_Deinit(g_spiBases[instance]);
    #elif FSL_FEATURE_SOC_DSPI_COUNT
    DSPI_Deinit(g_dspiBases[instance]);
    #endif
    #elif (SDK_VERSION == SDK_S32)
    LPSPI_DRV_SlaveDeinit(instance);
    #endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_AML_MasterGetDefaultConfig
 * Description   : Gets and stores default SPI master configuration.
 *
 *END**************************************************************************/
void SPI_AML_MasterGetDefaultConfig(spi_sdk_master_config_t *spiSdkMasterConfig)
{
    AML_ASSERT(spiSdkMasterConfig != NULL);

    #if (SDK_VERSION == SDK_KSDK)
    #if FSL_FEATURE_SOC_SPI_COUNT
    SPI_MasterGetDefaultConfig(spiSdkMasterConfig);
    #elif FSL_FEATURE_SOC_DSPI_COUNT
    DSPI_MasterGetDefaultConfig(spiSdkMasterConfig);
    #endif
    #elif (SDK_VERSION == SDK_S32)
    spiSdkMasterConfig->bitcount = 16U;
    spiSdkMasterConfig->bitsPerSec = 500000U;
    spiSdkMasterConfig->callback = NULL;
    spiSdkMasterConfig->callbackParam = NULL;
    spiSdkMasterConfig->clkPhase = LPSPI_CLOCK_PHASE_1ST_EDGE;
    spiSdkMasterConfig->clkPolarity = LPSPI_ACTIVE_HIGH;
    spiSdkMasterConfig->isPcsContinuous = false;
    spiSdkMasterConfig->lpspiSrcClk = 8000000U;
    spiSdkMasterConfig->lsbFirst= false;
    spiSdkMasterConfig->pcsPolarity = LPSPI_ACTIVE_LOW;
    spiSdkMasterConfig->rxDMAChannel = 2U;
    spiSdkMasterConfig->transferType = LPSPI_USING_INTERRUPTS;
    spiSdkMasterConfig->txDMAChannel = 2U;
    spiSdkMasterConfig->whichPcs = LPSPI_PCS0;
    #endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_AML_SlaveGetDefaultConfig
 * Description   : Gets and stores default SPI slave configuration.
 *
 *END**************************************************************************/
void SPI_AML_SlaveGetDefaultConfig(spi_sdk_slave_config_t *spiSdkSlaveConfig)
{
    AML_ASSERT(spiSdkSlaveConfig != NULL);

    #if (SDK_VERSION == SDK_KSDK)
    #if FSL_FEATURE_SOC_SPI_COUNT
    SPI_SlaveGetDefaultConfig(spiSdkSlaveConfig);
    #elif FSL_FEATURE_SOC_DSPI_COUNT
    DSPI_SlaveGetDefaultConfig(spiSdkSlaveConfig);
    #endif
    #elif (SDK_VERSION == SDK_S32)
    spiSdkSlaveConfig->bitcount = 16U;
    spiSdkSlaveConfig->callback = NULL;
	spiSdkSlaveConfig->callbackParam = NULL;
	spiSdkSlaveConfig->clkPhase = LPSPI_CLOCK_PHASE_1ST_EDGE;
    spiSdkSlaveConfig->clkPolarity = LPSPI_ACTIVE_HIGH;
    spiSdkSlaveConfig->lsbFirst= false;
    spiSdkSlaveConfig->pcsPolarity = LPSPI_ACTIVE_LOW;
    spiSdkSlaveConfig->rxDMAChannel = 2U;
    spiSdkSlaveConfig->transferType = LPSPI_USING_INTERRUPTS;
    spiSdkSlaveConfig->txDMAChannel = 2U;
    spiSdkSlaveConfig->whichPcs = LPSPI_PCS0;
    #endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_AML_MasterFillSdkConfig
 * Description   : Fills SDK SPI master configuration with AML data.
 *
 *END**************************************************************************/
void SPI_AML_MasterFillSdkConfig(const spi_aml_master_config_t *spiAmlMasterConfig,
        spi_sdk_master_config_t *spiSdkMasterConfig)
{
    AML_ASSERT(spiAmlMasterConfig != NULL);
    AML_ASSERT(spiSdkMasterConfig != NULL);

    SPI_AML_MasterGetDefaultConfig(spiSdkMasterConfig);

    #if (SDK_VERSION == SDK_KSDK)
    #if FSL_FEATURE_SOC_SPI_COUNT
    AML_ASSERT((spiAmlMasterConfig->bitCount == 8U) || (spiAmlMasterConfig->bitCount == 16U));

    spiSdkMasterConfig->baudRate_Bps = spiAmlMasterConfig->baudRateHz;
    spiSdkMasterConfig->phase = spiAmlMasterConfig->clkPhase;
    spiSdkMasterConfig->polarity = spiAmlMasterConfig->clkPolarity;
    spiSdkMasterConfig->direction = spiAmlMasterConfig->lsbFirst;
    spiSdkMasterConfig->dataMode = (spiAmlMasterConfig->bitCount == 16U) ? kSPI_16BitMode : kSPI_8BitMode;
    #elif FSL_FEATURE_SOC_DSPI_COUNT
    AML_ASSERT((spiAmlMasterConfig->bitCount >= 4) && (spiAmlMasterConfig->bitCount <= 16U));

    spiSdkMasterConfig->ctarConfig.baudRate = spiAmlMasterConfig->baudRateHz;
    spiSdkMasterConfig->ctarConfig.bitsPerFrame = spiAmlMasterConfig->bitCount;
    spiSdkMasterConfig->ctarConfig.cpha = spiAmlMasterConfig->clkPhase;
    spiSdkMasterConfig->ctarConfig.cpol = spiAmlMasterConfig->clkPolarity;
    spiSdkMasterConfig->ctarConfig.direction = spiAmlMasterConfig->lsbFirst;
    spiSdkMasterConfig->pcsActiveHighOrLow = spiAmlMasterConfig->pcsPolarity;
    #endif
    #elif (SDK_VERSION == SDK_S32)
    spiSdkMasterConfig->bitcount = spiAmlMasterConfig->bitCount;
    spiSdkMasterConfig->bitsPerSec = spiAmlMasterConfig->baudRateHz;
    spiSdkMasterConfig->clkPhase = spiAmlMasterConfig->clkPhase;
    spiSdkMasterConfig->clkPolarity = spiAmlMasterConfig->clkPolarity;
    spiSdkMasterConfig->lpspiSrcClk = spiAmlMasterConfig->sourceClockHz;
    spiSdkMasterConfig->lsbFirst= spiAmlMasterConfig->lsbFirst;
    spiSdkMasterConfig->pcsPolarity = spiAmlMasterConfig->pcsPolarity;
    #endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_AML_SlaveFillSdkConfig
 * Description   : Fills SDK SPI slave configuration with AML data.
 *
 *END**************************************************************************/
void SPI_AML_SlaveFillSdkConfig(const spi_aml_slave_config_t *spiAmlSlaveConfig,
        spi_sdk_slave_config_t *spiSdkSlaveConfig)
{
    AML_ASSERT(spiAmlSlaveConfig != NULL);
    AML_ASSERT(spiSdkSlaveConfig != NULL);

    SPI_AML_SlaveGetDefaultConfig(spiSdkSlaveConfig);

    #if (SDK_VERSION == SDK_KSDK)
    #if FSL_FEATURE_SOC_SPI_COUNT
    AML_ASSERT((spiAmlSlaveConfig->bitCount == 8U) || (spiAmlSlaveConfig->bitCount == 16U));

    spiSdkSlaveConfig->phase = spiAmlSlaveConfig->clkPhase;
    spiSdkSlaveConfig->polarity = spiAmlSlaveConfig->clkPolarity;
    spiSdkSlaveConfig->direction = spiAmlSlaveConfig->lsbFirst;
    spiSdkSlaveConfig->dataMode = (spiAmlSlaveConfig->bitCount == 16U) ? kSPI_16BitMode : kSPI_8BitMode;
    #elif FSL_FEATURE_SOC_DSPI_COUNT
    spiSdkSlaveConfig->ctarConfig.cpha = spiAmlSlaveConfig->clkPhase;
    spiSdkSlaveConfig->ctarConfig.cpol = spiAmlSlaveConfig->clkPolarity;
    #endif
    #elif (SDK_VERSION == SDK_S32)
    spiSdkSlaveConfig->bitcount = spiAmlSlaveConfig->bitCount;
    spiSdkSlaveConfig->clkPhase = spiAmlSlaveConfig->clkPhase;
    spiSdkSlaveConfig->clkPolarity = spiAmlSlaveConfig->clkPolarity;
    spiSdkSlaveConfig->lsbFirst= spiAmlSlaveConfig->lsbFirst;
    spiSdkSlaveConfig->pcsPolarity = spiAmlSlaveConfig->pcsPolarity;
    #endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_AML_MasterSelectDevice
 * Description   : Selects the device with software controlled chip-select pin (PCS).
 *
 *END**************************************************************************/
void SPI_AML_MasterSelectDevice(aml_instance_t portInstance,
        uint8_t pinIndex,
        spi_aml_pcs_polarity_t pcsPolarity)
{
#if (SDK_VERSION == SDK_KSDK)
    AML_ASSERT(portInstance < FSL_FEATURE_SOC_PORT_COUNT);
#elif (SDK_VERSION == SDK_S32)
    AML_ASSERT(portInstance < FEATURE_SOC_PORT_COUNT);
#endif

    AML_ASSERT(pcsPolarity <= spiPcsActiveLow);

    if (pcsPolarity == spiPcsActiveLow)
    {
        GPIO_AML_ClearOutput(portInstance, pinIndex);
    }
    else
    {
        GPIO_AML_SetOutput(portInstance, pinIndex);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_AML_MasterUnselectDevice
 * Description   : Unselects the device with software controlled chip-select pin (PCS).
 *
 *END**************************************************************************/
void SPI_AML_MasterUnselectDevice(aml_instance_t portInstance,
        uint8_t pinIndex,
        spi_aml_pcs_polarity_t pcsPolarity)
{
#if (SDK_VERSION == SDK_KSDK)
    AML_ASSERT(portInstance < FSL_FEATURE_SOC_PORT_COUNT);
#elif (SDK_VERSION == SDK_S32)
    AML_ASSERT(portInstance < FEATURE_SOC_PORT_COUNT);
#endif
    AML_ASSERT(pcsPolarity <= spiPcsActiveLow);

    if (pcsPolarity == spiPcsActiveLow)
    {
        GPIO_AML_SetOutput(portInstance, pinIndex);
    }
    else
    {
        GPIO_AML_ClearOutput(portInstance, pinIndex);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_AML_MasterTransferBlocking
 * Description   : Performs blocking master transfer of data. The methods returns
 *                 when all data are sent and received.
 *
 *END**************************************************************************/
status_t SPI_AML_MasterTransfer(aml_instance_t instance,
        spi_aml_transfer_t *masterTransfer)
{
    AML_ASSERT(instance < SPI_AML_DEV_CNT);
    AML_ASSERT(masterTransfer != NULL);

    #if (SDK_VERSION == SDK_KSDK)
    status_t error;

    #if FSL_FEATURE_SOC_SPI_COUNT
    spi_transfer_t xfer;

    xfer.txData = masterTransfer->txBuffer;
    xfer.rxData = masterTransfer->rxBuffer;
    xfer.dataSize = masterTransfer->dataSize;
    xfer.flags = masterTransfer->configFlags;

    error = SPI_MasterTransferBlocking(g_spiBases[instance], &xfer);

    if (error == kStatus_SPI_Busy)
    {
        return kStatus_AML_SPI_Busy;
    }
    #elif FSL_FEATURE_SOC_DSPI_COUNT
    dspi_transfer_t xfer;

    xfer.txData = masterTransfer->txBuffer;
    xfer.rxData = masterTransfer->rxBuffer;
    xfer.dataSize = masterTransfer->dataSize;
    xfer.configFlags = masterTransfer->configFlags;

    error = DSPI_MasterTransferBlocking(g_dspiBases[instance], &xfer);

    if (error == kStatus_DSPI_Busy)
    {
        return kStatus_AML_SPI_Busy;
    }
    #endif
    if (error == kStatus_Success)
    {
        return kStatus_Success;
    }
    else if (error == kStatus_InvalidArgument)
    {
        return kStatus_InvalidArgument;
    }
    else
    {
        return kStatus_AML_SPI_Error;
    }
    #elif (SDK_VERSION == SDK_S32)
    status_t error;

//    error = LPSPI_DRV_MasterTransferBlocking(instance, masterTransfer->txBuffer,
//            masterTransfer->rxBuffer, masterTransfer->dataSize, SPI_AML_TIMEOUT);

        error = LPSPI_DRV_MasterTransfer(instance, masterTransfer->txBuffer,
                masterTransfer->rxBuffer, masterTransfer->dataSize);
        while(LPSPI_DRV_MasterGetTransferStatus(instance, NULL) !=  STATUS_SUCCESS);

    if (error == STATUS_SUCCESS)
    {
        return kStatus_Success;
    }
    else if (error == STATUS_BUSY)
    {
        return kStatus_AML_SPI_Busy;
    }
    else
    {
        return kStatus_AML_SPI_Error;
    }
    #endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SPI_AML_SlaveTransfer
 * Description   : Performs slave transfer of data.
 *
 *END**************************************************************************/
status_t SPI_AML_SlaveTransfer(aml_instance_t instance,
        spi_aml_transfer_t *slaveTransfer,
        spi_slave_handle_t *slaveHandle,
        spi_slave_callback_t slaveCallback)
{
    AML_ASSERT(instance < SPI_AML_DEV_CNT);
    AML_ASSERT(slaveTransfer != NULL);
    AML_ASSERT(slaveHandle != NULL);

    #if (SDK_VERSION == SDK_KSDK)
    status_t error;

    #if FSL_FEATURE_SOC_SPI_COUNT
    spi_transfer_t xfer;

    SPI_SlaveTransferCreateHandle(g_spiBases[instance], slaveHandle, slaveCallback, NULL);

    xfer.txData = slaveTransfer->txBuffer;
    xfer.rxData = slaveTransfer->rxBuffer;
    xfer.dataSize = slaveTransfer->dataSize;
    xfer.flags = slaveTransfer->configFlags;

    error = SPI_SlaveTransferNonBlocking(g_spiBases[instance], slaveHandle, &xfer);

    if (error == kStatus_SPI_Busy)
    {
        return kStatus_AML_SPI_Busy;
    }
    #elif FSL_FEATURE_SOC_DSPI_COUNT
    dspi_transfer_t xfer;

    DSPI_SlaveTransferCreateHandle(g_dspiBases[instance], slaveHandle, slaveCallback, NULL);

    xfer.txData = slaveTransfer->txBuffer;
    xfer.rxData = slaveTransfer->rxBuffer;
    xfer.dataSize = slaveTransfer->dataSize;
    xfer.configFlags = slaveTransfer->configFlags;

    error = DSPI_SlaveTransferNonBlocking(g_dspiBases[instance], slaveHandle, &xfer);

    if (error == kStatus_DSPI_Busy)
    {
        return kStatus_AML_SPI_Busy;
    }
    #endif
    if (error == kStatus_Success)
    {
        return kStatus_Success;
    }
    else if (error == kStatus_InvalidArgument)
    {
        return kStatus_InvalidArgument;
    }
    else
    {
        return kStatus_AML_SPI_Error;
    }
    #elif (SDK_VERSION == SDK_S32)

    AML_UNUSED(slaveHandle);
    AML_UNUSED(slaveCallback);
    status_t error;

    error = LPSPI_DRV_SlaveTransfer(instance, slaveTransfer->txBuffer,
            slaveTransfer->rxBuffer, slaveTransfer->dataSize);

    if (error == STATUS_SUCCESS)
    {
        return kStatus_Success;
    }
    else if (error == STATUS_BUSY)
    {
        return kStatus_AML_SPI_Busy;
    }
    else
    {
        return kStatus_AML_SPI_Error;
    }
    #endif
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
