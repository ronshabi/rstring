/*
 * Copyright (c) 2025, Ron Shabi <ron@ronsh.net>
 * SPDX-License-Identifier: MIT
 */

#ifndef RSTRING_H
#define RSTRING_H

#include <stddef.h>

struct rstring
{
    size_t len;
    size_t cap;
    char  *data;
};

void rstring_init(struct rstring *rs);

#endif /* RSTRING_H */
