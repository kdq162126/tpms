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


/*!
 * @file  lz_init.c
 * @brief This module contains implementation of the SW driver for NCK2910AHN.
 *        Note that this module is built on the top of RCI module.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "lz_init.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

/*! Delay for RST pin toggle during boot routine. */
#define LZ_RST_HOLD_US              5000U
/*! Delay till the boot routine is complete. */
#define LZ_BOOT_DELAY_US            15000U

/*******************************************************************************
 * Code - general functions
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_SetupGPIOs
 * Description   : Configures GPIOs used by the Lizard driver.
 *
 *END**************************************************************************/
status_t LZ_SetupGPIOs(const lz_drv_config_t *drvConfig)
{
    AML_ASSERT(drvConfig != NULL);

    /* Set CSB pin as output with default value high (active low). */
    GPIO_AML_SetOutput(drvConfig->gpioConfig.csPin.gpioInstance,
            drvConfig->gpioConfig.csPin.gpioPinNumber);
    GPIO_AML_SetDirection(drvConfig->gpioConfig.csPin.gpioInstance,
            drvConfig->gpioConfig.csPin.gpioPinNumber, gpioDirDigitalOutput);

    /* Set INT pin as input. */
    GPIO_AML_SetDirection(drvConfig->gpioConfig.intPin.gpioInstance,
            drvConfig->gpioConfig.intPin.gpioPinNumber, gpioDirDigitalInput);

    /* Set RDY pin as input. */
    GPIO_AML_SetDirection(drvConfig->gpioConfig.rdyPin.gpioInstance,
            drvConfig->gpioConfig.rdyPin.gpioPinNumber, gpioDirDigitalInput);

    /* Set RST pin as output. */
    GPIO_AML_SetOutput(drvConfig->gpioConfig.rstPin.gpioInstance,
            drvConfig->gpioConfig.rstPin.gpioPinNumber);
    GPIO_AML_SetDirection(drvConfig->gpioConfig.rstPin.gpioInstance,
            drvConfig->gpioConfig.rstPin.gpioPinNumber, gpioDirDigitalOutput);

    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_SetupSPI
 * Description   : Configures SPI module used by the Lizard driver.
 *
 *END**************************************************************************/
status_t LZ_SetupSPI(const lz_drv_config_t *drvConfig,
        const spi_sdk_master_config_t *spiSdkMasterConfig)
{
    status_t status = kStatus_Success;

    AML_ASSERT(drvConfig != NULL);

//    if (spiSdkMasterConfig == NULL)
//    {   /* Needed when spiSdkMasterConfig argument is NULL. */
//        spi_sdk_master_config_t spiSdkConfig;
//        spi_aml_master_config_t spiAmlConfig;
//
//        if ((drvConfig->spiConfig.baudRate < 1000) ||
//                (drvConfig->spiConfig.baudRate > 2000000))
//        {
//            status = kStatus_LZ_InitFail;
//        }
//
//        if (kStatus_Success == status)
//        {
//            spiAmlConfig.baudRateHz = drvConfig->spiConfig.baudRate;
//            spiAmlConfig.bitCount = 8U;
//            /* The device is able to set different values even for CPOL and CPHA. */
//            spiAmlConfig.clkPhase = drvConfig->spiConfig.clkPhase;
//            spiAmlConfig.clkPolarity = drvConfig->spiConfig.clkPol;
//            spiAmlConfig.lsbFirst = false;
//            spiAmlConfig.pcsPolarity = spiPcsActiveLow;
//            spiAmlConfig.sourceClockHz = drvConfig->spiConfig.sourceClkHz;
//
//            SPI_AML_MasterFillSdkConfig(&spiAmlConfig, &spiSdkConfig);
//            SPI_AML_MasterInit(drvConfig->spiConfig.spiInstance, &spiSdkConfig,
//                    drvConfig->spiConfig.sourceClkHz);
//        }
//    }
//    else
//    {   /* Use provided spiSdkMasterConfig configuration. */
        SPI_AML_MasterInit(drvConfig->spiConfig.spiInstance, spiSdkMasterConfig,
                drvConfig->spiConfig.sourceClkHz);
//    }

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_Init
 * Description   : Initializes the Lizard driver based on user configuration.
 *
 *END**************************************************************************/
status_t LZ_Init(const lz_drv_config_t *drvConfig)
{
    status_t            status = kStatus_Success;
    static uint8_t             recvBuffer[LZ_EV_REBOOT_B];
    lz_fr_raw_t         recvFrame = {
            recvBuffer,
            0U
    };
    lz_resp_header_t    frHeader = {
            lzRespTypeCmdReply,
            lzCmdUnknown,
            0U,
            lzRespErrOk
    };
    uint8_t payloadLen = 0U;

    /* Preconditions. */
    AML_ASSERT(drvConfig != NULL);

    /* RST to low for t_rst_hold. */
    GPIO_AML_ClearOutput(drvConfig->gpioConfig.rstPin.gpioInstance,
            drvConfig->gpioConfig.rstPin.gpioPinNumber);
    WAIT_AML_WaitUs(LZ_RST_HOLD_US);
    GPIO_AML_SetOutput(drvConfig->gpioConfig.rstPin.gpioInstance,
            drvConfig->gpioConfig.rstPin.gpioPinNumber);

    /* Boot routine completed after t_boot_rci. */
    WAIT_AML_WaitUs(LZ_BOOT_DELAY_US);

    /* Read and check the Reboot event. */
    status = LZ_COM_ReadFrame(drvConfig, lzWaitRDY_INT, &recvFrame, LZ_EV_REBOOT_B);

    if (kStatus_Success == status)
    {
        status = LZ_RCI_ProcessHeader(&recvFrame, &frHeader, &payloadLen);
    }
    if (kStatus_Success == status)
    {
        if ((lzRespTypeEvent != frHeader.respType) || (lzEventReboot != frHeader.cmd) ||
                (LZ_RCI_GetRebootReason(recvBuffer) != lzRebootEvReasPOR))
        {
            status = kStatus_LZ_InitFail;
        }
    }

    return status;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
