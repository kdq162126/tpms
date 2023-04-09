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
#include "pins_driver.h"

/**********************************************************************
 * Interface for the switch buttons present on Lizard shield board FRDM2910ASENS
 * and on S32K144EVB board
 *
 * Switch		S32K GPIO
 * ------------------------------
 * SW1_LZ		PTD3
 * SW2_LZ		PTD5
 * SW2_S32		PTC12
 * SW3_S32		PTC13
 *
 **********************************************************************/


 /**********************************************************************
  * Local macros
  **********************************************************************/
#define SW_INT_LZ_INSTANCE	instanceA
#define SW_S32_INSTANCE		instanceC
#define SW_LZ_PORT_IRQN		PORTA_IRQn
#define SW_S32_PORT_IRQN	PORTC_IRQn
#define SW1_LZ_PIN_NB		(uint8_t)(3)
#define SW2_LZ_PIN_NB		(uint8_t)(5)
#define SW2_S32_PIN_NB		(uint8_t)(12)
#define SW3_S32_PIN_NB		(uint8_t)(13)
#define LZ_INT_PIN_NB		(uint8_t)(6)

  /**********************************************************************
   * Global variables
   **********************************************************************/
uint8_t u8Sw1_LZ_pressed;
uint8_t u8Sw2_LZ_pressed;
uint8_t u8Sw2_S32_pressed;
uint8_t u8Sw3_S32_pressed;
uint8_t u8LzIntSignal;

/**********************************************************************
 * Local declarations
 **********************************************************************/
void vPort_D_ISRHandler(void);
void vPort_C_ISRHandler(void);
void vPort_A_ISRHandler(void);


void GPIO_install_int(void)
{
   /* Install interrupts for GPIOs */

   /* Install LZ Button & INT interrupt handler */
   INT_SYS_InstallHandler(SW_LZ_PORT_IRQN, vPort_A_ISRHandler, (isr_t*)NULL);
   /* Enable LZ Button interrupt handler */
   INT_SYS_EnableIRQ(SW_LZ_PORT_IRQN);

   /* Install S32 Button interrupt handler */
   INT_SYS_InstallHandler(SW_S32_PORT_IRQN, vPort_C_ISRHandler, (isr_t*)NULL);
   /* Enable S32 Button interrupt handler */
   INT_SYS_EnableIRQ(SW_S32_PORT_IRQN);

   INT_SYS_SetPriority(SW_LZ_PORT_IRQN, 2);

   /* Init variables notifying the application that a button was pressed or INT signal occurred */
   u8Sw1_LZ_pressed = 0;
   u8Sw2_LZ_pressed = 0;
   u8Sw2_S32_pressed = 0;
   u8Sw3_S32_pressed = 0;
   u8LzIntSignal = 0;
}

void vPort_D_ISRHandler(void)
{
   uint32_t u32Flags;

   u32Flags = GPIO_AML_GetInterruptFlags(SW_INT_LZ_INSTANCE);

   /* Check if interrupt comes from SW1 */
   if (u32Flags & (1 << SW1_LZ_PIN_NB))
   {
      u8Sw1_LZ_pressed = 1;
      GPIO_AML_ClearInterruptFlags(SW_INT_LZ_INSTANCE, SW1_LZ_PIN_NB);
   }

   /* Check if interrupt comes from SW2 */
   if (u32Flags & (1 << SW2_LZ_PIN_NB))
   {
      u8Sw2_LZ_pressed = 1;
      GPIO_AML_ClearInterruptFlags(SW_INT_LZ_INSTANCE, SW2_LZ_PIN_NB);
   }

   /* Check if interrupt comes from LZ INT pin */
   if (u32Flags & (1 << LZ_INT_PIN_NB))
   {
      u8LzIntSignal = 1;
      GPIO_AML_ClearInterruptFlags(SW_INT_LZ_INSTANCE, LZ_INT_PIN_NB);
   }
}

void vPort_C_ISRHandler(void)
{
   uint32_t u32Flags;

   u32Flags = GPIO_AML_GetInterruptFlags(SW_S32_INSTANCE);

   /* Check if interrupt comes from SW1 */
   if (u32Flags & (1 << SW2_S32_PIN_NB))
   {
      u8Sw2_S32_pressed = 1;
      GPIO_AML_ClearInterruptFlags(SW_S32_INSTANCE, SW2_S32_PIN_NB);
   }

   /* Check if interrupt comes from SW2 */
   if (u32Flags & (1 << SW3_S32_PIN_NB))
   {
      u8Sw3_S32_pressed = 1;
      GPIO_AML_ClearInterruptFlags(SW_S32_INSTANCE, SW3_S32_PIN_NB);
   }
}

void vPort_A_ISRHandler(void)
{
   uint32_t u32Flags;

   u32Flags = GPIO_AML_GetInterruptFlags(SW_INT_LZ_INSTANCE);

   /* Check if interrupt comes from SW1 */
   if (u32Flags & (1 << SW1_LZ_PIN_NB))
   {
      u8Sw1_LZ_pressed = 1;
      GPIO_AML_ClearInterruptFlags(SW_INT_LZ_INSTANCE, SW1_LZ_PIN_NB);
   }

   /* Check if interrupt comes from SW2 */
   if (u32Flags & (1 << SW2_LZ_PIN_NB))
   {
      u8Sw2_LZ_pressed = 1;
      GPIO_AML_ClearInterruptFlags(SW_INT_LZ_INSTANCE, SW2_LZ_PIN_NB);
   }

   /* Check if interrupt comes from LZ INT pin */
   if (u32Flags & (1 << LZ_INT_PIN_NB))
   {
      u8LzIntSignal = 1;
      GPIO_AML_ClearInterruptFlags(SW_INT_LZ_INSTANCE, LZ_INT_PIN_NB);
   }
}

