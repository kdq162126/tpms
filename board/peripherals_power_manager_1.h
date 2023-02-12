/***********************************************************************************************************************
 * This file was generated by the S32 Config Tools. Any manual edits made to this file
 * will be overwritten if the respective S32 Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef power_manager_1_H
#define power_manager_1_H

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * The global macro will be used in function call of the module.
 *
 */
/*******************************************************************************
 * Included files 
 ******************************************************************************/
#include "power_manager.h"

/*******************************************************************************
 * Global variables 
 ******************************************************************************/
/*! @brief Count of user configuration structures */
#define POWER_MANAGER_CONFIG_CNT 1U

/*! @brief Array of pointers to User configuration structures */
extern power_manager_user_config_t * powerConfigsArr[1];

/*! @brief User configuration structure power_manager_InitConfig0 */
extern power_manager_user_config_t power_manager_InitConfig0;


/*! @brief Count of user Callbacks */
#define POWER_MANAGER_CALLBACK_CNT 0U
/*! @brief Array of pointers to User defined static Callbacks configuration structures */
extern power_manager_callback_user_config_t * powerStaticCallbacksConfigsArr[1];



#endif /* power_manager_1_H */
