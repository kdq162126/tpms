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
 * @file  lz_rci.h
 * @brief This module contains implementation of RCI commands.
 *        It contains function for manipulation with SPI frames.
 */

#ifndef SOURCE_LZ_RCI_H_
#define SOURCE_LZ_RCI_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "lz_rci_map.h"
#include "lz_types.h"

/*******************************************************************************
 * Defines - Values of the RCI frames
 ******************************************************************************/

/*!
 * @addtogroup lz_rci_h_enum_group
 * Module enumerations.
 */
/*! @{ */
/*! @brief List of all command, event and error codes and sub-codes included
 * in RCI frames. */
typedef enum
{
    /* Device configuration. */
    lzCmdDcSet = 0x71F6U,               /*!< Sets a configuration. */
    lzCmdDcGet = 0x71F0U,               /*!< Gets a configuration. */
    lzCmdDcCopy2Ram = 0x7111U,          /*!< Copies a configuration to RAM. */
    lzCmdDcCopy2Nv = 0x7110U,           /*!< Copies a configuration to non-volatile
                                             memory. */
    lzCmdDcPatchRam = 0x71D0U,          /*!< Alters a RAM based configuration. */
    lzCmdDcHash = 0x710FU,              /*!< Generates a hash for a configuration. */
    lzCmdDcGetStatus = 0x7103U,         /*!< Gets a configuration status. */
    /* Operating state. */
    lzCmdOsSetPolling = 0x0602U,        /*!< Sets the polling sequence in
                                             the RAM buffer. */
    lzCmdOsGetPolling = 0x0603U,        /*!< Gets the polling sequence from
                                             the RAM buffer. */
    lzCmdOsStorePolling = 0x0604U,      /*!< Stores the polling sequence to
                                             the persistent storage. */
    lzCmdOsSetMode = 0x0601U,           /*!< Controls the mode of the device
                                             (polling, continuous reception). */
    lzCmdOsStatus = 0x0600U,            /*!< Returns the current state
                                             of the device. */
    /* RX buffer. */
    lzCmdRbReadMsg = 0x0F80U,           /*!< Reads a single message/frame
                                             from the RX buffer. */
    lzCmdRbReadBytes = 0x0F10U,         /*!< Reads some bytes out of an unread
                                             frame. */
    lzCmdRbFlushMsg = 0x0F88U,          /*!< Flushes a frame from the RX buffer. */
    lzCmdRbStatus = 0x0F81U,            /*!< Returns the status of the RX buffer. */
    /* Personality. */
    lzCmdPeVersions = 0x03F0U,          /*!< Returns the version identifiers
                                             of the device. */
    lzCmdPeCapabilities = 0x030FU,      /*!< Returns the capabilities of
                                             the device. */
    /* Debug. */
    lzCmdDdSet = 0x6065U,               /*!< Sets parts of the data memory. */
    lzCmdDdGet = 0x606AU,               /*!< Reads parts of the data memory. */
    lzCmdDdHash = 0x605AU,              /*!< Gets a hash value from the data memory. */
    lzCmdDdSig = 0x605BU,               /*!< Generates a 3 byte signature of the
                                             non-volatile memory (EROM). */
    /* Wired interface. */
    lzCmdWiConfig = 0x0501U,            /*!< Sets the configuration of the wired
                                             interface. */
    lzCmdWiClear = 0x05FEU,             /*!< Clears all RCI messages in the buffer
                                             to ensure that the external host can
                                             immediately start reading the information
                                             that he is interested in. */
    lzCmdWiStore = 0x1001U,             /*!< Stores the configuration of the wired
                                             interface. */
    lzCmdWiEcho = 0x0100U,              /*!< Replies back with the input from
                                             the wired interface. */
    lzCmdWiEchoNxp = 0x0200U,           /*!< Replies back with the input from
                                             the wired interface with NXP
                                             defined transformations. */
    /* On board measurement. */
    lzCmdObmConf = 0x0801U,             /*!< Configures the ADC in order to perform
                                             a measurement. */
    lzCmdObmRead = 0x0802U,             /*!< Reads out the ADC and returns
                                             the read value. */
    lzCmdObmRssiOfmu = 0x0803U,         /*!< Read the RSSI and the OFMU values
                                             from the hardware. The hardware
                                             must be already set up to perform
                                             those measurements. */
    lzCmdObmSetFreqOffset = 0x0804U,    /*!< Sets the frequency offset. */
    lzCmdObmGetFreqOffset = 0x0805U,    /*!< Gets the frequency offset and the
                                             override enable flag. */
    /* Miscellaneous commands. */
    lzCmdMiscWaitUs = 0x0901U,          /*!< Blocks the device for a specified amount
                                             of microseconds. */
    lzCmdMiscPinToggle = 0x0902U,       /*!< Toggles pin 20 with a configured clock. */
    /* Configuration of Heartbeat event. */
    lzCmdEvTmrHeartbeat = 0x0700U,      /*!< Controls the period of the heartbeat
                                             event. */
    /* Events received from Lizard. */
    lzEventReboot = 0x8000U,            /*!< It is generated when the system starts
                                             and contains information why the system
                                             was reset. */
    lzEventHeartbeat = 0x8100U,         /*!< Heartbeat event generated by Lizard. */
    lzEventRecvFrame = 0x8200U,         /*!< Received frame event. */
    lzEventRfWUPFound = 0x8300U,        /*!< WUP was found. */
    lzEventRfFSYNCFound = 0x8301U,      /*!< FSYNC was found. */
    lzEventRfEOM = 0x8302U,             /*!< End of message. */
    lzEventRfFSYNCduringData = 0x8303U, /*!< FSYNC during DATA. */
    lzEventRfWUPduringFSYNC = 0x8304U,  /*!< WUP during FSYNC. */
    lzEventRfWUPduringDATA = 0x8305U,   /*!< WUP during data. */
    lzEventRfWUPfailed = 0x8381U,       /*!< WUP failed. */
    lzEventRfFSYNCfailed = 0x8382U,     /*!< FSYNC failed. */
    lzEventRfStartupFailed = 0x83F1U,   /*!< RF startup failed. */
    lzEventRfStartupSuccess = 0x83F2U,  /*!< RF startup success. */
    /* Communication error. */
    lzErrorCom = 0xFF00U,               /*!< Communication error. */
    lzCmdUnknown = 0xFFFFU              /*!< Unknown command. */
} lz_cmd_code_t;

/*! @brief Reason of the reboot.
 * It is the Reason byte in the event frame. */
typedef enum
{
    lzRebootEvReasPOR = 0x00U,        /*!< Power was applied and the device
                                           started with the cold boot vector. */
    lzRebootEvReasWDOverflow = 0x01U, /*!< Watchdog overflow event occurred
                                           and the device was reset due to
                                           that reason. */
    lzRebootEvReasBOReset = 0x02U,    /*!< Brownout occurred and the device
                                           was reset due to that reason. */
    lzRebootEvReasSWReset = 0x03U,    /*!< The user initiated reset using
                                           the wired interface. */
    lzRebootEvReasInactReset = 0x04U, /*!< Host was inactive. Reset generated
                                           by RCI when there were many pending
                                           messages that the host did not read. */
    lzRebootEvReasTxBufFull = 0x05U,  /*!< Triggered in case the wired interface
                                           TX buffer is full and would overflow
                                           (not applicable for RCI RX devices). */
    lzRebootEvUnknown = 0xFFU         /*!< Unknown reason code. */
} lz_rb_ev_reason_t;

/*! @brief List of possible command error codes received in response frame
 * (Status byte). */
typedef enum
{
    lzRespErrOk = 0x00U,                /*!< Command passed successfully. */
    /* Command errors. */
    lzRespErrCmdLength = 0xD8U,         /*!< Wrong command length. */
    lzRespErrCmdParam = 0xD9U,          /*!< Some of the parameters of the
                                             command were wrong. */
    lzRespErrCmdHighVoltage = 0xDAU,    /*!< Page write to flash failed. */
    lzRespErrCmdEmptyBuffer = 0xDBU,    /*!< Operated on empty buffer. */
    lzRespErrCmdAccessDenied = 0xDCU,   /*!< Commands is not allowed to be executed. */
    lzRespErrCmdNotAvailable = 0xDDU,   /*!< Command is not available. */
    /* Miscellaneous errors. */
    lzRespErrOsInterrupted = 0xC0U,     /*!< OS_SET_MODE was interrupted. */
    lzRespErrOsRfStartupFailed = 0xC1U, /*!< HW startup procedure failed. */
    /* Communication errors. */
    lzRespErrComIncomplete = 0xF7U,     /*!< The host delivers less than LEN + 1 bytes. */
    lzRespErrComCrc = 0xF8U,            /*!< transferred CRC does not match with
                                             calculated one. */
    lzRespErrComUnknownCmd = 0xF9U,     /*!< Command not recognized by the firmware. */
    lzRespErrComLength = 0xFAU,         /*!< Length byte of 0xFF received. */
    lzRespErrUnknown = 0xFFU            /*!< Unknown error. */
} lz_resp_err_code_t;

/*! @brief List of possible responses from the device (driver internal). */
typedef enum
{
    lzRespTypeCmdReply = 0x00U, /*!< Command reply (success or command error). */
    lzRespTypeEvent,            /*!< Reset, heartbeat or RF event. */
    lzRespTypeComError,         /*!< Communication error. */
    lzRespTypeNoResp,           /*!< No valid data (the length field in the
                                     RCI frame is zero). */
    lzRespTypeUnknown           /*!< Unknown response. */
} lz_resp_type_t;
/*! @} */

/*!
 * @addtogroup lz_rci_h_struct_group
 * Module structures.
 */
/*! @{ */
/*! @brief This structure contains parsed header of a received frame. */
typedef struct
{
    lz_resp_type_t respType;      /*!< Type of a received frame (response, event,
                                       communication error, etc.). */
    lz_cmd_code_t cmd;            /*!< Command and sub-command fields (higher byte
                                       is CMD, lower byte is Sub-CMD). */
    uint16_t timestamp;           /*!< Raw time stamp value. */
    lz_resp_err_code_t respError; /*!< The error code received. This is the
                                       Status field in the RCI frame.*/
} lz_resp_header_t;
/*! @} */

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @addtogroup lz_rci_h_functions_group
 * Module global functions.
 */
/*! @{ */
/*!
 * @brief This function determines type of the received response.
 *
 * It is assumed that valid response is passed.
 *
 * @param response Received response.
 *
 * @return Type of received response (event, reply to a cmd, error).
 */
lz_resp_type_t LZ_RCI_GetRespType(const uint8_t *response);

/*!
 * @brief This function returns a command code.
 *
 * It includes a command code and sub-command code. If the received frame is
 * an event, this is the event code and sub-event code. It is assumed that
 * valid response is passed.
 *
 * @param response Received response.
 *
 * @return Command code.
 */
lz_cmd_code_t LZ_RCI_GetCmdCode(const uint8_t *response);

/*!
 * @brief This function determines reason of a received reboot event.
 *
 * @param rebootEvent A frame containing the reboot event
 *
 * @return Reboot reason (rciRstEventReasonUnknown in case of invalid code).
 */
lz_rb_ev_reason_t LZ_RCI_GetRebootReason(const uint8_t *rebootEvent);

/*!
 * @brief This function processes a received frame. It checks type of the frame,
 * CRC, frame length and error code.
 *
 * @param response   A frame received from the device.
 * @param header     A structure where fields of the frame header are stored.
 * @param payloadLen Pointer where the payload length is stored in bytes.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t LZ_RCI_ProcessHeader(const lz_fr_raw_t *response,
        lz_resp_header_t *header, uint8_t *payloadLen);

/*!
 * @brief This function creates a RCI command frame. The frame contains all
 * required fields and can be send to the device.
 *
 * @param cmd        Command code including sub-command code (if any).
 * @param payload    Pointer to the payload data. It is copied into the frame.
 * @param payloadLen Length of the payload to be copied into the frame.
 * @param buffer     Pointer to the memory where resulting frame is be stored.
 */
void LZ_RCI_CreateFrame(lz_cmd_code_t cmd, const uint8_t *payload,
        uint8_t payloadLen, lz_fr_raw_t *buffer);
/*! @} */

#endif /* SOURCE_LZ_RCI_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
