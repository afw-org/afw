// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for annotated
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_value_annotated.c
 * @brief Implementation of afw_value interface for annotated
 */

#include "afw_internal.h"

#define impl_afw_value_optional_release NULL
#define impl_afw_value_get_reference NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "annotated"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_annotated_inf
#include "afw_value_impl_declares.h"


/* Allocate function for annotation value. */
AFW_DEFINE(afw_value_annotated_t *)
afw_value_annotation_allocate(
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_annotated_t *result;

    result = afw_pool_calloc_type(p, afw_value_annotated_t, xctx);
    result->inf = &afw_value_annotated_inf;

    return result;
}



/* Create function for annotated value. */
AFW_DEFINE(const afw_value_t *)
afw_value_annotated_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_t *value,
    const afw_object_t *annotation,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_annotated_t *result;

    result = afw_pool_calloc_type(p, afw_value_annotated_t, xctx);
    result->inf = &afw_value_annotated_inf;
    result->contextual = contextual;
    result->value = value;
    result->annotation = annotation;

    return (const afw_value_t *)result;
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
    const afw_value_annotated_t *self = (const afw_value_annotated_t *)instance;
    const afw_value_t *result;

    /* Push value on evaluation stack. */
    afw_xctx_evaluation_stack_push_value(instance, xctx);

    /** @fixme Add code to adjust based on annotation. */
    result = afw_value_evaluate(self->value, p, xctx);

    /* Pop value from evaluation stack and return result. */
    afw_xctx_evaluation_stack_pop_parameter_number(xctx);
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
 * Implementation of method compiler_listing for interface afw_value.
 */
void
impl_afw_value_produce_compiler_listing(
    const afw_value_t *instance,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_value_annotated_t *self =
        (const afw_value_annotated_t *)instance;
    afw_value_object_t annotation;

    if (self->annotation) {
        afw_value_compiler_listing_begin_value(writer, instance, NULL, xctx);
        afw_writer_write_z(writer, ": [", xctx);
        afw_writer_write_eol(writer, xctx);
        afw_writer_increment_indent(writer, xctx);

        annotation.inf = &afw_value_evaluated_object_inf;
        annotation.internal = self->annotation;

        afw_value_produce_compiler_listing((const afw_value_t *)&annotation, writer, xctx);
        afw_value_produce_compiler_listing(self->value, writer, xctx);

        afw_writer_decrement_indent(writer, xctx);
        afw_writer_write_z(writer, "]", xctx);
        afw_writer_write_eol(writer, xctx);
    }
    else {
        afw_value_produce_compiler_listing(self->value, writer, xctx);
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
    const afw_value_annotated_t *self =
        (const afw_value_annotated_t *)instance;
    afw_value_object_t annotation;

    if (self->annotation) {
        afw_writer_write_z(writer, "annotate(", xctx);
        if (writer->tab) {
            afw_writer_write_eol(writer, xctx);
            afw_writer_increment_indent(writer, xctx);
        }
        annotation.inf = &afw_value_evaluated_object_inf;
        annotation.internal = self->annotation;
        afw_value_decompile((const afw_value_t *)&annotation, writer, xctx);
        afw_writer_write_z(writer, ",", xctx);
        if (writer->tab) {
            afw_writer_write_eol(writer, xctx);
        }
        afw_value_decompile(self->value, writer, xctx);
        if (writer->tab) {
            afw_writer_write_eol(writer, xctx);
            afw_writer_decrement_indent(writer, xctx);
        }
        afw_writer_write_z(writer, ")", xctx);
    }
    else {
        afw_value_decompile(self->value, writer, xctx);
    }
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
    const afw_value_annotated_t *self =
        (const afw_value_annotated_t *)instance;

    /* Prime with info of value. */
    if (self->value) {
        afw_value_get_info(self->value, info, p, xctx);
    }

    /* Override with this implementation_id and contextual */
    info->value_inf_id = &instance->inf->rti.implementation_id;
    info->contextual = self->contextual;
}
