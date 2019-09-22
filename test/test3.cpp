/*
 * test1.cpp
 *
 *  Created on: Sep 8, 2019
 *      Author: joelai
 */
#include <moss/moss.h>
#include <cmsis/port.h>

// B8
#define BOARD_LED_PORT MKR4000_PORTB // 1
#define BOARD_LED_PIN 8

extern "C" {

void SystemInit(void) {

}

int main(int argc, char **argv) {
	for (int i = 0; i < argc; i++) {
		moss_debug("argv[%d/%d]: %s%s", i + 1, argc, argv[i], moss_newline);
	}

	PORT_setup(BOARD_LED_PORT, BOARD_LED_PIN, PORT_func_gpio, 1);

	while (1) {
		for (volatile int i = 0; i < 1000000; i++) ;
		PORT_toggle(BOARD_LED_PORT, BOARD_LED_PIN);
	}

	return 0;
}

}
