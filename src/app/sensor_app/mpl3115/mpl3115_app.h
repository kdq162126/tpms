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

#ifndef MPL3115_MPL3115_APP_H_
#define MPL3115_MPL3115_APP_H_

#define MODE_POLLING	(1)
#define MODE_INTERRUPT	(2)
#define MODE_ONE_SHOT	(3)

/* Select the demo mode for MPL3115 sensor
 *
 * In Polling mode, the MPL3115 is in Active mode and takes pressure and temperature measurements
 * periodically, at the sample rate. The user checks if new data is ready and reads it by pushing SW1 button.
 *
 * In Interrupt mode, the MPL3115 is in Active mode and takes pressure and temperature measurements
 * periodically, at the sample rate. When new data is ready, an interrupt is triggered and data is read.
 * The user can halt and restart the sensor by pushing SW1 button.
 *
 * In One-Shot mode, the MPL3115 remains in Standby mode and the user triggers temperature and pressure
 * acquisitions by pressing SW1 button.*/

//#define MPL3115_MODE	MODE_POLLING
//#define MPL3115_MODE	MODE_INTERRUPT
#define MPL3115_MODE	MODE_ONE_SHOT

/*********************************************************
 * Global variables
 *********************************************************/
#if (MPL3115_MODE == MODE_INTERRUPT)
extern uint8_t gu8MPL3115_INT2;
#endif

/*********************************************************
 * Public declarations
 *********************************************************/
#if (MPL3115_MODE == MODE_INTERRUPT)
void MPL3115_Cfg_interrupt (void);
#else
void MPL3115_Cfg_no_interrupt (void);
#endif
#if (MPL3115_MODE == MODE_ONE_SHOT)
void MPL3115_One_shot_measurement (void);
#else
void MPL3115_Start (void);
void MPL3115_Stop (void);
void MPL3115_Change_mode (void);
void MPL3115_Check_flag_read_data (void);
#endif

#endif /* MPL3115_MPL3115_APP_H_ */
