/***********************************************************************************************************************
 * This file was generated by the S32 Config Tools. Any manual edits made to this file
 * will be overwritten if the respective S32 Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef flexio_i2c_config_1_H
#define flexio_i2c_config_1_H

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
#include "flexio_i2c_driver.h"

/*******************************************************************************
 * Definitions 
 ******************************************************************************/
#define INST_FLEXIO_I2C_CONFIG_1  (0U)

/* External declaration of FlexioI2C configuration structure */
extern const flexio_i2c_master_user_config_t Flexio_i2c_Config0;

/* External declaration of user callback function */
extern void flexio_i2c_Transmit_Callback(i2c_master_event_t event, void *userData);



#endif /* flexio_i2c_config_1_H */
