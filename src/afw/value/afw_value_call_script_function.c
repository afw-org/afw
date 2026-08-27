// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for call_script_function
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file afw_value_call_script_function.c
 * @brief Call a script function (bind formals, evaluate body, return check).
 *
 * Formal evaluate/typeCheck: afw_function_script_evaluate_parameter_with_type
 * in afw_function_compiler_internal.c — not afw_function_evaluate_parameter.
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_get_reference NULL
#define impl_afw_value_get_assignable_value NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default
    
#define impl_afw_value_create_iterator NULL

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "call_script_function"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_call_script_function_inf
#define AFW_VALUE_SELF_T afw_value_call_script_function_t
#include "afw_value_impl_declares.h"



const afw_value_t *
afw_value_call_script_function(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_script_function_definition_t *script_function_definition,
    const afw_xctx_scope_t *enclosing_lexical_scope,
    afw_size_t argc,
    const afw_value_t * const * argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *value;

    /* Optimize is set to false since this is one time call. */
    value = afw_value_call_script_function_create(
        contextual, script_function_definition, enclosing_lexical_scope,
        argc, argv, false, p, xctx);
    return impl_afw_value_optional_evaluate(
        (AFW_VALUE_SELF_T *)value, p, xctx);
}



/* Create function for call value. */
AFW_DEFINE(const afw_value_t *)
afw_value_call_script_function_create(
    const afw_compile_value_contextual_t *contextual,
    const afw_value_script_function_definition_t *script_function_definition,
    const afw_xctx_scope_t *enclosing_lexical_scope,
    afw_size_t argc,
    const afw_value_t * const *argv,
    const afw_boolean_t allow_optimize,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    AFW_VALUE_SELF_T *self;
    afw_size_t i;
    const afw_value_script_function_parameter_t *param;

    self = afw_pool_calloc_type(p, AFW_VALUE_SELF_T, xctx);
    self->inf = &afw_value_call_script_function_inf;
    self->script_function_definition = script_function_definition;
    self->enclosing_lexical_scope = enclosing_lexical_scope;
    self->args.contextual = contextual;
    self->args.argc = argc;
    self->args.argv = argv;

    /*
     * Compile-time call-site checks when the callee is already known
     * (issue #28): object-literal args vs formal types.
     */
    if (AFW_VALUE_TYPE_CHECK_COMPILE_ENABLED(contextual, xctx) &&
        script_function_definition &&
        script_function_definition->parameters)
    {
        for (i = 0; i < script_function_definition->count && i < argc; i++) {
            param = script_function_definition->parameters[i];
            if (!param || !param->type || param->is_rest) {
                continue;
            }
            /* argv[0] is function; user args are argv[1..] */
            if (argv[i + 1]) {
                afw_value_type_check_compile_assignable(param->type,
                    argv[i + 1], "parameter", contextual, xctx);
            }
        }
    }
   
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
    const afw_value_script_function_definition_t *script;
    const afw_value_t *result;
    const afw_value_t *saved_script_result;
    afw_boolean_t saved_script_result_active;
    afw_boolean_t saved_script_result_written;
    const afw_xctx_scope_t *enclosing_lexical_scope;
    const afw_xctx_scope_t *parameter_scope;
    const afw_xctx_scope_t *caller_scope;
    const afw_value_t *value;
    const afw_value_script_function_parameter_t *const *params;
    const afw_value_t *const *arg;
    const afw_value_t *const *rest_argv;
    const afw_value_t * const *call_argv;
    const afw_array_t *rest_array;
    afw_size_t parameter_number;
    afw_size_t rest_argc;
    afw_size_t call_argc;
    afw_boolean_t parameter_scope_activated;

    result = NULL;
    saved_script_result = xctx->script_result;
    saved_script_result_active = xctx->script_result_active;
    saved_script_result_written = xctx->script_result_written;
    xctx->script_result = afw_value_undefined;
    xctx->script_result_active = true;
    xctx->script_result_written = false;
    caller_scope = afw_xctx_scope_current(xctx);
    parameter_scope = NULL;
    parameter_scope_activated = false;
    script = self->script_function_definition;

    /* Expand call-site ...spreads into a flat argv before binding. */
    afw_value_call_args_expand_spreads(
        self->args.argc, self->args.argv,
        &call_argc, &call_argv, p, xctx);

    /* If closure, use its enclosing static scope. */
    if (self->enclosing_lexical_scope) {
        enclosing_lexical_scope = self->enclosing_lexical_scope;
    }

    /*
     * If not closure, find the defining block on the caller lexical chain.
     * Param block's parent when present; else brace-body's parent
     * (`function() { … }` opens no param block). 0-symbol nested `{ }`
     * skip a scope, so match by block pointer through those parents.
     * Missing after that walk is still "not on the stack".
     */
    else {
        const afw_value_block_t *defining_block;

        defining_block = NULL;
        if (script->signature && script->signature->block) {
            defining_block = script->signature->block->parent_block;
            if (!defining_block) {
                defining_block = script->signature->block;
            }
        }
        else if (script->body && afw_value_is_block(script->body)) {
            defining_block =
                ((const afw_value_block_t *)script->body)->parent_block;
        }

        if (defining_block) {
            enclosing_lexical_scope = afw_xctx_scope_find_for_block(
                defining_block, caller_scope, xctx);
        }
        else {
            const afw_xctx_scope_t *from;

            enclosing_lexical_scope = NULL;
            for (from = caller_scope; from;
                from = from->parent_lexical_scope)
            {
                if (from->block &&
                    from->block->depth <= script->depth)
                {
                    enclosing_lexical_scope = from;
                    break;
                }
            }
            /*
             * Depth-only path has no skipped-frame block to walk. If the
             * caller is shallower than compile depth, this is the same
             * hole as a non-closure call after the defining function
             * returned.
             */
            if (enclosing_lexical_scope &&
                caller_scope && caller_scope->block &&
                caller_scope->block->depth < script->depth)
            {
                enclosing_lexical_scope = NULL;
            }
        }
        if (!enclosing_lexical_scope) {
            AFW_THROW_ERROR_Z(general,
                "Can not determine parent static scope for function",
                xctx);
        }
    }

    /* Save stack top which will be restored on return. */
    AFW_TRY {

        /* If there is a parameter block, make a parameter scope. */
        if (script->signature && script->signature->block) {

            /* Make a scope for parameters. */
            parameter_scope = afw_xctx_scope_create(
                script->signature->block, enclosing_lexical_scope, xctx);

            /*
             * Parameter binding (TS/ES-like, recursive-safe).
             *
             * --- Indexing (same argv layout as built-in adaptive functions) ---
             *   self->args.argv[0]              — this script function
             *   self->args.argv[1..argc]        — user args (1-based param #)
             *   self->args.argc                 — user arg count (no argv[0])
             *   script->parameters[0..count-1]  — formals (0-based C array)
             *   script->count                   — formal count
             *
             * So formal parameters[i] pairs with parameter_number (i+1) and
             * argv[i+1]. Loops often use parameter_number from 1 and
             * `arg = argv + 1`, or 0-based i with argv[i+1] / "Parameter i+1".
             * See afw_value_call_args_s / AFW_FUNCTION_ARGV.
             *
             * --- Bind order ---
             * 1) Evaluate *provided* arg expressions while the *caller*
             *    scope is still current. Activating the callee scope first
             *    broke recursion (hanoi: `num - 1` saw empty callee slots).
             * 2) Activate the parameter scope.
             * 3) Apply defaults (parameter scope so earlier params are
             *    visible, e.g. `function (x, y = x)`), then store simple
             *    names or run Pattern destructure.
             *
             * Zero-parameter functions still have a parameter block (name /
             * body parent); do not afw_pool_calloc a 0-byte bound_values.
             */
            {
                const afw_value_t **bound_values;
                afw_size_t i;
                afw_boolean_t provided;

                /* 0-based parallel to parameters[]; length script->count. */
                bound_values = NULL;
                if (script->count > 0) {
                    bound_values = afw_pool_calloc(p,
                        sizeof(afw_value_t *) * script->count, xctx);
                }

                /*
                 * Pass 1: evaluate only provided args in caller scope.
                 * parameter_number is 1-based; arg walks argv[1], argv[2], …
                 */
                for (parameter_number = 1,
                    params = script->parameters,
                    arg = call_argv + 1;
                    parameter_number <= script->count;
                    parameter_number++, params++, arg++)
                {
                    value = NULL;
                    if ((*params)->is_rest) {
                        if (call_argc >= script->count) {
                            rest_argc = call_argc - script->count + 1;
                            rest_argv = arg;
                        }
                        else {
                            rest_argc = 0;
                            rest_argv = NULL;
                        }
                        rest_array = afw_array_const_create_array_of_values(
                            rest_argv, rest_argc, p, xctx);
                        value = afw_value_create_unmanaged_array(
                            rest_array, p, xctx);
                    }
                    else if (parameter_number <= call_argc) {
                        /* Script formal: type graph + unit typeCheck policy. */
                        value =
                            afw_function_script_evaluate_parameter_with_type(
                                *arg, parameter_number,
                                (*params)->type,
                                script->contextual
                                    ? script->contextual
                                    : self->args.contextual,
                                p, xctx);
                    }
                    /* parameters[parameter_number - 1] ← this value */
                    bound_values[parameter_number - 1] = value;
                }

                /* Pass 2: activate, defaults, store / Pattern (0-based i). */
                afw_xctx_scope_activate(parameter_scope, xctx);
                parameter_scope_activated = true;

                for (i = 0, params = script->parameters;
                    i < script->count;
                    i++, params++)
                {
                    value = bound_values[i];
                    /* provided: caller supplied argv[i+1] (or rest). */
                    provided = (i + 1 <= call_argc) ||
                        (*params)->is_rest;

                    if ((*params)->is_rest) {
                        /* value already built in pass 1 */
                    }
                    else if (afw_value_is_undefined(value)) {
                        if ((*params)->default_value) {
                            /*
                             * Default evaluates with parameter scope active
                             * so prior parameters are visible (y = x).
                             */
                            value = afw_value_evaluate(
                                (*params)->default_value, p, xctx);
                        }
                        else if (!(*params)->is_optional && !provided) {
                            /* Human-facing parameter numbers are 1-based. */
                            AFW_THROW_ERROR_FZ(general, xctx,
                                "Parameter " AFW_SIZE_T_FMT " is required",
                                i + 1);
                        }
                    }

                    if ((*params)->assignment_target) {
                        if (!afw_value_is_undefined(value)) {
                            afw_function_script_assign_pattern(
                                (*params)->assignment_target, value,
                                afw_compile_assignment_type_parameter,
                                p, xctx);
                        }
                        /* else: leaves stay scope_create undefined (issue #131) */
                    }
                    else if ((*params)->symbol) {
                        /* NULL / missing optional → permanent undefined singleton. */
                        afw_value_slot_store(
                            afw_xctx_scope_symbol_get_value_address(
                                (*params)->symbol, parameter_scope, xctx),
                            value, p, xctx);
                    }
                }
            }

            /* If named function, set its symbol in parameter scope. */
            if (script->signature->function_name_symbol) {
                afw_xctx_scope_symbol_set_value(
                    script->signature->function_name_symbol,
                    (const afw_value_t *)script, xctx);
            }
        }

        /* If no parameters, just activate enclosing scope. */
        else {
            afw_xctx_scope_activate(enclosing_lexical_scope, xctx);
        }

        /* Brace body: same running-result rule as a script (issue #62). */
        if (afw_value_is_block(script->body)) {
            afw_function_execute_t exec;

            afw_memory_clear(&exec);
            exec.p = p;
            exec.xctx = xctx;
            result = afw_value_block_evaluate_block(&exec,
                (const afw_value_block_t *)script->body, p, xctx,
                false);
        }
        else {
            result = afw_value_evaluate(script->body, p, xctx);
        }
        /*
         * Declared : void is a procedure: discard the running result and
         * return the void singleton so a call statement does not write
         * (same as print()). Untyped empty body (void from let) still
         * becomes undefined.
         */
        if (script->returns &&
            afw_value_type_get_leaf_data_type(script->returns) ==
                afw_data_type_void)
        {
            result = afw_value_void;
        }
        else if (afw_value_is_void(result)) {
            result = afw_value_undefined;
        }

        /* Runtime return type check (issue #28): definition unit, else call. */
        {
            const afw_compile_value_contextual_t *check_ctx;

            check_ctx = script->contextual
                ? script->contextual
                : self->args.contextual;
            if (script->returns &&
                AFW_VALUE_TYPE_CHECK_RUNTIME_ENABLED(check_ctx, xctx) &&
                result)
            {
                afw_value_type_check_assignable(script->returns, result,
                    "return", check_ctx, xctx);
            }
        }

        /*
         * Return temp for a real occupant. void/undefined stay as-is so a
         * : void procedure is the void singleton and does not write
         * script_result.
         */
        if (result &&
            !afw_value_is_undefined(result) &&
            !afw_value_is_void(result) &&
            !afw_value_is_function_return_value(result))
        {
            result = afw_value_function_return_value_create(
                result,
                xctx->evaluation_heap ? xctx->evaluation_heap : xctx->p,
                xctx);
        }
        if (result &&
            !afw_value_is_undefined(result) &&
            !afw_value_is_void(result))
        {
            xctx->script_result = result;
            xctx->script_result_written = true;
        }
    }

    AFW_FINALLY{

        /* If there was a parameters scope, ... */
        if (parameter_scope)
        {
            /* If parameter_scope activated, deactivate. */
            if (parameter_scope_activated) {
                afw_xctx_scope_deactivate(parameter_scope, xctx);
            }

            /* If it didn't make it as far as activation, just release. */
            else {
                afw_xctx_scope_release(parameter_scope, xctx);
            }

        }

        /* If no parameter scope, just deactivate enclosing scope. */
        else {
            afw_xctx_scope_deactivate(enclosing_lexical_scope, xctx);
        }

        afw_xctx_statement_flow_reset_all_except_rethrow(xctx);
        afw_xctx_script_result_restore(
            saved_script_result,
            saved_script_result_active,
            saved_script_result_written,
            xctx);
    }

    AFW_ENDTRY;

    /* Make sure we're back in caller's scope. */
    if (caller_scope != afw_xctx_scope_current(xctx)) {
        AFW_THROW_ERROR_Z(general,
            "Caller scope not current on return from function",
            xctx);
    }

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
    const afw_utf8_t *name;
    const afw_value_t *callee;

    name = NULL;
    if (self->script_function_definition &&
        self->script_function_definition->signature &&
        self->script_function_definition->signature->function_name_value)
    {
        name = &self->script_function_definition->signature->
            function_name_value->internal;
    }

    if (name) {
        afw_writer_write_utf8(writer, name, xctx);
    }
    else {
        callee = self->args.argv ? self->args.argv[0] : NULL;
        if (!callee && self->script_function_definition) {
            callee = (const afw_value_t *)self->script_function_definition;
        }
        afw_value_decompile_value(callee, writer, xctx);
    }
    afw_value_decompile_call_args(writer, 1, &self->args, xctx);
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
