/** @author joelai */

#include "test3_board.h"

extern "C" {

// retarget
int _write(int fd, char *ptr, int len) {
  /* Write "len" of char from "ptr" to file id "fd"
   * Return number of char written.
   * Need implementing with UART here. */
  return len;
}

// retarget
int _read(int fd, char *ptr, int len) {
  /* Read "len" of char to "ptr" from file id "fd"
   * Return number of char read.
   * Need implementing with UART here. */
  return len;
}

void SystemInit(void) {
	MKR4000_NVMCTRL->CTRLB = MOSS_BITWORD(MKR4000_NVMCTRL->CTRLB,
			MKR4000_NVMCTRL_CTRLB_RWS_SHIFT, MKR4000_NVMCTRL_CTRLB_RWS_BITS,
			1);
	MKR4000_PM->APBAMASK |= (1 << MKR4000_PM_APBAMASK_GCLK_SHIFT);

	MKR4000_SYSCTRL->XOSC32K = (6 << MKR4000_SYSCTRL_XOSC32K_STARTUP_SHIFT) |
			(1 << MKR4000_SYSCTRL_XOSC32K_XTALEN_SHIFT) |
			(1 << MKR4000_SYSCTRL_XOSC32K_EN32K_SHIFT);
	MKR4000_SYSCTRL->XOSC32K |= (1 << MKR4000_SYSCTRL_XOSC32K_ENABLE_SHIFT);
	while (!(MKR4000_SYSCTRL->PCLKSR & (1 << MKR4000_SYSCTRL_PCLKSR_XOSC32KRDY_SHIFT)));

	MKR4000_GCLK->CTRL = (1 << MKR4000_GCLK_CTRL_SWRST_SHIFT);
	while ((MKR4000_GCLK->CTRL & (1 << MKR4000_GCLK_CTRL_SWRST_SHIFT)) &&
			(MKR4000_GCLK->STATUS & (1 << MKR4000_GCLK_STATUS_SYNCBUSY_SHIFT)));

	MKR4000_GCLK->GENDIV =


}

int main(int argc, char **argv) {
	for (int i = 0; i < argc; i++) {
		moss_debug("argv[%d/%d]: %s%s", i + 1, argc, argv[i], moss_newline);
	}

	PORT_setup(BOARD_LED_PORT, BOARD_LED_PIN, PORT_func_gpio, 1);

	PORT_setup(BOARD_USART1_RX_PORT, BOARD_USART1_RX_PIN, BOARD_USART1_PORT_FUNC, 1);
	PORT_setup(BOARD_USART1_TX_PORT, BOARD_USART1_TX_PIN, BOARD_USART1_PORT_FUNC, 1);
	if (BOARD_USART1_SERCOM == MKR4000_SERCOM5) {
		MKR4000_PM->APBCMASK |= (1 << MKR4000_PM_APBCMASK_SERCOM5_SHIFT);
		MKR4000_GCLK->CLKCTRL = (1 << MKR4000_GCLK_CLKCTRL_CLKEN_SHIFT) |
				(0 << MKR4000_GCLK_CLKCTRL_GEN_SHIFT) |
				(MKR4000_GCLK_ID_SERCOM5_CORE << MKR4000_GCLK_CLKCTRL_ID_SHIFT);
		while (MKR4000_GCLK->STATUS & (1 << MKR4000_GCLK_STATUS_SYNCBUSY_SHIFT));
	}
	USART_setup(BOARD_USART1_SERCOM, BOARD_USART1_PAD,
			(uint16_t)BOARD_USART1_BAUD(115200));

	while (1) {
		int c, r;

		for (volatile int i = 0; i < 1000000; i++) ;
		PORT_toggle(BOARD_LED_PORT, BOARD_LED_PIN);

		if ((r = USART_getc(BOARD_USART1_SERCOM)) >= 0) {
			c = r;
		} else if ((c < '0') || (c > 'Z')) {
			c = '0';
		}
		USART_putc(BOARD_USART1_SERCOM, c++);
	}

	return 0;
}

}
