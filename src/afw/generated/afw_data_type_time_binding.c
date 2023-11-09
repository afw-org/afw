// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Core Data Types 
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/*  ----------------------------- N O T E -------------------------------------
 *
 * This file is generated by "afwdev generate afw".
 *
 * Do not change this file directly or the changes will be lost the next time
 * this file is regenerated.
 *
 * -------------------------------------------------------------------------*/



/**
 * @file afw_data_type_time_binding.c
 * @brief Adaptive Framework core data types.
 */

#include "afw.h"
#include "afw_runtime_object_maps.h"
#include "afw_value_internal.h"

/* Declaration for method optional_release for referenced value. */
AFW_DECLARE_STATIC(void)
impl_afw_value_referenced_optional_release(
    const afw_value_t *instance,
    afw_xctx_t *xctx);


/* Declaration for method get_reference for unmanaged value. */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_value_get_reference(
    const afw_value_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/* Declaration for method get_reference for referenced value. */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_value_referenced_get_reference(
    const afw_value_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/* Declaration for method get_reference for permanent value. */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_value_permanent_get_reference(
    const afw_value_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

/* This is fully evaluated so optional_evaluate method is NULL. */
#define impl_afw_value_optional_evaluate NULL

/* Inf specific is always data type. */
#define AFW_IMPLEMENTATION_SPECIFIC (const void *)&afw_data_type_time_direct

/* Define inf variables for data_type and is_evaluated_of_data_type. */
#define AFW_IMPLEMENTATION_INF_VARIABLES \
    (const void *)&afw_data_type_time_direct, \
    (const void *)&afw_data_type_time_direct

/* Declares and rti/inf defines for interface afw_value */
/* This is the inf for unmanaged time values. For this one */
/* optional_release is NULL and get_reference returns new reference. */
#define AFW_IMPLEMENTATION_ID "time"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_time_inf
#define impl_afw_value_optional_release NULL
#define impl_afw_value_clone_or_reference impl_afw_value_get_reference
#define impl_afw_value_create_iterator NULL
#include "afw_value_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef impl_afw_value_optional_release
#undef impl_afw_value_clone_or_reference

/* Declares and rti/inf defines for interface afw_value */
/* This is the inf for referenced time values. For this one */
/* optional_release releases value and get_reference returns new reference. */
#define AFW_IMPLEMENTATION_ID "referenced_time"
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_referenced_time_inf
#define impl_afw_value_optional_release impl_afw_value_referenced_optional_release
#define impl_afw_value_clone_or_reference impl_afw_value_referenced_get_reference
#define AFW_VALUE_INF_ONLY 1
#include "afw_value_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef impl_afw_value_optional_release
#undef impl_afw_value_clone_or_reference
#undef AFW_VALUE_INF_ONLY

/* Declares and rti/inf defines for interface afw_value */
/* This is the inf for permanent time values. For this one */
/* optional_release is NULL and get_reference returns instance asis. */
#define AFW_IMPLEMENTATION_ID "permanent_time"
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_permanent_time_inf
#define impl_afw_value_optional_release NULL
#define impl_afw_value_clone_or_reference impl_afw_value_permanent_get_reference
#define AFW_VALUE_INF_ONLY 1
#include "afw_value_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef impl_afw_value_optional_release
#undef impl_afw_value_clone_or_reference
#undef AFW_VALUE_INF_ONLY

static const afw_value_string_t
impl_data_type_time_id_value = {
    {&afw_value_permanent_string_inf},
    AFW_UTF8_LITERAL("time")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_data_type_time_id_value =
{
    (const afw_value_t *)&impl_data_type_time_id_value
};

static const afw_utf8_t
impl_data_type_path_time =
    AFW_UTF8_LITERAL("/afw/_AdaptiveDataType_/time");

/* Data type time object. */
static const afw_runtime_object_indirect_t
impl_data_type_object_time = {
    {
        &afw_runtime_inf__AdaptiveDataType_,
        NULL,
        {
            NULL,
            NULL,
            &afw_self_s_time,
            &afw_self_s__AdaptiveDataType_,
            &impl_data_type_path_time
        }
    },
    (void *)&afw_data_type_time_direct
};

/* Value for empty array of time. */
AFW_DEFINE_INTERNAL_CONST_DATA(afw_array_wrapper_for_array_self_t)
impl_empty_array_of_time;

/* Value for empty array of time. */
AFW_DEFINE_INTERNAL_CONST_DATA(afw_value_array_t)
impl_value_empty_array_of_time;

/* Data type time instance. */
AFW_DEFINE_INTERNAL_CONST_DATA(afw_data_type_t)
afw_data_type_time_direct = {
    &afw_data_type_time_inf,
    (const afw_object_t *)&impl_data_type_object_time,
    AFW_UTF8_LITERAL("time"),
    AFW_UTF8_LITERAL("A time"),
    AFW_UTF8_LITERAL("Data type 'time' is ISO 8601 extended format hh:mm:ss with an optional time zone."),
    AFW_UTF8_LITERAL(""),
    afw_data_type_number_time,
    AFW_UTF8_LITERAL("1.3.6.1.4.1.1466.115.121.1.24"),
    AFW_UTF8_LITERAL("string"),
    AFW_UTF8_LITERAL(""),
    AFW_UTF8_LITERAL("afw_time_t"),
    sizeof(afw_time_t),
    (const afw_array_t *)&impl_empty_array_of_time,
    (const afw_value_t *)&impl_value_empty_array_of_time,
    &afw_value_time_inf,
    afw_compile_type_error,
    false,
    false,
    false,
    true,
    true,
    false
};

/* Value for empty array of time. */
AFW_DEFINE_INTERNAL_CONST_DATA(afw_array_wrapper_for_array_self_t)
impl_empty_array_of_time = {
    &afw_array_wrapper_for_array_inf,
    &afw_data_type_time_direct,
    0
};

/* Value for empty array of time. */
AFW_DEFINE_INTERNAL_CONST_DATA(afw_value_array_t)
impl_value_empty_array_of_time = {
    {&afw_value_permanent_array_inf},
    (const afw_array_t *)&impl_empty_array_of_time
};

/* Data type struct for time. */
AFW_DEFINE_CONST_DATA(afw_data_type_t *)
afw_data_type_time =
    &afw_data_type_time_direct;

/* Set property function for data type time values. */
AFW_DEFINE(void)
afw_object_set_property_as_time(
    const afw_object_t *object,
    const afw_utf8_t *property_name,
    const afw_time_t * internal,
    afw_xctx_t *xctx)
{
    const afw_value_t *v;

    if (!object->p) {
        AFW_THROW_ERROR_Z(general,
            "Object must have a pool",
            xctx);
    }

    v = afw_value_create_time(internal, object->p, xctx);
    afw_object_set_property(object, property_name, v, xctx);
}

/* Typesafe cast of data type time. */
AFW_DEFINE(const afw_time_t *)
afw_value_as_time(const afw_value_t *value, afw_xctx_t *xctx)
{
    value = afw_value_evaluate(value, xctx->p, xctx);
    if (!AFW_VALUE_IS_DATA_TYPE(value, time))
    {
        const afw_utf8_t *data_type_id;

        if (!value) {
            AFW_THROW_ERROR_Z(general,
                "Typesafe error: expecting 'time' but "
                "encountered 'undefined'",
                xctx);
        }

        data_type_id = afw_value_get_quick_data_type_id(value);
        AFW_THROW_ERROR_FZ(general, xctx,
            "Typesafe error: expecting 'time' but "
            "encountered " AFW_UTF8_FMT_Q ,
            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));
    }
    return &(((const afw_value_time_t *)value)->internal);
}

/* Allocate function for unmanaged data type time values. */
AFW_DEFINE(afw_value_time_t *)
afw_value_allocate_time(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_value_time_t *result;

    result = afw_pool_calloc(p, sizeof(afw_value_time_t),
        xctx);
    result->inf = &afw_value_time_inf;
    return result;
}

/* Create function for referenced data type time value. */
AFW_DEFINE(const afw_value_t *)
afw_value_create_referenced_time(const afw_time_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_value_time_t *v;

    v = afw_pool_calloc(p, sizeof(afw_value_time_t),
        xctx);
    v->inf = &afw_value_referenced_time_inf;
    if (internal) {
        memcpy(&v->internal, internal, sizeof(afw_time_t));
    }
    return &v->pub;
}

/* Create function for unmanaged data type time value. */
AFW_DEFINE(const afw_value_t *)
afw_value_create_time(const afw_time_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_value_time_t *v;

    v = afw_pool_calloc(p, sizeof(afw_value_time_t),
        xctx);
    v->inf = &afw_value_time_inf;
    if (internal) {
        memcpy(&v->internal, internal, sizeof(afw_time_t));
    }
    return &v->pub;
}

/* Convert data type time string to afw_time_t *. */
AFW_DEFINE(void)
afw_data_type_time_to_internal(afw_time_t *to_internal,
    const afw_utf8_t *from_utf8, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_data_type_utf8_to_internal(
        &afw_data_type_time_direct,
        (void *)to_internal, from_utf8, p, xctx);
}

/*  Convert data type time internal representation to utf-8. */
AFW_DEFINE(const afw_utf8_t *)
afw_data_type_time_to_utf8(const afw_time_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    return afw_data_type_internal_to_utf8(
        &afw_data_type_time_direct,
        internal, p, xctx);
}

/* Get property function for data type time values. */
AFW_DEFINE(const afw_time_t *)
afw_object_get_property_as_time_source(
    const afw_object_t *object,
    const afw_utf8_t *property_name,
    const afw_utf8_z_t *source_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    value = afw_object_get_property(object, property_name, xctx);
    if (!value) {
        return NULL;
    }

    value = afw_value_evaluate(value, p, xctx);
    if (!AFW_VALUE_IS_DATA_TYPE(value, time))
    {
        const afw_utf8_t *data_type_id;

        data_type_id = afw_value_get_quick_data_type_id(value);
        afw_error_set_fz(afw_error_code_general, source_z, xctx,
            "Typesafe error: expecting 'time' but "
            "encountered " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));
        longjmp(((xctx)->current_try->throw_jmp_buf), afw_error_code_general);
    }
    return &(((const afw_value_time_t *)value)->internal);
}

/* Get next property function for data type time values. */
AFW_DEFINE(const afw_time_t *)
afw_object_get_next_property_as_time_source(
    const afw_object_t *object,
    const afw_iterator_t * *iterator,
    const afw_utf8_t * *property_name,
    const afw_utf8_z_t *source_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    value = afw_object_get_next_property(object, iterator, property_name, xctx);
    if (!value) {
        return NULL;
    }

    value = afw_value_evaluate(value, p, xctx);
    if (!AFW_VALUE_IS_DATA_TYPE(value, time))
    {
        const afw_utf8_t *data_type_id;

        data_type_id = afw_value_get_quick_data_type_id(value);
        afw_error_set_fz(afw_error_code_general, source_z, xctx,
            "Typesafe error: expecting 'time' but "
            "encountered " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));
        longjmp(((xctx)->current_try->throw_jmp_buf), afw_error_code_general);
    }
    return &(((const afw_value_time_t *)value)->internal);
}

/* Implementation of method optional_release for referenced value. */
AFW_DECLARE_STATIC(void)
impl_afw_value_referenced_optional_release(
    const afw_value_t *instance,
    afw_xctx_t *xctx)
{
    /** @todo this needs to release reference of value or free it. */
}

/* Implementation of method get_reference for  unmanaged value. */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_value_get_reference(
    const afw_value_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    /** @todo this needs to addref or return a copy. */
    return instance;
}


/* Implementation of method get_reference for referenced value. */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_value_referenced_get_reference(
    const afw_value_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    /* For referenced value, FIXME. */
    return instance;
}


/* Implementation of method get_reference for permanent value. */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_value_permanent_get_reference(
    const afw_value_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    /* For permanent value, just return the instance passed. */
    return instance;
}

/*
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    const afw_value_t *instance,
    afw_xctx_t *xctx)
{
    return afw_data_type_time;
}

/*
 * Implementation of method compiler_listing for interface afw_value.
 */
void
impl_afw_value_produce_compiler_listing(
    const afw_value_t *instance,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    afw_data_type_value_compiler_listing(
        afw_data_type_time,
        writer, instance, xctx);
}

/*
 * Implementation of method decompile for interface afw_value.
 */
void
impl_afw_value_decompile(
    const afw_value_t *instance,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    afw_data_type_write_as_expression(
        afw_data_type_time,
        writer,
        (const void *)&(((const afw_value_common_t *)instance)->internal),
        xctx);
}

/*
 * Implementation of method get_info for interface afw_value.
 */
void
impl_afw_value_get_info(
    const afw_value_t *instance,
    afw_value_info_t *info,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_memory_clear(info);
    info->value_inf_id = &instance->inf->rti.implementation_id;
    info->evaluated_data_type = afw_data_type_time;
    info->optimized_value = instance;
}


/* Get next value from array of time. */
AFW_DEFINE(const afw_time_t *)
afw_array_of_time_get_next_source(
    const afw_array_t *instance,
    const afw_iterator_t * *iterator,
    const afw_utf8_z_t *source_z,
    afw_xctx_t *xctx)
{
    const void *internal;
    const afw_data_type_t *data_type;

    afw_array_get_next_internal(instance, iterator, &data_type, &internal, xctx);
    if (!internal) {
        return NULL;
    }
    if (data_type != afw_data_type_time) {
        const afw_utf8_t *data_type_id;

        data_type_id = &data_type->data_type_id;
        afw_error_set_fz(afw_error_code_general, source_z, xctx,
            "Typesafe error: expecting 'time' but "
            "encountered " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));
        longjmp(((xctx)->current_try->throw_jmp_buf), afw_error_code_general);
    }
    return (const afw_time_t *)internal;
}

/* Add value from array of time */
AFW_DEFINE(void)
afw_array_of_time_add(
    const afw_array_t *instance,
    const afw_time_t *value,
    afw_xctx_t *xctx)
{
    const afw_array_setter_t *setter;

    setter = afw_array_get_setter(instance, xctx);
    if (!setter) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }

    afw_array_setter_add_internal(setter, 
        afw_data_type_time,
        (const void *)value, xctx);
}

/* Remove value from array of time */
AFW_DEFINE(void)
afw_array_of_time_remove(
    const afw_array_t *instance,
    const afw_time_t *value,
    afw_xctx_t *xctx)
{
    const afw_array_setter_t *setter;

    setter = afw_array_get_setter(instance, xctx);
    if (!setter) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }

    afw_array_setter_remove_internal(setter, 
        afw_data_type_time,
        (const void *)value, xctx);
}
