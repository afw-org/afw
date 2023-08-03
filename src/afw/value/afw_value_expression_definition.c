// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for Expression Definition
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_value_expression_definition.c
 * @brief Implementation of afw_value interface for expression definition
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_get_reference NULL


/* Inf specific is always data type. */
#define AFW_IMPLEMENTATION_SPECIFIC (const void *)&afw_data_type_expression_direct

/* Define inf variables for data_type and is_evaluated_of_data_type. */
#define AFW_IMPLEMENTATION_INF_VARIABLES \
    (const void *)&afw_data_type_expression_direct, \
    NULL

#define impl_afw_value_optional_get_optimized NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "expression_definition"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_expression_definition_inf
#include "afw_value_impl_declares.h"


/* Create function for expression definition value. */
AFW_DEFINE(const afw_value_t *)
afw_value_expression_definition_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_assignment_t * const * assignments,
    const afw_value_t * value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_expression_definition_t *result;

    result = afw_pool_calloc_type(p, afw_value_expression_definition_t, xctx);
    result->inf = &afw_value_expression_definition_inf;
    result->contextual = contextual;
    result->assignments = assignments;
    result->value = value;
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
    const afw_value_expression_definition_t *self =
        (const afw_value_expression_definition_t *)instance;

    const afw_value_assignment_t * const *assignment;
    int local_top;
    const afw_value_t *v;
    const afw_value_t *result;
    const afw_compile_value_contextual_t *saved_contextual;

    /* Push value on evaluation stack. */
    afw_xctx_evaluation_stack_push_value(instance, xctx);
    saved_contextual = xctx->error->contextual;
    xctx->error->contextual = self->contextual;

    /* Save stack top which will be restored on return. */
    local_top = afw_xctx_begin_stack_frame(xctx);
    result = NULL;
    AFW_TRY {

        /* Push all assignments on to xctx stack. */
        if (self->assignments) {
            for (assignment = self->assignments; *assignment; assignment++) {
                v = afw_value_evaluate((*assignment)->value, p, xctx);
                afw_xctx_set_local_variable((*assignment)->name, v, xctx);
            }
        }

        /* Evaluate expression value. */
        result = afw_value_evaluate(self->value, p, xctx);
    }

    AFW_FINALLY{

        /* Restore xctx stack top to what it was on entry. */
        afw_xctx_end_stack_frame(local_top, xctx);

    }

    AFW_ENDTRY;
 
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
    return afw_data_type_expression;
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
    const afw_value_expression_definition_t *self =
        (const afw_value_expression_definition_t *)instance;
    const afw_value_assignment_t *const *a;

    afw_value_compiler_listing_begin_value(writer, instance,
        self->contextual, xctx);
    afw_writer_write_z(writer, ": [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    if (self->assignments) for (a = self->assignments; *a; a++) {
        afw_writer_write_utf8(writer, (*a)->name, xctx);
        afw_writer_write_z(writer, " = ", xctx);
        afw_value_compiler_listing_value((*a)->value, writer, xctx);
    }
    afw_value_compiler_listing_value(self->value, writer, xctx);

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
    const afw_value_expression_definition_t *self =
        (const afw_value_expression_definition_t *)instance;
    const afw_value_assignment_t * const *a;

    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
    }
    if (self->assignments) for (a = self->assignments; *a; a++) {
        afw_writer_write_utf8(writer, (*a)->name, xctx);
        if (writer->tab) {
            afw_writer_write_z(writer, " = ", xctx);
        }
        else {
            afw_writer_write_z(writer, "=", xctx);
        }
        afw_value_decompile_value((*a)->value, writer, xctx);
        afw_writer_write_z(writer, ",", xctx);
        if (writer->tab) {
            afw_writer_write_eol(writer, xctx);
        }
    }
    afw_value_decompile_value(self->value, writer, xctx);
    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
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
    const afw_value_expression_definition_t *self =
        (const afw_value_expression_definition_t *)instance;

    afw_memory_clear(info);
    info->value_inf_id = &instance->inf->rti.implementation_id;
    info->contextual = self->contextual;
    info->optimized_value = instance;

    /* Note: Maybe something can be done for optimized_value_data_type. */
}
