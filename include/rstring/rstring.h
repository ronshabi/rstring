/*
 * Copyright (c) 2025, Ron Shabi <ron@ronsh.net>
 * SPDX-License-Identifier: MIT
 */

#ifndef RSTRING_H
#define RSTRING_H

#include <stddef.h>
#include <stdint.h>

struct rstring
{
    size_t len;
    size_t cap;
    char  *data;
};

enum
{
    RSTRING_OK = 0,
    RSTRING_ERROR_ALLOC
};

typedef size_t rstring_status_t;

void
rstring_init(struct rstring *rs);

rstring_status_t
rstring_push_byte(struct rstring *rs, uint8_t byte);

rstring_status_t
rstring_ensure_capacity(struct rstring *rs, const size_t wanted_cap);

#endif /* RSTRING_H */
