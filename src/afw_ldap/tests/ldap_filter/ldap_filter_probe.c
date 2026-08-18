// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework LDAP filter helper probe
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#include "afw.h"

#include <stdio.h>
#include <string.h>

/**
 * @file ldap_filter_probe.c
 * @brief C probe for LDAP filter escape, attr check, and depth.
 *
 * Retrieve needs a directory and schema metadata. These helpers are
 * the splice/depth gates and can be called without a session.
 *
 * Same shape as tests/advanced/pool_alloc/pool_alloc_probe.c.
 */

const afw_utf8_t *
afw_ldap_internal_filter_escape(
    const afw_utf8_t *s,
    const afw_pool_t *p,
    afw_xctx_t *xctx);

void
afw_ldap_internal_filter_require_safe_attr(
    const afw_utf8_t *attr,
    afw_xctx_t *xctx);

void
afw_ldap_internal_filter_require_depth(
    const afw_query_criteria_filter_entry_t *entry,
    afw_xctx_t *xctx);

#define IMPL_NESTING_MAX 256

static void
impl_utf8_set(afw_utf8_t *s, const char *z, afw_size_t len)
{
    s->s = (const afw_utf8_octet_t *)z;
    s->len = len;
}

static int
impl_expect_escape(
    const char *in_z,
    afw_size_t in_len,
    const char *expect_z,
    afw_size_t expect_len,
    const afw_pool_t *p,
    afw_xctx_t *xctx,
    const char *label)
{
    afw_utf8_t in;
    const afw_utf8_t *out;

    impl_utf8_set(&in, in_z, in_len);
    out = afw_ldap_internal_filter_escape(&in, p, xctx);
    if (!out || out->len != expect_len ||
        memcmp(out->s, expect_z, expect_len) != 0)
    {
        fprintf(stderr, "%s: escape mismatch\n", label);
        return 1;
    }
    return 0;
}

static int
impl_expect_attr_ok(
    const char *z,
    const afw_pool_t *p,
    afw_xctx_t *xctx,
    const char *label)
{
    afw_utf8_t attr;
    int threw;

    (void)p;
    impl_utf8_set(&attr, z, strlen(z));
    threw = 0;
    AFW_TRY {
        afw_ldap_internal_filter_require_safe_attr(&attr, xctx);
    }
    AFW_CATCH_UNHANDLED {
        threw = 1;
        fprintf(stderr, "%s: unexpected throw %s\n", label,
            AFW_ERROR_THROWN->message_z
            ? AFW_ERROR_THROWN->message_z : "?");
    }
    AFW_ENDTRY;
    return threw;
}

static int
impl_expect_attr_bad(
    const char *z,
    afw_xctx_t *xctx,
    const char *label)
{
    afw_utf8_t attr;
    int threw;
    int unexpected;

    impl_utf8_set(&attr, z, z ? strlen(z) : 0);
    threw = 0;
    unexpected = 0;
    AFW_TRY {
        afw_ldap_internal_filter_require_safe_attr(&attr, xctx);
    }
    AFW_CATCH_UNHANDLED {
        if (AFW_ERROR_THROWN->code ==
            afw_error_code_query_too_complex)
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

static const afw_query_criteria_filter_entry_t *
impl_chain(afw_size_t n, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_query_criteria_filter_entry_t *nodes;
    afw_size_t i;

    if (n == 0) {
        return AFW_QUERY_CRITERIA_TRUE;
    }
    nodes = afw_pool_calloc(p,
        sizeof(afw_query_criteria_filter_entry_t) * n, xctx);
    for (i = 0; i < n; i++) {
        nodes[i].on_true = (i + 1 < n)
            ? &nodes[i + 1]
            : AFW_QUERY_CRITERIA_TRUE;
        nodes[i].on_false = AFW_QUERY_CRITERIA_FALSE;
    }
    return nodes;
}

static int
impl_escape_plain(const afw_pool_t *p, afw_xctx_t *xctx)
{
    int rc;

    rc = 0;
    rc |= impl_expect_escape("cn", 2, "cn", 2, p, xctx, "plain");
    rc |= impl_expect_escape("", 0, "", 0, p, xctx, "empty");
    rc |= impl_expect_escape("inetOrgPerson", 13,
        "inetOrgPerson", 13, p, xctx, "oc");
    /* UTF-8 e-acute must pass through. */
    rc |= impl_expect_escape("caf\xC3\xA9", 5,
        "caf\xC3\xA9", 5, p, xctx, "utf8");
    return rc;
}

static int
impl_escape_specials(const afw_pool_t *p, afw_xctx_t *xctx)
{
    int rc;
    char nul_in[3];
    char nul_out[5];

    rc = 0;
    rc |= impl_expect_escape("*", 1, "\\2A", 3, p, xctx, "star");
    rc |= impl_expect_escape("(", 1, "\\28", 3, p, xctx, "lparen");
    rc |= impl_expect_escape(")", 1, "\\29", 3, p, xctx, "rparen");
    rc |= impl_expect_escape("\\", 1, "\\5C", 3, p, xctx, "backslash");

    nul_in[0] = 'a';
    nul_in[1] = 0;
    nul_in[2] = 'b';
    nul_out[0] = 'a';
    nul_out[1] = '\\';
    nul_out[2] = '0';
    nul_out[3] = '0';
    nul_out[4] = 'b';
    rc |= impl_expect_escape(nul_in, 3, nul_out, 5, p, xctx, "nul");

    rc |= impl_expect_escape(
        "x)(objectClass=*", 16,
        "x\\29\\28objectClass=\\2A", 22,
        p, xctx, "mixed");
    return rc;
}

static int
impl_attr_ok(const afw_pool_t *p, afw_xctx_t *xctx)
{
    int rc;

    rc = 0;
    rc |= impl_expect_attr_ok("cn", p, xctx, "cn");
    rc |= impl_expect_attr_ok("objectClass", p, xctx, "objectClass");
    rc |= impl_expect_attr_ok("givenName", p, xctx, "givenName");
    rc |= impl_expect_attr_ok("2.5.4.3", p, xctx, "oid");
    rc |= impl_expect_attr_ok("cn;lang-en", p, xctx, "option");
    rc |= impl_expect_attr_ok("0", p, xctx, "single-digit-oid");
    return rc;
}

static int
impl_attr_bad(const afw_pool_t *p, afw_xctx_t *xctx)
{
    int rc;

    (void)p;
    rc = 0;
    rc |= impl_expect_attr_bad("cn)(x", xctx, "inject-name");
    rc |= impl_expect_attr_bad("cn*", xctx, "star-name");
    rc |= impl_expect_attr_bad("(cn", xctx, "lparen-name");
    rc |= impl_expect_attr_bad("1cn", xctx, "digit-then-alpha");
    rc |= impl_expect_attr_bad("1.", xctx, "trailing-dot");
    rc |= impl_expect_attr_bad(";lang-en", xctx, "option-only");
    rc |= impl_expect_attr_bad("", xctx, "empty");
    {
        int threw;
        int unexpected;

        threw = 0;
        unexpected = 0;
        AFW_TRY {
            afw_ldap_internal_filter_require_safe_attr(NULL, xctx);
        }
        AFW_CATCH_UNHANDLED {
            if (AFW_ERROR_THROWN->code ==
                afw_error_code_query_too_complex)
            {
                threw = 1;
            }
            else {
                unexpected = 1;
            }
        }
        AFW_ENDTRY;
        if (unexpected || !threw) {
            fprintf(stderr, "null-attr: %s\n",
                threw ? "wrong code" : "did not throw");
            rc |= 1;
        }
    }
    return rc;
}

static int
impl_depth_ok(const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_query_criteria_filter_entry_t *chain;
    int threw;

    chain = impl_chain(IMPL_NESTING_MAX, p, xctx);
    threw = 0;
    AFW_TRY {
        afw_ldap_internal_filter_require_depth(chain, xctx);
    }
    AFW_CATCH_UNHANDLED {
        threw = 1;
        fprintf(stderr, "depth-ok: unexpected throw %s\n",
            AFW_ERROR_THROWN->message_z
            ? AFW_ERROR_THROWN->message_z : "?");
    }
    AFW_ENDTRY;
    return threw;
}

static int
impl_depth_too_deep(const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_query_criteria_filter_entry_t *chain;
    int threw;
    int unexpected;

    chain = impl_chain(IMPL_NESTING_MAX + 1, p, xctx);
    threw = 0;
    unexpected = 0;
    AFW_TRY {
        afw_ldap_internal_filter_require_depth(chain, xctx);
    }
    AFW_CATCH_UNHANDLED {
        if (AFW_ERROR_THROWN->code ==
                afw_error_code_query_too_complex &&
            AFW_ERROR_THROWN->message_z &&
            strstr(AFW_ERROR_THROWN->message_z, "too deep"))
        {
            threw = 1;
        }
        else {
            unexpected = 1;
            fprintf(stderr, "depth-too-deep: threw %s\n",
                AFW_ERROR_THROWN->message_z
                ? AFW_ERROR_THROWN->message_z : "?");
        }
    }
    AFW_ENDTRY;
    if (unexpected) {
        return 1;
    }
    if (!threw) {
        fprintf(stderr, "depth-too-deep: did not throw\n");
        return 1;
    }
    return 0;
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

    if (strcmp(case_name, "escape-plain") == 0) {
        rc = impl_escape_plain(p, xctx);
    }
    else if (strcmp(case_name, "escape-specials") == 0) {
        rc = impl_escape_specials(p, xctx);
    }
    else if (strcmp(case_name, "attr-ok") == 0) {
        rc = impl_attr_ok(p, xctx);
    }
    else if (strcmp(case_name, "attr-bad") == 0) {
        rc = impl_attr_bad(p, xctx);
    }
    else if (strcmp(case_name, "depth-ok") == 0) {
        rc = impl_depth_ok(p, xctx);
    }
    else if (strcmp(case_name, "depth-too-deep") == 0) {
        rc = impl_depth_too_deep(p, xctx);
    }
    else {
        fprintf(stderr, "usage: ldap_filter_probe "
            "escape-plain|escape-specials|attr-ok|attr-bad|"
            "depth-ok|depth-too-deep\n");
        rc = 2;
    }

    afw_pool_release(p, xctx);
    afw_environment_release(xctx);
    return rc;
}
