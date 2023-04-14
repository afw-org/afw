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
#define impl_afw_value_get_reference NULL



#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_for_list

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_for_list

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "list_expression"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_list_expression_inf
#include "afw_value_impl_declares.h"

/** @todo Add contextual to decompile, etc. */

/* Create function for list expression value. */
AFW_DEFINE(const afw_value_t *)
afw_value_create_list_expression(
    const afw_compile_value_contextual_t *contextual,
    const afw_list_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_value_list_expression_t *self;

    self = afw_pool_calloc(p, sizeof(afw_value_list_expression_t),
        xctx);
    self->inf = &afw_value_list_expression_inf;
    self->contextual = contextual;
    self->internal = internal;
    return (const afw_value_t *)self;
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
    const afw_list_t *in;
    const afw_list_t *out;
    const afw_iterator_t *iterator;
    const afw_value_t *v;

    in = ((const afw_value_list_expression_t *)instance)->internal;
    out = afw_list_create_generic(p, xctx);

    for (iterator = NULL;;) {
        v = afw_list_get_next_value(in, &iterator, p, xctx);
        if (!v) {
            break;
        }
        v = afw_value_evaluate(v, p, xctx);
        afw_list_add_value(out, v, xctx);
    }

    return afw_value_create_list(out, p, xctx);
}

/*
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    const afw_value_t *instance,
    afw_xctx_t *xctx)
{
    return afw_data_type_list;
}

/*
 * Implementation of method get_evaluated_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_evaluated_data_type(
    const afw_value_t *instance,
    afw_xctx_t *xctx)
{
    return afw_data_type_list;
}

/*
 * Implementation of method get_evaluated_data_type_parameter for interface
 * afw_value.
 */
const afw_utf8_t *
impl_afw_value_get_evaluated_data_type_parameter(
    const afw_value_t *instance,
    afw_xctx_t *xctx)
{
    return NULL;
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
        afw_data_type_list,
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
        afw_data_type_list,
        writer,
        (const void *) & (((const afw_value_evaluated_t *)instance)->internal),
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
    const afw_value_list_expression_t *self =
        (const afw_value_list_expression_t *)instance;

    afw_memory_clear(info);
    info->value_inf_id = &instance->inf->rti.implementation_id;
    info->contextual = self->contextual;
}
