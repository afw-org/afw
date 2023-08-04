// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for Template Definition
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_value_template_definition.c
 * @brief Implementation of afw_value interface for template definition
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_get_reference NULL


/* Inf specific is always data type. */
#define AFW_IMPLEMENTATION_SPECIFIC (const void *)&afw_data_type_template_direct

/* Define inf variables for data_type and is_evaluated_of_data_type. */
#define AFW_IMPLEMENTATION_INF_VARIABLES \
    (const void *)&afw_data_type_template_direct, \
    NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "template_definition"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_template_definition_inf
#include "afw_value_impl_declares.h"


/* Create function for template definition value. */
AFW_DEFINE(const afw_value_t *)
afw_value_template_definition_create(
    const afw_compile_value_contextual_t *contextual,
    afw_size_t count,
    const afw_value_t * const * values,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_template_definition_t *result;

    if (count <= 0) return afw_value_null;

    result = afw_pool_calloc(p, sizeof(afw_value_template_definition_t), xctx);
    result->inf = &afw_value_template_definition_inf;
    result->contextual = contextual;
    result->count = count;
    result->values = afw_memory_dup(values,
        count * sizeof(afw_value_t *),
        p, xctx);

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
    const afw_value_template_definition_t *self =
        (const afw_value_template_definition_t *)instance;

    afw_size_t i;
    const afw_value_t * const * v;
    const afw_utf8_t **strings;
    const afw_utf8_t **s;
    const afw_value_t *a_value;
    afw_size_t len;
    afw_utf8_octet_t *concat;
    afw_utf8_octet_t *c;
    const afw_value_t *result;
    const afw_compile_value_contextual_t *saved_contextual;

    /* Push value on evaluation stack. */
    afw_xctx_evaluation_stack_push_value(instance, xctx);
    saved_contextual = xctx->error->contextual;
    xctx->error->contextual = self->contextual;

    /* If just one value, evaluate it. */
    if (self->count == 1) {
        result = afw_value_evaluate(*(self->values), p, xctx);
    }

    /* If no values, return empty string value. */
    else if (self->count == 0) {
        result = afw_value_empty_string;
    }

    /* If more than one value, produce concatenated string. */
    else {
        strings = afw_pool_calloc(p, sizeof(afw_utf8_t *) * self->count,
            xctx);
        for (i = 0, v = self->values, len = 0, s = strings;
            i < self->count;
            i++, v++, s++)
        {
            a_value = afw_value_evaluate(*v, p, xctx);
            *s = afw_value_as_utf8(a_value, p, xctx);
            if (*s) {
                len += (*s)->len;
            }
        }

        concat = NULL;
        if (len > 0) {
            concat = afw_pool_calloc(p, len, xctx);
            for (i = 0, c = concat, s = strings; i < self->count; i++, s++) {
                if (*s && (*s)->len > 0) {
                    memcpy(c, (*s)->s, (*s)->len);
                    c += (*s)->len;
                }
            }
        }

        result = afw_value_make_single_string(concat, len, p, xctx);
    }

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
    return afw_data_type_template;
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
    const afw_value_template_definition_t *self =
        (const afw_value_template_definition_t *)instance;
    afw_size_t i;

    afw_value_compiler_listing_begin_value(writer, instance,
        self->contextual, xctx);
    afw_writer_write_z(writer, ": [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    AFW_VALUE_COMPILER_LISTING_IF_NOT_LIMIT_EXCEEDED
    for (i = 0; i < self->count; i++) {
        afw_value_compiler_listing_value(self->values[i], writer, xctx);
    }

    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
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
    const afw_value_template_definition_t *self =
        (const afw_value_template_definition_t *)instance;
    afw_value_string_t string_value;

    if (self->count == 0) {
        afw_writer_write_z(writer, "null", xctx);
        return;
    }

    if (self->count == 1) {
        afw_value_decompile(self->values[0], writer, xctx);
        return;
    }

    afw_writer_write_z(writer, "evaluate_template(", xctx);

    /*impl_decompile_template(self, writer, xctx);*/
    string_value.inf = &afw_value_evaluated_string_inf;
    string_value.internal.s = self->contextual->compiled_value->full_source->s +
        self->contextual->value_offset;
    string_value.internal.len = self->contextual->value_size;
    afw_value_decompile((const afw_value_t *)&string_value, writer, xctx);

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
    const afw_value_template_definition_t *self =
        (const afw_value_template_definition_t *)instance;

    afw_memory_clear(info);
    info->value_inf_id = &instance->inf->rti.implementation_id;
    info->contextual = self->contextual;
    info->optimized_value = instance;

    /* Note: Maybe something can be done for optimized_value_data_type. */
}
