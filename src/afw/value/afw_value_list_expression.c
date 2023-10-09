// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for List Expression
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_value_list_expression.c
 * @brief afw_value implementation for list expression
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_clone_or_reference NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_for_array

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_for_array

#define impl_afw_value_create_iterator NULL

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "list_expression"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_list_expression_inf
#include "afw_value_impl_declares.h"

/** @todo Add contextual to decompile, etc. */

/* Create function for list expression value. */
AFW_DEFINE(const afw_value_t *)
afw_value_create_array_expression(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_value_list_expression_t *self;

    self = afw_pool_calloc(p, sizeof(afw_value_list_expression_t), xctx);
    self->inf = &afw_value_list_expression_inf;
    self->contextual = contextual;
    self->internal = internal;
    return &self->pub;
}

/*
 * Implementation of method optional_evaluate for interface afw_value.
 */
const afw_value_t *
impl_afw_value_optional_evaluate(
    const afw_value_t * instance,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    const afw_value_list_expression_t *self =
        (const afw_value_list_expression_t *)instance;
    const afw_value_t *result;

    result = afw_value_evaluate(self->internal, p, xctx);

    return result;
}

/*
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    const afw_value_t *instance,
    afw_xctx_t *xctx)
{
    return afw_data_type_array;
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
    const afw_value_list_expression_t *self =
        (const afw_value_list_expression_t *)instance;

    afw_value_compiler_listing_begin_value(writer, instance,
        self->contextual, xctx);
    afw_writer_write_z(writer, ": [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    afw_value_compiler_listing_value(self->internal, writer, xctx);

    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
    afw_writer_write_eol(writer, xctx);
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
    const afw_value_list_expression_t *self =
        (const afw_value_list_expression_t *)instance;

    afw_writer_write_z(writer, "list_expression(", xctx);
    afw_value_decompile_value(self->internal, writer, xctx);
    afw_writer_write_z(writer, ")", xctx);
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
    const afw_value_list_expression_t *self =
        (const afw_value_list_expression_t *)instance;

    afw_memory_clear(info);
    info->value_inf_id = &instance->inf->rti.implementation_id;
    info->contextual = self->contextual;
    info->optimized_value = instance;

    /* Note: Maybe something can be done for optimized_value_data_type. */
}
