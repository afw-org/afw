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
    afw_size_t argc,
    const afw_value_t * const * argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    /* Optimize is set to false since this is one time call. */
    value = afw_value_call_script_function_create(
        contextual, script_function_definition, argc, argv, false, p, xctx);
    return impl_afw_value_optional_evaluate(
        (AFW_VALUE_SELF_T *)value, p, xctx);
}



/* Create function for call value. */
AFW_DEFINE(const afw_value_t *)
afw_value_call_script_function_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_script_function_definition_t *script_function_definition,
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
    const afw_value_script_function_definition_t *l;
    const afw_value_t *result;
    const afw_value_t *value;
    afw_size_t parameter_number;
    const afw_value_script_function_parameter_t *const *params;
    const afw_value_t *const *arg;
    afw_name_value_t *cur;
    const afw_value_t *const *rest_argv;
    afw_xctx_scope_t *scope;
    const afw_xctx_scope_t *parent_static_scope;
    const afw_xctx_scope_t *scope_at_entry;
    afw_size_t rest_argc;
    const afw_array_t *rest_list;

    result = NULL;
    l = self->script_function_definition;


    /** @fixme This will all be cleaned up and rewritten. */

    /* This will need to change for closures. */
    /* Find enclosing static scope. */
    scope_at_entry = afw_xctx_scope_current(xctx);
    for (
        parent_static_scope = scope_at_entry;
        (
            parent_static_scope &&
            parent_static_scope->block->depth >= l->depth
        );
        parent_static_scope = parent_static_scope->parent_static_scope
    );
    if (!parent_static_scope ||
        scope_at_entry->block->depth < l->depth)
    {
        AFW_THROW_ERROR_Z(general,
            "Can not determine parent static scope for function",
            xctx);
    }

    /* Save stack top which will be restored on return. */
    AFW_TRY {

        /* If not a signature block, just switch to parent_static_scope. */
        if (!l->signature->block) {
            APR_ARRAY_PUSH(xctx->scope_stack, const afw_xctx_scope_t *) = 
                parent_static_scope;
        }

        /* If there is a signature, process it. */
        else {
            scope = afw_xctx_scope_begin(
                l->signature->block, parent_static_scope, xctx);

            /*
            * Push parameters without names onto stack. This is so
            * afw_function_evaluate_parameter_with_type() will not
            * see these parameters yet.
            * 
            * First argument starts at argv[1] so index in argv is the parameter
            * number.
            */
            for (parameter_number = 1,
                params = l->parameters,
                arg = self->args.argv + 1;
                parameter_number <= l->count;
                parameter_number++, params++, arg++)
            {
                /* If this is rest parameter ... */
                if ((*params)->is_rest) {

                    /* If extra unused parameters, pass them in rest object. */
                    if (self->args.argc >= l->count) {
                        rest_argc = self->args.argc - l->count + 1;
                        rest_argv = arg;
                    }

                    /* If no extra unused parameters, rest object is empty. */
                    else {
                        rest_argc = 0;
                        rest_argv = NULL;
                    }

                    /* Create rest list. */
                    rest_list = afw_array_const_create_array_of_values(
                        rest_argv, rest_argc, p, xctx);
                    value = afw_value_create_array(rest_list, p, xctx);
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

                cur = (afw_name_value_t *)apr_array_push(xctx->stack);
                cur->name = &afw_s_a_empty_string;
                cur->value = value;
            }

            /** @fixme This whole function is converted to use scope.*/
            /* Set parameter symbol values in scope. */
            for (
                cur = ((afw_name_value_t *)xctx->stack->elts) + scope->local_top,
                parameter_number = 1,
                params = l->parameters;
                parameter_number <= l->count;
                cur++,
                parameter_number++,
                params++)
            {
                scope->symbol_values[(*params)->symbol->index] = cur->value;
                cur->name = (*params)->name; // Old way also add support for destructuring.
            }

            /* If function named, set its symbol value in scope. */
            if (l->signature && l->signature->function_name_symbol) {
                scope->symbol_values[l->signature->function_name_symbol->index] =
                    (const afw_value_t *)self->script_function_definition; 
            }

            /*OLD> If there is a function name, make it a local variable. */
            if (l->signature && l->signature->function_name_value) {
                cur = (afw_name_value_t *)apr_array_push(xctx->stack);
                cur->name = &l->signature->function_name_value->internal;
                cur->value = (const afw_value_t *)self->script_function_definition;            
            }
        }

        /* Evaluate body. */
        result = afw_value_evaluate(l->body, p, xctx);
    }

    AFW_FINALLY{

        /* If there was a signature block, release it. */
        if (l->signature->block) {
            afw_xctx_scope_release(scope, xctx);
        }

        /* If there was a not signature block, get back to scope at entry. */
        else {
            apr_array_pop(xctx->scope_stack);
            if (scope_at_entry != afw_xctx_scope_current(xctx)) {
                AFW_THROW_ERROR_Z(general,
                    "Scope stack not restored to entry state",
                    xctx);
            }
        }
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
