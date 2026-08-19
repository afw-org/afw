// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW - String Functions
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_UTF8_H__
#define __AFW_UTF8_H__

#include "afw_minimal.h"

/**
 * @addtogroup afw_utf8
 * @{
 */

/**
 * @file afw_utf8.h
 * @brief UTF-8 string types and public string helpers.
 *
 * `afw_utf8_t` is a pointer + length. It does **not** know a pool and has
 * no refcount. Adaptive values (`const afw_value_t *`) are what can hold
 * a lifetime. See @ref afw_utf8 and `designs/c-naming-and-payloads.md`.
 *
 * ## Internal vs **external**
 *
 * **Internal** is AFW's NFC world: `afw_utf8_t`, `.s` + `.len`. Slices,
 * hashes, `memcpy`, `stream_write`, curl POSTFIELDS+SIZE. `.s` is octets
 * for `.len` bytes. There is usually **no** trailing `0` at `s[len]`.
 * The bytes **may contain U+0000**.
 *
 * **External** is not that world (libc, APR paths, LDAP DNs, libxml
 * regexp, logs, env names). Say **external C string** or
 * **external octets** when you need the shape. Cross with a named door:
 *
 * | External | Door |
 * |----------|------|
 * | C string | `afw_utf8_to_utf8_z` / `z_create` (throw if interior `0`) |
 * | Encode for logs/names | `forced_safe` (not promised NFC) |
 * | Octets + size | `as_memory` / write with `len` (NULs are data) |
 *
 * Generated literals: `afw_s_*` (internal) and `afw_z_*` (external C
 * string of the same `"…"`). `afw_utf8_utf8_z_t` is the utf8 + z pair
 * when the buffer is already a C string (see `afw_common.h`).
 *
 * Do **not** pass `string->s` to `fopen`, `strlen`, `strcmp`, or any
 * external C-string API.
 *
 * ## Naming
 *
 * **Less in the name, more we do.** Extra words take a safety off or pick
 * a non-default policy. **`p` only if something new lives there.**
 *
 * Prefix **`afw_utf8_z_`** when you **have** a C string. Prefix
 * **`afw_utf8_`** when you have length-prefixed `utf8`. Mixed predicates
 * spell both types in argument order (`starts_with_utf8_z`).
 *
 * | Name | What we do |
 * |------|------------|
 * | `create` | New struct in `p`, **copy** octets, NFC or throw |
 * | `create_no_copy` | New struct in `p`, **point** at `s`, NFC or throw |
 * | `z_to_utf8` | Ingest `utf8_z` (copy + NFC) |
 * | `z_as_utf8` | Ingest `utf8_z` (point; already NFC) |
 * | `z_set` / `z_set_no_copy` | Fill yours from a `utf8_z` |
 * | `set` / `set_no_copy` | Fill yours from octets + len |
 * | `clone` | Copy an existing `afw_utf8_t` (struct + bytes) |
 * | `to_utf8_z` / `z_create` | External C string (throw if embedded 0) |
 * | `forced_safe` | External encode; invalid/Cc as `^hex^`; not NFC; not a value |
 * | `create_property_name` | Same encode, then NFC (property name only) |
 *
 * `AFW_UTF8_LITERAL` is a trusted C `"…"` initializer (no check).
 */

AFW_BEGIN_DECLARES

/**
 * @brief Callback function for creating a string from an adaptive value.
 * @param value
 * @param own Indicate to only include own properties of any objects.
 * @param xctx of caller.
 * @return string
 */
typedef const afw_utf8_t *
(AFW_CALLBACK *afw_utf8_from_value_t) (
    const afw_value_t *value,
    afw_boolean_t own,
    afw_xctx_t *xctx);


/**
 * @brief Callback function for converting a string to an adaptive value.
 * @param from_utf8 utf-8 string to convert.
 * @param path Object path to use if string is an object or NULL.
 * @param xctx of caller.
 * @return value
 *
 * The path parameter is only used if the string being converted is an object
 * entity.
 *
 * The path must be of the form `/<adapter id>/<object type>/<id>`.  This path
 * is parsed to provide return values for the object's
 * afw_object_meta_get_object_id(), afw_object_meta_get_path(), and
 * afw_object_get_object_type() methods.
 */
typedef const afw_value_t *
(AFW_CALLBACK *afw_utf8_to_value_t) (
    const afw_utf8_t *from_utf8,
    const afw_utf8_t *path,
    afw_xctx_t *xctx);


/*  @brief Options for function afw_utf8_nfc(). */
typedef enum afw_utf8_nfc_option_e {

    /**
     * @brief Only check that input is valid UTF-8.
     *
     * If input is valid UTF-8 encoding, NULL is returned.  Otherwise, a
     * non-NULL pointer is returned.
     */
    afw_utf8_nfc_option_is_valid,

    /**
     * @brief Only check that input is UTF-8 NFC normalized.
     *
     * If input is valid NFC normalized UTF-8 encoding, NULL is returned.
     * Otherwise, a non-NULL pointer is returned.
     */
    afw_utf8_nfc_option_is_nfc,

    /** @brief If s is already normalized, use it directly for result->s. */
    afw_utf8_nfc_option_create,

    /** @brief If s is already normalized, make copy for result->s. */
    afw_utf8_nfc_option_create_copy

} afw_utf8_nfc_option_t;


/**
 * @brief UTF-8 NFC support function.
 * @param s pointer to utf-8 characters.
 * @param len is number of bytes.
 * @param option see afw_utf8_nfc_option_t.
 * @param p pool used for result.
 * @param xctx of caller.
 * @return NFC normalized utf8 string or NULL.
 *
 * See afw_utf8_nfc_option_t for more information.
 *
 * This function is designed to do as little work as possible to insure
 * that resulting utf8 string is NFC normalize.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_utf8_nfc(
    const afw_utf8_octet_t *s, afw_size_t len,
    afw_utf8_nfc_option_t option,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Get next codepoint in utf-8.
 * @param s is pointer to utf-8.
 * @param offset is address of offset in utf-8 string.  Will be incremented.
 * @len of s.
 * @param xctx of caller.
 * @return codepoint or -1 if invalid.
 * 
 * The offset incremented past the bytes making up the utf-8 codepoint
 * if the codepoint is valid.  If you don't want this behavior, make a copy
 * of offset to a temporary variable and past it to function.
 */
AFW_DECLARE(afw_code_point_t)
afw_utf8_next_code_point(const afw_utf8_octet_t *s, afw_size_t *offset,
    afw_size_t len, afw_xctx_t *xctx);


/**
 * @brief Convert a code point to utf8.
 * @param utf8 an array of 5 bytes to put result followed by a 0 byte.
 * @param cp to convert.
 * @param xctx of caller.
 * @return false if cp is not valid.
 */
AFW_DECLARE(afw_boolean_t)
afw_utf8_from_code_point(afw_utf8_octet_t utf8_z[5], afw_code_point_t cp,
    afw_xctx_t *xctx);


/**
 * @brief Determine if series of bytes is valid utf-8.
 * @param s pointer to bytes to be tested.
 * @param len is number of bytes.
 * @param xctx of caller.
 * @return true if valid utf-8.
 */
#define afw_utf8_is_valid(s, len, xctx) \
    (afw_utf8_nfc(s, len, afw_utf8_nfc_option_is_valid, \
        (xctx)->p, xctx) == NULL)



/**
 * @brief Determine if series of bytes is NFC normalized utf-8.
 * @param s pointer to bytes to be tested.
 * @param len is number of bytes.
 * @param p used if needed for test.
 * @param xctx of caller.
 * @return true if valid utf-8 and NFC normalize.
 */
#define afw_utf8_is_nfc(s, len, p, xctx) \
    (afw_utf8_nfc(s, len, afw_utf8_nfc_option_is_nfc, \
        p, xctx) == NULL)


/**
 * @brief View an afw_utf8_t as afw_memory_t (same layout).
 * @param string to view.
 * @param p unused (kept so the signature matches other create-style doors).
 * @param xctx of caller.
 * @return memory (cast of string).
 *
 * Cast only. Does not copy. The utf8 bytes are already NFC.
 * The view is pointer + size, not a C string.
 */
#define afw_utf8_as_memory(string, p, xctx) \
    ((const afw_memory_t *)(string))


/**
 * @brief Create utf-8 from afw_memory_t (copy + NFC, or throw).
 * @param memory octets that must be valid utf-8.
 * @param p pool used for result.
 * @param xctx of caller.
 * @return utf8
 *
 * Short name: copy into p and NFC. Invalid utf-8 throws.
 */
#define afw_utf8_from_memory(memory, p, xctx) \
    afw_utf8_nfc((const afw_utf8_octet_t *)(memory)->ptr, (memory)->size, \
        afw_utf8_nfc_option_create_copy, p, xctx)



/**
 * @brief Convert character encoding to a utf-8 in specified pool.
 * @param from_encoding
 * @param from
 * @param from_size
 * @param p pool used for result.
 * @param xctx of caller.
 * @return utf8
 *
 * The raw memory must represent valid utf-8 or an error is thrown.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_utf8_from_encoding(
    const afw_utf8_t * from_encoding,
    const char* * from, afw_size_t * from_size,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Create utf-8 in p (copy bytes, NFC or throw).
 * @param s pointer to octets.
 * @param len number of bytes or AFW_UTF8_Z_LEN.
 * @param p pool for the struct and the copy.
 * @param xctx of caller.
 * @return const afw_utf8_t in p.
 *
 * Short name is the safe door: always copy into p. Invalid utf-8 throws.
 * Valid but not NFC is normalized into p. The copy has no trailing 0;
 * do not use the result `->s` as a C string.
 */
#define afw_utf8_create(s, len, p, xctx) \
    afw_utf8_nfc(s, len, afw_utf8_nfc_option_create_copy, p, xctx)


/**
 * @brief Ingest a 0-terminated C string: new utf8 in p (copy, NFC or throw).
 * @param s_z 0-terminated octets.
 * @param p pool for the struct and the copy.
 * @param xctx of caller.
 *
 * You have a `utf8_z`; you get an `afw_utf8_t`. Inverse of
 * `afw_utf8_to_utf8_z`.
 */
#define afw_utf8_z_to_utf8(s_z, p, xctx) \
    afw_utf8_nfc(s_z, AFW_UTF8_Z_LEN, afw_utf8_nfc_option_create_copy, \
        p, xctx)


/**
 * @brief Create utf-8 in p pointing at s (no copy). NFC or throw.
 * @param s pointer to octets that must already be valid NFC.
 * @param len number of bytes or AFW_UTF8_Z_LEN.
 * @param p pool for the struct only.
 * @param xctx of caller.
 *
 * Extra words take the copy off. p holds the little struct; bytes stay at s
 * and must live as long as the result. Cannot rewrite someone else's memory,
 * so not-NFC throws. `s` is still not a C string. Ingest a `utf8_z` with
 * `afw_utf8_z_as_utf8` / `afw_utf8_z_to_utf8`.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_utf8_create_no_copy(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Ingest a 0-terminated C string: new utf8 pointing at s_z (no copy).
 *
 * `s_z` must already be valid NFC. Inverse of a C string you already own.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_utf8_z_as_utf8(
    const afw_utf8_z_t *s_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Clone a pointer array of utf-8 to specified pool.
 * @param count is number of pointers or -1 if NULL terminated.
 * @param pointers is list of afw_utf8_t pointers.
 * @param NULL_terminate indicates resulting array should be NULL terminated.
 * @param p to use.
 * @param xctx of caller.
 * @return array of afw_utf8_t pointers.
 *
 * The input strings must already be valid utf-8.  An error is thrown if it is
 * not.
 */
AFW_DECLARE(const afw_utf8_t * const *)
afw_utf8_clone_pointer_array(
    afw_size_t count,
    const afw_utf8_t * const * pointers,
    afw_boolean_t NULL_terminate,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Concat array of utf-8 with optional separator to specified pool.
 * @param strings is NULL terminated list of afw_utf8_t pointers.
 * @param separator or NULL for no separator.
 * @param p to use.
 * @param xctx of caller.
 * @return utf8 string.
 *
 * The input strings must already be valid utf-8.  An error is thrown if it is
 * not.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_utf8_array_to_utf8_with_separator(
    const afw_utf8_t * const * strings,
    const afw_utf8_t * separator,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Concat array of utf-8 with optional separator to specified pool.
 * @param strings is NULL terminated list of afw_utf8_t pointers.
 * @param separator or NULL for no separator.
 * @param p to use.
 * @param xctx of caller.
 * @return utf8_z 0 terminated string.
 *
 * The input strings must already be valid utf-8.  An error is thrown if it is
 * not, or if a piece or the separator contains a 0 byte.
 *
 * Use this (or `to_utf8_z` / `z_create`) when the result must be a
 * 0-terminated C string. Do not concatenate and then use a piece `->s`.
 */
AFW_DECLARE(const afw_utf8_z_t *)
afw_utf8_array_to_utf8_z_with_separator(
    const afw_utf8_t * const * strings,
    const afw_utf8_t * separator,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Concat array of utf-8 with optional separator to specified pool.
 * @param strings_z is NULL terminated list of afw_utf8_z_t pointers.
 * @param separator or NULL for no separator.
 * @param p to use.
 * @param xctx of caller.
 * @return utf8_z 0 terminated string.
 *
 * The input strings must already be valid utf-8.  An error is thrown if it is
 * not, or if the separator contains a 0 byte.
 */
AFW_DECLARE(const afw_utf8_z_t *)
afw_utf8_z_array_with_separator(
    const afw_utf8_z_t * const * strings_z,
    const afw_utf8_t * separator,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Clone a utf-8 string into a specific pool.
 * @param string to clone, or NULL.
 * @param p pool used for result.
 * @param xctx of caller.
 * @return utf8 string, or NULL if string is NULL.
 *
 * Clone copies the struct and the bytes (same byte policy as create).
 * The copy has no trailing 0.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_utf8_clone(
    const afw_utf8_t *string, const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Set a preallocated non-const afw_utf8_t from octets (copy + NFC).
 * @param to caller-owned struct to set.
 * @param s octets.
 * @param len number of bytes or AFW_UTF8_Z_LEN.
 * @param p pool for the copied bytes.
 * @param xctx of caller.
 *
 * Writes to->s / to->len. Bytes are copied into p (can NFC). Invalid throws.
 */
AFW_DECLARE(void)
afw_utf8_set(
    afw_utf8_t *to,
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Ingest a 0-terminated C string into a preallocated utf8 (copy + NFC).
 */
AFW_DECLARE(void)
afw_utf8_z_set(
    afw_utf8_t *to,
    const afw_utf8_z_t *s_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Set a preallocated afw_utf8_t pointing at s (no copy). NFC or throw.
 * @param to caller-owned struct to set.
 * @param s octets that must already be valid NFC.
 * @param len number of bytes or AFW_UTF8_Z_LEN.
 * @param xctx of caller.
 *
 * No p: nothing new is allocated. Bytes stay at s.
 */
AFW_DECLARE(void)
afw_utf8_set_no_copy(
    afw_utf8_t *to,
    const afw_utf8_octet_t *s,
    afw_size_t len,
    afw_xctx_t *xctx);

/**
 * @brief Ingest a 0-terminated C string into a preallocated utf8 (point).
 */
AFW_DECLARE(void)
afw_utf8_z_set_no_copy(
    afw_utf8_t *to,
    const afw_utf8_z_t *s_z,
    afw_xctx_t *xctx);

/**
 * @brief Set to from a forced_safe encoding of s (always copy).
 *
 * Result is valid utf-8, not promised NFC, not an Adaptive value.
 */
AFW_DECLARE(void)
afw_utf8_set_forced_safe(
    afw_utf8_t *to,
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Ingest a 0-terminated C string with forced_safe encode (copy).
 */
AFW_DECLARE(void)
afw_utf8_z_set_forced_safe(
    afw_utf8_t *to,
    const afw_utf8_z_t *s_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Create a forced_safe utf-8 in p (encode, always copy, no NFC).
 *
 * Encode: valid text passes through; '^' becomes '^^'; Unicode Cc
 * (U_CONTROL_CHAR) and invalid utf-8 runs become '^' + uppercase hex + '^'.
 * Whitespace/EOL is left as text. Never NFC-throws. Not an Adaptive value.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_utf8_create_forced_safe(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Ingest a 0-terminated C string with forced_safe encode (new utf8).
 */
AFW_DECLARE(const afw_utf8_t *)
afw_utf8_z_create_forced_safe(
    const afw_utf8_z_t *s_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Create an object property name from untrusted external name octets.
 *
 * Same encode as create_forced_safe, then NFC. Result is a property name.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_utf8_create_property_name(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

/**
 * @brief Ingest a 0-terminated C string as a property name (encode, then NFC).
 */
AFW_DECLARE(const afw_utf8_t *)
afw_utf8_z_create_property_name(
    const afw_utf8_z_t *s_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Convert utf-8 sting to lower case in specified pool.
 * @param s is valid NFC normalized UTF-8 string.
 * @param p is pool to use for converted string.
 * @param xctx of caller.
 * @return lower case utf8 string.
 *
 * The input string must already be valid NFC utf-8.
 *
 * This conversion is not locale specific.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_utf8_to_lower(
    const afw_utf8_t *s, const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Create a utf-8 sting with spaces normalized in specified pool.
 * @param s is valid NFC normalized UTF-8 string.
 * @param p is pool to use for space normalized string.
 * @param xctx of caller.
 * @return lower case utf8 string.
 *
 * The input string must already be valid NFC utf-8.
 *
 * If no normalization is required, the input string is returned.
 *
 * The whitespace character are hex 20, 09, 0d, and 0a.  All leading and
 * trailing whitespace characters are removed and internal sequences of
 * whitespace is replaced by a single hex 20.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_utf8_normalize_space(
    const afw_utf8_t *s, const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Concatenate strings with result in specifed pool.
 * @param p pool used for result.
 * @param xctx of caller.
 * @param ... 1 or more valid utf-8 strings followed with NULL.
 * @return utf8 string.
 */
AFW_DECLARE_ELLIPSIS(const afw_utf8_t *)
afw_utf8_concat(
    const afw_pool_t *p, afw_xctx_t *xctx, ...);


/**
 * @brief Concatenate strings with result in specifed pool.
 * @param p pool used for result.
 * @param xctx of caller.
 * @param va_list 1 or more valid utf-8 strings followed with NULL.
 * @return utf8 string.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_utf8_concat_v(const afw_pool_t *p,
    afw_xctx_t *xctx, va_list strings);



/**
 * @brief Determine the line and column of an offset in a string.
 * @param line_number is return pointer for 1 based line number.
 * @param column_number is return pointer for 1 based column number.
 * @param s is string to check.
 * @param offset is offset within string.
 * @param tab_size to use for '\t'.
 * @param xctx of caller.
 * @return false if there were no '\n' found.
 *
 * If there are no '\n', line_number will be set to 1 and column_number will
 * be set to offset adjusted by '\t'.
 *
 * This function does not validity check the string but should produce the
 * correct column number if the string is valid.
 */
AFW_DECLARE(afw_boolean_t)
afw_utf8_line_column_of_offset(
    afw_size_t *line_number,
    afw_size_t *column_number,
    const afw_utf8_t *s,
    afw_size_t offset,
    int tab_size,
    afw_xctx_t *xctx);



/**
 * @brief Determine the line count and maximum column in a string.
 * @param number_of_lines is return pointer for 1 based max line number.
 * @param max_column_number is return pointer for 1 based max column number.
 * @param s is string to check.
 * @param tab_size to use for '\t'.
 * @param xctx of caller.
 *
 * This function does not validity check the string but should produce the
 * correct column number if the string is valid.
 */
AFW_DECLARE(void)
afw_utf8_line_count_and_max_column(
    afw_size_t *number_of_lines,
    afw_size_t *max_column_number,
    const afw_utf8_t *s,
    int tab_size,
    afw_xctx_t *xctx);



/**
 * @brief Create a utf-8 string using a c format string in specified pool.
 * @param p pool used for result.
 * @param xctx of caller.
 * @param format_z string.
 * @param ... arguments for format_z.
 * @return utf8 string.
 *
 * Format output goes through create_forced_safe so a bad %s does not throw.
 */
AFW_DECLARE_ELLIPSIS(const afw_utf8_t *)
afw_utf8_printf(
    const afw_pool_t *p, afw_xctx_t *xctx, const afw_utf8_z_t *format_z,
    ...);


/**
 * @brief Create a utf-8 string using a c format string in specified pool.
 * @param format_z string.
 * @param arg arguments for format_z.
 * @param p pool used for result.
 * @param xctx of caller.
 * @return utf8 string.
 *
 * Format output goes through create_forced_safe so a bad %s does not throw.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_utf8_printf_v(
    const afw_utf8_z_t *format, va_list arg,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Convert utf8 to an external C string in specified pool.
 * @param string to convert.
 * @param p pool used for result.
 * @param xctx of caller.
 * @return utf8_z 0 terminated string.
 *
 * Always use this (or `afw_utf8_z_create`) at the external C-string
 * door. Do not use `string->s` as a C string: it is not 0-terminated
 * and may contain U+0000.
 *
 * The input is assumed to already be valid utf-8. Throws if the
 * length-prefixed bytes contain a 0. A C string cannot represent that
 * value. `forced_safe` / `z_printf` still encode U+0000.
 */
AFW_DECLARE(const afw_utf8_z_t *)
afw_utf8_to_utf8_z(
    const afw_utf8_t *string, const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Check to see if a string starts with another string.
 * @param string to check.
 * @param starts_with string.
 * @param xctx of caller.
 * @return boolean
 *
 * The input strings are assumed to already be valid utf-8.
 */
AFW_DECLARE(afw_boolean_t)
afw_utf8_starts_with(
    const afw_utf8_t *string, const afw_utf8_t *starts_with);


/**
 * @brief Check to see if a string starts with a utf8_z string.
 * @param string to check.
 * @param starts_with_z 0 terminated string.
 * @param xctx of caller.
 * @return boolean
 *
 * The input strings are assumed to already be valid utf-8.
 */
AFW_DECLARE(afw_boolean_t)
afw_utf8_starts_with_utf8_z(
    const afw_utf8_t *string, const afw_utf8_z_t *starts_with_z);


/**
 * @brief Check to see if a string ends with another string.
 * @param string to check.
 * @param ends_with string.
 * @param xctx of caller.
 * @return boolean
 *
 * The input strings are assumed to already be valid utf-8.
 */
AFW_DECLARE(afw_boolean_t)
afw_utf8_ends_with(
    const afw_utf8_t *string, const afw_utf8_t *ends_with);


/**
 * @brief Check to see if a string ends with a utf8_z string.
 * @param string to check.
 * @param ends_with_z 0 terminated string.
 * @param xctx of caller.
 * @return boolean
 *
 * The input strings are assumed to already be valid utf-8.
 */
AFW_DECLARE(afw_boolean_t)
afw_utf8_ends_with_utf8_z(
    const afw_utf8_t *string, const afw_utf8_z_t *ends_with_z);


/**
 * @brief Check to see if a string equals another string.
 * @param s1 string.
 * @param s2 string.
 * @param xctx of caller.
 * @return boolean
 *
 * The input strings are assumed to already be valid utf-8.
 */
AFW_DECLARE(afw_boolean_t)
afw_utf8_equal(
    const afw_utf8_t *s1, const afw_utf8_t *s2);


/**
 * @brief Check to see if a string contains another string.
 * @param s1 string.
 * @param s2 string.
 * @param xctx of caller.
 * @return boolean
 *
 * The input strings are assumed to already be valid utf-8.
 */
AFW_DECLARE(afw_boolean_t)
afw_utf8_contains(
    const afw_utf8_t *s1, const afw_utf8_t *s2);


/**
 * @brief Compare two strings.
 * @param s1 first string.
 * @param s2 second string.
 * @return 0 if equal, <0 if s1 < s2, >0 if s1 > s2.
 *
 * The input strings are assumed to already be valid utf-8.
 */
AFW_DECLARE(int)
afw_utf8_compare(
    const afw_utf8_t *s1, const afw_utf8_t *s2);


/**
 * @brief Compare two strings ignoring case.
 * @param s1 first string.
 * @param s2 second string.
 * @return 0 if equal, <0 if s1 < s2, >0 if s1 > s2.
 *
 * The input strings are assumed to already be valid utf-8.
 */
AFW_DECLARE(int)
afw_utf8_compare_ignore_case(
    const afw_utf8_t *s1, const afw_utf8_t *s2, afw_xctx_t *xctx);


/**
 * @brief Check to see if a string equals a utf8_z string.
 * @param s1 first string.
 * @param s2_z second 0 terminated string.
 * @param xctx of caller.
 * @return boolean
 *
 * The input strings are assumed to already be valid utf-8.
 */
AFW_DECLARE(afw_boolean_t)
afw_utf8_equal_utf8_z(
    const afw_utf8_t *s1, const afw_utf8_z_t *s2_z);


/**
 * @brief Check to see if a string equals a utf8_z string.
 * @param s is string that contains comma separated values.
 * @param p to use for result.
 * @param xctx of caller.
 * @return NULL terminated list of strings.
 *
 * The input strings are assumed to already be valid utf-8.
 */
AFW_DECLARE(const afw_utf8_t * const *)
afw_utf8_parse_csv(
    const afw_utf8_t *s,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Set result to a substring of string using byte indexes.
 * @param result pointer to string to be set.
 * @param string input.
 * @param start Inclusive zero based start index of substring.
 * @param end Exclusive end index of substring.  If end is beyond
 *    end of string, substring will include all bytes up to end
 *    of string.
 *
 * Result points into `string` (no copy). There is no trailing 0 at
 * `result->s[result->len]`. Do not use `result->s` as a C string.
 *
 * If string might contain codepoints > 127, you should use
 * afw_utf8_substring().
 */
AFW_DECLARE(void)
afw_utf8_substring_byte(
    afw_utf8_t *result, const afw_utf8_t *string, afw_size_t start,
    afw_size_t end);


/* -- Zero-terminated utf-8 (`utf8_z` / C string) -- */

/**
 * @brief Create a NFC normalized 0-terminated UTF-8 string in specified
 *    pool.
 * @param s pointer to utf-8 characters.
 * @param len is number of bytes.
 * @param p pool used for result.
 * @param xctx of caller.
 *
 * Always use this (or `afw_utf8_to_utf8_z`) at the external C-string
 * door from length-prefixed octets. Do not use `s` as a C string unless
 * `len` is `AFW_UTF8_Z_LEN` and `s` is already `utf8_z`.
 *
 * A copy is always required to add a 0 byte.
 *
 * Throws if s is not valid utf-8, or if the length-prefixed bytes
 * contain a 0 (a C string cannot represent that value).
 */
AFW_DECLARE(const afw_utf8_z_t *)
afw_utf8_z_create(
    const afw_utf8_octet_t *s, afw_size_t len, const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Returns true if series of bytes for len s1 starts with zero
 *    terminated string s2.
 * @param s pointer to utf-8 characters.
 * @param len1 is number of bytes.
 * @param s2_z 0 terminated string.
 * @return true or false.
 */
AFW_DECLARE(afw_boolean_t)
afw_utf8_len_starts_with_utf8_z(
    const afw_utf8_octet_t *s1, afw_size_t len1, const afw_utf8_z_t *s2_z);


/**
 * @brief Returns true if zero terminated s1 starts with zero terminated
 *    string s2.
 * @param s pointer to utf-8 characters.
 * @param s1_z 0 terminated string.
 * @param s2_z 0 terminated string.
 * @return true or false.
 */
AFW_DECLARE(afw_boolean_t)
afw_utf8_z_starts_with(
    const afw_utf8_z_t *s1_z, const afw_utf8_z_t *s2_z);


/**
 * @brief Compare two zero terminated utf-8 strings ignoring case.
 */
AFW_DECLARE(int)
afw_utf8_z_compare_ignore_case(
    const afw_utf8_z_t *s1, const afw_utf8_z_t *s2, afw_xctx_t *xctx);


/** @fixme Need to fix comments below and polish comments above. */

/**
 * Compare two zero terminated UTF8 strings to be equal.
 * Strings should already be normalized (NFC, etc.)
 */
AFW_DECLARE(afw_boolean_t)
afw_utf8_z_equal(
    const afw_utf8_z_t *s1, const afw_utf8_z_t *s2);


/**
 * Compare two zero terminated UTF8 strings to be equal.
 * Strings should already be normalized (NFC, etc.)
 *
 * @fixme FIXME This needs to change to be like
 * afw_utf8_z_compare_ignore_case()
 * when XACML is chanced not to use this.  New version has xctx so
 * error can be thrown if string is not UTF8 or too large.
 */
AFW_DECLARE(afw_boolean_t)
afw_utf8_z_equal_ignore_case(
    const afw_utf8_z_t *s1, const afw_utf8_z_t *s2);



/**
 * Concatenate zero terminated UTF8 strings in specifed pool.
 */
AFW_DECLARE_ELLIPSIS(const afw_utf8_z_t *)
afw_utf8_z_concat(
    const afw_pool_t *p, afw_xctx_t *xctx, ...);


/**
 * Query string to object.
 */
AFW_DECLARE(const afw_object_t *)
afw_utf8_z_query_string_to_object(
    const afw_utf8_z_t *s, afw_xctx_t *xctx);


/**
 * Create a utf8_z string using a c format string and va_list in specified pool.
 */
AFW_DECLARE(const afw_utf8_z_t *)
afw_utf8_z_printf_v(
    const afw_utf8_z_t *format_z, va_list ap,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * Create a utf8_z string using a c format string in specified pool.
 */
AFW_DECLARE_ELLIPSIS(const afw_utf8_z_t *)
afw_utf8_z_printf(
    const afw_pool_t *p, afw_xctx_t *xctx, const afw_utf8_z_t *format_z, ...);



/**
 * Returns pointer in path_z past last / or \.
 */
AFW_DECLARE(const afw_utf8_z_t *)
afw_utf8_z_file_name_from_path(
    const afw_utf8_z_t *path_z);


/**
 * @brief Returns value of source_z after last '/ 'or '\'.
 * @param source_z
 * @return pointer past last '/ 'or '\'.
 *
 * This function is intended to return the pointer into a passed __FILE__
 * that is past the last '/' or '\' for compilers that provide the full path.
 */
AFW_DECLARE(const afw_utf8_z_t *)
afw_utf8_z_source_file(const afw_utf8_z_t *source_z);



/**
 * @brief Create UTF-8 stream
 * @param streamId
 * @param p to use.
 * @param xctx of caller.
 * @return stream
 */
AFW_DECLARE(const afw_stream_t *)
afw_utf8_stream_create(
    const afw_utf8_t *streamId,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Create UTF-8 writer
 * @param tab string to use or NULL if no whitespace is to be added.
 * @param p to use.
 * @param xctx of caller.
 * @return writer
 */
AFW_DECLARE(const afw_writer_t *)
afw_utf8_writer_create(
    const afw_utf8_t *tab,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



/**
 * @brief Get the current string in a UTF-8 writer.
 * @param writer
 * @param current is address of a afw_utf8_t struct that will be updated.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_utf8_stream_get_current_cached_string(
    const afw_stream_t *stream,
    afw_utf8_t *current_cached_string,
    afw_xctx_t *xctx);



/**
 * @brief Get the current string in a UTF-8 writer.
 * @param writer
 * @param current is address of a afw_utf8_t struct that will be updated.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_utf8_writer_current_string(
    const afw_writer_t *writer,
    afw_utf8_t *current_string,
    afw_xctx_t *xctx);



AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_UTF8_H__ */

