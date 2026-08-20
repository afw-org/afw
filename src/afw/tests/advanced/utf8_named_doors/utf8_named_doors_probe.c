// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework utf8 named-door probe
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#include "afw.h"

#include <stdio.h>
#include <string.h>

/**
 * @file utf8_named_doors_probe.c
 * @brief C probe for create/set/no_copy/forced_safe/property_name.
 */

static int
impl_eq(const afw_utf8_t *s, const char *z, afw_size_t n, const char *label)
{
    if (!s || s->len != n || memcmp(s->s, z, n) != 0) {
        fprintf(stderr, "%s: got len=%lu\n", label,
            (unsigned long)(s ? s->len : 0));
        return 1;
    }
    return 0;
}

static int
impl_threw_nfc(void (*fn)(const afw_pool_t *, afw_xctx_t *),
    const afw_pool_t *p, afw_xctx_t *xctx, const char *label)
{
    int threw;

    threw = 0;
    AFW_TRY {
        fn(p, xctx);
    }
    AFW_CATCH_UNHANDLED {
        threw = 1;
    }
    AFW_ENDTRY;
    if (!threw) {
        fprintf(stderr, "%s: did not throw\n", label);
        return 1;
    }
    return 0;
}

static const char *impl_trunc = "\xC3";

static void
impl_create_trunc(const afw_pool_t *p, afw_xctx_t *xctx)
{
    (void)afw_utf8_create((const afw_utf8_octet_t *)impl_trunc, 1, p, xctx);
}

static void
impl_set_no_copy_trunc(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_t to;

    (void)p;
    afw_utf8_set_no_copy(&to, (const afw_utf8_octet_t *)impl_trunc, 1, xctx);
}

static void
impl_create_no_copy_trunc(const afw_pool_t *p, afw_xctx_t *xctx)
{
    (void)afw_utf8_create_no_copy(
        (const afw_utf8_octet_t *)impl_trunc, 1, p, xctx);
}

static int
impl_create_set_copy(const afw_pool_t *p, afw_xctx_t *xctx)
{
    const char *lit = "hello";
    const afw_utf8_t *c;
    afw_utf8_t to;

    c = afw_utf8_create((const afw_utf8_octet_t *)lit, 5, p, xctx);
    if (impl_eq(c, "hello", 5, "create")) {
        return 1;
    }
    if (c->s == (const afw_utf8_octet_t *)lit) {
        fprintf(stderr, "create: pointed at input (expected copy)\n");
        return 1;
    }

    afw_utf8_set(&to, (const afw_utf8_octet_t *)lit, 5, p, xctx);
    if (impl_eq(&to, "hello", 5, "set")) {
        return 1;
    }
    if (to.s == (const afw_utf8_octet_t *)lit) {
        fprintf(stderr, "set: pointed at input (expected copy)\n");
        return 1;
    }

    c = afw_utf8_z_to_utf8("abc", p, xctx);
    if (impl_eq(c, "abc", 3, "z_to_utf8")) {
        return 1;
    }

    if (impl_threw_nfc(impl_create_trunc, p, xctx, "create trunc")) {
        return 1;
    }
    return 0;
}

static int
impl_no_copy(const afw_pool_t *p, afw_xctx_t *xctx)
{
    const char *lit = "hello";
    const afw_utf8_t *c;
    afw_utf8_t to;

    c = afw_utf8_create_no_copy((const afw_utf8_octet_t *)lit, 5, p, xctx);
    if (impl_eq(c, "hello", 5, "create_no_copy")) {
        return 1;
    }
    if (c->s != (const afw_utf8_octet_t *)lit) {
        fprintf(stderr, "create_no_copy: did not point at input\n");
        return 1;
    }

    afw_utf8_set_no_copy(&to, (const afw_utf8_octet_t *)lit, 5, xctx);
    if (to.s != (const afw_utf8_octet_t *)lit || to.len != 5) {
        fprintf(stderr, "set_no_copy: did not point at input\n");
        return 1;
    }

    if (impl_threw_nfc(impl_set_no_copy_trunc, p, xctx, "set_no_copy trunc")) {
        return 1;
    }
    if (impl_threw_nfc(impl_create_no_copy_trunc, p, xctx,
        "create_no_copy trunc"))
    {
        return 1;
    }
    return 0;
}

static int
impl_forced_safe(const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t *c;
    char in[8];
    afw_utf8_t to;

    c = afw_utf8_create_forced_safe(
        (const afw_utf8_octet_t *)"hello", 5, p, xctx);
    if (impl_eq(c, "hello", 5, "forced_safe hello")) {
        return 1;
    }

    /* FOO + caret + BAR → FOO^^BAR */
    c = afw_utf8_create_forced_safe(
        (const afw_utf8_octet_t *)"FOO^BAR", 7, p, xctx);
    if (impl_eq(c, "FOO^^BAR", 8, "forced_safe caret")) {
        return 1;
    }

    /* FOO + 0xFF → FOO^FF^ */
    in[0] = 'F'; in[1] = 'O'; in[2] = 'O'; in[3] = (char)0xff;
    c = afw_utf8_create_forced_safe((const afw_utf8_octet_t *)in, 4, p, xctx);
    if (impl_eq(c, "FOO^FF^", 7, "forced_safe 0xff")) {
        return 1;
    }

    /* NUL is Cc → ^00^ */
    in[0] = 'a'; in[1] = 0; in[2] = 'b';
    c = afw_utf8_create_forced_safe((const afw_utf8_octet_t *)in, 3, p, xctx);
    if (impl_eq(c, "a^00^b", 6, "forced_safe nul")) {
        return 1;
    }

    /* newline stays */
    in[0] = 'a'; in[1] = '\n'; in[2] = 'b';
    c = afw_utf8_create_forced_safe((const afw_utf8_octet_t *)in, 3, p, xctx);
    if (impl_eq(c, "a\nb", 3, "forced_safe lf")) {
        return 1;
    }

    /* run of two invalid bytes in one pair */
    in[0] = (char)0xc0; in[1] = (char)0x80;
    c = afw_utf8_create_forced_safe((const afw_utf8_octet_t *)in, 2, p, xctx);
    if (impl_eq(c, "^C080^", 6, "forced_safe run")) {
        return 1;
    }

    afw_utf8_set_forced_safe(&to, (const afw_utf8_octet_t *)in, 2, p, xctx);
    if (impl_eq(&to, "^C080^", 6, "set_forced_safe")) {
        return 1;
    }
    return 0;
}

static int
impl_property_name(const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t *c;
    char in[8];

    c = afw_utf8_create_property_name(
        (const afw_utf8_octet_t *)"PATH", 4, p, xctx);
    if (impl_eq(c, "PATH", 4, "property PATH")) {
        return 1;
    }

    in[0] = 'B'; in[1] = 'A'; in[2] = 'D';
    in[3] = (char)0xff; in[4] = 'N';
    c = afw_utf8_create_property_name(
        (const afw_utf8_octet_t *)in, 5, p, xctx);
    if (impl_eq(c, "BAD^FF^N", 8, "property bad name")) {
        return 1;
    }

    c = afw_utf8_create_property_name(
        (const afw_utf8_octet_t *)"FOO^BAR", 7, p, xctx);
    if (impl_eq(c, "FOO^^BAR", 8, "property caret")) {
        return 1;
    }
    return 0;
}

static int
impl_printf_safe(const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t *c;
    char bad[4];

    bad[0] = 'x';
    bad[1] = (char)0xff;
    bad[2] = 0;
    c = afw_utf8_printf(p, xctx, "n=%s", bad);
    if (impl_eq(c, "n=x^FF^", 7, "printf forced_safe")) {
        return 1;
    }
    return 0;
}

static int
impl_printf_nul(const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t *c;
    afw_utf8_t s;
    char in[3];
    const afw_utf8_z_t *z;

    in[0] = 'a';
    in[1] = 0;
    in[2] = 'b';
    s.s = (const afw_utf8_octet_t *)in;
    s.len = 3;
    c = afw_utf8_printf(p, xctx, "x=" AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(&s));
    if (impl_eq(c, "x=a^00^b", 8, "printf nul")) {
        return 1;
    }

    c = afw_utf8_printf(p, xctx, "n=%d x=" AFW_UTF8_FMT, 3,
        AFW_UTF8_FMT_ARG(&s));
    if (impl_eq(c, "n=3 x=a^00^b", 12, "printf mixed")) {
        return 1;
    }

    z = afw_utf8_z_printf(p, xctx, "x=" AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(&s));
    if (!z || strcmp((const char *)z, "x=a^00^b") != 0) {
        fprintf(stderr, "z_printf nul: got %s\n", z ? (const char *)z : "?");
        return 1;
    }
    return 0;
}

static int
impl_error_backtrace(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_t fake;
    char in[3];
    const afw_object_t *obj;
    const afw_utf8_t *got;
    int rc;

    in[0] = 'x';
    in[1] = (char)0xff;
    in[2] = 'y';
    fake.s = (const afw_utf8_octet_t *)in;
    fake.len = 3;

    afw_flag_set(afw_s_a_flag_response_error_backtrace, true, xctx);

    rc = 0;
    AFW_TRY {
        AFW_THROW_ERROR_Z(general, "probe", xctx);
    }
    AFW_CATCH_UNHANDLED {
        xctx->error->backtrace = &fake;
        obj = afw_error_to_object(xctx->error, p, xctx);
        got = afw_object_get_property_as_string(
            obj, afw_v_backtrace, p, xctx);
        if (!got || got->len != 6 ||
            memcmp(got->s, "x^FF^y", 6) != 0)
        {
            fprintf(stderr, "error backtrace: got len=%lu\n",
                (unsigned long)(got ? got->len : 0));
            rc = 1;
        }
    }
    AFW_ENDTRY;
    return rc;
}

static int
impl_icu_error_name(const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_z_t *z;

    (void)p;
    (void)xctx;
    z = afw_utf8_icu_error_name_z(0);
    if (!z || strcmp((const char *)z, "U_ZERO_ERROR") != 0) {
        fprintf(stderr, "icu error name: got %s\n",
            z ? (const char *)z : "?");
        return 1;
    }
    return 0;
}

static int
impl_from_memory(const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t hello = AFW_UTF8_LITERAL("hello");
    const afw_memory_t *mem;
    const afw_utf8_t *back;

    mem = afw_utf8_as_memory(&hello, p, xctx);
    if (!mem || mem->size != 5 || memcmp(mem->ptr, "hello", 5) != 0) {
        fprintf(stderr, "as_memory failed\n");
        return 1;
    }
    back = afw_utf8_from_memory(mem, p, xctx);
    if (impl_eq(back, "hello", 5, "from_memory")) {
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

    if (strcmp(case_name, "create-set-copy") == 0) {
        rc = impl_create_set_copy(p, xctx);
    }
    else if (strcmp(case_name, "no-copy") == 0) {
        rc = impl_no_copy(p, xctx);
    }
    else if (strcmp(case_name, "forced-safe") == 0) {
        rc = impl_forced_safe(p, xctx);
    }
    else if (strcmp(case_name, "property-name") == 0) {
        rc = impl_property_name(p, xctx);
    }
    else if (strcmp(case_name, "printf-safe") == 0) {
        rc = impl_printf_safe(p, xctx);
    }
    else if (strcmp(case_name, "printf-nul") == 0) {
        rc = impl_printf_nul(p, xctx);
    }
    else if (strcmp(case_name, "error-backtrace") == 0) {
        rc = impl_error_backtrace(p, xctx);
    }
    else if (strcmp(case_name, "icu-error-name") == 0) {
        rc = impl_icu_error_name(p, xctx);
    }
    else if (strcmp(case_name, "from-memory") == 0) {
        rc = impl_from_memory(p, xctx);
    }
    else {
        fprintf(stderr, "usage: utf8_named_doors_probe "
            "create-set-copy|no-copy|forced-safe|property-name|"
            "printf-safe|printf-nul|error-backtrace|icu-error-name|"
            "from-memory\n");
        rc = 2;
    }

    afw_pool_release(p, xctx);
    afw_environment_release(xctx);
    return rc;
}
