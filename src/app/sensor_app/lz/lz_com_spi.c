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
 * @file  lz_com_spi.c
 * @brief This module contains implementation of the SPI communication
 *        (classic or extended SPI setup).
 */

 /*******************************************************************************
  * Includes
  ******************************************************************************/

#include "gpio_int.h"
#include "lz_com_spi.h"
#include "lz_rci_map.h"
#include "lz_rci.h"
#include "utils/nxp_console_adapter.h"
#include "utils/nxp_console.h"
#include "RF_parse.h"
#include "sdk_project_config.h"
#include "string.h"
  /*******************************************************************************
   * Defines
   ******************************************************************************/

   /*! Delay between consecutive transfers. */
#define LZ_COM_INTER_TRANSFER_DELAY_US 5000U
/*! Delay after CS pin is asserted low to make sure that the device is ready. */
#define LZ_COM_RTS_DELAY_US            5000U
/*! Timeout used to wait for a level of RDY pin after previous transfer. */
#define LZ_COM_RDY_CLR_TIMEOUT_US      100000U
/*! Timeout used to wait for a level of RDY pin after request to send. */
#define LZ_COM_RDY_RTS_TIMEOUT_US      100000U
/*! Timeout used to wait for a level of INT pin. */
#define LZ_COM_INT_TIMEOUT_US          1000000U
/*! Timeout measurement is split into steps. This macro defines value of the
 *  step in us. */
#define LZ_COM_TIMEOUT_STEP_US         20U
 /*! Active level of RDY_N pin. */
#define LZ_COM_RDY_PIN_ACTIVE          0U
/*! Active level of INT_N pin. */
#define LZ_COM_INT_PIN_ACTIVE          0U

/*******************************************************************************
 * Global variables
 ******************************************************************************/
uint8_t uLzRebootDetected = 0;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

 /*!
  * @brief This function requests bus to initiate new transfer.
  *
  * @param drvConfig Pointer to driver instance configuration.
  */
static void LZ_COM_RequestBus(const lz_drv_config_t* drvConfig);

/*!
 * @brief This function releases bus after the transfer is done.
 *
 * @param drvConfig Pointer to driver instance configuration.
 */
static void LZ_COM_ReleaseBus(const lz_drv_config_t* drvConfig);

/*!
 * @brief This function waits until the previous transfer is completed,
 * requests to initiate new transfer and waits until the device signalizes
 * CTS (clear to send).
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param wait      Handling of RDY_N and INT_N pins. It is applied only when the
 *                  six wire SPI mode is used.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
static status_t LZ_COM_RequestToSend(const lz_drv_config_t* drvConfig,
    lz_wait_t wait);

/*******************************************************************************
 * Code - internal functions
 ******************************************************************************/

 /*FUNCTION**********************************************************************
  *
  * Function Name : LZ_COM_RequestBus
  * Description   : This function requests bus to initiate new transfer.
  *
  *END**************************************************************************/
static void LZ_COM_RequestBus(const lz_drv_config_t* drvConfig)
{
    AML_ASSERT(drvConfig != NULL);

    /* Set CS pin low. (request to send) */
    // SPI_AML_MasterSelectDevice(drvConfig->gpioConfig.csPin.gpioInstance,
    //     drvConfig->gpioConfig.csPin.gpioPinNumber, spiPcsActiveLow);

    PINS_DRV_ClearPins(PTB, 1 << drvConfig->gpioConfig.csPin.gpioPinNumber);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_COM_ReleaseBus
 * Description   : This function releases bus after the transfer is done.
 *
 *END**************************************************************************/
static void LZ_COM_ReleaseBus(const lz_drv_config_t* drvConfig)
{
    AML_ASSERT(drvConfig != NULL);

    /* Set CS pin high. */
//    SPI_AML_MasterUnselectDevice(drvConfig->gpioConfig.csPin.gpioInstance,
//        drvConfig->gpioConfig.csPin.gpioPinNumber, spiPcsActiveLow);

    PINS_DRV_SetPins(PTB, 1 << drvConfig->gpioConfig.csPin.gpioPinNumber);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_COM_RequestToSend
 * Description   : This function waits until the previous transfer is completed,
 *                 requests to initiate new transfer and waits until the
 *                 device signalizes CTS (clear to send).
 *
 *END**************************************************************************/
static status_t LZ_COM_RequestToSend(const lz_drv_config_t* drvConfig,
    lz_wait_t wait)
{
    status_t status = kStatus_Success;

    AML_ASSERT(drvConfig != NULL);

    if (drvConfig->spiConfig.spiMode == lzSpiMode6Wired) /* 6-wired extended SPI communication */
    {
        if ((lzWaitRDY == wait) || (lzWaitRDY_INT == wait))
        {
            /* Wait until RDY pin is high to be sure that the previous transmission is
             * complete and the device is ready to receive another data. */
            status = LZ_COM_WaitPinLevel(&(drvConfig->gpioConfig.rdyPin), true,
                LZ_COM_RDY_CLR_TIMEOUT_US);
        }

        if (status == kStatus_Success)
        {
            if (lzWaitRDY_INT == wait)
            {
                /* Wait until INT pin is low to be sure that the device has prepared response data. */
                status = LZ_COM_WaitPinLevel(&(drvConfig->gpioConfig.intPin), false,
                    LZ_COM_INT_TIMEOUT_US);
                u8LzIntSignal = 0;
            }
        }

        if (status == kStatus_Success)
        {
            /* Set CS pin low. (request to send) */
            LZ_COM_RequestBus(drvConfig);

            /* Wait until RDY pin goes low. (clear to send) */
            status = LZ_COM_WaitPinLevel(&(drvConfig->gpioConfig.rdyPin), false,
                LZ_COM_RDY_RTS_TIMEOUT_US);

            if (status != kStatus_Success)
            {
                LZ_COM_ReleaseBus(drvConfig);
            }
        }
    }
    else /* Classic 4-wired SPI communication. */
    {
        /* Add a delay to be sure that the previous transfer is completed. */
        WAIT_AML_WaitUs(LZ_COM_INTER_TRANSFER_DELAY_US);

        LZ_COM_RequestBus(drvConfig);

        /* Add a delay similar to wait for RDY. */
        WAIT_AML_WaitUs(LZ_COM_RTS_DELAY_US);
    }

    return status;
}

/*******************************************************************************
 * Code - public functions
 ******************************************************************************/

 /*FUNCTION**********************************************************************
  *
  * Function Name : LZ_WaitPinLevel
  * Description   : This function waits until a GPIO pin has desired value.
  *
  *END**************************************************************************/
status_t LZ_COM_WaitPinLevel(const lz_gpio_sel_t* gpioPin, bool pinValExp,
    uint32_t tmoutUs)
{
    volatile bool pinVal;           /* Value of a pin. */
    uint32_t      tmoutUsTemp = 0U;

    AML_ASSERT(gpioPin != NULL);

    do
    {
        pinVal = (bool)GPIO_AML_ReadInput(gpioPin->gpioInstance, gpioPin->gpioPinNumber);
        // pinVal = (bool)PINS_DRV_ReadPins(gpioPin->gpioInstance)
        tmoutUsTemp += LZ_COM_TIMEOUT_STEP_US;

        /* WaitUS function ensures the timeout value is not less than a desired
         * time (tmoutUs). */
         //        WAIT_AML_WaitUs(LZ_COM_TIMEOUT_STEP_US);
        tmoutUsTemp += LZ_COM_TIMEOUT_STEP_US;
    } while ((pinVal != pinValExp) && (tmoutUs > tmoutUsTemp));

    return (tmoutUs > tmoutUsTemp) ? kStatus_Success : kStatus_LZ_ComWaitTimeout;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_IsDeviceReady
 * Description   : This function checks if the device is ready for transmission.
 *                 It reads value of the RDY_N pin and returns true if the pin
 *                 is active.
 *
 *END**************************************************************************/
bool LZ_COM_IsDeviceReady(const lz_drv_config_t* drvConfig)
{
    AML_ASSERT(drvConfig != NULL);

    return (GPIO_AML_ReadInput(drvConfig->gpioConfig.rdyPin.gpioInstance,
        drvConfig->gpioConfig.rdyPin.gpioPinNumber) == LZ_COM_RDY_PIN_ACTIVE);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_COM_HasDeviceDataPending
 * Description   : This function checks if there are data to be read from the
 *                 device. It reads value of the INT_N pin and returns true
 *                 if the pin is active.
 *
 *END**************************************************************************/
bool LZ_COM_HasDeviceDataPending(const lz_drv_config_t* drvConfig)
{
    AML_ASSERT(drvConfig != NULL);

    return (GPIO_AML_ReadInput(drvConfig->gpioConfig.intPin.gpioInstance,
        drvConfig->gpioConfig.intPin.gpioPinNumber) == LZ_COM_INT_PIN_ACTIVE);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_COM_ReadWriteFrame
 * Description   : This function sends data via SPI with use of classic (4-wired)
 *                 or extended (6-wired) SPI setup.
 *
 *END**************************************************************************/
status_t LZ_COM_ReadWriteFrame(const lz_drv_config_t* drvConfig, lz_wait_t wait,
    const lz_fr_raw_t* sendFrame, lz_fr_raw_t* recvFrame, uint8_t recvBufferSize)
{
    status_t           status = kStatus_Success;
    spi_aml_transfer_t amlSpiData = { /* SPI transfer structure. */
            NULL,
            NULL,
            0U,
            0U
    };
    uint8_t             recvLen = 0U;  /* Length of the received RCI frame. */

    /* Preconditions. */
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(wait <= lzWaitRDY_INT);
    AML_ASSERT(sendFrame != NULL);
    AML_ASSERT(sendFrame->frame != NULL);
    AML_ASSERT(sendFrame->frameLen > 0U);
    AML_ASSERT(recvFrame != NULL);
    AML_ASSERT(recvFrame->frame != NULL);
    AML_ASSERT(recvBufferSize >= sendFrame->frameLen);

    /* Clear number of received bytes in case of any error. */
    recvFrame->frameLen = 0U;

    status = LZ_COM_RequestToSend(drvConfig, wait);

    if (kStatus_Success == status)
    {   /* Transfer of data. */
        amlSpiData.rxBuffer = recvFrame->frame;
        amlSpiData.txBuffer = sendFrame->frame;

        /* Receive the first byte with length of the frame. */
        amlSpiData.dataSize = 1U;
        recvFrame->frameLen = 1U;

        status = SPI_AML_MasterTransfer(drvConfig->spiConfig.spiInstance, &amlSpiData);
    }

    if (kStatus_Success == status)
    {   /* Update the SPI data structure to receive the rest of the response frame. */
        recvLen = recvFrame->frame[0];

        if (0xFFU == recvLen)
        {   /* The device is not ready. */
            LZ_COM_ReleaseBus(drvConfig);
            status = kStatus_LZ_ComDeviceIsNotReady;
        }
        else if (0x00U == recvLen)
        {   /* There is no response. */
            amlSpiData.dataSize = sendFrame->frameLen > 0 ?
                (sendFrame->frameLen - 1U) : 0U;
        }
        else
        {   /* The length is specified. */
            if ((recvLen + 1U) > recvBufferSize)
            {
                status = kStatus_LZ_ComRecvBufOverflow;
            }
            else if ((sendFrame->frameLen - 1U) > recvLen)
            {
                amlSpiData.dataSize = sendFrame->frameLen - 1U;
            }
            else
            {
                amlSpiData.dataSize = recvLen;
            }

            /* Length of the received frame including the length byte. */
            recvFrame->frameLen = recvLen + 1;
        }
    }

    if ((kStatus_Success == status) && (amlSpiData.dataSize > 0U))
    {   /* Receive or write the rest of the data. */
        /* Update SPI buffers position to retain length byte. */
        amlSpiData.txBuffer += 1;
        amlSpiData.rxBuffer += 1;

        status = SPI_AML_MasterTransfer(drvConfig->spiConfig.spiInstance, &amlSpiData);

        /* Set position of SPI buffers back. */
        amlSpiData.txBuffer -= 1;
        amlSpiData.rxBuffer -= 1;
    }

    /* SPI bus must be released even if an error occurred. */
    LZ_COM_ReleaseBus(drvConfig);

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_ReadWriteFrameFixed
 * Description   : This function sends data via SPI with use of classic (4-wired)
 *                 or extended (6-wired) setup.
 *
 *END**************************************************************************/
status_t LZ_COM_ReadWriteFrameFixed(const lz_drv_config_t* drvConfig,
    lz_wait_t wait, const lz_fr_raw_t* sendFrame, lz_fr_raw_t* recvFrame)
{
    status_t           status = kStatus_Success;
    spi_aml_transfer_t amlSpiData = { /* SPI transfer structure. */
            NULL,
            NULL,
            0U,
            0U
    };

    /* Preconditions. */
    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(wait <= lzWaitRDY_INT);
    AML_ASSERT(sendFrame != NULL);
    AML_ASSERT(sendFrame->frame != NULL);
    AML_ASSERT(sendFrame->frameLen > 0U);
    AML_ASSERT(recvFrame != NULL);
    AML_ASSERT(recvFrame->frame != NULL);

    /* Clear the result in case of an error. */
    recvFrame->frameLen = 0u;

    status = LZ_COM_RequestToSend(drvConfig, wait);

    if (kStatus_Success == status)
    {   /* Set SPI transfer parameters. */
        amlSpiData.dataSize = sendFrame->frameLen;
        amlSpiData.rxBuffer = recvFrame->frame;
        amlSpiData.txBuffer = sendFrame->frame;

        /* Size of received data is fixed. */
        recvFrame->frameLen = sendFrame->frameLen;

        /* Proceed with SPI bidirectional transfer. */
        status = SPI_AML_MasterTransfer(drvConfig->spiConfig.spiInstance, &amlSpiData);

        if ((kStatus_Success == status) && (recvFrame->frame[0] == 0xFFU))
        {   /* Device is not ready to receive data. */
            status = kStatus_LZ_ComDeviceIsNotReady;
        }
    }

    /* SPI bus must be released even if an error occurred. */
    LZ_COM_ReleaseBus(drvConfig);

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_COM_ReadFrame
 * Description   : This function receives a frame via SPI. Note that it sends
 *                 dummy data (zeros).
 *
 *END**************************************************************************/
status_t LZ_COM_ReadFrame(const lz_drv_config_t* drvConfig,
    lz_wait_t wait, lz_fr_raw_t* recvFrame, uint8_t recvBufferSize)
{
    status_t    status;
    uint8_t     dummyBuffer[LZ_FR_MAX_SIZE_B];
    lz_fr_raw_t sendFrame = {
            dummyBuffer,
            1U
    };
#if(PRINT_SPI_MESSAGES)
    uint8_t		counter;
    lz_resp_header_t    frHeader = {
            lzRespTypeCmdReply,
            lzCmdUnknown,
            0U,
            lzRespErrOk
    };
    uint8_t payloadLen = 0U;
#endif

    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(wait <= lzWaitRDY_INT);
    AML_ASSERT(recvFrame != NULL);
    AML_ASSERT(recvFrame->frame != NULL);

    /* The length byte is set to 0 so the device won't
       interpret received data as a command. */
    (void)memset((void*)dummyBuffer, 0x00U, (size_t)LZ_FR_MAX_SIZE_B);

    status = LZ_COM_ReadWriteFrame(drvConfig, wait, &sendFrame, recvFrame, recvBufferSize);

#if(PRINT_SPI_MESSAGES)
    if (status == kStatus_Success) {
        PRINTF("\n\rRECEIVED : ");
        for (counter = 0; counter < recvFrame->frameLen; counter++)
            PRINTF("0x%X ", recvFrame->frame[counter]);
        status = LZ_RCI_ProcessHeader(recvFrame, &frHeader, &payloadLen);

        /* Check if this is a frame containing RF data, and if so, parse it */
        if (lzCmdRbReadMsg == frHeader.cmd)
        {
            RF_ParseTpmsData(recvFrame->frame);
        }
        else if ((lzRespTypeEvent == frHeader.respType) && (lzEventReboot == frHeader.cmd))
        {
            /* We need to detect reboot events to restart the reception */
            uLzRebootDetected = 1;
        }

        PRINTF("\n\r");
    }
    else
    {
        PRINTF("\n Read frame failed!");
    }
#endif

    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_COM_WriteFrame
 * Description   : This function sends a frame via SPI. Note that it discards
 *                 received data.
 *
 *END**************************************************************************/
status_t LZ_COM_WriteFrame(const lz_drv_config_t* drvConfig,
    lz_wait_t wait, const lz_fr_raw_t* sendFrame)
{
    uint8_t     dummyBuffer[LZ_FR_MAX_SIZE_B];
    lz_fr_raw_t recvFrame = {
            dummyBuffer,
            0U
    };

    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(wait <= lzWaitRDY_INT);
    AML_ASSERT(sendFrame != NULL);
    AML_ASSERT(sendFrame->frame != NULL);
    AML_ASSERT(sendFrame->frameLen > 0U);

    return LZ_COM_ReadWriteFrame(drvConfig, wait, sendFrame, &recvFrame, LZ_FR_MAX_SIZE_B);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_COM_ReadAfterWrite
 * Description   : This function sends a frame via SPI and reads a response
 *                 to the frame.
 *
 *END**************************************************************************/
status_t LZ_COM_ReadAfterWrite(const lz_drv_config_t* drvConfig, lz_wait_t wait,
    const lz_fr_raw_t* sendFrame, lz_fr_raw_t* recvFrame, uint8_t recvBufferSize)
{
    status_t  status = kStatus_Success;
    lz_wait_t waitWrite = lzWaitDis;    /* Wait option for writing only. */
    uint8_t   sendFrameLength = 0U;
    uint8_t 			counter = 0U;
    lz_resp_header_t    frHeader = {
            lzRespTypeCmdReply,
            lzCmdUnknown,
            0U,
            lzRespErrOk
    };
    uint8_t payloadLen = 0U;

    AML_ASSERT(drvConfig != NULL);
    AML_ASSERT(wait <= lzWaitRDY_INT);
    AML_ASSERT(sendFrame != NULL);
    AML_ASSERT(sendFrame->frame != NULL);
    AML_ASSERT(sendFrame->frameLen > 0U);
    AML_ASSERT(recvFrame != NULL);
    AML_ASSERT(recvFrame->frame != NULL);
    AML_ASSERT(recvBufferSize >= sendFrame->frameLen);

    /* Write a frame. Read data are rewritten in the next transmission.
     * LZ_COM_WriteFrame is not used to save memory (it allocates another buffer
     * for read data).
     * Note: waiting for INT level is not useful when read data are discarded. */
    waitWrite = (lzWaitRDY_INT == wait) ? lzWaitRDY : wait;
    status = LZ_COM_ReadWriteFrame(drvConfig, waitWrite, sendFrame, recvFrame,
        recvBufferSize);

    if (kStatus_Success == status)
    {   /* Read a response. */
        /* Update the first byte of send data (i.e. set the length byte to zero).
         * It indicates that the host logic does not want to send a CMD, but only
         * read data. */
        sendFrameLength = *(sendFrame->frame + LZ_FR_LENGTH_OF);
        *(sendFrame->frame + LZ_FR_LENGTH_OF) = 0x00U;

        status = LZ_COM_ReadWriteFrame(drvConfig, wait, sendFrame, recvFrame,
            recvBufferSize);

        /* Put the length value back to the send frame. */
        *(sendFrame->frame + LZ_FR_LENGTH_OF) = sendFrameLength;
    }

#if(PRINT_SPI_MESSAGES)
    PRINTF("\n\rSEND    : ");
    for (counter = 0; counter < sendFrame->frameLen; counter++)
        PRINTF("0x%X ", sendFrame->frame[counter]);
    status = LZ_RCI_ProcessHeader(sendFrame, &frHeader, &payloadLen);

    PRINTF("\n\rRECEIVED : ");
    for (counter = 0; counter < recvFrame->frameLen; counter++)
        PRINTF("0x%X ", recvFrame->frame[counter]);
    status = LZ_RCI_ProcessHeader(recvFrame, &frHeader, &payloadLen);

    PRINTF("\n\r");
#endif

    return status;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
