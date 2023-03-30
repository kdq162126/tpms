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
#include "peripherals_flexio_i2c_config_1.h"

/*******************************************************************************
 * flexio_i2c_config_1 initialization code
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'flexio_i2c_config_1'
- type: 'flexio_i2c_config'
- mode: 'general'
- custom_name_enabled: 'false'
- type_id: 'flexio_i2c'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'FLEXIO'
- config_sets:
  - flexio_i2c_driver:
    - flexio_i2c_Configuration:
      - 0:
        - name: 'Flexio_i2c_Config0'
        - readonly: 'true'
        - slaveAddress: '62'
        - driverType: 'FLEXIO_DRIVER_TYPE_POLLING'
        - baudRate: '400000'
        - sdaPin: '6'
        - sclPin: '7'
        - callback: 'flexio_i2c_Transmit_Callback'
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

/* FlexioI2C configuration structure */
const flexio_i2c_master_user_config_t Flexio_i2c_Config0 = {
  .slaveAddress = 62U,
  .driverType = FLEXIO_DRIVER_TYPE_POLLING,
  .baudRate = 400000UL,
  .sdaPin = 6U,
  .sclPin = 7U,
  .callback = flexio_i2c_Transmit_Callback,
  .callbackParam = NULL
};


