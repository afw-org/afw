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


/*
 * ICU backtracks, so a pattern such as (a+)+$ can take time exponential
 * in the length of the value. Each match may take this many steps of
 * the ICU match engine (about 0.1 second of backtracking on a current
 * machine; a linear scan of megabytes takes a small part of it) before it
 * fails with U_REGEX_TIME_OUT. RQL match patterns come from requests, so
 * this is what keeps one from holding a thread.
 */
#define IMPL_MATCH_TIME_LIMIT 1000


/*
 * A matcher for regexp on subject: a clone (the compiled one is never
 * matched itself) with the time limit, reading subject through *ut.
 * Nothing here throws; check *status. Close with impl_matcher_close().
 */
static URegularExpression *
impl_matcher_open(
    const afw_utf8_regexp_t *regexp,
    const afw_utf8_t *subject,
    UText *ut,
    UErrorCode *status)
{
    URegularExpression *rx;

    rx = uregex_clone(regexp->rx, status);
    utext_openUTF8(ut, subject->s, (int64_t)subject->len, status);
    uregex_setUText(rx, ut, status);
    uregex_setTimeLimit(rx, IMPL_MATCH_TIME_LIMIT, status);

    return rx;
}


static void
impl_matcher_close(URegularExpression *rx, UText *ut)
{
    if (rx) {
        uregex_close(rx);
    }
    utext_close(ut);
}


/* Throw for an ICU error while matching. */
static void
impl_throw_match_error(UErrorCode status, afw_xctx_t *xctx)
{
    if (status == U_REGEX_TIME_OUT || status == U_REGEX_STACK_OVERFLOW) {
        AFW_THROW_ERROR_RV_Z(payload_too_large, icu, status,
            "Regular expression match exceeded its limit: the pattern "
            "backtracks too much on this value",
            xctx);
    }
    AFW_THROW_ERROR_RV_Z(general, icu, status,
        "Regular expression match failed", xctx);
}


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

    /* Allocate first, so little can throw while rx is not registered. */
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

    /* Allocate first, so little can throw while rx is not registered. */
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


/* Byte offset of the first match of regexp in subject, or -1. */
static int64_t
impl_find(
    const afw_utf8_regexp_t *regexp,
    const afw_utf8_t *subject,
    afw_xctx_t *xctx)
{
    UText ut = UTEXT_INITIALIZER;
    UErrorCode status;
    URegularExpression *rx;
    int64_t start;

    /* Nothing between open and close can throw. */
    start = -1;
    status = U_ZERO_ERROR;
    rx = impl_matcher_open(regexp, subject, &ut, &status);
    if (uregex_find(rx, 0, &status)) {
        start = uregex_start64(rx, 0, &status);
    }
    impl_matcher_close(rx, &ut);

    if (U_FAILURE(status)) {
        impl_throw_match_error(status, xctx);
    }

    return start;
}


AFW_DEFINE(afw_boolean_t)
afw_utf8_regexp_find(
    const afw_utf8_regexp_t *regexp,
    const afw_utf8_t *subject,
    afw_xctx_t *xctx)
{
    return impl_find(regexp, subject, xctx) >= 0;
}


AFW_DEFINE(afw_integer_t)
afw_utf8_regexp_index_of(
    const afw_utf8_regexp_t *regexp,
    const afw_utf8_t *subject,
    afw_xctx_t *xctx)
{
    int64_t start;
    afw_integer_t index;
    afw_size_t i;

    start = impl_find(regexp, subject, xctx);
    if (start < 0) {
        return -1;
    }

    /* The UText's native index is a byte offset; count code points. */
    for (index = 0, i = 0; i < (afw_size_t)start; i++) {
        if ((subject->s[i] & 0xC0) != 0x80) {
            index++;
        }
    }

    return index;
}



/* Write replacement for the current match of rx in subject. */
static void
impl_write_replacement(
    URegularExpression *rx,
    const afw_utf8_t *subject,
    const afw_utf8_t *replacement,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    UErrorCode status;
    int32_t group_count;
    int32_t group;
    int64_t start;
    int64_t end;
    afw_utf8_t name;
    afw_size_t run;
    afw_size_t i;
    afw_size_t j;

    status = U_ZERO_ERROR;
    group_count = uregex_groupCount(rx, &status);

    for (run = 0, i = 0; i < replacement->len; ) {
        if (replacement->s[i] != '\\' && replacement->s[i] != '$') {
            i++;
            continue;
        }

        afw_writer_write(writer, replacement->s + run, i - run, xctx);

        /* A backslash makes the next code point literal. */
        if (replacement->s[i] == '\\') {
            i++;
            if (i >= replacement->len) {
                AFW_THROW_ERROR_Z(argument_error,
                    "The replacement ends with a backslash; write \\\\ "
                    "for a backslash",
                    xctx);
            }
            run = i;
            afw_utf8_next_code_point(replacement->s, &i, replacement->len,
                xctx);
            afw_writer_write(writer, replacement->s + run, i - run, xctx);
            run = i;
            continue;
        }

        /* $n: digits while the group exists, as ICU reads them. */
        i++;
        if (i < replacement->len &&
            replacement->s[i] >= '0' && replacement->s[i] <= '9')
        {
            group = replacement->s[i] - '0';
            for (i++; i < replacement->len &&
                replacement->s[i] >= '0' && replacement->s[i] <= '9' &&
                group * 10 + (replacement->s[i] - '0') <= group_count;
                i++)
            {
                group = group * 10 + (replacement->s[i] - '0');
            }
            if (group > group_count) {
                AFW_THROW_ERROR_FZ(argument_error, xctx,
                    "The replacement refers to group %d, but the regular "
                    "expression has %d",
                    (int)group, (int)group_count);
            }
        }

        /* ${name} */
        else if (i < replacement->len && replacement->s[i] == '{') {
            for (j = i + 1; j < replacement->len &&
                replacement->s[j] != '}'; j++);
            if (j >= replacement->len) {
                AFW_THROW_ERROR_Z(argument_error,
                    "The replacement has ${ without a closing }", xctx);
            }
            group = uregex_groupNumberFromCName(rx,
                (const char *)replacement->s + i + 1, (int32_t)(j - i - 1),
                &status);
            if (U_FAILURE(status)) {
                name.s = replacement->s + i + 1;
                name.len = j - i - 1;
                AFW_THROW_ERROR_FZ(argument_error, xctx,
                    "The replacement refers to group ${%ku}, which the "
                    "regular expression does not name",
                    &name);
            }
            i = j + 1;
        }

        else {
            AFW_THROW_ERROR_Z(argument_error,
                "In the replacement, $ must be followed by a group number "
                "or {name}; write \\$ for a $",
                xctx);
        }

        /* A group that did not take part in the match is empty. */
        start = uregex_start64(rx, group, &status);
        end = uregex_end64(rx, group, &status);
        if (U_FAILURE(status)) {
            AFW_THROW_ERROR_RV_Z(general, icu, status,
                "Regular expression group failed", xctx);
        }
        if (start >= 0) {
            afw_writer_write(writer, subject->s + start,
                (afw_size_t)(end - start), xctx);
        }
        run = i;
    }

    afw_writer_write(writer, replacement->s + run, i - run, xctx);
}


AFW_DEFINE(const afw_utf8_t *)
afw_utf8_regexp_replace(
    const afw_utf8_regexp_t *regexp,
    const afw_utf8_t *subject,
    const afw_utf8_t *replacement,
    afw_size_t limit,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    UText ut = UTEXT_INITIALIZER;
    UErrorCode status;
    URegularExpression *rx;
    const afw_writer_t *writer;
    const afw_utf8_t * volatile result;
    afw_utf8_t current;
    afw_size_t count;
    int64_t last;
    int64_t start;
    int64_t end;

    writer = afw_utf8_writer_create(NULL, p, xctx);

    status = U_ZERO_ERROR;
    rx = impl_matcher_open(regexp, subject, &ut, &status);
    if (U_FAILURE(status)) {
        impl_matcher_close(rx, &ut);
        afw_writer_release(writer, xctx);
        impl_throw_match_error(status, xctx);
    }

    result = NULL;
    AFW_TRY {
        for (last = 0, count = 0;
            count < limit && uregex_findNext(rx, &status);
            count++)
        {
            start = uregex_start64(rx, 0, &status);
            end = uregex_end64(rx, 0, &status);
            if (U_FAILURE(status)) {
                break;
            }
            afw_writer_write(writer, subject->s + last,
                (afw_size_t)(start - last), xctx);
            impl_write_replacement(rx, subject, replacement, writer, xctx);
            last = end;
        }
        if (U_FAILURE(status)) {
            impl_throw_match_error(status, xctx);
        }
        afw_writer_write(writer, subject->s + last,
            subject->len - (afw_size_t)last, xctx);

        /* The current string is a view into the writer's pool. */
        afw_utf8_writer_current_string(writer, &current, xctx);
        result = afw_utf8_clone(&current, p, xctx);
    }
    AFW_FINALLY {
        impl_matcher_close(rx, &ut);
        afw_writer_release(writer, xctx);
    }
    AFW_ENDTRY;

    return result;
}
