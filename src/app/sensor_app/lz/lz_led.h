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


#include "../aml/common_aml.h"
/*******************************************************************************
 * Defines
 ******************************************************************************/

#ifndef LZ_LZ_LED_H_
#define LZ_LZ_LED_H_

/* Red LED pin */
#define LZ_RED_PIN                  15U

/* Green LED pin */
#define LZ_GREEN_PIN                16U

/* Blue LED pin */
#define LZ_BLUE_PIN                 0U

/* LPED port used on S32k board. */
#define LZ_LPLED_INST                instanceD

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @brief This function initialise the LEDs
 *
 */
void LZ_LEDInit_off(aml_instance_t instance);

/*!
 * @brief This function turns the red LED ON
 *
 */
void LZ_LED_RedOn(aml_instance_t instance);
/*!
 * @brief This function turns the green LED ON
 *
 */
void LZ_LED_GreenOn(aml_instance_t instance);
/*!
 * @brief This function turns the blue LED ON
 *
 */
void LZ_LED_BlueOn(aml_instance_t instance);

/*!
 * @brief This function turns the yellow LED ON
 *
 */
void LZ_LED_YellowOn(aml_instance_t instance);

#endif /* LZ_LZ_LED_H_ */
