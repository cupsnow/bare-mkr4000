/** @author joelai */

#include "test3_board.h"

unsigned long SystemCoreClock = 0;

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
#if 1
	// @datasheet NVM Characteristics, read wait time
	MKR4000_NVMCTRL->CTRLB = MOSS_BITVAL(MKR4000_NVMCTRL->CTRLB,
			MOSS_BITMASK(MKR4000_NVMCTRL_CTRLB_RWS), 1 << MKR4000_NVMCTRL_CTRLB_RWS_SHIFT);

//	MKR4000_PM->APBAMASK |= (1 << MKR4000_PM_APBAMASK_GCLK_SHIFT);

	// @datasheet 32kHz Crystal Oscillator Characteristics, startup time
	MKR4000_SYSCTRL->XOSC32K = (6 << MKR4000_SYSCTRL_XOSC32K_STARTUP_SHIFT) |
					(1 << MKR4000_SYSCTRL_XOSC32K_XTALEN_SHIFT) |
					(1 << MKR4000_SYSCTRL_XOSC32K_EN32K_SHIFT);

	MKR4000_SYSCTRL->XOSC32K |= (1 << MKR4000_SYSCTRL_XOSC32K_ENABLE_SHIFT);

	while ((MKR4000_SYSCTRL->PCLKSR & (1 << MKR4000_SYSCTRL_PCLKSR_XOSC32KRDY_SHIFT)) == 0);

	MKR4000_GCLK->CTRL = (1 << MKR4000_GCLK_CTRL_SWRST_SHIFT);
	while ((MKR4000_GCLK->CTRL & (1 << MKR4000_GCLK_CTRL_SWRST_SHIFT)) &&
			(MKR4000_GCLK->STATUS & (1 << MKR4000_GCLK_STATUS_SYNCBUSY_SHIFT)));

	// choose generator#1, div/1
	MKR4000_GCLK->GENDIV = (1 << MKR4000_GCLK_GENDIV_ID_SHIFT);

	while (MKR4000_GCLK->STATUS & (1 << MKR4000_GCLK_STATUS_SYNCBUSY_SHIFT));

	// enable generator#1, src xosc32k
	MKR4000_GCLK->GENCTRL = (1 << MKR4000_GCLK_GENCTRL_ID_SHIFT) |
			(MKR4000_GCLK_SRC_XOSC32K << MKR4000_GCLK_GENCTRL_SRC_SHIFT) |
			(1 << MKR4000_GCLK_GENCTRL_GENEN_SHIFT);

	while (MKR4000_GCLK->STATUS & (1 << MKR4000_GCLK_STATUS_SYNCBUSY_SHIFT));

	// enable DFLL48M, src generator#1
	MKR4000_GCLK->CLKCTRL = (MKR4000_GCLK_CLK_DFF48M_REF << MKR4000_GCLK_CLKCTRL_ID_SHIFT) |
			(1 << MKR4000_GCLK_CLKCTRL_GEN_SHIFT) |
			(1 << MKR4000_GCLK_CLKCTRL_CLKEN_SHIFT);

	while (MKR4000_GCLK->STATUS & (1 << MKR4000_GCLK_STATUS_SYNCBUSY_SHIFT));

	// @errata1.2.1, DFLL clock must be requested before being configured
	MKR4000_SYSCTRL->DFLLCTRL &= ~(1 << MKR4000_SYSCTRL_DFLLCTRL_ONDEMAND_SHIFT);
//	MKR4000_SYSCTRL->DFLLCTRL = (1 << MKR4000_SYSCTRL_DFLLCTRL_ENABLE_SHIFT);

	while ((MKR4000_SYSCTRL->PCLKSR & (1 << MKR4000_SYSCTRL_PCLKSR_DFLLRDY_SHIFT)) == 0);

#define MOSS_BITVALHALF(_bits) ((1 << ((_bits) - 1)) - 1)

	MKR4000_SYSCTRL->DFLLMUL =
			(MOSS_BITVALHALF(MKR4000_SYSCTRL_DFLLMUL_CSTEP_BITS) << MKR4000_SYSCTRL_DFLLMUL_CSTEP_SHIFT) |
			(MOSS_BITVALHALF(MKR4000_SYSCTRL_DFLLMUL_FSTEP_BITS) << MKR4000_SYSCTRL_DFLLMUL_FSTEP_SHIFT) |
			((48000000lu / 32768lu) << MKR4000_SYSCTRL_DFLLMUL_MUL_SHIFT);

	while ((MKR4000_SYSCTRL->PCLKSR & (1 << MKR4000_SYSCTRL_PCLKSR_DFLLRDY_SHIFT)) == 0);

	MKR4000_SYSCTRL->DFLLCTRL |= (1 << MKR4000_SYSCTRL_DFLLCTRL_MODE_SHIFT) |
			(1 << MKR4000_SYSCTRL_DFLLCTRL_WAITLOCK_SHIFT) |
			(1 << MKR4000_SYSCTRL_DFLLCTRL_QLDIS_SHIFT);

	while ((MKR4000_SYSCTRL->PCLKSR & (1 << MKR4000_SYSCTRL_PCLKSR_DFLLRDY_SHIFT)) == 0);

	MKR4000_SYSCTRL->DFLLCTRL |= (1 << MKR4000_SYSCTRL_DFLLCTRL_ENABLE_SHIFT);

	while ((MKR4000_SYSCTRL->PCLKSR & (1 << MKR4000_SYSCTRL_PCLKSR_DFLLLCKC_SHIFT)) == 0 ||
			(MKR4000_SYSCTRL->PCLKSR & (1 << MKR4000_SYSCTRL_PCLKSR_DFLLLCKF_SHIFT)) == 0);

	while ((MKR4000_SYSCTRL->PCLKSR & (1 << MKR4000_SYSCTRL_PCLKSR_DFLLRDY_SHIFT)) == 0);

	// choose generator#0, div/1
	MKR4000_GCLK->GENDIV = (0 << MKR4000_GCLK_GENDIV_ID_SHIFT);

	while (MKR4000_GCLK->STATUS & (1 << MKR4000_GCLK_STATUS_SYNCBUSY_SHIFT));

	// enable generator#0, src dfll48m
	MKR4000_GCLK->GENCTRL = (0 << MKR4000_GCLK_GENCTRL_ID_SHIFT) |
			(MKR4000_GCLK_SRC_DFLL48M << MKR4000_GCLK_GENCTRL_SRC_SHIFT) |
			(1 << MKR4000_GCLK_GENCTRL_IDC_SHIFT) |
			(1 << MKR4000_GCLK_GENCTRL_GENEN_SHIFT);

	while (MKR4000_GCLK->STATUS & (1 << MKR4000_GCLK_STATUS_SYNCBUSY_SHIFT));

	// choose generator#3, div/1
	MKR4000_GCLK->GENDIV = (3 << MKR4000_GCLK_GENDIV_ID_SHIFT);

	while (MKR4000_GCLK->STATUS & (1 << MKR4000_GCLK_STATUS_SYNCBUSY_SHIFT));

	MKR4000_SYSCTRL->OSC8M = MOSS_BITVAL(MKR4000_SYSCTRL->OSC8M,
			MOSS_BITMASK(MKR4000_SYSCTRL_OSC8M_PRESC) | (1 << MKR4000_SYSCTRL_OSC8M_ONDEMAND_SHIFT),
			0);

	// enable generator#3, src osc8m
	MKR4000_GCLK->GENCTRL = (3 << MKR4000_GCLK_GENCTRL_ID_SHIFT) |
			(MKR4000_GCLK_SRC_OSC8M << MKR4000_GCLK_GENCTRL_SRC_SHIFT) |
			(1 << MKR4000_GCLK_GENCTRL_GENEN_SHIFT);

	while (MKR4000_GCLK->STATUS & (1 << MKR4000_GCLK_STATUS_SYNCBUSY_SHIFT));

	MKR4000_PM->CPUSEL = 0;
	MKR4000_PM->APBASEL = 0;
	MKR4000_PM->APBBSEL = 0;
	MKR4000_PM->APBCSEL = 0;

	MKR4000_NVMCTRL->CTRLB |= (1 << MKR4000_NVMCTRL_CTRLB_MANW_SHIFT);

	SystemCoreClock = 48000000ul;

	__enable_irq();

#endif
}

int main(int argc, char **argv) {
//	for (int i = 0; i < argc; i++) {
//		moss_debug("argv[%d/%d]: %s%s", i + 1, argc, argv[i], moss_newline);
//	}

	PORT_setup(BOARD_LED_PORT, BOARD_LED_PIN, PORT_func_gpio, 1);

	PORT_setup(BOARD_USART1_RX_PORT, BOARD_USART1_RX_PIN, BOARD_USART1_PORT_FUNC, 1);
	PORT_setup(BOARD_USART1_TX_PORT, BOARD_USART1_TX_PIN, BOARD_USART1_PORT_FUNC, 1);

	if (BOARD_USART1_SERCOM == MKR4000_SERCOM5) {
		MKR4000_PM->APBCMASK |= (1 << MKR4000_PM_APBCMASK_SERCOM5_SHIFT);

		NVIC_EnableIRQ(SERCOM5_IRQn);
		NVIC_SetPriority (SERCOM5_IRQn, ((1<<__NVIC_PRIO_BITS) - 1));

		// enable SERCOM5, src generator#0
		MKR4000_GCLK->CLKCTRL = (MKR4000_GCLK_CLK_SERCOM5_CORE << MKR4000_GCLK_CLKCTRL_ID_SHIFT) |
				(0 << MKR4000_GCLK_CLKCTRL_GEN_SHIFT) |
				(1 << MKR4000_GCLK_CLKCTRL_CLKEN_SHIFT);

		while (MKR4000_GCLK->STATUS & (1 << MKR4000_GCLK_STATUS_SYNCBUSY_SHIFT));
	}

	USART_setup(BOARD_USART1_SERCOM, BOARD_USART1_PAD, USART_BAUD(115200));

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
