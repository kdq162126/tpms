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
#include "peripherals_mpu_config_1.h"

/*******************************************************************************
 * mpu_config_1 initialization code
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'mpu_config_1'
- type: 'mpu'
- mode: 'general'
- custom_name_enabled: 'false'
- type_id: 'mpu'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'MPU'
- config_sets:
  - mpu:
    - MPUConfig:
      - 0:
        - name: 'MPU_Cfg0'
        - readonly: 'true'
        - mpuRegionConfig:
          - 0:
            - regionEnable: 'true'
            - regionName: 'Region 0'
            - startAddr: '0x0'
            - endAddr: '0xFFFFFFFF'
            - masterAccRight: 'AccessRightConfig0'
            - processIdentifier: '0x00'
            - processIdMask: '0x00'
    - mpuMasterAccessRight:
      - 0:
        - accessRightName: 'AccessRightConfig0'
        - MasterAccessRightReadOnly: 'true'
        - mpuAccessRight:
          - 0:
            - master: 'CORE'
            - idTypeSuppervisor: 'MPU_SUPERVISOR'
            - idTypeUser: 'USER_RWX'
            - processIdentifierEnable: 'false'
          - 1:
            - master: 'DEBUGGER'
            - idTypeSuppervisor: 'MPU_SUPERVISOR'
            - idTypeUser: 'USER_RWX'
            - processIdentifierEnable: 'false'
          - 2:
            - master: 'DMA'
            - idTypeSuppervisor: 'MPU_SUPERVISOR'
            - idTypeUser: 'USER_RWX'
            - processIdentifierEnable: 'false'
    - quick_selection: 'defaultConfig'
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

    /* MPU user configuration 0 */
const mpu_user_config_t MPU_Cfg0[] =
{
/*!Region 0 */
       {
          .startAddr = 0x0U,
          .endAddr = 0xFFFFFFFFU,
          .masterAccRight = AccessRightConfig0,
          .processIdentifier = 0x0U,
          .processIdMask = 0x0U
       }
};
    /*! Master access rights configuration 0 */
const mpu_master_access_right_t AccessRightConfig0[] =
{
      /*! CORE */
       {
          .masterNum = FEATURE_MPU_MASTER_CORE,
          .accessRight = MPU_SUPERVISOR_USER_RWX,
          .processIdentifierEnable = false
       },

      /*! DEBUGGER */
       {
          .masterNum = FEATURE_MPU_MASTER_DEBUGGER,
          .accessRight = MPU_SUPERVISOR_USER_RWX,
          .processIdentifierEnable = false
       },

      /*! DMA */
       {
          .masterNum = FEATURE_MPU_MASTER_DMA,
          .accessRight = MPU_SUPERVISOR_USER_RWX,
          .processIdentifierEnable = false
       }
};

