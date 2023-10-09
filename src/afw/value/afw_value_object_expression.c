// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for Object Expression
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_value_object_expression.c
 * @brief afw_value implementation for object expression
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_clone_or_reference NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_for_object

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_for_object
    
#define impl_afw_value_create_iterator NULL

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "object_expression"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_object_expression_inf
#include "afw_value_impl_declares.h"


/** @todo Add contextual to decompile, etc. */


/* Create function for object expression value. */
AFW_DEFINE(const afw_value_t *)
afw_value_create_object_expression(
    const afw_compile_value_contextual_t *contextual,
    const afw_object_t * internal,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_value_object_expression_t *self;

    self = afw_pool_calloc(p, sizeof(afw_value_object_expression_t),
        xctx);
    self->inf = &afw_value_object_expression_inf;
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
    const afw_object_t *from;
    const afw_object_t *to;
    const afw_value_t *v;
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;

    from = ((const afw_value_object_expression_t *)instance)->internal;
    to = afw_object_create(p, xctx);

    for (iterator = NULL;;) {
        v = afw_object_get_next_property(from, &iterator, &property_name,
            xctx);
        if (!v) {
            break;
        }
        v = afw_value_evaluate(v, p, xctx);
        afw_object_set_property(to, property_name, v, xctx);
    }

    return afw_value_create_object(to, p, xctx);
}

/*
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    const afw_value_t *instance,
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
    afw_data_type_value_compiler_listing(
        afw_data_type_object,
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
        afw_data_type_object,
        writer,
        (const void *)&(((const afw_value_evaluated_t *)instance)->internal),
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
    const afw_value_object_expression_t *self =
        (const afw_value_object_expression_t *)instance;

    afw_memory_clear(info);
    info->value_inf_id = &instance->inf->rti.implementation_id;
    info->contextual = self->contextual;
    info->optimized_value = instance;

    /* Note: Maybe something can be done for optimized_value_data_type. */
}
