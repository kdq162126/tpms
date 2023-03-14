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
 * @file  nxp_console_adapter.h
 * @brief This module implements adapter functions needed to get running KSDK
 *        debug console code in the S32 SDK environment.
 */
#ifndef _NXP_CONSOLE_ADAPTER_H_
#define _NXP_CONSOLE_ADAPTER_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <string.h>
#include <stdint.h>

#include "lpuart_driver.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

/* Debug console type definition. */
/* Note that this adapter implements only LPUART to be used for debug console !*/
#define DEBUG_CONSOLE_DEVICE_TYPE_NONE 0U   /*!< No debug console.             */
#define DEBUG_CONSOLE_DEVICE_TYPE_UART 1U   /*!< Debug console base on UART.   */
#define DEBUG_CONSOLE_DEVICE_TYPE_LPUART 2U /*!< Debug console base on LPUART. */
#define DEBUG_CONSOLE_DEVICE_TYPE_LPSCI 3U  /*!< Debug console base on LPSCI.  */
#define DEBUG_CONSOLE_DEVICE_TYPE_USBCDC 4U /*!< Debug console base on USBCDC. */

/* Number of LPUART instances on the S32K144. */
#define FSL_FEATURE_SOC_LPUART_COUNT LPUART_INSTANCE_COUNT

/* Timeout to complete blocking transfer. */
#define XFER_TIMEOUT 1000U

/* The LPUART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE DEBUG_CONSOLE_DEVICE_TYPE_LPUART
/* Based address of the LPUART instance used for virtual serial port. */
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART1

/* Definition of types to retain compatibility.  */
typedef long long int64_t;
typedef unsigned long long uint64_t;

/*!
 * @addtogroup fsl_console_adapter_enum_group
 * Module enumerations.
 * @{
 */

 /*! @} */
 
/*!
 * @addtogroup fsl_console_adapter_struct_group
 * Module structures.
 * @{
 */

/*! @} */
 
/*******************************************************************************
 * Global variables
 ******************************************************************************/
/*!
 * @addtogroup fsl_console_adapter_globals_group
 * Module global variables.
 * @{
 */

/*! @} */

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @addtogroup fsl_console_adapter_functions_group
 * Module global functions.
 * @{
 */

/*!
 * @brief Gets instance for LPUART module base pointer.
 *
 * @parma lpuartBase LPUART base pointer.
 *
 * @return Instance matching given base pointer.
 */
uint32_t LPUART_GetInstance(void *lpuartBase);

/*!
 * @brief Implements adapter function for writing data to LPUART.
 *
 * Implements adapter function which translates S32 SDK signature
 * format into KSDK 2.0 signature. It accesses LPUART low level
 * function used for sending data.
 *
 * @param base      LPUART peripheral base address.
 * @param data      Start address of the data to write.
 * @param length    Size of the data to write.
 *
 * @return              None.
 */
void LPUART_WriteBlocking(LPUART_Type *base, const uint8_t *buffer, size_t length);


/*!
 * @brief Implements adapter function for reading data from LPUART.
 *
 * Implements adapter function which translates S32 SDK signature
 * format into KSDK 2.0 signature. It accesses LPUART low level
 * function used for reading data.
 *
 * @param base      LPUART peripheral base address.
 * @param data      Start address of the buffer to store the received data.
 * @param length    Size of the buffer.
 *
 * @return          Whether the reading was successful or not.
 * @retval kStatus_Success          Execution successfully
 * @retval kStatus_Fail             Execution failure
 */
status_t LPUART_ReadBlocking(LPUART_Type *base, uint8_t *buffer, size_t length);

/*! @} */

#endif /* _NXP_CONSOLE_ADAPTER_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
