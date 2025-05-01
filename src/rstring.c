/*
 * Copyright (c) 2025, Ron Shabi <ron@ronsh.net>
 * SPDX-License-Identifier: MIT
 *
 * rstring.c
 * ---------
 * Subroutines related to the 'rstring' data structure.
 */

#include <rstring/rstring.h>

/* malloc, realloc */
#include <stdlib.h>


#define RSTRING_INITIAL_CAPACITY 8

/* Sentinel empty buffer */
char _rstring_empty[1];


/*----------------------------------------------------------------------------*/
/* INTERNAL MACROS                                                            */
/*----------------------------------------------------------------------------*/

#define ENSURE_CAPACITY(rs, newcap)                                            \
    do                                                                         \
    {                                                                          \
        if (newcap >= rs->cap)                                                 \
        {                                                                      \
            rstring_status_t __temp_rc = rstring_ensure_capacity(rs, newcap);  \
            if (__temp_rc != RSTRING_OK)                                       \
            {                                                                  \
                return __temp_rc;                                              \
            }                                                                  \
        }                                                                      \
    } while (0)


    
void rstring_init(struct rstring *rs)
{
    rs->len           = 0;
    rs->cap           = 0;
    rs->data          = _rstring_empty;
    _rstring_empty[0] = '\0';
}

rstring_status_t
rstring_ensure_capacity(struct rstring *rs, const size_t wanted_cap)
{
    size_t new_size = rs->cap < RSTRING_INITIAL_CAPACITY ? RSTRING_INITIAL_CAPACITY : rs->cap;

    char *p = NULL;

    /* Grow by 1.5 */
    while (new_size < wanted_cap) {
        new_size = (new_size * 3) / 2;
    }

    if (rs->cap == 0) {
        p = malloc(new_size);
    } else {
        p = realloc(rs->data, new_size);
    }

    if (!p) {
        return RSTRING_ERROR_ALLOC;
    }

    rs->cap = new_size;
    rs->data = p;

    return RSTRING_OK;
}
