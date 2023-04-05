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
 * @file  lz_types.h
 * @brief This file contains definitions common for driver modules.
 */
#ifndef SOURCE_LZ_TYPES_H_
#define SOURCE_LZ_TYPES_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "../aml/common_aml.h"
#include "../aml/gpio_aml.h"
#include "../aml/spi_aml/spi_aml.h"
#include "../aml/wait_aml/wait_aml.h"

#include <string.h>

/*******************************************************************************
 * Defines
 ******************************************************************************/

/*!
 * @addtogroup lz_types_h_enum_group
 * Module enumerations.
 */
/*! @{ */
/*! @brief Specific error codes for the Lizard driver. */
enum _lz_status_t
{
    /*! Actual size of a received frame does not match value of the Length field
     * in the frame. */
    kStatus_LZ_ComResponseLength = MAKE_STATUS(kStatusGroup_LZ, 0U),
    /*! Number of received bytes is higher than size of receive buffer. */
    kStatus_LZ_ComRecvBufOverflow = MAKE_STATUS(kStatusGroup_LZ, 1U),
    /*! CRC check of received data failed. */
    kStatus_LZ_ComResponseIntegrity = MAKE_STATUS(kStatusGroup_LZ, 2U),
    /*! Unrecognized format of received data. */
    kStatus_LZ_ComResponseFormat = MAKE_STATUS(kStatusGroup_LZ, 3U),
    /*! Communication timeout. Timeout value is defined by macro for each case. */
    kStatus_LZ_ComWaitTimeout = MAKE_STATUS(kStatusGroup_LZ, 4U),
    /*! The device is not ready to receive any data. */
    kStatus_LZ_ComDeviceIsNotReady = MAKE_STATUS(kStatusGroup_LZ, 5U),
    /*! The device does not have any data to send. */
    kStatus_LZ_ComNoResponse = MAKE_STATUS(kStatusGroup_LZ, 6U),
    /*! Initialization of the Lizard device failed. */
    kStatus_LZ_InitFail = MAKE_STATUS(kStatusGroup_LZ, 7U),
    /*! Status code included in RCI response frame indicates an error. */
    kStatus_LZ_RespErr = MAKE_STATUS(kStatusGroup_LZ, 8U),
};

/*! @brief The used SPI mode. */
typedef enum
{
    lzSpiMode4Wired = 0U, /*!< Classic 4 wired SPI. */
    lzSpiMode6Wired       /*!< Extended 6 wired SPI (INT and RDY utilized). */
} lz_spi_mode_t;
/*! @} */
 
/*!
 * @addtogroup lz_types_h_struct_group
 * Module structures.
 */
/*! @{ */
/*! @brief Structure for GPIO selection. */
typedef struct
{
    gpio_aml_instance_t gpioInstance; /*!< Used GPIO instance (port). */
    uint32_t gpioPinNumber;           /*!< Used GPIO pin number. */
} lz_gpio_sel_t;

/*! @brief Structure for SPI configuration. */
typedef struct
{
    aml_instance_t spiInstance;      /*!< Used SPI instance. */
    lz_spi_mode_t spiMode;           /*!< 4 wire or 6 wire SPI mode. */
    spi_aml_clock_polarity_t clkPol; /*!< Clock polarity. */
    spi_aml_clock_phase_t clkPhase;  /*!< Clock phase. */
    uint32_t sourceClkHz;            /*!< Clock source for SPI peripheral. */
    uint32_t baudRate;               /*!< SPI baudrate. */
} lz_spi_config_t;

/*! @brief Structure for GPIO configuration. */
typedef struct
{
    lz_gpio_sel_t csPin;  /*!< GPIO selection for SW chip select. */
    lz_gpio_sel_t intPin; /*!< GPIO selection for INT. */
    lz_gpio_sel_t rdyPin; /*!< GPIO selection for RDY. */
    lz_gpio_sel_t rstPin; /*!< GPIO selection for RST. */
} lz_gpio_config_t;

/*! @brief This data structure is used by the Lizard driver (this is the first
 * parameter of most the Lizard functions). */
typedef struct
{
    lz_gpio_config_t gpioConfig; /*!< GPIO configuration. */
    lz_spi_config_t spiConfig;   /*!< SPI configuration. */
} lz_drv_config_t;

/*! @brief This structure represents raw data of the RCI frame. */
typedef struct
{
    uint8_t *frame;   /*!< Pointer to the raw RCI frame. */
    uint8_t frameLen; /*!< The size of the frame in bytes (including
                           the Length field of RCI frame). */
} lz_fr_raw_t;
/*! @} */
 
#endif /* SOURCE_LZ_TYPES_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
