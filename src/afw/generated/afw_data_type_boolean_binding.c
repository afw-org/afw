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
 * @file afw_data_type_boolean_binding.c
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
impl_afw_value_unmanaged_get_reference(
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
#define AFW_IMPLEMENTATION_SPECIFIC (const void *)&afw_data_type_boolean_direct

/* Define inf variables for data_type and is_evaluated_of_data_type. */
#define AFW_IMPLEMENTATION_INF_VARIABLES \
    (const void *)&afw_data_type_boolean_direct, \
    (const void *)&afw_data_type_boolean_direct

/* Declares and rti/inf defines for interface afw_value */
/* This is the inf for unmanaged boolean values. For this one */
/* optional_release is NULL and get_reference returns new reference. */
#define AFW_IMPLEMENTATION_ID "boolean"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_unmanaged_boolean_inf
#define impl_afw_value_optional_release NULL
#define impl_afw_value_clone_or_reference impl_afw_value_unmanaged_get_reference
#define impl_afw_value_create_iterator NULL
#include "afw_value_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef impl_afw_value_optional_release
#undef impl_afw_value_clone_or_reference

/* Declares and rti/inf defines for interface afw_value */
/* This is the inf for referenced boolean values. For this one */
/* optional_release releases value and get_reference returns new reference. */
#define AFW_IMPLEMENTATION_ID "referenced_boolean"
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_referenced_boolean_inf
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
/* This is the inf for permanent boolean values. For this one */
/* optional_release is NULL and get_reference returns instance asis. */
#define AFW_IMPLEMENTATION_ID "permanent_boolean"
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_permanent_boolean_inf
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
impl_data_type_boolean_id_value = {
    {&afw_value_permanent_string_inf},
    AFW_UTF8_LITERAL("boolean")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_data_type_boolean_id_value =
{
    (const afw_value_t *)&impl_data_type_boolean_id_value
};

static const afw_utf8_t
impl_data_type_path_boolean =
    AFW_UTF8_LITERAL("/afw/_AdaptiveDataType_/boolean");

/* Data type boolean object. */
static const afw_runtime_object_indirect_t
impl_data_type_object_boolean = {
    {
        &afw_runtime_inf__AdaptiveDataType_,
        NULL,
        {
            NULL,
            NULL,
            &afw_self_s_boolean,
            &afw_self_s__AdaptiveDataType_,
            &impl_data_type_path_boolean
        }
    },
    (void *)&afw_data_type_boolean_direct
};

/* Value for empty array of boolean. */
AFW_DEFINE_INTERNAL_CONST_DATA(afw_array_wrapper_for_array_self_t)
impl_empty_array_of_boolean;

/* Value for empty array of boolean. */
AFW_DEFINE_INTERNAL_CONST_DATA(afw_value_array_t)
impl_value_empty_array_of_boolean;

/* Data type boolean instance. */
AFW_DEFINE_INTERNAL_CONST_DATA(afw_data_type_t)
afw_data_type_boolean_direct = {
    &afw_data_type_boolean_inf,
    (const afw_object_t *)&impl_data_type_object_boolean,
    AFW_UTF8_LITERAL("boolean"),
    AFW_UTF8_LITERAL("A boolean value"),
    AFW_UTF8_LITERAL("Data type 'boolean' holds a boolean value. Its to_string() value is 'true' or 'false'. Its from_string() true value can be mixed case 't', 'true' or '1'. Its from_string() false value can be mixed case 'f', 'false' or '0'."),
    AFW_UTF8_LITERAL(""),
    afw_data_type_number_boolean,
    AFW_UTF8_LITERAL("1.3.6.1.4.1.1466.115.121.1.7"),
    AFW_UTF8_LITERAL("boolean"),
    AFW_UTF8_LITERAL(""),
    AFW_UTF8_LITERAL("afw_boolean_t"),
    sizeof(afw_boolean_t),
    (const afw_array_t *)&impl_empty_array_of_boolean,
    (const afw_value_t *)&impl_value_empty_array_of_boolean,
    &afw_value_unmanaged_boolean_inf,
    afw_compile_type_error,
    true,
    false,
    true,
    false,
    true,
    false
};

/* Value for empty array of boolean. */
AFW_DEFINE_INTERNAL_CONST_DATA(afw_array_wrapper_for_array_self_t)
impl_empty_array_of_boolean = {
    &afw_array_wrapper_for_array_inf,
    &afw_data_type_boolean_direct,
    0
};

/* Value for empty array of boolean. */
AFW_DEFINE_INTERNAL_CONST_DATA(afw_value_array_t)
impl_value_empty_array_of_boolean = {
    {&afw_value_permanent_array_inf},
    (const afw_array_t *)&impl_empty_array_of_boolean
};

/* Data type struct for boolean. */
AFW_DEFINE_CONST_DATA(afw_data_type_t *)
afw_data_type_boolean =
    &afw_data_type_boolean_direct;

/* Set property function for data type boolean values. */
AFW_DEFINE(void)
afw_object_set_property_as_boolean(
    const afw_object_t *object,
    const afw_utf8_t *property_name,
    afw_boolean_t internal,
    afw_xctx_t *xctx)
{
    const afw_value_t *v;

    if (!object->p) {
        AFW_THROW_ERROR_Z(general,
            "Object must have a pool",
            xctx);
    }

    v = afw_value_create_boolean_unmanaged(internal, object->p, xctx);
    afw_object_set_property(object, property_name, v, xctx);
}

/* Typesafe cast of data type boolean. */
AFW_DEFINE(afw_boolean_t)
afw_value_as_boolean(const afw_value_t *value, afw_xctx_t *xctx)
{
    value = afw_value_evaluate(value, xctx->p, xctx);
    if (!AFW_VALUE_IS_DATA_TYPE(value, boolean))
    {
        const afw_utf8_t *data_type_id;

        if (!value) {
            AFW_THROW_ERROR_Z(general,
                "Typesafe error: expecting 'boolean' but "
                "encountered 'undefined'",
                xctx);
        }

        data_type_id = afw_value_get_quick_data_type_id(value);
        AFW_THROW_ERROR_FZ(general, xctx,
            "Typesafe error: expecting 'boolean' but "
            "encountered " AFW_UTF8_FMT_Q ,
            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));
    }
    return (((const afw_value_boolean_t *)value)->internal);
}

/* Allocate function for unmanaged data type boolean values. */
AFW_DEFINE(afw_value_boolean_t *)
afw_value_allocate_boolean(const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_value_boolean_t *result;

    result = afw_pool_calloc(p, sizeof(afw_value_boolean_t),
        xctx);
    result->inf = &afw_value_unmanaged_boolean_inf;
    return result;
}

/* Create function for referenced data type boolean value. */
AFW_DEFINE(const afw_value_t *)
afw_value_create_referenced_boolean(afw_boolean_t internal,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_value_boolean_t *v;

    v = afw_pool_calloc(p, sizeof(afw_value_boolean_t),
        xctx);
    v->inf = &afw_value_referenced_boolean_inf;
    v->internal = internal;
    return &v->pub;
}

/* Create function for unmanaged data type boolean value. */
AFW_DEFINE(const afw_value_t *)
afw_value_create_boolean_unmanaged(afw_boolean_t internal,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_value_boolean_t *v;

    v = afw_pool_calloc(p, sizeof(afw_value_boolean_t),
        xctx);
    v->inf = &afw_value_unmanaged_boolean_inf;
    v->internal = internal;
    return &v->pub;
}

/* Convert data type boolean string to afw_boolean_t *. */
AFW_DEFINE(void)
afw_data_type_boolean_to_internal(afw_boolean_t *to_internal,
    const afw_utf8_t *from_utf8, const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_data_type_utf8_to_internal(
        &afw_data_type_boolean_direct,
        (void *)to_internal, from_utf8, p, xctx);
}

/*  Convert data type boolean internal representation to utf-8. */
AFW_DEFINE(const afw_utf8_t *)
afw_data_type_boolean_to_utf8(afw_boolean_t internal,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    return afw_data_type_internal_to_utf8(
        &afw_data_type_boolean_direct,
        &internal, p, xctx);
}

/* Get property function for data type boolean values. */
AFW_DEFINE(afw_boolean_t)
afw_object_get_property_as_boolean_source(
    const afw_object_t *object,
    const afw_utf8_t *property_name,
    afw_boolean_t *found,
    const afw_utf8_z_t *source_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    *found = false;
    value = afw_object_get_property(object, property_name, xctx);
    if (!value) {
        return 0;
    }

    value = afw_value_evaluate(value, p, xctx);
    *found = true;
    if (!AFW_VALUE_IS_DATA_TYPE(value, boolean))
    {
        const afw_utf8_t *data_type_id;

        data_type_id = afw_value_get_quick_data_type_id(value);
        afw_error_set_fz(afw_error_code_general, source_z, xctx,
            "Typesafe error: expecting 'boolean' but "
            "encountered " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));
        longjmp(((xctx)->current_try->throw_jmp_buf), afw_error_code_general);
    }
    return (((const afw_value_boolean_t *)value)->internal);
}

/* Get next property function for data type boolean values. */
AFW_DEFINE(afw_boolean_t)
afw_object_get_next_property_as_boolean_source(
    const afw_object_t *object,
    const afw_iterator_t * *iterator,
    const afw_utf8_t * *property_name,
    afw_boolean_t *found,
    const afw_utf8_z_t *source_z,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    *found = false;
    value = afw_object_get_next_property(object, iterator, property_name, xctx);
    if (!value) {
        return 0;
    }

    value = afw_value_evaluate(value, p, xctx);
    *found = true;
    if (!AFW_VALUE_IS_DATA_TYPE(value, boolean))
    {
        const afw_utf8_t *data_type_id;

        data_type_id = afw_value_get_quick_data_type_id(value);
        afw_error_set_fz(afw_error_code_general, source_z, xctx,
            "Typesafe error: expecting 'boolean' but "
            "encountered " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));
        longjmp(((xctx)->current_try->throw_jmp_buf), afw_error_code_general);
    }
    return (((const afw_value_boolean_t *)value)->internal);
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
impl_afw_value_unmanaged_get_reference(
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
    return afw_data_type_boolean;
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
        afw_data_type_boolean,
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
        afw_data_type_boolean,
        writer,
        (const void *)&(((const afw_value_unmanaged_t *)instance)->internal),
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
    info->evaluated_data_type = afw_data_type_boolean;
    info->optimized_value = instance;
}


/* Get next value from array of boolean. */
AFW_DEFINE(afw_boolean_t)
afw_array_of_boolean_get_next_source(
    const afw_array_t *instance,
    const afw_iterator_t * *iterator,
    afw_boolean_t *found,
    const afw_utf8_z_t *source_z,
    afw_xctx_t *xctx)
{
    const void *internal;
    const afw_data_type_t *data_type;

    afw_array_get_next_internal(instance, iterator, &data_type, &internal, xctx);
    *found = true;
    if (!internal) {
        *found = false;
        return 0;
    }
    if (data_type != afw_data_type_boolean) {
        const afw_utf8_t *data_type_id;

        data_type_id = &data_type->data_type_id;
        afw_error_set_fz(afw_error_code_general, source_z, xctx,
            "Typesafe error: expecting 'boolean' but "
            "encountered " AFW_UTF8_FMT_Q,
            AFW_UTF8_FMT_OPTIONAL_UNDEFINED_ARG(data_type_id));
        longjmp(((xctx)->current_try->throw_jmp_buf), afw_error_code_general);
    }
    return *(afw_boolean_t *)internal;
}

/* Add value from array of boolean */
AFW_DEFINE(void)
afw_array_of_boolean_add(
    const afw_array_t *instance,
    const afw_boolean_t *value,
    afw_xctx_t *xctx)
{
    const afw_array_setter_t *setter;

    setter = afw_array_get_setter(instance, xctx);
    if (!setter) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }

    afw_array_setter_add_internal(setter, 
        afw_data_type_boolean,
        (const void *)value, xctx);
}

/* Remove value from array of boolean */
AFW_DEFINE(void)
afw_array_of_boolean_remove(
    const afw_array_t *instance,
    const afw_boolean_t *value,
    afw_xctx_t *xctx)
{
    const afw_array_setter_t *setter;

    setter = afw_array_get_setter(instance, xctx);
    if (!setter) {
        AFW_LIST_ERROR_OBJECT_IMMUTABLE;
    }

    afw_array_setter_remove_internal(setter, 
        afw_data_type_boolean,
        (const void *)value, xctx);
}
