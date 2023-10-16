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
 * @file afw_data_type_void_binding.c
 * @brief Adaptive Framework core data types.
 */

#include "afw.h"
#include "afw_runtime_object_maps.h"
#include "afw_value_internal.h"

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
#define AFW_IMPLEMENTATION_SPECIFIC (const void *)&afw_data_type_void_direct

/* Define inf variables for data_type and is_evaluated_of_data_type. */
#define AFW_IMPLEMENTATION_INF_VARIABLES \
    (const void *)&afw_data_type_void_direct, \
    (const void *)&afw_data_type_void_direct

/* Declares and rti/inf defines for interface afw_value */
/* This is the inf for permanent void values. For this one */
/* optional_release is NULL and get_reference returns instance asis. */
#define AFW_IMPLEMENTATION_ID "permanent_void"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_permanent_void_inf
#define impl_afw_value_optional_release NULL
#define impl_afw_value_clone_or_reference impl_afw_value_permanent_get_reference
#define impl_afw_value_create_iterator NULL
#include "afw_value_impl_declares.h"
#undef AFW_IMPLEMENTATION_ID
#undef AFW_IMPLEMENTATION_INF_LABEL
#undef impl_afw_value_optional_release
#undef impl_afw_value_clone_or_reference

static const afw_value_string_t
impl_data_type_void_id_value = {
    {&afw_value_permanent_string_inf},
    AFW_UTF8_LITERAL("void")
};

AFW_DEFINE_CONST_DATA(afw_value_t *)
afw_data_type_void_id_value =
{
    (const afw_value_t *)&impl_data_type_void_id_value
};

static const afw_utf8_t
impl_data_type_path_void =
    AFW_UTF8_LITERAL("/afw/_AdaptiveDataType_/void");

/* Data type void object. */
static const afw_runtime_object_indirect_t
impl_data_type_object_void = {
    {
        &afw_runtime_inf__AdaptiveDataType_,
        NULL,
        {
            NULL,
            NULL,
            &afw_self_s_void,
            &afw_self_s__AdaptiveDataType_,
            &impl_data_type_path_void
        }
    },
    (void *)&afw_data_type_void_direct
};

/* Value for empty array of void. */
AFW_DEFINE_INTERNAL_CONST_DATA(afw_array_wrapper_for_array_self_t)
impl_empty_array_of_void;

/* Value for empty array of void. */
AFW_DEFINE_INTERNAL_CONST_DATA(afw_value_array_t)
impl_value_empty_array_of_void;

/* Data type void instance. */
AFW_DEFINE_INTERNAL_CONST_DATA(afw_data_type_t)
afw_data_type_void_direct = {
    &afw_data_type_void_inf,
    (const afw_object_t *)&impl_data_type_object_void,
    AFW_UTF8_LITERAL("void"),
    AFW_UTF8_LITERAL("No value"),
    AFW_UTF8_LITERAL("Commonly used as a return type for functions to indicate that the function does not return any value. While technically a variable could be of type void, in practice, it can only hold the values undefined (or null if compile::strictNullChecks flag is not set.)"),
    AFW_UTF8_LITERAL(""),
    afw_data_type_number_void,
    AFW_UTF8_LITERAL(""),
    AFW_UTF8_LITERAL("null"),
    AFW_UTF8_LITERAL(""),
    AFW_UTF8_LITERAL("void *"),
    sizeof(void *),
    NULL,
    NULL,
    NULL,
    afw_compile_type_error,
    true,
    false,
    true,
    false,
    true,
    true
};

/* Data type struct for void. */
AFW_DEFINE_CONST_DATA(afw_data_type_t *)
afw_data_type_void =
    &afw_data_type_void_direct;

/* Implementation of method get_reference for permanent value. */
AFW_DECLARE_STATIC(const afw_value_t *)
impl_afw_value_permanent_get_reference(
    const afw_value_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    /* For unmanaged value, just return the instance passed. */
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
    return afw_data_type_void;
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
        afw_data_type_void,
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
        afw_data_type_void,
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
    info->evaluated_data_type = afw_data_type_void;
    info->optimized_value = instance;
}

