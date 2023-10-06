// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Value Functions
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_value.c
 * @brief Adaptive Framework Value Functions
 */

#include "afw_internal.h"
#include <libxml/xmlregexp.h>



static const afw_utf8_t impl_s_a_quote = AFW_UTF8_LITERAL("\"");

static const afw_value_null_t
impl_value_null = {
    &afw_value_permanent_null_inf,
    NULL
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_null =
{ (const afw_value_t *)&impl_value_null };

static const afw_value_undefined_t
impl_value_undefined = {
    &afw_value_permanent_undefined_inf,
    NULL
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_undefined =
{ (const afw_value_t *)&impl_value_undefined };

static const afw_value_null_t
impl_value_unique_default_case_value = {
    &afw_value_permanent_null_inf,
    NULL
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_unique_default_case_value =
{ (const afw_value_t *)&impl_value_unique_default_case_value };

static const afw_value_integer_t
impl_value_integer_1 = {
    &afw_value_permanent_integer_inf,
    1
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_integer_1 =
{ (const afw_value_t *)&impl_value_integer_1 };

static const afw_value_string_t
impl_value_empty_string = {
    &afw_value_permanent_string_inf,
    AFW_UTF8_LITERAL("")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_empty_string =
{ (const afw_value_t *)&impl_value_empty_string };

static const afw_value_boolean_t
impl_value_true = {
    &afw_value_permanent_boolean_inf,
    AFW_TRUE
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_true =
{ (const afw_value_t *)&impl_value_true };

static const afw_value_boolean_t
impl_value_false = {
    &afw_value_permanent_boolean_inf,
    AFW_FALSE
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_value_false =
{ (const afw_value_t *)&impl_value_false };


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



/* Return undecorated value. */
AFW_DEFINE(const afw_value_t *)
afw_value_undecorate(const afw_value_t *value)
{
    AFW_VALUE_UNDECORATE(value)

    return value;
}



/* Determine if value's undecorated inf is the supplied one. */
AFW_DEFINE(afw_boolean_t)
afw_value_undecorated_inf_is(
    const afw_value_t *value,
    const afw_value_inf_t *inf)
{
    if (!value) {
        return false;
    }

    if (value->inf == &afw_value_compiled_value_inf) {
        value = ((const afw_value_compiled_value_t *)value)->root_value;
    }

    return value->inf == inf;
}



/* Determine if value and all of it contained values are evaluated. */
AFW_DEFINE(afw_boolean_t)
afw_value_is_fully_evaluated(
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    afw_boolean_t result;
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;
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
    afw_value_evaluated_t *evaluated;

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
        evaluated = afw_value_evaluated_allocate(
            value->inf->is_evaluated_of_data_type, p, xctx);
        afw_data_type_clone_internal(value->inf->is_evaluated_of_data_type,
            (void *)&evaluated->internal,
            (const void *)&((const afw_value_evaluated_t *)value)->internal,
            p, xctx);
        return (const afw_value_t *)evaluated;
    }

    /* If value is not evaluated, evaluate it. */
    return afw_value_evaluate(value, p, xctx);
}



/* Convert value->internal to afw_utf8_z_t * */
AFW_DEFINE(const afw_utf8_z_t *)
afw_value_as_utf8_z(const afw_value_t *value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t *string;
    const afw_utf8_z_t *result;

    result = NULL;
    string = afw_value_as_utf8(value, p, xctx);
    if (string) {
        result = afw_utf8_z_create(string->s, string->len, p, xctx);
    }

    /* Return result or NULL if invalid. */
    return result;
}


/* Convert value to casted utf8 in specified pool. */
AFW_DEFINE(const afw_utf8_t *)
afw_value_as_casted_utf8(
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
        s = afw_value_as_array_of_utf8(value, p, xctx);
        
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
            afw_value_as_utf8(value, p, xctx),
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
    const afw_iterator_t *iterator;
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


/* Return result of afw_value_one_and_only() as string. */
AFW_DEFINE(const afw_utf8_t *)
afw_value_one_and_only_as_utf8(
    const afw_value_t *value, const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_t *v;
    const afw_utf8_t *result;

    value = afw_value_evaluate(value, p, xctx);
    v = afw_value_one_and_only(value, p, xctx);
    result = afw_value_as_utf8(v, p, xctx);
    return result;
}


/* Convert value->internal to afw_utf8_t * */
AFW_DEFINE(const afw_utf8_t *)
afw_value_as_utf8(const afw_value_t *value,
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



/* Make an afw_value_evaluated_t String using string in specified pool. */
AFW_DEFINE(const afw_value_t *)
afw_value_make_single_string(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_string_t *single;

    single = afw_pool_malloc_type(p, afw_value_string_t, xctx);
    single->inf = &afw_value_evaluated_string_inf;
    single->internal.s = s;
    single->internal.len = (len == AFW_UTF8_Z_LEN) ? strlen(s) : len;
    return (const afw_value_t *)single;

}

/* Make an afw_value_evaluated_t String using copy of string in specified pool. */
AFW_DEFINE(const afw_value_t *)
afw_value_make_string_copy(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_string_t *single;

    single = afw_pool_malloc_type(p, afw_value_string_t, xctx);
    single->inf = &afw_value_evaluated_string_inf;
    single->internal.len = (len == AFW_UTF8_Z_LEN) ? strlen(s) : len;
    single->internal.s = (single->internal.len > 0)
        ? afw_memory_dup(s, single->internal.len, p, xctx)
        : NULL;

    return (const afw_value_t *)single;

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
    return afw_value_create_string(string, p, xctx);
}



/* Evaluate a value with additional insecure context. */
AFW_DEFINE(const afw_value_t *)
afw_value_evaluate_with_additional_untrusted_qualified_variables(
    const afw_value_t *value,
    const afw_value_t *untrusted_qualified_variables,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_iterator_t *iterator;
    const afw_object_t *object;
    const afw_utf8_t *property_name;
    const afw_object_t *qualifier_object;
    const afw_value_t *result;
    int top;

    result = NULL;
    if (untrusted_qualified_variables) {
        AFW_VALUE_ASSERT_IS_DATA_TYPE(untrusted_qualified_variables, object, xctx);

        top = afw_xctx_qualifier_stack_top_get(xctx);
        AFW_TRY {

            iterator = NULL;
            while ((object = afw_object_old_get_next_property_as_object(
                ((const afw_value_object_t *)untrusted_qualified_variables)->internal,
                &iterator, &property_name, xctx)))
            {
                qualifier_object = afw_compile_object_all_template_properties(
                    object, NULL, NULL, p, xctx);
                afw_xctx_qualifier_stack_qualifier_object_push(property_name, qualifier_object,
                    false, p, xctx);
            }

            result = afw_value_evaluate(value, p, xctx);
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
    afw_value_evaluated_t *single;
    const afw_array_t *list;
    const afw_iterator_t *iterator;
    const void *internal;
    const afw_data_type_t *data_type;
    afw_size_t evaluate_count;

    /* Evaluate value. */
    result = value;
    AFW_VALUE_UNDECORATE(result);
    for (evaluate_count = 0;
        result && !afw_value_is_defined_and_evaluated(result);
        evaluate_count++)
    {
        if (evaluate_count >= /** @fixme make parameter */ 20) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "afw_value_convert() value required > %d evaluations",
                20);
        }
        result = afw_value_evaluate(result, p, xctx);
        AFW_VALUE_UNDECORATE(result);
    }

    if (!result) {
        if (!required) {
            return NULL;
        }
        AFW_THROW_ERROR_Z(undefined, "Result is undefined", xctx);
    }

    /* If to_data_type is any, return result now. */
    if (!to_data_type || to_data_type == afw_data_type_any) {
        return result;
    }

    v_data_type = afw_value_get_data_type(result, xctx);

    if (v_data_type != to_data_type) {

        /* Upconvert to one entry list. */
        if (to_data_type == afw_data_type_array) {
            list = afw_array_create_wrapper_for_array(
                &((afw_value_evaluated_t *)value)->internal, false,
                v_data_type, 1, p, xctx);
            result = afw_value_create_array(list, p, xctx);
        }

        /* Down convert from a single entry list. */
        else if (v_data_type == afw_data_type_array &&
            !afw_data_type_is_string(to_data_type))
        {
            list = ((const afw_value_array_t *)value)->internal;
            if (afw_array_get_count(list, xctx) != 1) {
                AFW_THROW_ERROR_Z(general,
                    "Can't down convert a list with more than one entry",
                    xctx);
            }
            iterator = NULL;
            afw_array_get_next_internal(list,
                &iterator, &data_type, &internal, xctx);
            single = afw_value_evaluated_allocate(to_data_type, p, xctx);
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
            result = (const afw_value_t *)single;
        }

        /* Not list. */
        else {
            single = afw_value_evaluated_allocate(to_data_type, p, xctx);
            afw_data_type_convert_internal(
                v_data_type,
                &single->internal,
                &((const afw_value_evaluated_t *)value)->internal,
                to_data_type,
                p, xctx);
            result = (const afw_value_t *)single;
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
        AFW_THROW_ERROR_Z(undefined, "Value is undefined", xctx);
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
        result = afw_value_create_string(string, p, xctx);
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
    return afw_value_create_dateTime(&dateTime, p, xctx);
}



/* Create a dateTime value with current local time. */
AFW_DEFINE(const afw_value_t *)
afw_value_create_dateTime_now_local(
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_dateTime_t dateTime;

    afw_dateTime_set_now(&dateTime, NULL, xctx);
    return afw_value_create_dateTime(&dateTime, p, xctx);
}



static const afw_value_t **
impl_value_as_array_of_values(
    const afw_value_t *value, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_size_t count;
    const afw_value_t **e;
    const afw_value_t **result;
    const afw_iterator_t *iterator;

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
afw_value_as_array_of_values(const afw_value_t * value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    return impl_value_as_array_of_values(value, p, xctx);
}



/* Return a NULL terminated list of strings in a specified pool. */
AFW_DEFINE(const afw_utf8_t * const *)
afw_value_as_array_of_utf8(const afw_value_t * value,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_utf8_t **result;
    const afw_utf8_t **e;

    /*
     * Get NULL terminated array for value pointers and replace the pointers
     * in place with the result of calling as_string() on the value. 
     */
    result = (const afw_utf8_t **)impl_value_as_array_of_values(value,
        p, xctx);
    for (e = result; *e; e++) {
        *e = afw_value_as_utf8((const afw_value_t *)*e, p, xctx);
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
            i1 = (const char *)&((const afw_value_evaluated_t *)value1)->internal;
            i2 = (const char *)&((const afw_value_evaluated_t *)value2)->internal;
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


    /* Make sure value1 and value2 are undecorated. */
    AFW_VALUE_UNDECORATE(value1);
    AFW_VALUE_UNDECORATE(value2);

    if (afw_value_quick_data_type(value1) != afw_value_quick_data_type(value2))
    {
        AFW_THROW_ERROR_Z(general, "value 1 and 2 are not same type", xctx);
    }

    if (afw_value_is_defined_and_evaluated(value1)) {
        i1 = (const void *)&((const afw_value_evaluated_t *)value1)->internal;
        i2 = (const void *)&((const afw_value_evaluated_t *)value2)->internal;
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


/* Register core value infs. */
AFW_DEFINE(void)
afw_value_register_core_value_infs(afw_xctx_t *xctx)
{
    afw_environment_register_value_inf(
        &afw_value_assignment_target_inf.rti.implementation_id,
        &afw_value_assignment_target_inf, xctx);

    afw_environment_register_value_inf(
        &afw_value_compiled_value_inf.rti.implementation_id,
        &afw_value_compiled_value_inf, xctx);

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
        &afw_value_reference_by_key_inf.rti.implementation_id,
        &afw_value_reference_by_key_inf, xctx);

    afw_environment_register_value_inf(
        &afw_value_template_definition_inf.rti.implementation_id,
        &afw_value_template_definition_inf, xctx);

    afw_environment_register_value_inf(
        &afw_value_qualified_variable_reference_inf.rti.implementation_id,
        &afw_value_qualified_variable_reference_inf, xctx);
}
