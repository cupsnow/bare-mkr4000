/** @author joelai */

#ifndef _H_MKR4000_CMSIS_SERCOM
#define _H_MKR4000_CMSIS_SERCOM

#include "mkr4000.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup MKR4000_CMSIS_USART USART
 *
 * @ingroup MKR4000_CMSIS_SERCOM
 *
 * @{
 */

/** USART register map. */
typedef struct {
	__IOM uint32_t CTRLA, CTRLB; // 0..0x7
	__IM uint8_t RESERVED1[4]; // 0x8..0xb
	__IOM uint16_t BAUD; // 0xc..0xd
	__IOM uint8_t RXPL; // 0xe
	__IM uint8_t RESERVED2[5]; // 0xf..0x13
	__IOM uint8_t INTENCLR; // 0x14
	__IM uint8_t RESERVED3[1]; // 0x15
	__IOM uint8_t INTENSET; // 0x16
	__IM uint8_t RESERVED4[1]; // 0x17
	__IOM uint8_t INTFLAG; // 0x18
	__IM uint8_t RESERVED5[1]; // 0x19
	__IOM uint16_t STATUS; // 0x1a..0x1b
	__IOM uint32_t SYNCBUSY; // 0x1c..0x1f
	__IM uint8_t RESERVED6[8]; // 0x20..0x27
	__IOM uint16_t DATA; // 0x28..0x29
	__IM uint8_t RESERVED7[6]; // 0x2a..0x2f
	__IOM uint8_t DBGCTRL; // 0x30
	__IM uint8_t RESERVED8[3]; // 0x31..0x33
	__IOM uint16_t FIFOSPACE, FIFOPTR; // 0x34..0x37
} MKR4000_USART_T;

#define MKR4000_USART_CTRLA_SWRST_SHIFT 0
#define MKR4000_USART_CTRLA_ENABLE_SHIFT 1

#define MKR4000_USART_CTRLA_MODE_SHIFT 2
#define MKR4000_USART_CTRLA_MODE_BITS 3

#define MKR4000_USART_CTRLA_RUNSTANDBY_SHIFT 7

#define MKR4000_USART_CTRLA_IBON_SHIFT 8

#define MKR4000_USART_CTRLA_SAMPR_SHIFT 13
#define MKR4000_USART_CTRLA_SAMPR_BITS 3

#define MKR4000_USART_CTRLA_TXPO_SHIFT 16
#define MKR4000_USART_CTRLA_TXPO_BITS 2

#define MKR4000_USART_CTRLA_RXPO_SHIFT 20
#define MKR4000_USART_CTRLA_RXPO_BITS 2

#define MKR4000_USART_CTRLA_SAMPA_SHIFT 22
#define MKR4000_USART_CTRLA_SAMPA_BITS 2

#define MKR4000_USART_CTRLA_FORM_SHIFT 24
#define MKR4000_USART_CTRLA_FORM_BITS 4

#define MKR4000_USART_CTRLA_CMODE_SHIFT 28
#define MKR4000_USART_CTRLA_CPOR_SHIFT 29
#define MKR4000_USART_CTRLA_DORD_SHIFT 30

#define MKR4000_USART_CTRLB_CHSIZE_SHIFT 0
#define MKR4000_USART_CTRLB_CHSIZE_BITS 3

#define MKR4000_USART_CTRLB_SBMODE_SHIFT 6
#define MKR4000_USART_CTRLB_COLDEN_SHIFT 8
#define MKR4000_USART_CTRLB_SFDE_SHIFT 9
#define MKR4000_USART_CTRLB_ENC_SHIFT 10
#define MKR4000_USART_CTRLB_PMODE_SHIFT 13

#define MKR4000_USART_CTRLB_TXEN_SHIFT 16
#define MKR4000_USART_CTRLB_RXEN_SHIFT 17

#define MKR4000_USART_CTRLB_FIFOCLR_SHIFT 22
#define MKR4000_USART_CTRLB_FIFOCLR_BITS 2

#define MKR4000_USART_BAUD_FP_SHIFT 13
#define MKR4000_USART_BAUD_FP_BITS 3

#define MKR4000_USART_INTENCLR_DRE_SHIFT 0
#define MKR4000_USART_INTENCLR_TXC_SHIFT 1
#define MKR4000_USART_INTENCLR_RXC_SHIFT 2
#define MKR4000_USART_INTENCLR_RXS_SHIFT 3
#define MKR4000_USART_INTENCLR_CTSIC_SHIFT 4
#define MKR4000_USART_INTENCLR_RXBRK_SHIFT 5
#define MKR4000_USART_INTENCLR_ERROR_SHIFT 7

#define MKR4000_USART_INTENSET_DRE_SHIFT 0
#define MKR4000_USART_INTENSET_TXC_SHIFT 1
#define MKR4000_USART_INTENSET_RXC_SHIFT 2
#define MKR4000_USART_INTENSET_RXS_SHIFT 3
#define MKR4000_USART_INTENSET_CTSIC_SHIFT 4
#define MKR4000_USART_INTENSET_RXBRK_SHIFT 5
#define MKR4000_USART_INTENSET_ERROR_SHIFT 7

#define MKR4000_USART_INTFLAG_DRE_SHIFT 0
#define MKR4000_USART_INTFLAG_TXC_SHIFT 1
#define MKR4000_USART_INTFLAG_RXC_SHIFT 2
#define MKR4000_USART_INTFLAG_RXS_SHIFT 3
#define MKR4000_USART_INTFLAG_CTSIC_SHIFT 4
#define MKR4000_USART_INTFLAG_RXBRK_SHIFT 5
#define MKR4000_USART_INTFLAG_ERROR_SHIFT 7

#define MKR4000_USART_STATUS_PERR_SHIFT 0
#define MKR4000_USART_STATUS_FERR_SHIFT 1
#define MKR4000_USART_STATUS_BUFOVF_SHIFT 2
#define MKR4000_USART_STATUS_CTS_SHIFT 3
#define MKR4000_USART_STATUS_ISF_SHIFT 4
#define MKR4000_USART_STATUS_COLL_SHIFT 5
#define MKR4000_USART_STATUS_TXE_SHIFT 6

#define MKR4000_USART_SYNCBUSY_SWRST_SHIFT 0
#define MKR4000_USART_SYNCBUSY_ENABLE_SHIFT 1
#define MKR4000_USART_SYNCBUSY_CTRLB_SHIFT 2

#define MKR4000_USART_DATA_DATA_SHIFT 0
#define MKR4000_USART_DATA_DATA_BITS 9

#define MKR4000_USART_DBGCTRL_DBGSTOP_SHIFT 0

#define MKR4000_USART_FIFOSPACE_TXSPACE_SHIFT 0
#define MKR4000_USART_FIFOSPACE_TXSPACE_BITS 5

#define MKR4000_USART_FIFOSPACE_RXSPACE_SHIFT 8
#define MKR4000_USART_FIFOSPACE_RXSPACE_BITS 5

#define MKR4000_USART_FIFOPTR_CPUWRPTR_SHIFT 0
#define MKR4000_USART_FIFOPTR_CPUWRPTR_BITS 4

#define MKR4000_USART_FIFOPTR_CPURDPTR_SHIFT 8
#define MKR4000_USART_FIFOPTR_CPURDPTR_BITS 4

typedef enum {
	MKR4000_USART_PAD_R0 = (0 << MKR4000_USART_CTRLA_RXPO_SHIFT),
	MKR4000_USART_PAD_R1 = (1 << MKR4000_USART_CTRLA_RXPO_SHIFT),
	MKR4000_USART_PAD_R2 = (2 << MKR4000_USART_CTRLA_RXPO_SHIFT),
	MKR4000_USART_PAD_R3 = (3 << MKR4000_USART_CTRLA_RXPO_SHIFT),
	MKR4000_USART_PAD_T0X1 = (0 << MKR4000_USART_CTRLA_TXPO_SHIFT),
	MKR4000_USART_PAD_T2X3 = (1 << MKR4000_USART_CTRLA_TXPO_SHIFT),
	MKR4000_USART_PAD_T0RC = (2 << MKR4000_USART_CTRLA_TXPO_SHIFT),
#define MKR4000_USART_PAD_RT(_r, _t) \
		MKR4000_USART_PAD_ ## _r ## _t = MKR4000_USART_PAD_ ## _r | MKR4000_USART_PAD_ ## _t
	MKR4000_USART_PAD_RT(R0, T2X3),
	MKR4000_USART_PAD_RT(R1, T0X1),
	MKR4000_USART_PAD_RT(R1, T2X3),
	MKR4000_USART_PAD_RT(R1, T0RC),
	MKR4000_USART_PAD_RT(R2, T0X1),
	MKR4000_USART_PAD_RT(R2, T0RC),
	MKR4000_USART_PAD_RT(R3, T0X1),
	MKR4000_USART_PAD_RT(R3, T2X3),
} MKR4000_USART_PAD_T;

/** @} MKR4000_CMSIS_USART */

/** @defgroup MKR4000_CMSIS_SERCOM SERCOM
 *
 * @ingroup MKR4000_CMSIS
 *
 * @{
 */

typedef union {
	MKR4000_USART_T usart;
} MKR4000_SERCOM_T;

/** SERCOM0 instance. */
#define MKR4000_SERCOM0 ((MKR4000_SERCOM_T*)MKR4000_SERCOM_BASE)

/** SERCOM1 instance. */
#define MKR4000_SERCOM1 ((MKR4000_SERCOM_T*)(MKR4000_SERCOM_BASE + 0x400lu))

/** SERCOM2 instance. */
#define MKR4000_SERCOM2 ((MKR4000_SERCOM_T*)(MKR4000_SERCOM_BASE + 0x400lu * 2))

/** SERCOM3 instance. */
#define MKR4000_SERCOM3 ((MKR4000_SERCOM_T*)(MKR4000_SERCOM_BASE + 0x400lu * 3))

/** SERCOM4 instance. */
#define MKR4000_SERCOM4 ((MKR4000_SERCOM_T*)(MKR4000_SERCOM_BASE + 0x400lu * 4))

/** SERCOM5 instance. */
#define MKR4000_SERCOM5 ((MKR4000_SERCOM_T*)(MKR4000_SERCOM_BASE + 0x400lu * 5))

void USART_setup(MKR4000_SERCOM_T *sercom, MKR4000_USART_PAD_T pad,
		unsigned baud);
void USART_putc(MKR4000_SERCOM_T *sercom, int data);
int USART_getc(MKR4000_SERCOM_T *sercom);

/** @} MKR4000_CMSIS_SERCOM */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _H_MKR4000_CMSIS_SERCOM */
