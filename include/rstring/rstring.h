/*
 * Copyright (c) 2025, Ron Shabi <ron@ronsh.net>
 * SPDX-License-Identifier: MIT
 */

#ifndef RSTRING_H
#define RSTRING_H

#include <stdbool.h> /* bool, true, false */
#include <stddef.h>  /* size_t */
#include <stdint.h>  /* uint8_t */
#include <string.h>  /* memcmp */
#include <strings.h> /* strcasecmp */

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
rstring_push(struct rstring *dest, const struct rstring *src);

rstring_status_t
rstring_push_byte(struct rstring *rs, uint8_t byte);

rstring_status_t
rstring_push_str(struct rstring *rs, const char *str);

rstring_status_t
rstring_ensure_capacity(struct rstring *rs, const size_t wanted_cap);

static inline int
rstring_cmp(const struct rstring *rs1, const struct rstring *rs2)
{
    return memcmp(rs1->data, rs2->data, rs1->len);
}

static inline int
rstring_cmp_str(const struct rstring *rs, const char *str)
{
    return strcmp(rs->data, str);
}

static inline int
rstring_cmp_ignore_case(const struct rstring *rs1, const struct rstring *rs2)
{
    return strcasecmp(rs1->data, rs2->data);
}

static inline int
rstring_cmp_str_ignore_case(const struct rstring *rs, const char *str)
{
    return strcasecmp(rs->data, str);
}

static inline bool
rstring_equals(const struct rstring *rs1, const struct rstring *rs2)
{
    if (rs1->len != rs2->len)
    {
        return false;
    }

    return rstring_cmp(rs1, rs2) == 0;
}

static inline bool
rstring_equals_str(const struct rstring *rs, const char *str)
{
    return rstring_cmp_str(rs, str) == 0;
}

static inline bool
rstring_equals_ignore_case(const struct rstring *rs1, const struct rstring *rs2)
{
    if (rs1->len != rs2->len)
    {
        return false;
    }

    return rstring_cmp_ignore_case(rs1, rs2) == 0;
}

static inline bool
rstring_equals_str_ignore_case(const struct rstring *rs, const char *str)
{
    return rstring_cmp_str_ignore_case(rs, str) == 0;
}

inline void
rstring_clear(struct rstring *rs)
{
    if (rs->cap)
    {
        rs->data[0] = '\0';
    }

    rs->len = 0;
}

void
rstring_free(struct rstring *rs);

#endif /* RSTRING_H */
