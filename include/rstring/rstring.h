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

/**
 * @brief Finds the first occurrence of a byte in an rstring, from a given
 * offset.
 *
 * @param rs Pointer to the rstring to search in.
 * @param byte The byte value to search for.
 * @param from The offset from |rs->data| to start the search from.
 * @return The byte's offset from |rs->data| if found, else RSTRING_NOT_FOUND
 */
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

/**
 * @brief Finds the last last occurrence of a byte in an rstring.
 *
 * @param rs Pointer to the rstring to search in.
 * @param byte The byte value to search for.
 * @return The byte's offset from |rs->data| if found, else `RSTRING_NOT_FOUND`.
 */
size_t
rstring_find_last_byte(const struct rstring *rs, uint8_t byte);

/*----------------------------------------------------------------------------*/

/**
 * @brief Finds the first occurrence of an rstring inside another rstring, from
 * a given offset.
 *
 * @param haystack Pointer to the rstring to search in.
 * @param needle Pointer to the rstring to search for
 * @param from The offset to start searching from.
 * @return The offset from |rs->data| in which |needle| can be found, else
 * `RSTRING_NOT_FOUND`.
 */
size_t
rstring_find_first(const struct rstring *haystack, const struct rstring *needle,
                   size_t from);

/*----------------------------------------------------------------------------*/

/**
 * @brief Finds the first occurrence of a null-terminated C string inside
 * another rstring, from a given offset.
 *
 * @param haystack Pointer to the rstring to search in.
 * @param needle Pointer to the C string to to search for.
 * @param from The offset to start searching from.
 * @return The offset from |rs->data| in which |needle| can be found, else
 * `RSTRING_NOT_FOUND`.
 */
size_t
rstring_find_first_str(const struct rstring *haystack, const char *needle,
                       size_t from);

/*----------------------------------------------------------------------------*/

/**
 * @brief ASCII case-insensitive variant of rstring_find_first.
 */
size_t
rstring_find_first_ignore_case(const struct rstring *haystack,
                               const struct rstring *needle, size_t from);

/*----------------------------------------------------------------------------*/

/**
 * @brief ASCII case-insensitive variant of rstring_find_first_str.
 */
size_t
rstring_find_first_str_ignore_case(const struct rstring *haystack,
                                   const char *needle, size_t from);

/*----------------------------------------------------------------------------*/

/**
 * @brief Clears the contents of an rstring (lightweight version), resets
 * length.
 *
 * This function ensures the first byte of the rstring's data is zeroed, so that
 * other rstring (and C library) functions regard it as an empty string.
 *
 * In case the rstring was already an empty string, this is a no-op.
 *
 * @param rs Pointer to the rstring to clear.
 */
inline void
rstring_clear(struct rstring *rs)
{
    if (rs->cap)
    {
        rs->data[0] = '\0';
        rs->len     = 0;
    }
}

/*----------------------------------------------------------------------------*/

/**
 * @brief Checks if the rstring is an empty string.
 *
 * Emptyness-checking does not depend on whether the rstring is using
 * dynamically allocated memory or is pointing to a static sentinel buffer. It
 * depends only on the length specifier of the rstring structure, which is not
 * violated as long as usage is through rstring library functions, and memory is
 * not corrupted.
 *
 * @param rs Pointer to the rstring to check.
 * @return `true` if the rstring is empty, false otherwise.
 */
inline bool
rstring_is_empty(const struct rstring *rs)
{
    return rs->len == 0;
}

/*----------------------------------------------------------------------------*/

/**
 * @brief Frees the memory which was dynamically allocated by an rstring.
 *
 * If the rstring isn't using memory which was dynamically allocated, this is
 * equivalent to a no-op and is safe to use (for idiomatic reasons).
 *
 * Calling this function leads to resetting the length, capacity and data
 * buffers for the rstring it was called on, as if it were to be initialized
 * with `rstring_init`.
 *
 * After freeing, |rs| could be re-used given you initialize it first (this
 * maintains API stability).
 *
 * If freeing allocated memory fails, the behavior is undefined.
 *
 * @param rs Pointer to the rstring to free.
 */
void
rstring_free(struct rstring *rs);

#endif /* RSTRING_H */
