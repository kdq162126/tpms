/***********************************************************************************************************************
 * This file was generated by the S32 Configuration Tools. Any manual edits made to this file
 * will be overwritten if the respective S32 Configuration Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Peripherals v10.0
processor: S32K144
package_id: S32K144_LQFP100
mcu_data: s32sdk_s32k1xx_rtm_402
processor_version: 0.0.0
functionalGroups:
- name: BOARD_InitPeripherals
  UUID: a6d43dcb-893a-49cd-9d9d-0bb26ff0f05b
  called_from_default_init: true
  selectedCore: core0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/*******************************************************************************
 * Included files 
 ******************************************************************************/
#include "peripherals_flexio_uart_config_1.h"

/*******************************************************************************
 * flexio_uart_config_1 initialization code
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'flexio_uart_config_1'
- type: 'flexio_uart_config'
- mode: 'general'
- custom_name_enabled: 'false'
- type_id: 'flexio_uart'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'FLEXIO'
- config_sets:
  - flexio_uart_driver:
    - flexio_uart_Configuration:
      - 0:
        - name: 'Flexio_uart_Config0_TX'
        - readonly: 'true'
        - driverType: 'FLEXIO_DRIVER_TYPE_INTERRUPTS'
        - baudRate: '115200'
        - bitCount: '8'
        - direction: 'FLEXIO_UART_DIRECTION_TX'
        - dataPin: '1'
        - callback: 'flexio_uart_TX_Callback0'
      - 1:
        - name: 'Flexio_uart_Config0_RX'
        - readonly: 'true'
        - driverType: 'FLEXIO_DRIVER_TYPE_INTERRUPTS'
        - baudRate: '115200'
        - bitCount: '8'
        - direction: 'FLEXIO_UART_DIRECTION_RX'
        - dataPin: '0'
        - callback: 'flexio_uart_RX_Callback0'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.
 * The external variables will be used in other source files in application code.
 *
 */

/* FlexioUART configuration structure */
const flexio_uart_user_config_t Flexio_uart_Config0_TX = {
  .driverType = FLEXIO_DRIVER_TYPE_INTERRUPTS,
  .baudRate = 115200UL,
  .bitCount = 8U,
  .direction = FLEXIO_UART_DIRECTION_TX,
  .dataPin = 1U,
  .callback = flexio_uart_TX_Callback0,
  .callbackParam = NULL
};

const flexio_uart_user_config_t Flexio_uart_Config0_RX = {
  .driverType = FLEXIO_DRIVER_TYPE_INTERRUPTS,
  .baudRate = 115200UL,
  .bitCount = 8U,
  .direction = FLEXIO_UART_DIRECTION_RX,
  .dataPin = 0U,
  .callback = flexio_uart_RX_Callback0,
  .callbackParam = NULL
};


