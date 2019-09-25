/*
 * test1.cpp
 *
 *  Created on: Sep 8, 2019
 *      Author: joelai
 */
#include <moss/moss.h>

static moss_test_flag_t test_case_bitmask(moss_test_case_t *test_case) {
	MOSS_TEST_ASSERT_RETURN(MOSS_BITMASK(3, 2) == 0x18,
			test_case, failed_suite);
	MOSS_TEST_ASSERT_RETURN(MOSS_BITWORD(0x5a, 3, 2, 0x2) == 0x52,
			test_case, failed_suite);
	return moss_test_flag_result_pass;
}

static moss_test_flag_t test_case_moss_buf(moss_test_case_t *test_case) {
	moss_buf_t fb;
	uint8_t buf[10];
	int r;

	memset(&fb, 0, sizeof(fb));
	fb.data = buf;
	fb.cap = 8;

	fb.pos = 5; fb.lmt = 2; buf[5] = '1'; buf[6] = '2';
	r = moss_buf_write(&fb, "abcdefghi", 9);
	MOSS_TEST_ASSERT_RETURN((r == 3) && (fb.pos == 5) && (fb.lmt == 8) &&
			(memcmp(buf, "bcdef12a", 8) == 0), test_case, failed_suite);
	return moss_test_flag_result_pass;
}

int main(int argc, char **argv) {
	int i;
	moss_test_t test_base;

	for (int i = 0; i < argc; i++) {
		moss_debug("argv[%d/%d]: %s%s", i + 1, argc, argv[i], moss_newline);
	}

	MOSS_TEST_INIT(&test_base, "BASE");
	MOSS_TEST_CASE_INIT4(&test_base, "BASE/MOSS_BUF", &test_case_moss_buf);
	MOSS_TEST_CASE_INIT4(&test_base, "BASE/BITMASK", &test_case_bitmask);
	MOSS_TEST_RUN(&test_base);
	MOSS_TEST_REPORT(&test_base);

	return 0;
}

