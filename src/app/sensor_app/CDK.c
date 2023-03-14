/*
   (c) NXP B.V. 2009-2018. All rights reserved.

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

#include <stdio.h>
#include "CDK.h"
#include "utils/nxp_console_adapter.h"
#include "utils/nxp_console.h"

status_t CDK_RCI_LOG_Execute(const lz_drv_config_t *drvConfig, lz_wait_t wait,
        lz_fr_raw_t *sendFrame, lz_fr_raw_t *recvFrame, uint8_t recvBufferSize){

	status_t Status = kStatus_Success;
	status_t retStatus = kStatus_Success;
	uint8_t COMMAND000_OS_SET_MODE[] = {0x05,0x06,0x01,0x06,0x00,0x8c};
	sendFrame->frameLen = sizeof(COMMAND000_OS_SET_MODE);
	sendFrame->frame = COMMAND000_OS_SET_MODE;
	Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
	if(Status!=kStatus_Success) {PRINTF("Error in COMMAND0"); retStatus = kStatus_Fail;}

	uint8_t COMMAND001_PE_VERSIONS[] = {0x03,0x03,0xf0,0x5c};
	sendFrame->frameLen = sizeof(COMMAND001_PE_VERSIONS);
	sendFrame->frame = COMMAND001_PE_VERSIONS;
	Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
	if(Status!=kStatus_Success) {PRINTF("Error in COMMAND1"); retStatus = kStatus_Fail;}

	uint8_t COMMAND002_PE_CAPABILITY[] = {0x03,0x03,0x0f,0xaf};
	sendFrame->frameLen = sizeof(COMMAND002_PE_CAPABILITY);
	sendFrame->frame = COMMAND002_PE_CAPABILITY;
	Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
	if(Status!=kStatus_Success) {PRINTF("Error in COMMAND2"); retStatus = kStatus_Fail;}

	uint8_t COMMAND003_OS_STATUS[] = {0x03,0x06,0x00,0xc3};
	sendFrame->frameLen = sizeof(COMMAND003_OS_STATUS);
	sendFrame->frame = COMMAND003_OS_STATUS;
	Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
	if(Status!=kStatus_Success) {PRINTF("Error in COMMAND3"); retStatus = kStatus_Fail;}

	uint8_t COMMAND004_DC_SET[] = {0x0b,0x71,0xf6,0x00,0x00,0x80,0x74,0x3d,0x35,0x9f,0x00,0x58};
	sendFrame->frameLen = sizeof(COMMAND004_DC_SET);
	sendFrame->frame = COMMAND004_DC_SET;
	Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
	if(Status!=kStatus_Success) {PRINTF("Error in COMMAND4"); retStatus = kStatus_Fail;}

	uint8_t COMMAND005_DC_SET[] = {0x08,0x71,0xf6,0x00,0x01,0xae,0x1d,0x00,0x75};
	sendFrame->frameLen = sizeof(COMMAND005_DC_SET);
	sendFrame->frame = COMMAND005_DC_SET;
	Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
	if(Status!=kStatus_Success) {PRINTF("Error in COMMAND5"); retStatus = kStatus_Fail;}

	uint8_t COMMAND006_DC_SET[] = {0x33,0x71,0xf6,0x00,0x02,0x23,0xae,0x1d,0x03,0x00,0x2b,0xa3,0x00,0x00,0xff,0x00,0x00,0x00,0xef,0x20,0x42,0x05,0x22,0x01,0x8e,0x00,0x22,0x01,0x47,0x08,0x02,0x20,0xb4,0x00,0x2c,0x40,0x07,0x62,0x00,0x11,0xcd,0x40,0x80,0x3b,0xa0,0x1f,0x20,0x2f,0x74,0x00,0x00,0x40};
	sendFrame->frameLen = sizeof(COMMAND006_DC_SET);
	sendFrame->frame = COMMAND006_DC_SET;
	Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
	if(Status!=kStatus_Success) {PRINTF("Error in COMMAND6"); retStatus = kStatus_Fail;}

	uint8_t COMMAND007_DC_SET[] = {0x32,0x71,0xf6,0x00,0x03,0xb0,0xb3,0x2c,0xb0,0x07,0xa6,0x9a,0xb8,0x91,0x4c,0x5b,0x19,0x86,0x00,0xf6,0x48,0x5f,0x31,0x2a,0x3d,0x2d,0x00,0x88,0x05,0x21,0x60,0x33,0x40,0x41,0x80,0x8a,0x10,0x4f,0x06,0x40,0x00,0x00,0x00,0x00,0x00,0x56,0x08,0x00,0x00,0x00,0x5d};
	sendFrame->frameLen = sizeof(COMMAND007_DC_SET);
	sendFrame->frame = COMMAND007_DC_SET;
	Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
	if(Status!=kStatus_Success) {PRINTF("Error in COMMAND7"); retStatus = kStatus_Fail;}

	uint8_t COMMAND008_DC_SET[] = {0x12,0x71,0xf6,0x00,0x04,0x40,0x1f,0x3f,0x04,0x80,0x7b,0xa4,0xb8,0x05,0xff,0x20,0x00,0x00,0x49};
	sendFrame->frameLen = sizeof(COMMAND008_DC_SET);
	sendFrame->frame = COMMAND008_DC_SET;
	Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
	if(Status!=kStatus_Success) {PRINTF("Error in COMMAND8"); retStatus = kStatus_Fail;}

	uint8_t COMMAND009_DC_SET[] = {0x17,0x71,0xf6,0x00,0x05,0x00,0x00,0xaa,0xaa,0xe0,0x60,0x10,0x28,0x00,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0a};
	sendFrame->frameLen = sizeof(COMMAND009_DC_SET);
	sendFrame->frame = COMMAND009_DC_SET;
	Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
	if(Status!=kStatus_Success) {PRINTF("Error in COMMAND9"); retStatus = kStatus_Fail;}

	uint8_t COMMAND010_DC_SET[] = {0x0e,0x71,0xf6,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x53};
	sendFrame->frameLen = sizeof(COMMAND010_DC_SET);
	sendFrame->frame = COMMAND010_DC_SET;
	Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
	if(Status!=kStatus_Success) {PRINTF("Error in COMMAND10"); retStatus = kStatus_Fail;}

	uint8_t COMMAND011_DC_SET[] = {0x0a,0x71,0xf6,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0xfd};
	sendFrame->frameLen = sizeof(COMMAND011_DC_SET);
	sendFrame->frame = COMMAND011_DC_SET;
	Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
	if(Status!=kStatus_Success) {PRINTF("Error in COMMAND11"); retStatus = kStatus_Fail;}

	uint8_t COMMAND012_DC_SET[] = {0x0c,0x71,0xf6,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xd5};
	sendFrame->frameLen = sizeof(COMMAND012_DC_SET);
	sendFrame->frame = COMMAND012_DC_SET;
	Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
	if(Status!=kStatus_Success) {PRINTF("Error in COMMAND12"); retStatus = kStatus_Fail;}

	uint8_t COMMAND013_DC_SET[] = {0x08,0x71,0xf6,0x00,0x09,0x00,0x00,0x00,0x1f};
	sendFrame->frameLen = sizeof(COMMAND013_DC_SET);
	sendFrame->frame = COMMAND013_DC_SET;
	Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
	if(Status!=kStatus_Success) {PRINTF("Error in COMMAND13"); retStatus = kStatus_Fail;}

	uint8_t COMMAND014_OS_SET_MODE[] = {0x05,0x06,0x01,0x03,0x00,0xcd};
	sendFrame->frameLen = sizeof(COMMAND014_OS_SET_MODE);
	sendFrame->frame = COMMAND014_OS_SET_MODE;
	Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
	if(Status!=kStatus_Success) {PRINTF("Error in COMMAND14"); retStatus = kStatus_Fail;}

	return retStatus;
}
