// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for Object Expression
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_value_object_expression.c
 * @brief afw_value implementation for object expression
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_get_reference NULL
#define impl_afw_value_get_assignable_value NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_for_object

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_for_object
    
#define impl_afw_value_create_iterator NULL

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "object_expression"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_object_expression_inf
#define AFW_VALUE_SELF_T afw_value_object_expression_t
#include "afw_value_impl_declares.h"


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
    self->contextual = contextual;
    self->internal = internal;
    return &self->pub;
}

/*
 * Implementation of method optional_evaluate for interface afw_value.
 */
const afw_value_t *
impl_afw_value_optional_evaluate(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    const afw_object_t *from;
    const afw_object_t *to;
    const afw_value_t *v;
    const afw_iterator_old_t *iterator;
    const afw_value_t *property_name;

    from = ((const afw_value_object_expression_t *)&self->pub)->internal;
    to = afw_object_create_script_wrapper(p, xctx);

    for (iterator = NULL;;) {
        v = afw_object_get_next_property(from, &iterator, &property_name,
            xctx);
        if (!v) {
            break;
        }
        v = afw_value_evaluate(v, p, xctx);
        afw_object_set_property(to, property_name, v, xctx);
    }

    return afw_object_as_value(to, p, xctx);
}

/*
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    return afw_data_type_object;
}

/*
 * Implementation of method compiler_listing for interface afw_value.
 *
 * Walk unevaluated property values on the compile-time object. Do not use
 * afw_value_as_object() / data_type object listing here — that evaluates the
 * expression (fails for free variables during compile listing).
 */
void
impl_afw_value_produce_compiler_listing(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_iterator_old_t *iterator;
    const afw_value_t *property_name;
    const afw_value_t *pv;

    afw_value_compiler_listing_begin_value(writer, &self->pub,
        self->contextual, xctx);
    afw_writer_write_z(writer, ": [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    AFW_VALUE_COMPILER_LISTING_IF_NOT_LIMIT_EXCEEDED
    for (iterator = NULL;;) {
        pv = afw_object_get_next_property(self->internal, &iterator,
            &property_name, xctx);
        if (!pv) {
            break;
        }
        afw_writer_write_z(writer, "property ", xctx);
        afw_writer_write_utf8(writer,
            afw_object_property_name_display_utf8(property_name, xctx),
            xctx);
        afw_writer_write_z(writer, " ", xctx);
        afw_value_compiler_listing_value(pv, writer, xctx);
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
    AFW_VALUE_SELF_T *self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    afw_data_type_write_as_expression(
        afw_data_type_object,
        writer,
        (const void *)&(((const afw_value_common_t *)&self->pub)->internal),
        xctx);
}

/*
 * Implementation of method get_info for interface afw_value.
 */
void
impl_afw_value_get_info(
    AFW_VALUE_SELF_T *self,
    afw_value_info_t *info,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{

    afw_memory_clear(info);
    info->value_inf_id = &self->pub.inf->rti.implementation_id;
    info->contextual = self->contextual;
    info->optimized_value = &self->pub;

    /* Note: Maybe something can be done for optimized_value_data_type. */
}
