// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for Lambda Definition
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_value_script_function.c
 * @brief Implementation of afw_value interface for lambda definition
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL

/* Inf specific is always data type. */
#define AFW_IMPLEMENTATION_SPECIFIC (const void *)&afw_data_type_function_direct

/* Define inf variables for data_type and is_evaluated_of_data_type. */
#define AFW_IMPLEMENTATION_INF_VARIABLES \
    (const void *)&afw_data_type_function_direct, \
    NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

#define impl_afw_value_optional_evaluate NULL
    
#define impl_afw_value_create_iterator NULL

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "script_function"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_script_function_definition_inf
#define AFW_VALUE_SELF_T afw_value_script_function_definition_t
#include "afw_value_impl_declares.h"


/* Create function for lambda definition value. */
AFW_DEFINE(const afw_value_t *)
afw_value_script_function_definition_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_block_t *enclosing_block,
    const afw_value_script_function_signature_t *signature,
    const afw_value_type_t *returns,
    afw_size_t count,
    const afw_value_script_function_parameter_t **parameters,
    const afw_value_t *body,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_script_function_definition_t *self;

    /** @fixme Work about in all creates if copy needed. */

    self = afw_pool_calloc_type(p, afw_value_script_function_definition_t, xctx);
    self->inf = &afw_value_script_function_definition_inf;
    self->contextual = contextual;
    self->enclosing_block = enclosing_block;
    self->depth = enclosing_block ? enclosing_block->depth : 0;
    self->signature = signature;
    self->returns = returns;
    self->count = count;
    self->parameters = parameters;
    self->body = body;
   
    /** @fixme add optimization. */
    self->optimized_value = (const afw_value_t *)self;

    /** @fixme Get right data type. */
    //self->evaluated_data_type = afw_data_type_string;
    
    
    return &self->pub;
}


/*
 * Keep the definition alive (compiled unit). Slot fill is
 * get_assignable_value → closure_binding.
 */
const afw_value_t *
impl_afw_value_get_reference(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    (void)xctx;
    return &self->pub;
}


/*
 * Store-time bind (#35): wrap as closure_binding and take the hold
 * the caller (slot_store) will release.
 */
const afw_value_t *
impl_afw_value_get_assignable_value(
    AFW_VALUE_SELF_T *self,
    afw_xctx_t *xctx)
{
    const afw_value_t *binding;

    binding = afw_value_closure_binding_create_if_needed(
        &self->pub, xctx);
    if (binding == &self->pub) {
        return &self->pub;
    }
    return afw_value_add_reference(binding, xctx);
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
 * Implementation of method compiler_listing for interface afw_value.
 */
void
impl_afw_value_produce_compiler_listing(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    afw_size_t i;

    afw_value_compiler_listing_begin_value(writer, &self->pub,
        self->contextual, xctx);
    afw_writer_write_z(writer, ": [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    afw_writer_write_z(writer, "returns: ", xctx);
    afw_value_compiler_listing_name_and_type(writer, NULL, self->returns, xctx);
    afw_writer_write_eol(writer, xctx);

    afw_writer_write_z(writer, "parameters: [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    for (i = 0; i < self->count; i++) {
        if (self->parameters[i]->is_rest) {
            afw_writer_write_z(writer, "...", xctx);
        }
        if (self->parameters[i]->assignment_target) {
            afw_value_produce_compiler_listing(
                self->parameters[i]->assignment_target, writer, xctx);
        }
        else {
            afw_value_compiler_listing_name_and_type(
                writer, self->parameters[i]->name
                    ? &self->parameters[i]->name->internal : NULL,
                self->parameters[i]->type, xctx);
        }
        if (self->parameters[i]->default_value) {
            afw_writer_write_z(writer, " = ", xctx);
            afw_value_compiler_listing_value(
                self->parameters[i]->default_value, writer, xctx);
        }
        else if (self->parameters[i]->is_optional) {
            afw_writer_write_z(writer, "?", xctx);
            afw_writer_write_eol(writer, xctx);
        }
        else {
            afw_writer_write_eol(writer, xctx);
        }
    }
    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
    afw_writer_write_eol(writer, xctx);

    afw_writer_write_z(writer, "signature ", xctx);
    if (self->signature->block) {
        afw_value_produce_compiler_listing(
            (const afw_value_t *)self->signature->block, writer, xctx);
    }
    else {
        afw_writer_write_z(writer, "undefined", xctx);
        afw_writer_write_eol(writer, xctx);
    }

    afw_writer_write_z(writer, "body ", xctx);
    if (self->body) {
        afw_value_produce_compiler_listing(self->body, writer, xctx);
    }
    else {
        afw_writer_write_z(writer, "undefined", xctx);
        afw_writer_write_eol(writer, xctx);
    }

    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
    afw_writer_write_eol(writer, xctx);
}


/*
 * Implementation of method decompile for interface afw_value.
 *
 * Synthetic call #script_function(param..., body[, returnType]) —
 * params are surface-like: name[: Type], optional ?, default = expr, rest ...
 * Return type is a trailing Type after the body when non-default.
 */
void
impl_afw_value_decompile(
    AFW_VALUE_SELF_T *self,
    const afw_writer_t * writer,
    afw_xctx_t *xctx)
{
    const afw_value_script_function_parameter_t *param;
    afw_size_t i;
    afw_boolean_t need_comma;
    afw_boolean_t write_returns;

    afw_value_decompile_write_synthetic_function_name(&self->pub, writer, xctx);
    afw_writer_write_z(writer, "(", xctx);
    if (writer->tab) {
        afw_writer_increment_indent(writer, xctx);
    }

    need_comma = false;
    for (i = 0; i < self->count; i++) {
        param = self->parameters[i];
        if (need_comma) {
            afw_writer_write_z(writer, ",", xctx);
        }
        if (writer->tab) {
            afw_writer_write_eol(writer, xctx);
        }
        if (param->is_rest) {
            afw_writer_write_z(writer, "...", xctx);
        }
        if (param->assignment_target) {
            /* Surface Pattern (not #assignment_target wrapper). */
            afw_value_decompile_assignment_pattern(
                param->assignment_target, writer, xctx);
        }
        else {
            /* Bare identifier (not a string) so Type annotations use ':'. */
            afw_writer_write_utf8(writer, &param->name->internal, xctx);
            afw_value_decompile_optional_type(param->type, writer, xctx);
        }
        if (param->is_optional && !param->default_value) {
            afw_writer_write_z(writer, "?", xctx);
        }
        if (param->default_value) {
            afw_writer_write_z(writer, writer->tab ? " = " : "=", xctx);
            afw_value_decompile_value(param->default_value, writer, xctx);
        }
        need_comma = true;
    }

    if (need_comma) {
        afw_writer_write_z(writer, ",", xctx);
    }
    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
    }
    afw_value_decompile_value(self->body, writer, xctx);

    /*
     * Trailing return Type after body when present and not bare any.
     */
    write_returns = false;
    if (self->returns) {
        if (self->returns->kind != afw_value_type_kind_data_type ||
            self->returns->data_type != afw_data_type_any)
        {
            write_returns = true;
        }
    }
    if (write_returns) {
        afw_writer_write_z(writer, ",", xctx);
        if (writer->tab) {
            afw_writer_write_eol(writer, xctx);
        }
        afw_value_decompile_type(self->returns, writer, xctx);
    }

    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
        afw_writer_decrement_indent(writer, xctx);
    }
    afw_writer_write_z(writer, ")", xctx);
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
    info->evaluated_data_type = self->evaluated_data_type;
    info->optimized_value = self->optimized_value;
}
