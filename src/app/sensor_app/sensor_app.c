#include "utils/nxp_console_adapter.h"
#include "utils/nxp_console.h"
#include "pin_mux.h"
#include "gpio_int.h"
#include "RF.h"
#include "sensor_selection.h"
#include "lpuart1.h"

#include "sensor_app.h"
#include "sdk_project_config.h"

void HandleSensorAppTask(void* pv) {
	(void)pv;

	GPIO_install_int();

	/* Init LPUART1 - must be done before Lizard and sensors init
	* Set SDK_DEBUGCONSOLE to 1U to enable debug prints, set it to 0 to disable */
//	(void)DbgConsole_Init(BOARD_DEBUG_UART_BASEADDR, BOARD_DEBUG_UART_TYPE, &lpUartState1, &lpuart_1_InitConfig0);

	/* Lizard driver initialization */
	RF_LZ_init();
	/* Lizard configuration for reception of TPMS frames */
	RF_LZ_config();

	while (true)
	{
		/* Check if we have received an RF frame and display it */
		RF_LZ_check_for_message_received();

		OSIF_TimeDelay(100);
	}
}
