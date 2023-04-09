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
 * @file  lz_com_spi.h
 * @brief This module contains implementation of the SPI communication
 *        (classic or extended SPI setup).
 */

#ifndef SOURCE_LZ_COM_SPI_H_
#define SOURCE_LZ_COM_SPI_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "lz_types.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/
/*******************************************************************************
* Lizard Freedom board connections to the FRDM-S32K144 freedom board
*
* Pin function  | FRDM-S32K144 pin
* -----------------------------------------------------------------------------
* CLK           | PTB2/LPSPI0_SCK
* MISO          | PTB3/LPSPI0_SIN
* MOSI          | PTB4/LPSPI0_SOUT
* CS_N          | PTB5
* INT_N         | PTD14
* RDY_N         | PTA14
* RST_N         | PTE7
*******************************************************************************/

/* CS GPIO pin */
#define LZ_CS_INST                  instanceE
#define LZ_CS_PIN                   13U


/* INT GPIO pin */
#define LZ_INT_INST                 instanceA
#define LZ_INT_PIN                  6U

/* RDY GPIO pin */
#define LZ_RDY_INST                 instanceA
#define LZ_RDY_PIN                  7U

/* RST GPIO pin */
#define LZ_RST_INST                 instanceA
#define LZ_RST_PIN                  9U

/* LPSPI used by Lizard device (LPSPI 0). */
#define LZ_LPSPI_INST                2U

/* Define to enable or disable the prints of the received messages
 * 0u: Disabled
 * 1u: Enabled */
#define PRINT_SPI_MESSAGES 1u

/*!
 * @addtogroup lz_com_spi_h_enum_group
 * Module enumerations.
 */
/*! @{ */
/*! @brief It defines how to handle the INT_N and RDY_N signals when six wire
 * SPI mode is used. */
typedef enum
{
    lzWaitDis = 0U, /*!< No checking of INT_N or RDY_N signals. */
    lzWaitRDY,      /*!< Waiting for passive level of RDY_N
                         signal before activation of SPI CS is
                         enabled, INT_N is not checked. */
    lzWaitRDY_INT   /*!< Waiting for passive level of RDY_N
                         signal and active level of INT_N signal
                         before activation of SPI CS is enabled. */
} lz_wait_t;

/* Global variable */
extern uint8_t uLzRebootDetected;

/*! @} */

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @addtogroup lz_com_spi_h_functions_group
 * Module global functions.
 */
/*! @{ */
/*!
 * @brief This function waits until a GPIO pin has desired value.
 *
 * @param gpioPin   A GPIO configuration structure.
 * @param pinValExp Desired level of the pin (true - log. 1, false - log. 0).
 * @param tmoutUs   Timeout in number of microseconds.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t LZ_COM_WaitPinLevel(const lz_gpio_sel_t* gpioPin, bool pinValExp,
        uint32_t tmoutUs);

/*!
 * @brief This function checks if the device is ready for transmission. It
 * reads value of the RDY_N pin and returns true if the pin is active.
 *
 * @param drvConfig Pointer to driver instance configuration.
 *
 * @return It returns true when the Lizard device is ready for SPI transmission.
 */
bool LZ_COM_IsDeviceReady(const lz_drv_config_t *drvConfig);

/*!
 * @brief This function checks if there are data to be read from the device. It
 * reads value of the INT_N pin and returns true if the pin is active.
 *
 * @param drvConfig Pointer to driver instance configuration.
 *
 * @return It returns true if the Lizard device has data pending.
 */
bool LZ_COM_HasDeviceDataPending(const lz_drv_config_t *drvConfig);

/*!
 * @brief This function sends data via SPI with use of classic (4-wired) or
 * extended (6-wired) SPI setup.
 *
 * It transfers data in both directions, number bytes is specified in
 * sendFrame->frameLen. The amount of data to be transferred might be adjusted
 * in case that the response length is longer. The sendFrame->frame array must
 * have sufficient length (see LZ_FR_MAX_SIZE_B macro), unused bytes may have
 * any value.
 *
 * There are 4 possible situations regarding the response length:
 *        1. length > recvBufferSize      => the buffer size is not sufficient
 *        2. length == 0xFF               => the device is not ready
 *        3. length == 0x00               => the device has nothing to send
 *        4. length > sendFrame->frameLen => additional bytes will be transferred
 *
 *        Note that if the INT and RDY pins are not utilized this function
 *        behaves just like ordinary SPI but adds some delays in order
 *        to make sure that the transfer will be successful.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param wait      Handling of RDY_N and INT_N pins. It is applied only
 *                  when the six wire SPI mode is used.
 * @param sendFrame A frame to be sent. The user must provide pointer to
 *                  a buffer and data size to be transferred.
 * @param recvFrame A structure where received data are stored. The
 *                  recvFrame->frameLen variable is only used to store actual
 *                  size of received data. The user must provide a buffer
 *                  (recvFrame->frame) with sufficient length (see LZ_FR_MAX_SIZE_B macro).
 * @param recvBufferSize Size of the receive buffer in bytes (recvFrame->frame).
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t LZ_COM_ReadWriteFrame(const lz_drv_config_t *drvConfig, lz_wait_t wait,
        const lz_fr_raw_t *sendFrame, lz_fr_raw_t *recvFrame, uint8_t recvBufferSize);

/*!
 * @brief This function sends data via SPI with use of classic (4-wired) or
 * extended (6-wired) setup.
 *
 * It transfers in both directions the fixed number of bytes specified by
 * parameter sendFrame->frameLen.
 *
 * Note that if the INT and RDY pins are not utilized this function
 * behaves just like ordinary SPI but adds some delays in order
 * to make sure that the transfer will be successful.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param wait      Handling of RDY_N and INT_N pins. It is applied only
 *                  when the six wire SPI mode is used.
 * @param sendFrame A frame to be sent. The user must provide pointer to
 *                  a buffer and data size to be transferred.
 * @param recvFrame A structure where received data are stored. The
 *                  recvFrame->frameLen variable is only used to store actual
 *                  size of received data. The user must provide a buffer
 *                  (recvFrame->frame) with sufficient length (see LZ_FR_MAX_SIZE_B macro).
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t LZ_COM_ReadWriteFrameFixed(const lz_drv_config_t *drvConfig,
        lz_wait_t wait, const lz_fr_raw_t *sendFrame, lz_fr_raw_t *recvFrame);

/*!
 * @brief This function receives a frame via SPI. Note that it sends dummy data
 * (zeros).
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param wait      Handling of RDY_N and INT_N pins. It is applied only
 *                  when the six wire SPI mode is used.
 * @param recvFrame A structure where received data are stored. The
 *                  recvFrame->frameLen variable is only used to store actual
 *                  size of received data. The user must provide a buffer
 *                  (recvFrame->frame) with sufficient length (see LZ_FR_MAX_SIZE_B macro).
 * @param recvBufferSize Size of the receive buffer in bytes (recvFrame->frame).
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t LZ_COM_ReadFrame(const lz_drv_config_t *drvConfig,
        lz_wait_t wait, lz_fr_raw_t *recvFrame, uint8_t recvBufferSize);

/*!
 * @brief This function sends a frame via SPI. Note that it discards received data.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param wait      Handling of RDY_N and INT_N pins. It is applied only
 *                  when the six wire SPI mode is used.
 * @param sendFrame A frame to be sent. The user must provide pointer to
 *                  a buffer and data size to be transferred.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t LZ_COM_WriteFrame(const lz_drv_config_t *drvConfig,
        lz_wait_t wait, const lz_fr_raw_t *sendFrame);

/*!
 * @brief This function sends a frame via SPI and reads a response to the frame.
 *
 * It performs two SPI transfers, one for sending a frame (received data are
 * discarded) and the other for receiving the response (dummy data - zeros -
 * are sent).
 *
 * It uses LZ_COM_ReadWriteFrame for both transmissions.
 *
 * @param drvConfig Pointer to driver instance configuration.
 * @param wait      Handling of RDY_N and INT_N pins. It is applied only
 *                  when the six wire SPI mode is used.
 * @param sendFrame A frame to be sent. The user must provide pointer to
 *                  a buffer and data size to be transferred.
 * @param recvFrame A structure where received data are stored. The
 *                  recvFrame->frameLen variable is only used to store actual
 *                  size of received data. The user must provide a buffer
 *                  (recvFrame->frame) with sufficient length (see LZ_FR_MAX_SIZE_B macro).
 * @param recvBufferSize Size of the receive buffer in bytes (recvFrame->frame).
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t LZ_COM_ReadAfterWrite(const lz_drv_config_t *drvConfig, lz_wait_t wait,
        const lz_fr_raw_t *sendFrame, lz_fr_raw_t *recvFrame, uint8_t recvBufferSize);
/*! @} */

#endif /* SOURCE_LZ_COM_SPI_H_ */
