/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v10.0
processor: S32K144
package_id: S32K144_LQFP100
mcu_data: s32sdk_s32k1xx_rtm_402
processor_version: 0.0.0
pin_labels:
- {pin_num: '62', pin_signal: PTA17, label: DQ3}
- {pin_num: '82', pin_signal: PTA16, label: DQ2}
- {pin_num: '54', pin_signal: PTB0, label: FLASH_CS}
- {pin_num: '53', pin_signal: PTB1, label: FLASH_MOSI}
- {pin_num: '47', pin_signal: PTB3, label: FLASH_MISO}
- {pin_num: '48', pin_signal: PTB2, label: FLASH_CLK}
- {pin_num: '16', pin_signal: PTB6, label: XTAL}
- {pin_num: '15', pin_signal: PTB7, label: EXTAL}
- {pin_num: '98', pin_signal: PTA4, label: SWDIO}
- {pin_num: '96', pin_signal: PTC4, label: SCLK}
- {pin_num: '72', pin_signal: PTA3, label: BLE_SCL}
- {pin_num: '73', pin_signal: PTA2, label: BLE_SDA}
- {pin_num: '63', pin_signal: PTB17, label: TPMS_CS}
- {pin_num: '64', pin_signal: PTB16, label: TPMS_MOSI}
- {pin_num: '65', pin_signal: PTB15, label: TPMS_MISO}
- {pin_num: '66', pin_signal: PTB14, label: TPMS_CLK}
- {pin_num: '81', pin_signal: PTC6, label: RX_LTE}
- {pin_num: '80', pin_signal: PTC7, label: TX_LTE}
- {pin_num: '55', pin_signal: PTC9, label: LCD_GPIO1}
- {pin_num: '52', pin_signal: PTC10, label: LCD_GPIO2}
- {pin_num: '51', pin_signal: PTC11, label: LCD_GPIO3}
- {pin_num: '50', pin_signal: PTC12, label: LCD_GPIO4}
- {pin_num: '49', pin_signal: PTC13, label: LCD_GPIO5}
- {pin_num: '46', pin_signal: PTC14, label: LCD_GPIO6}
- {pin_num: '45', pin_signal: PTC15, label: LCD_GPIO7}
- {pin_num: '44', pin_signal: PTC16, label: LCD_GPIO8}
- {pin_num: '43', pin_signal: PTC17, label: LCD_GPIO9}
- {pin_num: '32', pin_signal: PTD6, label: LCD_GPIO10}
- {pin_num: '31', pin_signal: PTD7, label: LCD_GPIO11}
- {pin_num: '42', pin_signal: PTD8, label: LCD_GPIO12}
- {pin_num: '41', pin_signal: PTD9, label: LCD_GPIO13}
- {pin_num: '36', pin_signal: PTD10, label: LCD_GPIO14}
- {pin_num: '35', pin_signal: PTD11, label: LCD_GPIO15}
- {pin_num: '34', pin_signal: PTD12, label: LCD_GPIO16}
- {pin_num: '22', pin_signal: PTD15, label: LCD_GPIO17}
- {pin_num: '21', pin_signal: PTD16, label: LCD_GPIO18}
- {pin_num: '20', pin_signal: PTD17, label: LCD_GPIO19}
- {pin_num: '94', pin_signal: PTE0, label: LCD_GPIO20}
- {pin_num: '93', pin_signal: PTE1, label: LCD_GPIO21}
- {pin_num: '85', pin_signal: PTE2, label: LCD_GPIO22}
- {pin_num: '18', pin_signal: PTE3, label: LCD_GPIO23}
- {pin_num: '9', pin_signal: PTE4, label: LCD_SDA}
- {pin_num: '8', pin_signal: PTE5, label: LCD_SCL}
- {pin_num: '84', pin_signal: PTE6, label: LCD_GPIO26}
- {pin_num: '59', pin_signal: PTE7, label: LCD_GPIO27}
- {pin_num: '26', pin_signal: PTE8, label: LCD_GPIO28}
- {pin_num: '23', pin_signal: PTE9, label: LCD_GPIO29}
- {pin_num: '6', pin_signal: PTE10, label: LCD_GPIO30}
- {pin_num: '5', pin_signal: PTE11, label: LCD_GPIO31}
- {pin_num: '19', pin_signal: PTE12, label: LCD_GPIO32}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.
 * The external variables will be used in other source files in application code.
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 11.4, Conversion between a pointer and integer type.
 * The cast is required to initialize a pointer with an unsigned long define, representing an address.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 11.6, Cast from unsigned int to pointer.
 * The cast is required to initialize a pointer with an unsigned long define, representing an address.
 *
 */

#include "pin_mux.h"

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0}
- pin_list:
  - {pin_num: '73', peripheral: LPI2C0, signal: 'sda, sda', pin_signal: PTA2}
  - {pin_num: '72', peripheral: LPI2C0, signal: 'scl, scl', pin_signal: PTA3}
  - {pin_num: '98', peripheral: SWD, signal: dio, pin_signal: PTA4}
  - {pin_num: '96', peripheral: SWD, signal: clk, pin_signal: PTC4}
  - {pin_num: '82', peripheral: PORTA, signal: 'port, 16', pin_signal: PTA16, direction: OUTPUT}
  - {pin_num: '62', peripheral: PORTA, signal: 'port, 17', pin_signal: PTA17, direction: OUTPUT}
  - {pin_num: '54', peripheral: LPSPI0, signal: 'pcs, 0', pin_signal: PTB0, direction: OUTPUT}
  - {pin_num: '53', peripheral: LPSPI0, signal: sout, pin_signal: PTB1, direction: OUTPUT}
  - {pin_num: '47', peripheral: LPSPI0, signal: sin, pin_signal: PTB3, direction: INPUT}
  - {pin_num: '48', peripheral: LPSPI0, signal: 'sck, sck', pin_signal: PTB2, direction: OUTPUT}
  - {pin_num: '16', peripheral: OSC, signal: xtal, pin_signal: PTB6}
  - {pin_num: '15', peripheral: OSC, signal: extal, pin_signal: PTB7}
  - {pin_num: '81', peripheral: LPUART1, signal: rxd, pin_signal: PTC6}
  - {pin_num: '80', peripheral: LPUART1, signal: txd, pin_signal: PTC7, direction: OUTPUT}
  - {pin_num: '79', peripheral: FLEXIO, signal: 'fxio_d, 2', pin_signal: PTA0}
  - {pin_num: '91', peripheral: FLEXIO, signal: 'fxio_d, 1', pin_signal: PTA11}
  - {pin_num: '8', peripheral: FLEXIO, signal: 'fxio_d, 7', pin_signal: PTE5}
  - {pin_num: '9', peripheral: FLEXIO, signal: 'fxio_d, 6', pin_signal: PTE4}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* Generate array of configured pin structures */
pin_settings_config_t g_pin_mux_InitConfigArr0[NUM_OF_CONFIGURED_PINS0] = {
    {
        .base            = PORTA,
        .pinPortIdx      = 0U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_ALT4,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
    {
        .base            = PORTA,
        .pinPortIdx      = 11U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_ALT4,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
    {
        .base            = PORTA,
        .pinPortIdx      = 16U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_AS_GPIO,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = PTA,
        .direction       = GPIO_OUTPUT_DIRECTION,
        .digitalFilter   = false,
        .initValue       = 0U,
    },
    {
        .base            = PORTA,
        .pinPortIdx      = 17U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_AS_GPIO,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = PTA,
        .direction       = GPIO_OUTPUT_DIRECTION,
        .digitalFilter   = false,
        .initValue       = 0U,
    },
    {
        .base            = PORTA,
        .pinPortIdx      = 2U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_ALT3,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
    {
        .base            = PORTA,
        .pinPortIdx      = 3U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_ALT3,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
    {
        .base            = PORTA,
        .pinPortIdx      = 4U,
        .pullConfig      = PORT_INTERNAL_PULL_UP_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_ALT7,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
    {
        .base            = PORTB,
        .pinPortIdx      = 0U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_ALT3,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
    {
        .base            = PORTB,
        .pinPortIdx      = 1U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_ALT3,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
    {
        .base            = PORTB,
        .pinPortIdx      = 2U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_ALT3,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
    {
        .base            = PORTB,
        .pinPortIdx      = 3U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_ALT3,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
    {
        .base            = PORTB,
        .pinPortIdx      = 6U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_PIN_DISABLED,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
    {
        .base            = PORTB,
        .pinPortIdx      = 7U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_PIN_DISABLED,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
    {
        .base            = PORTC,
        .pinPortIdx      = 4U,
        .pullConfig      = PORT_INTERNAL_PULL_DOWN_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_ALT7,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
    {
        .base            = PORTC,
        .pinPortIdx      = 6U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_ALT2,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
    {
        .base            = PORTC,
        .pinPortIdx      = 7U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_ALT2,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
    {
        .base            = PORTE,
        .pinPortIdx      = 4U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_ALT6,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
    {
        .base            = PORTE,
        .pinPortIdx      = 5U,
        .pullConfig      = PORT_INTERNAL_PULL_NOT_ENABLED,
        .driveSelect     = PORT_LOW_DRIVE_STRENGTH,
        .passiveFilter   = false,
        .mux             = PORT_MUX_ALT6,
        .pinLock         = false,
        .intConfig       = PORT_DMA_INT_DISABLED,
        .clearIntFlag    = false,
        .gpioBase        = NULL,
        .digitalFilter   = false,
    },
};
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
