// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for Function Definition
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_value_function_definition.c
 * @brief Implementation of afw_value interface for function
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_get_reference NULL


/* Inf specific is always data type. */
#define AFW_IMPLEMENTATION_SPECIFIC (const void *)&afw_data_type_function_direct

/* Define inf variables for data_type and is_evaluated_of_data_type. */
#define AFW_IMPLEMENTATION_INF_VARIABLES \
    (const void *)&afw_data_type_function_direct, \
    NULL

#define impl_afw_value_optional_get_optimized NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "function_definition"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_function_definition_inf
#include "afw_value_impl_declares.h"


/*
 * Implementation of method optional_evaluate for interface afw_value.
 */
const afw_value_t *
impl_afw_value_optional_evaluate(
    const afw_value_t * instance,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    return instance;
}

/*
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    const afw_value_t * instance,
    afw_xctx_t *xctx)
{
    return afw_data_type_object;
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
    const afw_value_function_definition_t *self =
        (const afw_value_function_definition_t *)instance;

    afw_value_compiler_listing_begin_value(writer, instance, NULL, xctx);
    afw_writer_write_z(writer, " ", xctx);
    afw_writer_write_utf8(writer, &self->functionId, xctx);
    afw_writer_write_eol(writer, xctx);
}

/*
 * Implementation of method decompile for interface afw_value.
 */
void
impl_afw_value_decompile(
    const afw_value_t * instance,
    const afw_writer_t * writer,
    afw_xctx_t *xctx)
{
    const afw_value_function_definition_t *self =
        (const afw_value_function_definition_t *)instance;

    afw_writer_write_utf8(writer, &self->functionId, xctx);
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
    const afw_value_function_definition_t *self =
        (const afw_value_function_definition_t *)instance;

    afw_memory_clear(info);
    info->value_inf_id = &instance->inf->rti.implementation_id;
    info->detail = &self->functionId;
    info->optimized_value = instance;

    /* Note: Maybe something can be done for optimized_value_data_type. */
}
