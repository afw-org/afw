// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for Variable Reference
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_value_closure_binding.c
 * @brief Implementation of afw_value interface for variable reference
 */

#include "afw_internal.h"


#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "closure_binding"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_closure_binding_inf
#define AFW_VALUE_SELF_T afw_value_closure_binding_t
#include "afw_value_impl_declares.h"


/* Create function for variable reference value. */
AFW_DEFINE(const afw_value_t *)
afw_value_closure_binding_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_script_function_definition_t *script_function_definition,
    const afw_xctx_scope_t *scope,
    afw_xctx_t *xctx)
{
    AFW_VALUE_SELF_T *self;
    self = afw_pool_calloc_type(scope->p, AFW_VALUE_SELF_T, xctx);
    self->inf = &afw_value_closure_binding_inf;
    self->script_function_definition = script_function_definition;
    self->scope = scope;
    self->reference_count = 1;
    afw_xctx_scope_add_reference(scope, xctx);

    return (afw_value_t *)self;
}


/*
 * Implementation of method optional_release for interface afw_value.
 */
void
impl_afw_value_optional_release(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t * xctx)
{
    afw_xctx_scope_release(self->scope, xctx);
}

/*
 * Implementation of method get_reference for interface afw_value.
 */
const afw_value_t *
impl_afw_value_get_reference(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    afw_xctx_scope_add_reference(self->scope, xctx);
    return (afw_value_t *)self;
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
    return NULL;
}

/*
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    return NULL;
}


/*
 * Implementation of method get_evaluated_meta for interface afw_value.
 */
const afw_value_t *
impl_afw_value_get_evaluated_meta(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    return NULL;
}


/*
 * Implementation of method decompile for interface afw_value.
 */
void
impl_afw_value_produce_compiler_listing(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
}


/*
 * Implementation of method decompile for interface afw_value.
 */
void
impl_afw_value_decompile(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t * writer,
    afw_xctx_t *xctx)
{
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
    // info->evaluated_data_type = self->evaluated_data_type;
    // info->optimized_value = self->optimized_value;
}
