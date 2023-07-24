// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for Variable Reference
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_value_qualified_variable_reference.c
 * @brief Implementation of afw_value interface for variable reference
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_get_reference NULL



#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "qualified_variable_reference"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_qualified_variable_reference_inf
#include "afw_value_impl_declares.h"


/* Create function for variable reference value. */
AFW_DEFINE(const afw_value_t *)
afw_value_qualified_variable_reference_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_utf8_t *qualifier,
    const afw_utf8_t *name,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_qualified_variable_reference_t *result;
    result = afw_pool_calloc_type(p, afw_value_qualified_variable_reference_t, xctx);
    result->inf = &afw_value_qualified_variable_reference_inf;
    result->contextual = contextual;
    memcpy(&result->name, name, sizeof(afw_utf8_t));
    if (qualifier) {
        memcpy(&result->qualifier, qualifier, sizeof(afw_utf8_t));
        result->optionally_qualified_name = afw_utf8_concat(p, xctx,
            &result->qualifier, &afw_s_a_qualification_operator, &result->name,
            NULL);
    }
    else {
        result->optionally_qualified_name = &result->name;
    }
    result->backtrace_detail = result->optionally_qualified_name;

    return (afw_value_t *)result;
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
    const afw_value_qualified_variable_reference_t *self =
        (const afw_value_qualified_variable_reference_t *)instance;
    const afw_value_t *result;
    const afw_compile_value_contextual_t *saved_contextual;

    /* Push value on evaluation stack. */
    afw_xctx_evaluation_stack_push_value(instance, xctx);
    saved_contextual = xctx->error->contextual;
    xctx->error->contextual = self->contextual;

    /* Try to locate qualified variable. */
    result = afw_xctx_get_qualified_variable(&self->qualifier, &self->name,
        xctx);

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
    return NULL;
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
    const afw_value_qualified_variable_reference_t *self =
        (const afw_value_qualified_variable_reference_t *)instance;
    afw_value_meta_object_self_t *meta;

    meta = afw_value_internal_create_meta_object_self(
        instance, p, xctx);
    meta->key = self->optionally_qualified_name;
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
    const afw_value_qualified_variable_reference_t *self =
        (const afw_value_qualified_variable_reference_t *)instance;

    afw_value_compiler_listing_begin_value(writer, instance,
        self->contextual, xctx);
    afw_writer_write_z(writer, " ", xctx);
    if (self->qualifier.len > 0) {
        afw_writer_write_utf8(writer, &self->qualifier, xctx);
        afw_writer_write_z(writer, "::", xctx);
    }
    afw_writer_write_utf8(writer, &self->name, xctx);
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
    const afw_value_qualified_variable_reference_t *self =
        (const afw_value_qualified_variable_reference_t *)instance;

    if (self->qualifier.len > 0) {
        afw_writer_write_utf8(writer, &self->qualifier, xctx);
        afw_writer_write_z(writer, "::", xctx);
    }
    afw_writer_write_utf8(writer, &self->name, xctx);

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
    const afw_value_qualified_variable_reference_t *self =
        (const afw_value_qualified_variable_reference_t *)instance;

    afw_memory_clear(info);
    info->value_inf_id = &instance->inf->rti.implementation_id;
    info->contextual = self->contextual;
    info->detail = self->backtrace_detail;
}
