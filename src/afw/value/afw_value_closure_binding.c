// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for Variable Reference
 *
 * Copyright (c) 2010-2024 Clemson University
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
#define impl_afw_value_create_iterator NULL
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
    /*
     * Create at 0. First add_reference (slot or overlay set) pins the
     * defining scope; matching last-release drops it. Do not pin here:
     * `o.fn = function…` is not a named slot, but the wrapper's set is.
     */
    self->reference_count = 0;

    return &self->pub;
}



/*
 * Bind a script function to its defining scope when stored (assign, return,
 * object/array literal). Capture at the store, not at a later call, and not
 * by hoisting names.
 */
AFW_DEFINE(const afw_value_t *)
afw_value_closure_binding_create_if_needed(
    const afw_value_t *value,
    afw_xctx_t *xctx)
{
    const afw_value_script_function_definition_t *function;
    const afw_xctx_scope_t *scope;
    afw_size_t defining_depth;

    if (!value || !afw_value_is_script_function_definition(value)) {
        return value;
    }

    function = (const afw_value_script_function_definition_t *)value;
    scope = afw_xctx_scope_current(xctx);
    if (!scope || !scope->block) {
        return value;
    }

    /*
     * Prefer signature param block's parent depth when present — same reason
     * as call_script_function: script_function->depth can lag #block unwrap
     * renumbering.
     */
    defining_depth = function->depth;
    if (function->signature && function->signature->block) {
        if (function->signature->block->parent_block) {
            defining_depth =
                function->signature->block->parent_block->depth;
        }
        else {
            defining_depth = 0;
        }
    }

    /*
     * Bind the defining scope, not a nested block we happen to be in (e.g.
     * `if { c0 = tick }` where tick was created in the enclosing body).
     * Returning the raw definition here used to skip the hold and break
     * later calls. Walk the lexical parent chain; do not hoist names.
     */
    for (; scope; scope = scope->parent_lexical_scope) {
        if (defining_depth == scope->block->depth) {
            break;
        }
    }
    if (!scope) {
        AFW_THROW_ERROR_Z(general,
            "Internal error: scope not found", xctx);
    }
    return afw_value_closure_binding_create(function, scope, xctx);
}


/*
 * Implementation of method optional_release for interface afw_value.
 */
void
impl_afw_value_optional_release(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t * xctx)
{
    if (self->reference_count == 0) {
        return;
    }
    if (self->reference_count == 1) {
        self->reference_count = 0;
        afw_xctx_scope_release(self->enclosing_lexical_scope, xctx);
        return;
    }
    self->reference_count--;
}

/*
 * Implementation of method get_reference for interface afw_value.
 */
const afw_value_t *
impl_afw_value_clone_or_reference(
    AFW_VALUE_SELF_T *self,
    const afw_pool_t * p,
    afw_xctx_t * xctx)
{
    (void)p;
    self->reference_count++;
    if (self->reference_count == 1) {
        afw_xctx_scope_get_reference(self->enclosing_lexical_scope, xctx);
    }
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
     return &self->pub;
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
        name = afw_s_a_anonymous_function_id;
    }
    afw_value_compiler_listing_begin_value(
        writer, (const afw_value_t *)self, NULL, xctx);
    afw_writer_write_z(writer, " ", xctx);
    afw_writer_write_utf8(writer, name, xctx);
    afw_writer_write_eol(writer, xctx);
}


/*
 * Implementation of method decompile for interface afw_value.
 *
 * Synthetic call #closure_binding(script_function_definition).
 */
void
impl_afw_value_decompile(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t * writer,
    afw_xctx_t *xctx)
{
    const afw_value_t *argv[1];

    afw_value_decompile_write_synthetic_function_name(
        (const afw_value_t *)self, writer, xctx);
    argv[0] = (const afw_value_t *)self->script_function_definition;
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
    //contextual
    //detail
    if (self->script_function_definition->returns &&
        self->script_function_definition->returns->kind ==
            afw_value_type_kind_data_type)
    {
        info->evaluated_data_type =
            self->script_function_definition->returns->data_type;
    }
    info->optimized_value = (const afw_value_t *)self;
    //extended_value_type
}
