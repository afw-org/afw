// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for Variable Reference
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_value_variable_reference.c
 * @brief Implementation of afw_value interface for variable reference
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_get_reference NULL

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "variable_reference"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_variable_reference_inf
#include "afw_value_impl_declares.h"



/* Create function for variable reference value. */
AFW_DEFINE(const afw_value_t *)
afw_value_variable_reference_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_block_symbol_t *symbol,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_variable_reference_t *self;

    self = afw_pool_calloc_type(p, afw_value_variable_reference_t, xctx);
    self->inf = &afw_value_variable_reference_inf;
    self->contextual = contextual;
    self->symbol = symbol;

    /** @fixme add optimization. */
    self->optimized_value = (const afw_value_t *)self;

    return (afw_value_t *)self;
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
    const afw_value_variable_reference_t *self =
        (const afw_value_variable_reference_t *)instance;
    const afw_value_t *result;
    const afw_compile_value_contextual_t *saved_contextual;

    /* Push value on evaluation stack. */
    afw_xctx_evaluation_stack_push_value(instance, xctx);
    saved_contextual = xctx->error->contextual;
    xctx->error->contextual = self->contextual;

    /* Try to locate a variable. */
    result = afw_xctx_get_qualified_variable(NULL, self->symbol->name, xctx);

    /* Pop value from evaluation stack and return result. */
    afw_xctx_evaluation_stack_pop_value(xctx);
    xctx->error->contextual = saved_contextual;
    return result;
}

/*
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    const afw_value_t * instance,
    afw_xctx_t *xctx)
{
    const afw_value_variable_reference_t *self =
        (const afw_value_variable_reference_t *)instance;

    return self->evaluated_data_type;
}


/*
 * Implementation of method get_evaluated_meta for interface afw_value.
 */
const afw_value_t *
impl_afw_value_get_evaluated_meta(
    const afw_value_t *instance,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_variable_reference_t *self =
        (const afw_value_variable_reference_t *)instance;
    afw_value_meta_object_self_t *meta;

    meta = afw_value_internal_create_meta_object_self(
        instance, p, xctx);
    meta->key = self->symbol->name;
    if (afw_value_is_object(meta->evaluated_value)) {
        meta->additional =
            (((const afw_value_object_t *)meta->evaluated_value)->
                internal)->meta.meta_object;
    }
    return (const afw_value_t *)& meta->meta_object_value;
}


/*
 * Implementation of method decompile for interface afw_value.
 */
void
impl_afw_value_produce_compiler_listing(
    const afw_value_t *instance,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_value_variable_reference_t *self =
        (const afw_value_variable_reference_t *)instance;

    afw_value_compiler_listing_begin_value(writer, instance,
        self->contextual, xctx);
    afw_writer_write_z(writer, " name=", xctx);
    afw_writer_write_utf8(writer, self->symbol->name, xctx);
    afw_writer_write_z(writer, " block=", xctx);
    afw_writer_write_size(writer, self->symbol->parent_block->number, xctx);
    afw_writer_write_z(writer, " depth=", xctx);
    afw_writer_write_size(writer, self->symbol->parent_block->depth, xctx);
    afw_writer_write_z(writer, " index=", xctx);
    afw_writer_write_size(writer, self->symbol->index, xctx);
    afw_writer_write_eol(writer, xctx);

    if (self->evaluated_data_type) {
        afw_writer_write_z(writer, "evaluated_data_type: ", xctx);
        afw_writer_write_utf8(writer,
            &self->evaluated_data_type->data_type_id, xctx);
        afw_writer_write_eol(writer, xctx);
    }

    if (self->optimized_value != instance) {
        afw_writer_write_z(writer, "optimized_value: ", xctx);
        afw_value_produce_compiler_listing(self->optimized_value, writer, xctx);
        afw_writer_write_eol(writer, xctx);
    }
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
    const afw_value_variable_reference_t *self =
        (const afw_value_variable_reference_t *)instance;

    afw_writer_write_utf8(writer, self->symbol->name, xctx);
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
    const afw_value_variable_reference_t *self =
        (const afw_value_variable_reference_t *)instance;

    afw_memory_clear(info);
    info->value_inf_id = &instance->inf->rti.implementation_id;
    info->contextual = self->contextual;
    info->evaluated_data_type = self->evaluated_data_type;
    info->optimized_value = self->optimized_value;

    /* Note: Maybe something can be done for optimized_value_data_type. */
}
