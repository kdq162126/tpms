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
#include "FTM.h"
#include "interrupt_manager.h"
#include "LF.h"


void NVIC_init_IRQs (void);
void FTM0_overflow_isr (void);
void LF_carrier_on (void);
void LF_carrier_off (void);
void LF_init_state_machine(void);
void LF_update_data(void);
void LF_write_data(uint8_t u8CurrentHalfByteTransmitted);
void LF_write_one (void);
void LF_write_zero (void);


#define CARRIER_ON		LF_carrier_on();
#define CARRIER_OFF		LF_carrier_off();

/*
 * Enum
 */
enum State_Machine_Tx {preamble_Tx , pattern_Tx , data_Tx , eof_Tx , interframe_Tx};

/*
 * Private global variables
 */
enum State_Machine_Tx current_Tx_state;
uint16_t gu16NbBytesTransmitted;
uint8_t gu8CurrentHalfByteTransmitted;
uint8_t gu8CurrentBitTransmitted;
uint8_t gu8GoToNextHalfByte;
uint8_t gu8CurrentHalfByteNb;
uint8_t gu8PreamblePieceNb;
uint8_t gu8PatternPieceNb;
uint8_t gu8EofQuarterNb;
uint8_t gu8CurrentHalfBit;
uint8_t gu8LastBitsToTransmit;

/*
 * Public global variables
 */
uint8_t gau8LfTxBuffer[LF_MAX_BUFFER_SIZE];
uint16_t gu16NbBytesToTransmit;

/*
 * Private global variables
 */
volatile uint8_t gu8LfTxEnded;



/************************************************************************************
 * Description: initializes FTM0, FTM2 and GPIOs for LF transmission
 * Input: void
 * Output: void
 * Remark: PTD10 and PTD11 are configured in pin_mux.c
 ************************************************************************************/
void LF_init (void)
{
	FTM0_init();
	FTM2_init();
	FTM2_CH0_CH1_PWM_init();
	NVIC_init_IRQs();
}

/************************************************************************************
 * Description: triggers the start of the transmission by starting FTM0
 * Input: void
 * Output: void
 ************************************************************************************/
void LF_start (void)
{
	LF_init_state_machine();
	current_Tx_state = preamble_Tx;
	FTM0_start_counter();
}

/************************************************************************************
 * Description: generates a 125kHz carrier by starting FTM2
 * Input: void
 * Output: void
 ************************************************************************************/
void LF_carrier_on (void)
{
	FTM2_start_counter();
}

/************************************************************************************
 * Description: stops the carrier generation by stopping FTM2
 * Input: void
 * Output: void
 ************************************************************************************/
void LF_carrier_off (void)
{
	FTM2_stop_counter();
}

/************************************************************************************
 * Description: installs and enables the interrupt vector for FTM0 overflow
 * Input: void
 * Output: void
 ************************************************************************************/
void NVIC_init_IRQs (void)
{
    /* Install handler for the Timer overflow interrupt and enable it */
    INT_SYS_InstallHandler(FTM0_Ovf_Reload_IRQn, &FTM0_overflow_isr, (isr_t*) 0U);
    INT_SYS_EnableIRQ(FTM0_Ovf_Reload_IRQn);
}

/************************************************************************************
 * Description: initializes the LF transmission state machine
 * Input: void
 * Output: void
 ************************************************************************************/
void LF_init_state_machine(void)
{
  gu8CurrentBitTransmitted = 0x00;
  gu8CurrentHalfBit = 0x00;
  gu8GoToNextHalfByte = 0;
  gu8CurrentHalfByteNb = 0x00;
  gu16NbBytesTransmitted = 0;
  gu8CurrentHalfByteTransmitted = (uint8_t)(gau8LfTxBuffer[gu16NbBytesTransmitted] >> 4);
  gu8PreamblePieceNb = 0x00;
  gu8PatternPieceNb = 0x00;
  gu8EofQuarterNb = 0x00;
  current_Tx_state = interframe_Tx;
  gu8LastBitsToTransmit = 0x00;

  gu8LfTxEnded = 0x00;

  CARRIER_OFF;
}

/************************************************************************************
 * Description: keeps the program in a loop while waiting for the end of the LF
 * 				transmission
 * Input: void
 * Output: void
 ************************************************************************************/
void LF_wait_end_tx (void)
{
	while (gu8LfTxEnded == 0);
	FTM0_stop_counter();
}

/************************************************************************************
 * Description: FTM0 interrupt vector. It includes the LF transmission state machine
 * Input: void
 * Output: void
 ************************************************************************************/
void FTM0_overflow_isr (void)
{
	FTM0_clear_flag();

	  switch (current_Tx_state) {

	  case interframe_Tx:
	    // Do nothing
	    break;

	  case preamble_Tx:
	    if (gu8PreamblePieceNb == 31)
	    {
	      // Low state
	      CARRIER_OFF;

	      gu8PreamblePieceNb = 0x00;
	      current_Tx_state = pattern_Tx;

	    } else {

	      if ((gu8PreamblePieceNb & 0x01) == 0x00)
	      {
	        // High state
	        CARRIER_ON;
	      }
	      else
	      {
	        // Low state
	        CARRIER_OFF;
	      }
	      gu8PreamblePieceNb ++;
	    }

	    break;

	  case pattern_Tx: // Implements 9T pattern

	    if (gu8PatternPieceNb < 0x03)
	    {
	      // High state
	      CARRIER_ON;
	      gu8PatternPieceNb ++;

	    } else if (gu8PatternPieceNb < 0x06) {

	      // Low state
	      CARRIER_OFF;
	      gu8PatternPieceNb ++;

	    } else if (gu8PatternPieceNb < 0x07) {

	      // High state
	      CARRIER_ON;
	      gu8PatternPieceNb ++;

	    } else if (gu8PatternPieceNb < 0x08) {

	      // Low state
	      CARRIER_OFF;
	      gu8PatternPieceNb ++;

	    } else if (gu8PatternPieceNb < 0x0A) {

	      // High state
	      CARRIER_ON;
	      gu8PatternPieceNb ++;

	    } else if (gu8PatternPieceNb < 0x0C) {

	      // Low state
	      CARRIER_OFF;
	      gu8PatternPieceNb ++;

	    } else if (gu8PatternPieceNb < 0x0E) {

	      // High state
	      CARRIER_ON;
	      gu8PatternPieceNb ++;

	    } else if (gu8PatternPieceNb < 0x10) {

	      // Low state
	      CARRIER_OFF;
	      gu8PatternPieceNb ++;

	    } else if (gu8PatternPieceNb < 0x11) {

	      // High state
	      CARRIER_ON;
	      gu8PatternPieceNb ++;

	    } else if (gu8PatternPieceNb < 0x12) {

	      // Low state
	      CARRIER_OFF;
	      gu8PatternPieceNb = 0x00;
	      current_Tx_state = data_Tx;
	    }

	    break;

	  case data_Tx:

	    if (gu16NbBytesTransmitted == (gu16NbBytesToTransmit - 1))
	    {
	      if (gu8LastBitsToTransmit == 15)
	      {
	        current_Tx_state = eof_Tx;
	        // High state
	        CARRIER_ON;

	        gu16NbBytesTransmitted = 0;
	      } else {
	        gu8LastBitsToTransmit ++;
	        LF_update_data();
	      }
	    } else {
	      LF_update_data();
	    }

	    break;

	  case eof_Tx:
	    if (gu8EofQuarterNb == 3)
	    {
	      LF_init_state_machine();
	      gu8EofQuarterNb = 0x00;
	      gu8LfTxEnded = 1; // Set after calling LF_init_state_machine();
	      // Low state
	      CARRIER_OFF;

	    } else {
	      // High state
	      CARRIER_ON;
	      gu8EofQuarterNb ++;
	    }
	    break;
	  }
}

/************************************************************************************
 * Description: writes a 0
 * Input: void
 * Output: void
 ************************************************************************************/
void LF_write_zero (void)
{
  if (gu8CurrentHalfBit == 0x00)
  {
    // High state
    CARRIER_ON;

    gu8CurrentHalfBit ++;
  } else {
    // Low state
    CARRIER_OFF;

    gu8CurrentHalfBit = 0x00;

    if (gu8CurrentBitTransmitted == 0x03)
    {
      gu8CurrentBitTransmitted = 0x00;
      gu8GoToNextHalfByte = 1;
    } else {
      gu8CurrentBitTransmitted ++;
    }
  }
}

/************************************************************************************
 * Description: writes a 1
 * Input: void
 * Output: void
 ************************************************************************************/
void LF_write_one (void)
{
  if (gu8CurrentHalfBit == 0x00)
  {
    // Low state
    CARRIER_OFF;

    gu8CurrentHalfBit ++;
  } else {
    // High state
    CARRIER_ON;

    gu8CurrentHalfBit = 0x00;

     if (gu8CurrentBitTransmitted == 0x03)
    {
      gu8CurrentBitTransmitted = 0x00;
      gu8GoToNextHalfByte = 1;
    } else {
      gu8CurrentBitTransmitted ++;
    }
  }
}

/************************************************************************************
 * Description: udpate the Manchester-encoded byte to transmit
 * Input: void
 * Output: void
 ************************************************************************************/
void LF_update_data(void)
{

  if (gu8GoToNextHalfByte == 1) {
    if (gu8CurrentHalfByteNb == 0x00) {
      gu8CurrentHalfByteTransmitted = (uint8_t)(gau8LfTxBuffer[gu16NbBytesTransmitted] & 0x0F);
      gu8CurrentHalfByteNb ++;
    } else {
      gu16NbBytesTransmitted ++;
      gu8CurrentHalfByteTransmitted = (uint8_t)(gau8LfTxBuffer[gu16NbBytesTransmitted] >> 4);
      gu8CurrentHalfByteNb = 0x00;
    }
      gu8GoToNextHalfByte = 0;

  } else {

    // Do nothing

  }

   LF_write_data(gu8CurrentHalfByteTransmitted);

}

/************************************************************************************
 * Description: writes a byte
 * Input: void
 * Output: void
 ************************************************************************************/
void LF_write_data(uint8_t u8CurrentHalfByteTransmitted)
{
  unsigned char hex_word[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    switch (hex_word[u8CurrentHalfByteTransmitted])
    {
      case '0':
        switch (gu8CurrentBitTransmitted) {
          case 0x00: LF_write_zero();
          break;
          case 0x01: LF_write_zero();
          break;
          case 0x02: LF_write_zero();
          break;
          case 0x03: LF_write_zero();
          break;
        }

      break;
      case '1':
        switch (gu8CurrentBitTransmitted) {
          case 0x00: LF_write_zero();
          break;
          case 0x01: LF_write_zero();
          break;
          case 0x02: LF_write_zero();
          break;
          case 0x03: LF_write_one();
          break;
        }
      break;
      case '2':
        switch (gu8CurrentBitTransmitted) {
          case 0x00: LF_write_zero();
          break;
          case 0x01: LF_write_zero();
          break;
          case 0x02: LF_write_one();
          break;
          case 0x03: LF_write_zero();
          break;
        }
      break;
      case '3':
        switch (gu8CurrentBitTransmitted) {
          case 0x00: LF_write_zero();
          break;
          case 0x01: LF_write_zero();
          break;
          case 0x02: LF_write_one();
          break;
          case 0x03: LF_write_one();
          break;
        }
      break;
      case '4':
        switch (gu8CurrentBitTransmitted) {
          case 0x00: LF_write_zero();
          break;
          case 0x01: LF_write_one();
          break;
          case 0x02: LF_write_zero();
          break;
          case 0x03: LF_write_zero();
          break;
        }
      break;
      case '5':
        switch (gu8CurrentBitTransmitted) {
          case 0x00: LF_write_zero();
          break;
          case 0x01: LF_write_one();
          break;
          case 0x02: LF_write_zero();
          break;
          case 0x03: LF_write_one();
          break;
        }
      break;
      case '6':
       switch (gu8CurrentBitTransmitted) {
          case 0x00: LF_write_zero();
          break;
          case 0x01: LF_write_one();
          break;
          case 0x02: LF_write_one();
          break;
          case 0x03: LF_write_zero();
          break;
        }
      break;
      case '7':
        switch (gu8CurrentBitTransmitted) {
          case 0x00: LF_write_zero();
          break;
          case 0x01: LF_write_one();
          break;
          case 0x02: LF_write_one();
          break;
          case 0x03: LF_write_one();
          break;
        }
      break;
      case '8':
       switch (gu8CurrentBitTransmitted) {
          case 0x00: LF_write_one();
          break;
          case 0x01: LF_write_zero();
          break;
          case 0x02: LF_write_zero();
          break;
          case 0x03: LF_write_zero();
          break;
        }
      break;
      case '9':
        switch (gu8CurrentBitTransmitted) {
          case 0x00: LF_write_one();
          break;
          case 0x01: LF_write_zero();
          break;
          case 0x02: LF_write_zero();
          break;
          case 0x03: LF_write_one();
          break;
        }
      break;
      case 'A':
        switch (gu8CurrentBitTransmitted) {
          case 0x00: LF_write_one();
          break;
          case 0x01: LF_write_zero();
          break;
          case 0x02: LF_write_one();
          break;
          case 0x03: LF_write_zero();
          break;
        }
      break;
      case 'B':
        switch (gu8CurrentBitTransmitted) {
          case 0x00: LF_write_one();
          break;
          case 0x01: LF_write_zero();
          break;
          case 0x02: LF_write_one();
          break;
          case 0x03: LF_write_one();
          break;
        }
      break;
      case 'C':
       switch (gu8CurrentBitTransmitted) {
          case 0x00: LF_write_one();
          break;
          case 0x01: LF_write_one();
          break;
          case 0x02: LF_write_zero();
          break;
          case 0x03: LF_write_zero();
          break;
        }
      break;
      case 'D':
        switch (gu8CurrentBitTransmitted) {
          case 0x00: LF_write_one();
          break;
          case 0x01: LF_write_one();
          break;
          case 0x02: LF_write_zero();
          break;
          case 0x03: LF_write_one();
          break;
        }
      break;
      case 'E':
        switch (gu8CurrentBitTransmitted) {
          case 0x00: LF_write_one();
          break;
          case 0x01: LF_write_one();
          break;
          case 0x02: LF_write_one();
          break;
          case 0x03: LF_write_zero();
         break;
        }
      break;
      case 'F':
        switch (gu8CurrentBitTransmitted) {
          case 0x00: LF_write_one();
          break;
          case 0x01: LF_write_one();
          break;
          case 0x02: LF_write_one();
          break;
          case 0x03: LF_write_one();
          break;
        }
      break;
    }
}

