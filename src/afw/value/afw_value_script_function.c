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
#define impl_afw_value_clone_or_reference NULL

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
#include "afw_value_impl_declares.h"


/* Create function for lambda definition value. */
AFW_DEFINE(const afw_value_t *)
afw_value_script_function_definition_create(
    const afw_compile_value_contextual_t *contextual,
    afw_size_t depth,
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
    self->depth = depth;
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
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    const afw_value_t * instance,
    afw_xctx_t *xctx)
{
    return afw_data_type_function;
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
    const afw_value_script_function_definition_t *self =
        (const afw_value_script_function_definition_t *)instance;
    afw_size_t i;

    afw_value_compiler_listing_begin_value(writer, instance,
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
        afw_value_compiler_listing_name_and_type(
            writer, self->parameters[i]->name, self->parameters[i]->type, xctx);
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
 */
void
impl_afw_value_decompile(
    const afw_value_t * instance,
    const afw_writer_t * writer,
    afw_xctx_t *xctx)
{
    const afw_value_script_function_definition_t *self =
        (const afw_value_script_function_definition_t *)instance;
    afw_size_t i;

    afw_writer_write_z(writer, "function ", xctx);
    afw_value_compiler_listing_name_and_type(writer, NULL, self->returns, xctx);

    afw_writer_write_z(writer, "(", xctx);
    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
        afw_writer_increment_indent(writer, xctx);
    }

    for (i = 0; i < self->count; i++) {
        if (i != 0) {      
            afw_writer_write_z(writer, ",", xctx);
            if (writer->tab) {
                afw_writer_write_eol(writer, xctx);
            }
        }
        afw_value_compiler_listing_name_and_type(
            writer, NULL, self->parameters[i]->type, xctx);
        afw_writer_write_utf8(writer, self->parameters[i]->name, xctx);
    }
    afw_writer_write_z(writer, ")", xctx);

    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
        afw_writer_decrement_indent(writer, xctx);
    }

    afw_writer_write_z(writer, "(", xctx);
    if (writer->tab) {
        afw_writer_write_eol(writer, xctx);
        afw_writer_increment_indent(writer, xctx);
    }
    afw_value_decompile(self->body, writer, xctx);
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
    const afw_value_t *instance,
    afw_value_info_t *info,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_script_function_definition_t *self =
        (const afw_value_script_function_definition_t *)instance;

    afw_memory_clear(info);
    info->value_inf_id = &instance->inf->rti.implementation_id;
    info->contextual = self->contextual;
    info->evaluated_data_type = self->evaluated_data_type;
    info->optimized_value = self->optimized_value;
}
