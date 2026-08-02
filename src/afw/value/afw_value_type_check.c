// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework value type checking (issue #28)
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_value_type_check.c
 * @brief Compile/runtime type-check mode and assignability (incl. structural).
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



static afw_boolean_t
impl_strict_null_checks(afw_xctx_t *xctx)
{
    return afw_flag_is_active(
        xctx->env->flag_index_compile_strictNullChecks_active, xctx);
}



/**
 * Property value from an evaluated object, object_expression, or
 * object_construct (static names only). Returns NULL if absent.
 * has_open_props is set when a construct has spread/expr names so missing
 * required props cannot be proven.
 */
static const afw_value_t *
impl_get_typed_object_property(
    const afw_value_t *value,
    const afw_utf8_t *name,
    afw_boolean_t *has_open_props,
    afw_xctx_t *xctx)
{
    const afw_object_t *obj;
    const afw_value_object_construct_t *construct;
    const afw_value_object_construct_entry_t *e;
    const afw_value_t *found;

    *has_open_props = false;

    if (AFW_VALUE_IS_DATA_TYPE(value, object)) {
        obj = ((const afw_value_object_t *)value)->internal;
        return afw_object_get_property(obj, name, xctx);
    }

    if (afw_value_is_object_expression(value)) {
        obj = ((const afw_value_object_expression_t *)value)->internal;
        return afw_object_get_property(obj, name, xctx);
    }

    if (afw_value_is_object_construct(value)) {
        construct = (const afw_value_object_construct_t *)value;
        found = NULL;
        for (e = construct->entries; e; e = e->next) {
            if (e->type == afw_value_object_construct_entry_spread ||
                e->type == afw_value_object_construct_entry_name_expr)
            {
                *has_open_props = true;
            }
            else if (e->type == afw_value_object_construct_entry_static &&
                e->static_name &&
                afw_utf8_equal(e->static_name, name))
            {
                found = e->value;
            }
        }
        return found;
    }

    /* Unevaluated / non-inspectable object-shaped value. */
    *has_open_props = true;
    return NULL;
}



static afw_boolean_t
impl_object_type_properties_assignable(
    const afw_value_type_t *expected,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_value_type_property_t *prop;
    const afw_value_t *pv;
    afw_boolean_t has_open_props;
    afw_size_t i;
    const afw_value_type_t *base;

    /* Interface extends: value must satisfy each base type. */
    for (i = 0; i < expected->object.extends_count; i++) {
        base = expected->object.extends[i];
        if (base &&
            !afw_value_type_is_assignable(base, value, xctx))
        {
            return false;
        }
    }

    for (prop = expected->object.properties; prop; prop = prop->next) {
        if (!prop->name) {
            continue;
        }
        pv = impl_get_typed_object_property(value, prop->name,
            &has_open_props, xctx);

        if (!pv || afw_value_is_undefined(pv)) {
            if (prop->optional) {
                continue;
            }
            /* Spread / dynamic names: cannot prove required prop missing. */
            if (has_open_props) {
                continue;
            }
            return false;
        }

        if (prop->type && !afw_value_type_is_any(prop->type)) {
            if (!afw_value_type_is_assignable(prop->type, pv, xctx)) {
                return false;
            }
        }
    }

    return true;
}



static const afw_array_t *
impl_try_array_internal(const afw_value_t *value)
{
    if (AFW_VALUE_IS_DATA_TYPE(value, array)) {
        return ((const afw_value_array_t *)value)->internal;
    }
    /*
     * list_expression wraps an unevaluated array constructor call in some
     * cases; only deep-check plain evaluated arrays for now.
     */
    return NULL;
}



static afw_boolean_t
impl_array_elements_assignable(
    const afw_value_type_t *element_type,
    const afw_array_t *arr,
    afw_xctx_t *xctx)
{
    afw_size_t count;
    afw_size_t i;
    const afw_value_t *elem;

    if (!element_type || afw_value_type_is_any(element_type)) {
        return true;
    }

    count = afw_array_get_count(arr, xctx);
    for (i = 0; i < count; i++) {
        elem = afw_array_get_entry_value(arr, i, xctx->p, xctx);
        if (!afw_value_type_is_assignable(element_type, elem, xctx)) {
            return false;
        }
    }
    return true;
}



static afw_boolean_t
impl_tuple_elements_assignable(
    const afw_value_type_t *expected,
    const afw_array_t *arr,
    afw_xctx_t *xctx)
{
    afw_size_t count;
    afw_size_t i;
    const afw_value_t *elem;
    const afw_value_type_t *et;

    count = afw_array_get_count(arr, xctx);
    if (count != expected->tuple.count) {
        return false;
    }

    for (i = 0; i < expected->tuple.count; i++) {
        et = expected->tuple.elements[i];
        elem = afw_array_get_entry_value(arr, i, xctx->p, xctx);
        if (et && !afw_value_type_is_any(et)) {
            if (!afw_value_type_is_assignable(et, elem, xctx)) {
                return false;
            }
        }
    }
    return true;
}



AFW_DEFINE(afw_boolean_t)
afw_value_type_is_assignable(
    const afw_value_type_t *expected,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_data_type_t *want;
    const afw_data_type_t *got;
    const afw_array_t *arr;
    afw_boolean_t strict_null;

    if (afw_value_type_is_any(expected)) {
        return true;
    }
    if (!value) {
        return false;
    }

    strict_null = impl_strict_null_checks(xctx);

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

    /* Array: container + optional element types when inspectable. */
    if (expected->kind == afw_value_type_kind_array) {
        if (impl_is_nullish_value(value, xctx)) {
            return !strict_null;
        }
        got = afw_value_get_data_type(value, xctx);
        if (got != afw_data_type_array) {
            return false;
        }
        arr = impl_try_array_internal(value);
        if (!arr) {
            return true;
        }
        return impl_array_elements_assignable(
            expected->array.element, arr, xctx);
    }

    /* Tuple: fixed length + per-position types when inspectable. */
    if (expected->kind == afw_value_type_kind_tuple) {
        if (impl_is_nullish_value(value, xctx)) {
            return !strict_null;
        }
        got = afw_value_get_data_type(value, xctx);
        if (got != afw_data_type_array) {
            return false;
        }
        arr = impl_try_array_internal(value);
        if (!arr) {
            return true;
        }
        return impl_tuple_elements_assignable(expected, arr, xctx);
    }

    /* Object shape / interface: properties + extends when inspectable. */
    if (expected->kind == afw_value_type_kind_object) {
        if (impl_is_nullish_value(value, xctx)) {
            return !strict_null;
        }
        got = afw_value_get_data_type(value, xctx);
        if (got != afw_data_type_object) {
            return false;
        }
        return impl_object_type_properties_assignable(expected, value, xctx);
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
        if (impl_is_nullish_value(value, xctx)) {
            return !strict_null;
        }
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

    /* array leaf type accepts array values (element types not checked). */
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
