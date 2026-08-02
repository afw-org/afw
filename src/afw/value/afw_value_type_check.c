// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework value type checking (issue #28)
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_value_type_check.c
 * @brief Compile/runtime type-check mode and leaf assignability.
 */

#include "afw_internal.h"



AFW_DEFINE(afw_value_type_check_mode_t)
afw_value_type_check_mode(afw_xctx_t *xctx)
{
    /* compileOnly wins when both are set. */
    if (afw_flag_is_active(
        xctx->env->flag_index_compile_typeCheckCompileOnly_active, xctx))
    {
        return afw_value_type_check_mode_compile_only;
    }
    if (afw_flag_is_active(
        xctx->env->flag_index_compile_typeCheck_active, xctx))
    {
        return afw_value_type_check_mode_on;
    }
    return afw_value_type_check_mode_off;
}



AFW_DEFINE(afw_boolean_t)
afw_value_type_check_compile_enabled(afw_xctx_t *xctx)
{
    return afw_value_type_check_mode(xctx) != afw_value_type_check_mode_off;
}



AFW_DEFINE(afw_boolean_t)
afw_value_type_check_runtime_enabled(afw_xctx_t *xctx)
{
    return afw_value_type_check_mode(xctx) == afw_value_type_check_mode_on;
}



AFW_DEFINE(afw_boolean_t)
afw_value_type_is_any(const afw_value_type_t *type)
{
    if (!type) {
        return true;
    }
    if (type->kind == afw_value_type_kind_data_type &&
        (!type->data_type || type->data_type == afw_data_type_any))
    {
        return true;
    }
    return false;
}



AFW_DEFINE(const afw_data_type_t *)
afw_value_type_get_leaf_data_type(const afw_value_type_t *type)
{
    if (type && type->kind == afw_value_type_kind_data_type) {
        return type->data_type;
    }
    return NULL;
}



static afw_boolean_t
impl_is_nullish_value(const afw_value_t *value, afw_xctx_t *xctx)
{
    const afw_data_type_t *dt;

    if (afw_value_is_undefined(value)) {
        return true;
    }
    dt = afw_value_get_data_type(value, xctx);
    if (dt == afw_data_type_null || dt == afw_data_type_undefined) {
        return true;
    }
    return false;
}



AFW_DEFINE(afw_boolean_t)
afw_value_type_is_assignable(
    const afw_value_type_t *expected,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_data_type_t *want;
    const afw_data_type_t *got;
    afw_boolean_t strict_null;

    if (afw_value_type_is_any(expected)) {
        return true;
    }
    if (!value) {
        return false;
    }

    /* Union: assignable if matches any member. */
    if (expected->kind == afw_value_type_kind_union) {
        afw_size_t i;

        for (i = 0; i < expected->compound.count; i++) {
            if (afw_value_type_is_assignable(
                expected->compound.members[i], value, xctx))
            {
                return true;
            }
        }
        return false;
    }

    /* Intersection: must match every member. */
    if (expected->kind == afw_value_type_kind_intersection) {
        afw_size_t i;

        for (i = 0; i < expected->compound.count; i++) {
            if (!afw_value_type_is_assignable(
                expected->compound.members[i], value, xctx))
            {
                return false;
            }
        }
        return expected->compound.count > 0;
    }

    /* Array / tuple: value must be an array (element types later). */
    if (expected->kind == afw_value_type_kind_array ||
        expected->kind == afw_value_type_kind_tuple)
    {
        got = afw_value_get_data_type(value, xctx);
        return got == afw_data_type_array;
    }

    /* Object shape / interface: value must be an object. */
    if (expected->kind == afw_value_type_kind_object) {
        got = afw_value_get_data_type(value, xctx);
        return got == afw_data_type_object;
    }

    /* Named reference: use resolved type if present. */
    if (expected->kind == afw_value_type_kind_reference) {
        if (expected->reference.resolved) {
            return afw_value_type_is_assignable(
                expected->reference.resolved, value, xctx);
        }
        return !afw_value_is_undefined(value);
    }

    /* Function type: value should be a function. */
    if (expected->kind == afw_value_type_kind_function) {
        got = afw_value_get_data_type(value, xctx);
        return got == afw_data_type_function;
    }

    want = afw_value_type_get_leaf_data_type(expected);
    if (!want) {
        return !afw_value_is_undefined(value);
    }

    if (want == afw_data_type_any || want == afw_data_type_unknown) {
        /* unknown accepts any value (TS: assign TO unknown). */
        return true;
    }

    strict_null = afw_flag_is_active(
        xctx->env->flag_index_compile_strictNullChecks_active, xctx);

    if (want == afw_data_type_void) {
        /* void slot: undefined, or null if not strictNullChecks. */
        if (afw_value_is_undefined(value)) {
            return true;
        }
        got = afw_value_get_data_type(value, xctx);
        if (got == afw_data_type_undefined) {
            return true;
        }
        if (!strict_null && got == afw_data_type_null) {
            return true;
        }
        return false;
    }

    if (impl_is_nullish_value(value, xctx)) {
        if (!strict_null) {
            /* Without strictNullChecks, nullish may assign (TS-ish loose). */
            return true;
        }
        /* strictNullChecks: only if type is null or undefined leaf. */
        return want == afw_data_type_null || want == afw_data_type_undefined;
    }

    got = afw_value_get_data_type(value, xctx);
    if (!got) {
        /* Unknown runtime type — do not fail assignability. */
        return true;
    }

    if (got == want) {
        return true;
    }

    /* array leaf type accepts array values (element types not checked v1). */
    if (want == afw_data_type_array && got == afw_data_type_array) {
        return true;
    }
    if (want == afw_data_type_object && got == afw_data_type_object) {
        return true;
    }

    return false;
}



AFW_DEFINE(void)
afw_value_type_check_assignable(
    const afw_value_type_t *expected,
    const afw_value_t *value,
    const afw_utf8_z_t *what,
    afw_xctx_t *xctx)
{
    const afw_data_type_t *want;
    const afw_data_type_t *got;
    const afw_utf8_t composite_lit = AFW_UTF8_LITERAL("composite");
    const afw_utf8_t unknown_lit = AFW_UTF8_LITERAL("unknown");
    const afw_utf8_t *want_id;
    const afw_utf8_t *got_id;

    if (!afw_value_type_check_runtime_enabled(xctx)) {
        return;
    }
    if (afw_value_type_is_assignable(expected, value, xctx)) {
        return;
    }

    want = afw_value_type_get_leaf_data_type(expected);
    got = value ? afw_value_get_data_type(value, xctx) : NULL;
    want_id = want ? &want->data_type_id : &composite_lit;
    got_id = got ? &got->data_type_id : &unknown_lit;

    AFW_THROW_ERROR_FZ(general, xctx,
        "Type error in %s: expected " AFW_UTF8_FMT
        " but got " AFW_UTF8_FMT,
        what ? what : "value",
        AFW_UTF8_FMT_ARG(want_id),
        AFW_UTF8_FMT_ARG(got_id));
}



AFW_DEFINE(void)
afw_value_type_check_compile_assignable(
    const afw_value_type_t *expected,
    const afw_value_t *value,
    const afw_utf8_z_t *what,
    afw_xctx_t *xctx)
{
    const afw_data_type_t *want;
    const afw_data_type_t *got;
    const afw_utf8_t composite_lit = AFW_UTF8_LITERAL("composite");
    const afw_utf8_t *want_id;

    if (!afw_value_type_check_compile_enabled(xctx)) {
        return;
    }
    if (afw_value_type_is_any(expected) || !value) {
        return;
    }

    /* Only check when RHS already has a known data type at compile time. */
    got = afw_value_get_data_type(value, xctx);
    if (!got) {
        return;
    }

    if (afw_value_type_is_assignable(expected, value, xctx)) {
        return;
    }

    want = afw_value_type_get_leaf_data_type(expected);
    want_id = want ? &want->data_type_id : &composite_lit;

    AFW_THROW_ERROR_FZ(syntax, xctx,
        "Type error in %s: expected " AFW_UTF8_FMT
        " but got " AFW_UTF8_FMT,
        what ? what : "value",
        AFW_UTF8_FMT_ARG(want_id),
        AFW_UTF8_FMT_ARG(&got->data_type_id));
}
