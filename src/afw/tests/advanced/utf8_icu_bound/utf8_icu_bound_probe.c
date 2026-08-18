// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework UTF-8 to_lower / compare bound probe
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#include "afw.h"

#include <stdio.h>
#include <string.h>

/**
 * @file utf8_icu_bound_probe.c
 * @brief C probe for bounded ICU next/append in to_lower / compare.
 *
 * Script constructors validate UTF-8. This boots a core environment and
 * calls the C APIs with .s/.len set by hand, including a truncated
 * multi-byte sequence.
 *
 * Same shape as tests/advanced/pool_alloc/pool_alloc_probe.c.
 */

static void
impl_utf8_set(afw_utf8_t *s, const char *z, afw_size_t len)
{
    s->s = (const afw_utf8_octet_t *)z;
    s->len = len;
}

static int
impl_expect_throw(
    const afw_utf8_t *s1,
    const afw_utf8_t *s2,
    afw_boolean_t do_lower,
    const afw_pool_t *p,
    afw_xctx_t *xctx,
    const char *label)
{
    int threw;
    int unexpected;

    threw = 0;
    unexpected = 0;
    AFW_TRY {
        if (do_lower) {
            (void)afw_utf8_to_lower(s1, p, xctx);
        }
        else {
            (void)afw_utf8_compare_ignore_case(s1, s2, xctx);
        }
    }
    AFW_CATCH_UNHANDLED {
        if (AFW_ERROR_THROWN->message_z &&
            strstr(AFW_ERROR_THROWN->message_z, "Not valid UTF-8"))
        {
            threw = 1;
        }
        else {
            unexpected = 1;
            fprintf(stderr, "%s: threw %s\n", label,
                AFW_ERROR_THROWN->message_z
                ? AFW_ERROR_THROWN->message_z : "?");
        }
    }
    AFW_ENDTRY;

    if (unexpected) {
        return 1;
    }
    if (!threw) {
        fprintf(stderr, "%s: did not throw\n", label);
        return 1;
    }
    return 0;
}

static int
impl_to_lower_valid(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_t in;
    const afw_utf8_t *out;
    const afw_utf8_t already = AFW_UTF8_LITERAL("already");
    const afw_utf8_t empty = AFW_UTF8_LITERAL("");

    impl_utf8_set(&in, "ABC", 3);
    out = afw_utf8_to_lower(&in, p, xctx);
    if (!out || out->len != 3 || memcmp(out->s, "abc", 3) != 0) {
        fprintf(stderr, "to_lower valid: ABC mismatch\n");
        return 1;
    }

    out = afw_utf8_to_lower(&already, p, xctx);
    if (out != &already) {
        fprintf(stderr, "to_lower valid: already-lower not reused\n");
        return 1;
    }

    out = afw_utf8_to_lower(&empty, p, xctx);
    if (out != &empty) {
        fprintf(stderr, "to_lower valid: empty not reused\n");
        return 1;
    }

    /* U+00C9 LATIN CAPITAL E WITH ACUTE -> U+00E9 */
    impl_utf8_set(&in, "\xC3\x89", 2);
    out = afw_utf8_to_lower(&in, p, xctx);
    if (!out || out->len != 2 ||
        memcmp(out->s, "\xC3\xA9", 2) != 0)
    {
        fprintf(stderr, "to_lower valid: E-acute mismatch\n");
        return 1;
    }

    return 0;
}

static int
impl_to_lower_truncated(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_t in;
    int rc;

    rc = 0;
    /* 2-byte lead, no trail. */
    impl_utf8_set(&in, "\xC3", 1);
    rc |= impl_expect_throw(&in, NULL, true, p, xctx, "to_lower 2-byte");
    /* 3-byte lead + one trail, missing last. */
    impl_utf8_set(&in, "\xE2\x82", 2);
    rc |= impl_expect_throw(&in, NULL, true, p, xctx, "to_lower 3-byte");
    return rc;
}

static int
impl_compare_valid(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_t a;
    afw_utf8_t b;
    int cmp;

    (void)p;
    impl_utf8_set(&a, "AbC", 3);
    impl_utf8_set(&b, "aBc", 3);
    cmp = afw_utf8_compare_ignore_case(&a, &b, xctx);
    if (cmp != 0) {
        fprintf(stderr, "compare valid: AbC vs aBc -> %d\n", cmp);
        return 1;
    }

    impl_utf8_set(&a, "abc", 3);
    impl_utf8_set(&b, "abd", 3);
    cmp = afw_utf8_compare_ignore_case(&a, &b, xctx);
    if (cmp >= 0) {
        fprintf(stderr, "compare valid: abc vs abd -> %d\n", cmp);
        return 1;
    }

    impl_utf8_set(&a, "ab", 2);
    impl_utf8_set(&b, "abc", 3);
    cmp = afw_utf8_compare_ignore_case(&a, &b, xctx);
    if (cmp >= 0) {
        fprintf(stderr, "compare valid: ab vs abc -> %d\n", cmp);
        return 1;
    }

    return 0;
}

static int
impl_compare_truncated(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_t a;
    afw_utf8_t b;
    int rc;

    (void)p;
    rc = 0;
    impl_utf8_set(&a, "\xC3", 1);
    impl_utf8_set(&b, "a", 1);
    rc |= impl_expect_throw(&a, &b, false, p, xctx, "compare s1");
    impl_utf8_set(&a, "a", 1);
    impl_utf8_set(&b, "\xC3", 1);
    rc |= impl_expect_throw(&a, &b, false, p, xctx, "compare s2");
    return rc;
}

int
main(int argc, char **argv)
{
    const afw_error_t *create_error;
    afw_xctx_t *xctx;
    const afw_pool_t *p;
    const char *case_name;
    int rc;

    xctx = afw_environment_create(afw_version(), argc,
        (const char * const *)argv, &create_error);
    if (!xctx) {
        fprintf(stderr, "environment create failed\n");
        return 2;
    }

    case_name = (argc > 1) ? argv[1] : "";
    p = afw_pool_create(xctx->p, xctx);
    rc = 0;

    if (strcmp(case_name, "to_lower-valid") == 0) {
        rc = impl_to_lower_valid(p, xctx);
    }
    else if (strcmp(case_name, "to_lower-truncated") == 0) {
        rc = impl_to_lower_truncated(p, xctx);
    }
    else if (strcmp(case_name, "compare-valid") == 0) {
        rc = impl_compare_valid(p, xctx);
    }
    else if (strcmp(case_name, "compare-truncated") == 0) {
        rc = impl_compare_truncated(p, xctx);
    }
    else {
        fprintf(stderr, "usage: utf8_icu_bound_probe "
            "to_lower-valid|to_lower-truncated|"
            "compare-valid|compare-truncated\n");
        rc = 2;
    }

    afw_pool_release(p, xctx);
    afw_environment_release(xctx);
    return rc;
}
