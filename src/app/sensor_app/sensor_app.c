#include "Cpu.h"

#include "utils/nxp_console_adapter.h"
#include "utils/nxp_console.h"
#include "pin_mux.h"
#include "gpio_int.h"
#include "RF.h"
#include "lf/LF.h"
#include "handle_cmd.h"
#include "sensor_selection.h"
#ifdef USE_MPL3115
#include "mpl3115/mpl3115_app.h"
#endif
#ifdef USE_NCS10131
#include "ncs10131/ncs10131_app.h"
#endif
#include "lpuart1.h"
#include "LED_LZ.h"

#include "sensor_app.h"
#include "sdk_project_config.h"

uint32_t x = 0;

void HandleSensorAppTask(void* pv) {
	(void)pv;

	/* GPIO install interrupts: install interrupts for the switches (buttons), and LZ INT signal */
	GPIO_install_int();

//	LPSPI_DRV_MasterInit(INST_LPSPI_1, &lpspi_1State, &lpspiCom1_MasterConfig0);

	/* Init LPUART1 - must be done before Lizard and sensors init
	 * Set SDK_DEBUGCONSOLE to 1U to enable debug prints, set it to 0 to disable */
	(void)DbgConsole_Init(BOARD_DEBUG_UART_BASEADDR, BOARD_DEBUG_UART_TYPE, &lpUartState1, &lpuart_1_InitConfig0);

	/* Lizard driver initialization */
	RF_LZ_init();
	/* Lizard configuration for reception of TPMS frames */
	RF_LZ_config();

#ifdef USE_MPL3115
	/* Sensor: MPL3115 Init */
#if (MPL3115_MODE == MODE_INTERRUPT)
	/* Configure the sensor, enable interrupts. Set mode to Active.
	 * New sensor data is read when ready.
	 * Change mode (Active <-> Standby) by pushing SW1 button. */
	MPL3115_Cfg_interrupt();
	MPL3115_Start();
#elif (MPL3115_MODE == MODE_POLLING)
	/* Configure the sensor, do not enable interrupts. Set mode to Active.
	 * Check if new sensor data is ready, and read it, by pushing SW1 button. */
	MPL3115_Cfg_no_interrupt();
	MPL3115_Start();
#elif (MPL3115_MODE == MODE_ONE_SHOT)
	/* Configure the sensor, do not enable interrupts. Keep sensor in Standby.
	 * Trigger new sensor data measurement by pushing SW1 button. */
	 //MPL3115_Cfg_no_interrupt();
#endif
#endif

#ifdef USE_NCS10131
	/* Sensor: NCS10131 Init
	 * Sensor data is read when SW2 button is pushed */
	NCS10131_Init();
#endif

	/* FTM0 and FTM2 initialization for LF transmission */
	LF_init();

	/* UART: install the callback for rx events */
	LPUART_DRV_InstallRxCallback(INST_LPUART1, UART_rxCallback, NULL);
	/* Enable command reception */
	UART_EnableRxInterrupt();
	//QueueHandle_t xQueue = NULL;
		//xQueue = xQueueCreate(1, sizeof(unsigned long));

	while (true)
	{
		x++;
		/* Check if we have received an RF frame and display it */
		RF_LZ_check_for_message_received();
		PRINTF("\n\r-----------------%d     \n\r", x);
		OSIF_TimeDelay(2000);
		/* Check if we have received a command via UART and execute it */
		if (1 == gu8CmdComplete)
		{
			UART_HandleCmd();
			UART_EnableRxInterrupt();
		}

#ifdef USE_MPL3115
#if (MPL3115_MODE == MODE_INTERRUPT)

		/* Check MPL3115 INT2 */
		if (gu8MPL3115_INT2)
		{
			gu8MPL3115_INT2 = 0;
			MPL3115_Check_flag_read_data();
		}

#endif
#endif

		/* Handle buttons */
		if (1 == u8Sw1_LZ_pressed)
		{
			u8Sw1_LZ_pressed = 0;
			LED3_Toggle();

#ifdef USE_MPL3115
#if (MPL3115_MODE == MODE_POLLING)
			/* Check if new sensor data is ready, and if so, read it */
			MPL3115_Check_flag_read_data();
#elif (MPL3115_MODE == MODE_INTERRUPT)
			/* Change MPL3115 mode: if currently active, set to Standby, and vice-versa */
			MPL3115_Change_mode();
#elif (MPL3115_MODE == MODE_ONE_SHOT)
			/* Trigger data acquisition once */
			MPL3115_One_shot_measurement();
#endif
#endif
		}

		if (1 == u8Sw2_LZ_pressed)
		{
			u8Sw2_LZ_pressed = 0;
			LED2_Toggle();

#ifdef USE_NCS10131
			/* Read NCS10131 data once */
			NCS10131_Read_data();
#endif
		}

		if (1 == u8Sw3_S32_pressed)
		{
			u8Sw3_S32_pressed = 0;
			LED4_Toggle();
		}

		if (1 == u8Sw2_S32_pressed)
		{
			u8Sw2_S32_pressed = 0;
			LED5_Toggle();
		}

	} /* end while(true) */
}
