/*
   (c) NXP B.V. 2021, 2022. All rights reserved.

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

#include "device_registers.h"	/* include peripheral declarations S32K144 */
#include "lf/LF.h"
#include "callbacks.h"
#include "lpuart_driver.h"
#include "utils/nxp_console.h"
#include "handle_cmd.h"
#include "sdk_project_config.h"

/* Parsing bytes received */
#define  CTRL_C    0x03
#define  LF        0x0A
#define  CR        0x0D

/* Local macros */
#define CONVERSION_STATUS_OK	0
#define CONVERSION_STATUS_KO	1


/* Declarations */
uint8_t gau8UartRxBuffer[UART_MAX_BUFFER_SIZE] = {0};
uint8_t u8UartBufferIndex = 0;
volatile uint8_t gu8CmdComplete = 0;

/* Local functions */
static uint8_t CMD_TransmitLF (uint8_t *pu8AsciiBuffer);
static uint8_t CMD_AsciiToHex (const uint8_t *pu8Ascii, uint8_t *pu8HexArray, uint8_t u8NbChar);



/************************************************************************************
 * Description: UART callback for reception byte by byte
 * Input: void
 * Output: void
 ************************************************************************************/
void UART_rxCallback(void *driverState, uart_event_t event, void *userData)
{
    /* Unused parameters */
    (void)driverState;
    (void)userData;

    /* Check the event type */
    if (event == UART_EVENT_RX_FULL)
    {
        /* The reception stops when CR is received or the buffer is full */
        if ((gau8UartRxBuffer[u8UartBufferIndex] != CR) && (u8UartBufferIndex != (UART_MAX_BUFFER_SIZE - 1U)))
        {
        	/* Increment buffer index but discard CTRL_C and LF (overwrite them) */
        	if ((CTRL_C != gau8UartRxBuffer[u8UartBufferIndex]) && (LF != gau8UartRxBuffer[u8UartBufferIndex]))
        	{
                /* Update the buffer index */
            	u8UartBufferIndex++;
        	}

        	/* Update rx buffer */
            LPUART_DRV_SetRxBuffer(INST_LPUART_1, &gau8UartRxBuffer[u8UartBufferIndex], 1U);
        }
        else /* Rx interrupts will be disabled in the ISR */
        {
        	gu8CmdComplete = 1;
        }
    }
}

/************************************************************************************
 * Description: enables interrupts related to reception for LPUART1
 * Input: void
 * Output: void
 ************************************************************************************/
void UART_EnableRxInterrupt (void)
{
    LPUART_DRV_ReceiveData(INST_LPUART_1, gau8UartRxBuffer, 1U);
}

/************************************************************************************
 * Description: checks the format of the command received via UART
 * Input: void
 * Output: void
 ************************************************************************************/
void UART_HandleCmd (void)
{
	/* Check if reception ended because buffer was full */
	if (CR != gau8UartRxBuffer[u8UartBufferIndex])
	{
    	PRINTF("Command invalid: too long or does not end with CR \n\r");
	}
	else if (('L' == gau8UartRxBuffer[0]) && ('F' == gau8UartRxBuffer[1]) && (' ' == gau8UartRxBuffer[2]))
	{
		PRINTF("LF transmission request detected \n\r");

		if (CONVERSION_STATUS_OK == CMD_TransmitLF(&gau8UartRxBuffer[3]))
			PRINTF("LF command transmitted \n\r");
		else
			PRINTF("Invalid LF command content (non-hexa bytes) \n\r");
	}
	else
	{
		PRINTF("Command not recognized \n\r");
	}

	gu8CmdComplete = 0;
	u8UartBufferIndex = 0;
}

/************************************************************************************
 * Description: converts the bytes received via UART into hexa bytes and transmits
 * 				the LF command
 * Input: pu8AsciiBuffer, pointer to the first byte of the ascii buffer to convert
 * Output: status of the conversion
 * 			- CONVERSION_STATUS_OK if conversion succeeded and LF cmd could be sent
 * 			- CONVERSION_STATUS_KO if conversion failed and LF cmd was not sent
 ************************************************************************************/
static uint8_t CMD_TransmitLF (uint8_t *pu8AsciiBuffer)
{
	uint8_t u8NbLfChar;
	uint8_t u8Status;

	u8NbLfChar = u8UartBufferIndex + 1 - UART_LF_CMD_OVERHEAD;

	if (CONVERSION_STATUS_OK == CMD_AsciiToHex(pu8AsciiBuffer, gau8LfTxBuffer, u8NbLfChar))
	{
		gu16NbBytesToTransmit = u8NbLfChar/2;
		LF_start();
		LF_wait_end_tx();
		u8Status = CONVERSION_STATUS_OK;
	}
	else
	{
		u8Status = CONVERSION_STATUS_KO;
	}

	return u8Status;
}

/************************************************************************************
 * Description: converts ASCII bytes into hexadecimal
 * Input: *pu8Ascii, pointer to the location where the ASCII bytes are stored
 * 		  *pu8HexArray, pointer to the location where the hexa bytes will be stored
 * 		  u8NbChar, number of ASCII bytes
 * Output: status of the conversion
 * 			- CONVERSION_STATUS_OK if conversion is success
 * 			- CONVERSION_STATUS_KO if conversion failed due to wrong ASCII format
 ************************************************************************************/
static uint8_t CMD_AsciiToHex (const uint8_t *pu8Ascii, uint8_t *pu8HexArray, uint8_t u8NbChar)
{
  uint8_t i;
  uint8_t u8Ascii;
  uint8_t u8Hex;
  uint8_t u8ConversionStatus;

  /* Assume OK and update if necessary */
  u8ConversionStatus = CONVERSION_STATUS_OK;

  /* Two ascii characters make one UINT8: we should have an even number of characters */
  if (u8NbChar % 2 != 0)
	  u8ConversionStatus = CONVERSION_STATUS_KO;

  /* Convert the characters */
  for (i=0; i<(u8NbChar/2); i++)
  {
	  u8Ascii = *pu8Ascii++;

	  /* High nibble */
	  if ((u8Ascii >= '0') && (u8Ascii <= '9'))
		  u8Hex = u8Ascii - '0';
	  else if (((u8Ascii & 0xDF) >= 'A') && ((u8Ascii & 0xDF) <= 'F'))
		  u8Hex = (u8Ascii & 0xDF) - '7';
	  else
	  {
		  u8ConversionStatus = CONVERSION_STATUS_KO;
		  break;
	  }

	  u8Hex = u8Hex * 16; // High nibble

	  u8Ascii = *pu8Ascii++;

	  /* Low nibble */
	  if ((u8Ascii >= '0') && (u8Ascii <= '9'))
		  u8Hex += u8Ascii - '0';
	  else if (((u8Ascii & 0xDF) >= 'A') && ((u8Ascii & 0xDF) <= 'F'))
		  u8Hex += (u8Ascii & 0xDF) - '7';
	  else
	  {
		  u8ConversionStatus = CONVERSION_STATUS_KO;
		  break;
	  }

	  /* Store the result */
	  *pu8HexArray++ = u8Hex;
  }

  return u8ConversionStatus;
}

