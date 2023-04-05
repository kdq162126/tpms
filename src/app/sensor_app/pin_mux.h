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

#ifndef pin_mux_H
#define pin_mux_H

/* MODULE pin_mux. */
#include "pins_driver.h"

/* Summary of the pins specific to this TPMS application configured in g_pin_mux_InitConfigArr.
 *
 * RESOURCES PRESENT ON THE FRDM2910ASENS
 *
 * *LED
 * PTB0, amber LED: MUX = GPIO, output low (LED ON)
 * PTB12, blue LED: MUX = GPIO, output low (LED ON)
 * PTE0, red LED: MUX = GPIO, output low (LED ON)
 * PTE9, green LED: MUX = GPIO, output low (LED ON)
 *
 * *Switch
 * PTD3, SW1: MUX = GPIO, pull-down enabled, interrupt on rising edge
 * PTD5, SW2: MUX = GPIO, pull-down enabled, interrupt on rising edge
 *
 * *LF
 * PTD10, FTM2CH0: MUX = ALT2
 * PTD11, FTM2CH1: MUX = ALT2
 *
 * *MPL3115 (if enabled in sensor_selection.h)
 * PTA2, I2C: MUX = ALT3
 * PTA3, I2C: MUX = ALT3
 * PTB11, INT1: disabled, not used
 * PTB8, INT2: MUX = GPIO, pull-up enabled, interrupt on falling edge
 *
 * *NCS10131 (if enabled in sensor_selection.h)
 * PTB14, SS_B: MUX = ALT3
 * PTB15, MISO: MUX = ALT3
 * PTB16, MOSI: MUX = ALT3
 * PTB17, SS_B: MUX = ALT3
 *
 * RESOURCES PRESENT ON THE S32K144EVB
 *
 * *RGD LED
 * PTD0, blue: MUX = GPIO, output low (LED ON)
 * PTD15, red: MUX = GPIO, output low (LED ON)
 * PTD16, green: MUX = GPIO, output low (LED ON)
 *
 * *Switch
 * PTC12, SW2: MUX = GPIO, pull-down enabled, interrupt on rising edge
 * PTC13, SW3: MUX = GPIO, pull-down enabled, interrupt on rising edge
 *
 * */

/*! @brief User number of configured pins */
#define NUM_OF_CONFIGURED_PINS 89

/*! @brief User configuration structure */
extern pin_settings_config_t g_pin_mux_InitConfigArr[NUM_OF_CONFIGURED_PINS];

/* END pin_mux. */
#endif /* #ifndef pin_mux_H */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/
