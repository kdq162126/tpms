/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : clockMan1.h
**     Project     : LZ_S32K144_Demo
**     Processor   : S32K144_100
**     Component   : clock_manager
**     Version     : Component SDK_S32K14x_09, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_S32K14x_09
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-09-04, 12:42, # CodeGen: 0
**     Contents    :
**         CLOCK_SYS_Init                    - status_t CLOCK_SYS_Init(clock_manager_user_config_t const **...
**         CLOCK_SYS_UpdateConfiguration     - status_t CLOCK_SYS_UpdateConfiguration(uint8_t...
**         CLOCK_SYS_SetConfiguration        - status_t CLOCK_SYS_SetConfiguration(clock_manager_user_config_t const * config);
**         CLOCK_SYS_GetCurrentConfiguration - uint8_t CLOCK_SYS_GetCurrentConfiguration(void);
**         CLOCK_SYS_GetErrorCallback        - clock_manager_callback_user_config_t* CLOCK_SYS_GetErrorCallback(void);
**         CLOCK_SYS_GetFreq                 - status_t CLOCK_SYS_GetFreq(clock_names_t clockName,uint32_t * frequency);
**         CLOCK_DRV_Init                    - status_t CLOCK_DRV_Init(clock_manager_user_config_t const * config);
**         CLOCK_DRV_GetFreq                 - status_t CLOCK_DRV_GetFreq(clock_names_t clockName, uint32_t * frequency);
**         CLOCK_DRV_EnablePeripheralClock   - void CLOCK_DRV_EnablePeripheralClock(clock_names_t clockName, const...
**         CLOCK_DRV_DisablePeripheralClock  - void CLOCK_DRV_DisablePeripheralClock(clock_names_t clockName);
**         CLOCK_DRV_SetSystemClock          - status_t CLOCK_DRV_SetSystemClock(const pwr_modes_t * mode, const...
**         CLOCK_DRV_GetSystemClockSource    - void CLOCK_DRV_GetSystemClockSource(sys_clk_config_t *sysClkConfig);
**         CLOCK_DRV_SetIrc                  - status_t CLOCK_DRV_SetIrc(clock_names_t clockName, bool enable, const...
**         CLOCK_DRV_SetXosc                 - status_t CLOCK_DRV_SetXosc(clock_names_t clockName, bool enable, const...
**         CLOCK_DRV_SetPll                  - status_t CLOCK_DRV_SetPll(clock_names_t clockName, bool enable, const...
**         CLOCK_DRV_SetLpo                  - status_t CLOCK_DRV_SetLpo(bool enable);
**
**     Copyright 1997 - 2015 Freescale Semiconductor, Inc.
**     Copyright 2016-2017 NXP
**     All Rights Reserved.
**     
**     THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
**     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
**     OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
**     IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
**     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
**     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
**     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
**     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
**     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
**     THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file clockMan1.h
** @version 01.00
*/
/*!
**  @addtogroup clockMan1_module clockMan1 module documentation
**  @{
*/
#ifndef clockMan1_H
#define clockMan1_H
/* MODULE clockMan1. */

#include <stdbool.h>
#include <stdint.h>

/* Include inherited beans */
#include "Cpu.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, Global macro not referenced.
 * Application or driver example may not use all symbols that are
 * generated by configurations generator.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.11, When an array with external linkage
 * is declared, its size should be explicitly specified.
 * The number of configurations/callbacks can be zero.
 * On the other side C language forbids declaring array of size zero.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.
 * The external variables will be used in other source files in application code.
 *
 */

/*! @brief User configuration structure 0 */
extern clock_manager_user_config_t clockMan1_InitConfig0;

/*! @brief Count of user configuration structures */
#define CLOCK_MANAGER_CONFIG_CNT 1U

/*! @brief Array of pointers to User configuration structures */
extern clock_manager_user_config_t const *_g_clockManConfigsArr[];

/*! @brief User peripheral configuration structure 0 */
extern peripheral_clock_config_t peripheralClockConfig0[];

/*! @brief Count of peripheral clock user configurations */
#define _NUM_OF_PERIPHERAL_CLOCKS_0 12U


/*! @brief Count of user Callbacks */
#define CLOCK_MANAGER_CALLBACK_CNT 0U

/*! @brief Array of User callbacks */
extern clock_manager_callback_user_config_t *g_clockManCallbacksArr[];
#endif
/* ifndef clockMan1_H */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/

