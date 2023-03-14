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
 * @file wait_aml.h
 *
 * This driver creates abstraction for WAIT functions for S32 SDK and KSDK.
 */

#ifndef SOURCE_WAIT_AML_H_
#define SOURCE_WAIT_AML_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include "../common_aml.h"

#if (SDK_VERSION == SDK_KSDK)
#include "fsl_clock.h"
#elif (SDK_VERSION == SDK_S32)
#include "clock_manager.h"
#include "device_registers.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @addtogroup macro_group
 * @{
 */
#define WAIT_AML_SYSTEM_CLOCK_FREQ   (WAIT_AML_GetSystemClockFreq())

#define WAIT_AML_GET_CYCLES_FOR_MS(ms, freq) (((freq) / 1000U) * (ms))            /*!< Gets needed cycles for specified delay in milliseconds, calculation is based on core clock frequency. */
#define WAIT_AML_GET_CYCLES_FOR_US(us, freq) (((freq) / 1000U) * (us) / 1000U)    /*!< Gets needed cycles for specified delay in microseconds, calculation is based on core clock frequency. */
#define WAIT_AML_GET_CYCLES_FOR_NS(ns, freq) (((freq) / 1000000U) * (ns) / 1000U) /*!< Gets needed cycles for specified delay in nanoseconds, calculation is based on core clock frequency. */
/*! @} */

#if defined(__thumb__) && !defined(__thumb2__) /* Thumb instruction set only */
/*!
 * @brief Waits for exact number of cycles which can be expressed as multiple of 4.
 *
 * MOV - 1 cycle
 * SUB - 1 cycle
 * BNE - 1 cycle or 2 cycles if jump is realized
 *
 * Output list (empty) - which registers are output and how to map them to C code.
 * Input list (Cycles) - which registers are input and how to map them to C code.
 * Clobber list (r0, r1, cc) - which registers might have changed during
 * execution of asm code (compiler will have to reload them).
 *
 * @param Cycles | Number of cycles to wait.
 */
#define WAIT_AML_WAIT_FOR_MUL4_CYCLES(cycles) \
  __asm( \
    "mov r0, %[cycles] \n\t" \
    "0: \n\t"                \
      "sub r0, #4 \n\t"      \
      "nop \n\t"             \
    "bne 0b \n\t"            \
     :                       \
     : [cycles] "r" (cycles) \
     : "r0", "r1", "cc"      \
  )
  
#else /* Thumb2 or A32 instruction set */

/*!
 * @brief Waits for exact number of cycles which can be expressed as multiple of 4.
 *
 * @param Cycles | Number of cycles to wait.
 */
#define WAIT_AML_WAIT_FOR_MUL4_CYCLES(cycles) \
  __asm( \
    "movs r0, %[cycles] \n"  \
    "0: \n"                  \
      "subs r0, r0, #4 \n"   \
      "nop \n\t"             \
    "bne 0b \n"              \
     :                       \
     : [cycles] "r" (cycles) \
     : "r0", "r1", "cc"      \
  )

#endif

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @addtogroup function_group
 * @{
 */ 

/*!
 * @brief Returns system clock frequency.
*/
uint32_t WAIT_AML_GetSystemClockFreq(void);

/*!
 * @brief Waits for specified amount of cycles which is given by 32bit 
 *        value range. Assumption for this function is that target 
 *        architecture is using 32bit general purpose registers.
 * 
 * @param cycles - Number of cycles to wait.
 */
void WAIT_AML_WaitCycles(uint32_t cycles);

/*!
 * @brief Waits for specified amount of seconds.
 *
 * @param delay - Number of seconds to wait.
 */
void WAIT_AML_WaitSec(uint16_t delay);

/*!
 * @brief Waits for specified amount of milliseconds.
 *
 * @param delay - Number of milliseconds to wait.
 */
void WAIT_AML_WaitMs(uint16_t delay);

/*!
 * @brief Waits for specified amount of microseconds.
 *
 * @param delay - Number of microseconds to wait.
 */
void WAIT_AML_WaitUs(uint16_t delay);
/*! @} */

#endif /* SOURCE_WAIT_AML_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
