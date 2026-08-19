// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW - String Functions
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_utf8.c
 * @brief UTF-8 string create, compare, and conversion functions.
 */

#include "afw_internal.h"
#include <unicode/utf8.h>
#include <unicode/utf.h>
#include <unicode/uchar.h>
#include <unicode/unorm2.h>
#include <unicode/ustring.h>
#include <unicode/utypes.h>
#include <string.h>
#include <stddef.h>

static const afw_utf8_z_t * impl_z_empty = "";

/* afw_utf8_t with 0 len and null s. */
static const afw_utf8_t impl_utf8_null = { NULL, 0 };

#define IMPL_WHITESPACE(c) \
((c) == 0x20 || (c) == 0x09 || (c) == 0x0d || (c) == 0x0a)

/* Get next codepoint. */
AFW_DEFINE(afw_code_point_t)
afw_utf8_next_code_point(const afw_utf8_octet_t *s, afw_size_t *offset,
    afw_size_t len, afw_xctx_t *xctx)
{
    UChar32 cp;
    int32_t slen;
    int32_t soffset;

    /* If len is AFW_UTF8_Z_LEN, set it to strlen(s). */
    if (len == AFW_UTF8_Z_LEN) {
        len = strlen(s);
    }

    /* ICU only supports int32 length. */
    soffset = afw_safe_cast_size_to_int32(*offset, xctx);
    slen = afw_safe_cast_size_to_int32(len, xctx);
    if (*offset != soffset && len != slen) {
        AFW_THROW_ERROR_Z(general, "offset or len exceeds icu max", xctx);
    }

    if (*offset >= len) {
        cp = -1;
    }
    else {
        U8_NEXT((const uint8_t *)s, soffset, slen, cp);
        *offset = soffset;
    }

    return cp;
}


/* Convert a code point to utf8. */
AFW_DEFINE(afw_boolean_t)
afw_utf8_from_code_point(afw_utf8_octet_t utf8_z[5], afw_code_point_t cp,
    afw_xctx_t *xctx)
{
    UBool isError;
    int32_t i = 0;

    isError = FALSE;
    U8_APPEND((afw_octet_t *)utf8_z, i, 4, cp, isError);
    utf8_z[i] = 0;
    return (isError == FALSE);
}


/* ICU UErrorCode name as a static ASCII C string. Never throws. */
AFW_DEFINE(const afw_utf8_z_t *)
afw_utf8_icu_error_name_z(int rv)
{
    const char *s;

    s = u_errorName((UErrorCode)rv);
    return s ? (const afw_utf8_z_t *)s : impl_z_empty;
}


/*
 * UTF-8 NFC support function.
 *
 * IMPORTANT: If option is afw_utf8_nfc_option_is_valid, p must not be used.  Macro
 *           afw_utf8_is_valid() calls this using xctx's pool, which can be a
 *           problem.
 *
 */
AFW_DEFINE(const afw_utf8_t *)
afw_utf8_nfc(
    const afw_utf8_octet_t *s, afw_size_t len,
    afw_utf8_nfc_option_t option,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    UChar32 c;
    int32_t i, length;
    const afw_utf8_octet_t *string;
    UNormalizationCheckResult is_nfc;
    const afw_utf8_t *result;
    afw_utf8_t *new_result;
    afw_utf8_octet_t *result_s;
    UErrorCode errorCode;
    int32_t input_utf16_length;
    UChar *input_utf16;
    int32_t output_utf16_length;
    UChar *output_utf16;
    const UNormalizer2* nfc;

    /* If s is NULL or length is 0, return NULL or empty string. */
    if (!s || len == 0) {
        if (option == afw_utf8_nfc_option_is_valid ||
            option == afw_utf8_nfc_option_is_nfc)
        {
            return NULL;
        }
        return afw_s_a_empty_string;
    }

    /* If len is AFW_UTF8_Z_LEN, set it to strlen(s). */
    if (len == AFW_UTF8_Z_LEN) {
        len = strlen(s);
    }

    /* If s starts with BOM, skip it. */
    if (len >= 3 &&
        (afw_octet_t)s[0] == 0xEF &&
        (afw_octet_t)s[1] == 0xBB &&
        (afw_octet_t)s[2] == 0xBF)
    {
        s += 3;
        len -= 3;
    }

    /* ICU only supports 32 bit non-negative lengths. */
    if (len > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "ICU implementation restrict - len to large or negative", xctx);
    }

    /* Do a fast check to determine if further normalization is required. */
    string = s;
    length = (int32_t)len;
    is_nfc = UNORM_YES;
    for (i = 0; i < length;) {
        U8_NEXT(string, i, length, c);

        /* If codepoint is invalid ... */
        if (c < 0) {
            /*
             * If "is" option, return non-NULL pointer to indicate not valid.
             */
            if (option == afw_utf8_nfc_option_is_valid ||
                option == afw_utf8_nfc_option_is_nfc)
            {
                return &impl_utf8_null;
            }

            /* For other options, throw an error. */
            AFW_THROW_ERROR_Z(general, "Not valid UTF-8", xctx);
        }

        /* If c < 0x0300, c is NFC normalized. */
        if (c < 0x0300) continue;

        /* If NFC_QUICK_CHECK is not UNORM_YES, break. */
        is_nfc = u_getIntPropertyValue(c, UCHAR_NFC_QUICK_CHECK);
        if (is_nfc != UNORM_YES) break;
    }

    /*
     * If only checking for valid utf8, return NULL to indicate input is valid
     * utf8.
     *
     * NOTE: No code before this point should access p.  See IMPORTANT note
     *       above.
     */
    if (option == afw_utf8_nfc_option_is_valid) {
        return NULL;
    }

    /*
     * If only checking for NFC normalize and know for sure already, return
     * return NULL or non-NULL result.
     */
    if (option == afw_utf8_nfc_option_is_nfc) {
        if (is_nfc == UNORM_YES) {
            return NULL;
        }
        else if (is_nfc == UNORM_NO) {
            return &impl_utf8_null;
        }
        /* is_nfc == UNORM_MAYBE will fall through. */
    }

    /*
     * If fast check indicates NFC normalized already and is a create option,
     * return result without further processing.
     */
    else if (is_nfc == UNORM_YES) {

        /* If not option create, return result using input s. */
        if (option == afw_utf8_nfc_option_create) {
            new_result = afw_pool_calloc_type(p, afw_utf8_t, xctx);
            new_result->s = s;
            new_result->len = len;
            return new_result;
        }

        /* If not option create_copy, return result using copy of input s. */
        else if (option == afw_utf8_nfc_option_create_copy) {
            new_result = afw_pool_calloc_type(p, afw_utf8_t, xctx);
            if (len > 0) {
                result_s = afw_pool_calloc(p, len, xctx);
                memcpy(result_s, s, len);
                new_result->s = result_s;
                new_result->len = len;
            }
            return new_result;
        }
    }

    /*
     * At this point, will need to convert to UTF-16 to use ICU functions.
     *
     * Use AFW_TRY to make sure malloc memory is freed.
     */
    output_utf16 = NULL;
    input_utf16 = NULL;
    AFW_TRY {

        result = NULL;

        /* Convert utf-8 to UChar[] */
        errorCode = U_ZERO_ERROR;
        string = s;
        length = (int32_t)len;
        input_utf16 = malloc(length * sizeof(UChar));
        u_strFromUTF8Lenient(input_utf16,
            length, &input_utf16_length, string, length,
            &errorCode);
        if (U_FAILURE(errorCode)) {
            AFW_THROW_ERROR_RV_Z(general, icu, errorCode,
                "u_strFromUTF8Lenient() failed", xctx);
        }

        /* Get nfc normalizer. */
        nfc = unorm2_getNFCInstance(&errorCode);
        if (U_FAILURE(errorCode)) {
            AFW_THROW_ERROR_RV_Z(general, icu, errorCode,
                "unorm2_getNFCInstance() failed", xctx);
        }

        /* If is_nfc == UNORM_MAYBE, check to see if already normalized. */
        if (is_nfc == UNORM_MAYBE) {

            /* If normalized already, set result. */
            if (unorm2_isNormalized(nfc, input_utf16,
                input_utf16_length, &errorCode))
            {

                /*
                 * If just checking for normalization, return NULL to indicate
                 * normalized.
                 */
                if (option == afw_utf8_nfc_option_is_nfc) {
                    result = NULL;
                    break;
                }

                /*
                 * If length is 0, return NULL.  Note: probably can't
                 * get to this point in code.
                 */
                if (len == 0) {
                    result = NULL;
                    break;
                }

                /* If option create, result uses input s. */
                if (option == afw_utf8_nfc_option_create) {
                    new_result = afw_pool_calloc_type(p, afw_utf8_t, xctx);
                    new_result->s = s;
                    new_result->len = len;
                    result = new_result;
                    break;
                }

                /* If not option create_copy, return result using copy of input s. */
                if (option == afw_utf8_nfc_option_create_copy) {
                    new_result = afw_pool_calloc_type(p, afw_utf8_t, xctx);
                    result_s = afw_pool_calloc(p, len, xctx);
                    memcpy(result_s, s, len);
                    new_result->s = result_s;
                    new_result->len = len;
                    result = new_result;
                    break;
                }

                /* Invalid option. */
                AFW_THROW_ERROR_Z(general, "Invalid afw_utf8_nfc() option",
                    xctx);
            }

            /*
             * If not normalized and just checking normalization, result is
             * a non-NULL pointer.
             */
            else if (option == afw_utf8_nfc_option_is_nfc) {
                    result = &impl_utf8_null;
                    break;
            }
        }

        /* At this point normalization is required. */
        output_utf16_length = unorm2_normalize(nfc,
            input_utf16, input_utf16_length,
            NULL, 0, &errorCode);
        if (errorCode != U_BUFFER_OVERFLOW_ERROR && U_FAILURE(errorCode)) {
            AFW_THROW_ERROR_RV_Z(general, icu, errorCode,
                "unorm2_normalize() preflight failed", xctx);
        }
        errorCode = U_ZERO_ERROR;
        output_utf16 = malloc(output_utf16_length * 2);
        output_utf16_length = unorm2_normalize(nfc,
            input_utf16, input_utf16_length,
            output_utf16, output_utf16_length, &errorCode);
        if (!U_SUCCESS(errorCode)) {
            AFW_THROW_ERROR_RV_Z(general, icu, errorCode,
                "unorm2_normalize() failed", xctx);
        }

        /* Convert normalized result to utf-8 and set result. */
        u_strToUTF8(NULL, 0, &length, output_utf16, output_utf16_length,
            &errorCode);
        if (errorCode != U_BUFFER_OVERFLOW_ERROR && U_FAILURE(errorCode)) {
            AFW_THROW_ERROR_RV_Z(general, icu, errorCode,
                "u_strToUTF8() preflight failed", xctx);
        }
        errorCode = U_ZERO_ERROR;
        new_result = afw_pool_calloc_type(p, afw_utf8_t, xctx);
        result_s = afw_pool_calloc(p, length, xctx);
        new_result->s = result_s;
        new_result->len = length;
        u_strToUTF8(result_s, length, &length, output_utf16,
            output_utf16_length, &errorCode);
        if (!U_SUCCESS(errorCode)) {
            AFW_THROW_ERROR_RV_Z(general, icu, errorCode,
                "u_strToUTF8() failed", xctx);
        }
        result = new_result;
    }

    /* Always free malloced memory. */
    AFW_FINALLY{
        if (input_utf16) free(input_utf16);
        if (output_utf16) free(output_utf16);
    }

    AFW_ENDTRY;

    /* Return result. */
    return result;
}


/*
 * forced_safe encode: valid text through; '^' -> '^^'; Cc and invalid
 * utf-8 runs -> '^' + uppercase hex + '^'. Whitespace/EOL left as text.
 */
#define IMPL_FORCED_SAFE_ESC ((afw_utf8_octet_t)'^')

static const afw_utf8_octet_t impl_hex_digit[] = "0123456789ABCDEF";

typedef enum {
    impl_enc_text,
    impl_enc_caret,
    impl_enc_hex
} impl_enc_kind_t;

static impl_enc_kind_t
impl_forced_safe_kind(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    afw_size_t i,
    afw_size_t *end)
{
    UChar32 cp;
    int32_t ii;
    int32_t slen;
    afw_size_t start;

    start = i;
    slen = (int32_t)len;
    ii = (int32_t)i;
    U8_NEXT(s, ii, slen, cp);
    *end = (afw_size_t)ii;
    if (*end <= start) {
        *end = start + 1;
    }

    if (cp < 0) {
        return impl_enc_hex;
    }
    if (cp == 0x5E) {
        return impl_enc_caret;
    }
    if (afw_code_point_is_whitespace_or_eol((afw_code_point_t)cp)) {
        return impl_enc_text;
    }
    if (afw_code_point_is_control((afw_code_point_t)cp)) {
        return impl_enc_hex;
    }
    return impl_enc_text;
}

static const afw_utf8_t *
impl_utf8_encode_forced_safe(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_utf8_t *result;
    afw_utf8_octet_t *out;
    afw_size_t i;
    afw_size_t end;
    afw_size_t need;
    afw_size_t o;
    afw_size_t b;
    afw_boolean_t in_hex;
    impl_enc_kind_t kind;

    if (len == AFW_UTF8_Z_LEN) {
        len = (s) ? strlen((const char *)s) : 0;
    }
    if (!s || len == 0) {
        return afw_s_a_empty_string;
    }

    need = 0;
    in_hex = false;
    for (i = 0; i < len; i = end) {
        kind = impl_forced_safe_kind(s, len, i, &end);
        if (kind == impl_enc_hex) {
            if (!in_hex) {
                need += 1;
                in_hex = true;
            }
            need += 2 * (end - i);
        }
        else {
            if (in_hex) {
                need += 1;
                in_hex = false;
            }
            need += (kind == impl_enc_caret) ? 2 : (end - i);
        }
    }
    if (in_hex) {
        need += 1;
    }

    result = afw_pool_calloc_type(p, afw_utf8_t, xctx);
    if (need == 0) {
        return result;
    }
    out = afw_pool_malloc(p, need, xctx);
    result->s = out;
    result->len = need;

    o = 0;
    in_hex = false;
    for (i = 0; i < len; i = end) {
        kind = impl_forced_safe_kind(s, len, i, &end);
        if (kind == impl_enc_hex) {
            if (!in_hex) {
                out[o++] = IMPL_FORCED_SAFE_ESC;
                in_hex = true;
            }
            for (b = i; b < end; b++) {
                out[o++] = impl_hex_digit[(s[b] >> 4) & 0x0f];
                out[o++] = impl_hex_digit[s[b] & 0x0f];
            }
        }
        else {
            if (in_hex) {
                out[o++] = IMPL_FORCED_SAFE_ESC;
                in_hex = false;
            }
            if (kind == impl_enc_caret) {
                out[o++] = IMPL_FORCED_SAFE_ESC;
                out[o++] = IMPL_FORCED_SAFE_ESC;
            }
            else {
                memcpy(out + o, s + i, end - i);
                o += end - i;
            }
        }
    }
    if (in_hex) {
        out[o++] = IMPL_FORCED_SAFE_ESC;
    }
    result->len = o;
    return result;
}


AFW_DEFINE(const afw_utf8_t *)
afw_utf8_create_no_copy(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_utf8_t *result;

    if (!s) {
        return afw_s_a_empty_string;
    }
    if (len == AFW_UTF8_Z_LEN) {
        len = strlen((const char *)s);
    }
    if (len == 0) {
        return afw_s_a_empty_string;
    }
    if (!afw_utf8_is_nfc(s, len, p, xctx)) {
        AFW_THROW_ERROR_Z(general, "Not valid UTF-8 NFC", xctx);
    }
    result = afw_pool_calloc_type(p, afw_utf8_t, xctx);
    result->s = s;
    result->len = len;
    return result;
}


AFW_DEFINE(const afw_utf8_t *)
afw_utf8_z_as_utf8(
    const afw_utf8_z_t *s_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_utf8_create_no_copy(
        (const afw_utf8_octet_t *)s_z, AFW_UTF8_Z_LEN, p, xctx);
}


AFW_DEFINE(void)
afw_utf8_set(
    afw_utf8_t *to,
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *created;

    created = afw_utf8_create(s, len, p, xctx);
    to->s = created->s;
    to->len = created->len;
}


AFW_DEFINE(void)
afw_utf8_z_set(
    afw_utf8_t *to,
    const afw_utf8_z_t *s_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_utf8_set(to, (const afw_utf8_octet_t *)s_z, AFW_UTF8_Z_LEN, p, xctx);
}


AFW_DEFINE(void)
afw_utf8_set_no_copy(
    afw_utf8_t *to,
    const afw_utf8_octet_t *s,
    afw_size_t len,
    afw_xctx_t *xctx)
{
    if (!s) {
        to->s = NULL;
        to->len = 0;
        return;
    }
    if (len == AFW_UTF8_Z_LEN) {
        len = strlen((const char *)s);
    }
    if (len == 0) {
        to->s = NULL;
        to->len = 0;
        return;
    }
    if (!afw_utf8_is_nfc(s, len, xctx->p, xctx)) {
        AFW_THROW_ERROR_Z(general, "Not valid UTF-8 NFC", xctx);
    }
    to->s = s;
    to->len = len;
}


AFW_DEFINE(void)
afw_utf8_z_set_no_copy(
    afw_utf8_t *to,
    const afw_utf8_z_t *s_z,
    afw_xctx_t *xctx)
{
    afw_utf8_set_no_copy(to, (const afw_utf8_octet_t *)s_z,
        AFW_UTF8_Z_LEN, xctx);
}


AFW_DEFINE(const afw_utf8_t *)
afw_utf8_create_forced_safe(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return impl_utf8_encode_forced_safe(s, len, p, xctx);
}


AFW_DEFINE(const afw_utf8_t *)
afw_utf8_z_create_forced_safe(
    const afw_utf8_z_t *s_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return impl_utf8_encode_forced_safe(
        (const afw_utf8_octet_t *)s_z, AFW_UTF8_Z_LEN, p, xctx);
}


AFW_DEFINE(void)
afw_utf8_set_forced_safe(
    afw_utf8_t *to,
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *created;

    created = impl_utf8_encode_forced_safe(s, len, p, xctx);
    to->s = created->s;
    to->len = created->len;
}


AFW_DEFINE(void)
afw_utf8_z_set_forced_safe(
    afw_utf8_t *to,
    const afw_utf8_z_t *s_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_utf8_set_forced_safe(to, (const afw_utf8_octet_t *)s_z,
        AFW_UTF8_Z_LEN, p, xctx);
}


AFW_DEFINE(const afw_utf8_t *)
afw_utf8_create_property_name(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *encoded;

    encoded = impl_utf8_encode_forced_safe(s, len, p, xctx);
    return afw_utf8_create(encoded->s, encoded->len, p, xctx);
}


AFW_DEFINE(const afw_utf8_t *)
afw_utf8_z_create_property_name(
    const afw_utf8_z_t *s_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_utf8_create_property_name(
        (const afw_utf8_octet_t *)s_z, AFW_UTF8_Z_LEN, p, xctx);
}


static void
impl_throw_if_embedded_nul(
    const afw_utf8_octet_t *s, afw_size_t len, afw_xctx_t *xctx)
{
    if (s && len > 0 && memchr(s, 0, len)) {
        AFW_THROW_ERROR_Z(general,
            "Embedded NUL in utf-8 string used as C string",
            xctx);
    }
}


/*
 * Convert utf8 to utf8_z in specified pool.
 */
AFW_DEFINE(const afw_utf8_z_t *)
afw_utf8_to_utf8_z(
    const afw_utf8_t *string, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_z_t *result;

    if (!string || string->len == 0) {
        return impl_z_empty;
    }

    impl_throw_if_embedded_nul(string->s, string->len, xctx);

    result = afw_pool_malloc(p, string->len + 1, xctx);
    memcpy(result, string->s, string->len);
    result[string->len] = 0;
    return result;
}


/*
 * Create a NFC normalized zero terminated UTF-8 string in specified
 * pool.
 */
AFW_DEFINE(const afw_utf8_z_t *)
afw_utf8_z_create(
    const afw_utf8_octet_t *s, afw_size_t len, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t *temp;
    afw_size_t s_len;
    afw_utf8_z_t *s_z;

    /* Determine len and if 0, return empty string. */
    s_len = (len == AFW_UTF8_Z_LEN) ? strlen(s) : len;
    if (!s || s_len == 0) {
        return impl_z_empty;
    }

    impl_throw_if_embedded_nul(s, s_len, xctx);

    /* Allocate memory for string including length byte. */
    s_z = afw_pool_malloc(p, s_len + 1, xctx);

    memcpy(s_z, s, s_len);
    s_z[s_len] = 0;

    /* Give afw_utf8_nfc a chance to normalize result. */
    temp = afw_utf8_nfc(s_z, s_len + 1, afw_utf8_nfc_option_create,
        p, xctx);

    /* Return just s part of temp. */
    return temp->s;
}


/* Convert character encoding to a utf-8 in specified pool. */
AFW_DECLARE(const afw_utf8_t *) afw_utf8_from_encoding(
    const afw_utf8_t * from_encoding,
    const char* * from, afw_size_t * from_size,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}


/* Concatenate strings in specifed pool. */
AFW_DEFINE_ELLIPSIS(const afw_utf8_t *) afw_utf8_concat(const afw_pool_t *p,
    afw_xctx_t *xctx, ...)
{
    va_list strings;
    const afw_utf8_t *result;

    /* Calculate size needed to hold concatenated strings. */
    va_start(strings, xctx);
    result = afw_utf8_concat_v(p, xctx, strings);
    va_end(strings);

    /* Return result. */
    return result;
}

/* Concatenate strings in specifed pool. */
AFW_DEFINE(const afw_utf8_t *) afw_utf8_concat_v(
    const afw_pool_t *p, afw_xctx_t *xctx, va_list strings)
{
    afw_utf8_t *string;
    afw_utf8_octet_t *c;
    afw_utf8_t *result;
    va_list original_strings;

    va_copy(original_strings,strings);
    /* Allocate memory for afw_utf8_t result. */
    result = afw_pool_calloc_type(p, afw_utf8_t, xctx);

    /* Calculate size needed to hold concatenated strings. */
    while ((string = va_arg(strings, afw_utf8_t *))) {
        result->len += string->len;
    }

    /* Allocate memory and concatenate strings. */
    if (result->len > 0) {
        c = (afw_utf8_octet_t *)afw_pool_malloc(p,
            result->len, xctx);
        result->s = c;
        while ((string = va_arg(original_strings, afw_utf8_t *))) {
            if (string->len > 0) {
                memcpy(c, string->s, string->len);
                c += string->len;
            }
        }
    }

    /* Return result. */
    return result;
}



/*
 * AFW_UTF8_FMT is "%.*s". libc %.*s stops at an interior 0. Copy n
 * bytes, then forced_safe the assembled buffer (length, not strlen).
 * Viewable text (logs, errors, traces), not a data-file writer.
 *
 * va_arg lives in this function. Do not pass ap to apr_pvsprintf
 * mid-walk (the list is then indeterminate).
 */
#define IMPL_PSPRINTF(val) \
    ((star_width && star_prec) \
        ? apr_psprintf(apr_p, spec, aw, aprec, (val)) \
        : star_width \
            ? apr_psprintf(apr_p, spec, aw, (val)) \
            : star_prec \
                ? apr_psprintf(apr_p, spec, aprec, (val)) \
                : apr_psprintf(apr_p, spec, (val)))

static void
impl_format_to_writer(
    const afw_writer_t *w,
    const afw_utf8_z_t *format_z,
    va_list ap,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    apr_pool_t *apr_p;
    const afw_utf8_z_t *f;
    const afw_utf8_z_t *start;
    const afw_utf8_z_t *piece;
    const char *s;
    char spec[64];
    char conv;
    char lenmod;
    int star_width;
    int star_prec;
    int aw;
    int aprec;
    int n;
    afw_size_t spec_len;

    apr_p = afw_pool_get_apr_pool(p);
    f = format_z;
    while (*f) {
        if (*f != '%') {
            start = f;
            while (*f && *f != '%') {
                f++;
            }
            afw_writer_write(w, start, (afw_size_t)(f - start), xctx);
            continue;
        }
        if (f[1] == '%') {
            afw_writer_write(w, "%", 1, xctx);
            f += 2;
            continue;
        }
        /* AFW_UTF8_FMT: copy n bytes, including interior 0. */
        if (f[1] == '.' && f[2] == '*' && f[3] == 's') {
            n = va_arg(ap, int);
            s = va_arg(ap, const char *);
            if (n > 0 && s) {
                afw_writer_write(w, s, (afw_size_t)n, xctx);
            }
            f += 4;
            continue;
        }
        start = f;
        f++;
        star_width = 0;
        star_prec = 0;
        lenmod = 0;
        while (*f && strchr("-+ #0'", *f)) {
            f++;
        }
        if (*f == '*') {
            star_width = 1;
            f++;
        }
        else {
            while (*f >= '0' && *f <= '9') {
                f++;
            }
        }
        if (*f == '.') {
            f++;
            if (*f == '*') {
                star_prec = 1;
                f++;
            }
            else {
                while (*f >= '0' && *f <= '9') {
                    f++;
                }
            }
        }
        if (*f == 'h') {
            lenmod = 'h';
            f++;
            if (*f == 'h') {
                f++;
            }
        }
        else if (*f == 'l') {
            lenmod = 'l';
            f++;
            if (*f == 'l') {
                lenmod = 'L';
                f++;
            }
        }
        else if (*f && strchr("Lztjq", *f)) {
            lenmod = *f;
            f++;
        }
        if (!*f) {
            afw_writer_write(w, start, (afw_size_t)(f - start), xctx);
            break;
        }
        conv = *f++;
        spec_len = (afw_size_t)(f - start);
        if (spec_len == 0 || spec_len >= sizeof(spec)) {
            afw_writer_write(w, start, spec_len, xctx);
            continue;
        }
        memcpy(spec, start, spec_len);
        spec[spec_len] = 0;
        aw = 0;
        aprec = 0;
        if (star_width) {
            aw = va_arg(ap, int);
        }
        if (star_prec) {
            aprec = va_arg(ap, int);
        }
        piece = NULL;
        if (conv == 's') {
            s = va_arg(ap, const char *);
            piece = IMPL_PSPRINTF(s);
        }
        else if (conv == 'c') {
            n = va_arg(ap, int);
            piece = IMPL_PSPRINTF(n);
        }
        else if (conv == 'p') {
            piece = IMPL_PSPRINTF(va_arg(ap, void *));
        }
        else if (strchr("di", conv)) {
            if (lenmod == 'L' || lenmod == 'j' || lenmod == 'q') {
                piece = IMPL_PSPRINTF(va_arg(ap, long long));
            }
            else if (lenmod == 'l') {
                piece = IMPL_PSPRINTF(va_arg(ap, long));
            }
            else if (lenmod == 'z' || lenmod == 't') {
                piece = IMPL_PSPRINTF(va_arg(ap, ptrdiff_t));
            }
            else {
                piece = IMPL_PSPRINTF(va_arg(ap, int));
            }
        }
        else if (strchr("uoxX", conv)) {
            if (lenmod == 'L' || lenmod == 'j' || lenmod == 'q') {
                piece = IMPL_PSPRINTF(va_arg(ap, unsigned long long));
            }
            else if (lenmod == 'l') {
                piece = IMPL_PSPRINTF(va_arg(ap, unsigned long));
            }
            else if (lenmod == 'z' || lenmod == 't') {
                piece = IMPL_PSPRINTF(va_arg(ap, afw_size_t));
            }
            else {
                piece = IMPL_PSPRINTF(va_arg(ap, unsigned));
            }
        }
        else if (strchr("fFeEgGaA", conv)) {
            if (lenmod == 'L') {
                piece = IMPL_PSPRINTF(va_arg(ap, long double));
            }
            else {
                piece = IMPL_PSPRINTF(va_arg(ap, double));
            }
        }
        if (piece) {
            afw_writer_write_z(w, piece, xctx);
        }
        else {
            afw_writer_write(w, start, spec_len, xctx);
        }
    }
}

#undef IMPL_PSPRINTF


static const afw_utf8_t *
impl_printf_forced_safe(
    const afw_utf8_z_t *format_z,
    va_list ap,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_writer_t *w;
    afw_utf8_t assembled;
    const afw_utf8_t *safe;

    w = afw_utf8_writer_create(NULL, p, xctx);
    impl_format_to_writer(w, format_z, ap, p, xctx);
    afw_utf8_writer_current_string(w, &assembled, xctx);
    safe = afw_utf8_create_forced_safe(
        assembled.s, assembled.len, p, xctx);
    afw_writer_release(w, xctx);
    return safe;
}


/* Create a string using a c format string. */
AFW_DEFINE_ELLIPSIS(const afw_utf8_t *)
afw_utf8_printf(
    const afw_pool_t *p, afw_xctx_t *xctx, const afw_utf8_z_t *format, ...)
{
    va_list arg;
    const afw_utf8_t *result;

    va_start(arg, format);
    result = impl_printf_forced_safe(format, arg, p, xctx);
    va_end(arg);
    return result;
}


/* Create a string using a c format string. */
AFW_DEFINE(const afw_utf8_t *)
afw_utf8_printf_v(
    const afw_utf8_z_t *format, va_list arg,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    return impl_printf_forced_safe(format, arg, p, xctx);
}



/* Clone a utf-8 string into a specific pool. */
AFW_DEFINE(const afw_utf8_t *)
afw_utf8_clone(
    const afw_utf8_t *string, const afw_pool_t *p, afw_xctx_t *xctx)
{
    return (string)
        ? afw_utf8_create(string->s, string->len, p, xctx)
        : NULL;
}


/* Set result to a substring of string using byte indexes. */
AFW_DEFINE(void)
afw_utf8_substring_byte(
    afw_utf8_t *result, const afw_utf8_t *string, afw_size_t start,
    afw_size_t end)
{
    if (end > string->len) end = string->len;
    result->len = (end > start) ? end - start : 0;
    result->s = (result->len > 0) ? string->s + start : NULL;
}


/* Check to see if a string starts with another string. */
AFW_DEFINE(afw_boolean_t) afw_utf8_starts_with(
    const afw_utf8_t *string, const afw_utf8_t *starts_with)
{
    return (string->len >= starts_with->len &&
        memcmp(string->s, starts_with->s, starts_with->len) == 0);
}


/* Check to see if a string starts with a utf8_z string. */
AFW_DEFINE(afw_boolean_t) afw_utf8_starts_with_utf8_z(
    const afw_utf8_t *string, const afw_utf8_z_t *starts_with_z)
{
    afw_size_t len = strlen(starts_with_z);
    return (string->len >= len &&
        memcmp(string->s, starts_with_z, len) == 0);
}


/* Returns true if series of bytes for len s1 starts with s2_z. */
AFW_DEFINE(afw_boolean_t)
afw_utf8_len_starts_with_utf8_z(
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


/* Returns true if zero terminated s1 starts with s2. */
AFW_DEFINE(afw_boolean_t)
afw_utf8_z_starts_with(
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


/* Check to see if a string ends with another string. */
AFW_DEFINE(afw_boolean_t)
afw_utf8_ends_with(
    const afw_utf8_t *string, const afw_utf8_t *ends_with)
{
    return (string->len >= ends_with->len &&
        memcmp(string->s + (string->len - ends_with->len),
            ends_with->s, ends_with->len) == 0);
}


/* Check to see if a string ends with a utf8_z string. */
AFW_DEFINE(afw_boolean_t)
afw_utf8_ends_with_utf8_z(
    const afw_utf8_t *string, const afw_utf8_z_t *ends_with_z)
{
    afw_size_t len = strlen(ends_with_z);
    return (string->len >= len &&
        memcmp(string->s + (string->len - len), ends_with_z, len) == 0);
}


/* Check to see if a string equals another string. */
AFW_DEFINE(afw_boolean_t) afw_utf8_equal(
    const afw_utf8_t *s1, const afw_utf8_t *s2)
{
    return afw_utf8_compare(s1, s2) == 0;
}


/* Check to see if a string contains another string. */
AFW_DEFINE(afw_boolean_t)
afw_utf8_contains(
    const afw_utf8_t *s1, const afw_utf8_t *s2)
{
    UChar32 cp;
    int32_t i;
    int32_t length;
    int32_t prev;

    if (!s1) {
        return false;
    }

    if (!s2) {
        return true;
    }

    /*
     * Strings are assumed valid UTF-8. Step by code point so search does not
     * start mid multi-byte sequence (#153). No xctx on this API — use ICU
     * U8_NEXT (same as afw_utf8_next_code_point body).
     */
    if (s1->len > (afw_size_t)INT32_MAX || s2->len > (afw_size_t)INT32_MAX) {
        return false;
    }
    length = (int32_t)s1->len;
    for (i = 0; s2->len <= (afw_size_t)(length - i); ) {
        if (memcmp(s1->s + i, s2->s, s2->len) == 0) {
            return true;
        }
        prev = i;
        U8_NEXT((const uint8_t *)s1->s, i, length, cp);
        if (cp < 0 || i <= prev) {
            break;
        }
    }

    return false;
}


/* Compare two strings. */
AFW_DEFINE(int) afw_utf8_compare(
    const afw_utf8_t *s1, const afw_utf8_t *s2)
{
    /*
    UErrorCode status = U_ZERO_ERROR; 
    UCharIterator sIter, tIter;

    uiter_setUTF8(&sIter, s1->s, s1->len);
    uiter_setUTF8(&tIter, s2->s, s2->len);
    return ucol_strcollIter(myCollation, &sIter, &tIter, &status);
     */

    int result;

    if (!s1 || !s2) {
        if (!s1 && !s2) {
            return 0;
        }
        if (s1) {
            return 1;
        }
        return -1;
    }

    if (s1->len == s2->len) {
        return memcmp(s1->s, s2->s, s1->len);
    }
    else if (s1->len < s2->len) {
        result = memcmp(s1->s, s2->s, s1->len);
        if (result == 0) {
            return -1;
        }
        return result;
    }
    else {
        result = memcmp(s1->s, s2->s, s2->len);
        if (result == 0) {
            return 1;
        }
        return result;
    }
}


/* Convert utf-8 sting to lower case in specified pool. */
AFW_DEFINE(const afw_utf8_t *) afw_utf8_to_lower(
    const afw_utf8_t *s, const afw_pool_t *p, afw_xctx_t *xctx)
{
    UChar32 c;
    UChar32 c_lower;
    int32_t i1, i2, len1, len2;
    const uint8_t *cs1;
    uint8_t *cs2;
    afw_utf8_t *result;
    afw_boolean_t already_lower_case;
    UBool isError;

    if (!s) {
        return NULL;
    }

    if (s->len == 0) {
        return s;
    }

    /* ICU only supports 32 bit non-negative lengths. */
    if (s->len > AFW_INT32_MAX) {
        AFW_THROW_ERROR_Z(general,
            "ICU implementation restrict - len to large or negative", xctx);
    }

    len1 = (int32_t) s->len;
    cs1 = (const uint8_t *)s->s;

    /* Pre-scan to see if already lower case and to get length of result. */
    for (i1 = 0, len2 = 0, already_lower_case = true; i1 < len1; )
    {
        U8_NEXT(cs1, i1, len1, c);
        if (c < 0) {
            AFW_THROW_ERROR_Z(general, "Not valid UTF-8", xctx);
        }
        len2 += U8_LENGTH(c);
        if (c != u_tolower(c)) {
            already_lower_case = false;
        }
    }

    /* If already lower case, return s passed. */
    if (already_lower_case) {
        return s;
    }

    /* Convert s to lower case. */
    cs2 = afw_pool_calloc(p, len2, xctx);
    result = afw_pool_calloc_type(p, afw_utf8_t, xctx);
    result->s = (const afw_utf8_octet_t *)cs2;
    result->len = len2;

    for (i1 = 0, i2 = 0; i1 < len1;) {
        U8_NEXT(cs1, i1, len1, c);
        if (c < 0) {
            AFW_THROW_ERROR_Z(general, "Not valid UTF-8", xctx);
        }
        c_lower = u_tolower(c);
        isError = false;
        U8_APPEND(cs2, i2, len2, c_lower, isError);
        if (isError) {
            AFW_THROW_ERROR_Z(general, "Not valid UTF-8", xctx);
        }
    }

    return result;
}

/* Create a utf-8 sting with spaces normalized in specified pool. */
AFW_DEFINE(const afw_utf8_t *) afw_utf8_normalize_space(
    const afw_utf8_t *s, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_octet_t *c, *start, *end, *start_fix;
    afw_utf8_octet_t *new_c;
    afw_size_t len, result_len, new_len;
    afw_utf8_t *result;
    afw_boolean_t last_ws;

    /* If len is 0, just return input s. */
    if (s->len == 0) return s;

    /* Set start after leading whitespace. */
    for (c = s->s, len = s->len; len > 0 && IMPL_WHITESPACE(*c); len--, c++);
    start = c;

    /* Set end past last char before trailing whitespace. */
    for (c = s->s + s->len - 1; len > 0 && IMPL_WHITESPACE(*c); len--, c--);
    end = c + 1;

    /* Check for whitespace sequences between start and end. */
    result_len = len;
    for (c = start, start_fix = NULL; len > 0; len--, c++) {
        if (IMPL_WHITESPACE(*c)) {
            if (start_fix) break;
            start_fix = c;
            if (*c != 0x20) break;
        }
        else {
            start_fix = NULL;
        }
    }

    /* If no leading, trailing, or embedded whitespace sequences, return s. */
    if (!start_fix && start == s->s && end == s->s + s->len) return s;

    /* Make a new afw_utf_t and set to not include leading and trailing ws. */
    result = afw_pool_calloc_type(p, afw_utf8_t, xctx);
    result->len = result_len;
    result->s = start;
    
    /* If there are no embedded sequences, return this. */
    if (!start_fix) {
        return result;
    }

    /* Figure out how long new string will be with sequences removed. */
    new_len = result->len;
    len = end - start_fix;
    for (c = start_fix, last_ws = false; len > 0; len--, c++)
    {
        if (IMPL_WHITESPACE(*c)) {
            if (last_ws) new_len--;
            last_ws = true;
        }
        else {
            last_ws = false;
        }
    }

    /* Return new string populated with sequences replaced with 0x20. */
    c = result->s;
    new_c = afw_pool_malloc(p, new_len, xctx);
    result->s = new_c;
    len = result->len;
    result->len = new_len;
    for (last_ws = false; len > 0; c++, len--) {
        if (IMPL_WHITESPACE(*c)) {
            if (!last_ws) *new_c++ = 0x20;
            last_ws = true;
        }
        else {
            *new_c++ = *c;
            last_ws = false;
        }
    }
    return result;
}


/* Compare two strings. */
AFW_DEFINE(int) afw_utf8_compare_ignore_case(
    const afw_utf8_t *s1, const afw_utf8_t *s2, afw_xctx_t *xctx)
{
    UChar32 c1, c2;
    int32_t i1, i2, len1, len2;
    const uint8_t *cs1, *cs2;

    /* ICU only supports 32 bit non-negative lengths. */
    if (s1->len > AFW_INT32_MAX ||
        s2->len > AFW_INT32_MAX)
    {
        AFW_THROW_ERROR_Z(general,
            "ICU implementation restrict - len to large or negative", xctx);
    }

    cs1 = (const uint8_t *)s1->s;
    cs2 = (const uint8_t *)s2->s;
    len1 = (int32_t)s1->len;
    len2 = (int32_t)s2->len;
    i1 = 0;
    i2 = 0;

    while (i1 < len1 && i2 < len2) {
        U8_NEXT(cs1, i1, len1, c1);
        U8_NEXT(cs2, i2, len2, c2);
        if (c1 < 0 || c2 < 0) {
            AFW_THROW_ERROR_Z(general, "Not valid UTF-8", xctx);
        }
        c1 = u_tolower(c1);
        c2 = u_tolower(c2);
        if (c1 != c2) {
            return (c1 > c2) ? 1 : -1;
        }
    }

    while (i1 < len1) {
        U8_NEXT(cs1, i1, len1, c1);
        if (c1 < 0) {
            AFW_THROW_ERROR_Z(general, "Not valid UTF-8", xctx);
        }
        return 1;
    }
    while (i2 < len2) {
        U8_NEXT(cs2, i2, len2, c2);
        if (c2 < 0) {
            AFW_THROW_ERROR_Z(general, "Not valid UTF-8", xctx);
        }
        return -1;
    }

    return 0;
}


/* Check to see if a string equals a utf8_z string. */
AFW_DEFINE(afw_boolean_t) afw_utf8_equal_utf8_z(
    const afw_utf8_t *s1, const afw_utf8_z_t *s2_z)
{
    afw_size_t len = (s2_z) ? strlen(s2_z) : 0;
    return (s1->len == len &&
        (s1->len == 0 || memcmp(s1->s, s2_z, len) == 0));
}


/* Compare two zero terminated utf-8 strings ignoring case. */
AFW_DEFINE(int)
afw_utf8_z_compare_ignore_case(
    const afw_utf8_z_t *s1, const afw_utf8_z_t *s2, afw_xctx_t *xctx)
{
    afw_utf8_t a1, a2;

    afw_utf8_z_set_no_copy(&a1, s1, xctx);
    afw_utf8_z_set_no_copy(&a2, s2, xctx);

    return afw_utf8_compare_ignore_case(&a1, &a2, xctx);
}


/* Compare two zero terminated UTF8 strings to be equal. */
AFW_DEFINE(afw_boolean_t)
afw_utf8_z_equal(
    const afw_utf8_z_t *s1, const afw_utf8_z_t *s2)
{
    return (s1 && s2 && (strcmp((const char *)s1, (const char *)s2) == 0
        || s1 == s2)) ? TRUE : FALSE;
}


/*
 * Compare two zero terminated UTF8 strings to be equal, ignoring case.
 *
 * @fixme See header: needs xctx when XACML no longer uses this.
 */
AFW_DEFINE(afw_boolean_t)
afw_utf8_z_equal_ignore_case(
    const afw_utf8_z_t *s1, const afw_utf8_z_t *s2)
{
    return (strcasecmp((const char *)s1, (const char *)s2) == 0)
        ? true : false;
}


/* Concatenate zero terminated UTF8 strings. */
static const afw_utf8_z_t * impl_u8z_concat_v(
    const afw_pool_t *p, afw_xctx_t *xctx, va_list ap)
{
    afw_size_t sz;
    afw_utf8_z_t *s;
    afw_utf8_z_t *s2;
    afw_utf8_z_t *result;
    va_list original_ap;

    va_copy(original_ap, ap);

    /* Calculate size needed to hold concatenated strings. */
    sz = 1;
    while ((s = va_arg(ap, afw_utf8_z_t *))) {
        sz += strlen((const char *)s);
    }

    /* Allocate memory and concatenate strings. */
    s2 = (afw_utf8_z_t *)afw_pool_malloc(p, sz, xctx);
    result = s2;
    while ((s = va_arg(original_ap, afw_utf8_z_t *))) {
        while ((*s2++ = *s++));
        s2--;
    }

    /* Return result. */
    return result;
}

/* Concatenate zero terminated UTF8 strings. */
AFW_DEFINE_ELLIPSIS(const afw_utf8_z_t *) afw_utf8_z_concat(
    const afw_pool_t *p, afw_xctx_t *xctx, ...)
{
    va_list ap;
    const afw_utf8_z_t *result;

    /* Calculate size needed to hold concatenated strings. */
    va_start(ap, xctx);
    result = impl_u8z_concat_v(p, xctx, ap);
    va_end(ap);

    /* Return result. */
    return result;
}


/* Create a utf8_z string using a c format string and va_list in specified pool. */
AFW_DEFINE(const afw_utf8_z_t *)
afw_utf8_z_printf_v(
    const afw_utf8_z_t *format_z, va_list ap,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *safe;
    afw_utf8_z_t *z;
    afw_size_t n;

    safe = impl_printf_forced_safe(format_z, ap, p, xctx);
    n = safe->len;
    z = afw_pool_malloc(p, n + 1, xctx);
    if (n > 0) {
        memcpy(z, safe->s, n);
    }
    z[n] = 0;
    return z;
}


/* Create a utf8_z string using a c format string in specified pool. */
AFW_DEFINE_ELLIPSIS(const afw_utf8_z_t *)
afw_utf8_z_printf(
    const afw_pool_t *p, afw_xctx_t *xctx, const afw_utf8_z_t *format_z, ...)
{
    va_list ap;
    const afw_utf8_z_t *result;

    va_start(ap, format_z);
    result = afw_utf8_z_printf_v(format_z, ap, p, xctx);
    va_end(ap);

    return result;
}

/* Clone a pointer array of utf-8 to specified pool. */
AFW_DEFINE(const afw_utf8_t * const *)
afw_utf8_clone_pointer_array(
    afw_size_t count,
    const afw_utf8_t * const * pointers,
    afw_boolean_t NULL_terminate,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t * const *in;
    afw_utf8_t * *out;
    afw_utf8_octet_t *s;
    const afw_utf8_t * const *result;
 
    if (!pointers) return NULL;

    if (count == -1) {
        for (in = pointers, count = 0; *in; in++, count++);
    }

    if (count == 0 && !NULL_terminate) return NULL;

    out = afw_pool_malloc(p,
        ((NULL_terminate) ? count + 1 : count) * sizeof(afw_utf8_t *),
        xctx);
    result = (const afw_utf8_t * const *)out;

    for (in = pointers; count > 0; count--, in++, out++) {
        *out = afw_pool_calloc_type(p, afw_utf8_t, xctx);
        if ((*in)->len > 0) {
            (*out)->len = (*in)->len;
            s = afw_pool_malloc(p, (*in)->len, xctx);
            (*out)->s = s;
            memcpy(s, (*in)->s, (*in)->len);
        }
    }

    if (NULL_terminate) {
        *out = NULL;
    }

    return result;
}


/* Concat array of utf-8 with optional separator to specified pool. */
AFW_DEFINE(const afw_utf8_t *)
afw_utf8_array_to_utf8_with_separator(
    const afw_utf8_t * const * strings,
    const afw_utf8_t * separator,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_size_t n, count;
    afw_size_t len;
    const afw_utf8_t * const * c;
    afw_utf8_t * result;
    afw_utf8_octet_t * s;

    if (!strings || *strings == NULL) return afw_s_a_empty_string;

    len = 0;
    for (count = 0, c = strings; *c; count++, c++)
    {
        len += (*c)->len;
    }

    if (separator) {
        len += (count - 1) * separator->len;
    }

    if (len == 0) return afw_s_a_empty_string;

    s = afw_pool_malloc(p, len, xctx);
    result = afw_pool_calloc_type(p, afw_utf8_t, xctx);
    result->s = s;
    result->len = len;

    for (n = 1, c = strings; *c; n++, c++) {
        memcpy(s, (*c)->s, (*c)->len);
        s += (*c)->len;
        if (n < count && separator) {
            memcpy(s, separator->s, separator->len);
            s += separator->len;
        }
    }

    return result;
}


/* Concat array of utf-8 with optional separator to specified pool. */
AFW_DEFINE(const afw_utf8_z_t *)
afw_utf8_array_to_utf8_z_with_separator(
    const afw_utf8_t * const * strings,
    const afw_utf8_t * separator,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_size_t n, count;
    afw_size_t len;
    const afw_utf8_t * const * c;
    const afw_utf8_z_t * result;
    afw_utf8_z_t * o;

    if (!strings || *strings == NULL) return "";

    len = 1;
    for (count = 0, c = strings; *c; count++, c++)
    {
        impl_throw_if_embedded_nul((*c)->s, (*c)->len, xctx);
        len += (*c)->len;
    }

    if (separator) {
        impl_throw_if_embedded_nul(separator->s, separator->len, xctx);
        len += (count - 1) * separator->len;
    }

    if (len == 1) return "";

    o = afw_pool_malloc(p, len, xctx);
    result = o;
    for (n = 1, c = strings; *c; n++, c++) {
        memcpy(o, (*c)->s, (*c)->len);
        o += (*c)->len;
        if (n < count && separator) {
            memcpy(o, separator->s, separator->len);
            o += separator->len;
        }
    }
    *o = 0;

    return result;
}


/* Concat array of utf-8 with optional separator to specified pool. */
AFW_DEFINE(const afw_utf8_z_t *)
afw_utf8_z_array_with_separator(
    const afw_utf8_z_t * const * strings_z,
    const afw_utf8_t * separator,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_size_t n, count;
    afw_size_t len;
    const afw_utf8_z_t * const * c_z;
    const afw_utf8_z_t * result;
    afw_utf8_z_t * o;

    if (!strings_z || *strings_z == 0) return "";

    len = 1;
    for (count = 0, c_z = strings_z; *c_z; count++, c_z++)
    {
        len += strlen(*c_z);
    }

    if (separator) {
        impl_throw_if_embedded_nul(separator->s, separator->len, xctx);
        len += (count - 1) * separator->len;
    }

    if (len == 1) return "";

    o = afw_pool_malloc(p, len, xctx);
    result = o;
    for (n = 1, c_z = strings_z; *c_z; n++, c_z++) {
        memcpy(o, *c_z, strlen(*c_z));
        o += strlen(*c_z);
        if (n < count && separator) {
            memcpy(o, separator->s, separator->len);
            o += separator->len;
        }
    }
    *o = 0;

    return result;
}


/* Returns pointer in path_z past last / or \. */
AFW_DEFINE(const afw_utf8_z_t *)
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


/* Returns value of source_z after last '/ 'or '\'. */
AFW_DEFINE(const afw_utf8_z_t *)
afw_utf8_z_source_file(const afw_utf8_z_t *source_z) {
    const afw_utf8_z_t *result;
    const afw_utf8_z_t *c;

    result = source_z;
    if (result) {
        for (c = result; *c; c++) {
            if (*c == '/' || *c == '\\') {
                result = c + 1;
            }
        }
    }

    return result;
}



/* Determine the line and column of an offset in a string. */
/** @todo change this to also return code point offset. */
AFW_DEFINE(afw_boolean_t)
afw_utf8_line_column_of_offset(
    afw_size_t *line_number,
    afw_size_t *column_number,
    const afw_utf8_t *s,
    afw_size_t offset,
    int tab_size,
    afw_xctx_t *xctx)
{
    afw_size_t newlines;
    afw_size_t line_offset;
    const afw_octet_t *c;
    const afw_octet_t *end;
    afw_boolean_t result;

    for (
        newlines = line_offset = 0,
        c =  (const afw_octet_t *)s->s,
        end = c + (offset <= s->len ? offset : s->len);
        c < end;
        c++)
    {
        if (*c == '\n') {
            newlines++;
            line_offset = 0;
        }
        else if (*c == '\t') {
            line_offset = (line_offset + tab_size) % tab_size * tab_size;
        }
        else if ((*c < 128 || *c >= 0b11000000) && *c != '\r') {
            line_offset++;
        }
    }

    if (newlines == 0) {
        *line_number = 1;
        *column_number = line_offset + 1;
        result = false;
    }
    else {
        *line_number = newlines + 1;
        *column_number = line_offset + 1;
        result = true;
    }

    return result;
}



/* Determine the line and column of an offset in a string. */
/** @todo change this to also return octet offset. */
AFW_DEFINE(void)
afw_utf8_line_count_and_max_column(
    afw_size_t *number_of_lines,
    afw_size_t *max_column_number,
    const afw_utf8_t *s,
    int tab_size,
    afw_xctx_t *xctx)
{
    afw_size_t column_number;
    afw_code_point_t cp;
    afw_size_t offset;

    *number_of_lines = 1;
    *max_column_number = 0;

    for (offset = 0, column_number=1;;) {
        cp = afw_utf8_next_code_point(s->s, &offset, s->len, xctx);
        if (cp < 0) {
            break;
        }
        if (cp == '\t') {
            column_number += tab_size;
        }
        else if (afw_code_point_is_eol(cp)) {
            *number_of_lines += 1;
            column_number = 1;
        }
        else {
            column_number++;
        }
        if (*max_column_number < column_number) {
            *max_column_number = column_number;
        }
    }
}



AFW_DEFINE(const afw_utf8_t * const *)
afw_utf8_parse_csv(
    const afw_utf8_t *s,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_size_t count;
    afw_size_t sz;
    const afw_utf8_octet_t *c;
    const afw_utf8_octet_t *b;
    const afw_utf8_t **result;
    const afw_utf8_t **v;

    for (count = 1, sz = s->len, c = s->s; sz > 0; sz--, c++) {
        if (*c == ',') count++;
    }

    result = afw_pool_calloc(p, sizeof(afw_utf8_t *)*(count+1), xctx);
    for (sz = s->len, b = c = s->s, v = result; ; sz--, c++) {
        if (sz <= 0 || *c == ',' || *c == ';') {
            *v = afw_utf8_create(b, c - b, p, xctx);
            v++;
            if (sz <= 0) break;
            if (*c == ';') {
                for (; sz > 0 && *c != ','; sz--, c++);
                if (sz <= 0) break;
            }
            b = c + 1;
        }
    }

    *v = NULL;
    return result;
}


