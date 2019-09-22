#include <moss/moss.h>

static int moss_rb_cmp(moss_rb_entry_t *a, moss_rb_entry_t *b)
{
	if (a->cmp) return (a->cmp)(a, b);
	return (long)a - (long)b;
}
RB_GENERATE(moss_rb_tree_rec, moss_rb_entry_rec, entry, moss_rb_cmp);

size_t moss_stripl(const void **buf, size_t sz, const char *ext) {
	while (sz > 0) {
		if ((*(char**)buf)[0] == '\0' || (ext && strchr(ext, (*(char**)buf)[0]))) {
			*(char**)buf += 1;
			sz--;
			continue;
		}
		break;
	}
	return sz;
}

int moss_buf_read(moss_buf_t *buf, void *data, size_t sz) {
	size_t _sz = sz = MOSS_MIN(sz, buf->lmt);
	while (sz > 0) {
		char *data_pos = (char*)buf->data + buf->pos;
		int data_sz = buf->cap - buf->pos;
		if (data_sz > sz) data_sz = sz;
		memcpy(data, data_pos, data_sz);
		if ((buf->pos += data_sz) >= buf->cap)
			buf->pos -= buf->cap;
		buf->lmt -= data_sz;
		sz -= data_sz;
		data = (char*)data + data_sz;
	}
	return _sz;
}

int moss_buf_expand(moss_buf_t *buf, size_t cap, int retain) {
	void *data;

	if (cap <= 0 || buf->cap >= cap) return 0;
	cap = (cap + 0x3fff) & ~0x3fff;
	if (!(data = malloc(cap))) {
		return -1;
	}
	if (buf->data) {
		if (retain && buf->lmt > 0) {
			moss_buf_read(buf, data, buf->lmt);
			buf->pos = 0;
		}
		free(buf->data);
	}
	buf->data = data;
	buf->cap = cap;
	return 0;
}

int moss_buf_vprintf(moss_buf_t *buf, const char *fmt, va_list va) {
	int r;
	char ch, *ch_pos = NULL;

	if (!fmt) return 0;
	if (buf->pos + buf->lmt < buf->cap) {
		ch = *(ch_pos = (char*)buf->data + buf->pos + buf->lmt);
	}
	r = vsnprintf((char*)buf->data + buf->pos + buf->lmt,
			buf->cap - buf->pos - buf->lmt, fmt, va);
	if (buf->pos + buf->lmt + r >= buf->cap) {
		if (ch_pos) *ch_pos = ch;
		return -1;
	}
	buf->lmt += r;
	return 0;
}

int moss_buf_printf(moss_buf_t *buf, const char *fmt, ...) {
	int r;
	va_list va;

	if (!fmt) return 0;
	va_start(va, fmt);
	r = moss_buf_vprintf(buf, fmt, va);
	va_end(va);
	return r;
}

int moss_vlogf(moss_buf_t *buf, unsigned flag, const char *tag, long lno,
		const char *fmt, va_list va) {
	char tm_str[32];
	moss_buf_t tm_buf = {.data = tm_str, .cap = sizeof(tm_str) - 1};

	if (moss_logt(&tm_buf, flag, tag, lno) != 0) {
		((char*)tm_buf.data)[tm_buf.lmt = 0] = '\0';
	} else {
		((char*)tm_buf.data)[tm_buf.lmt++] = ' ';
		((char*)tm_buf.data)[tm_buf.lmt++] = '\0';
	}

	if (moss_buf_printf(buf, "%s %s%s #%ld ",
			moss_level_str(flag & moss_log_level_mask, ""),
			tm_str, tag, lno) != 0) {
		return -1;
	}

	if (moss_buf_vprintf(buf, fmt, va) != 0) {
		return -1;
	}
	return 0;
}

int moss_logf(moss_buf_t *buf, unsigned flag, const char *tag, long lno,
		const char *fmt, ...) {
	va_list va;
	int r;

	va_start(va, fmt);
	r = moss_vlogf(buf, flag, tag, lno, fmt, va);
	va_end(va);
	return r;
}

int moss_readline(int (*getc)(void *arg), void *arg, char *_nl)
{
	int c, n;
	char nl;

	for (n = 0, nl = 0; (c = (*getc)(arg)) >= 0; n++) {
		if (c == MOSS_LF) {
			if (_nl) *_nl = nl + 1;
			return n - nl;
		}
		nl = (c == MOSS_CR);
	}
	if (_nl) *_nl = 0;
	return (n <= 0) ? -1 : n;
}

int moss_log_level_max = moss_log_level_info;

#ifdef __GNUC__
void moss_matrix_mul_v4sf(int am, int an, float *a, int bn, float *b,
		float *c) {
	int m, n;

	for (m = 0; m < am; m++) {
		for (n = 0; n < bn; n++) {
			v4sf_t va, vb, vc;

			switch(an) {
			case 4:
				va[3] = a[3]; vb[3] = b[n + bn + bn + bn];
			case 3:
				va[2] = a[2]; vb[2] = b[n + bn + bn];
			case 2:
				va[1] = a[1]; vb[1] = b[n + bn];
			case 1:
			default:
				va[0] = a[0]; vb[0] = b[n];
			}

			vc = va * vb;
			*c = vc[0];

			switch(an) {
			case 4:
				*c += vc[3];
			case 3:
				*c += vc[2];
			case 2:
				*c += vc[1];
			case 1:
			default:
				;
			}
			c++;
		}
		a += an;
	}
}
#endif

void moss_matrix_mul_sw(int am, int an, float *a, int bn, float *b,
		float *c) {
	int m, n;

	for (m = 0; m < am; m++) {
		for (n = 0; n < bn; n++) {
			int z, bm;
			register float cr;

			for (cr = a[0] * b[n], bm = n + bn, z = 1; z < an; bm += bn, z++) {
				cr += a[z] * b[bm];
			}
			*c++ = cr;
		}
		a += an;
	}
}

void moss_matrix_mul(int am, int an, float *a, int bn, float *b,
		float *c) {
#if defined(__GNUC__) && 0
	if (an <= 4) {
		moss_matrix_mul_v4sf(am, an, a, bn, b, c);
		return;
	}
#endif
	moss_matrix_mul_sw(am, an, a, bn, b, c);
}

moss_test_flag_t moss_test_runner(moss_test_case_t *suite_runner) {
	moss_test_t *suite = MOSS_CONTAINER_OF(suite_runner, moss_test_t, runner);
	moss_tailq_entry_t *qent;

	moss_debug("Start test suite[%s]\n", suite->runner.name);

	// The most use-case for setup on suite
	if ((suite_runner->flag_result == moss_test_flag_result_pass) &&
			suite->setup &&
			((suite->setup)(suite) != moss_test_flag_result_pass)) {
		suite_runner->flag_result = moss_test_flag_result_failed_suite;
		if (!suite_runner->cause) suite_runner->cause = "SETUP";
		moss_debug("Setup failed test suite[%s]\n", suite_runner->name);
	}

	TAILQ_FOREACH(qent, &suite->cases, entry) {
		moss_test_case_t *case_runner = MOSS_CONTAINER_OF(qent,
				moss_test_case_t, qent);

		// Populate prerequisite to contained suites and cases.
		if ((suite_runner->flag_result == moss_test_flag_result_failed_suite) ||
				(suite_runner->flag_result == moss_test_flag_result_prerequisite)) {
			case_runner->flag_result = moss_test_flag_result_prerequisite;
			case_runner->cause = "PREREQUISITE";
			moss_debug("%s for test %s[%s]\n",
					MOSS_TEST_RESULT_STR(case_runner->flag_result, "UNKNOWN result"),
					MOSS_TEST_CLASS_STR(case_runner->flag_class, "UNKNOWN class"),
					case_runner->name);

			if (case_runner->flag_class == moss_test_flag_class_suite) {
				(case_runner->run)(case_runner);
			}
			continue;
		}

		// Test suite failure do not break containing suite.
		if (case_runner->flag_class == moss_test_flag_class_suite) {
			case_runner->flag_result = (case_runner->run)(case_runner);
			if (case_runner->flag_result != moss_test_flag_result_pass) {
				if (!case_runner->cause) case_runner->cause = "RUN";
				if (suite_runner->flag_result == moss_test_flag_result_pass) {
					suite_runner->flag_result = moss_test_flag_result_failed;
					suite_runner->cause = case_runner->name;
				}
			}
			continue;
		}

		moss_debug("Start test case[%s]\n", case_runner->name);
		if ((case_runner->flag_result = (case_runner->run)(case_runner)) !=
				moss_test_flag_result_pass) {
			if (!case_runner->cause) case_runner->cause = "RUN";
			moss_debug("%s for test case[%s]\n"
					"  Cause: %s\n",
					MOSS_TEST_RESULT_STR(case_runner->flag_result, "UNKNOWN result"),
					case_runner->name, case_runner->cause);
			if (suite_runner->flag_result < case_runner->flag_result) {
				suite_runner->flag_result = case_runner->flag_result;
				suite_runner->cause = case_runner->name;
				moss_debug("%s for test suite[%s]\n"
						"  Cause: %s\n",
						MOSS_TEST_RESULT_STR(suite_runner->flag_result, "UNKNOWN result"),
						suite_runner->name, suite_runner->cause);
			}
		}
		moss_debug("Stopped test case[%s]\n", case_runner->name);
	}

	if (suite->shutdown) (suite->shutdown)(suite);

	moss_debug("Stopped test suite[%s]\n", suite_runner->name);

	return suite_runner->flag_result;
}

int moss_test_report(moss_test_t *suite, moss_test_report_t *report_runner) {
	moss_tailq_entry_t *qent;
	int r = 0, pass = 0, failed = 0, total = 0, failed_prereq = 0;

	TAILQ_FOREACH(qent, &suite->cases, entry) {
		moss_test_case_t *case_runner = MOSS_CONTAINER_OF(qent,
				moss_test_case_t, qent);

		if (report_runner && report_runner->runner &&
				((r = (*report_runner->runner)(case_runner, report_runner)) != 0)) {
			moss_error("Report runner break\n");
			break;
		}

		if (case_runner->flag_class == moss_test_flag_class_suite) {
			if ((r = moss_test_report(MOSS_CONTAINER_OF(case_runner,
					moss_test_t, runner), report_runner)) != 0) {
				moss_error("Report suite break\n");
				break;
			}
			continue;
		}
		total++;
		switch(case_runner->flag_result) {
		case moss_test_flag_result_pass:
			pass++;
			break;
		case moss_test_flag_result_failed:
			failed++;
			break;
		case moss_test_flag_result_failed_suite:
			failed++;
			break;
		case moss_test_flag_result_prerequisite:
			failed++;
			failed_prereq++;
			break;
		default:
			failed++;
			break;
		}
		if (case_runner->flag_result == moss_test_flag_result_failed ||
				case_runner->flag_result == moss_test_flag_result_failed_suite) {
			moss_debug("Report result %s, test case[%s], #%d in suite[%s]\n"
					"  Cause: %s\n",
					MOSS_TEST_RESULT_STR(case_runner->flag_result, "UNKNOWN"),
					case_runner->name, total, suite->runner.name,
					(case_runner->cause ? case_runner->cause : "UNKNOWN"));
		} else {
			moss_debug("Report result %s, test case[%s], #%d in suite[%s]\n",
					MOSS_TEST_RESULT_STR(case_runner->flag_result, "UNKNOWN"),
					case_runner->name, total, suite->runner.name);
		}
	}

	moss_debug("%s result %s, test suite[%s]\n"
			"  Summary test cases PASS: %d, FAILED: %d(PREREQUISITE: %d), TOTAL: %d\n",
			(r != 0 ? "Report(incomplete)" : "Report"),
			MOSS_TEST_RESULT_STR(suite->runner.flag_result, "UNKNOWN"),
			suite->runner.name, pass, failed, failed_prereq, total);

	if (report_runner) {
		report_runner->total += total;
		report_runner->pass += pass;
		report_runner->failed += failed;
		report_runner->failed_prereq += failed_prereq;
	}
	return r;
}

void moss_int2hexstr(void *_buf, unsigned val, int width, int cap) {
	char *buf = (char*)_buf;
	int nw;

	if (!buf || width < 1) return;
	buf += width;

	nw = MOSS_MIN(width, sizeof(val) * 2);
	width -= nw;
	while (nw-- > 0) {
		int d = val & 0xf;
		*--buf = _moss_int2hexstr(d, cap);
		val >>= 4;
	}
	while (width-- > 0) {
		*--buf = '?';
	}
}

int moss_showhex(void *_buf, const void *_data, size_t sz, unsigned long _addr,
		moss_showhex_sout_t sout, void *arg) {
#if MOSS_SHOWHEX_BUF_MIN < 78
#  error "MOSS_SHOWHEX_BUF_MIN minimal 78"
#endif
/* 0         1         2         3         4         5         6         7
 * 7FFF 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F | 0123456789abcdef
 *      ----------------------------------------------- | ----------------
 * DDBE                                           1f 20 |               .
 * DDC0 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f 30 | !"#$%&'()*+,-./0
 */
	char *buf = (char*)_buf, *data = (char*)_data;
	int hpos = 0, r = 0, nlen = strlen(moss_newline);
	unsigned addr;

	addr = (unsigned)(_addr >= 0x10000lu ? _addr / 0x10000lu : 0);
	moss_int2hexstr(buf, addr, 4, 'A');
	buf[4] = ' ';
	snprintf(buf + 5, MOSS_SHOWHEX_BUF_MIN - 5,
			"00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F | 0123456789abcdef%s", moss_newline);
	if ((r = sout(arg, buf, 71 + nlen)) < 0) return r;

	snprintf(buf, MOSS_SHOWHEX_BUF_MIN,
			"     ----------------------------------------------- | ----------------%s", moss_newline);
	if ((r = sout(arg, buf, 71 + nlen)) < 0) return r;

	addr = (unsigned)_addr;
	while (sz > 0) {
		int i;

		if (hpos == 0) {
			moss_int2hexstr(buf, addr & (~0xf), 4, 'A');
			for (i = 4; i < 71; i++) buf[i] = ' ';
			buf[53] = '|';
		}
		if ((addr & 0xf) == hpos) {
			moss_int2hexstr(buf + hpos * 3 + 5, *data, 2, 'a');
			buf[hpos + 55] = isprint(*data) ? *data : '.';
			data++;
			addr++;
			sz--;
		}
		if (++hpos > 0xf || sz == 0) {
			hpos = 0;
			memcpy(buf + 71, moss_newline, nlen);
			buf[71 + nlen] = '\0';
			if ((r = sout(arg, buf, 71 + nlen) < 0)) return r;
		}
	}
	return r;
}

long moss_showhex_sout(moss_buf_t *buf, const void *msg, unsigned long len) {
	if (buf->pos >= buf->cap) return -1;
	if (len > buf->cap - buf->pos - 1) len = buf->cap - buf->pos - 1;
	if (len > 0) memcpy((char*)buf->data + buf->pos, msg, len);
	((char*)buf->data)[buf->pos += len] = '\0';
	if (buf->pos + 1 >= buf->cap) return -1;
	return 0;
}
