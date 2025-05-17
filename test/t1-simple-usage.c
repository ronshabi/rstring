#include <stdio.h>
#include <stdlib.h>

#include "common.h"

int
main()
{
    struct rstring x;
    struct rstring y;

    rstring_init(&x);
    rstring_push_byte(&x, 'h');
    rstring_push_byte(&x, 'e');
    rstring_push_byte(&x, 'l');
    rstring_push_byte(&x, 'l');
    rstring_push_byte(&x, 'o');

    printf("rstring_push_byte: (%s)\n", x.data);

    rstring_push_str(&x, " world!");

    printf("rstring_push_str: (%s)\n", x.data);

    rstring_init(&y);
    rstring_push_str(&y, "hello world!");

    if (rstring_cmp(&x, &y) != 0)
    {
        printf("comparison should have returned 0\n");
        exit(1);
    }

    rstring_free(&x);
    rstring_free(&y);

    return 0;
}
