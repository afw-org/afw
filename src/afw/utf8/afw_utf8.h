// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW - String Functions
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_UTF8_H__
#define __AFW_UTF8_H__


/** @addtogroup afw_c_api
 * @{
 */

/** @addtogroup afw_utf8 UTF-8 NFC string
 *
 * All utf8 strings must be valid utf-8.
 *
 * @{
 */

/**
 * @file afw_utf8.h
 * @brief AFW String functions.
 *
 * This is the header file for AFW String functions.
 */

#include "afw_minimal.h"

AFW_BEGIN_DECLARES

/**
 * @brief Callback function for creating a string from an adaptive value.
 * @param value
 * @param Indicate to only include own properties of any objects.
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
 * @param utf-8 string to convert.
 * @param path Object path to use if string is an object or NULL.
 * @param object_id Optional id of object if value is object or NULL.
 * @param xctx of caller.
 * @return value
 *
 * The path parameter is only used if the string being converted is an object
 * entity.
 *
 * The path must be of the form /<adaptor id>/<object type>/<id>.  This path
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
 * @brief Convert utf-8 string to raw in specified pool.
 * @param string to convert.
 * @param p pool used for result.
 * @param xctx of caller.
 * @return raw
 */
AFW_DEFINE_STATIC_INLINE(const afw_memory_t *)
afw_utf8_as_raw(
    const afw_utf8_t *string, const afw_pool_t *p, afw_xctx_t *xctx)
{
    return (const afw_memory_t *)string;
}


/**
 * @brief Convert raw to a utf-8 NFC normalizing if necessary in specified
 *    pool.
 * @param raw value to convert.
 * @param p pool used for result.
 * @param xctx of caller.
 * @return utf8
 *
 * The raw memory must represent valid utf-8 or an error is thrown.
 */
AFW_DEFINE_STATIC_INLINE(const afw_utf8_t *)
afw_utf8_from_raw(
    const afw_memory_t *raw, const afw_pool_t *p, afw_xctx_t *xctx)
{
    return afw_utf8_nfc((const afw_utf8_octet_t *)raw->ptr, raw->size,
        afw_utf8_nfc_option_create, p, xctx);
}



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
 * @brief Create utf-8 string without copy unless necessary in pool specified.
 * @param s pointer to utf-8 characters.
 * @param len is number of bytes.
 * @param p pool used for result.
 * @param xctx of caller.
 * @return utf8 string.
 *
 * This only creates a new afw_string_t struct and sets its values.
 *
 * The input string will be NFC normalized if it is not already.
 */
#define afw_utf8_create(s,len, p,  xctx) \
    afw_utf8_nfc(s, len, afw_utf8_nfc_option_create, p, xctx)



/**
 * @brief Make utf-8 string without copy in specified pool.
 * @param s_z pointer to zero terminated utf-8 characters.
 * @param len is number of bytes.
 * @param p pool used for result.
 * @param xctx of caller.
 * @return utf8 string.
 *
 * The input string will be NFC normalized if it is not already.
 */
#define afw_utf8_from_utf8_z(s_z, p, xctx) \
    afw_utf8_nfc(s_z, AFW_UTF8_Z_LEN, afw_utf8_nfc_option_create, \
        p, xctx)


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
 * not.
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
 * not.
 */
AFW_DECLARE(const afw_utf8_z_t *)
afw_utf8_z_array_to_utf8_z_with_separator(
    const afw_utf8_z_t * const * strings_z,
    const afw_utf8_t * separator,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Clone a utf-8 string into a specific pool.
 * @param string.
 * @param len is number of bytes.
 * @param p pool used for result.
 * @param xctx of caller.
 * @return utf8 string.
 *
 * The input string is assumed to already be valid utf-8.
 */
AFW_DEFINE_STATIC_INLINE(const afw_utf8_t *)
afw_utf8_clone(
    const afw_utf8_t *string, const afw_pool_t *p, afw_xctx_t *xctx)
{
    return (string)
        ? afw_utf8_nfc(string->s, string->len,
            afw_utf8_nfc_option_create_copy, p, xctx)
        : NULL;
}



/**
 * @brief Make a utf-8 sting from chars in pool specified.
 * @param s pointer to chars.
 * @param len of chars in bytes or AFW_UTF8_Z_LEN.
 * @param p pool used for result.
 * @param xctx of caller.
 * @return utf8 string.
 *
 * The input string must already be valid utf-8.  An error is thrown if it is
 * not.
 */
#define afw_utf8_create_copy(s, len, p, xctx) \
    afw_utf8_nfc(s, len, afw_utf8_nfc_option_create_copy, p, xctx)



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
 * Care must be used using this function.  No check is made to determine
 * if strings are valid utf-8.
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
 * Care must be used using this function.  No check is made to determine
 * if strings are valid utf-8.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_utf8_printf_v(
    const afw_utf8_z_t *format, va_list arg,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Convert utf8 to utf8_z in specified pool.
 * @param string to convert.
 * @param p pool used for result.
 * @param xctx of caller.
 * @return utf8_z 0 terminated string.
 *
 * The input strings are assumed to already be valid utf-8.
 */
AFW_DEFINE_STATIC_INLINE(const afw_utf8_z_t *)
afw_utf8_to_utf8_z(
    const afw_utf8_t *string, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_z_t * result;

    result = afw_pool_malloc(p, string->len + 1, xctx);
    memcpy(result, string->s, string->len);
    result[string->len] = 0;
    return result;
}


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
afw_utf8_starts_with_z(
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
afw_utf8_ends_with_z(
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
 * If string might contain codepoints > 127, you should use
 * afw_utf8_substring().
 */
AFW_DEFINE_STATIC_INLINE(void)
afw_utf8_substring_byte(
    afw_utf8_t *result, const afw_utf8_t *string, afw_size_t start,
    afw_size_t end)
{
    if (end > string->len) end = string->len;
    result->len = (end > start) ? end - start : 0;
    result->s = (result->len > 0) ? string->s + start : NULL;
}


/* -- The following are primarily zero terminate utf-8 string functions. -- */

/**
 * @brief Create a NFC Normalized zero terminated UTF-8 string in specified
 *    pool.
 * @param s pointer to utf-8 characters.
 * @param len is number of bytes.
 * @param p pool used for result.
 * @param xctx of caller.
 *
 * A copy is always required to add a 0 byte.
 *
 * An error is thrown if s is not valid utf-8.
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
AFW_DEFINE_STATIC_INLINE(afw_boolean_t)
afw_utf8_len_starts_with_z(
    const afw_utf8_octet_t *s1, afw_size_t len1, const afw_utf8_z_t *s2_z)
{
    while (*s2_z) {
        if (len1-- <= 0 || *s1 != *s2_z) {
            return false;
        }
        s1++;
        s2_z++;
    }

    return true;
}


/**
 * @brief Returns true if zero terminated s1 starts with zero terminated
 *    string s2.
 * @param s pointer to utf-8 characters.
 * @param s1_z 0 terminated string.
 * @param s2_z 0 terminated string.
 * @return true or false.
 */
AFW_DEFINE_STATIC_INLINE(afw_boolean_t)
afw_utf8_z_starts_with_z(
    const afw_utf8_z_t *s1_z, const afw_utf8_z_t *s2_z)
{
    while (*s2_z) {
        if (!*s1_z || *s1_z != *s2_z) {
            return false;
        }
        s1_z++;
        s2_z++;
    }

    return true;
}


/**
 * @brief Compare two zero terminated utf-8 strings ignoring case.
 *
 * 
 */
AFW_DEFINE_STATIC_INLINE(int)
afw_utf8_z_compare_ignore_case(
    const afw_utf8_z_t *s1, const afw_utf8_z_t *s2, afw_xctx_t *xctx)
{
    afw_utf8_t a1, a2;

    a1.s = s1;
    a1.len = s1 ? strlen(s1): 0;

    a2.s = s2;
    a2.len = s2 ? strlen(s2): 0;

    return afw_utf8_compare_ignore_case(&a1, &a2, xctx);
}


/** @fixme Need to fix comments below and polish comments above. */

/**
 * Compare two zero terminated UTF8 strings to be equal.
 * Strings should already be normalized (NFC, etc.)
 */
AFW_DEFINE_STATIC_INLINE(afw_boolean_t)
afw_utf8_z_equal(
    const afw_utf8_z_t *s1, const afw_utf8_z_t *s2)
{
    return (s1 && s2 && (strcmp((const char *)s1, (const char *)s2) == 0
        || s1 == s2)) ? TRUE : FALSE;
}


/**
 * Compare two zero terminated UTF8 strings to be equal.
 * Strings should already be normalized (NFC, etc.)
 *
 * @fixme FIXME This needs to change to be like
 * afw_utf8_z_compare_ignore_case()
 * when XACML is chanced not to use this.  New version has xctx so
 * error can be thrown if string is not UTF8 or too large.
 */
AFW_DEFINE_STATIC_INLINE(afw_boolean_t)
afw_utf8_z_equal_ignore_case(
    const afw_utf8_z_t *s1, const afw_utf8_z_t *s2)
{
    return (strcasecmp((const char *)s1, (const char *)s2) == 0) ? true : false;
}



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
AFW_DEFINE_STATIC_INLINE(const afw_utf8_z_t *)
afw_utf8_z_printf(
    const afw_pool_t *p, afw_xctx_t *xctx, const afw_utf8_z_t *format_z, ...)
{
    va_list ap;
    const afw_utf8_z_t *result;

    va_start(ap, format_z);
    result = afw_utf8_z_printf_v(format_z, ap, p, xctx);
    va_end(ap);

    return result;
};



/**
 * Returns pointer in path_z past last / or \.
 */
AFW_DEFINE_STATIC_INLINE(const afw_utf8_z_t *)
afw_utf8_z_file_name_from_path(
    const afw_utf8_z_t *path_z)
{
    const afw_utf8_z_t *file_name;
    const afw_utf8_z_t *c;

    if (!path_z) return "";

    for (c = file_name = path_z; *c; c++) {
        if ((*c == '/') || (*c == '\\')) {
            file_name = c + 1;
        }
    }

    return file_name;
}


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

/** @} */
/** @} */

#endif /* __AFW_UTF8_H__ */

