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

#include <CDK.h>
#include "gpio_int.h"
#include "sdk_project_config.h"
#include "lz/lz_init.h"
#include "LED_RGB.h"
#include "lz/lz_com_spi.h"
#include "utils/nxp_console_adapter.h"
#include "utils/nxp_console.h"
#include "RF.h"
#include "app.h"

#define MAX_BUFFER_LENGTH            57U

/* Trim settings -- for debug and development purposes only */
#define TRIM_MISC_CFG_ADDR			(0x7EB2)
#define TRIM_AUTO_FLUSH_MASK		(4)
#define TRIM_FALLBACK_TOUT_ADDR		(0x7A1E)
#define TRIM_FALLBACK_TOUT_VALUE	(20) // 20ms

/* Reception type -- for debug and development purposes only */
#define RX_TYPE_CONTINOUS			(0x03)
#define RX_TYPE_POLLING				(0x04)


uint8_t             sendBuffer[MAX_BUFFER_LENGTH];
lz_fr_raw_t         sendFrame = {
        sendBuffer,
        0U
};
uint8_t             recvBuffer[MAX_BUFFER_LENGTH];
lz_fr_raw_t         recvFrame = {
        recvBuffer,
        0U
};

lz_drv_config_t lzDrvConfig;
volatile status_t status;
bool init_ok;
uint8_t u8RxType = RX_TYPE_CONTINOUS; // for development purposes only

/* Local functions */
status_t RF_LZ_Read_Status(const lz_drv_config_t* drvConfig, lz_wait_t wait,
    lz_fr_raw_t* sendFrame, lz_fr_raw_t* recvFrame, uint8_t recvBufferSize);
static status_t RF_LZ_Start_Continuous_Reception(const lz_drv_config_t* drvConfig, lz_wait_t wait,
    lz_fr_raw_t* sendFrame, lz_fr_raw_t* recvFrame, uint8_t recvBufferSize);
static status_t RF_LZ_Start_Polling(const lz_drv_config_t* drvConfig, lz_wait_t wait,
    lz_fr_raw_t* sendFrame, lz_fr_raw_t* recvFrame, uint8_t recvBufferSize);
/* /!\ Below functions are for debug and development purposes only */
static void RF_LZ_Check_Rx_Type(const lz_drv_config_t* drvConfig);
static status_t RF_LZ_Check_Trim(const lz_drv_config_t* drvConfig);
static status_t Read_1Bytes_DD_GET(const lz_drv_config_t* drvConfig, uint16_t u16Address, uint8_t* pu8Data, uint8_t u8PrintInfo);
static status_t Read_2Bytes_DD_GET(const lz_drv_config_t* drvConfig, uint16_t u16Address, uint16_t* pu16Data, uint8_t u8PrintInfo);
static status_t Write_1Bytes_DD_SET(const lz_drv_config_t* drvConfig, uint16_t u16Address, uint8_t u8Data, uint8_t u8PrintInfo);
static status_t Write_2Bytes_DD_SET(const lz_drv_config_t* drvConfig, uint16_t u16Address, uint16_t u16Data, uint8_t u8PrintInfo);


/************************************************************************************
 * Description: initializes the Lizard driver
 * Input: void
 * Output void
 ************************************************************************************/
void RF_LZ_init(void)
{

    /* Lizard driver initialization. */
    /* User GPIOs initialization. */
    /* CS pin. */
    lzDrvConfig.gpioConfig.csPin.gpioInstance = LZ_CS_INST;
    lzDrvConfig.gpioConfig.csPin.gpioPinNumber = LZ_CS_PIN;
    /* INT pin. */
    lzDrvConfig.gpioConfig.intPin.gpioInstance = LZ_INT_INST;
    lzDrvConfig.gpioConfig.intPin.gpioPinNumber = LZ_INT_PIN;
    /* RDY pin. */
    lzDrvConfig.gpioConfig.rdyPin.gpioInstance = LZ_RDY_INST;
    lzDrvConfig.gpioConfig.rdyPin.gpioPinNumber = LZ_RDY_PIN;
    /* RST pin. */
    lzDrvConfig.gpioConfig.rstPin.gpioInstance = LZ_RST_INST;
    lzDrvConfig.gpioConfig.rstPin.gpioPinNumber = LZ_RST_PIN;

    if (LZ_SetupGPIOs(&lzDrvConfig) == kStatus_Success)
    {
        /* The rest of SPI configuration depends on the MCU selection
        *  and must be set by the user. */
        lzDrvConfig.spiConfig.spiInstance = LZ_LPSPI_INST;
        lzDrvConfig.spiConfig.spiMode = lzSpiMode6Wired;
        lzDrvConfig.spiConfig.clkPol = spiClockPolarityActiveLow; /* CPOL = 1. */
        lzDrvConfig.spiConfig.clkPhase = spiClockPhaseFirstEdge;  /* CPHA = 0. */
        // FIXME
        lzDrvConfig.spiConfig.sourceClkHz = lpspiCom1_MasterConfig0.lpspiSrcClk;
        //        lzDrvConfig.spiConfig.sourceClkHz = lpspiCom1_MasterConfig0.lpspiSrcClk;

        lzDrvConfig.spiConfig.baudRate = 2000000U;  /* 2 MHz. */

        if (LZ_SetupSPI(&lzDrvConfig, NULL) == kStatus_Success)
        {
            /* Lizard driver initialization. */
            PRINTF("---------------------------------------------------------\n\r");
            PRINTF("SW version 1.0.0.3.9.20.18!\n\r");
            PRINTF("SPI init OK! Application started!\n\r");
            PRINTF("---------------------------------------------------------\n\r");
            LED_RGB_Off();
            status = LZ_Init(&lzDrvConfig);
        }
        else
        {
            PRINTF("\n\r SPI driver failed after reset \n\r ");
            LED_RGB_Red_On();
        }
    }
    /* End of Lizard driver initialization. */
}

/************************************************************************************
 * Description: configures Lizard to receive the TPMS frames
 * Input: void
 * Output: void
 * Note: select the frequency (315MHz/434MHz) in CDK_tpms.h
 ************************************************************************************/
void RF_LZ_config(void)
{
    init_ok = false; // assume false and update later

    /* First, check the trim settings of the device to make sure that Auto-Flush is enabled
     * and the appropriate fallback timeout duration is configured.
     *  /!\ IMPORTANT: trim configuration must be performed only once in the device lifetime. It
     * is recommended it to perform it at production, and not in the application project.
     * Here it is read and written for development and debug purposes only.
     */
    if (status == kStatus_Success) {
        PRINTF("\n\r-------------------\n\r");
        PRINTF("| CHECKING DEVICE TRIM |\n\r");
        PRINTF("-----------------------\n\n\r");
        PRINTF("\n\r");
        status = RF_LZ_Check_Trim(&lzDrvConfig);
        PRINTF("CHECKING DEVICE TRIM status %d\n\r", status);
    }

    /* Now configure the RF settings */
    PRINTF("\n\r-------------------\n\r");
    PRINTF("| SENDING CDK MESSAGES |\n\r");
    PRINTF("-----------------------\n\n\r");
    PRINTF("\n\r");
    if (status == kStatus_Success) {
        init_ok = true;
        uLzRebootDetected = 0;
        status = CDK_RCI_LOG_Execute(&lzDrvConfig, lzWaitRDY_INT, &sendFrame, &recvFrame, MAX_BUFFER_LENGTH);	// Send all SPI commands from the CDK RCI log
        PRINTF("-------------------------------\n\r");
        PRINTF("|        LZ ROUTINE RUNNING    |\n\r");
        PRINTF("-------------------------------\n\n\r");

        if (status == kStatus_Success) {
            LED_RGB_Blue_On();
        }
        else
        {
            PRINTF("\n\rAtleast one error in response of CDK messages!!! Please check response bytes  \n\r ");
            LED_RGB_Yellow_On();
        }

        /* In this demo, depending on the content of CDK.c file, Continuous Reception or Polling is configured.
         * After a reboot event, the S32K must restart the reception, either Continuous Reception or Polling,
         * depending on what was configured in the config above. In order to know which one was configured,
         * we read the NCK2910 status here and store the information in the variable u8RxType.
         * /!\ This is done for development purposes only. In a final application, the program knows whether
         * Continuous Reception or Polling is configured, so there is no need to read the status here. */
        RF_LZ_Check_Rx_Type(&lzDrvConfig);
    }
    else
    {
        PRINTF("\n\r Reboot event failed after reset, or error while reading/writing the trim \n\r ");
        LED_RGB_Red_On();
    }
}

/************************************************************************************
 * Description: if the initialization was successful, checks if a new frame was
 * 				received, and displays it if this is the case.
 * Input: void
 * Output: void
 ************************************************************************************/
void RF_LZ_check_for_message_received(void)
{
    uint32_t press = 0;
    float presure = 0;
    uint8_t temp = 0;
    float vol = 0.0;
    if (init_ok == true)
    {
        /* An interrupt flag is configured to be set if there is an interrupt on INT PIN
        Due to CDK_RCI_LOG_Execute, it will be set many times and will be cleared after reading SPI response,
        after all the CDK exported messages are sent.
        Now if it sets again, then its due to radio message or event on lizard that needs to be read */
        if (u8LzIntSignal)
        {
            if (LZ_COM_ReadFrame(&lzDrvConfig, lzWaitRDY_INT, &recvFrame, MAX_BUFFER_LENGTH) == kStatus_Success)
            {
                // uint8_t sensorDataHeader[3] = { 0x1f, 0x0f, 0x80 };
                // if (memcmp(recvBuffer, sensorDataHeader, 3) == 0) {
                uint8_t id[8];
                memset(id, 0, 8);
                btox((char*)id, (char*)&recvBuffer[10], 8);

                char postionMapIds[TIRE_NUMBER][8] = { TIRE_LEFT_FRONT_ID, TIRE_RIGHT_FRONT_ID, TIRE_LEFT_BACK_ID , TIRE_RIGHT_BACK_ID };
                if (tpmsApp.lcdDriver.state == LCD_ST_ACTIVE) {
                    Tire* p_tire;
                    for (uint8_t i = 0; i < TIRE_NUMBER; i++) {
                        // p_tire = &tpmsApp.tires[i];
                        // if (memcmp(p_tire->id, id, 8) != 0) {   // Check if ID already exist
                        //     if (tpmsApp.tires[i].state == TIRE_ST_ACTIVE) continue;
                        // }
                        if (memcmp(id, postionMapIds[i], 8) != 0) continue;
                        p_tire = &tpmsApp.tires[i];

                        memcpy(p_tire->id, id, 8);
                        p_tire->press.value = TireGetPressure(recvBuffer[16] * 256 + recvBuffer[17]);
                        SegmentWrite(&p_tire->press, p_tire->press.value / 100);
                        // p_tire->soc = recvBuffer[22] + 122;
                        p_tire->soc = TireGetSoc(recvBuffer[22] + 122);
                        p_tire->temp.value = recvBuffer[23] - 55;
                        SegmentWrite(&p_tire->temp, p_tire->temp.value);
                        p_tire->state = TIRE_ST_ACTIVE;
                        break;
                    }
                }
                // }


                int i = 0;
                PRINTF("Ma ID Lop: 0x");
                for (i = 10; i < 14; i++)
                    PRINTF("%02X", recvBuffer[i]);
                PRINTF("          \n\r");
                ///*
                press = recvBuffer[16] * 256 + recvBuffer[17];
                presure = 0.824 * (float)press + 88.353;
                PRINTF("\n\rAp Suat Lop: %.2fKpa         \n\r", presure);
                vol = (float)(recvBuffer[22]) / 100 + 1.22;
                PRINTF("\n\rDIen ap Pin: %.2fV       \n\r", vol);
                temp = recvBuffer[23] - 55;
                PRINTF("\n\rNhiet Do Lop: %dC       \n\r", temp);
                //PRINTF("\n\r-----------------%d     \n\r", i);
                //PRINTF("Float: %2.2f\n", 3.245);
                //*/


            /* If a reboot event was detected, we need to restart the reception */
                if (1 == uLzRebootDetected)
                {
                    //PRINTF("\n\r 1 == uLzRebootDetected \n\r ");
                    /* Depending on the configuration, restart Continuous Reception or Polling */
                    if (RX_TYPE_POLLING == u8RxType)
                    {
                        if (kStatus_Success == RF_LZ_Start_Polling(&lzDrvConfig, lzWaitRDY_INT, &sendFrame, &recvFrame, MAX_BUFFER_LENGTH))
                        {
                            PRINTF("\n\r Reboot detected, Polling successfully restarted \n\r ");
                        }
                        else
                        {
                            PRINTF("\n\r Reboot detected but Polling could not be restarted \n\r ");
                        }
                    }
                    else /* Continuous Reception */
                    {
                        PRINTF("\n\r else condition\n\r ");
                        if (kStatus_Success == RF_LZ_Start_Continuous_Reception(&lzDrvConfig, lzWaitRDY_INT, &sendFrame, &recvFrame, MAX_BUFFER_LENGTH))
                        {
                            PRINTF("\n\r Reboot detected, Continuous Reception successfully restarted \n\r ");
                        }
                        else
                        {
                            PRINTF("\n\r Reboot detected but Continuous Reception could not be restarted \n\r ");
                        }
                    }

                    uLzRebootDetected = 0;
                }
                else {
                    //PRINTF("\n\r 0 == uLzRebootDetected \n\r ");
                }
            }
            else
            {
                LED_RGB_Red_On();
                //PRINTF("\n\r xxxxxxxxxxxxxxxxxxxxxxxxxxxxx \n\r ");
            }
            LED_RGB_Off();
        }

    } // end init ok
}

/************************************************************************************
 * Description: transmits a command to read the Operating Status
 * Input: parameters for the SPI transfer
 * Output: the status of the command execution
 ************************************************************************************/
status_t RF_LZ_Read_Status(const lz_drv_config_t* drvConfig, lz_wait_t wait,
    lz_fr_raw_t* sendFrame, lz_fr_raw_t* recvFrame, uint8_t recvBufferSize) {

    status_t Status = kStatus_Success;
    status_t retStatus = kStatus_Success;

    uint8_t COMMAND003_OS_STATUS[] = { 0x03,0x06,0x00,0xc3 };
    sendFrame->frameLen = sizeof(COMMAND003_OS_STATUS);
    sendFrame->frame = COMMAND003_OS_STATUS;
    Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
    if (Status != kStatus_Success) { PRINTF("Error in COMMAND3"); retStatus = kStatus_Fail; }

    return retStatus;
}

/************************************************************************************
 * Description: transmits a command to set the Operating Status to continuous
 * 				reception
 * Input: parameters for the SPI transfer
 * Output: the status of the command execution
 ************************************************************************************/
static status_t RF_LZ_Start_Continuous_Reception(const lz_drv_config_t* drvConfig, lz_wait_t wait,
    lz_fr_raw_t* sendFrame, lz_fr_raw_t* recvFrame, uint8_t recvBufferSize) {

    status_t Status = kStatus_Success;
    status_t retStatus = kStatus_Success;

    uint8_t COMMAND014_OS_SET_MODE[] = { 0x05,0x06,0x01,0x03,0x00,0xcd };
    sendFrame->frameLen = sizeof(COMMAND014_OS_SET_MODE);
    sendFrame->frame = COMMAND014_OS_SET_MODE;
    Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
    if (Status != kStatus_Success) { PRINTF("Error in COMMAND14"); retStatus = kStatus_Fail; }

    return retStatus;
}

/************************************************************************************
 * Description: transmits a command to set the Operating Status to polling
 * Input: parameters for the SPI transfer
 * Output: the status of the command execution
 ************************************************************************************/
static status_t RF_LZ_Start_Polling(const lz_drv_config_t* drvConfig, lz_wait_t wait,
    lz_fr_raw_t* sendFrame, lz_fr_raw_t* recvFrame, uint8_t recvBufferSize) {

    status_t Status = kStatus_Success;
    status_t retStatus = kStatus_Success;

    uint8_t COMMAND014_OS_SET_MODE[] = { 0x05,0x06,0x01,0x04,0x00,0xa6 };
    sendFrame->frameLen = sizeof(COMMAND014_OS_SET_MODE);
    sendFrame->frame = COMMAND014_OS_SET_MODE;
    Status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, sendFrame, recvFrame, recvBufferSize);
    if (Status != kStatus_Success) { PRINTF("Error in COMMAND14"); retStatus = kStatus_Fail; }

    return retStatus;
}

/* /!\  /!\  /!\  /!\  /!\  /!\  /!\  /!\  /!\  /!\  /!\  /!\  /!\  /!\  /!\  /!\  /!\  */
/* Note that the functions below are provided for development and debug purposes only   */
/* Trim should be read and written during production and not in the application project */

/************************************************************************************
 * Description: checks if Continuous Reception or Polling is configured
 * Input: pointer to Lizard driver configuration
 * Output: void
 ************************************************************************************/
static void RF_LZ_Check_Rx_Type(const lz_drv_config_t* drvConfig)
{
    /* Assume it is Continuous Reception and update later if necessary */
    u8RxType = RX_TYPE_CONTINOUS;

    /* Read Status */
    if (kStatus_Success == RF_LZ_Read_Status(&lzDrvConfig, lzWaitRDY_INT, &sendFrame, &recvFrame, MAX_BUFFER_LENGTH))
    {
        /* Check if the received message is a success message, and if polling is configured */
        if ((0x00 == recvFrame.frame[5]) && (RX_TYPE_POLLING == recvFrame.frame[6]))
        {
            u8RxType = RX_TYPE_POLLING;
        }
    }
}

/************************************************************************************
 * Description: checks if the trim is configured, and if not, writes it with the
 * appropriate values for this application.
 * Input: pointer to Lizard driver configuration
 * Output: the status of the command execution
 ************************************************************************************/
static status_t RF_LZ_Check_Trim(const lz_drv_config_t* drvConfig)
{
    status_t Status = kStatus_Success;
    status_t retStatus = kStatus_Success;
    uint8_t u8TrimMiscCfg;
    uint16_t u16TrimFallbackTout;

    /* The trim settings are stored in EROM memory, which has a limited number of write
     * cycles. So, write the trim only if necessary i.e. only if it is not already configured.
     * So, first read the trim value, and then perform the write if the actual trim is different
     * from the desired trim */

     /* Read auto-flush setting */
    Status = Read_1Bytes_DD_GET(drvConfig, TRIM_MISC_CFG_ADDR, &u8TrimMiscCfg, 0);

    /* Check value configured and perform write if necessary */
    if (kStatus_Success != Status) {
        retStatus = Status;
    }
    else if (TRIM_AUTO_FLUSH_MASK != (u8TrimMiscCfg & TRIM_AUTO_FLUSH_MASK)) {
        u8TrimMiscCfg |= TRIM_AUTO_FLUSH_MASK; // Enable auto-flush
        retStatus = Write_1Bytes_DD_SET(drvConfig, TRIM_MISC_CFG_ADDR, u8TrimMiscCfg, 0);
    }

    /* Read fallback timeout setting */
    Status = Read_2Bytes_DD_GET(drvConfig, TRIM_FALLBACK_TOUT_ADDR, &u16TrimFallbackTout, 0);

    /* Check value configured and perform write if necessary */
    if (kStatus_Success != Status) {
        retStatus = Status;
    }
    else if ((uint16_t)(TRIM_FALLBACK_TOUT_VALUE) != u16TrimFallbackTout) {
        retStatus |= Write_2Bytes_DD_SET(drvConfig, TRIM_FALLBACK_TOUT_ADDR, TRIM_FALLBACK_TOUT_VALUE, 0);
    }

    return retStatus;
}

/************************************************************************************
 * Description: reads one byte of trim
 * Input:
 * lz_drv_config_t: pointer to Lizard driver configuration
 * u16Address: address to read
 * pu8Data: pointer to variable storing the value read
 * u8PrintInfo: 1 to print transfer info, 0 otherwise
 * Output: the status of the command execution
 ************************************************************************************/
static status_t Read_1Bytes_DD_GET(const lz_drv_config_t* drvConfig, uint16_t u16Address, uint8_t* pu8Data, uint8_t u8PrintInfo)
{
    status_t    status;
    uint8_t     SendBuffer[20];

    /* clear buffer */
    (void)memset((void*)SendBuffer, 0x00U, (size_t)20);

    lz_fr_raw_t SendFrame = {
            SendBuffer,
            1U
    };

    uint8_t     ReceiveBuffer[20];

    /* clear buffer */
    (void)memset((void*)ReceiveBuffer, 0x00U, (size_t)20);

    lz_fr_raw_t ReceiveFrame = {
            ReceiveBuffer,
            1U
    };

    /* Read 1 Bytes, Address 0x0000 */
    uint8_t payload[] = { 0x01, 0x00, 0x00 };
    uint8_t payloadLen = sizeof(payload);

    payload[1] = (uint8_t)(u16Address & 0x00FF);
    payload[2] = (uint8_t)(u16Address >> 8);

    LZ_RCI_CreateFrame(lzCmdDdGet, payload, payloadLen, &SendFrame);

    if (u8PrintInfo)
    {
        uint8_t counter;
        PRINTF("\n\rLen  : %d\n\r", SendFrame.frameLen);
        PRINTF("MOSI : ");
        for (counter = 0; counter < SendFrame.frameLen; counter++) {
            PRINTF("0x%02X ", SendFrame.frame[counter]);
        }
        PRINTF("\n\r");
    }

    status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, &SendFrame, &ReceiveFrame, 255);

    if (status == kStatus_Success)
    {
        uint8_t counter;
        if (u8PrintInfo)
        {
            PRINTF("Len  : %d\n\r", ReceiveFrame.frameLen);
            PRINTF("MISO : ");
            for (counter = 0; counter < ReceiveFrame.frameLen; counter++) {
                PRINTF("0x%02X ", ReceiveFrame.frame[counter]);
            }
            PRINTF("\n\r");
        }
    }
    else
    {
        PRINTF("\n Read frame failed!");
    }

    *pu8Data = ReceiveFrame.frame[6];

    return status;
}

/************************************************************************************
 * Description: reads two bytes of trim
 * Input:
 * lz_drv_config_t: pointer to Lizard driver configuration
 * u16Address: address to read
 * pu16Data: pointer to variable storing the value read
 * u8PrintInfo: 1 to print transfer info, 0 otherwise
 * Output: the status of the command execution
 ************************************************************************************/
static status_t Read_2Bytes_DD_GET(const lz_drv_config_t* drvConfig, uint16_t u16Address, uint16_t* pu16Data, uint8_t u8PrintInfo)
{
    status_t    status;
    uint8_t     SendBuffer[20];

    /* clear buffer */
    (void)memset((void*)SendBuffer, 0x00U, (size_t)20);

    lz_fr_raw_t SendFrame = {
            SendBuffer,
            1U
    };

    uint8_t     ReceiveBuffer[20];

    /* clear buffer */
    (void)memset((void*)ReceiveBuffer, 0x00U, (size_t)20);

    lz_fr_raw_t ReceiveFrame = {
            ReceiveBuffer,
            1U
    };

    /* Read 2 Bytes, Address 0x0000 */
    uint8_t payload[] = { 0x02, 0x00, 0x00 };
    uint8_t payloadLen = sizeof(payload);

    payload[1] = (uint8_t)(u16Address & 0x00FF);
    payload[2] = (uint8_t)(u16Address >> 8);

    LZ_RCI_CreateFrame(lzCmdDdGet, payload, payloadLen, &SendFrame);

    if (u8PrintInfo)
    {
        uint8_t counter;
        PRINTF("\n\rLen  : %d\n\r", SendFrame.frameLen);
        PRINTF("MOSI : ");
        for (counter = 0; counter < SendFrame.frameLen; counter++) {
            PRINTF("0x%02X ", SendFrame.frame[counter]);
        }
        PRINTF("\n\r");
    }

    status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, &SendFrame, &ReceiveFrame, 255);

    if (status == kStatus_Success)
    {
        uint8_t counter;
        if (u8PrintInfo)
        {
            PRINTF("Len  : %d\n\r", ReceiveFrame.frameLen);
            PRINTF("MISO : ");
            for (counter = 0; counter < ReceiveFrame.frameLen; counter++) {
                PRINTF("0x%02X ", ReceiveFrame.frame[counter]);
            }
            PRINTF("\n\r");
        }
    }
    else
    {
        PRINTF("\n Read frame failed!");
    }

    *pu16Data = ReceiveFrame.frame[6];
    pu16Data++;
    *pu16Data = ReceiveFrame.frame[7];

    return status;
}

/************************************************************************************
 * Description: writes one byte of trim
 * Input:
 * lz_drv_config_t: pointer to Lizard driver configuration
 * u16Address: address to write
 * u8Data: value to write
 * u8PrintInfo: 1 to print transfer info, 0 otherwise
 * Output: the status of the command execution
 ************************************************************************************/
static status_t Write_1Bytes_DD_SET(const lz_drv_config_t* drvConfig, uint16_t u16Address, uint8_t u8Data, uint8_t u8PrintInfo)
{
    status_t    status;
    uint8_t     SendBuffer[20];

    /* clear buffer */
    (void)memset((void*)SendBuffer, 0x00U, (size_t)20);

    lz_fr_raw_t SendFrame = {
            SendBuffer,
            1U
    };

    uint8_t     ReceiveBuffer[20];

    /* clear buffer */
    (void)memset((void*)ReceiveBuffer, 0x00U, (size_t)20);

    lz_fr_raw_t ReceiveFrame = {
            ReceiveBuffer,
            1U
    };

    /* Write 1 Bytes, Address 0x0000, Data 0x00 */
    uint8_t payload[] = { 0x01, 0x00, 0x00, 0x00 };
    uint8_t payloadLen = sizeof(payload);

    payload[1] = (uint8_t)(u16Address & 0x00FF);
    payload[2] = (uint8_t)(u16Address >> 8);
    payload[3] = (uint8_t)(u8Data);

    LZ_RCI_CreateFrame(lzCmdDdSet, payload, payloadLen, &SendFrame);

    if (u8PrintInfo)
    {
        uint8_t counter;
        PRINTF("\n\rLen  : %d\n\r", SendFrame.frameLen);
        PRINTF("MOSI : ");
        for (counter = 0; counter < SendFrame.frameLen; counter++) {
            PRINTF("0x%02X ", SendFrame.frame[counter]);
        }
        PRINTF("\n\r");
    }

    status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, &SendFrame, &ReceiveFrame, 255);

    if (status == kStatus_Success)
    {
        uint8_t counter;
        if (u8PrintInfo)
        {
            PRINTF("Len  : %d\n\r", ReceiveFrame.frameLen);
            PRINTF("MISO : ");
            for (counter = 0; counter < ReceiveFrame.frameLen; counter++) {
                PRINTF("0x%02X ", ReceiveFrame.frame[counter]);
            }
            PRINTF("\n\r");
        }
    }
    else
    {
        PRINTF("\n Read frame failed!");
    }

    return status;
}

/************************************************************************************
 * Description: writes two bytes of trim
 * Input:
 * lz_drv_config_t: pointer to Lizard driver configuration
 * u16Address: address to write
 * u16Data: value to write
 * u8PrintInfo: 1 to print transfer info, 0 otherwise
 * Output: the status of the command execution
 ************************************************************************************/
static status_t Write_2Bytes_DD_SET(const lz_drv_config_t* drvConfig, uint16_t u16Address, uint16_t u16Data, uint8_t u8PrintInfo)
{
    status_t    status;
    uint8_t     SendBuffer[20];

    /* clear buffer */
    (void)memset((void*)SendBuffer, 0x00U, (size_t)20);

    lz_fr_raw_t SendFrame = {
            SendBuffer,
            1U
    };

    uint8_t     ReceiveBuffer[20];

    /* clear buffer */
    (void)memset((void*)ReceiveBuffer, 0x00U, (size_t)20);

    lz_fr_raw_t ReceiveFrame = {
            ReceiveBuffer,
            1U
    };

    /* Write 2 Bytes, Address 0x0000, Data 0x0000 */
    uint8_t payload[] = { 0x02, 0x00, 0x00, 0x00, 0x00 };
    uint8_t payloadLen = sizeof(payload);

    payload[1] = (uint8_t)(u16Address & 0x00FF);
    payload[2] = (uint8_t)(u16Address >> 8);
    payload[3] = (uint8_t)(u16Data & 0x00FF);
    payload[4] = (uint8_t)(u16Data >> 8);

    LZ_RCI_CreateFrame(lzCmdDdSet, payload, payloadLen, &SendFrame);

    if (u8PrintInfo)
    {
        uint8_t counter;
        PRINTF("\n\rLen  : %d\n\r", SendFrame.frameLen);
        PRINTF("MOSI : ");
        for (counter = 0; counter < SendFrame.frameLen; counter++) {
            PRINTF("0x%02X ", SendFrame.frame[counter]);
        }
        PRINTF("\n\r");
    }

    status = LZ_COM_ReadAfterWrite(drvConfig, lzWaitRDY_INT, &SendFrame, &ReceiveFrame, 255);

    if (status == kStatus_Success)
    {
        uint8_t counter;
        if (u8PrintInfo)
        {
            PRINTF("Len  : %d\n\r", ReceiveFrame.frameLen);
            PRINTF("MISO : ");
            for (counter = 0; counter < ReceiveFrame.frameLen; counter++) {
                PRINTF("0x%02X ", ReceiveFrame.frame[counter]);
            }
            PRINTF("\n\r");
        }
    }
    else
    {
        PRINTF("\n Read frame failed!");
    }

    return status;
}

