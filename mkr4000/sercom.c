/** @author joelai */

#include <moss/moss.h>
#include <cmsis/sercom.h>

void USART_setup(MKR4000_SERCOM_T *sercom, MKR4000_USART_PAD_T pad,
		uint16_t baud) {
	while(sercom->usart.SYNCBUSY & (1 << MKR4000_USART_SYNCBUSY_ENABLE_SHIFT));
	sercom->usart.CTRLA &= ~(1 << MKR4000_USART_CTRLA_ENABLE_SHIFT);
	if (baud == 0) return;

	while(sercom->usart.SYNCBUSY & (1 << MKR4000_USART_SYNCBUSY_SWRST_SHIFT));
	sercom->usart.CTRLA |= (1 << MKR4000_USART_CTRLA_SWRST_SHIFT);

	while ((sercom->usart.CTRLA & (1 << MKR4000_USART_CTRLA_SWRST_SHIFT)) ||
			(sercom->usart.SYNCBUSY & ((1 << MKR4000_USART_SYNCBUSY_SWRST_SHIFT) |
					(1 << MKR4000_USART_SYNCBUSY_ENABLE_SHIFT)))) {
		;
	}
	sercom->usart.CTRLA = pad | (1 << MKR4000_USART_CTRLA_MODE_SHIFT) |
			(1 << MKR4000_USART_CTRLA_DORD_SHIFT);

	while(sercom->usart.SYNCBUSY & (1 << MKR4000_USART_SYNCBUSY_CTRLB_SHIFT));
	sercom->usart.CTRLB = (1 << MKR4000_USART_CTRLB_TXEN_SHIFT) |
			(1 << MKR4000_USART_CTRLB_RXEN_SHIFT);

	sercom->usart.BAUD = baud;

	while(sercom->usart.SYNCBUSY & (1 << MKR4000_USART_SYNCBUSY_ENABLE_SHIFT));
	sercom->usart.CTRLA |= (1 << MKR4000_USART_CTRLA_ENABLE_SHIFT);
}

void USART_putc(MKR4000_SERCOM_T *sercom, int data) {
	while(!(sercom->usart.INTFLAG & (1 << MKR4000_USART_INTFLAG_DRE_SHIFT)));
	sercom->usart.DATA = (uint16_t)data;
}

int USART_getc(MKR4000_SERCOM_T *sercom) {
	return (sercom->usart.INTFLAG & (1 << MKR4000_USART_INTFLAG_RXC_SHIFT)) ?
			(uint16_t)sercom->usart.DATA : -1;
}
