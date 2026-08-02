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
 *
 * Excess-property checks on object literals are compile-only (see
 * afw_value_type_check_compile_assignable). Runtime remains open structural
 * so adaptive objects may carry extra properties.
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



/* ---------- type text for error messages ---------- */

static const afw_utf8_z_t *
impl_type_to_z(
    const afw_value_type_t *type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_writer_t *writer;
    afw_utf8_t current;

    if (afw_value_type_is_any(type)) {
        return "any";
    }

    writer = afw_utf8_writer_create(NULL, p, xctx);
    if (!afw_value_decompile_type(type, writer, xctx)) {
        return "any";
    }
    afw_utf8_writer_current_string(writer, &current, xctx);
    if (current.len == 0) {
        return "any";
    }
    return afw_utf8_to_utf8_z(&current, p, xctx);
}



static const afw_utf8_z_t *
impl_value_type_to_z(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_data_type_t *got;

    if (!value || afw_value_is_undefined(value)) {
        return "undefined";
    }
    got = afw_value_get_data_type(value, xctx);
    if (!got) {
        return "unknown";
    }
    return afw_utf8_to_utf8_z(&got->data_type_id, p, xctx);
}



/* ---------- type-to-type assignability (for function formals/returns) ---------- */

static afw_boolean_t
impl_type_is_type_assignable(
    const afw_value_type_t *to,
    const afw_value_type_t *from,
    afw_xctx_t *xctx);



static afw_boolean_t
impl_type_is_type_assignable(
    const afw_value_type_t *to,
    const afw_value_type_t *from,
    afw_xctx_t *xctx)
{
    const afw_data_type_t *want;
    const afw_data_type_t *got;
    afw_size_t i;
    const afw_value_type_property_t *prop;
    const afw_value_type_property_t *from_prop;
    const afw_value_type_function_param_t *tp;
    const afw_value_type_function_param_t *fp;

    if (afw_value_type_is_any(to)) {
        return true;
    }
    if (afw_value_type_is_any(from)) {
        /* Unannotated / any source is accepted (escape hatch). */
        return true;
    }

    if (to->kind == afw_value_type_kind_reference) {
        if (to->reference.resolved) {
            return impl_type_is_type_assignable(to->reference.resolved,
                from, xctx);
        }
        return true;
    }
    if (from->kind == afw_value_type_kind_reference) {
        if (from->reference.resolved) {
            return impl_type_is_type_assignable(to, from->reference.resolved,
                xctx);
        }
        return true;
    }

    if (to->kind == afw_value_type_kind_union) {
        for (i = 0; i < to->compound.count; i++) {
            if (impl_type_is_type_assignable(to->compound.members[i],
                from, xctx))
            {
                return true;
            }
        }
        return false;
    }
    if (from->kind == afw_value_type_kind_union) {
        for (i = 0; i < from->compound.count; i++) {
            if (!impl_type_is_type_assignable(to, from->compound.members[i],
                xctx))
            {
                return false;
            }
        }
        return from->compound.count > 0;
    }

    if (to->kind == afw_value_type_kind_intersection) {
        for (i = 0; i < to->compound.count; i++) {
            if (!impl_type_is_type_assignable(to->compound.members[i],
                from, xctx))
            {
                return false;
            }
        }
        return to->compound.count > 0;
    }

    if (to->kind == afw_value_type_kind_array) {
        if (from->kind != afw_value_type_kind_array &&
            !(from->kind == afw_value_type_kind_data_type &&
                from->data_type == afw_data_type_array))
        {
            return false;
        }
        if (from->kind == afw_value_type_kind_array) {
            return impl_type_is_type_assignable(to->array.element,
                from->array.element, xctx);
        }
        return true;
    }

    if (to->kind == afw_value_type_kind_tuple) {
        if (from->kind != afw_value_type_kind_tuple) {
            return false;
        }
        if (to->tuple.count != from->tuple.count) {
            return false;
        }
        for (i = 0; i < to->tuple.count; i++) {
            if (!impl_type_is_type_assignable(to->tuple.elements[i],
                from->tuple.elements[i], xctx))
            {
                return false;
            }
        }
        return true;
    }

    if (to->kind == afw_value_type_kind_object) {
        if (from->kind != afw_value_type_kind_object &&
            !(from->kind == afw_value_type_kind_data_type &&
                from->data_type == afw_data_type_object))
        {
            return false;
        }
        if (from->kind != afw_value_type_kind_object) {
            return true;
        }
        /* Structural: each required `to` prop must exist on `from`. */
        for (prop = to->object.properties; prop; prop = prop->next) {
            if (!prop->name) {
                continue;
            }
            from_prop = NULL;
            for (from_prop = from->object.properties; from_prop;
                from_prop = from_prop->next)
            {
                if (from_prop->name &&
                    afw_utf8_equal(from_prop->name, prop->name))
                {
                    break;
                }
            }
            if (!from_prop) {
                if (prop->optional) {
                    continue;
                }
                return false;
            }
            if (!impl_type_is_type_assignable(prop->type, from_prop->type,
                xctx))
            {
                return false;
            }
        }
        return true;
    }

    if (to->kind == afw_value_type_kind_function) {
        if (from->kind != afw_value_type_kind_function) {
            return false;
        }
        /* Return: from.return assignable to to.return. */
        if (!impl_type_is_type_assignable(to->function.returns,
            from->function.returns, xctx))
        {
            return false;
        }
        /* Params: contravariant — to.param assignable to from.param. */
        tp = to->function.parameters;
        fp = from->function.parameters;
        while (tp) {
            if (tp->is_rest) {
                break;
            }
            if (!fp) {
                if (!tp->optional) {
                    return false;
                }
                tp = tp->next;
                continue;
            }
            if (!impl_type_is_type_assignable(fp->type, tp->type, xctx)) {
                return false;
            }
            tp = tp->next;
            fp = fp->next;
        }
        return true;
    }

    want = afw_value_type_get_leaf_data_type(to);
    got = afw_value_type_get_leaf_data_type(from);
    if (!want) {
        return true;
    }
    if (!got) {
        return true;
    }
    if (want == got) {
        return true;
    }
    if (want == afw_data_type_array && got == afw_data_type_array) {
        return true;
    }
    if (want == afw_data_type_object && got == afw_data_type_object) {
        return true;
    }
    return false;
}



/* ---------- value inspection helpers ---------- */

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

    *has_open_props = true;
    return NULL;
}



static const afw_array_t *
impl_try_array_internal(const afw_value_t *value)
{
    if (AFW_VALUE_IS_DATA_TYPE(value, array)) {
        return ((const afw_value_array_t *)value)->internal;
    }
    return NULL;
}



static const afw_value_script_function_definition_t *
impl_as_script_function(const afw_value_t *value)
{
    if (afw_value_is_script_function_definition(value)) {
        return (const afw_value_script_function_definition_t *)value;
    }
    if (afw_value_is_closure_binding(value)) {
        return ((const afw_value_closure_binding_t *)value)->
            script_function_definition;
    }
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



/**
 * Compare script function definition to expected function type.
 * Param positions are contravariant; return is covariant (simplified).
 * Built-ins without a signature only get a shallow "is function" check.
 */
static afw_boolean_t
impl_function_value_assignable(
    const afw_value_type_t *expected,
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_value_script_function_definition_t *fn;
    const afw_value_type_function_param_t *ep;
    const afw_value_script_function_parameter_t *vp;
    const afw_value_type_t *v_ret;
    const afw_value_type_t *v_param_type;
    afw_size_t i;

    fn = impl_as_script_function(value);
    if (!fn) {
        /* Built-in / non-script function: container check only. */
        return true;
    }

    v_ret = fn->returns;
    if (!v_ret && fn->signature) {
        v_ret = fn->signature->returns;
    }
    if (!impl_type_is_type_assignable(expected->function.returns, v_ret,
        xctx))
    {
        return false;
    }

    ep = expected->function.parameters;
    for (i = 0; ep; ep = ep->next, i++) {
        if (ep->is_rest) {
            break;
        }
        if (i >= fn->count) {
            if (!ep->optional) {
                return false;
            }
            continue;
        }
        vp = fn->parameters[i];
        if (!vp) {
            if (!ep->optional) {
                return false;
            }
            continue;
        }
        v_param_type = vp->type;
        /* Contravariant: expected formal assignable to value formal. */
        if (!impl_type_is_type_assignable(v_param_type, ep->type, xctx)) {
            return false;
        }
    }

    return true;
}



/* ---------- mismatch detail for richer errors ---------- */

static const afw_utf8_z_t *
impl_mismatch_detail(
    const afw_value_type_t *expected,
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_data_type_t *got;
    const afw_array_t *arr;
    const afw_value_type_property_t *prop;
    const afw_value_t *pv;
    afw_boolean_t has_open_props;
    afw_size_t count;
    afw_size_t i;
    const afw_value_t *elem;
    const afw_utf8_z_t *want_z;
    const afw_utf8_z_t *got_z;
    const afw_value_script_function_definition_t *fn;

    if (!expected || afw_value_type_is_any(expected)) {
        return NULL;
    }

    want_z = impl_type_to_z(expected, p, xctx);
    got_z = impl_value_type_to_z(value, p, xctx);

    if (expected->kind == afw_value_type_kind_object) {
        got = afw_value_get_data_type(value, xctx);
        if (got != afw_data_type_object) {
            return afw_utf8_z_printf(p, xctx,
                "expected object type %s but got %s", want_z, got_z);
        }
        for (prop = expected->object.properties; prop; prop = prop->next) {
            if (!prop->name || prop->optional) {
                continue;
            }
            pv = impl_get_typed_object_property(value, prop->name,
                &has_open_props, xctx);
            if ((!pv || afw_value_is_undefined(pv)) && !has_open_props) {
                return afw_utf8_z_printf(p, xctx,
                    "missing required property " AFW_UTF8_FMT_Q
                    " for type %s",
                    AFW_UTF8_FMT_ARG(prop->name), want_z);
            }
            if (pv && prop->type &&
                !afw_value_type_is_any(prop->type) &&
                !afw_value_type_is_assignable(prop->type, pv, xctx))
            {
                return afw_utf8_z_printf(p, xctx,
                    "property " AFW_UTF8_FMT_Q
                    ": expected %s but got %s",
                    AFW_UTF8_FMT_ARG(prop->name),
                    impl_type_to_z(prop->type, p, xctx),
                    impl_value_type_to_z(pv, p, xctx));
            }
        }
        return afw_utf8_z_printf(p, xctx,
            "value does not match object type %s", want_z);
    }

    if (expected->kind == afw_value_type_kind_array) {
        got = afw_value_get_data_type(value, xctx);
        if (got != afw_data_type_array) {
            return afw_utf8_z_printf(p, xctx,
                "expected array type %s but got %s", want_z, got_z);
        }
        arr = impl_try_array_internal(value);
        if (arr && expected->array.element &&
            !afw_value_type_is_any(expected->array.element))
        {
            count = afw_array_get_count(arr, xctx);
            for (i = 0; i < count; i++) {
                elem = afw_array_get_entry_value(arr, i, p, xctx);
                if (!afw_value_type_is_assignable(expected->array.element,
                    elem, xctx))
                {
                    return afw_utf8_z_printf(p, xctx,
                        "array element %lu: expected %s but got %s",
                        (unsigned long)i,
                        impl_type_to_z(expected->array.element, p, xctx),
                        impl_value_type_to_z(elem, p, xctx));
                }
            }
        }
        return afw_utf8_z_printf(p, xctx,
            "value does not match array type %s", want_z);
    }

    if (expected->kind == afw_value_type_kind_tuple) {
        got = afw_value_get_data_type(value, xctx);
        if (got != afw_data_type_array) {
            return afw_utf8_z_printf(p, xctx,
                "expected tuple type %s but got %s", want_z, got_z);
        }
        arr = impl_try_array_internal(value);
        if (arr) {
            count = afw_array_get_count(arr, xctx);
            if (count != expected->tuple.count) {
                return afw_utf8_z_printf(p, xctx,
                    "tuple length %lu but expected %lu for type %s",
                    (unsigned long)count,
                    (unsigned long)expected->tuple.count,
                    want_z);
            }
            for (i = 0; i < expected->tuple.count; i++) {
                elem = afw_array_get_entry_value(arr, i, p, xctx);
                if (expected->tuple.elements[i] &&
                    !afw_value_type_is_any(expected->tuple.elements[i]) &&
                    !afw_value_type_is_assignable(
                        expected->tuple.elements[i], elem, xctx))
                {
                    return afw_utf8_z_printf(p, xctx,
                        "tuple element %lu: expected %s but got %s",
                        (unsigned long)i,
                        impl_type_to_z(expected->tuple.elements[i], p, xctx),
                        impl_value_type_to_z(elem, p, xctx));
                }
            }
        }
        return afw_utf8_z_printf(p, xctx,
            "value does not match tuple type %s", want_z);
    }

    if (expected->kind == afw_value_type_kind_function) {
        got = afw_value_get_data_type(value, xctx);
        if (got != afw_data_type_function) {
            return afw_utf8_z_printf(p, xctx,
                "expected function type %s but got %s", want_z, got_z);
        }
        fn = impl_as_script_function(value);
        if (fn) {
            return afw_utf8_z_printf(p, xctx,
                "function signature does not match type %s", want_z);
        }
        return afw_utf8_z_printf(p, xctx,
            "expected function type %s", want_z);
    }

    if (expected->kind == afw_value_type_kind_union ||
        expected->kind == afw_value_type_kind_intersection)
    {
        return afw_utf8_z_printf(p, xctx,
            "expected %s but got %s", want_z, got_z);
    }

    /* Leaf / default */
    return afw_utf8_z_printf(p, xctx,
        "expected %s but got %s", want_z, got_z);
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

    if (expected->kind == afw_value_type_kind_reference) {
        if (expected->reference.resolved) {
            return afw_value_type_is_assignable(
                expected->reference.resolved, value, xctx);
        }
        return !afw_value_is_undefined(value);
    }

    if (expected->kind == afw_value_type_kind_function) {
        if (impl_is_nullish_value(value, xctx)) {
            return !strict_null;
        }
        got = afw_value_get_data_type(value, xctx);
        if (got != afw_data_type_function) {
            return false;
        }
        return impl_function_value_assignable(expected, value, xctx);
    }

    want = afw_value_type_get_leaf_data_type(expected);
    if (!want) {
        return !afw_value_is_undefined(value);
    }

    if (want == afw_data_type_any || want == afw_data_type_unknown) {
        return true;
    }

    if (want == afw_data_type_void) {
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
            return true;
        }
        return want == afw_data_type_null || want == afw_data_type_undefined;
    }

    got = afw_value_get_data_type(value, xctx);
    if (!got) {
        return true;
    }

    if (got == want) {
        return true;
    }

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
    const afw_utf8_z_t *detail;

    if (!afw_value_type_check_runtime_enabled(xctx)) {
        return;
    }
    if (afw_value_type_is_assignable(expected, value, xctx)) {
        return;
    }

    detail = impl_mismatch_detail(expected, value, xctx->p, xctx);
    if (detail) {
        AFW_THROW_ERROR_FZ(general, xctx,
            "Type error in %s: %s",
            what ? what : "value",
            detail);
    }

    AFW_THROW_ERROR_FZ(general, xctx,
        "Type error in %s: expected %s but got %s",
        what ? what : "value",
        impl_type_to_z(expected, xctx->p, xctx),
        impl_value_type_to_z(value, xctx->p, xctx));
}



/*
 * Excess-property checks — compile time only, on object literals.
 * Runtime assignability stays open so adaptive objects with extra
 * properties remain valid for script-local shapes.
 *
 * Object literal RHS: object_construct / object_expression / evaluated
 * object value used at compile time. Skip when the construct has spreads
 * or expression property names (set of keys not statically known).
 */

static const afw_value_type_t *
impl_resolve_type(const afw_value_type_t *type)
{
    if (!type) {
        return NULL;
    }
    if (type->kind == afw_value_type_kind_reference &&
        type->reference.resolved)
    {
        return impl_resolve_type(type->reference.resolved);
    }
    return type;
}



static afw_boolean_t
impl_object_type_declares_name(
    const afw_value_type_t *type,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    const afw_value_type_property_t *prop;
    afw_size_t i;
    const afw_value_type_t *base;

    type = impl_resolve_type(type);
    if (!type || type->kind != afw_value_type_kind_object) {
        return false;
    }

    for (i = 0; i < type->object.extends_count; i++) {
        base = type->object.extends[i];
        if (base && impl_object_type_declares_name(base, name, xctx)) {
            return true;
        }
    }

    for (prop = type->object.properties; prop; prop = prop->next) {
        if (prop->name && afw_utf8_equal(prop->name, name)) {
            return true;
        }
    }

    return false;
}



static const afw_value_type_t *
impl_object_type_property_type(
    const afw_value_type_t *type,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    const afw_value_type_property_t *prop;
    afw_size_t i;
    const afw_value_type_t *base;
    const afw_value_type_t *found;

    type = impl_resolve_type(type);
    if (!type || type->kind != afw_value_type_kind_object) {
        return NULL;
    }

    for (prop = type->object.properties; prop; prop = prop->next) {
        if (prop->name && afw_utf8_equal(prop->name, name)) {
            return prop->type;
        }
    }

    for (i = 0; i < type->object.extends_count; i++) {
        base = type->object.extends[i];
        found = impl_object_type_property_type(base, name, xctx);
        if (found) {
            return found;
        }
    }

    return NULL;
}



/**
 * @return true if value is an object literal (for excess-property checks).
 * @param open_keys set when keys are not fully static (skip excess).
 */
static afw_boolean_t
impl_is_object_literal_for_excess(
    const afw_value_t *value,
    afw_boolean_t *open_keys)
{
    const afw_value_object_construct_t *construct;
    const afw_value_object_construct_entry_t *e;

    *open_keys = false;

    if (afw_value_is_object_construct(value)) {
        construct = (const afw_value_object_construct_t *)value;
        for (e = construct->entries; e; e = e->next) {
            if (e->type == afw_value_object_construct_entry_spread ||
                e->type == afw_value_object_construct_entry_name_expr)
            {
                *open_keys = true;
                break;
            }
        }
        return true;
    }

    if (afw_value_is_object_expression(value)) {
        return true;
    }

    /*
     * Compile-time constant object literals evaluate to unmanaged objects
     * while still being the direct RHS of const/let.
     */
    if (AFW_VALUE_IS_DATA_TYPE(value, object)) {
        return true;
    }

    return false;
}



static void
impl_foreach_object_literal_prop(
    const afw_value_t *value,
    void (*cb)(
        const afw_utf8_t *name,
        const afw_value_t *prop_value,
        void *data,
        afw_xctx_t *xctx),
    void *data,
    afw_xctx_t *xctx)
{
    const afw_object_t *obj;
    const afw_iterator_t *iterator;
    const afw_utf8_t *name;
    const afw_value_t *pv;
    const afw_value_object_construct_t *construct;
    const afw_value_object_construct_entry_t *e;

    if (afw_value_is_object_construct(value)) {
        construct = (const afw_value_object_construct_t *)value;
        for (e = construct->entries; e; e = e->next) {
            if (e->type == afw_value_object_construct_entry_static &&
                e->static_name)
            {
                cb(e->static_name, e->value, data, xctx);
            }
        }
        return;
    }

    if (afw_value_is_object_expression(value)) {
        obj = ((const afw_value_object_expression_t *)value)->internal;
    }
    else if (AFW_VALUE_IS_DATA_TYPE(value, object)) {
        obj = ((const afw_value_object_t *)value)->internal;
    }
    else {
        return;
    }

    for (iterator = NULL;;) {
        pv = afw_object_get_next_property(obj, &iterator, &name, xctx);
        if (!pv) {
            break;
        }
        if (name) {
            cb(name, pv, data, xctx);
        }
    }
}



typedef struct {
    const afw_value_type_t *expected;
    const afw_utf8_z_t *what;
} impl_excess_ctx_t;



static void
impl_excess_prop_cb(
    const afw_utf8_t *name,
    const afw_value_t *prop_value,
    void *data,
    afw_xctx_t *xctx)
{
    impl_excess_ctx_t *ctx = (impl_excess_ctx_t *)data;
    const afw_value_type_t *prop_type;
    afw_boolean_t open_keys;

    if (!impl_object_type_declares_name(ctx->expected, name, xctx)) {
        AFW_THROW_ERROR_FZ(syntax, xctx,
            "Type error in %s: excess property " AFW_UTF8_FMT_Q
            " is not declared on type %s",
            ctx->what ? ctx->what : "assignment",
            AFW_UTF8_FMT_ARG(name),
            impl_type_to_z(ctx->expected, xctx->p, xctx));
    }

    /* Nested object literals against nested object property types. */
    prop_type = impl_object_type_property_type(ctx->expected, name, xctx);
    prop_type = impl_resolve_type(prop_type);
    if (prop_type && prop_type->kind == afw_value_type_kind_object &&
        prop_value &&
        impl_is_object_literal_for_excess(prop_value, &open_keys) &&
        !open_keys)
    {
        impl_excess_ctx_t nested;

        nested.expected = prop_type;
        nested.what = ctx->what;
        impl_foreach_object_literal_prop(prop_value, impl_excess_prop_cb,
            &nested, xctx);
    }
}



static void
impl_compile_check_excess_properties(
    const afw_value_type_t *expected,
    const afw_value_t *value,
    const afw_utf8_z_t *what,
    afw_xctx_t *xctx)
{
    afw_boolean_t open_keys;
    impl_excess_ctx_t ctx;

    expected = impl_resolve_type(expected);
    if (!expected || expected->kind != afw_value_type_kind_object) {
        return;
    }
    if (!value || !impl_is_object_literal_for_excess(value, &open_keys)) {
        return;
    }
    if (open_keys) {
        /* Spread / computed keys: cannot prove excess. */
        return;
    }

    ctx.expected = expected;
    ctx.what = what;
    impl_foreach_object_literal_prop(value, impl_excess_prop_cb, &ctx, xctx);
}



AFW_DEFINE(void)
afw_value_type_check_compile_assignable(
    const afw_value_type_t *expected,
    const afw_value_t *value,
    const afw_utf8_z_t *what,
    afw_xctx_t *xctx)
{
    const afw_data_type_t *got;
    const afw_utf8_z_t *detail;

    if (!afw_value_type_check_compile_enabled(xctx)) {
        return;
    }
    if (afw_value_type_is_any(expected) || !value) {
        return;
    }

    got = afw_value_get_data_type(value, xctx);
    if (!got || got == afw_data_type_any || got == afw_data_type_unknown) {
        /*
         * Unknown / untyped RHS (e.g. bare symbol reference): do not fail
         * compile checks. Excess-property and structural rules apply when
         * the RHS is a known literal or concrete data type.
         */
        return;
    }

    if (!afw_value_type_is_assignable(expected, value, xctx)) {
        detail = impl_mismatch_detail(expected, value, xctx->p, xctx);
        if (detail) {
            AFW_THROW_ERROR_FZ(syntax, xctx,
                "Type error in %s: %s",
                what ? what : "value",
                detail);
        }

        AFW_THROW_ERROR_FZ(syntax, xctx,
            "Type error in %s: expected %s but got %s",
            what ? what : "value",
            impl_type_to_z(expected, xctx->p, xctx),
            afw_utf8_to_utf8_z(&got->data_type_id, xctx->p, xctx));
    }

    /*
     * Additional compile-only rule: reject unknown keys on object literals.
     * Not applied at runtime (open structural for adaptive data).
     */
    impl_compile_check_excess_properties(expected, value, what, xctx);
}
