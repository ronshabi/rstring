/*
 * Copyright (c) 2025, Ron Shabi <ron@ronsh.net>
 * SPDX-License-Identifier: MIT
 *
 * rstring.c
 * ---------
 * Subroutines related to the 'rstring' data structure.
 */

#include <rstring/rstring.h>

/* Sentinel empty buffer */
char _rstring_empty[1];

void
rstring_init(struct rstring *rs)
{
    rs->len           = 0;
    rs->cap           = 0;
    rs->data          = _rstring_empty;
    _rstring_empty[0] = '\0';
}
