// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Number
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_number.c
 * @brief Adaptive Framework Number.
 */

#include "afw_internal.h"
#include <float.h>

/* Convert a double to utf8 in specified pool. */
AFW_DEFINE(const afw_utf8_t *)
afw_number_double_to_utf8(
    afw_double_t d,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    /* IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT
     *
     * Make sure s is one longer than precision plus 1 for \0 plus a
     * little padding because of implementation differences that may
     * not count leading sign in precision, etc.  For %.16g, 32 is used
     * just to be safe.
     */
    char s[32];

    /*
     * %.16E will be able to print minus, plus 16 digits, plus period,
     * plus 'e', plus sign, plus 3 digits.
     *
     * JSON only supports finite numbers.  If not finite, return a string
     * with "NaN", "-INF", or "INF".
     *
     * For the E conversion style [-]d.dddE±dd is used.
     */
    /*
     * Note:
     *
     * The exponent contains at least two digits, more digits are used only
     * if necessary. If the value is ​0​, the exponent is also ​0​.
     *
     * Considered using %.15G since last digit may be wrong with %.16G, but
     * that would be a problem as well because part of the time %.16G is
     * correct.
     *
     * 9,007,199,254,740,992 (the largest integer 2^53) is correct for %.16G,
     * 9,007,199,254,740,993 is 9,007,199,254,740,992 for %.16G.
     *
     * Note:
     * 
     * # says decimal point character is written even if no digits follow it.
     *
     */
    afw_size_t i;
    char dst[32];
    char *pdst = &dst[0];
    char *epos;

    if (afw_number_is_finite(d)) {
        sprintf(s, "%#.16E", d);

        /* remove '+' and leading zero if any after 'E' inserted by sprintf */
        strcpy(pdst, s);
        epos = strchr(pdst, 'E');

        for (i = epos - dst; dst[i] != '\0'; i++) {
            if (dst[i] == '0' || dst[i] == '+')  continue;
            else  pdst[0] = dst[i];
            pdst++;
        }

        /* make sure it is null terminated */
        pdst[0] = '\0';

        if (strlen(dst)==1) 
            strcpy(dst, AFW_NUMBER_Q_EXPONENT_ZERO);

        /* Find most significant digits without trailing zeroes before 'E' */
        epos = strchr(s, 'E');

        for (i = epos - s - 1; i > 0; i--) {
            if (s[i] == '0') continue;

            /* there must be at least one digit after the '.' */
            else if (s[i] == '.') {
                s[i + 1] = '0';
                s[i + 2] = '\0';
                break;
            }
            else { 
                s[i + 1] = '\0'; 
                break;
            }
        }

        /* put the 2 pieces back together minus unnecessary zeroes and + */
        strcat(s, dst);

    } else if (afw_number_is_NaN(d)) {
        strcpy(s, AFW_NUMBER_Q_NAN);
    } else if (d <= DBL_MAX) {
        strcpy(s, AFW_NUMBER_Q_NEGATIVE_INFINITY);
    } else {
        strcpy(s, AFW_NUMBER_Q_INFINITY);
    }

    return afw_utf8_create_copy(s, AFW_UTF8_Z_LEN, p, xctx);
}


/* Calculate buffer needed for integer as utf8. */
AFW_DEFINE_STATIC_INLINE(afw_size_t)
impl_integer_len_as_u8(
    afw_integer_t i)
{
    afw_size_t len;

    if (i == AFW_INTEGER_MIN) {
        return strlen(AFW_INTEGER_Q_MIN);
    }

    len = 0;
    if (i < 0) {
        len++;
        i = -i;
    }

    do {
        len++;
        i = i / 10;
    } while (i);

    return len;
}


/* Convert an integer to u8 in supplied buffer that is correct size. */
AFW_DEFINE_STATIC_INLINE(void)
impl_number_integer_set_u8(
    afw_integer_t i, afw_utf8_octet_t *s, afw_size_t needed, afw_xctx_t *xctx)
{
    if (i == AFW_INTEGER_MIN) {
        memcpy(s, AFW_INTEGER_Q_MIN, needed);
    }

    else {
        if (i < 0) {
            *s = '-';
            i = -i;
        }
        s += needed;
        do {
            *--s = '0' + (i % 10);
            i /= 10;
        } while (i);
    }
}



/* Convert an integer to utf8 in specified pool. */
AFW_DEFINE(const afw_utf8_t *)
afw_number_integer_to_utf8(
    afw_integer_t i,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_t *result;
    afw_utf8_octet_t *s;

    result = afw_pool_calloc_type(p, afw_utf8_t, xctx);
    result->len = impl_integer_len_as_u8(i);
    s = afw_pool_calloc(p, result->len, xctx);
    impl_number_integer_set_u8(i, s, result->len, xctx);
    result->s = s;

    return result;
}


/* Convert an integer to utf8 in supplied buffer. */
AFW_DEFINE(afw_size_t)
afw_number_integer_set_u8(
    afw_integer_t i, afw_utf8_octet_t *s, afw_size_t len,
    afw_xctx_t *xctx)
{
    afw_size_t needed;

    needed = impl_integer_len_as_u8(i);
    impl_number_integer_set_u8(i, s, needed, xctx);

    return needed;
}



/* Parse an integer or double. */
AFW_DEFINE(afw_size_t)
afw_number_parse(
    const afw_utf8_octet_t *cursor, afw_size_t len,
    afw_integer_t *i, afw_double_t *d, afw_boolean_t *is_double, 
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_octet_t *c;
    const afw_utf8_octet_t *s;
    afw_boolean_t zero;
    afw_boolean_t is_integer;
    afw_boolean_t is_negative;
    afw_integer_t negative;
    double number;
    double d0;

    c = cursor;
    if (is_double) *is_double = false;

    /* If parse double, allow Infinity, -Infinity, and NaN. */
    if (d) {
        d0 = 0;
        if (afw_utf8_len_starts_with_z(c, len, AFW_NUMBER_Q_INF)) {
            *d = 1 / d0;
            if (is_double) *is_double = true;
            return strlen(AFW_NUMBER_Q_INF);
        }
        
        else if (afw_utf8_len_starts_with_z(c, len, AFW_NUMBER_Q_INFINITY)) {
            *d = 1 / d0;
            if (is_double) *is_double = true;
            return strlen(AFW_NUMBER_Q_INFINITY);
        }
        
        else if (afw_utf8_len_starts_with_z(c, len, AFW_NUMBER_Q_NEGATIVE_INF)) {
            *d = -1 / d0;
            if (is_double) *is_double = true;
            return strlen(AFW_NUMBER_Q_NEGATIVE_INF);
        }
        
        else if (afw_utf8_len_starts_with_z(c, len,
            AFW_NUMBER_Q_NEGATIVE_INFINITY))
        {
            *d = -1 / d0;
            if (is_double) *is_double = true;
            return strlen(AFW_NUMBER_Q_NEGATIVE_INFINITY);
        }

        else if (afw_utf8_len_starts_with_z(c, len, AFW_NUMBER_Q_NAN)) {
            *d = sqrt(-1);
            if (is_double) *is_double = true;
            return strlen(AFW_NUMBER_Q_NAN);
        }
        
        else if (afw_utf8_len_starts_with_z(c, len, AFW_NUMBER_Q_NEGATIVE_NAN)) {
            *d = sqrt(-1);
            if (is_double) *is_double = true;
            return strlen(AFW_NUMBER_Q_NEGATIVE_NAN);
        }
    }


    /* Parse integer part and prescan floating point part. */
    zero = 1;
    is_integer = true;
    is_negative = false;
    do {

        /* Number can't end here. */
        if (len <= 0) {
            return -1;
        }

        /* Optional leading '-' */
        if (*c == '-') {
            c++;
            (len)--;
            is_negative = true;
        }

        /*
         * Ignore '+'. If caller doesn't allow, it can check first char for
         * '+' before calling.
         */
        if (*c == '+') {
            c++;
            (len)--;
        }

        /* Number can't end here. */
        if (len <= 0) {
            return -1;
        }

        /* Can be '0' or digits'1'-'9' followed by additional digits '0'-'9'. */
        if (*c >= '1' && *c <= '9') {
            zero = 0;
            /* Sum digits as a negative number.  Negative is one larger. */
            negative = -((*c) - '0');
            c++;
            (len)--;
            while (len > 0 && *c >= '0' && *c <= '9') {
                negative = (negative * 10) - ((*c) - '0');
                if (negative > 0) {
                    is_integer = false;
                }
                c++;
                (len)--;
            }
        }
        else if (*c == '0') {
            negative = 0;
            c++;
            (len)--;
        }
        else {
            return -1;
        }

        /* Number is an integer if at end of buffer. */
        if (len <= 0) {
            break;
        }

        /* If not checking for double, don't check further. */
        if (!d) break;

        /* Next can be an optional period followed by digits '0'-'9'. */
        if (*c == '.') {
            c++;
            (len)--;
            is_integer = false;
            /* Number can't end here. */
            if (len <= 0 || *c < '0' || *c > '9') {
                return -1;
            }
            while (len > 0 && *c >= '0' && *c <= '9') {
                if (*c != '0') {
                    zero = 0;
                }
                c++;
                (len)--;
            }
        }

        /* Number can end here. */
        if (len <= 0) {
            break;
        }


        /*
         * Next can be an 'e' or 'E' followed by optional '-' or '+' followed
         * by digits '0' - '9'.
         */
        if (*c == 'e' || *c == 'E') {
            is_integer = false;
            /* Skip 'e' or 'E'. */
            c++;
            (len)--;
            /* Number can't end here. */
            if (len <= 0) {
                return -1;
            }
            /* Skip '+' or '-'. */
            if (*c == '+' || *c == '-') {
                c++;
                (len)--;
            }
            /* Number can't end here. */
            if (len <= 0 || *c < '0' || *c > '9') {
                return -1;
            }
            /* Skip digits. */
            while (len > 0 && *c >= '0' && *c <= '9') {
                c++;
                (len)--;
            }
        }
    } while (0);

    /*
     * If is_integer (no '.', 'e', or 'E'), return integer.
     *
     * Note: javascript treats all numbers as double, but adaptive
     *       frameworks treats integers as afw_integer_t.
     */
    if (is_integer) {
        if (is_double) *is_double = false;
        if (!i) return -1;
        if (is_negative) {
            *i = negative;
        }
        else if (negative != AFW_INTEGER_MIN) {
            *i = -negative;
        }
        else {
            return -1;
        }
        return (afw_size_t)(c - cursor);
    }


    /*
     * Not integer, create a double value. Use atof to convert number if it
     * is not zero.
     */
    if (is_double) *is_double = true;
    if (!d) return -1;
    number = 0;
    if (!zero) {
        s = apr_pstrndup(afw_pool_get_apr_pool(p), cursor, c - cursor);
        errno = 0;
        number = strtod(s, NULL);
        if (errno != 0) {
            return -1;
        }
    }

    *d = number;
    return (afw_size_t)(c - cursor);
}

AFW_DEFINE(int)
afw_number_bytes_needed_size_t(afw_size_t i)
{
    int result;

    for (result = 1; i > 9; i /= 10) {
        result++;
    }

    return result;
}

AFW_DEFINE(int)
afw_number_bytes_needed_integer(afw_integer_t i)
{
    int result;

    for (result = 1; i > 9; i /= 10) {
        result++;
    }

    return result;
}
