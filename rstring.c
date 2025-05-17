/*
 * Copyright (c) 2025, Ron Shabi <ron@ronsh.net>
 * SPDX-License-Identifier: MIT
 *
 * rstring.c
 * ---------
 * Subroutines related to the 'rstring' data structure.
 */

#include <stdlib.h> /* malloc, realloc */

#include <string.h>  /* strlen, memcpy */
#include <strings.h> /* strncasecmp*/

#include "rstring.h"

/*----------------------------------------------------------------------------*/
/* STATIC BUFFERS                                                             */
/*----------------------------------------------------------------------------*/

/* Sentinel empty buffer */
char _rstring_empty[1];

/*----------------------------------------------------------------------------*/
/* INTERNAL MACROS                                                            */
/*----------------------------------------------------------------------------*/

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

static size_t
rstring_internal_find_first(const char *haystack, const char *needle,
                            size_t haystack_len, size_t needle_len, size_t from,
                            bool ignore_case)
{
    char *p = NULL;

    if (needle_len > haystack_len)
    {
        return RSTRING_NOT_FOUND;
    }

    if (from > haystack_len - needle_len)
    {
        return RSTRING_NOT_FOUND;
    }

    if (ignore_case)
    {
        p = strcasestr(haystack + from, needle);
    }
    else
    {
        p = strstr(haystack + from, needle);
    }

    if (!p)
    {
        return RSTRING_NOT_FOUND;
    }

    return p - haystack;
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

/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/

rstring_status_t
rstring_push(struct rstring *dest, const struct rstring *src)
{
    return rstring_internal_push(dest, src->data, src->len);
}

/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/

rstring_status_t
rstring_push_str(struct rstring *rs, const char *str)
{
    return rstring_internal_push(rs, str, strlen(str));
}

/*----------------------------------------------------------------------------*/

void
rstring_free(struct rstring *rs)
{
    if (rs->cap != 0)
    {
        free(rs->data);
        rs->len  = 0;
        rs->cap  = 0;
        rs->data = _rstring_empty;
    }
}

/*----------------------------------------------------------------------------*/

size_t
rstring_find_last_byte(const struct rstring *rs, uint8_t byte)
{
    const uint8_t *p = (uint8_t *) rs->data;

    if (rs->len == 0)
    {
        return RSTRING_NOT_FOUND;
    }

    if (rs->len == 1)
    {
        return (p[0] == byte) ? 0 : RSTRING_NOT_FOUND;
    }

    for (size_t i = rs->len - 1; i != 0; --i)
    {
        if (p[i] == byte)
        {
            return i;
        }
    }

    if (p[0] == byte)
    {
        return 0;
    }

    return RSTRING_NOT_FOUND;
}

/*----------------------------------------------------------------------------*/

size_t
rstring_find_first(const struct rstring *haystack, const struct rstring *needle,
                   size_t from)
{
    return rstring_internal_find_first(haystack->data,
                                       needle->data,
                                       haystack->len,
                                       needle->len,
                                       from,
                                       false);
}

/*----------------------------------------------------------------------------*/

size_t
rstring_find_first_str(const struct rstring *haystack, const char *needle,
                       size_t from)
{
    const size_t needle_len = strlen(needle);
    return rstring_internal_find_first(haystack->data,
                                       needle,
                                       haystack->len,
                                       needle_len,
                                       from,
                                       false);
}

/*----------------------------------------------------------------------------*/

size_t
rstring_find_first_ignore_case(const struct rstring *haystack,
                               const struct rstring *needle, size_t from)
{
    return rstring_internal_find_first(haystack->data,
                                       needle->data,
                                       haystack->len,
                                       needle->len,
                                       from,
                                       true);
}

/*----------------------------------------------------------------------------*/

size_t
rstring_find_first_str_ignore_case(const struct rstring *haystack,
                                   const char *needle, size_t from)
{
    const size_t needle_len = strlen(needle);
    return rstring_internal_find_first(haystack->data,
                                       needle,
                                       haystack->len,
                                       needle_len,
                                       from,
                                       true);
}
