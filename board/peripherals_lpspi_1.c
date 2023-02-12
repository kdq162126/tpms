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
#include "peripherals_lpspi_1.h"

/*******************************************************************************
 * lpspi_1 initialization code
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'lpspi_1'
- type: 'lpspi_config'
- mode: 'general_soft'
- custom_name_enabled: 'false'
- type_id: 'lpspi'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'LPSPI_0'
- config_sets:
  - lpspi:
    - lpspiMasterConfig:
      - 0:
        - name: 'lpspi_0_MasterConfig0'
        - readOnly: 'true'
        - bitsPerSec: '1000000'
        - whichPcs: 'LPSPI_PCS0'
        - pcsPolarity: 'LPSPI_ACTIVE_HIGH'
        - isPcsContinuous: 'false'
        - bitcount: '8'
        - lpspiSrcClk: '8000000'
        - clkPhase: 'LPSPI_CLOCK_PHASE_1ST_EDGE'
        - clkPolarity: 'LPSPI_SCK_ACTIVE_HIGH'
        - lsbFirst: 'false'
        - transferType: 'LPSPI_USING_INTERRUPTS'
        - rxDMAChannel: '0'
        - txDMAChannel: '0'
        - callback: 'NULL'
        - callbackParam: 'NULL'
    - lpspiSlaveConfig:
      - 0:
        - name: 'lpspi_0_SlaveConfig0'
        - readOnly: 'true'
        - whichPcs: 'LPSPI_PCS0'
        - pcsPolarity: 'LPSPI_ACTIVE_HIGH'
        - bitcount: '8'
        - clkPhase: 'LPSPI_CLOCK_PHASE_1ST_EDGE'
        - clkPolarity: 'LPSPI_SCK_ACTIVE_HIGH'
        - lsbFirst: 'false'
        - transferType: 'LPSPI_USING_INTERRUPTS'
        - rxDMAChannel: '0'
        - txDMAChannel: '0'
        - callback: 'NULL'
        - callbackParam: 'NULL'
    - quick_selection: 'lpspi_default'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global] 
 * Violates MISRA 2012 Required Rule 8.4, A compatible declaration shall be 
 * visible when an object or function with external linkage is defined..
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.
 * The external variables will be used in other source files in application code.
 *
 */

/* Define state structure for current SPI instance */
lpspi_state_t lpspi_1State;

/* LPSPI Master Configurations 0 */

const lpspi_master_config_t lpspi_0_MasterConfig0 = {
  .bitsPerSec = 1000000UL,
  .whichPcs = LPSPI_PCS0,
  .pcsPolarity = LPSPI_ACTIVE_HIGH,
  .isPcsContinuous = false,
  .bitcount = 8U,
  .lpspiSrcClk = 8000000UL,
  .clkPhase = LPSPI_CLOCK_PHASE_1ST_EDGE,
  .clkPolarity = LPSPI_SCK_ACTIVE_HIGH,
  .lsbFirst = false,
  .transferType = LPSPI_USING_INTERRUPTS,
  .rxDMAChannel = 0U,
  .txDMAChannel = 0U,
  .callback = NULL,
  .callbackParam = NULL
};

/* LPSPI Slave Configurations 0 */

const lpspi_slave_config_t lpspi_0_SlaveConfig0 = {
  .whichPcs = LPSPI_PCS0,
  .pcsPolarity = LPSPI_ACTIVE_HIGH,
  .bitcount = 8U,
  .clkPhase = LPSPI_CLOCK_PHASE_1ST_EDGE,
  .clkPolarity = LPSPI_SCK_ACTIVE_HIGH,
  .lsbFirst = false,
  .transferType = LPSPI_USING_INTERRUPTS,
  .rxDMAChannel = 0U,
  .txDMAChannel = 0U,
  .callback = NULL,
  .callbackParam = NULL
};



