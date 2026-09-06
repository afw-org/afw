// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Value Functions
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_value.c
 * @brief Core value evaluate, clone, and utility functions.
 */

#include "afw_internal.h"
#include <libxml/xmlregexp.h>
#include <math.h>



static const afw_utf8_t impl_s_a_quote = AFW_UTF8_LITERAL("\"");

static const afw_value_null_t
impl_value_null = {
    {&afw_value_permanent_null_inf},
    NULL
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_null =
{ &impl_value_null.pub };

static const afw_value_undefined_t
impl_value_undefined = {
    {&afw_value_permanent_undefined_inf},
    NULL
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_undefined =
{ &impl_value_undefined.pub };


/* NULL-safe get_reference. */
AFW_DEFINE(const afw_value_t *)
afw_value_add_reference(
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    if (!value || !value->inf || !value->inf->get_reference) {
        return value;
    }
    return afw_value_get_reference(value, xctx);
}


/* NULL-safe get_assignable_value. */
AFW_DEFINE(const afw_value_t *)
afw_value_as_assignable(
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    if (!value || !value->inf || !value->inf->get_assignable_value) {
        return value;
    }
    return afw_value_get_assignable_value(value, xctx);
}


/* NULL-safe optional_release. */
AFW_DEFINE(void)
afw_value_release(
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    if (!value || afw_value_is_undefined(value) ||
        !value->inf || !value->inf->optional_release)
    {
        return;
    }
    afw_value_optional_release(value, xctx);
}


/* Assign into a slot. */
AFW_DEFINE(void)
afw_value_slot_store(
    const afw_value_t **slot,
    const afw_value_t *incoming,
    afw_xctx_t *xctx)
{
    const afw_value_t *assignable;
    afw_boolean_t unmanaged_compiled_value;

    if (!incoming) {
        incoming = afw_value_undefined;
    }
    if (*slot == incoming) {
        return;
    }
    /* Isolate incoming before releasing the occupant. Incoming may
     * still point at the occupant's bytes (s = s + s, substring onto
     * self). Release-first lets the pool reuse that block, then
     * create_managed memcpy is dest==src (issue #275).
     */
    unmanaged_compiled_value =
        incoming && incoming->inf == &afw_value_compiled_value_inf;
    assignable = afw_value_as_assignable(incoming, xctx);
    if (*slot == assignable) {
        return;
    }
    /*
     * Unmanaged compiled_value get_assignable_value extra-holds the unit
     * pool and stamps the assignable face (same pointer). Release the
     * original to drop the birth hold.
     */
    if (unmanaged_compiled_value) {
        afw_value_release(incoming, xctx);
    }
    afw_value_release(*slot, xctx);
    *slot = assignable;
}


static const afw_value_void_t
impl_value_void = {
    {&afw_value_permanent_void_inf},
    NULL
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_void =
{ &impl_value_void.pub };

static const afw_value_null_t
impl_value_unique_default_case_value = {
    {&afw_value_permanent_null_inf},
    NULL
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_unique_default_case_value =
{ &impl_value_unique_default_case_value.pub };


static const afw_value_double_t
impl_value_double_max = {
    { &afw_value_permanent_double_inf },
    AFW_DOUBLE_MAX
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_double_max =
{ &impl_value_double_max.pub };


static const afw_value_double_t
impl_value_double_min = {
    { &afw_value_permanent_double_inf },
    AFW_DOUBLE_MIN
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_double_min =
{ &impl_value_double_min.pub };


static const afw_value_double_t
impl_value_double_min_subnormal = {
    { &afw_value_permanent_double_inf },
    AFW_DOUBLE_MIN_SUBNORMAL
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_double_min_subnormal =
{ &impl_value_double_min_subnormal.pub };


static const afw_value_double_t
impl_value_double_epsilon = {
    { &afw_value_permanent_double_inf },
    AFW_DOUBLE_EPSILON
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_double_epsilon =
{ &impl_value_double_epsilon.pub };


static const afw_value_double_t
impl_value_double_pi = {
    { &afw_value_permanent_double_inf },
    AFW_DOUBLE_PI
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_double_pi =
{ &impl_value_double_pi.pub };


static const afw_value_double_t
impl_value_double_e = {
    { &afw_value_permanent_double_inf },
    AFW_DOUBLE_E
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_double_e =
{ &impl_value_double_e.pub };


static const afw_value_double_t
impl_value_infinity = {
    { &afw_value_permanent_double_inf },
    INFINITY
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_infinity =
{ &impl_value_infinity.pub };


static const afw_value_double_t
impl_value_minus_infinity = {
    { &afw_value_permanent_double_inf },
    -INFINITY
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_minus_infinity =
{ &impl_value_minus_infinity.pub };


static const afw_value_double_t
impl_value_NaN = {
    { &afw_value_permanent_double_inf },
    NAN
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_NaN =
{ &impl_value_NaN.pub };


static const afw_value_double_t
impl_value_double_zero = {
    { &afw_value_permanent_double_inf },
    0.0
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_double_zero =
{ &impl_value_double_zero.pub };


static const afw_value_double_t
impl_value_minus_zero = {
    { &afw_value_permanent_double_inf },
    -0.0
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_minus_zero =
{ &impl_value_minus_zero.pub };


static const afw_value_integer_t
impl_value_integer_max = {
    { &afw_value_permanent_integer_inf },
    AFW_INTEGER_MAX
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_integer_max =
{ &impl_value_integer_max.pub };


static const afw_value_integer_t
impl_value_integer_min = {
    { &afw_value_permanent_integer_inf },
    AFW_INTEGER_MIN
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_integer_min =
{ &impl_value_integer_min.pub };


/* Compile a value. */
AFW_DEFINE(const afw_value_t *)
afw_value_compile(
    const afw_value_t *value,
    const afw_utf8_t *source_location,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *evaluated;
    const afw_value_t *compiled_value;
    const afw_data_type_t *data_type;
    const afw_utf8_t *source;

    evaluated = afw_value_evaluate(value, p, xctx);
    data_type = afw_value_get_data_type(evaluated, xctx);
    if (!data_type || data_type->compile_type == afw_compile_type_error)
    {
        AFW_THROW_ERROR_Z(general, "Value can not be compiled", xctx);
    }
    source = &((const afw_value_string_t *)evaluated)->internal;
    compiled_value = afw_compile_to_value_with_callback(
        source, NULL, NULL,
        source_location, data_type->compile_type,
        afw_compile_residual_check_to_full, \
        NULL, NULL, p, xctx);

    return compiled_value;
}


/* Compile a value using specified compile type. */
AFW_DEFINE(const afw_value_t *)
afw_value_compile_as(
    const afw_value_t *value,
    const afw_utf8_t *source_location,
    afw_compile_type_t compile_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *evaluated;
    const afw_value_t *compiled_value;
    const afw_data_type_t *data_type;
    const afw_utf8_t *source;

    /* Compile type must match passed or be string*/
    evaluated = afw_value_evaluate(value, p, xctx);
    data_type = afw_value_get_data_type(evaluated, xctx);
    if (!afw_data_type_is_string(data_type)) {
        if (!data_type || data_type->compile_type != compile_type )
        {
            AFW_THROW_ERROR_Z(general,
                "Value data type does not match compile_type", xctx);
        }
    }

    /* All data types with a compile type must have afw_utf8_t source. */
    source = &((const afw_value_string_t *)evaluated)->internal;

    /* Compile source. */
    compiled_value = afw_compile_to_value_with_callback(
        source, NULL, NULL,
        source_location, compile_type,
        afw_compile_residual_check_to_full, \
        NULL, NULL, p, xctx);

    return compiled_value;
}




/* Compile and evaluate a value. */
AFW_DEFINE(const afw_value_t *)
afw_value_compile_and_evaluate(
    const afw_value_t *value,
    const afw_utf8_t *source_location,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *compiled_value;
    const afw_value_t *result;

    compiled_value = afw_value_compile(value, source_location, p, xctx);
    result = afw_value_evaluate(compiled_value, p, xctx);

    return result;
}




/* Compile and evaluate a value using specified compile type. */
AFW_DEFINE(const afw_value_t *)
afw_value_compile_and_evaluate_as(
    const afw_value_t *value,
    const afw_utf8_t *source_location,
    afw_compile_type_t compile_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *compiled_value;
    const afw_value_t *result;

    compiled_value = afw_value_compile_as(value, source_location, compile_type,
        p, xctx);
    result = afw_value_evaluate(compiled_value, p, xctx);

    return result;
}



/* Determine if value and all of it contained values are evaluated. */
AFW_DEFINE(afw_boolean_t)
afw_value_is_fully_evaluated(
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_boolean_t result;
    const afw_iterator_old_t *iterator;
    const afw_value_t *property_name;
    const afw_value_t *v;

    result = true;

    if (!afw_value_is_defined_and_evaluated(value)) {
        result = false;
    }

    else if (afw_value_is_object(value))
    {
        for (iterator = NULL;;) {
            v = afw_object_get_next_property(
                ((const afw_value_object_t *)value)->internal,
                &iterator, &property_name, xctx);
            if (!v) {
                break;
            }
            if (!afw_value_is_fully_evaluated(v, xctx)) {
                result = false;
                break;
            }
        }
    }

    else if (afw_value_is_array(value))
    {
        for (iterator = NULL;;) {
            v = afw_array_get_next_value(
                ((const afw_value_array_t *)value)->internal,
                &iterator, xctx->p, xctx);
            if (!v) {
                break;
            }
            if (!afw_value_is_fully_evaluated(v, xctx)) {
                result = false;
                break;
            }
        }
    }

    return result;
}


/* Determine if value is scalar. */
AFW_DEFINE(afw_boolean_t)
afw_value_is_scalar(const afw_value_t *value, afw_xctx_t *xctx)
{
    const afw_data_type_t *data_type;

    data_type = afw_value_get_data_type(value, xctx);
    return (data_type) ? data_type->scalar : false;
};


/* Clone a value to specified pool. */
AFW_DEFINE(const afw_value_t *)
afw_value_clone(const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_value_common_t *evaluated;

    /* If value is NULL, return NULL. */
    if (!value) {
        return value;
    }

    /** @fixme Use values clone or reference for this. */
    if (afw_value_is_nullish(value)) {
        return value;
    }

    /* If value is evaluated, clone it. */
    if (value->inf->is_evaluated_of_data_type) {
        /* Independent instance + dual face; not a wrap of the original. */
        if (AFW_VALUE_IS_DATA_TYPE(value, object)) {
            const afw_object_t *from =
                ((const afw_value_object_t *)value)->internal;
            const afw_object_t *to = NULL;

            afw_data_type_clone_internal(afw_data_type_object,
                (void *)&to, &from, p, xctx);
            return afw_object_as_value(to, p, xctx);
        }
        if (AFW_VALUE_IS_DATA_TYPE(value, array)) {
            const afw_array_t *from =
                ((const afw_value_array_t *)value)->internal;
            const afw_array_t *to = NULL;

            afw_data_type_clone_internal(afw_data_type_array,
                (void *)&to, &from, p, xctx);
            return afw_array_as_value(to, p, xctx);
        }
        evaluated = afw_value_common_allocate(
            value->inf->is_evaluated_of_data_type, p, xctx);
        afw_data_type_clone_internal(value->inf->is_evaluated_of_data_type,
            (void *)&evaluated->internal,
            (const void *)&((const afw_value_common_t *)value)->internal,
            p, xctx);
        return &evaluated->pub;
    }

    /* If value is not evaluated, evaluate it. */
    return afw_value_evaluate(value, p, xctx);
}


/* Deep clone an evaluated value unmanaged into dest p. */
AFW_DEFINE(const afw_value_t *)
afw_value_clone_unmanaged(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_data_type_t *dt;

    dt = (value && value->inf) ? value->inf->is_evaluated_of_data_type : NULL;
    if (!dt || !dt->clone_value_unmanaged) {
        AFW_THROW_ERROR_Z(conversion_error,
            "clone_unmanaged requires an evaluated value", xctx);
    }
    return dt->clone_value_unmanaged(value, p, xctx);
}


/* Clone an evaluated value managed in xctx->p. */
AFW_DEFINE(const afw_value_t *)
afw_value_clone_managed(
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_data_type_t *dt;

    dt = (value && value->inf) ? value->inf->is_evaluated_of_data_type : NULL;
    if (!dt || !dt->clone_value_managed) {
        AFW_THROW_ERROR_Z(conversion_error,
            "clone_managed requires an evaluated value", xctx);
    }
    return dt->clone_value_managed(value, xctx);
}



/* Unmanaged/permanent object clone_or_reference: face overlay, do not wrap a face. */
AFW_DEFINE(const afw_value_t *)
afw_value_object_hold(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *obj;
    const afw_object_t *face;

    if (!value) {
        return value;
    }
    obj = ((const afw_value_object_t *)value)->internal;
    if (!obj) {
        return value;
    }
    /* Slot must not store unmanaged_* inf. Prefer managed dual-face
     * or the bag's assignable dual-face (wrappers already have it). */
    if (afw_object_is_memory_managed(obj)) {
        afw_object_get_reference(obj, xctx);
        return obj->value;
    }
    if (value->inf == &afw_value_assignable_object_inf) {
        afw_object_get_reference(obj, xctx);
        return value;
    }
    if (obj->value &&
        obj->value->inf == &afw_value_assignable_object_inf)
    {
        afw_object_get_reference(obj, xctx);
        return obj->value;
    }
    if (!p) {
        p = xctx->p;
    }
    face = afw_object_create_wrapper_unmanaged(obj, p, xctx);
    afw_object_get_reference(face, xctx);
    return face->value;
}


/* Unmanaged/permanent array clone_or_reference: face overlay, do not wrap a face. */
AFW_DEFINE(const afw_value_t *)
afw_value_array_hold(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_array_t *arr;
    const afw_array_t *face;

    if (!value) {
        return value;
    }
    arr = ((const afw_value_array_t *)value)->internal;
    if (!arr) {
        return value;
    }
    if (afw_array_is_memory_managed(arr)) {
        afw_array_get_reference(arr, xctx);
        return arr->value;
    }
    if (value->inf == &afw_value_assignable_array_inf) {
        afw_array_get_reference(arr, xctx);
        return value;
    }
    if (arr->value &&
        arr->value->inf == &afw_value_assignable_array_inf)
    {
        afw_array_get_reference(arr, xctx);
        return arr->value;
    }
    if (!p) {
        p = xctx->p;
    }
    /*
     * Custom immutable views (metas, const arrays): no setter and not a
     * memory array. Wrapping would materialize a mutable overlay.
     * Pin with assignable inf so a slot can release without throw.
     */
    if (!afw_array_is_memory(arr) &&
        !afw_array_get_setter(arr, xctx))
    {
        afw_value_array_t *v;

        v = afw_pool_calloc(p, sizeof(afw_value_array_t), xctx);
        v->inf = &afw_value_assignable_array_inf;
        v->internal = arr;
        afw_array_get_reference(arr, xctx);
        return &v->pub;
    }
    face = afw_array_create_wrapper_unmanaged(arr, p, xctx);
    afw_array_get_reference(face, xctx);
    return face->value;
}


/* Convert any evaluated value to NUL-terminated utf8. */
AFW_DEFINE(const afw_utf8_z_t *)
afw_value_convert_to_utf8_z(const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t *string;
    const afw_utf8_z_t *result;

    result = NULL;
    string = afw_value_convert_to_utf8(value, p, xctx);
    if (string) {
        result = afw_utf8_z_create(string->s, string->len, p, xctx);
    }

    /* Return result or NULL if invalid. */
    return result;
}


/* Convert value to datatype("...") utf8 in specified pool. */
AFW_DEFINE(const afw_utf8_t *)
afw_value_convert_to_casted_utf8(
    const afw_value_t *value, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t *result;
    const afw_data_type_t *data_type;
    const afw_utf8_t * const *s;
    afw_size_t n;
    afw_size_t len;
    afw_utf8_octet_t *c;

    value = afw_value_evaluate(value, p, xctx);
    if (!value) {
        return NULL;
    }
    data_type = afw_value_get_data_type(value, xctx);

    /** @fixme change from bag to list when tests are modified. */
    if (afw_value_is_array(value)) {
        s = afw_value_convert_to_null_terminated_utf8(value, p, xctx);
        
        if (!s[0]) {
            len = 5; /* "bag()" */
        }
        else {
            for (n = 0, len = 3 /* "bag" */; s[n]; n++) {
                /*
                 * '<datatype>("<string value>") plus surrounding '()' for bag
                 * or ', '
                 */
                len += s[n]->len + 6 + data_type->data_type_id.len;
            }
        }

        c = afw_pool_calloc(p, len, xctx);
        result = afw_utf8_create(c, len, p, xctx);

        memcpy(c, "bag(", 4);
        c += 4;

        for (n = 0; s[n]; n++) {
            if (n != 0) {
                *c++ = ',';
                *c++ = ' ';
            }
            memcpy(c, data_type->data_type_id.s,
                data_type->data_type_id.len);
            c += data_type->data_type_id.len;
            *c++ = '(';
            *c++ = '\"';
            memcpy(c, s[n]->s, s[n]->len);
            c += s[n]->len;
            *c++ = '\"';
            *c++ = ')';
        }

        *c++ = ')';
    }

    else {
        result = afw_utf8_concat(p, xctx,
            &data_type->data_type_id,
            afw_s_a_open_parenthesis, &impl_s_a_quote,
            afw_value_convert_to_utf8(value, p, xctx),
            &impl_s_a_quote, afw_s_a_close_parenthesis,
            NULL);
    }

    return result;
}


/* Return single value from one entry list or single value. */
AFW_DEFINE(const afw_value_t *)
afw_value_one_and_only(
    const afw_value_t *value, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_iterator_old_t *iterator;
    const afw_array_t *list;
    const afw_value_t *result;

    /* Result is NULL if value is NULL.  */
    if (!value) {
        return NULL;
    }
    result = NULL;
    value = afw_value_evaluate(value, p, xctx);

    /* If list, use it's first value if there is exactly one entry. */
    if (afw_value_is_array(value)) {
        iterator = NULL;
        list = ((const afw_value_array_t *)value)->internal;
        result = afw_array_get_next_value(list, &iterator, p, xctx);
        if (result) {
            if (afw_array_get_next_value(list, &iterator, p, xctx)) {
                result = NULL;
            }
        }
    }
    /* Else result is just value. */
    else {
        result = value;
    }

    return result;
}


/* Convert afw_value_one_and_only() to utf8. */
AFW_DEFINE(const afw_utf8_t *)
afw_value_one_and_only_convert_to_utf8(
    const afw_value_t *value, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_t *v;
    const afw_utf8_t *result;

    value = afw_value_evaluate(value, p, xctx);
    v = afw_value_one_and_only(value, p, xctx);
    result = afw_value_convert_to_utf8(v, p, xctx);
    return result;
}


/* Convert any evaluated value to utf8. */
AFW_DEFINE(const afw_utf8_t *)
afw_value_convert_to_utf8(const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t *result;
    const afw_data_type_t *data_type;

    /* Result is NULL if value is NULL.  */
    if (!value) {
        return NULL;
    }
    value = afw_value_evaluate(value, p, xctx);
    data_type = afw_value_get_data_type(value, xctx);
    if (!data_type) {
        AFW_THROW_ERROR_Z(general, "Expecting data type", xctx);
    }
    result = afw_data_type_internal_to_utf8(
        data_type, AFW_VALUE_INTERNAL(value), p, xctx);
    return result;
}



/*
 * Create Adaptive value from untrusted external octets: string if valid
 * UTF-8 (NFC), otherwise hexBinary with a copy of the same bytes.
 */
AFW_DEFINE(const afw_value_t *)
afw_value_create_from_external_octets(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *string;
    afw_memory_t memory;
    const afw_byte_t *copy;

    if (len == AFW_UTF8_Z_LEN) {
        len = (s) ? strlen(s) : 0;
    }

    /* Empty or NULL → empty string (not hexBinary). */
    if (!s || len == 0) {
        return afw_v_a_empty_string;
    }

    /* Valid UTF-8 → NFC string value. */
    if (afw_utf8_is_valid(s, len, xctx)) {
        string = afw_utf8_create(s, len, p, xctx);
        return afw_value_create_unmanaged_string(string, p, xctx);
    }

    /* Invalid UTF-8 → hexBinary with owned copy of the same bytes. */
    copy = afw_memory_dup(s, len, p, xctx);
    memory.ptr = copy;
    memory.size = len;
    return afw_value_create_unmanaged_hexBinary(&memory, p, xctx);
}


/* NUL-terminated convenience wrapper. */
AFW_DEFINE(const afw_value_t *)
afw_value_create_from_external_z(
    const char *s_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return afw_value_create_from_external_octets(
        (const afw_utf8_octet_t *)s_z, AFW_UTF8_Z_LEN, p, xctx);
}


/* Make an afw_value_string_t from in specified pool. */
AFW_DEFINE(const afw_value_t *)
afw_value_create_string_from_u8z(
    const afw_utf8_z_t *string_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *string;

    string = afw_utf8_create(string_z, AFW_UTF8_Z_LEN, p, xctx);
    return afw_value_create_unmanaged_string(string, p, xctx);
}



/* Evaluate a value with additional insecure context. */
AFW_DEFINE(const afw_value_t *)
afw_value_evaluate_with_additional_untrusted_qualified_variables(
    const afw_value_t *value,
    const afw_value_t *untrusted_qualified_variables,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_iterator_old_t *iterator;
    const afw_object_t *object;
    const afw_value_t *property_name;
    const afw_object_t *qualifier_object;
    const afw_value_t *result;
    int top;

    result = NULL;
    if (untrusted_qualified_variables) {
        /*
         * Second argument of evaluate() may be wrap_literal_object(...) or
         * other unevaluated form (issue #17). Evaluate to a finished object
         * before cast / property walk.
         */
        untrusted_qualified_variables = afw_value_evaluate(
            untrusted_qualified_variables, p, xctx);
        AFW_VALUE_ASSERT_IS_DATA_TYPE(untrusted_qualified_variables, object,
            xctx);

        top = afw_xctx_qualifier_stack_top_get(xctx);
        AFW_TRY {

            iterator = NULL;
            while ((object = afw_object_get_next_property_as_object_internal(
                ((const afw_value_object_t *)untrusted_qualified_variables)->internal,
                &iterator, &property_name, p, xctx)))
            {
                qualifier_object = afw_compile_object_all_template_properties(
                    object, NULL, NULL, p, xctx);
                afw_xctx_qualifier_stack_qualifier_object_push(
                    afw_object_string_property_name_as_utf8(
                        property_name, xctx),
                    qualifier_object,
                    false, p, xctx);
            }

            /* Fully evaluate while untrusted frames are still on the stack. */
            result = afw_value_evaluate(value, p, xctx);
            while (result && afw_value_is_compiled_value(result)) {
                result = afw_value_evaluate(result, p, xctx);
            }
        }

        AFW_FINALLY{
            afw_xctx_qualifier_stack_top_set(top, xctx);
        }

        AFW_ENDTRY;
    }

    else {
        result = afw_value_evaluate(value, p, xctx);
    }

    return result;
}


/* Convert a value to a value/data type. */
AFW_DEFINE(const afw_value_t *)
afw_value_convert(
    const afw_value_t *value,
    const afw_data_type_t *to_data_type,
    afw_boolean_t required,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_t *result;
    const afw_data_type_t *v_data_type;
    afw_value_common_t *single;
    const afw_array_t *list;
    const afw_iterator_old_t *iterator;
    const void *internal;
    const afw_data_type_t *data_type;
    afw_size_t evaluate_count;

    /* Evaluate value. */
    result = value;
    for (evaluate_count = 0;
        result && result->inf->optional_evaluate;
        evaluate_count++)
    {
        if (evaluate_count >= /** @fixme make parameter */ 20) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "afw_value_convert() value required > %d evaluations",
                20);
        }
        result = afw_value_evaluate(result, p, xctx);
    }

    if (!result) {
        if (!required) {
            return NULL;
        }
        AFW_THROW_ERROR_Z(undefined_value, "Result is undefined", xctx);
    }

    /* If to_data_type is any, return result now. */
    if (!to_data_type || to_data_type == afw_data_type_any) {
        return result;
    }

    v_data_type = afw_value_get_data_type(result, xctx);

    if (v_data_type != to_data_type) {

        /* Upconvert to one entry list. */
        if (to_data_type == afw_data_type_array) {
            list = afw_array_create_unmanaged_from_c_array(
                &((afw_value_common_t *)result)->internal, false,
                v_data_type, 1, p, xctx);
            result = afw_value_create_unmanaged_array(list, p, xctx);
        }

        /* Down convert from a single entry list. */
        else if (v_data_type == afw_data_type_array &&
            !afw_data_type_is_string(to_data_type))
        {
            list = ((const afw_value_array_t *)result)->internal;
            if (afw_array_get_count(list, xctx) != 1) {
                AFW_THROW_ERROR_Z(conversion_error,
                    "Can't down convert an array with more than one entry",
                    xctx);
            }
            iterator = NULL;
            afw_array_get_next_internal(list,
                &iterator, &data_type, &internal, xctx);
            single = afw_value_common_allocate(to_data_type, p, xctx);
            if (data_type == to_data_type) {
                memcpy(AFW_VALUE_INTERNAL(single), internal, data_type->c_type_size);
            }
            else {
                afw_data_type_convert_internal(
                    data_type,
                    &single->internal,
                    internal,
                    to_data_type,
                    p, xctx);
            }
            result = &single->pub;
        }

        /* Not list. */
        else {
            single = afw_value_common_allocate(to_data_type, p, xctx);
            afw_data_type_convert_internal(
                v_data_type,
                &single->internal,
                &((const afw_value_common_t *)result)->internal,
                to_data_type,
                p, xctx);
            result = &single->pub;
        }
    }

    /* Return either original or converted value. */
    return result;
}



/* Convert a value to a string value. */
AFW_DEFINE(const afw_value_t *)
afw_value_convert_to_string(
    const afw_value_t *value,
    afw_boolean_t allow_undefined,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_t *result;

    result = afw_value_convert(value, afw_data_type_string, false, p, xctx);
    if (!result) {
        if (allow_undefined) {
            return afw_v_undefined;
        }
        AFW_THROW_ERROR_Z(undefined_value, "Value is undefined", xctx);
    }

    return result;
}



/** Convert internal value to single_string value. */
AFW_DEFINE(const afw_value_t *)
afw_value_string_from_internal(
    const afw_value_t *value, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t *string;
    const afw_value_t *result;

    /* If already String, just return it. */
    if (afw_value_is_string(value)) {
        result = value;
    }

    /* If not, convert value to string and return single_string value. */
    else {
        string = afw_data_type_internal_to_utf8(
            afw_value_get_data_type(value, xctx), value,
            p, xctx);
        result = afw_value_create_unmanaged_string(string, p, xctx);
    }

    return result;
}



/* Create a dateTime value with current time. */
AFW_DEFINE(const afw_value_t *)
afw_value_create_dateTime_now_utc(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_dateTime_t dateTime;

    afw_dateTime_set_now(NULL, &dateTime, xctx);
    return afw_value_create_unmanaged_dateTime(&dateTime, p, xctx);
}



/* Create a dateTime value with current local time. */
AFW_DEFINE(const afw_value_t *)
afw_value_create_dateTime_now_local(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_dateTime_t dateTime;

    afw_dateTime_set_now(&dateTime, NULL, xctx);
    return afw_value_create_unmanaged_dateTime(&dateTime, p, xctx);
}



static const afw_value_t **
impl_value_to_null_terminated_values(
    const afw_value_t *value, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_size_t count;
    const afw_value_t **e;
    const afw_value_t **result;
    const afw_iterator_old_t *iterator;

    /* If value is NULL, n is 0. */
    if (!value) {
        count = 0;
    }

    /* If value is a list, count is number of entries in list. */
    else if (afw_value_is_array(value)) {
        count = afw_array_get_count(
            ((const afw_value_array_t *)value)->internal, xctx);
    }

    /* For other types of values, count is 1. */
    else {
        count = 1;
    }

    /* Allocate storage for result array with n + 1 entries. */
    e = result = afw_pool_malloc(p, sizeof(afw_value_t *)*(count + 1), xctx);
    e[count] = NULL;

    /* Return now if count is 0. */
    if (count == 0) {
        return result;
    }

    /* If value is a list, added list values. */
    else if (afw_value_is_array(value)) {
        for (iterator = NULL; ; e++) {
            *e = afw_array_get_next_value(
                ((const afw_value_array_t *)value)->internal,
                &iterator, p, xctx);
            if (!*e) break;
        }
    }

    /* For other types of values, just add them. */
    else {
        *e = value;
    }

    /* Return result. */
    return result;
}



/* Return a NULL terminated list of values in a specified pool. */
AFW_DEFINE(const afw_value_t * const *)
afw_value_to_null_terminated_values(const afw_value_t * value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    return impl_value_to_null_terminated_values(value, p, xctx);
}


/*
 * Array formal / HOF choke: array as-is; non-array keyless iterator
 * (utf8 code-point sequence) materializes to a temporary array (#153).
 */
AFW_DEFINE(const afw_value_t *)
afw_value_convert_to_array_sequence(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_data_type_t *dt;
    const afw_data_type_t *element_dt;
    const afw_array_t *array;
    const afw_value_t *v;
    afw_iterator_t iterator;

    if (!value || afw_value_is_array(value)) {
        return value;
    }
    if (!afw_value_has_iterator(value)) {
        return value;
    }

    dt = value->inf->is_evaluated_of_data_type;
    element_dt = dt ? dt->iterator_return_data_type : NULL;
    if (element_dt) {
        array = afw_array_create_unmanaged_of(element_dt, p, xctx);
    }
    else {
        array = afw_array_create_unmanaged(p, xctx);
    }

    afw_value_initialize_iterator(value, &iterator, xctx);
    while ((v = afw_iterator_get_next(&iterator, p, xctx)) != NULL) {
        afw_array_push_value(array, v, xctx);
    }

    return afw_value_create_unmanaged_array(array, p, xctx);
}



/* Return a NULL terminated list of strings in a specified pool. */
AFW_DEFINE(const afw_utf8_t * const *)
afw_value_convert_to_null_terminated_utf8(const afw_value_t * value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t **result;
    const afw_utf8_t **e;

    /*
     * Get NULL terminated array for value pointers and replace the pointers
     * in place with convert_to_utf8 of each value. 
     */
    result = (const afw_utf8_t **)impl_value_to_null_terminated_values(value,
        p, xctx);
    for (e = result; *e; e++) {
        *e = afw_value_convert_to_utf8((const afw_value_t *)*e, p, xctx);
    }

    /* Return result. */
    return result;
}



/* Test whether two values are equal. */
AFW_DEFINE(afw_boolean_t)
afw_value_equal(const afw_value_t *value1, const afw_value_t *value2,
    afw_xctx_t *xctx)
{
    const char *i1, *i2;
    afw_boolean_t result;

    if (value1 == value2)
    {
        result = true;
    }

    else if (afw_value_is_undefined(value1))
    {
        if (afw_value_is_undefined(value2))
        {
            result = true;
        }
        else
        {
            result = false;
        }
    }

    else if (afw_value_is_undefined(value2)) {
        result = false;
    }

    else if (value1->inf->is_evaluated_of_data_type)
    {
        if (value1->inf->is_evaluated_of_data_type ==
            value2->inf->is_evaluated_of_data_type)
        {
            i1 = (const char *)&((const afw_value_common_t *)value1)->internal;
            i2 = (const char *)&((const afw_value_common_t *)value2)->internal;
            result = afw_data_type_compare_internal(
                value1->inf->is_evaluated_of_data_type, i1, i2, xctx) == 0;
        }
        else
        {
            result = false;
        }
    }

    else
    {
        result = false;
    }

    return result;
}

/* Compare two values. */
AFW_DEFINE(int)
afw_value_compare(
    const afw_value_t *value1, const afw_value_t *value2,
    afw_xctx_t *xctx)
{
    int result;
    const void *i1, *i2;

    if (afw_value_quick_data_type(value1) != afw_value_quick_data_type(value2))
    {
        AFW_THROW_ERROR_Z(general, "value 1 and 2 are not same type", xctx);
    }

    if (afw_value_is_defined_and_evaluated(value1)) {
        i1 = (const void *)&((const afw_value_common_t *)value1)->internal;
        i2 = (const void *)&((const afw_value_common_t *)value2)->internal;
        result = afw_data_type_compare_internal(
            afw_value_get_data_type(value1, xctx),
            i1, i2, xctx);
    }

    else {
        AFW_THROW_ERROR_Z(general,
            "afw_value_compare() only supports evaluated values",
            xctx);
    }

    return result;
}



/* Check to see if a value contains a substring. */
AFW_DEFINE(afw_boolean_t)
afw_value_contains(
    const afw_value_t *value,
    const afw_value_t *substring,
    afw_xctx_t *xctx)
{
    const afw_data_type_t *value_data_type;
    const afw_data_type_t *substring_data_type;
    const afw_utf8_t *v;
    const afw_utf8_t *ss;
    afw_size_t i;
    afw_boolean_t result;

    value_data_type = afw_value_get_data_type(value, xctx);
    if (!value_data_type ||
        !afw_utf8_equal(&value_data_type->cType, afw_s_afw_utf8_t))
    {
        AFW_THROW_ERROR_Z(general,
            "value must have a data type with cType of afw_utf8_t",
            xctx);
    }

    substring_data_type = afw_value_get_data_type(substring, xctx);
    if (!substring_data_type ||
        !afw_utf8_equal(&substring_data_type->cType, afw_s_afw_utf8_t))
    {
        AFW_THROW_ERROR_Z(general,
            "substring must have a data type with cType of afw_utf8_t",
            xctx);
    }

    v = &((const afw_value_string_t *)value)->internal;
    ss = &((const afw_value_string_t *)substring)->internal;

    for (result = false, i = 0; v->len - i >= ss->len; i++) {
        if (memcmp(v->s + i, ss->s, ss->len) == 0) {
            result = true;
            break;
        }
    }

    return result;
}


/* Register core value infs (libafw bootstrap; see afw_value_internal.h). */
void
afw_value_register_core_value_infs(afw_xctx_t *xctx)
{
    afw_environment_register_value_inf(
        &afw_value_assignment_target_inf.rti.implementation_id,
        &afw_value_assignment_target_inf, xctx);

    afw_environment_register_value_inf(
        &afw_value_script_type_declaration_inf.rti.implementation_id,
        &afw_value_script_type_declaration_inf, xctx);

    afw_environment_register_value_inf(
        &afw_value_compiled_value_inf.rti.implementation_id,
        &afw_value_compiled_value_inf, xctx);

    afw_environment_register_value_inf(
        &afw_value_compiled_value_assignable_inf.rti.implementation_id,
        &afw_value_compiled_value_assignable_inf, xctx);

    afw_environment_register_value_inf(
        &afw_value_call_inf.rti.implementation_id,
        &afw_value_call_inf, xctx);

    afw_environment_register_value_inf(
        &afw_value_function_definition_inf.rti.implementation_id,
        &afw_value_function_definition_inf, xctx);

    afw_environment_register_value_inf(
        &afw_value_script_function_definition_inf.rti.implementation_id,
        &afw_value_script_function_definition_inf, xctx);

    afw_environment_register_value_inf(
        &afw_value_list_expression_inf.rti.implementation_id,
        &afw_value_list_expression_inf, xctx);

    afw_environment_register_value_inf(
        &afw_value_object_expression_inf.rti.implementation_id,
        &afw_value_object_expression_inf, xctx);

    afw_environment_register_value_inf(
        &afw_value_object_construct_inf.rti.implementation_id,
        &afw_value_object_construct_inf, xctx);

    afw_environment_register_value_inf(
        &afw_value_reference_by_key_inf.rti.implementation_id,
        &afw_value_reference_by_key_inf, xctx);

    afw_environment_register_value_inf(
        &afw_value_template_definition_inf.rti.implementation_id,
        &afw_value_template_definition_inf, xctx);

    afw_environment_register_value_inf(
        &afw_value_qualified_variable_reference_inf.rti.implementation_id,
        &afw_value_qualified_variable_reference_inf, xctx);
}
