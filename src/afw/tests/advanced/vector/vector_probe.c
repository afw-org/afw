// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework vector probe
 *
 * Copyright (c) 2010-2026 Clemson University
 *
 */

#include "afw.h"

#include <stdio.h>
#include <string.h>

/**
 * @file vector_probe.c
 * @brief C probe for afw_vector.
 */

AFW_VECTOR_STRUCT(impl_int_vector_s, int);
typedef struct impl_int_vector_s impl_int_vector_t;


static int
impl_fail(const char *label, const char *detail)
{
    fprintf(stderr, "%s: %s\n", label, detail);
    return 1;
}


static int
impl_push_grow(afw_xctx_t *xctx)
{
    impl_int_vector_t *v;
    afw_size_t i;

    v = afw_vector_create(impl_int_vector_t, 2, xctx->p, xctx);
    for (i = 0; i < 20; i++) {
        afw_vector_push(v, xctx) = (int)i;
    }
    if (v->count != 20) {
        return impl_fail("push_grow", "count");
    }
    if (v->allocated < 20) {
        return impl_fail("push_grow", "allocated");
    }
    for (i = 0; i < 20; i++) {
        if (v->entries[i] != (int)i) {
            return impl_fail("push_grow", "value");
        }
    }
    return 0;
}


static int
impl_insert_remove(afw_xctx_t *xctx)
{
    impl_int_vector_t *v;

    v = afw_vector_create(impl_int_vector_t, 0, xctx->p, xctx);
    afw_vector_push(v, xctx) = 1;
    afw_vector_push(v, xctx) = 3;
    afw_vector_insert(v, 1, xctx) = 2;
    afw_vector_insert(v, 0, xctx) = 0;
    afw_vector_insert(v, 4, xctx) = 4;
    if (v->count != 5) {
        return impl_fail("insert_remove", "count after insert");
    }
    if (v->entries[0] != 0 || v->entries[1] != 1 || v->entries[2] != 2 ||
        v->entries[3] != 3 || v->entries[4] != 4)
    {
        return impl_fail("insert_remove", "order after insert");
    }
    afw_vector_remove(v, 0, xctx);
    afw_vector_remove(v, 2, xctx);
    if (v->count != 3) {
        return impl_fail("insert_remove", "count after remove");
    }
    if (v->entries[0] != 1 || v->entries[1] != 2 || v->entries[2] != 4) {
        return impl_fail("insert_remove", "order after remove");
    }
    return 0;
}


static int
impl_pop_clear(afw_xctx_t *xctx)
{
    impl_int_vector_t *v;
    int last;
    afw_size_t cap;

    v = afw_vector_create(impl_int_vector_t, 4, xctx->p, xctx);
    afw_vector_push(v, xctx) = 10;
    afw_vector_push(v, xctx) = 20;
    last = afw_vector_last(v);
    afw_vector_pop(v, xctx);
    if (last != 20 || v->count != 1 || v->entries[0] != 10) {
        return impl_fail("pop_clear", "pop");
    }
    cap = v->allocated;
    afw_vector_clear(v);
    if (v->count != 0) {
        return impl_fail("pop_clear", "count");
    }
    if (v->allocated != cap) {
        return impl_fail("pop_clear", "capacity");
    }
    return 0;
}


static int
impl_copy(afw_xctx_t *xctx)
{
    impl_int_vector_t *v;
    impl_int_vector_t *c;

    v = afw_vector_create(impl_int_vector_t, 2, xctx->p, xctx);
    afw_vector_push(v, xctx) = 7;
    afw_vector_push(v, xctx) = 8;
    c = afw_vector_copy(impl_int_vector_t, v, xctx->p, xctx);
    if (c->count != 2 || c->entries[0] != 7 || c->entries[1] != 8) {
        return impl_fail("copy", "values");
    }
    c->entries[0] = 9;
    if (v->entries[0] != 7) {
        return impl_fail("copy", "alias");
    }
    return 0;
}


static int
impl_append(afw_xctx_t *xctx)
{
    impl_int_vector_t *v;
    int add[3];

    v = afw_vector_create(impl_int_vector_t, 2, xctx->p, xctx);
    afw_vector_push(v, xctx) = 1;
    add[0] = 2;
    add[1] = 3;
    add[2] = 4;
    afw_vector_append(v, add, 3, xctx);
    if (v->count != 4 || v->entries[0] != 1 || v->entries[1] != 2 ||
        v->entries[2] != 3 || v->entries[3] != 4)
    {
        return impl_fail("append", "values");
    }
    afw_vector_release(v, xctx);
    return 0;
}


static int
impl_copy_entries(afw_xctx_t *xctx)
{
    impl_int_vector_t *v;
    int *out;
    int poison;
    afw_size_t n;
    afw_size_t i;

    v = afw_vector_create(impl_int_vector_t, 2, xctx->p, xctx);
    for (i = 0; i < 12; i++) {
        afw_vector_push(v, xctx) = (int)i;
    }
    n = 99;
    poison = 0;
    out = &poison;
    afw_vector_copy_entries(v, &n, &out, xctx->p, xctx);
    if (n != 12) {
        return impl_fail("copy_entries", "count");
    }
    if (!out) {
        return impl_fail("copy_entries", "ptr");
    }
    for (i = 0; i < 12; i++) {
        if (out[i] != (int)i) {
            return impl_fail("copy_entries", "value");
        }
    }
    out[0] = 99;
    if (v->entries[0] != 0) {
        return impl_fail("copy_entries", "alias");
    }
    if (v->count != 12) {
        return impl_fail("copy_entries", "source count");
    }
    afw_vector_release(v, xctx);
    return 0;
}


static int
impl_copy_entries_and_release(afw_xctx_t *xctx)
{
    impl_int_vector_t *v;
    int *out;
    int poison;
    afw_size_t n;

    v = afw_vector_create(impl_int_vector_t, 4, xctx->p, xctx);
    afw_vector_push(v, xctx) = 1;
    afw_vector_push(v, xctx) = 2;
    afw_vector_copy_entries_and_release(v, &n, &out, xctx->p, xctx);
    if (n != 2 || !out || out[0] != 1 || out[1] != 2) {
        return impl_fail("copy_entries_and_release", "values");
    }

    v = afw_vector_create(impl_int_vector_t, 8, xctx->p, xctx);
    n = 99;
    poison = 0;
    out = &poison;
    afw_vector_copy_entries_and_release(v, &n, &out, xctx->p, xctx);
    if (n != 0 || out != NULL) {
        return impl_fail("copy_entries_and_release", "empty");
    }
    return 0;
}


static int
impl_expect_throw(
    const char *label,
    afw_error_code_t code,
    void (*fn)(impl_int_vector_t *, afw_xctx_t *),
    impl_int_vector_t *v,
    afw_xctx_t *xctx)
{
    int threw;

    threw = 0;
    AFW_TRY {
        fn(v, xctx);
    }
    AFW_CATCH_UNHANDLED {
        if (AFW_ERROR_THROWN->code == code) {
            threw = 1;
        }
        else {
            fprintf(stderr, "%s: threw %s\n", label,
                AFW_ERROR_THROWN->message_z
                ? AFW_ERROR_THROWN->message_z : "?");
            return 1;
        }
    }
    AFW_ENDTRY;

    if (!threw) {
        return impl_fail(label, "did not throw");
    }
    return 0;
}


static void
impl_pop_empty(impl_int_vector_t *v, afw_xctx_t *xctx)
{
    afw_vector_pop(v, xctx);
}


static void
impl_insert_past(impl_int_vector_t *v, afw_xctx_t *xctx)
{
    afw_vector_insert(v, 1, xctx) = 1;
}


static void
impl_remove_past(impl_int_vector_t *v, afw_xctx_t *xctx)
{
    afw_vector_remove(v, 0, xctx);
}


static int
impl_underflow(afw_xctx_t *xctx)
{
    impl_int_vector_t *v;

    v = afw_vector_create(impl_int_vector_t, 0, xctx->p, xctx);
    return impl_expect_throw("underflow", afw_error_code_general,
        impl_pop_empty, v, xctx);
}


static int
impl_bad_index(afw_xctx_t *xctx)
{
    impl_int_vector_t *v;
    int rc;

    v = afw_vector_create(impl_int_vector_t, 0, xctx->p, xctx);
    rc = impl_expect_throw("bad_index insert",
        afw_error_code_argument_error, impl_insert_past, v, xctx);
    if (rc != 0) {
        return rc;
    }
    return impl_expect_throw("bad_index remove",
        afw_error_code_argument_error, impl_remove_past, v, xctx);
}


int
main(int argc, char **argv)
{
    const afw_error_t *create_error;
    afw_xctx_t *xctx;
    const char *case_name;
    int rc;

    xctx = afw_environment_create(afw_version(), argc,
        (const char * const *)argv, &create_error);
    if (!xctx) {
        fprintf(stderr, "environment create failed\n");
        return 2;
    }

    case_name = (argc > 1) ? argv[1] : "";
    rc = 0;

    if (strcmp(case_name, "push_grow") == 0) {
        rc = impl_push_grow(xctx);
    }
    else if (strcmp(case_name, "insert_remove") == 0) {
        rc = impl_insert_remove(xctx);
    }
    else if (strcmp(case_name, "pop_clear") == 0) {
        rc = impl_pop_clear(xctx);
    }
    else if (strcmp(case_name, "copy") == 0) {
        rc = impl_copy(xctx);
    }
    else if (strcmp(case_name, "append") == 0) {
        rc = impl_append(xctx);
    }
    else if (strcmp(case_name, "copy_entries") == 0) {
        rc = impl_copy_entries(xctx);
    }
    else if (strcmp(case_name, "copy_entries_and_release") == 0) {
        rc = impl_copy_entries_and_release(xctx);
    }
    else if (strcmp(case_name, "underflow") == 0) {
        rc = impl_underflow(xctx);
    }
    else if (strcmp(case_name, "bad_index") == 0) {
        rc = impl_bad_index(xctx);
    }
    else {
        fprintf(stderr, "usage: vector_probe "
            "push_grow|insert_remove|pop_clear|copy|append|"
            "copy_entries|copy_entries_and_release|"
            "underflow|bad_index\n");
        rc = 2;
    }

    afw_environment_release(xctx);
    return rc;
}
