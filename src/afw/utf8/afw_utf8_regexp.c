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

    /* Capturing groups, not counting group 0. */
    afw_size_t group_count;

    /* Named groups in group number order. */
    const afw_utf8_regexp_group_name_t *names;
    afw_size_t names_count;
};


/*
 * Most backtrack stack one match may use: ICU's own default. A match on a
 * thread under limitRequestPoolBytes gets less when the request has less
 * left (see impl_matcher_open()).
 */
#define IMPL_MATCH_STACK_LIMIT_MAX ((afw_size_t)8000000)


/*
 * A matcher for regexp on subject: a clone (the compiled one is never
 * matched itself) reading subject through *ut, with two limits. ICU
 * backtracks, and RQL match patterns come from requests, so these are
 * what keep one match from holding a thread or its memory:
 *
 * - Steps: a pattern such as (a+)+$ can take time exponential in the
 *   length of the value without using memory. A match that takes more
 *   than env->limit_regexp_match_steps steps of the ICU match engine
 *   (limitRegexpMatchSteps; 0 = unlimited) fails with U_REGEX_TIME_OUT.
 *
 * - Backtrack stack: ICU keeps it in memory it allocates itself, outside
 *   the pools, and grows it by doubling, so its peak is about twice its
 *   limit. The limit is half of what this request may still allocate
 *   under limitRequestPoolBytes, and at most
 *   IMPL_MATCH_STACK_LIMIT_MAX. A match that needs more fails with
 *   U_REGEX_STACK_OVERFLOW.
 *
 * Nothing here throws; check *status. Close with impl_matcher_close().
 */
static URegularExpression *
impl_matcher_open(
    const afw_utf8_regexp_t *regexp,
    const afw_utf8_t *subject,
    UText *ut,
    UErrorCode *status,
    afw_xctx_t *xctx)
{
    URegularExpression *rx;
    afw_size_t limit;
    afw_size_t stack;

    limit = xctx->env->limit_regexp_match_steps;
    if (limit > INT32_MAX) {
        limit = INT32_MAX;
    }

    /* At least 1 byte: a stack limit of 0 means no limit to ICU. */
    stack = afw_xctx_internal_request_pool_bytes_remaining(xctx,
        xctx->thread) / 2;
    if (stack > IMPL_MATCH_STACK_LIMIT_MAX) {
        stack = IMPL_MATCH_STACK_LIMIT_MAX;
    }
    if (stack == 0) {
        stack = 1;
    }

    rx = uregex_clone(regexp->rx, status);
    utext_openUTF8(ut, subject->s, (int64_t)subject->len, status);
    uregex_setUText(rx, ut, status);
    uregex_setTimeLimit(rx, (int32_t)limit, status);
    uregex_setStackLimit(rx, (int32_t)stack, status);

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


/*
 * Throw for an ICU error while matching.
 *
 * The lazy-loop wording of the U_REGEX_TIME_OUT message is for an ICU
 * bug: a lazy loop over a group that can match nothing ((b|)*?) never
 * finishes, because ICU breaks such loops for (...)* and (...)+ but not
 * for (...)*? and others (ICU-2905). ICU tracks it as ICU-23047, planned
 * for ICU 79.1; still present in 78.3. Once AFW requires an ICU with the
 * fix, that wording (and the handbook note) can go; see AFW #613. The
 * step limit itself stays: exponential backtracking (^(a+)+$) needs it.
 */
static void
impl_throw_match_error(UErrorCode status, afw_xctx_t *xctx)
{
    if (status == U_REGEX_TIME_OUT) {
        AFW_THROW_ERROR_RV_Z(payload_too_large, icu, status,
            "Regular expression match exceeded limitRegexpMatchSteps: the "
            "pattern backtracks too much on this value, or has a lazy loop "
            "over a group that can match nothing, such as (b|)*? (write "
            "b*?)",
            xctx);
    }
    if (status == U_REGEX_STACK_OVERFLOW) {
        AFW_THROW_ERROR_RV_Z(payload_too_large, icu, status,
            "Regular expression match exceeded its backtrack memory (at "
            "most 8 MB, and at most half of what is left for this request "
            "under limitRequestPoolBytes): the pattern backtracks too "
            "deeply on this value",
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


/*
 * Set the group count and names of a newly compiled regexp.
 *
 * ICU has no call that lists a pattern's group names, so the pattern
 * is scanned for every (?<name> and ICU is asked whether name is a
 * group. Every named group is written (?<name>, so none is missed; text
 * that only looks like one (in a class, a comment, \Q...\E) either
 * names no group, and is skipped, or names a real group, which is then
 * found twice and kept once.
 */
static void
impl_set_groups(
    afw_utf8_regexp_t *self,
    const afw_utf8_t *pattern,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_utf8_regexp_group_name_t *names;
    afw_utf8_regexp_group_name_t entry;
    UErrorCode status;
    int32_t number;
    afw_size_t count;
    afw_size_t i;
    afw_size_t j;
    afw_size_t k;

    status = U_ZERO_ERROR;
    number = uregex_groupCount(self->rx, &status);
    self->group_count = (U_SUCCESS(status) && number > 0) ? number : 0;
    if (self->group_count == 0) {
        return;
    }

    /* At most one name per group. */
    names = NULL;
    count = 0;
    for (i = 0; i + 3 < pattern->len; i++) {
        if (pattern->s[i] != '(' || pattern->s[i + 1] != '?' ||
            pattern->s[i + 2] != '<')
        {
            continue;
        }
        for (j = i + 3; j < pattern->len &&
            ((pattern->s[j] >= 'a' && pattern->s[j] <= 'z') ||
            (pattern->s[j] >= 'A' && pattern->s[j] <= 'Z') ||
            (pattern->s[j] >= '0' && pattern->s[j] <= '9'));
            j++);
        if (j == i + 3 || j >= pattern->len || pattern->s[j] != '>') {
            continue;
        }
        status = U_ZERO_ERROR;
        number = uregex_groupNumberFromCName(self->rx,
            (const char *)pattern->s + i + 3, (int32_t)(j - i - 3),
            &status);
        if (U_FAILURE(status) || number <= 0) {
            continue;
        }
        for (k = 0; k < count && names[k].number != (afw_size_t)number;
            k++);
        if (k < count) {
            continue;
        }
        if (!names) {
            names = afw_pool_calloc(p,
                sizeof(afw_utf8_regexp_group_name_t) * self->group_count,
                xctx);
        }
        names[count].name.s = pattern->s + i + 3;
        names[count].name.len = j - i - 3;
        names[count].number = (afw_size_t)number;
        count++;
    }

    /* In group number order (the order of the groups in the pattern). */
    for (i = 1; i < count; i++) {
        afw_memory_copy(&entry, &names[i]);
        for (j = i; j > 0 && names[j - 1].number > entry.number; j--) {
            afw_memory_copy(&names[j], &names[j - 1]);
        }
        afw_memory_copy(&names[j], &entry);
    }

    /* The names point into pattern, which may not live as long as p. */
    for (i = 0; i < count; i++) {
        afw_memory_copy(&names[i].name,
            afw_utf8_clone(&names[i].name, p, xctx));
    }

    self->names = names;
    self->names_count = count;
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
    impl_set_groups(self, pattern, p, xctx);

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

    /* The names are copied, since regexp may not live as long as p. */
    self->group_count = regexp->group_count;
    self->names_count = regexp->names_count;
    if (regexp->names_count > 0) {
        afw_utf8_regexp_group_name_t *names;
        afw_size_t i;

        names = afw_pool_calloc(p,
            sizeof(afw_utf8_regexp_group_name_t) * regexp->names_count,
            xctx);
        for (i = 0; i < regexp->names_count; i++) {
            names[i].number = regexp->names[i].number;
            afw_memory_copy(&names[i].name,
                afw_utf8_clone(&regexp->names[i].name, p, xctx));
        }
        self->names = names;
    }

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
    rx = impl_matcher_open(regexp, subject, &ut, &status, xctx);
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
    rx = impl_matcher_open(regexp, subject, &ut, &status, xctx);
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



AFW_DEFINE(afw_size_t)
afw_utf8_regexp_groups(
    const afw_utf8_regexp_t *regexp,
    const afw_utf8_regexp_group_name_t **names,
    afw_size_t *names_count)
{
    if (names) {
        *names = regexp->names;
    }
    if (names_count) {
        *names_count = regexp->names_count;
    }
    return regexp->group_count;
}



AFW_DEFINE(afw_size_t)
afw_utf8_regexp_for_each_match(
    const afw_utf8_regexp_t *regexp,
    const afw_utf8_t *subject,
    afw_size_t limit,
    afw_utf8_regexp_match_cb_t callback,
    void *data,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    UText ut = UTEXT_INITIALIZER;
    UErrorCode status;
    URegularExpression *rx;
    afw_utf8_regexp_match_t match;
    afw_integer_t *starts;
    afw_integer_t *ends;
    afw_size_t volatile count;
    afw_size_t counted;
    afw_size_t group;

    if (limit == 0) {
        return 0;
    }

    /* Allocate first, so nothing here throws while rx is open. */
    starts = afw_pool_malloc(p,
        sizeof(afw_integer_t) * (regexp->group_count + 1), xctx);
    ends = afw_pool_malloc(p,
        sizeof(afw_integer_t) * (regexp->group_count + 1), xctx);
    match.subject = subject;
    match.group_count = regexp->group_count;
    match.starts = starts;
    match.ends = ends;
    match.index = 0;

    status = U_ZERO_ERROR;
    rx = impl_matcher_open(regexp, subject, &ut, &status, xctx);
    if (U_FAILURE(status)) {
        impl_matcher_close(rx, &ut);
        impl_throw_match_error(status, xctx);
    }

    count = 0;
    AFW_TRY {
        /* Code points are counted from where the last match started. */
        counted = 0;
        while (count < limit && uregex_findNext(rx, &status)) {
            for (group = 0; group <= regexp->group_count; group++) {
                starts[group] = uregex_start64(rx, (int32_t)group, &status);
                ends[group] = uregex_end64(rx, (int32_t)group, &status);
            }
            if (U_FAILURE(status)) {
                break;
            }
            for (; counted < (afw_size_t)starts[0]; counted++) {
                if ((subject->s[counted] & 0xC0) != 0x80) {
                    match.index++;
                }
            }
            count++;
            if (!callback(&match, data, xctx)) {
                break;
            }
        }
        if (U_FAILURE(status)) {
            impl_throw_match_error(status, xctx);
        }
    }
    AFW_FINALLY {
        impl_matcher_close(rx, &ut);
    }
    AFW_ENDTRY;

    return count;
}



/* Pattern_White_Space above ASCII: U+0085, U+200E, U+200F, U+2028, U+2029. */
#define IMPL_IS_PATTERN_WHITE_SPACE_ABOVE_ASCII(cp) \
    ((cp) == 0x85 || (cp) == 0x200E || (cp) == 0x200F || \
    (cp) == 0x2028 || (cp) == 0x2029)


AFW_DEFINE(const afw_utf8_t *)
afw_utf8_regexp_escape(
    const afw_utf8_t *s,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_utf8_t *escaped;
    afw_utf8_octet_t *result;
    afw_utf8_octet_t *c;
    afw_size_t i;
    afw_size_t start;
    afw_code_point_t cp;

    /* At most \x{hhhhh} (10 bytes) for each code point of 1 or more bytes. */
    result = afw_pool_malloc(p, s->len * 10 + 1, xctx);
    c = result;

    for (i = 0; i < s->len; ) {
        start = i;
        cp = afw_utf8_next_code_point(s->s, &i, s->len, xctx);
        if (cp < 0) {
            AFW_THROW_ERROR_Z(argument_error,
                "Invalid UTF-8 in regexp_escape", xctx);
        }

        /* Kept as is. */
        if ((cp >= 'a' && cp <= 'z') || (cp >= 'A' && cp <= 'Z') ||
            cp == '_' ||
            (cp >= '0' && cp <= '9' && start > 0) ||
            (cp > 0x7F && !IMPL_IS_PATTERN_WHITE_SPACE_ABOVE_ASCII(cp)))
        {
            memcpy(c, s->s + start, i - start);
            c += i - start;
        }

        /*
         * Hex: controls and white space, which mean nothing with flag x,
         * and a leading digit, which could continue a back reference.
         */
        else if (cp <= ' ' || cp == 0x7F || cp > 0x7F ||
            (cp >= '0' && cp <= '9'))
        {
            c += sprintf((char *)c, "\\x{%x}", (unsigned int)cp);
        }

        /* Any other ASCII character with a backslash is literal. */
        else {
            *c++ = '\\';
            *c++ = (afw_utf8_octet_t)cp;
        }
    }

    escaped = afw_pool_calloc_type(p, afw_utf8_t, xctx);
    escaped->s = result;
    escaped->len = c - result;

    return escaped;
}
