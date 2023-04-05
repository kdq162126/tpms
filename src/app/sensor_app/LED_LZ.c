/*
   (c) NXP B.V. 2022. All rights reserved.

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

#include <CDK.h>
#include "sdk_project_config.h"
#include "aml/gpio_aml.h"

/**********************************************************************
 * Interface for the LEDs present on Lizard shield board FRDM2910ASENS
 * Pins are initialized in PINS_DRV_Init()
 *
 * LED		COLOR		S32K GPIO
 * ------------------------------
 * LED2		GREEN		PTE9
 * LED3		RED			PTE0
 * LED4		BLUE		PTB12
 * LED5		AMBER		PTB0
 *
 **********************************************************************/


/**********************************************************************
 * Local macros
 **********************************************************************/
#define LED_2_3_INSTANCE	instanceE
#define LED_4_5_INSTANCE	instanceB
#define LED_2_PIN_NB		(uint8_t)(9)
#define LED_3_PIN_NB		(uint8_t)(0)
#define LED_4_PIN_NB		(uint8_t)(12)
#define LED_5_PIN_NB		(uint8_t)(0)



void LED2_On (void)
{
	/* Clear output (LED ON) */
	GPIO_AML_ClearOutput(LED_2_3_INSTANCE, LED_2_PIN_NB);
}

void LED3_On (void)
{
	/* Clear output (LED ON) */
	GPIO_AML_ClearOutput(LED_2_3_INSTANCE, LED_3_PIN_NB);
}

void LED4_On (void)
{
	/* Clear output (LED ON) */
	GPIO_AML_ClearOutput(LED_4_5_INSTANCE, LED_4_PIN_NB);
}

void LED5_On (void)
{
	/* Clear output (LED ON) */
	GPIO_AML_ClearOutput(LED_4_5_INSTANCE, LED_5_PIN_NB);
}

void LED2_Off (void)
{
	/* Set output (LED Off) */
	GPIO_AML_SetOutput(LED_2_3_INSTANCE, LED_2_PIN_NB);
}

void LED3_Off (void)
{
	/* Set output (LED Off) */
	GPIO_AML_SetOutput(LED_2_3_INSTANCE, LED_3_PIN_NB);
}

void LED4_Off (void)
{
	/* Set output (LED Off) */
	GPIO_AML_SetOutput(LED_4_5_INSTANCE, LED_4_PIN_NB);
}

void LED5_Off (void)
{
	/* Set output (LED Off) */
	GPIO_AML_SetOutput(LED_4_5_INSTANCE, LED_5_PIN_NB);
}

void LED2_Toggle (void)
{
	/* Toggle output */
	GPIO_AML_ToggleOutput(LED_2_3_INSTANCE, LED_2_PIN_NB);
}

void LED3_Toggle (void)
{
	/* Toggle output */
	GPIO_AML_ToggleOutput(LED_2_3_INSTANCE, LED_3_PIN_NB);
}

void LED4_Toggle (void)
{
	/* Toggle output */
	GPIO_AML_ToggleOutput(LED_4_5_INSTANCE, LED_4_PIN_NB);
}

void LED5_Toggle (void)
{
	/* Toggle output */
	GPIO_AML_ToggleOutput(LED_4_5_INSTANCE, LED_5_PIN_NB);
}

