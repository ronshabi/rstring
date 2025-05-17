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

#define RSTRING_OK          0
#define RSTRING_ERROR_ALLOC 1
#define RSTRING_NOT_FOUND   (SIZE_MAX)

typedef size_t rstring_status_t;

#define RSTRING_INITIAL_CAPACITY 8

/*----------------------------------------------------------------------------*/

/**
 * @brief Initialize an rstring structure to an empty string.
 *
 * @param rs Pointer to the rstring to initialize.
 */
void
rstring_init(struct rstring *rs);

/*----------------------------------------------------------------------------*/

/**
 * @brief Append the contents of one rstring to another.
 *
 * This function appends the contents of |src| to the end of |dest|,
 * growing the destination buffer if needed.
 *
 * The resulting |dest| string remains null-terminated.
 *
 * @param dest Pointer to the destination rstring, which will be modified.
 * @param src Pointer to the source rstring to append.
 * @return `RSTRING_OK` on success, error code if memory
 * allocation fails.
 *
 * @note If allocation fails, |dest|'s underlying buffer is not corrupted.
 */
rstring_status_t
rstring_push(struct rstring *dest, const struct rstring *src);

/*----------------------------------------------------------------------------*/

/**
 * @brief Appends a single byte to the end of an rstring.
 *
 * This function adds one byte (which may be a non-printable or null byte)
 * to the end of the dynamic string. It ensures there is enough space and
 * preserves null-termination.
 *
 * @param rs Pointer to the rstring to append to.
 * @param byte The byte to append.
 * @return `RSTRING_OK` on success, error code if memory
 * allocation fails.
 *
 * @note If allocation fails, |rs|'s memory is not corrupted.
 */
rstring_status_t
rstring_push_byte(struct rstring *rs, uint8_t byte);

/*----------------------------------------------------------------------------*/

/**
 * @brief Appends a null-terminated C string to an rstring.
 *
 * This function appends the contents of |str| - a C string, to the end of the
 * rstring |rs|. The length of |str| is determined using `strlen`.
 * The resulting rstring remains null-terminated.
 *
 * @param rs Pointer to the rstring to append to.
 * @param str Pointer to the null-terminated C string to append
 * @return `RSTRING_OK` on success, error code if memory
 * allocation fails.
 *
 * @note If allocation fails, |rs|'s underlying buffer is not corrupted.
 */
rstring_status_t
rstring_push_str(struct rstring *rs, const char *str);

/*----------------------------------------------------------------------------*/

/**
 * @brief Ensures that an rstring has at least the specified capacity.
 *
 * If the current capacity of the string is less than |wanted_cap|, this
 * function reallocates the underlying buffer to accommodate at least
 * |wanted_cap| bytes.
 *
 * The growth strategy increases capacity by 1.5 until the requirement is met.
 *
 * If the current capacity is 0, it starts from `RSTRING_INITIAL_CAPACITY`.
 *
 * On success, the internal buffer pointer and capacity are updated.
 *
 * @param rs Pointer to the rstring to possibly grow.
 * @param wanted_cap  Minimum capacity required, in bytes.
 * @return `RSTRING_OK` on success, error code if memory
 * allocation fails.
 *
 * @note If allocation fails, |rs|'s underlying buffer and capacity are not
 * corrupted.
 */
rstring_status_t
rstring_ensure_capacity(struct rstring *rs, size_t wanted_cap);

/*----------------------------------------------------------------------------*/

/* FIXME: check max between rs1 and rs2's lengths. This may lead to bugs when
 * null bytes are in the middle of the strings? */

/**
 * @brief Compares the contents of two rstrings.
 *
 * Performs byte-wise comparison of the first |rs1|->len bytes and of |rs1| and
 * |rs2|.
 *
 * @param rs1 Pointer to the first rstring.
 * @param rs2 Pointer to the second rstring.
 * @return The equivalent result of a memcmp between the strings.
 */
static inline int
rstring_cmp(const struct rstring *rs1, const struct rstring *rs2)
{
    return memcmp(rs1->data, rs2->data, rs1->len);
}

/*----------------------------------------------------------------------------*/

/**
 * @brief Compares an rstring to a null-terminated C string.

 * This function performs a standard C-style `strcmp` between the contents
 * of the rstring |rs| and the given C string.

 * @param rs Pointer to the rstring.
 * @param str Pointer to the null-terminated C string.
 * @return The equivalent result of a strcmp between the strings.
 *
 * @note If the rstring contains null bytes, behavior is undefined.
 */
static inline int
rstring_cmp_str(const struct rstring *rs, const char *str)
{
    return strcmp(rs->data, str);
}

/*----------------------------------------------------------------------------*/

/**
 * @brief ASCII case-insensitive variant of rstring_cmp.

 * @param rs1 Pointer to the first rstring.
 * @param rs2 Pointer to the second rstring.
 * @return The equivalent result of a strcasecmp between the strings.
 */
static inline int
rstring_cmp_ignore_case(const struct rstring *rs1, const struct rstring *rs2)
{
    return strcasecmp(rs1->data, rs2->data);
}

/*----------------------------------------------------------------------------*/

/**
 * @brief ASCII case-insensitive variant of rstring_cmp_str.

 * @param rs Pointer to the rstring.
 * @param str Pointer to the null-terminated C string.
 * @return The equivalent result of a strcasecmp between the strings.
 */
static inline int
rstring_cmp_str_ignore_case(const struct rstring *rs, const char *str)
{
    return strcasecmp(rs->data, str);
}

/*----------------------------------------------------------------------------*/

/**
 * @brief Checks whether two rstrings are equal.
 *
 * @param rs1 Pointer to the first rstring.
 * @param rs2 Pointer to the second rstring.
 * @return true if the strings are equal in their lengths and contents
 * (byte-by-byte), false otherwise.
 */
static inline bool
rstring_equals(const struct rstring *rs1, const struct rstring *rs2)
{
    if (rs1->len != rs2->len)
    {
        return false;
    }

    return rstring_cmp(rs1, rs2) == 0;
}

/*----------------------------------------------------------------------------*/

/**
 * @brief Checks whether an rstring is equal to a null-terminated C string.
 *
 * @param rs Pointer to the rstring.
 * @param str Pointer to the null-terminated C string.
 * @return true if the strings are equal (byte-by-byte), false otherwise.
 */
static inline bool
rstring_equals_str(const struct rstring *rs, const char *str)
{
    return rstring_cmp_str(rs, str) == 0;
}

/*----------------------------------------------------------------------------*/

/**
 * @brief ASCII case-insensitive variant of rstring_cmp.
 */
static inline bool
rstring_equals_ignore_case(const struct rstring *rs1, const struct rstring *rs2)
{
    if (rs1->len != rs2->len)
    {
        return false;
    }

    return rstring_cmp_ignore_case(rs1, rs2) == 0;
}

/*----------------------------------------------------------------------------*/

/**
 * @brief ASCII case-insensitive variant of rstring_cmp_str.
 */
static inline bool
rstring_equals_str_ignore_case(const struct rstring *rs, const char *str)
{
    return rstring_cmp_str_ignore_case(rs, str) == 0;
}

/*----------------------------------------------------------------------------*/

static inline size_t
rstring_find_first_byte(const struct rstring *rs, uint8_t byte, size_t from)
{
    if (from >= rs->len)
    {
        return RSTRING_NOT_FOUND;
    }

    const char *p = memchr(rs->data + from, byte, rs->len - from);
    if (p == NULL)
    {
        return RSTRING_NOT_FOUND;
    }

    return p - rs->data;
}

/*----------------------------------------------------------------------------*/

size_t
rstring_find_last_byte(const struct rstring *rs, uint8_t byte);

/*----------------------------------------------------------------------------*/

size_t
rstring_find_first(const struct rstring *haystack, const struct rstring *needle,
                   size_t from);

/*----------------------------------------------------------------------------*/

size_t
rstring_find_first_str(const struct rstring *haystack, const char *needle,
                       size_t from);

/*----------------------------------------------------------------------------*/

size_t
rstring_find_first_ignore_case(const struct rstring *haystack,
                               const struct rstring *needle, size_t from);

/*----------------------------------------------------------------------------*/

size_t
rstring_find_first_str_ignore_case(const struct rstring *haystack,
                                   const char *needle, size_t from);

/*----------------------------------------------------------------------------*/

inline void
rstring_clear(struct rstring *rs)
{
    if (rs->cap)
    {
        rs->data[0] = '\0';
    }

    rs->len = 0;
}

/*----------------------------------------------------------------------------*/

inline bool
rstring_is_empty(const struct rstring *rs)
{
    return rs->len == 0;
}

/*----------------------------------------------------------------------------*/

void
rstring_free(struct rstring *rs);

#endif /* RSTRING_H */
