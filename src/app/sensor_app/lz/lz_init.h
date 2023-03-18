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
 * @file  lz_init.h
 * @brief This module contains implementation of the SW driver for NCK2910AHN.
 *        Note that this module is built on the top of RCI module.
 */

#ifndef SOURCE_LZ_INIT_H_
#define SOURCE_LZ_INIT_H_

 /*******************************************************************************
  * Includes
  ******************************************************************************/

#include "lz_com_spi.h"
#include "lz_rci_map.h"
#include "lz_rci.h"
#include "lz_types.h"
#include "sdk_project_config.h"

  /*******************************************************************************
   * API
   ******************************************************************************/

   /*!
    * @addtogroup lz_init_h_functions_group
    * Module global functions.
    */
    /*! @{ */
    /*!
     * @brief Configuration of GPIOs used for the Lizard driver.
     *
     * Note that this function should not be used in SDK S32 when using
     * pin_mux component.
     *
     * @param drvConfig Pointer to driver instance configuration.
     *
     * @return Status result of the function (kStatus_Success on success).
     */
status_t LZ_SetupGPIOs(const lz_drv_config_t* drvConfig);

/*!
 * @brief Configures SPI module used by the Lizard driver.
 *
 * User can either provide required inputs or pass through SDK configuration
 * structure. In former case SDK configuration structure is created automatically,
 * filled with provided inputs and the rest of configuration items uses default
 * values. In the latter case it is passed without any changes directly to the
 * initialization function.
 *
 * @param drvConfig          Pointer to driver instance configuration.
 * @param spiSdkMasterConfig SDK configuration structure.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t LZ_SetupSPI(const lz_drv_config_t* drvConfig,
   const spi_sdk_master_config_t* spiSdkMasterConfig);

/*!
 * @brief Initializes the Lizard driver (invokes the boot routine).
 *
 * This function resets the device (using RST_N pin) and checks if the Reboot
 * event occurred. If it didn't, kStatus_LZ_InitFail is returned.
 *
 * @param drvConfig Pointer to driver instance configuration.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
status_t LZ_Init(const lz_drv_config_t* drvConfig);

/*! @} */

#endif /* SOURCE_LZ_INIT_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
