// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW - Regular expressions
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

/**
 * @file afw_utf8_regexp.c
 * @brief Regular expressions on UTF-8 strings (ICU uregex over UText).
 *
 * ICU reads the UTF-8 directly through UText, so neither the pattern nor
 * the subject is converted to UTF-16 by the caller. A URegularExpression
 * holds the current subject and match state, so the compiled one is
 * never matched itself: each find matches on a clone, which shares the
 * compiled pattern.
 */

#include "afw_internal.h"
#include <unicode/uregex.h>
#include <unicode/utext.h>


struct afw_utf8_regexp_s {
    URegularExpression *rx;
};


static void
impl_regexp_cleanup(
    void *data, void *data2, const afw_pool_t *p, afw_xctx_t *xctx)
{
    (void)data2;
    (void)p;
    (void)xctx;
    uregex_close(((afw_utf8_regexp_t *)data)->rx);
}


AFW_DEFINE(const afw_utf8_regexp_t *)
afw_utf8_regexp_compile(
    const afw_utf8_t *pattern,
    afw_uint32_t flags,
    const afw_utf8_t **error_message,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_utf8_regexp_t *self;
    UText ut = UTEXT_INITIALIZER;
    UParseError pe;
    UErrorCode status;
    URegularExpression *rx;
    uint32_t icu_flags;

    *error_message = NULL;

    /* Allocate first, so nothing can throw while rx is not registered. */
    self = afw_pool_calloc_type(p, afw_utf8_regexp_t, xctx);

    icu_flags = 0;
    if (flags & AFW_UTF8_REGEXP_CASE_INSENSITIVE) {
        icu_flags |= UREGEX_CASE_INSENSITIVE;
    }

    status = U_ZERO_ERROR;
    utext_openUTF8(&ut, pattern->s, (int64_t)pattern->len, &status);
    if (U_FAILURE(status)) {
        AFW_THROW_ERROR_RV_Z(general, icu, status,
            "utext_openUTF8() failed", xctx);
    }

    /* The pattern is copied, so ut can be closed right away. */
    rx = uregex_openUText(&ut, icu_flags, &pe, &status);
    utext_close(&ut);

    if (U_FAILURE(status)) {
        if (rx) {
            uregex_close(rx);
        }
        if (status == U_MEMORY_ALLOCATION_ERROR) {
            AFW_THROW_ERROR_RV_Z(memory, icu, status,
                "uregex_openUText() failed", xctx);
        }
        *error_message = afw_utf8_printf(p, xctx,
            "%s at line %d offset %d",
            u_errorName(status), pe.line, pe.offset);
        return NULL;
    }

    self->rx = rx;
    afw_pool_register_cleanup(p, self, NULL, impl_regexp_cleanup, xctx);

    return self;
}


AFW_DEFINE(const afw_utf8_regexp_t *)
afw_utf8_regexp_clone(
    const afw_utf8_regexp_t *regexp,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_utf8_regexp_t *self;
    UErrorCode status;
    URegularExpression *rx;

    /* Allocate first, so nothing can throw while rx is not registered. */
    self = afw_pool_calloc_type(p, afw_utf8_regexp_t, xctx);

    status = U_ZERO_ERROR;
    rx = uregex_clone(regexp->rx, &status);
    if (U_FAILURE(status)) {
        if (rx) {
            uregex_close(rx);
        }
        AFW_THROW_ERROR_RV_Z(memory, icu, status,
            "uregex_clone() failed", xctx);
    }

    self->rx = rx;
    afw_pool_register_cleanup(p, self, NULL, impl_regexp_cleanup, xctx);

    return self;
}


AFW_DEFINE(afw_boolean_t)
afw_utf8_regexp_find(
    const afw_utf8_regexp_t *regexp,
    const afw_utf8_t *subject,
    afw_xctx_t *xctx)
{
    UText ut = UTEXT_INITIALIZER;
    UErrorCode status;
    URegularExpression *rx;
    UBool found;

    /* Nothing between the clone and its close can throw. */
    status = U_ZERO_ERROR;
    rx = uregex_clone(regexp->rx, &status);
    utext_openUTF8(&ut, subject->s, (int64_t)subject->len, &status);
    uregex_setUText(rx, &ut, &status);
    found = uregex_find(rx, 0, &status);
    if (rx) {
        uregex_close(rx);
    }
    utext_close(&ut);

    if (U_FAILURE(status)) {
        AFW_THROW_ERROR_RV_Z(general, icu, status,
            "Regular expression find failed", xctx);
    }

    return found;
}
