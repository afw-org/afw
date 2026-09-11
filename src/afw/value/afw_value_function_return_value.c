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
 * Create: RC 1, occupant stored via get_assignable_value. Evaluate peeks
 * the occupant. Last release of the wrapper releases the occupant and
 * frees the header. Leftover wrappers live on the evaluation stack until
 * the call that received them pops.
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
    self = afw_pool_calloc_type(p, AFW_VALUE_SELF_T, xctx);
    self->inf = &afw_value_function_return_value_inf;
    self->p = p;
    self->reference_count = 1;
    self->return_value = afw_value_get_assignable(return_value, xctx);
    return &self->pub;
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
    return afw_value_get_assignable(self->return_value, xctx);
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
    afw_xctx_evaluation_stack_push_value(&self->pub, xctx);
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
