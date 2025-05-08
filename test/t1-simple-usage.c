#include <stdio.h>

#include <rstring/rstring.h>

int
main()
{
    struct rstring x;
    rstring_init(&x);
    rstring_push_byte(&x, 'h');
    rstring_push_byte(&x, 'e');
    rstring_push_byte(&x, 'l');
    rstring_push_byte(&x, 'l');
    rstring_push_byte(&x, 'o');

    printf("(%s)\n", x.data);

    rstring_free(&x);

    return 0;
}
