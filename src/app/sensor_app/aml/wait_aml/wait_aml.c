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


/*
 * File: wait_aml.c
 *
 * This driver creates abstraction for WAIT functions for S32 SDK and KSDK.
 */

/*******************************************************************************
* Includes
 ******************************************************************************/
#include "wait_aml.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : WAIT_AML_GetSystemClockFreq
 * Description   : Returns system clock frequency.
 *
 *END**************************************************************************/
uint32_t WAIT_AML_GetSystemClockFreq(void)
{
	uint32_t freq;

#if (SDK_VERSION == SDK_KSDK)
	freq = CLOCK_GetCoreSysClkFreq();
#elif (SDK_VERSION == SDK_S32)
	CLOCK_SYS_GetFreq(CORE_CLOCK, &freq);
#endif

	return freq;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WAIT_AML_WaitCycles
 * Description   : Waits for specified amount of cycles which is given by 32bit 
 *                 value range. Assumption for this function is that target 
 *                 architecture is using 32bit general purpose registers.
 *
 *END**************************************************************************/
void WAIT_AML_WaitCycles(uint32_t cycles)
{
    /* Advance to next multiple of 4. Value 0x04U ensures that the number
     * is not zero. */
    cycles = (cycles & 0xFFFFFFFCU) | 0x04U;

    WAIT_AML_WAIT_FOR_MUL4_CYCLES(cycles);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WAIT_AML_WaitSec
 * Description   : Waits for specified amount of seconds.
 *
 *END**************************************************************************/
void WAIT_AML_WaitSec(uint16_t delay)
{
    for (; delay > 0U; delay--) {
        WAIT_AML_WaitMs(1000U);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WAIT_AML_WaitMs
 * Description   : Waits for specified amount of milliseconds.
 *
 *END**************************************************************************/
void WAIT_AML_WaitMs(uint16_t delay)
{
    uint32_t cycles = (uint32_t) WAIT_AML_GET_CYCLES_FOR_MS(1U, WAIT_AML_SYSTEM_CLOCK_FREQ);

    /* Advance to multiple of 4. */
    cycles = cycles & 0xFFFFFFFCU;

    for (; delay > 0U; delay--) {
        WAIT_AML_WAIT_FOR_MUL4_CYCLES(cycles);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : WAIT_AML_WaitUs
 * Description   : Waits for specified amount of microseconds.
 *
 *END**************************************************************************/
void WAIT_AML_WaitUs(uint16_t delay)
{
//    uint32_t cycles = (uint32_t) WAIT_AML_GET_CYCLES_FOR_US(delay, WAIT_AML_SYSTEM_CLOCK_FREQ);
//
//    /* Advance to next multiple of 4. Value 0x04U ensures that the number
//     * is not zero. */
//    cycles = (cycles & 0xFFFFFFFCU) | 0x04U;
//    WAIT_AML_WAIT_FOR_MUL4_CYCLES(cycles);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
