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
 * @file  lz_rci_map.h
 * @brief This file contains mapping of the RCI frames.
 */
#ifndef SOURCE_LZ_RCI_MAP_H_
#define SOURCE_LZ_RCI_MAP_H_


/*******************************************************************************
 * General definitions
 ******************************************************************************/

/*!
 * @name General bit operations */
/*! @{ */

/*!
 * @brief Macro for setting value of the bit given by the mask.
 *
 * @param Msg Message to be modified
 * @param Mask Bit selection in message
 */
#define LZ_SET_BIT_VALUE(Msg, Mask) ((Msg) | (Mask))

/*!
 * @brief Macro for unsetting value of the bit given by the mask.
 *
 * @param Msg Message to be modified
 * @param Mask Bit selection in message
 */
#define LZ_UNSET_BIT_VALUE(Msg, Mask) ((Msg) & ~(Mask))

/*!
 * @brief Macro for getting value of the bit given by the mask.
 *
 * @param Msg Message to be read
 * @param Mask Bit selection in message
 * @param Shift Bit shift in message
 */
#define LZ_GET_BIT_VALUE(Msg, Mask, Shift) (((Msg) & (Mask)) >> (Shift))

/*!
 * @brief Macro for setting value of bits given by the mask.
 *
 * @param Msg Message to be modified
 * @param Mask Bits selection in message
 * @param Shift Bits shift in message
 * @param Val Value to be applied
 * @param Range Admissible range of value
 */
#define LZ_SET_BITS_VALUE(Msg, Mask, Shift, Val, Range) (((Msg) & ~(Mask)) | (((Val) & (Range)) << (Shift)))

/*!
 * @brief Macro for getting value of bits given by the mask.
 *
 * @param Msg Message to be read
 * @param Mask Bits selection in message
 * @param Shift Bits shift in message
 */
#define LZ_GET_BITS_VALUE(Msg, Mask, Shift) (((Msg) & (Mask)) >> (Shift))
/*! @} */

/*! @name Min/max macros */
/* @{ */
#if !defined(LZ_MIN)
#define LZ_MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#if !defined(LZ_MAX)
#define LZ_MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
/* @} */

/*******************************************************************************
 * Defines - general structure
 ******************************************************************************/

/*!
 * @name RCI frame
 * Definitions common for all RCI frames (commands, events, responses, errors).
 */
/*! @{ */
/*! Maximal size of the RCI frame (1B is length byte + 254B data). */
#define LZ_FR_MAX_SIZE_B       0xFFU
/*! Size of the RCI frame header (only the common part). */
#define LZ_FR_HEADER_B         0x03U
/*! Offset of the Length field in the RCI frame. */
#define LZ_FR_LENGTH_OF        0x00U
/*! Offset of the Command/Event code field in the RCI frame. */
#define LZ_FR_CMD_OF           0x01U
/*! Offset of the Sub-command/Event type field in the RCI frame. */
#define LZ_FR_SUBCMD_OF        0x02U
/*! Offset of the Payload field in the RCI frame. */
#define LZ_FR_PAYLOAD_OF       0x03U
/*! Offset of the CRC field from the beginning of the RCI frame
 * (including length byte). */
#define LZ_FR_CRC_OF(length)   ((length) - 1U)
/*! @} */

/*!
 * @name RCI response frame
 * Definitions common for all RCI response frames.
 */
/*! @{ */
/*! Size of the response frame header (only the common part). */
#define LZ_FR_ACK_HEADER_B          0x06U
/*! Offset of the Length field in the RCI response frame. */
#define LZ_FR_ACK_LENGTH_OF         0x00U
/*! Offset of the Command/Event code field in the RCI response frame. */
#define LZ_FR_ACK_CMD_OF            0x01U
/*! Offset of the Sub-command/Event type field in the RCI response frame. */
#define LZ_FR_ACK_SUBCMD_OF         0x02U
/*! Offset of the Timestamp (low byte) field in the RCI response frame. */
#define LZ_FR_ACK_TIMESTAMP_LB_OF   0x03U
/*! Offset of the Timestamp (high byte) field in the RCI response frame. */
#define LZ_FR_ACK_TIMESTAMP_HB_OF   (LZ_FR_ACK_TIMESTAMP_LB_OF + 1U)
/*! Offset of the Status field in the RCI response frame. */
#define LZ_FR_ACK_STATUS_OF         0x05U
/*! Offset of the Payload field in the RCI response frame. */
#define LZ_FR_ACK_PAYLOAD_OF        0x06U

/*!
 * @brief It gets value (2 bytes) of CMD and Sub-CMD fields from a received frame.
 *
 * @param frame pointer to a received frame.
 */
#define LZ_GET_CMD(frame) \
    (((uint16_t)(*((frame) + LZ_FR_ACK_CMD_OF)) << 8U) | \
    (*((frame) + LZ_FR_ACK_SUBCMD_OF)))

/*!
 * @brief It gets value (2 bytes) of time stamp fields from a received frame.
 *
 * @param frame pointer to a received frame.
 */
#define LZ_GET_TIMESTAMP(frame) \
    (((uint16_t)(*((frame) + LZ_FR_ACK_TIMESTAMP_HB_OF)) << 8U) | \
    (*((frame) + LZ_FR_ACK_TIMESTAMP_LB_OF)))
/*! @} */

/*!
 * @name Command IDs
 * The min. and max. value of the command ID.
 */
/*! @{ */
#define LZ_CMD_FIRST_ID             0x0000U
#define LZ_CMD_LAST_ID              0xFFFFU
/*! @} */

/*******************************************************************************
 * Defines - RCI command and event codes
 ******************************************************************************/
/*!
 * @name RCI command and event codes
 *
 * Command and event codes without sub-codes.
 */
/*! @{ */
/*! Reboot event code. */
#define LZ_EV_REBOOT_CODE           0x80U
/*! Heartbeat event code. */
#define LZ_EV_HEARTBEAT_CODE        0x81U
/*! Received frame event code. */
#define LZ_EV_RECV_FRAME_CODE       0x82U
/*! RF event code. */
#define LZ_EV_RF_CODE               0x84U

/*! Communication error code. */
#define LZ_ERR_COM_CODE             0xFFU

/*! Code common for all Personality commands. */
#define LZ_CMD_PE_CODE              0x03U
/*! Code common for all Operating State commands. */
#define LZ_CMD_OS_CODE              0x06U
/*! Code common for all Event handling commands. */
#define LZ_CMD_EV_CODE              0x07U
/*! Code common for all On Board Measurement commands. */
#define LZ_CMD_OBM_CODE             0x08U
/*! Code common for all Miscellaneous commands. */
#define LZ_CMD_MISC_CODE            0x09U
/*! Code common for all Rx Buffer commands. */
#define LZ_CMD_RB_CODE              0x0FU
/*! Code common for all Debug commands. */
#define LZ_CMD_DD_CODE              0x60U
/*! Code common for all Device Configuration commands. */
#define LZ_CMD_DC_CODE              0x71U

/*! WI ECHO command code. */
#define LZ_CMD_WI_ECHO_CODE         0x01U
/*! WI ECHO NXP command code. */
#define LZ_CMD_WI_ECHO_NXP_CODE     0x02U
/*! WI CONFIG command code. */
#define LZ_CMD_WI_CONFIG_CODE       0x05U
/*! WI CLEAR command code. */
#define LZ_CMD_WI_CLEAR_CODE        0x05U
/*! WI STORE command code. */
#define LZ_CMD_WI_STORE_CODE        0x10U
/*! @} */

/*******************************************************************************
 * Defines - Wired interface commands
 ******************************************************************************/

/*!
 * @name WI_ECHO and WI_ECHO_NXP commands
 */
/*! @{ */
/* Size of the WI_ECHO and WI_ECHO_NXP header in bytes. */
#define LZ_WI_ECHO_HEADER_B         2U
/*! Offset of the Payload in the WI_ECHO and WI_ECHO_NXP commands. */
#define LZ_WI_ECHO_PAYLOAD_OF       2U
/*! Max. size of the paylaod in the WI_ECHO and WI_ECHO_NXP commands. */
#define LZ_WI_ECHO_PAYLOAD_MAX_B    252U
/*! @} */

/*******************************************************************************
 * Defines - Events received from Lizard
 ******************************************************************************/

/*!
 * @name Reboot event
 */
/*! @{ */
/*! Size of the Reboot event (includes byte 0). */
#define LZ_EV_REBOOT_B              8U

/*! Offset of the Reason field in the Reboot event frame. */
#define LZ_EV_REBOOT_REASON_OF      LZ_FR_ACK_PAYLOAD_OF
/*! @} */

#endif /* SOURCE_LZ_RCI_MAP_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
