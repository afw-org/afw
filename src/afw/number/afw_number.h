// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Number Header
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

#ifndef __AFW_NUMBER_H__
#define __AFW_NUMBER_H__

#include "afw_common.h"
#include <float.h>

/**
 * @defgroup afw_number Number
 * @ingroup afw_c_api_public
 *
 * Adaptive Framework number support.
 *
 * @{
 */

/**
 * @file afw_number.h
 * @brief Adaptive Framework Number Header.
 */

AFW_BEGIN_DECLARES

#define AFW_NUMBER_Q_INF                "INF"
#define AFW_NUMBER_Q_INFINITY           "Infinity"
#define AFW_NUMBER_Q_NEGATIVE_INF       "-INF"
#define AFW_NUMBER_Q_NEGATIVE_INFINITY  "-Infinity"
#define AFW_NUMBER_Q_NAN                "NaN"
#define AFW_NUMBER_Q_NEGATIVE_NAN       "-NaN"
#define AFW_NUMBER_Q_EXPONENT_ZERO      "E0"

/**
 * @brief Determine if double is finite.
 * @param d is double to check.
 * @return true if number is finite.
 */
AFW_DEFINE_STATIC_INLINE(afw_boolean_t)
afw_number_is_finite(double d)
{
    return (d <= DBL_MAX && d >= -DBL_MAX);
}


/**
 * @brief Determine if double is positive infinity.
 * @param d is double to check.
 * @return true if number is positive infinity.
 */
AFW_DEFINE_STATIC_INLINE(afw_boolean_t)
afw_number_is_positive_infinity(double d)
{
    return (d > DBL_MAX);
}


/**
 * @brief Determine if double is negative infinity.
 * @param d is double to check.
 * @return true if number is negative infinity.
 */
AFW_DEFINE_STATIC_INLINE(afw_boolean_t)
afw_number_is_negative_infinity(double d)
{
    return (d < -DBL_MAX);
}


/**
 * @brief Determine if double is not a number.
 * @param d is double to check.
 * @return true if number is not a number (NaN).
 *
 * If a number is not a number (NaN) the number compared to itself will not be
 * true.
 */
AFW_DEFINE_STATIC_INLINE(afw_boolean_t)
afw_number_is_NaN(double d)
{
    return !(d == d);
}


/**
 * @brief Parse an integer or double using specified pool for work area.
 * @param cursor is pointer to char where number starts.
 * @param len of buffer remaining starting at cursor.
 * @param i is NULL or pointer to place to return integer.
 * @param d is NULL or pointer to place to return double.
 * @param is_double is NULL or pointer to place to set to true if number is a
 *    double.
 * @param p to use for wa.
 * @param xctx of caller.
 * @return len of number in buffer starting at buffer or or -1 if error.
 */
AFW_DECLARE(afw_size_t)
afw_number_parse(
    const afw_utf8_octet_t *cursor, afw_size_t len,
    afw_integer_t *i, afw_double_t *d, afw_boolean_t *is_double, 
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Parse an integer using specified pool for work area.
 * @param cursor is pointer to char where number starts.
 * @param len of buffer remaining starting at cursor.
 * @param i is NULL or pointer to place to return integer.
 * @param p to use for wa.
 * @param xctx of caller.
 * @return len of number in buffer starting at buffer or or -1 if error.
 */
#define afw_number_parse_integer(cursor, len, i, p, xctx) \
    afw_number_parse(cursor, len, i, NULL, NULL, p, xctx)


/**
 * @brief Parse a double using specified pool for work area.
 * @param cursor is pointer to char where number starts.
 * @param len of buffer remaining starting at cursor.
 * @param d is NULL or pointer to place to return double.
 * @param p to use for wa.
 * @param xctx of caller.
 * @return len of number in buffer starting at buffer or or -1 if error.
 */
#define afw_number_parse_double(cursor, len, d, p, xctx) \
    afw_number_parse(cursor, len, NULL, d, NULL, p, xctx)


/**
 * @brief Convert a double to utf8 in specified pool.
 * @param d is double to convert.
 * @param p to use for result.
 * @param xctx of caller.
 * @return utf8 representation of double.
 *
 * The canonical representation for float is defined by prohibiting
 * certain options from the Lexical representation (�3.2.4.1).
 * Specifically, the exponent must be indicated by "E". Leading zeroes
 * and the preceding optional "+" sign are prohibited in the exponent.
 * If the exponent is zero, it must be indicated by "E0". For the
 * mantissa, the preceding optional "+" sign is prohibited and the
 * decimal point is required. Leading and trailing zeroes are prohibited
 * subject to the following: number representations must be normalized
 * such that there is a single digit which is non-zero to the left of
 * the decimal point and at least a single digit to the right of the
 * decimal point unless the value being represented is zero.
 * The canonical representation for zero is 0.0E0.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_number_double_to_utf8(
    afw_double_t d,
    const afw_pool_t *p, afw_xctx_t *xctx);


/**
 * @brief Convert a utf8 string to double in specified pool.
 * @param s is utf8 sting to convert.
 * @param p to use for result.
 * @param xctx of caller.
 * @return double in internal representation.
 */
AFW_DEFINE_STATIC_INLINE(afw_double_t)
afw_number_utf8_to_double(
    const afw_utf8_t *s, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_double_t d;
    afw_boolean_t is_double;
    afw_size_t len;

    len = afw_number_parse(s->s, s->len, NULL, &d, &is_double, p, xctx);
    if (!is_double || len != s->len) {
        AFW_THROW_ERROR_Z(general, "Invalid double", xctx);
    }
    return d;
}



/**
 * @brief Convert an integer to utf8 in specified pool.
 * @param i is integer to convert.
 * @param p to use for result.
 * @param xctx of caller.
 * @return utf8 representation of integer.
 */
AFW_DECLARE(const afw_utf8_t *)
afw_number_integer_to_utf8(
    afw_integer_t i,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Convert an integer to u8 in supplied buffer.
 * @param i is integer to convert.
 * @param s is address of place to put result.
 * @param len of place to put result.
 * @param p to use for result.
 * @param xctx of caller.
 * @return len used.
 */
AFW_DECLARE(afw_size_t)
afw_number_integer_set_u8(
    afw_integer_t i, afw_utf8_octet_t *s, afw_size_t len, afw_xctx_t *xctx);
   

/**
 * @brief Convert a utf8 string to integer in specified pool.
 * @param s is utf8 sting to convert.
 * @param p to use for result.
 * @param xctx of caller.
 * @return integer in internal representation.
 */
AFW_DEFINE_STATIC_INLINE(afw_integer_t)
afw_number_utf8_to_integer(
    const afw_utf8_t *s, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_integer_t i;
    afw_boolean_t is_double;
    afw_size_t len;

    len = afw_number_parse(s->s, s->len, &i, NULL, &is_double, p, xctx);
    if (is_double || len != s->len) {
        AFW_THROW_ERROR_Z(general, "Invalid integer", xctx);
    }
    return i;
}



/**
 * @brief Determine bytes needed to hold printable size_t.
 * @param i to check.
 * @return number of bytes needed.
 */
AFW_DECLARE(int)
afw_number_bytes_needed_size_t(afw_size_t i);



/**
 * @brief Determine bytes needed to hold printable afw_integer_t.
 * @param i to check.
 * @return number of bytes needed.
 */
AFW_DEFINE(int)
afw_number_bytes_needed_integer(afw_integer_t i);


AFW_END_DECLARES

/** @} */

#endif /* __AFW_NUMBER_H__ */
