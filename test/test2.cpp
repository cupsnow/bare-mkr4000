/*
 * test1.cpp
 *
 *  Created on: Sep 8, 2019
 *      Author: joelai
 */
#include <moss/moss.h>
#include <sam.h>

#define BOARD_LED_PORT                    (1)
#define BOARD_LED_PIN                     (8)

extern "C" {

inline void LED_init(void) { PORT->Group[BOARD_LED_PORT].DIRSET.reg = (1<<BOARD_LED_PIN); }
inline void LED_on(void) { PORT->Group[BOARD_LED_PORT].OUTSET.reg = (1<<BOARD_LED_PIN); }
inline void LED_off(void) { PORT->Group[BOARD_LED_PORT].OUTCLR.reg = (1<<BOARD_LED_PIN); }
inline void LED_toggle(void) { PORT->Group[BOARD_LED_PORT].OUTTGL.reg = (1<<BOARD_LED_PIN); }

void LED_pulse();

void SystemInit(void) {

}

}

int main(int argc, char **argv) {

	for (int i = 0; i < argc; i++) {
		moss_debug("argv[%d/%d]: %s%s", i + 1, argc, argv[i], moss_newline);
	}

	LED_init();

	while (1) {
		for (volatile int i = 0; i < 1000000; i++) ;
		LED_toggle();
	}

	return 0;
}

