// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for function return value
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_value_function_return_value.c
 * @brief Implementation of afw_value interface for function return temps
 *
 * Return-experiment wrapper. May be adjusted or thrown away.
 */

#include "afw_internal.h"


#define impl_afw_value_create_iterator NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "function_return_value"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_function_return_value_inf
#define AFW_VALUE_SELF_T afw_value_function_return_value_t
#include "afw_value_impl_declares.h"


/* Create function for function return value. */
AFW_DEFINE(const afw_value_t *)
afw_value_function_return_value_create(
    const afw_value_t *return_value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    AFW_VALUE_SELF_T *self;

    if (!return_value ||
        afw_value_is_undefined(return_value) ||
        afw_value_is_void(return_value) ||
        afw_value_is_function_return_value(return_value))
    {
        return return_value;
    }
    if (!p) {
        p = xctx->p;
    }
    /* Caller already chose dest_p (usually the caller's evaluation heap). */
    self = afw_pool_calloc_type(p, AFW_VALUE_SELF_T, xctx);
    self->inf = &afw_value_function_return_value_inf;
    self->p = p;
    /* Birth hold of the wrapper. Occupant is assignable (return slot). */
    self->reference_count = 1;
    self->return_value = return_value
        ? afw_value_get_assignable(return_value, xctx)
        : NULL;
    return &self->pub;
}


/* Host consume: same as get_assignable_value of a return temp. */
AFW_DEFINE(const afw_value_t *)
afw_value_function_return_value_consume(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    if (!afw_value_is_function_return_value(value)) {
        return value;
    }
    return afw_value_get_assignable(value, xctx);
}


static const afw_value_t *
impl_raw_optional_evaluate(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    if (!value || !value->inf || !value->inf->optional_evaluate) {
        return value;
    }
    return value->inf->optional_evaluate(value, p, xctx);
}


/* Everyday evaluate: consume a return temp so callers see the occupant. */
AFW_DEFINE(const afw_value_t *)
afw_value_evaluate_impl(
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;

    result = impl_raw_optional_evaluate(value, p, xctx);
    if (afw_value_is_function_return_value(result)) {
        result = afw_value_get_assignable(result, xctx);
    }
    return result;
}


/*
 * Parameter window: raw evaluate so we still see a return temp, then
 * get_assignable_value (hold inner, release wrapper) and park occupant.
 */
AFW_DEFINE(void)
afw_value_evaluate_for_parameter(
    const afw_value_t **parked,
    const afw_value_t **evaluated,
    const afw_value_t *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;

    *parked = NULL;
    *evaluated = value;

    if (value &&
        (!value->inf ||
            ((afw_size_t)value->inf <= 4096) ||
            (((afw_size_t)value->inf) & (sizeof(void *) - 1)) != 0))
    {
        AFW_THROW_ERROR_Z(general,
            "evaluate_for_parameter: value inf is not a pointer", xctx);
    }

    if (afw_value_is_function_return_value(value)) {
        *evaluated = afw_value_get_assignable(value, xctx);
        *parked = *evaluated;
        return;
    }
    if (afw_value_is_defined_and_evaluated(value)) {
        return;
    }

    result = impl_raw_optional_evaluate(value, p, xctx);
    if (result &&
        (!result->inf ||
            ((afw_size_t)result->inf <= 4096) ||
            (((afw_size_t)result->inf) & (sizeof(void *) - 1)) != 0))
    {
        AFW_THROW_ERROR_Z(general,
            "evaluate_for_parameter: evaluate produced a non-value", xctx);
    }
    if (afw_value_is_function_return_value(result)) {
        *evaluated = afw_value_get_assignable(result, xctx);
        *parked = *evaluated;
        return;
    }
    *evaluated = result;
}


AFW_DEFINE(const afw_value_t *)
afw_value_evaluate_and_park(
    const afw_value_t *value,
    afw_size_t parameter_number,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *parked;
    const afw_value_t *evaluated;

    afw_xctx_evaluation_stack_push_parameter_number(parameter_number, xctx);
    afw_value_evaluate_for_parameter(&parked, &evaluated, value, p, xctx);
    if (parked) {
        afw_xctx_evaluation_stack_pop_parameter_number(parked, xctx);
    }
    else {
        afw_xctx_evaluation_stack_pop(xctx);
    }
    return evaluated;
}


/*
 * Implementation of method optional_release for interface afw_value.
 */
void
impl_afw_value_optional_release(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    if (self->reference_count == 0) {
        return;
    }
    if (self->reference_count == 1) {
        self->reference_count = 0;
        afw_value_release(self->return_value, xctx);
        afw_pool_free_memory(self->p, self, sizeof(AFW_VALUE_SELF_T), xctx);
        return;
    }
    self->reference_count--;
}


/*
 * Implementation of method get_reference for interface afw_value.
 */
const afw_value_t *
impl_afw_value_get_reference(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)xctx;
    self->reference_count++;
    return &self->pub;
}


/*
 * Implementation of method get_assignable_value for interface afw_value.
 */
const afw_value_t *
impl_afw_value_get_assignable_value(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_value_t *inner;

    inner = self->return_value;
    if (self->reference_count <= 1) {
        /* Unique wrapper: transfer occupant to caller. Header stays
         * in self->p until that pool dies. */
        self->return_value = NULL;
        self->reference_count = 0;
        return inner;
    }
    inner = afw_value_get_assignable(inner, xctx);
    self->reference_count--;
    return inner;
}


/*
 * Implementation of method optional_evaluate for interface afw_value.
 */
const afw_value_t *
impl_afw_value_optional_evaluate(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    (void)p;
    (void)xctx;
    /* Peek only. Consume is get_assignable_value. */
    return self->return_value;
}


/*
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    if (!self->return_value ||
        !self->return_value->inf ||
        !self->return_value->inf->get_data_type)
    {
        return NULL;
    }
    return afw_value_get_data_type(self->return_value, xctx);
}


/*
 * Implementation of method produce_compiler_listing for interface afw_value.
 */
void
impl_afw_value_produce_compiler_listing(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_begin_value(writer, &self->pub, NULL, xctx);
    afw_writer_write_z(writer, " ", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);
    afw_value_compiler_listing_value(self->return_value, writer, xctx);
    afw_writer_decrement_indent(writer, xctx);
}


/*
 * Implementation of method decompile for interface afw_value.
 *
 * Synthetic call #function_return_value(return_value).
 */
void
impl_afw_value_decompile(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_value_t *argv[1];

    afw_value_decompile_write_synthetic_function_name(&self->pub, writer, xctx);
    argv[0] = self->return_value;
    afw_value_decompile_value_list(writer, 1, argv, xctx);
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
    info->value_inf_id = &self->inf->rti.implementation_id;
    info->optimized_value = &self->pub;
    if (self->return_value &&
        self->return_value->inf &&
        self->return_value->inf->get_data_type)
    {
        info->evaluated_data_type =
            afw_value_get_data_type(self->return_value, xctx);
    }
    (void)p;
}
