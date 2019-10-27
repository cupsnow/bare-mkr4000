/** @author joelai */

#ifndef _H_TEST3_BOARD
#define _H_TEST3_BOARD

/** @defgroup TEST3_BOARD
 * Board configuration.
 *
 * IO connection
 *
 * | PA00     | PA01    | PA02  | PA03  | PA04  | PA05  | PA06  | PA07 |
 * | :---     | :---    | :---  | :---  | :---  | :---  | :---  | :--- |
 * | XIN32    | XOUT32  | FPAG? | FPAG? | FPAG? | FPAG? | FPAG? | VBAT |
 *
 * | PA08     | PA09    | PA10  | PA11  | PA12     | PA13     | PA14     | PA15     |
 * | :---     | :---    | :---  | :---  | :---     | :---     | :---     | :---     |
 * | I2C_SDA  | I2C_SCL | FPAG? | FPAG? | JTAG_TDI | JTAG_TCK | JTAG_TMS | JTAG_TDO |
 * | FPAG?    | FPAG?   |       |       |          |          |          |          |
 *
 * | PA16     | PA17    | PA18   | PA19     | PA20  | PA21  | PA22  | PA23  |
 * | :---     | :---    | :---   | :---     | :---  | :---  | :---  | :---  |
 * | FPAG?    | FPAG?   | USB_ID | FPAG?    | FPAG? | FPAG? | FPAG? | FPAG? |
 * | SPI_MOSI | SPI_SCK |        | SPI_MISO |       |       |       |       |
 *
 * | PA24    | PA25    | PA27     | PA28  | PA30  | PA31  |
 * | :---    | :---    | :---     | :---  | :---  | :---  |
 * | USB_DM  | USB_DP  | FPAG_CLK | FPAG? | SWCLK | SWDIO |
 *
 * | PB02  | PB03  |
 * | :---  | :---  |
 * | FPAG? | FPAG? |
 *
 * | PB08  | PB09  | PB10  | PB11  |
 * | :---  | :---  | :---  | :---  |
 * | LED_R | FPAG? | FPAG? | FPAG? |
 *
 * | PB22        | PB23        |
 * | :---        | :---        |
 * | FPAG?       | FPAG?       |
 * | SECOM5,PAD2 | SECOM5,PAD3 |
 * | USART1_TX   | USART1_RX   |
 *
 * @{
 */

#include <moss/moss.h>
#include <cmsis/nvmctrl.h>
#include <cmsis/sysctrl.h>
#include <cmsis/port.h>
#include <cmsis/sercom.h>
#include <cmsis/pm.h>
#include <cmsis/gclk.h>

#define BOARD_I2C_SDA_PORT MKR4000_PORTA
#define BOARD_I2C_SDA_PIN 8
#define BOARD_I2C_SCL_PORT MKR4000_PORTA
#define BOARD_I2C_SCL_PIN 9

#define BOARD_LED_PORT MKR4000_PORTB
#define BOARD_LED_PIN 8

#define BOARD_USART1_RX_PORT MKR4000_PORTB
#define BOARD_USART1_RX_PIN 23
#define BOARD_USART1_TX_PORT MKR4000_PORTB
#define BOARD_USART1_TX_PIN 22
#define BOARD_USART1_PORT_FUNC PORT_func('D')
#define BOARD_USART1_SERCOM MKR4000_SERCOM5
#define BOARD_USART1_PAD MKR4000_USART_PAD_R3T2X3
#define BOARD_USART1_BAUD(_bps) (65536 * (1 - 16 * (_bps) / 48000000ul))

/** @} TEST3_BOARD */

#endif /* _H_TEST3_BOARD */
