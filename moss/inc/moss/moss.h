/*
 * Copyright (c) 2019, Advanced Radar Technology Co., Ltd
 * All rights reserved.
 *
 */

/** @defgroup MOSS_MISC_API Host misc
 * @ingroup MIGRU_MISC_API
 * @brief Public miscellaneous API for host.
 *
 *   This module provide public miscellaneous API for application. More detail
 * grouped in subpage.
 */

#ifndef _H_MIGRU_TEST_GTRACK_MOSS
#define _H_MIGRU_TEST_GTRACK_MOSS

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include <errno.h>
#include <sys/tree.h>
#include <sys/queue.h>

/** @addtogroup MOSS_MISC_API
 * @{
 */

/** Minimal */
#define MOSS_MIN(_a, _b) ((_a) <= (_b) ? (_a) : (_b))

/** Stringify */
#define MOSS_STRINGIFY(_s) # _s

/** Stringify expansion. */
#define MOSS_STRINGIFY2(_s) MOSS_STRINGIFY(_s)

/** Get parent from structure member.
 *
 * Example:
 * @code{.c}
 *
 * @endcode
 */
#define MOSS_CONTAINER_OF(_obj, _type, _member) \
	((_type *)((_obj) ? ((char*)(_obj) - offsetof(_type, _member)) : NULL))

/** Count array. */
#define MOSS_ARRAYSIZE(_a) (sizeof(_a) / sizeof((_a)[0]))

#define MOSS_ASSERT_BUILD(_cond) ((void)sizeof(char[1 - 2 * !(_cond)]))

/** moss flag mask.
 *
 *   Define group of flag mask in the enumeration.  The group of flag occupied
 * \<ITEM\>_mask_offset (count from bit0), and take \<ITEM\>_mask_bits.
 * \<ITEM\>_mask used to filter the occupied value.
 *
 * Example:
 * @code{.c}
 * typedef enum flag_enum {
 *   MOSS_FLAG_MASK(flag_class, 0, 2),
 *   MOSS_FLAG(flag_class, _case, 0),
 *   MOSS_FLAG(flag_class, _suite, 1),
 *   MOSS_FLAG_MASK(flag_result, 2, 2),
 *   MOSS_FLAG(flag_result, _pass, 0),
 *   MOSS_FLAG(flag_result, _failed_memory, 1),
 *   MOSS_FLAG(flag_result, _failed_io, 2),
 * } flag_t;
 *
 * flag_t flags = flag_class_suite | flag_result_failed_io; // 0x9
 * flag_t flags_class = flags & flag_class_mask; // 0x1
 * flag_t flags_result = flags & flag_result_mask; // 0x8
 * @endcode
 */
#define MOSS_FLAG_MASK(_name, _offset, _bits) \
		_name ## _mask_offset = _offset, \
		_name ## _mask_bits = _bits, \
		_name ## _mask = (((1 << (_bits)) - 1) << (_offset))

/** moss flag
 *
 * Reference to MOSS_FLAG_MASK()
 */
#define MOSS_FLAG(_base, _name, _val) \
	_base ## _name = ((_val) << _base ## _mask_offset)

/**
 * @} MOSS_MISC_API
 */

#define MOSS_CR '\r'
#define MOSS_LF '\n'

#ifdef __cplusplus
extern "C" {
#endif

extern const char *moss_newline;
#define moss_endl moss_newline

typedef struct moss_rb_entry_rec {
	RB_ENTRY(moss_rb_entry_rec) entry;
	int (*cmp)(struct moss_rb_entry_rec *a, struct moss_rb_entry_rec *b);
} moss_rb_entry_t;
typedef RB_HEAD(moss_rb_tree_rec, moss_rb_entry_rec) moss_rb_tree_t;
RB_PROTOTYPE(moss_rb_tree_rec, moss_rb_entry_rec, entry, );

typedef struct moss_tailq_entry_rec {
	TAILQ_ENTRY(moss_tailq_entry_rec) entry;
} moss_tailq_entry_t;
typedef TAILQ_HEAD(moss_tailq_rec, moss_tailq_entry_rec) moss_tailq_t;

size_t moss_stripl(const void **buf, size_t sz, const char *ext);

/** @defgroup MOSS_BUF_API moss buffer.
 * @ingroup MOSS_MISC_API
 * @brief moss buffer provide viewport for memory.
 */

/** @addtogroup MOSS_BUF_API
 * @{
 */
/** moss buffer data structure. */
typedef struct moss_buf_rec {
	size_t pos, /**< Current data position of the data memory. */
			cap, /**< Total capable of the data memory. */
			lmt; /**< Valid data range. */
	void *data; /**< Pointer to data memory. */
} moss_buf_t;

/** Read from moss buffer to mempry.
 *
 * @param buf
 * @param data
 * @param sz
 * @return
 */
int moss_buf_read(moss_buf_t *buf, void *data, size_t sz);

/** Expand moss buffer capable.
 *
 * @param buf
 * @param cap
 * @param retain
 * @return 0 when success, others when failure.
 */
int moss_buf_expand(moss_buf_t *buf, size_t cap, int retain);

/** Printf to moss buffer.
 *
 * - Assume valid data occupied buf->lmt bytes from buf->pos.
 * - Printf from buf->pos + buf->lmt and append trailing 0.
 * - After printf successful, add the size of written character(no count
 *   trailing 0) to buf->lmt.
 * - If printf failed, restore the first character after original valid data
 *   (useful to retain the string trailing 0).
 *
 * @param buf
 * @param fmt
 * @param va
 * @return
 */
int moss_buf_vprintf(moss_buf_t *buf, const char *fmt, va_list va)
		__attribute__((format(printf, 2, 0)));

/** Printf to moss buffer.
 *
 * Reference to moss_buf_vprintf()
 *
 * @param buf
 * @param fmt
 * @return
 */
int moss_buf_printf(moss_buf_t *buf, const char *fmt, ...)
		__attribute__((format(printf, 2, 3)));

/**
 * @} MOSS_BUF_API
 */

typedef enum moss_log_level_enum {
	MOSS_FLAG_MASK(moss_log_level, 0, 8),
	MOSS_FLAG(moss_log_level, _error, 8),
	MOSS_FLAG(moss_log_level, _info, 9),
	MOSS_FLAG(moss_log_level, _debug, 10),
	MOSS_FLAG(moss_log_level, _verbose, 11),
} moss_log_level_t;

extern int moss_logt(moss_buf_t *buf, unsigned flag, const char *tag, long lno);

int moss_vlogf(moss_buf_t *buf, unsigned flag, const char *tag, long lno,
		const char *fmt, va_list va) __attribute__((format(printf, 5, 0)));
int moss_logf(moss_buf_t *buf, unsigned flag, const char *tag, long lno,
		const char *fmt, ...) __attribute__((format(printf, 5, 6)));

extern int moss_log_level_max;

#define moss_level_str(_lvl, _na) \
	((_lvl) == moss_log_level_error ? "ERROR" : \
	(_lvl) == moss_log_level_info ? "INFO" : \
	(_lvl) == moss_log_level_debug ? "Debug" : \
	(_lvl) == moss_log_level_verbose ? "verbose" : \
	(_na))

extern int moss_log(unsigned lvl, const char *tag, long lno,
		const char *fmt, ...) __attribute__((format(printf, 4, 5)));

#define moss_error(...) moss_log(moss_log_level_error, __func__, __LINE__, __VA_ARGS__)
#define moss_debug(...) moss_log(moss_log_level_debug, __func__, __LINE__, __VA_ARGS__)
#define moss_info(...) moss_log(moss_log_level_info, __func__, __LINE__, __VA_ARGS__)
#define moss_verbose(...) moss_log(moss_log_level_verbose, __func__, __LINE__, __VA_ARGS__)

int moss_readline(int (*getc)(void *arg), void *arg, char *_nl);
extern int moss_file_size(const char *path);

/*
 * m(2) x n(3), b[m][n], [b11, b12, b13, b21, b22, b23]
 *
 *             b11 *b12  b13
 *             b21 *b22  b23
 *
 *  a11  a12   c11  c12  c13
 * *a21 *a22   c21 *c22  c23  c22 = a21 * b12 + a22 * b22
 *  a31  a32   c31  c32  c33
 *  a41  a42   c41  c42  c43
 */

#ifdef __GNUC__
/* 4 float vector type */
typedef float v4sf_t __attribute__((vector_size(sizeof(float) * 4)));

void moss_matrix_mul_v4sf(int am, int an, float *a, int bn, float *b, float *c);
#endif

void moss_matrix_mul_sw(int am, int an, float *a, int bn, float *b, float *c);
void moss_matrix_mul(int am, int an, float *a, int bn, float *b, float *c);

extern unsigned long moss_ts1_get(unsigned long *ts0);

/** @defgroup MOSS_TEST_API test case.
 * @ingroup MOSS_MISC_API
 * @brief Public method to write test case.
 *
 * - Test groups to tree.
 * - Test suite could contain test suites and cases.
 * - The tree runs deep first.
 * - Test case failure may break the containing suite.
 *
 *  @dot "Run test"
 *  digraph G {
 *    subgraph test_suite {
 *      setup;
 *      loop_content[label="loop content"];
 *      shutdown;
 *
 *      setup -> loop_content;
 *      setup -> failed_suite;
 *      loop_content -> shutdown;
 *    };
 *
 *    subgraph test_case {
 *      test_runner;
 *    };
 *
 *    subgraph loop_content {
 *
 *    	runner -> test_suite;
 *    	runner -> test_case;
 *
 *    };
 *
 *  }
 *  @enddot
 */

/** @addtogroup MOSS_TEST_API
 * @{
 */

/** Flags used in API.
 */
typedef enum moss_test_flag_enum {
	MOSS_FLAG_MASK(moss_test_flag_result, 0, 4),
	MOSS_FLAG(moss_test_flag_result, _pass, 0), /**< Test passed. */

	MOSS_FLAG(moss_test_flag_result,_failed, 1), /**< Test failure. */

	/** This test failed and populate to containing suite failure. */
	MOSS_FLAG(moss_test_flag_result,_failed_suite, 2),

	/** This test failed due to prerequisite, ie. former failed suite. */
	MOSS_FLAG(moss_test_flag_result,_prerequisite, 3),

	MOSS_FLAG_MASK(moss_test_flag_class, 4, 2),
	MOSS_FLAG(moss_test_flag_class, _case, 0),
	MOSS_FLAG(moss_test_flag_class, _suite, 1),
} moss_test_flag_t;

typedef struct moss_test_case_rec {
	const char *name, *cause;
	moss_tailq_entry_t qent;
	moss_test_flag_t (*run)(struct moss_test_case_rec*);
	moss_test_flag_t flag_class, flag_result;
} moss_test_case_t;

typedef struct moss_test_rec {
	moss_tailq_t cases;
	moss_test_case_t runner;
	moss_test_flag_t (*setup)(struct moss_test_rec*);
	void (*shutdown)(struct moss_test_rec*);
} moss_test_t;

/** Add to suite. */
#define MOSS_TEST_ADD(_baseobj, _obj) \
		TAILQ_INSERT_TAIL(&(_baseobj)->cases, &(_obj)->qent, entry)

/** Initialize test suite data structure. */
#define MOSS_TEST_INIT(_obj, _name) do { \
	memset(_obj, 0, sizeof(*(_obj))); \
	TAILQ_INIT(&(_obj)->cases); \
	(_obj)->runner.name = _name; \
	(_obj)->runner.flag_class = moss_test_flag_class_suite; \
	(_obj)->runner.run = moss_test_runner; \
} while(0)

#define MOSS_TEST_INIT2(_baseobj, _obj, _name) do { \
	MOSS_TEST_INIT(_obj, _name); \
	MOSS_TEST_ADD(_baseobj, &(_obj)->runner); \
} while(0)

/** Run test suite. */
#define MOSS_TEST_RUN(_obj) (_obj)->runner.run(&(_obj)->runner)

/** Initialize test case data structure. */
#define MOSS_TEST_CASE_INIT(_obj, _name, _runner) do { \
	memset(_obj, 0, sizeof(*(_obj))); \
	(_obj)->name = _name; \
	(_obj)->flag_class = moss_test_flag_class_case; \
	(_obj)->run = _runner; \
} while(0)

#define MOSS_TEST_CASE_INIT2(_baseobj, _obj, _name, _case_runner) do { \
	MOSS_TEST_CASE_INIT(_obj, _name, _case_runner); \
	MOSS_TEST_ADD(_baseobj, _obj); \
} while(0)

#define MOSS_TEST_CASE_INIT4(_baseobj, _name, _case_runner) do { \
	static moss_test_case_t _obj; \
	MOSS_TEST_CASE_INIT(&_obj, _name, _case_runner); \
	MOSS_TEST_ADD(_baseobj, &_obj); \
} while(0)

/** Show test suite report. */
#define MOSS_TEST_REPORT(_obj) do { \
	moss_test_report_t report_info = {0}; \
	moss_test_report(_obj, &report_info); \
	moss_debug("Report result %s, test suite[%s]\n" \
			"  Summary total cases PASS: %d, FAILED: %d(PREREQUISITE: %d), TOTAL: %d\n", \
			MOSS_TEST_RESULT_STR((_obj)->runner.flag_result, "UNKNOWN"), \
			(_obj)->runner.name, report_info.pass, report_info.failed, \
			report_info.failed_prereq, report_info.total); \
} while(0)

#define MOSS_TEST_RESULT_STR(_val, _unknown) ( \
		(_val) == moss_test_flag_result_pass ? "PASS" : \
		(_val) == moss_test_flag_result_failed ? "FAILED" : \
		(_val) == moss_test_flag_result_failed_suite ? "FAILED-SUITE" : \
		(_val) == moss_test_flag_result_prerequisite ? "FAILED-PREREQUISITE" : \
		_unknown)

#define MOSS_TEST_CLASS_STR(_val, _unknown) ( \
		(_val) == moss_test_flag_class_suite ? "suite" : \
		(_val) == moss_test_flag_class_case ? "case" : \
		_unknown)

#define MOSS_TEST_ASSERT_RETURN(_cond, _runner, _res) if (!(_cond)) { \
	(_runner)->cause = "#" MOSS_STRINGIFY2(__LINE__) \
			" " MOSS_STRINGIFY2(_cond); \
	return ((_runner)->flag_result = moss_test_flag_result_ ## _res); \
}

#define MOSS_TEST_ASSERT_THEN(_cond, _runner, _res) if (!(_cond)) { \
	(_runner)->cause = "#" MOSS_STRINGIFY2(__LINE__) \
			" " MOSS_STRINGIFY2(_cond); \
	(_runner)->flag_result = moss_test_flag_result_ ## _res; \
} if (!(_cond))

/** Start test suite.
 *
 *   Context to execute test case and reentrant for contained test suite.
 *
 *  **Reentrant with prerequisite failure suite:**
 *  ```
 *  - Skip setup
 *  - Populate prerequisite failure in contained test suite and case.
 *  ```
 *
 * @param
 * @return
 */
moss_test_flag_t moss_test_runner(moss_test_case_t*);

typedef struct moss_test_report_rec {
	int (*runner)(moss_test_case_t*, struct moss_test_report_rec*);
	int pass, failed, total, failed_prereq;
} moss_test_report_t;

int moss_test_report(moss_test_t*, moss_test_report_t*);

/**
 * @} MOSS_TEST_API
 */

#define _moss_int2hexstr(_d, _a) ((_d) < 10 ? (_d) + '0' : (_d) - 10 + (_a))
void moss_int2hexstr(void *_buf, unsigned val, int width, int cap);

/** @defgroup MOSS_TEST_API test case.
 * @ingroup MOSS_MISC_API
 * @brief Public method to write test case.
 */

/** @addtogroup MOSS_TEST_API
 * @{
 */
#define MOSS_SHOWHEX_BUF_MIN 78
typedef long (*moss_showhex_sout_t)(void*, const void*, unsigned long);
/** Hex dump to buffer.
 *
 * Example:
 * @code
 * 	char ln_buf[MOSS_SHOWHEX_BUF_MIN];
 * 	moss_buf_t hex_buf = {.data = _buf, .cap = _buf_sz};
 * 	moss_showhex(ln_buf, _data, _data_sz, _addr,
 * 	    (moss_showhex_sout_t)&moss_showhex_sout, &hex_buf);
 * @endcode
 *
 * @param _buf
 * @param _data
 * @param sz
 * @param _addr
 * @param sout
 * @param arg
 * @return
 */
int moss_showhex(void *_buf, const void *_data, size_t sz, unsigned long _addr,
		moss_showhex_sout_t sout, void *arg);
long moss_showhex_sout(moss_buf_t *buf, const void *msg, unsigned long len);

typedef volatile uint32_t* iop32_t;
typedef volatile uint16_t* iop16_t;
typedef volatile uint8_t* iop8_t;

/**
 * get mask of the bit[3..4]: MOSS_BITMASK(3, 2) -> 0x18
 */
#define MOSS_BITMASK(_shift, _bits) (((1 << (_bits)) - 1) << (_shift))

/**
 * set 0x2 into bit[3..4] of 0x5a: MOSS_BITWORD(0x5a, 3, 2, 0x2) -> 0x52
 */
#define MOSS_BITWORD(_word, _shift, _bits, _field) \
	(((_word) & ~MOSS_BITMASK(_shift, _bits)) | \
	(((_field) & MOSS_BITMASK(0, _bits)) << (_shift)))

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _H_MIGRU_TEST_GTRACK_MOSS */
