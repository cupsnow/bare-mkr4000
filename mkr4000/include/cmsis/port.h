/** @author joelai */

#ifndef _H_MKR4000_CMSIS_PORT
#define _H_MKR4000_CMSIS_PORT

#include <cmsis/mkr4000.h>

/** @defgroup MKR4000_CMSIS_PORT Port
 *
 * @ingroup MKR4000_CMSIS
 *
 * Port multiplexing
 * - PMUX_PMUX: Set 0..8 for function A..I
 *
 * @{
 */

/** Port register map. */
typedef struct {
	__IM uint32_t DIR; // 0
	__IOM uint32_t DIRCLR, DIRSET, DIRTGL; // 0x4..0xf
	__IM uint32_t OUT; // 0x10
	__IOM uint32_t OUTCLR, OUTSET, OUTTGL; // 0x14..0x1f
	__IM uint32_t IN; // 0x20
	__IOM uint32_t CTRL, WRCONFIG; // 0x24..0x2b
	__IM uint8_t RESERVED1[4]; // 0x2c..0x2f
	__IOM uint8_t PMUX[16], PINCFG[32]; // 0x30..0x5f
} MKR4000_PORT_T;

/** Port A instance. */
#define MKR4000_PORTA ((MKR4000_PORT_T*)MKR4000_PORT_BASE)

/** Port B instance. */
#define MKR4000_PORTB ((MKR4000_PORT_T*)(MKR4000_PORT_BASE + 0x80lu))

#define MKR4000_PORT_PMUX_PMUXE_SHIFT 0
#define MKR4000_PORT_PMUX_PMUXO_SHIFT 4
#define MKR4000_PORT_PMUX_PMUX_BITS 4

#define MKR4000_PORT_PINCFG_PMUXEN_SHIFT 0
#define MKR4000_PORT_PINCFG_INEN_SHIFT 1

/** @} MKR4000_CMSIS_PORT */

#define PORT_func_gpio ((char)-1)

#define PORT_dir(_port, _pin, _out) if (_out) { \
	(_port)->DIRSET |= (1 << (_pin)); \
} else { \
	(_port)->DIRCLR |= (1 << (_pin)); \
}

#define PORT_set(_port, _pin, _val) if (_val) { \
	(_port)->OUTSET |= (1 << (_pin)); \
} else { \
	(_port)->OUTCLR |= (1 << (_pin)); \
}

#define PORT_get(_port, _pin) (((_port)->IN >> (_pin)) & 0x1)
#define PORT_toggle(_port, _pin) (_port)->OUTTGL |= (1 << (_pin))

#ifdef __cplusplus
extern "C" {
#endif

void PORT_setup(MKR4000_PORT_T *port, char pin, char func, char dir_out);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _H_MKR4000_CMSIS_PORT */
