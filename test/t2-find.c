#include <stdio.h>
#include <stdlib.h>

#include <rstring/rstring.h>

struct rstring init_test(const char *test_name, const char *str)
{
    struct rstring rs;
    rstring_status_t rc;

    rstring_init(&rs);
    rc = rstring_push_str(&rs, str);

    if (rc != RSTRING_OK) {
        printf("Test '%s' failed: can't push string '%s'\n", test_name, str);
        exit(1);
    }

    return rs;
}

void find_first_byte_test(const char *str, uint8_t byte, size_t from, size_t result)
{
    struct rstring rs = init_test(__FUNCTION__, str);

    size_t given_result = rstring_find_first_byte(&rs, byte, from);

    if (result != given_result) {
        printf("Test find_first_byte_test failed:\n"
            "In string '%s', find byte 0x%02x, from offset %zu\n"
            "Result: %zu\n"
            "Expected: %zu\n", str, byte, from, given_result, result);
        exit(1);
    }

    rstring_free(&rs);
}

            "In string '%s', find byte 0x%02x\n"
            "Result: %zu\n"
            "Expected: %zu\n", str, byte, given_result, result);
        exit(1);
    }

    rstring_free(&rs);
}

int main()
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

    return 0;
}