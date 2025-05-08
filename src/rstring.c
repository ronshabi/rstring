/*
 * Copyright (c) 2025, Ron Shabi <ron@ronsh.net>
 * SPDX-License-Identifier: MIT
 *
 * rstring.c
 * ---------
 * Subroutines related to the 'rstring' data structure.
 */

#include <stdlib.h> /* malloc, realloc */

#include <string.h> /* strlen, memcpy */

#include <rstring/rstring.h>

/*----------------------------------------------------------------------------*/
/* STATIC BUFFERS                                                             */
/*----------------------------------------------------------------------------*/

/* Sentinel empty buffer */
char _rstring_empty[1];

/*----------------------------------------------------------------------------*/
/* INTERNAL MACROS                                                            */
/*----------------------------------------------------------------------------*/
#define RSTRING_INITIAL_CAPACITY 8

/*
 * Only call ensure_capacity when needed, because calling ensure_capacity
 * triggers either a malloc or a realloc, which is expensive.
 */
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

/*----------------------------------------------------------------------------*/
/* INTERNAL FUNCTIONS */
/*----------------------------------------------------------------------------*/

static rstring_status_t
rstring_internal_push(struct rstring *dest, const char *src, size_t n)
{
    if (n == 0)
    {
        return RSTRING_OK;
    }

    const size_t new_length = dest->len + n;
    ENSURE_CAPACITY(dest, new_length + 1);    // Add one for nullterm
    memcpy(dest->data + dest->len, src, n);
    dest->len              = new_length;
    dest->data[new_length] = '\0';
    return RSTRING_OK;
}

/*----------------------------------------------------------------------------*/
/* PUBLIC FUNCTIONS */
/*----------------------------------------------------------------------------*/

void
rstring_init(struct rstring *rs)
{
    rs->len           = 0;
    rs->data          = _rstring_empty;
    rs->cap           = 0;
    _rstring_empty[0] = '\0';
}

rstring_status_t
rstring_ensure_capacity(struct rstring *rs, const size_t wanted_cap)
{
    size_t new_size =
        rs->cap < RSTRING_INITIAL_CAPACITY ? RSTRING_INITIAL_CAPACITY : rs->cap;

    char *p = NULL;

    /* Grow by 1.5 */
    while (new_size < wanted_cap)
    {
        new_size = (new_size * 3) / 2;
    }

    if (rs->cap == 0)
    {
        p = malloc(new_size);
    }
    else
    {
        p = realloc(rs->data, new_size);
    }

    if (!p)
    {
        return RSTRING_ERROR_ALLOC;
    }

    rs->cap  = new_size;
    rs->data = p;

    return RSTRING_OK;
}

rstring_status_t
rstring_push(struct rstring *dest, const struct rstring *src)
{
    return rstring_internal_push(dest, src->data, src->len);
}

rstring_status_t
rstring_push_byte(struct rstring *rs, uint8_t byte)
{
    const size_t new_length = rs->len + 1;

    ENSURE_CAPACITY(rs, new_length + 1);

    ((uint8_t *) rs->data)[rs->len] = byte;
    rs->len                         = new_length;
    rs->data[rs->len]               = '\0';
    return RSTRING_OK;
}

rstring_status_t
rstring_push_str(struct rstring *rs, const char *str)
{
    return rstring_internal_push(rs, str, strlen(str));
}

void
rstring_free(struct rstring *rs)
{
    if (rs->cap != 0)
    {
        free(rs->data);
    }

    rs->len  = 0;
    rs->cap  = 0;
    rs->data = _rstring_empty;
}
