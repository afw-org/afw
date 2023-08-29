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


/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "closure_binding"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_closure_binding_inf
#define AFW_VALUE_SELF_T afw_value_closure_binding_t
#include "afw_value_impl_declares.h"


/* Create function closure binding value. */
AFW_DEFINE(const afw_value_t *)
afw_value_closure_binding_create(
    const afw_value_script_function_definition_t *script_function_definition,
    const afw_xctx_scope_t *enclosing_lexical_scope,
    afw_xctx_t *xctx)
{
    AFW_VALUE_SELF_T *self;
    self = afw_pool_calloc_type(
        enclosing_lexical_scope->p, AFW_VALUE_SELF_T, xctx);
    self->inf = &afw_value_closure_binding_inf;
    self->script_function_definition = script_function_definition;
    self->enclosing_lexical_scope = enclosing_lexical_scope;
    self->reference_count = 1;
    afw_xctx_scope_add_reference(enclosing_lexical_scope, xctx);

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
    if (self->reference_count <= 1) {
        afw_xctx_scope_release(self->enclosing_lexical_scope, xctx);
    }
    else {
        self->reference_count--;
    }
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
    self->reference_count++;
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
     return (afw_value_t *)self;
}

/*
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    return afw_data_type_function;
}


/*
 * Implementation of method get_evaluated_meta for interface afw_value.
 */
const afw_value_t *
impl_afw_value_get_evaluated_meta(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    return afw_value_internal_get_evaluated_meta_default(
        (afw_value_t *)self, p, xctx);
}

/*
 * Implementation of method get_evaluated_metas for interface afw_value.
 */
const afw_value_t *
impl_afw_value_get_evaluated_metas(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    return afw_value_internal_get_evaluated_metas_default(
        (afw_value_t *)self, p, xctx);
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
    const afw_utf8_t *name;

    if (self->script_function_definition->signature->function_name_value) {
        name = &self->script_function_definition->signature->
            function_name_value->internal;
    }
    else {
        name = &afw_s_a_anonymous_function_id;
    }
    afw_value_compiler_listing_begin_value(
        writer, (const afw_value_t *)self, NULL, xctx);
    afw_writer_write_z(writer, " ", xctx);
    afw_writer_write_utf8(writer, name, xctx);
    afw_writer_write_eol(writer, xctx);
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
    //contextual
    //detail
    info->evaluated_data_type =
        self->script_function_definition->returns->data_type;
    info->optimized_value = (const afw_value_t *)self;
    //extended_value_type
}
