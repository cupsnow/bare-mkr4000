/*
 * test1.cpp
 *
 *  Created on: Sep 8, 2019
 *      Author: joelai
 */
#include <moss/moss.h>

void test_bits(void) {
	moss_debug("MOSS_BITMASK(3, 2): 0x%x%s",
			(unsigned)MOSS_BITMASK(3, 2), moss_newline);
	moss_debug("MOSS_BITWORD(0x5a, 3, 2, 0x2): 0x%x%s",
			(unsigned)MOSS_BITWORD(0x5a, 3, 2, 0x2), moss_newline);
}

int main(int argc, char **argv) {

	for (int i = 0; i < argc; i++) {
		moss_debug("argv[%d/%d]: %s%s", i + 1, argc, argv[i], moss_newline);
	}

	test_bits();
	return 0;
}

