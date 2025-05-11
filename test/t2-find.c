#define _GNU_SOURCE /* strcasestr */

#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>

#include <rstring/rstring.h>

#include "common.h"

struct rstring
init_test(const char *test_name, const char *str)
{
    struct rstring   rs;
    rstring_status_t rc;

    rstring_init(&rs);
    rc = rstring_push_str(&rs, str);

    if (rc != RSTRING_OK)
    {
        test_fail(test_name, "can't push string '%s'\n", str);
    }

    return rs;
}

void
find_first_byte_test(const char *str, uint8_t byte, size_t from, size_t result)
{
    struct rstring rs = init_test(__FUNCTION__, str);

    size_t given_result = rstring_find_first_byte(&rs, byte, from);

    if (result != given_result)
    {
        test_fail(__FUNCTION__,
                  "In string '%s', find byte 0x%02x, from offset %zu\n"
                  "Result: %zu\n"
                  "Expected: %zu\n",
                  str,
                  byte,
                  from,
                  given_result,
                  result);
    }

    rstring_free(&rs);
}

void
find_last_byte_test(const char *str, uint8_t byte, size_t result)
{
    struct rstring rs = init_test(__FUNCTION__, str);

    size_t given_result = rstring_find_last_byte(&rs, byte);

    if (result != given_result)
    {
        test_fail(__FUNCTION__,
                  "In string '%s', find byte 0x%02x\n"
                  "Result: %zu\n"
                  "Expected: %zu\n",
                  str,
                  byte,
                  given_result,
                  result);
    }

    rstring_free(&rs);
}

void
find_first_test(const char *haystack, const char *needle, size_t from,
                bool ignore_case, size_t result)
{
    // Here I'm gonna test both the regular and the 'str' variant of these
    // functions. They should return the same results irrelevent to how what is
    // the type of the second argument.

    struct rstring rs_haystack =
        init_test("find_first_test (haystack)", haystack);
    struct rstring rs_needle = init_test("find_first_test (needle)", needle);

    size_t res_rs_rs  = 0;
    size_t res_rs_str = 0;

    if (ignore_case)
    {
        res_rs_rs =
            rstring_find_first_ignore_case(&rs_haystack, &rs_needle, from);
        res_rs_str =
            rstring_find_first_str_ignore_case(&rs_haystack, needle, from);
    }
    else
    {
        res_rs_rs  = rstring_find_first(&rs_haystack, &rs_needle, from);
        res_rs_str = rstring_find_first_str(&rs_haystack, needle, from);
    }

    if (res_rs_rs != result || res_rs_str != result)
    {
        test_fail(__FUNCTION__,
                  "\n\t> haystack: '%s'\n"
                  "\t> needle: '%s'\n"
                  "\t> from: %zu\n"
                  "\t> ignore case?: %s\n"
                  "\t> expected: %zu\n"
                  "\t> result of regular variant: %zu\n"
                  "\t> result of str variant: %zu\n",
                  haystack,
                  needle,
                  from,
                  ignore_case ? "true" : "false",
                  result,
                  res_rs_rs,
                  res_rs_str);
    }

    rstring_free(&rs_haystack);
    rstring_free(&rs_needle);
}

int
main()
{
    find_first_byte_test("", 0x00, 0, RSTRING_NOT_FOUND);
    find_first_byte_test(" ", 0x00, 0, RSTRING_NOT_FOUND);
    find_first_byte_test(" ", ' ', 0, 0);
    find_first_byte_test("a ", ' ', 0, 1);
    find_first_byte_test(" aaa ", 'a', 0, 1);
    find_first_byte_test(" aaa ", 'a', 1, 1);
    find_first_byte_test(" aaa ", 'a', 2, 2);
    find_first_byte_test(" aaa ", 'a', 3, 3);
    find_first_byte_test(" aaa ", ' ', 3, 4);
    find_first_byte_test(" aaa ", 'a', 50, RSTRING_NOT_FOUND);
    find_first_byte_test(" aaa ", '?', 0, RSTRING_NOT_FOUND);

    find_last_byte_test("", 0x00, RSTRING_NOT_FOUND);
    find_last_byte_test(" ", 0x00, RSTRING_NOT_FOUND);
    find_last_byte_test(" ", ' ', 0);
    find_last_byte_test(" abc", 'a', 1);
    find_last_byte_test(" \x80\x90\xFC\x10\x20\xFC", '\xFC', 6);
    find_last_byte_test(" \x80\x90\xFC\x10\x20\xFC", '\x20', 5);

    return 0;
}
