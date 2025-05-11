#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <stdarg.h>

// Common function every test is most likely to use
// clang-tidy: nolint (testname and fmt are both char* arguments)
__attribute__((format(printf, 2, 3), noreturn)) static inline void
test_fail(const char *testname, const char *fmt, ...)    // NOLINT
{
    va_list ap;
    va_start(ap, fmt);

    printf("[FAIL: %s]  ", testname);

    vprintf(fmt, ap);

    va_end(ap);

    printf("\n");
    exit(1);    // NOLINT (exit is not thread safe)
}
