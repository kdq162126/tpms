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
 * @file  nxp_console_adapter.c
 * @brief This module implements adapter functions needed to get running KSDK
 *        debug console code in the S32 SDK environment.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "nxp_console_adapter.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! Table of base addresses for LPUART instances. */
static LPUART_Type* const g_lpuartBases[] = LPUART_BASE_PTRS;

/*******************************************************************************
 * Code - internal functions
 ******************************************************************************/
 
/*******************************************************************************
 * Code - public functions
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_GetInstance
 * Description   : Gets instance for LPUART module base pointer.
 *
 *END**************************************************************************/
uint32_t LPUART_GetInstance(void *lpuartBase)
{
    DEV_ASSERT(lpuartBase != NULL);

    uint32_t instance;

    LPUART_Type *base = (LPUART_Type *)lpuartBase;

    /* Find the instance index from base address mappings. */
    for (instance = 0U; instance < LPUART_INSTANCE_COUNT; instance++)
    {
        if (g_lpuartBases[instance] == base)
        {
            break;
        }
    }

    DEV_ASSERT(instance < LPUART_INSTANCE_COUNT);

    return instance;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_WriteBlocking
 * Description   : Implements adapter function which translates S32 SDK signature
 *                 format into KSDK 2.0 signature. It accesses LPUART low level
 *                 function used for sending data.
 *
 *END**************************************************************************/
void LPUART_WriteBlocking(LPUART_Type *base, const uint8_t *buffer, size_t length)
{
    /* We don't care about return value. */
//    LPUART_DRV_SendDataBlocking(LPUART_GetInstance(base), buffer, \
//            (uint32_t)length, XFER_TIMEOUT);
	LPUART_DRV_SendDataPolling(LPUART_GetInstance(base), buffer, \
			            (uint32_t)length);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPUART_ReadBlocking
 * Description   : Implements adapter function which translates S32 SDK signature
 *                 format into KSDK 2.0 signature. It accesses LPUART low level
 *                 function used for reading data.
 *
 *END**************************************************************************/
status_t LPUART_ReadBlocking(LPUART_Type *base, uint8_t *buffer, size_t length)
{
    status_t status;

    status = LPUART_DRV_ReceiveDataBlocking(LPUART_GetInstance(base), buffer, \
            (uint32_t)length, XFER_TIMEOUT);

    return (status != STATUS_SUCCESS) ? STATUS_ERROR : STATUS_SUCCESS;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
