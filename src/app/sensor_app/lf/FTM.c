/*
   (c) NXP B.V. 2021-2022. All rights reserved.

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

#include "device_registers.h"
#include "lf/FTM.h"

/* Note that pins are configured in PINS_DRV_Init().
 *
 * Pins definitions
 * ===================================================
 *
 * Pin number        | Function
 * ----------------- |------------------
 * PTD10             | FTM2CH0
 * PTD11			 | FTM2CH1
 */

/* FTM0 clocks the baud rate (3906bps)
 * => One bit time is 256us and we need two interrupts/bit time due to Manchester encoding
 * => overflow every 128us
 */
void FTM0_init(void)
{
	/**
	 * FTM0 Clocking:
	 * ==================================================
	 */
	PCC->PCCn[PCC_FTM0_INDEX] &= ~PCC_PCCn_CGC_MASK; 	/* Ensure clk disabled for config 	*/
	PCC->PCCn[PCC_FTM0_INDEX] |= PCC_PCCn_PCS(0b010)	/* Clock Src=1, 8 MHz SIRCDIV1_CLK  */
                              |  PCC_PCCn_CGC_MASK;  	/* Enable clock for FTM regs 		*/

	/*!
	 * FTM0 Initialization:
	 * ===================================================
	 */
	FTM0->MODE |= FTM_MODE_WPDIS_MASK;  /* Write protect to registers disabled (default) 				*/
	FTM0->SC	=	FTM_SC_TOIE_MASK    /* TOIE (Timer Overflow Interrupt Ena) = 1           			*/
				   |FTM_SC_PS(0);     	/* PS (Prescaler factor) = 0. Prescaler = 1 					*/
										/* CPWMS (Center aligned PWM Select) = 0 (default, up count) 	*/
										/* CLKS (Clock source) = 0 (default, no clock; FTM disabled) 	*/

	FTM0->COMBINE = 0x00000000;	/* FTM mode settings used: DECAPENx, MCOMBINEx, COMBINEx=0  */
	FTM0->POL = 0x00000000;    	/* Polarity for all channels is active high (default) 		*/
	FTM0->MOD = 1024 -1 ;     	/* FTM0 counter final value (used for PWM mode) 			*/
								/* FTM0 Period = MOD-CNTIN+0x0001 = 1024 ctr clks   		*/
}

/* FTM2 clocks the carrier signal (125kHz)
 * => overflow every 8us
 */
void FTM2_init(void)
{
	/**
	 * FTM2 Clocking:
	 * ==================================================
	 */
	PCC->PCCn[PCC_FTM2_INDEX] &= ~PCC_PCCn_CGC_MASK; 	/* Ensure clk disabled for config 	*/
	PCC->PCCn[PCC_FTM2_INDEX] |= PCC_PCCn_PCS(0b010)	/* Clock Src=1, 8 MHz SIRCDIV1_CLK  */
                              |  PCC_PCCn_CGC_MASK;  	/* Enable clock for FTM regs 		*/

	/*!
	 * FTM2 Initialization:
	 * ===================================================
	 */
	FTM2->MODE |= FTM_MODE_WPDIS_MASK;  /* Write protect to registers disabled (default) 				*/
	FTM2->SC	=	FTM_SC_PWMEN0_MASK	/* Enable PWM channel 0 output									*/
					|FTM_SC_PWMEN1_MASK /* Enable PWM channel 1 output									*/
					|FTM_SC_PS(0);     	/* TOIE (Timer Overflow Interrupt Ena) = 0 (default) 			*/
										/* CPWMS (Center aligned PWM Select) = 0 (default, up count) 	*/
										/* CLKS (Clock source) = 0 (default, no clock; FTM disabled) 	*/
										/* PS (Prescaler factor) = 0. Prescaler = 1 					*/

	FTM2->COMBINE = 0x00000000;	/* FTM mode settings used: DECAPENx, MCOMBINEx, COMBINEx=0  */
	FTM2->POL = 0x00000000;    	/* Polarity for all channels is active high (default) 		*/
	FTM2->MOD = 64 -1 ;     	/* FTM2 counter final value (used for PWM mode) 			*/
								/* FTM2 Period = MOD-CNTIN+0x0001 = 64 ctr clks  		    */
}


void FTM2_CH0_CH1_PWM_init(void)
{
	/**
	 * FTM2, Channel 0 and Channel 1 in PWM Mode, but inverted:
	 * ==================================================
	 */
	FTM2->CONTROLS[0].CnSC = FTM_CnSC_MSB_MASK
							|FTM_CnSC_ELSB_MASK;  	/* FTM0 ch1: edge-aligned PWM, low true pulses 		    */
													/* CHIE (Chan Interrupt Ena) = 0 (default) 			    */
													/* MSB:MSA (chan Mode Select)=0b10, Edge Align PWM		*/
													/* ELSB:ELSA (chan Edge/Level Select)=0b10, low true 	*/

	FTM2->CONTROLS[0].CnV =  32;	/* FTM2 ch0 compare value (50% duty cycle) */

	FTM2->CONTROLS[1].CnSC = FTM_CnSC_MSB_MASK
							|FTM_CnSC_ELSA_MASK;  	/* FTM0 ch1: edge-aligned PWM, high true pulses 		*/
													/* CHIE (Chan Interrupt Ena) = 0 (default) 			    */
													/* MSB:MSA (chan Mode Select)=0b10, Edge Align PWM		*/
													/* ELSB:ELSA (chan Edge/Level Select)=0b01, high true 	*/

	FTM2->CONTROLS[1].CnV =  32;	/* FTM2 ch1 compare value (50% duty cycle) */
}

void FTM0_enable_interrupt (void)
{
	FTM0->SC |= FTM_SC_TOIE(1);
}

void FTM0_disable_interrupt (void)
{
	FTM0->SC &= ~FTM_SC_TOIE_MASK;
}

void FTM0_clear_flag (void)
{
	FTM0->SC;
	FTM0->SC &= ~FTM_SC_TOF_MASK;
	FTM0->SC;
}

void FTM0_start_counter (void)
{
	FTM0->SC |= FTM_SC_CLKS(3);
	/* Start FTM0 counter with clk source = external clock (SOSCDIV1_CLK)*/
}

void FTM2_start_counter (void)
{
	FTM2->SC |= FTM_SC_CLKS(3);
	/* Start FTM2 counter with clk source = external clock (SOSCDIV1_CLK)*/
}

void FTM0_stop_counter (void)
{
	FTM0->SC &= ~FTM_SC_CLKS_MASK;
}

void FTM2_stop_counter (void)
{
	FTM2->SC &= ~FTM_SC_CLKS_MASK;
}


