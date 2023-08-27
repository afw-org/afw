// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for call_script_function
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_value_call_script_function.c
 * @brief Implementation of afw_value interface for call_script_function
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_get_reference NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "call_script_function"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_call_script_function_inf
#define AFW_VALUE_SELF_T afw_value_call_script_function_t
#include "afw_value_impl_declares.h"



AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_value_call_script_function(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_script_function_definition_t *script_function_definition,
    const afw_xctx_scope_t *enclosing_static_scope,
    afw_size_t argc,
    const afw_value_t * const * argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    /* Optimize is set to false since this is one time call. */
    value = afw_value_call_script_function_create(
        contextual, script_function_definition, enclosing_static_scope,
        argc, argv, false, p, xctx);
    return impl_afw_value_optional_evaluate(
        (AFW_VALUE_SELF_T *)value, p, xctx);
}



/* Create function for call value. */
AFW_DEFINE(const afw_value_t *)
afw_value_call_script_function_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_script_function_definition_t *script_function_definition,
    const afw_xctx_scope_t *enclosing_static_scope,
    afw_size_t argc,
    const afw_value_t * const *argv,
    const afw_boolean_t allow_optimize,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    AFW_VALUE_SELF_T *self;

    self = afw_pool_calloc_type(p, AFW_VALUE_SELF_T, xctx);
    self->inf = &afw_value_call_script_function_inf;
    self->script_function_definition = script_function_definition;
    self->enclosing_static_scope = enclosing_static_scope;
    self->args.contextual = contextual;
    self->args.argc = argc;
    self->args.argv = argv;
   
    /** @fixme add optimization. */
    self->optimized_value = (const afw_value_t *)self;

    /** @fixme Get right data type. */
    //self->evaluated_data_type = afw_data_type_string;
    

    return (const afw_value_t *)self;
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
    const afw_value_script_function_definition_t *script;
    const afw_value_t *result;
    const afw_xctx_scope_t *enclosing_static_scope;
    afw_xctx_scope_t *parameter_scope;
    const afw_xctx_scope_t *caller_scope;
    const afw_value_t *value;
    const afw_value_script_function_parameter_t *const *params;
    const afw_value_t *const *arg;
    const afw_value_t *const *rest_argv;
    const afw_array_t *rest_array;
    afw_size_t parameter_number;
    afw_size_t rest_argc;
    afw_boolean_t parameter_scope_activated;

    result = NULL;
    caller_scope = afw_xctx_scope_current(xctx);
    parameter_scope = NULL;
    parameter_scope_activated = false;
    script = self->script_function_definition;

    /* If closure, use its enclosing static scope. */
    if (self->enclosing_static_scope) {
        enclosing_static_scope = self->enclosing_static_scope;
    }

    /* If not closure, use the one at correct depth from caller scope chain. */
    else {                                                                                                                                                        /* Find enclosing static scope. */
        for (
            enclosing_static_scope = caller_scope;
            (
                enclosing_static_scope &&
                enclosing_static_scope->block->depth > script->depth
            );
            enclosing_static_scope =
                enclosing_static_scope->parent_static_scope
        );
        if (!enclosing_static_scope ||
            caller_scope->block->depth < script->depth)
        {
            AFW_THROW_ERROR_Z(general,
                "Can not determine parent static scope for function",
                xctx);
        }
    }

    /* Save stack top which will be restored on return. */
    AFW_TRY {

        /* If there is are parameters, make a parameter block. */
        if (script->signature->block) {

            /* Make a scope for parameters. */
            parameter_scope = afw_xctx_scope_create(
                script->signature->block, enclosing_static_scope, xctx);

            /* Set parameters in scope. */
            for (parameter_number = 1,
                params = script->parameters,
                arg = self->args.argv + 1;
                parameter_number <= script->count;
                parameter_number++, params++, arg++)
            {
                /* If this is rest parameter ... */
                if ((*params)->is_rest) {

                    /* If extra unused parameters, pass them in rest object. */
                    if (self->args.argc >= script->count) {
                        rest_argc = self->args.argc - script->count + 1;
                        rest_argv = arg;
                    }

                    /* If no extra unused parameters, rest object is empty. */
                    else {
                        rest_argc = 0;
                        rest_argv = NULL;
                    }

                    /* Create rest list. */
                    rest_array = afw_array_const_create_array_of_values(
                        rest_argv, rest_argc, p, xctx);
                    value = afw_value_create_array(rest_array, p, xctx);
                }

                /* If not rest parameter */
                else {
                    value = NULL;
                    if (parameter_number <= self->args.argc) {
                        value = afw_function_evaluate_parameter_with_type(
                            *arg, parameter_number,
                            (*params)->type,
                            p, xctx);
                    }

                    if (!value) {
                        if ((*params)->default_value) {
                            value = (*params)->default_value;
                        }
                        else if (!(*params)->is_optional) {
                            AFW_THROW_ERROR_FZ(general, xctx,
                                "Parameter " AFW_SIZE_T_FMT " is required",
                                parameter_number);
                        }
                    }
                }

                /* Set parameter value in parameters scope. */
                *afw_xctx_scope_symbol_get_value_address(
                    (*params)->symbol, parameter_scope, xctx) = value;
            }

            /* Activate the parameter scope. */
            afw_xctx_scope_activate(parameter_scope, xctx);
            parameter_scope_activated = true;
            
            /* If named function, set its symbol in parameter scope. */
            if (script->signature->function_name_symbol) {
                afw_xctx_scope_symbol_set_value(
                    script->signature->function_name_symbol,
                    (const afw_value_t *)script, xctx);
            }
        }

        /* If no parameters, activate functions parent static scope. */
        else {
            afw_xctx_scope_add_reference(enclosing_static_scope, xctx);
            afw_xctx_scope_activate(enclosing_static_scope, xctx);
        }

        /* Evaluate body. */
        result = afw_value_evaluate(script->body, p, xctx);
    }

    AFW_FINALLY{

        /* If there was a parameters scope, release it. */
        if (parameter_scope)
        {
            /* Make sure it was activated so release will succeed. */
            if (!parameter_scope_activated) {
                afw_xctx_scope_activate(parameter_scope, xctx);
            }

            /* Release parameters scope. */
            afw_xctx_scope_release(parameter_scope, xctx);
        }

        /* Return to caller's scope. */
        afw_xctx_scope_release(enclosing_static_scope, xctx);
        afw_xctx_scope_activate(caller_scope, xctx);
    }

    AFW_ENDTRY;

    /* Return result. */
    return result;
}


/*
 * Implementation of method get_data_type for interface afw_value.
 */
const afw_data_type_t *
impl_afw_value_get_data_type(
    AFW_VALUE_SELF_T * self,
    afw_xctx_t *xctx)
{
    return NULL;
}


/*
 * Implementation of method decompile for interface afw_value.
 */
void
impl_afw_value_produce_compiler_listing(
    AFW_VALUE_SELF_T * self,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    afw_value_compiler_listing_begin_value(writer, (const afw_value_t *)self,
        self->args.contextual, xctx);
    afw_writer_write_z(writer, ": [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    if (self->evaluated_data_type) {
        afw_writer_write_z(writer, "evaluated_data_type: ", xctx);
        afw_writer_write_utf8(writer,
            &self->evaluated_data_type->data_type_id, xctx);
        afw_writer_write_eol(writer, xctx);
    }

    if (self->optimized_value != (const afw_value_t *)self) {
        afw_writer_write_z(writer, "optimized_value: ", xctx);
        afw_value_produce_compiler_listing(self->optimized_value, writer, xctx);
        afw_writer_write_eol(writer, xctx);
    }

    afw_value_compiler_listing_value(self->args.argv[0], writer, xctx);

    afw_writer_write_z(writer, "arguments : [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);
    for (afw_size_t i = 1; i <= self->args.argc; i++) {
        afw_value_compiler_listing_value(self->args.argv[i], writer, xctx);
    }
    afw_writer_decrement_indent(writer, xctx);
    afw_writer_write_z(writer, "]", xctx);
    afw_writer_write_eol(writer, xctx);

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
    const afw_writer_t * writer,
    afw_xctx_t *xctx)
{
    /*FIXME

    if (self->qualifier.len > 0) {
        afw_writer_write_utf8(writer, &self->qualifier, xctx);
        afw_writer_write_z(writer, "::", xctx);
    }
    afw_writer_write_utf8(writer, &self->name, xctx);
    afw_value_decompile_call_args(writer, 0, &self->args, xctx);
     */
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
    info->contextual = self->args.contextual;
    info->evaluated_data_type = self->evaluated_data_type;
    info->optimized_value = self->optimized_value;
}
