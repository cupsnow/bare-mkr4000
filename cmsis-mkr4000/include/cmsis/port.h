/*
 * port.h
 *
 *  Created on: Sep 22, 2019
 *      Author: joelai
 */

#ifndef _H_TEST26_CMSIS_MKR4000_PORT
#define _H_TEST26_CMSIS_MKR4000_PORT

#include <cmsis/mkr4000.h>

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

void PORT_setup(MKR4000_PORT_TypeDef *port, char pin, char func, char dir_out);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _H_TEST26_CMSIS_MKR4000_PORT */
