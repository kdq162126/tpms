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
 * @file  lz_rci.c
 * @brief This module contains implementation of RCI commands.
 *        It contains function for manipulation with SPI frames.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "lz_rci.h"
#include "nxp_console_adapter.h"
#include "nxp_console.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

/*! Shifting either MSB or LSB in CRC calculation. */
#define LZ_RCI_CRC_MSB_FIRST            1U

/*! Printing  the variable. */
#define LZ_RCI_PRINT_CASE(p)            case(p): printVal = #p; break;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief This function calculates CRC of one byte with default polynomial 0x07.
 *
 * @param inputByte  Input data.
 * @param initialCRC Current CRC value.
 *
 * @return calculated CRC-8 value of the byte.
 */
static uint8_t LZ_RCI_CalculateCRC8Byte(uint8_t inputByte, uint8_t initialCRC);

/*!
 * @brief This function calculates CRC-8 checksum with default polynomial 0x07.
 *
 * @param data Data for the checksum calculation.
 * @param len  Length of the data.
 *
 * @return calculated CRC-8 value.
 */
static uint8_t LZ_RCI_CalcCRC(const uint8_t *data, uint8_t len);

/*!
 * @brief This function creates a RCI frame.
 *
 * It fills common fields of header, provided payload and calculated CRC into
 * a buffer.
 *
 * @param cmd        Command code and sub-code.
 * @param payload    Payload bytes. It is copied only when the pointer is not
 *                   NULL and payloadLen is non-zero value.
 * @param paylaodLen Payload size (in bytes). It is used to calculate size of
 *                   the RCI frame.
 * @param headerLen  Header size (in bytes). It is used to calculate size of
 *                   the RCI frame.
 * @param buffer     Pointer where the RCI frame is stored.
 */
static void LZ_RCI_CreateFrameInt(lz_cmd_code_t cmd, const uint8_t *payload,
        uint8_t payloadLen, uint8_t headerLen, lz_fr_raw_t *buffer);

/*!
 * @brief This function checks the length field in a received response frame.
 *
 * @param response Received RCI frame.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
static status_t LZ_RCI_CheckRespLen(const lz_fr_raw_t *response);

/*******************************************************************************
 * Code - internal functions
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_RCI_CalculateCRC8Byte
 * Description   : This function calculates CRC of one byte with default
 *                 polynomial 0x07.
 *
 *END**************************************************************************/
static uint8_t LZ_RCI_CalculateCRC8Byte(uint8_t inputByte, uint8_t initialCRC)
{
    uint16_t wCRC = initialCRC; /* CRC initialized with previous CRC */
    uint8_t  inputShift = 0U;   /* Shifting value. */
    uint8_t  temp = 0U;
    uint8_t  i = 0U;

    /* Calculation is done for every bit of inputByte */
    for (i = 0u; i < 8U; i++)
    {
        /* Shift wCRC to the left */
        wCRC <<= 1U;
        /* Shifting either MSB or LSB first. */
        inputShift = (LZ_RCI_CRC_MSB_FIRST > 0U) ? ((8U - 1U) - i) : i;

        temp = (inputByte >> inputShift) & 0x01U;
        if (((wCRC > 0xFFU) ^ temp) == 1U)
        {
            wCRC ^= 0x07U;
        }

        /* Mask out last 8 bits of wCRC */
        wCRC &= 0xFFU;
    }

    /* Return wCRC casted to byte (8 MSbits are 0 because they were masked out). */
    return (uint8_t)wCRC;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_RCI_CalcCRC
 * Description   : This function calculates CRC-8 checksum with default polynomial
 *                 0x07.
 *
 *END**************************************************************************/
static uint8_t LZ_RCI_CalcCRC(const uint8_t *data, uint8_t len)
{
    uint8_t currentCRC = 0U; /* Current CRC value. */
    uint8_t i = 0U;

    for(i = 0U; i < len; i++)
    {
        currentCRC = LZ_RCI_CalculateCRC8Byte(*(data + i), currentCRC);
    }
    return currentCRC;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_RCI_CreateFrameInt
 * Description   : This function creates a RCI frame.
 *
 *END**************************************************************************/
static void LZ_RCI_CreateFrameInt(lz_cmd_code_t cmd, const uint8_t *payload,
        uint8_t payloadLen, uint8_t headerLen, lz_fr_raw_t *buffer)
{
    /* Preconditions. */
    AML_ASSERT((cmd >= LZ_CMD_FIRST_ID) && (cmd <= LZ_CMD_LAST_ID));
    AML_ASSERT(payloadLen <= (LZ_FR_MAX_SIZE_B - LZ_FR_HEADER_B));
    AML_ASSERT(buffer != NULL);
    AML_ASSERT(buffer->frame != NULL);

    /* Size of the whole frame. Payload offset is the size of header part. */
    buffer->frameLen = headerLen + payloadLen + 1U;

    /* Add length, command and sub-command fields. */
    *(buffer->frame + LZ_FR_LENGTH_OF) = buffer->frameLen - 1U;
    *(buffer->frame + LZ_FR_CMD_OF) = (uint8_t)((cmd & 0xFF00U) >> 8U);

    if ((lzCmdWiEcho != cmd) && (lzCmdWiEcho != cmd))
    {   /* Echo commands do not have sub-command field. */
        *(buffer->frame + LZ_FR_SUBCMD_OF) = (uint8_t)(cmd & 0x00FFU);
    }

    /* Copy the payload if any. */
    if ((payloadLen > 0U) && (payload != NULL))
    {
        (void) memcpy((void*)(buffer->frame + headerLen), (void*)payload,
                (size_t)payloadLen);
    }

    /* Adds CRC-8 checksum from byte 0 to (length - 2). */
    *(buffer->frame + (buffer->frameLen - 1)) =
            LZ_RCI_CalcCRC(buffer->frame, LZ_FR_CRC_OF(buffer->frameLen));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_RCI_CheckRespLen
 * Description   : This function checks the length field in a received response
 *                 frame.
 *
 *END**************************************************************************/
static status_t LZ_RCI_CheckRespLen(const lz_fr_raw_t *response)
{
    status_t status = kStatus_Fail;
    uint8_t  lengthField = 0U;      /* Value of the length field. */

    lengthField = *(response->frame + LZ_FR_ACK_LENGTH_OF);

    /* Check frame length. */
    if (0U == lengthField)
    {   /* No response, this can happen when INT pin is not used or the device
         * does not have any data to send. */
        status = kStatus_LZ_ComNoResponse;
    }
    else if (0xFFU == lengthField)
    {   /* The device was not ready to receive data. */
        status = kStatus_LZ_ComDeviceIsNotReady;
    }
    else if ((lengthField + 1U) != response->frameLen)
    {   /* The length field does not match size of received data. */
        status = kStatus_LZ_ComResponseLength;
    }
    else
    {
        status = kStatus_Success;
    }

    return status;
}

/*******************************************************************************
 * Code - public functions
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_RCI_GetRespType
 * Description   : This function determines type of the received response.
 *
 *END**************************************************************************/
lz_resp_type_t LZ_RCI_GetRespType(const uint8_t *response)
{
    lz_resp_type_t respType = lzRespTypeUnknown;

    AML_ASSERT(response != NULL);

    switch (*(response + LZ_FR_CMD_OF))
    {
        case LZ_ERR_COM_CODE:
            respType = lzRespTypeComError;
            break;

        case LZ_EV_HEARTBEAT_CODE:
        case LZ_EV_REBOOT_CODE:
        case LZ_EV_RF_CODE:
            respType = lzRespTypeEvent;
            break;

        case LZ_CMD_DC_CODE:
        case LZ_CMD_OS_CODE:
        case LZ_CMD_RB_CODE:
        case LZ_CMD_PE_CODE:
        case LZ_CMD_DD_CODE:
        /* LZ_CMD_WI_CONFIG_CODE command is not included in this list, because
         * it has the same command code as LZ_CMD_WI_CLEAR_CODE. */
        case LZ_CMD_WI_CLEAR_CODE:
        case LZ_CMD_WI_STORE_CODE:
        case LZ_CMD_WI_ECHO_CODE:
        case LZ_CMD_WI_ECHO_NXP_CODE:
        case LZ_CMD_OBM_CODE:
        case LZ_CMD_MISC_CODE:
        case LZ_CMD_EV_CODE:
            respType = lzRespTypeCmdReply;
            break;

        default:
            respType = lzRespTypeUnknown;
            break;
    }

    return respType;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_RCI_GetCmdCode
 * Description   : This function returns a command code.
 *
 *END**************************************************************************/
lz_cmd_code_t LZ_RCI_GetCmdCode(const uint8_t *response)
{
    lz_cmd_code_t cmd = lzCmdUnknown;
    uint8_t       temp = 0U;
    const char*   printVal = 0;

    AML_ASSERT(response != NULL);

    temp = *(response + LZ_FR_ACK_CMD_OF);

    if (LZ_CMD_WI_ECHO_CODE == temp)
    {   /* Echo command does not have sub-command field. */
        cmd = lzCmdWiEcho;
    }
    if (LZ_CMD_WI_ECHO_NXP_CODE == temp)
    {   /* Echo NXP command does not have sub-command field. */
        cmd = lzCmdWiEchoNxp;
    }
    else
    {
        cmd = (lz_cmd_code_t)LZ_GET_CMD(response);
    	switch (cmd)
        {
            /* Device configuration. */
            LZ_RCI_PRINT_CASE(lzCmdDcSet);
            LZ_RCI_PRINT_CASE(lzCmdDcGet);
            LZ_RCI_PRINT_CASE(lzCmdDcCopy2Ram);
            LZ_RCI_PRINT_CASE(lzCmdDcCopy2Nv);
            LZ_RCI_PRINT_CASE(lzCmdDcPatchRam);
            LZ_RCI_PRINT_CASE(lzCmdDcHash);
            LZ_RCI_PRINT_CASE(lzCmdDcGetStatus);
            /* Operating state. */
            LZ_RCI_PRINT_CASE(lzCmdOsSetPolling);
            LZ_RCI_PRINT_CASE(lzCmdOsGetPolling);
            LZ_RCI_PRINT_CASE(lzCmdOsStorePolling);
            LZ_RCI_PRINT_CASE(lzCmdOsSetMode);
            LZ_RCI_PRINT_CASE(lzCmdOsStatus);
            /* RX buffer. */
            LZ_RCI_PRINT_CASE(lzCmdRbReadMsg);
            LZ_RCI_PRINT_CASE(lzCmdRbReadBytes);
            LZ_RCI_PRINT_CASE(lzCmdRbFlushMsg);
            LZ_RCI_PRINT_CASE(lzCmdRbStatus);
            /* Personality. */
            LZ_RCI_PRINT_CASE(lzCmdPeVersions);
            LZ_RCI_PRINT_CASE(lzCmdPeCapabilities);
            /* Debug. */
            LZ_RCI_PRINT_CASE(lzCmdDdSet);
            LZ_RCI_PRINT_CASE(lzCmdDdGet);
            LZ_RCI_PRINT_CASE(lzCmdDdHash);
            LZ_RCI_PRINT_CASE(lzCmdDdSig);
            /* Wired interface. */
            LZ_RCI_PRINT_CASE(lzCmdWiConfig);
            LZ_RCI_PRINT_CASE(lzCmdWiClear);
            LZ_RCI_PRINT_CASE(lzCmdWiStore);
            LZ_RCI_PRINT_CASE(lzCmdWiEcho);
            LZ_RCI_PRINT_CASE(lzCmdWiEchoNxp);
            /* On board measurement. */
            LZ_RCI_PRINT_CASE(lzCmdObmConf);
            LZ_RCI_PRINT_CASE(lzCmdObmRead);
            LZ_RCI_PRINT_CASE(lzCmdObmRssiOfmu);
            LZ_RCI_PRINT_CASE(lzCmdObmSetFreqOffset);
            LZ_RCI_PRINT_CASE(lzCmdObmGetFreqOffset);
            /* Miscellaneous commands. */
            LZ_RCI_PRINT_CASE(lzCmdMiscWaitUs);
            LZ_RCI_PRINT_CASE(lzCmdMiscPinToggle);
            /* Configuration of Heartbeat event. */
            LZ_RCI_PRINT_CASE(lzCmdEvTmrHeartbeat);
            /* Events received from Lizard. */
            LZ_RCI_PRINT_CASE(lzEventReboot);
            LZ_RCI_PRINT_CASE(lzEventHeartbeat);
            LZ_RCI_PRINT_CASE(lzEventRfWUPFound);
            LZ_RCI_PRINT_CASE(lzEventRfFSYNCFound);
            LZ_RCI_PRINT_CASE(lzEventRfEOM);
            LZ_RCI_PRINT_CASE(lzEventRfFSYNCduringData);
            LZ_RCI_PRINT_CASE(lzEventRfWUPduringFSYNC);
            LZ_RCI_PRINT_CASE(lzEventRfWUPduringDATA);
            LZ_RCI_PRINT_CASE(lzEventRfWUPfailed);
            LZ_RCI_PRINT_CASE(lzEventRfFSYNCfailed);
            LZ_RCI_PRINT_CASE(lzEventRfStartupFailed);
            LZ_RCI_PRINT_CASE(lzEventRfStartupSuccess);
            /* Communication error. */
            LZ_RCI_PRINT_CASE(lzErrorCom);
            LZ_RCI_PRINT_CASE(lzCmdUnknown);
            default:
                cmd = lzCmdUnknown;
                break;
        }
    	PRINTF("\n\rFRAME CMD CODE: %s", printVal);
    }
    return cmd;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_RCI_GetRebootReason
 * Description   : This function determines reason of a received reboot event.
 *
 *END**************************************************************************/
lz_rb_ev_reason_t LZ_RCI_GetRebootReason(const uint8_t *eventFrame)
{
    uint8_t reason = 0U;

    AML_ASSERT(eventFrame != NULL);

    reason = *(eventFrame + LZ_EV_REBOOT_REASON_OF);

    if (lzRebootEvReasTxBufFull < reason)
    {
        reason = (uint8_t)lzRebootEvUnknown;
    }

    return (lz_rb_ev_reason_t)reason;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_RCI_ProcessHeader
 * Description   : This function processes a received frame. It checks type
 *                 of the frame, CRC, frame length and error code.
 *
 *END**************************************************************************/
status_t LZ_RCI_ProcessHeader(const lz_fr_raw_t *response,
        lz_resp_header_t *header, uint8_t *payloadLen)
{
    status_t status = kStatus_Fail;

    AML_ASSERT(NULL != response);
    AML_ASSERT(NULL != response->frame);
    AML_ASSERT(0U < response->frameLen);
    AML_ASSERT(NULL != header);
    AML_ASSERT(NULL != payloadLen);

    /* Set default values of result in );of any error. */
    header->cmd = lzCmdUnknown;
    (*payloadLen) = 0U;
    header->respError = lzRespErrUnknown;
    header->respType = lzRespTypeUnknown;
    header->timestamp = 0U;

    status = LZ_RCI_CheckRespLen(response);

    /* Parse header of the frame. */
    if (lzRespTypeNoResp == status)
    {   /* The length field in the received frame is set to zero. */
        header->respType = lzRespTypeNoResp;
    }

    /* Check CRC. */
    if (kStatus_Success == status)
    {
        if (LZ_RCI_CalcCRC(response->frame, response->frameLen - 1U) !=
                *(response->frame + LZ_FR_CRC_OF(response->frameLen)))
        {
            status = kStatus_LZ_ComResponseIntegrity;
            header->respType = lzRespTypeNoResp;
        	PRINTF("\n\r CRC ERROR in FRAME \n\r");
        }
    }
    else
    {
    	PRINTF("\n\r RESPONSE LENGTH ERROR \n\r");
    }

    /* Get command code. */
    if (kStatus_Success == status)
    {
        header->respType = LZ_RCI_GetRespType(response->frame);
        header->cmd = LZ_RCI_GetCmdCode(response->frame);

        if ((lzCmdUnknown == header->cmd) || (lzRespTypeUnknown == header->respType))
        {
            status = kStatus_LZ_ComResponseFormat;
        }
    }
    /* Fetch other header fields. */
    if (kStatus_Success == status)
    {
        if ((lzRespTypeCmdReply == header->respType) &&
                ((lzCmdWiEcho == header->cmd) || (lzCmdWiEchoNxp == header->cmd)))
        {   /* Echo commands do not have time stamp and status fields. */
            (*payloadLen) = response->frameLen - (LZ_WI_ECHO_HEADER_B + 1U);
            header->respError = lzRespErrOk;
        }
        else if ((lzRespTypeCmdReply == header->respType) ||
                (lzRespTypeComError == header->respType) ||
                (lzRespTypeEvent == header->respType))
        {   /* Get time stamp, status and payload length. */
            header->timestamp = LZ_GET_TIMESTAMP(response->frame);
            header->respError = (lz_resp_err_code_t)
                    (*(response->frame + LZ_FR_ACK_STATUS_OF));
            (*payloadLen) = response->frameLen - (LZ_FR_ACK_HEADER_B + 1U);

            status = (header->respError == lzRespErrOk) ? kStatus_Success : kStatus_LZ_RespErr;
        }
        else
        {
            status = kStatus_LZ_ComResponseFormat;
        }
    }
    else
    {
    	PRINTF("\n\r UNEXPECTED COMMAND CODE in FRAME \n\r");
    }
    return status;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LZ_RCI_CreateFrame
 * Description   : This function creates a RCI command frame.
 *
 *END**************************************************************************/
void LZ_RCI_CreateFrame(lz_cmd_code_t cmd, const uint8_t *payload,
        uint8_t payloadLen, lz_fr_raw_t *buffer)
{
    uint8_t payloadOf = 0U;

    /* Preconditions. */
    AML_ASSERT((cmd >= LZ_CMD_FIRST_ID) && (cmd <= LZ_CMD_LAST_ID));
    AML_ASSERT(payloadLen <= (LZ_FR_MAX_SIZE_B - LZ_FR_HEADER_B));
    AML_ASSERT(buffer != NULL);

    /* Offset of payload differs for some commands. */
    if ((cmd == lzCmdWiEcho) || (cmd == lzCmdWiEchoNxp))
    {
        payloadOf = LZ_WI_ECHO_PAYLOAD_OF;
    }
    else
    {
        payloadOf = LZ_FR_PAYLOAD_OF;
    }

    LZ_RCI_CreateFrameInt(cmd, payload, payloadLen, payloadOf, buffer);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
