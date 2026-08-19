// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework utf8 to utf8_z probe
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#include "afw.h"

#include <stdio.h>
#include <string.h>

/**
 * @file utf8_to_utf8_z_probe.c
 * @brief C probe for to_utf8_z / z_create / array-to-z embedded NUL.
 *
 * Adaptive strings are length-prefixed. This boots a core environment
 * and calls the C-string doors with .s/.len set by hand.
 */

static void
impl_utf8_set(afw_utf8_t *s, const char *z, afw_size_t len)
{
    s->s = (const afw_utf8_octet_t *)z;
    s->len = len;
}

static int
impl_expect_throw(
    void (*fn)(const afw_pool_t *, afw_xctx_t *),
    const afw_pool_t *p,
    afw_xctx_t *xctx,
    const char *label)
{
    int threw;

    threw = 0;
    AFW_TRY {
        fn(p, xctx);
    }
    AFW_CATCH_UNHANDLED {
        if (AFW_ERROR_THROWN->code == afw_error_code_general &&
            AFW_ERROR_THROWN->message_z &&
            strstr(AFW_ERROR_THROWN->message_z, "Embedded NUL"))
        {
            threw = 1;
        }
        else {
            fprintf(stderr, "%s: threw %s\n", label,
                AFW_ERROR_THROWN->message_z
                ? AFW_ERROR_THROWN->message_z : "?");
        }
    }
    AFW_ENDTRY;

    if (!threw) {
        fprintf(stderr, "%s: did not throw\n", label);
        return 1;
    }
    return 0;
}

static int
impl_empty(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_t s;
    const afw_utf8_z_t *z;

    z = afw_utf8_to_utf8_z(NULL, p, xctx);
    if (!z || z[0] != 0) {
        fprintf(stderr, "NULL string: expected empty\n");
        return 1;
    }

    impl_utf8_set(&s, "", 0);
    z = afw_utf8_to_utf8_z(&s, p, xctx);
    if (!z || z[0] != 0) {
        fprintf(stderr, "zero len: expected empty\n");
        return 1;
    }
    return 0;
}

static int
impl_ok(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_t s;
    const afw_utf8_z_t *z;

    impl_utf8_set(&s, "hello", 5);
    z = afw_utf8_to_utf8_z(&s, p, xctx);
    if (!z || strlen(z) != 5 || memcmp(z, "hello", 5) != 0) {
        fprintf(stderr, "ok: bad convert\n");
        return 1;
    }
    return 0;
}

static const char impl_embedded[] = {
    'a', 'b', 'c', 0, 'd', 'e', 'f'
};

static void
impl_to_z_embedded(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_t s;

    impl_utf8_set(&s, impl_embedded, sizeof(impl_embedded));
    (void)afw_utf8_to_utf8_z(&s, p, xctx);
}

static void
impl_to_z_single_nul(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_t s;

    impl_utf8_set(&s, "\0", 1);
    (void)afw_utf8_to_utf8_z(&s, p, xctx);
}

static void
impl_z_create_embedded(const afw_pool_t *p, afw_xctx_t *xctx)
{
    (void)afw_utf8_z_create(
        (const afw_utf8_octet_t *)impl_embedded,
        sizeof(impl_embedded), p, xctx);
}

static int
impl_embedded_nul(const afw_pool_t *p, afw_xctx_t *xctx)
{
    if (impl_expect_throw(impl_to_z_embedded, p, xctx,
        "to_utf8_z embedded"))
    {
        return 1;
    }
    return impl_expect_throw(impl_to_z_single_nul, p, xctx,
        "to_utf8_z single NUL");
}

static int
impl_z_create(const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_z_t *z;

    z = afw_utf8_z_create(
        (const afw_utf8_octet_t *)"hello", 5, p, xctx);
    if (!z || strlen(z) != 5 || memcmp(z, "hello", 5) != 0) {
        fprintf(stderr, "z_create ok: bad convert\n");
        return 1;
    }

    return impl_expect_throw(impl_z_create_embedded, p, xctx,
        "z_create embedded");
}

static void
impl_array_z_piece(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_t hello, bad;
    const afw_utf8_t *list[3];

    impl_utf8_set(&hello, "hello", 5);
    impl_utf8_set(&bad, impl_embedded, sizeof(impl_embedded));
    list[0] = &hello;
    list[1] = &bad;
    list[2] = NULL;
    (void)afw_utf8_array_to_utf8_z_with_separator(list, NULL, p, xctx);
}

static void
impl_array_z_sep(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_t hello, world, sep;
    const afw_utf8_t *list[3];

    impl_utf8_set(&hello, "hello", 5);
    impl_utf8_set(&world, "world", 5);
    impl_utf8_set(&sep, impl_embedded, sizeof(impl_embedded));
    list[0] = &hello;
    list[1] = &world;
    list[2] = NULL;
    (void)afw_utf8_array_to_utf8_z_with_separator(list, &sep, p, xctx);
}

static void
impl_z_array_sep(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_t sep;
    const afw_utf8_z_t *list[3];

    impl_utf8_set(&sep, impl_embedded, sizeof(impl_embedded));
    list[0] = "hello";
    list[1] = "world";
    list[2] = NULL;
    (void)afw_utf8_z_array_to_utf8_z_with_separator(list, &sep, p, xctx);
}

static int
impl_array_z(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_utf8_t hello, world, comma, bad;
    const afw_utf8_t *ok[3];
    const afw_utf8_t *keep[2];
    const afw_utf8_t *utf8;
    const afw_utf8_z_t *z;

    impl_utf8_set(&hello, "hello", 5);
    impl_utf8_set(&world, "world", 5);
    impl_utf8_set(&comma, ", ", 2);
    impl_utf8_set(&bad, impl_embedded, sizeof(impl_embedded));

    ok[0] = &hello;
    ok[1] = &world;
    ok[2] = NULL;
    z = afw_utf8_array_to_utf8_z_with_separator(ok, &comma, p, xctx);
    if (!z || strcmp(z, "hello, world") != 0) {
        fprintf(stderr, "array-z ok: got %s\n", z ? z : "(null)");
        return 1;
    }

    keep[0] = &bad;
    keep[1] = NULL;
    utf8 = afw_utf8_array_to_utf8_with_separator(keep, NULL, p, xctx);
    if (!utf8 || utf8->len != sizeof(impl_embedded) ||
        memcmp(utf8->s, impl_embedded, sizeof(impl_embedded)) != 0)
    {
        fprintf(stderr, "array utf8: should keep embedded NUL\n");
        return 1;
    }

    if (impl_expect_throw(impl_array_z_piece, p, xctx,
        "array-z piece"))
    {
        return 1;
    }
    if (impl_expect_throw(impl_array_z_sep, p, xctx,
        "array-z separator"))
    {
        return 1;
    }
    return impl_expect_throw(impl_z_array_sep, p, xctx,
        "z-array separator");
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

    if (strcmp(case_name, "empty") == 0) {
        rc = impl_empty(p, xctx);
    }
    else if (strcmp(case_name, "ok") == 0) {
        rc = impl_ok(p, xctx);
    }
    else if (strcmp(case_name, "embedded") == 0) {
        rc = impl_embedded_nul(p, xctx);
    }
    else if (strcmp(case_name, "z-create") == 0) {
        rc = impl_z_create(p, xctx);
    }
    else if (strcmp(case_name, "array-z") == 0) {
        rc = impl_array_z(p, xctx);
    }
    else {
        fprintf(stderr, "usage: utf8_to_utf8_z_probe "
            "empty|ok|embedded|z-create|array-z\n");
        rc = 2;
    }

    afw_pool_release(p, xctx);
    afw_environment_release(xctx);
    return rc;
}
