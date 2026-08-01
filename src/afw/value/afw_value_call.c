// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for call
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_value_call.c
 * @brief Implementation of afw_value interface for call
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_clone_or_reference NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default
    
#define impl_afw_value_create_iterator NULL

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "call"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_call_inf
#define AFW_VALUE_SELF_T afw_value_call_t
#include "afw_value_impl_declares.h"




/* Create function for call value. */
AFW_DEFINE(const afw_value_t *)
afw_value_call_create(
    const afw_compile_value_contextual_t *contextual,
    afw_size_t argc,
    const afw_value_t * const *argv,
    const afw_boolean_t allow_optimize,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_call_t *self;

    if (!argv[0]) {
        AFW_THROW_ERROR_Z(general,
            "afw_value_call_create() argv[0] must be specified",
            xctx);
    }

    /*
     * If this is a function definition, return the self of the more specific
     * afw_value_call_built_in_function_create() to reduce the execution time
     * code path.
     */
    if (afw_value_is_function_definition(argv[0])) {
        return afw_value_call_built_in_function_create(
            contextual, argc, argv, allow_optimize, p, xctx);
    }

    /*
     * If this is a script function definition, return the self of the more 
     * specific afw_value_call_script_function_create() to reduce the execution
     * time code path.
     */
    if (afw_value_is_script_function_definition(argv[0])) {
        return afw_value_call_script_function_create(
            contextual,
            (const afw_value_script_function_definition_t *)argv[0], NULL,
            argc, argv, allow_optimize, p, xctx);
    }

    /*
     * If this is a script function definition, return the self of the more 
     * specific afw_value_call_script_function_create() to reduce the execution
     * time code path.
     */
    if (afw_value_is_closure_binding(argv[0])) {
        return afw_value_call_script_function_create(
            contextual,
                ((const afw_value_closure_binding_t *)argv[0])->
                    script_function_definition,
                ((const afw_value_closure_binding_t *)argv[0])->
                    enclosing_lexical_scope,
            argc, argv, allow_optimize, p, xctx);
    }

    /*
     * If this is a symbol reference directly to a script function definition,
     * return the self of the more specific
     * afw_value_call_script_function_create() to reduce the execution time
     * code path.
     */
    if (afw_value_is_symbol_reference(argv[0])) {
        const afw_value_symbol_reference_t *ref =
            (const afw_value_symbol_reference_t *)argv[0];
        if (afw_value_is_script_function_definition(
            ref->symbol->initial_value))
        {
            return afw_value_call_script_function_create(
                contextual,
                (const afw_value_script_function_definition_t *)
                    ref->symbol->initial_value,
                NULL,
                argc, argv, allow_optimize, p, xctx);
        }
        if (afw_value_is_closure_binding(ref->symbol->initial_value))
        {
            return afw_value_call_script_function_create(
                contextual,
                ((const afw_value_closure_binding_t *)
                    ref->symbol->initial_value)->script_function_definition,
                ((const afw_value_closure_binding_t *)
                    ref->symbol->initial_value)->enclosing_lexical_scope,
                argc, argv, allow_optimize, p, xctx);
        }
    }

    self = afw_pool_calloc_type(p, afw_value_call_t, xctx);
    self->inf = &afw_value_call_inf;
    self->function_value = argv[0];
    self->args.contextual = contextual;
    self->args.argc = argc;
    self->args.argv = argv;
   
    /** @fixme add optimization. */
    self->optimized_value = (const afw_value_t *)self;

    /** @fixme Get right data type. */
    //self->evaluated_data_type = afw_data_type_string;
    

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
    const afw_compile_value_contextual_t *saved_contextual;
    const afw_value_t *function_value;
    const afw_value_t *result;

    /* Push value on evaluation stack. */
    afw_xctx_evaluation_stack_push_value(&self->pub, xctx);
    saved_contextual = xctx->error->contextual;
    xctx->error->contextual = self->args.contextual;

    /*
     * The function value may need to be evaluated since it's likely to be
     * a variable reference or something else that needs to be evaluated
     * first. Function afw_value_call_create() will have already determined
     * if the function value is a built-in or script function definition and
     * called the appropriate create function, so those should not be
     * encountered here.
     */
    function_value = afw_value_evaluate(self->function_value, p, xctx);

    /* If function_value is a compile value, evaluate it. */
    if (afw_value_is_compiled_value(function_value)) {
        function_value = afw_value_evaluate(function_value, p, xctx);
    }

    /*
     * This is most likely a script function call since built-ins are usually
     * handled by afw_value_built_in_function. Check and handle for script
     * functions first.
     */
    {
        afw_size_t call_argc;
        const afw_value_t * const *call_argv;

        afw_value_call_args_expand_spreads(
            self->args.argc, self->args.argv,
            &call_argc, &call_argv, p, xctx);

        if (afw_value_is_script_function_definition(function_value)) {
            result = afw_value_call_script_function(
                self->args.contextual,
                (afw_value_script_function_definition_t *)function_value,
                NULL,
                call_argc, call_argv, p, xctx);
        }

        /*
         * This may still be a call to a built in function if a variable is
         * assigned a built-in function definition.
         */
        else if (afw_value_is_function_definition(function_value)) {
            result = afw_value_call_built_in_function(
                self->args.contextual,
                (const afw_value_function_definition_t *)function_value,
                call_argc, call_argv, p, xctx);
        }

        /*
         * This may still be a call to a closure if a variable is assigned one.
         */
        else if (afw_value_is_closure_binding(function_value)) {
            result = afw_value_call_script_function(
                self->args.contextual,
                ((const afw_value_closure_binding_t *)function_value)->
                    script_function_definition,
                ((const afw_value_closure_binding_t *)function_value)->
                    enclosing_lexical_scope,
                call_argc, call_argv, p, xctx);
        }

        /* Otherwise, this must be a thunk call. */
        else if (afw_value_is_function_thunk(function_value)) {
            result = ((const afw_value_function_thunk_t *)function_value)->
                execute(
                    (const afw_value_function_thunk_t *)function_value,
                    call_argc, call_argv, p, xctx);
        }

        /* Not a value function value at this point. */
        else {
            AFW_THROW_ERROR_Z(general, "Invalid function_value", xctx);
        }
    }

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
    AFW_VALUE_SELF_T *self,
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

    afw_value_compiler_listing_begin_value(writer, &self->pub,
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

    if (self->optimized_value != &self->pub) {
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
    const afw_value_t *callee;

    callee = self->function_value;
    if (!callee && self->args.argv) {
        callee = self->args.argv[0];
    }
    afw_value_decompile_value(callee, writer, xctx);
    afw_value_decompile_call_args(writer, 1, &self->args, xctx);
}


/*
 * Expand call-site ...spreads (list_expression markers) into flat argv.
 */
AFW_DEFINE(void)
afw_value_call_args_expand_spreads(
    afw_size_t argc_in,
    const afw_value_t * const *argv_in,
    afw_size_t *argc_out,
    const afw_value_t * const **argv_out,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_size_t i;
    afw_size_t j;
    afw_size_t total;
    afw_size_t n;
    afw_boolean_t any_spread;
    const afw_value_t *arg;
    const afw_value_t *evaled;
    const afw_value_t **new_argv;
    const afw_array_t *arr;
    const afw_iterator_t *it;
    const afw_value_t *elem;

    *argc_out = argc_in;
    *argv_out = argv_in;
    if (!argv_in || argc_in == 0) {
        return;
    }

    any_spread = false;
    for (i = 1; i <= argc_in; i++) {
        if (argv_in[i] && afw_value_is_array_expression(argv_in[i])) {
            any_spread = true;
            break;
        }
    }
    if (!any_spread) {
        return;
    }

    /* Count expanded size. */
    total = 0;
    for (i = 1; i <= argc_in; i++) {
        arg = argv_in[i];
        if (arg && afw_value_is_array_expression(arg)) {
            evaled = afw_value_evaluate(arg, p, xctx);
            if (!afw_value_is_array(evaled)) {
                AFW_THROW_ERROR_Z(general,
                    "Call-site spread (...) requires an array", xctx);
            }
            arr = ((const afw_value_array_t *)evaled)->internal;
            n = 0;
            it = NULL;
            for (;;) {
                elem = afw_array_get_next_value(arr, &it, p, xctx);
                if (!elem) {
                    break;
                }
                n++;
            }
            total += n;
        }
        else {
            total++;
        }
    }

    new_argv = afw_pool_malloc(p,
        sizeof(afw_value_t *) * (total + 1), xctx);
    new_argv[0] = argv_in[0];
    j = 1;
    for (i = 1; i <= argc_in; i++) {
        arg = argv_in[i];
        if (arg && afw_value_is_array_expression(arg)) {
            evaled = afw_value_evaluate(arg, p, xctx);
            arr = ((const afw_value_array_t *)evaled)->internal;
            it = NULL;
            for (;;) {
                elem = afw_array_get_next_value(arr, &it, p, xctx);
                if (!elem) {
                    break;
                }
                new_argv[j++] = elem;
            }
        }
        else {
            new_argv[j++] = arg;
        }
    }

    *argc_out = total;
    *argv_out = new_argv;
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
    info->contextual = self->args.contextual;
    info->evaluated_data_type = self->evaluated_data_type;
    info->optimized_value = self->optimized_value;
}
