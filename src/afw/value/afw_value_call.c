// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for call
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_value_call.c
 * @brief Implementation of afw_value interface for call
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_get_reference NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "call"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_call_inf
#include "afw_value_impl_declares.h"



static const afw_value_t *
impl_call_script_function(
    const afw_value_t *lambda,
    afw_size_t argc,
    const afw_value_t * const * argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_script_function_definition_t *l;
    const afw_value_t *result;
    const afw_value_t *value;
    afw_size_t parameter_number;
    const afw_value_script_function_parameter_t *const *params;
    const afw_value_t *const *arg;
    afw_name_value_t *cur;
    const afw_value_t *const *rest_argv;
    afw_size_t rest_argc;
    const afw_list_t *rest_list;
    int local_top;

    result = NULL;
    if (!afw_value_is_script_function(lambda)) {
        AFW_THROW_ERROR_Z(general,
            "impl_call_script_function() must be called with script function",
            xctx);
    }
    l = (const afw_value_script_function_definition_t *)lambda;
    if (!afw_value_is_script_function(lambda)) {
        lambda = afw_value_evaluate(lambda, p, xctx);
        if (!afw_value_is_script_function(lambda)) {
            AFW_THROW_ERROR_Z(general,
                "impl_call_script_function() expects lambda value",
                xctx);
        }
        l = (const afw_value_script_function_definition_t *)lambda;
    }

    /* Save stack top which will be restored on return. */
    local_top = afw_xctx_begin_stack_frame(xctx);
    AFW_TRY {

        /*
         * Push parameters without names onto stack. This is so
         * afw_function_evaluate_parameter_with_type() will not
         * see these parameters yet.
         * 
         * First argument starts at argv[1] so index in argv is the parameter
         * number.
         */
        for (parameter_number = 1, params = l->parameters, arg = argv + 1;
            parameter_number <= l->count;
            parameter_number++, params++, arg++)
        {
            /* If this is rest parameter ... */
            if ((*params)->is_rest) {

                /* If extra unused parameters, pass them in rest object. */
                if (argc >= l->count) {
                    rest_argc = argc - l->count + 1;
                    rest_argv = arg;
                }

                /* If no extra unused parameters, rest object is empty. */
                else {
                    rest_argc = 0;
                    rest_argv = NULL;
                }

                /* Create rest list. */
                rest_list = afw_list_const_create_array_of_values(
                    rest_argv, rest_argc, p, xctx);
                value = afw_value_create_list(rest_list, p, xctx);
            }

            /* If not rest parameter */
            else {
                value = NULL;
                if (parameter_number <= argc) {
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

            cur = (afw_name_value_t *)apr_array_push(xctx->stack);
            cur->name = &afw_s_a_empty_string;
            cur->value = value;
        }

        /* Set parameter names so they will be seen as local variables. */
        for (
            cur = ((afw_name_value_t *)xctx->stack->elts) + local_top,
            parameter_number = 1,
            params = l->parameters;
            parameter_number <= l->count;
            cur++,
            parameter_number++,
            params++)
        {
            cur->name = (*params)->name;
        }

        /* If there is a function name, make it a local variable. */
        if (l->signature && l->signature->function_name_value) {
            cur = (afw_name_value_t *)apr_array_push(xctx->stack);
            cur->name = &l->signature->function_name_value->internal;
            cur->value = lambda;            
        }

        /* Evaluate body. */
        result = afw_value_evaluate(l->body, p, xctx);
    }

    AFW_FINALLY{

        /* Restore xctx stack top to what it was on entry. */
        afw_xctx_end_stack_frame(local_top, xctx);

    }

    AFW_ENDTRY;

    /* Return result. */
    return result;
}



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
    

    return (const afw_value_t *)self;
}



/*
 * Implementation of method optional_evaluate for interface afw_value.
 */
const afw_value_t *
impl_afw_value_optional_evaluate(
    const afw_value_t * instance,
    const afw_pool_t * p,
    afw_xctx_t *xctx)
{
    const afw_value_call_t *self =
        (const afw_value_call_t *)instance;
    const afw_compile_value_contextual_t *saved_contextual;
    const afw_value_t *function_value;
    const afw_value_t *result;

    /* Push value on evaluation stack. */
    afw_xctx_evaluation_stack_push_value(instance, xctx);
    saved_contextual = xctx->error->contextual;
    xctx->error->contextual = self->args.contextual;

    /*
     * The function value may need to be evaluated since it's likely to be
     * a variable reference or something else that needs to be evaluated
     * first.
     */
    function_value = afw_value_evaluate(self->function_value, p, xctx);

    /*
     * @fixme 
     * 
     * This extra evaluate was put in for some reason but I'm not sure it's
     * needed anymore. I've left it here commented out in case that need is
     * discovered.
     * 
     * The original fixme comment was:
     *     this should be done at the time variable is defined???
     */
    //if (afw_value_is_compiled_value(function_value)) {
        //function_value = afw_value_evaluate(function_value, p, xctx);
    //}

    /*
     * This is most likely a script function call since built-ins are usually
     * handled by afw_value_built_in_function. Check and handle for script
     * functions first.
     */
    if (afw_value_is_script_function(function_value)) {
        result = impl_call_script_function(function_value,
            self->args.argc, self->args.argv, p, xctx);
    }

    /*
     * This may still be a call to a built in function if a variable is assigned
     * a built-in function definition.
     */
    else if (afw_value_is_function_definition(function_value)) {
        result = afw_value_call_built_in_function(
            self->args.contextual,
            (const afw_value_function_definition_t *)function_value,
            self->args.argc, self->args.argv, p, xctx);
    }

    /* Otherwise, this must be a thunk call. */
    else if (afw_value_is_function_thunk(function_value)) {
        result = ((const afw_value_function_thunk_t *)function_value)->
            execute(
                (const afw_value_function_thunk_t *)function_value,
                self->args.argc, self->args.argv, p, xctx);
    }
    else {
        AFW_THROW_ERROR_Z(general, "Invalid function_value", xctx);
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
    const afw_value_t * instance,
    afw_xctx_t *xctx)
{
    return NULL;
}


/*
 * Implementation of method decompile for interface afw_value.
 */
void
impl_afw_value_produce_compiler_listing(
    const afw_value_t *instance,
    const afw_writer_t *writer,
    afw_xctx_t *xctx)
{
    const afw_value_call_t *self =
        (const afw_value_call_t *)instance;

    afw_value_compiler_listing_begin_value(writer, instance,
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

    if (self->optimized_value != instance) {
        afw_writer_write_z(writer, "optimized_value: ", xctx);
        afw_value_produce_compiler_listing(self->optimized_value, writer, xctx);
        afw_writer_write_eol(writer, xctx);
    }

    afw_value_compiler_listing_call_args(writer, &self->args, xctx);

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
    /*FIXME

    const afw_value_call_t *self =
        (const afw_value_call_t *)instance;

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
    const afw_value_t *instance,
    afw_value_info_t *info,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_call_t *self =
        (const afw_value_call_t *)instance;

    afw_memory_clear(info);
    info->value_inf_id = &instance->inf->rti.implementation_id;
    info->contextual = self->args.contextual;
    info->evaluated_data_type = self->evaluated_data_type;
    info->optimized_value = self->optimized_value;
}
