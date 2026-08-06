// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework — script compiler-internal runtime
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_compiler_internal.c
 * @brief Script IR runtime (not built-in Adaptive library formals).
 *
 * Contents:
 * - execute_* for Adaptive function category `compiler_internal` (function
 *   **ids** like const/let/assign stay stable for decompile round-trip).
 *   Documented for debug; not normal author surface syntax.
 * - Script formal evaluate: afw_function_script_evaluate_parameter_with_type
 *   (used only by afw_value_call_script_function).
 * - Pattern / assign bind helpers: afw_function_script_assign_pattern and
 *   const/let/assign type checks under unit typeCheck policy.
 *
 * Built-in Adaptive formals: afw_function.c / AFW_FUNCTION_EVALUATE_* only.
 */

#include "afw_internal.h"


static const afw_value_t *
impl_assign(
    const afw_value_t *target,
    const afw_value_t *value,
    afw_compile_internal_assignment_type_t assignment_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx);



static void
impl_assign_value(
    const afw_value_t *target,
    const afw_value_t *value,
    afw_compile_internal_assignment_type_t assignment_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/*
 * Evaluate a script-function formal (call_script_function only).
 *
 * When runtime typeCheck is on for contextual's unit: strict assignability.
 * Otherwise: leaf data_type convert (legacy). Not used by Adaptive built-ins.
 */
AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_function_script_evaluate_parameter_with_type(
    const afw_value_t *value,
    afw_size_t parameter_number,
    const afw_value_type_t *type,
    const afw_compile_value_contextual_t *contextual,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;
    const afw_data_type_t *want_dt;

    /* Leaf convert only; composites use type_check when enabled. */
    want_dt = NULL;
    if (type && type->kind == afw_value_type_kind_data_type) {
        want_dt = type->data_type;
    }

    if (type) {
        afw_value_type_check_call_arg_object_literal(type, value,
            "parameter", contextual, xctx);
    }

    if (afw_value_is_defined_and_evaluated(value) &&
        (!want_dt || want_dt == afw_data_type_any ||
            afw_value_get_data_type(value, xctx) == want_dt))
    {
        if (AFW_VALUE_TYPE_CHECK_RUNTIME_ENABLED(contextual, xctx)) {
            afw_value_type_check_assignable(type, value,
                "parameter", contextual, xctx);
        }
        return value;
    }

    afw_xctx_evaluation_stack_push_parameter_number(parameter_number, xctx);
    result = afw_value_evaluate(value, p, xctx);

    if (AFW_VALUE_TYPE_CHECK_RUNTIME_ENABLED(contextual, xctx)) {
        afw_value_type_check_assignable(type, result, "parameter",
            contextual, xctx);
    }
    else if (result && want_dt && want_dt != afw_data_type_any &&
        afw_value_get_data_type(result, xctx) != want_dt)
    {
        result = afw_value_convert(result, want_dt, true, p, xctx);
    }

    afw_xctx_evaluation_stack_pop_parameter_number(xctx);
    return result;
}


/* Pattern parameters and shared destructure bind paths. */
AFW_DEFINE_INTERNAL(void)
afw_function_script_assign_pattern(
    const afw_value_t *target,
    const afw_value_t *value,
    afw_compile_internal_assignment_type_t assignment_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    /*
     * Objects and arrays: no clone-on-bind — issue #17 faces
     * (wrap_literal_object / wrap_literal_array) isolate shared literals.
     */
    if (value && !afw_value_is_undefined(value)) {
        value = afw_value_evaluate(value, p, xctx);
    }
    impl_assign_value(target, value, assignment_type, p, xctx);
}


static const afw_value_t *
impl_create_closure_if_needed(
    const afw_value_script_function_definition_t *function,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_xctx_scope_t *scope;
    const afw_value_t *result;
    afw_size_t defining_depth;

    scope = afw_xctx_scope_current(xctx);

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

    if (defining_depth >= scope->block->depth) {
        for (; scope; scope = scope->parent_lexical_scope) {
            if (defining_depth == scope->block->depth) {
                break;
            }
        }
        if (!scope) {
            AFW_THROW_ERROR_Z(general,
                "Internal error: scope not found", xctx);
        }
        result = afw_value_closure_binding_create(function, scope, xctx);
    }
    else {
        result = &function->pub;
    }

    return result;
}



static void
impl_list_destructure(
    const afw_compile_assignment_target_t *at,
    const afw_compile_list_destructure_t *ld,
    const afw_value_t *value,
    afw_compile_internal_assignment_type_t assignment_type,
    const afw_compile_value_contextual_t *contextual,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_compile_assignment_element_t *ae;
    const afw_iterator_t *iterator;
    const afw_value_t *v;
    const afw_array_t *rest;
    afw_boolean_t eol;

    if (!afw_value_is_array(value)) {
        AFW_THROW_ERROR_Z(general,
            "Array destructure can only be performed on an array", xctx);
    }

    for (eol = false,
        iterator = NULL,
        ae = ld->assignment_element,
        v = NULL;
        ae;
        ae = ae->next)
    {
        if (!eol) {
            v = afw_array_get_next_value(
                ((const afw_value_array_t *)value)->internal,
                &iterator, p, xctx);
            if (!v) {
                eol = true;
            }
        }
        if (!ae->assignment_target) {
            continue; /* hole */
        }
        if (eol) {
            v = ae->default_value;
        }
        /* Missing element and no default → undefined (TS/ES-like). */
        if (!v) {
            v = afw_value_undefined;
        }
        /* Element annotation on Pattern (may also live on symbol->type). */
        if (ae->type && v && !afw_value_is_undefined(v)) {
            afw_value_type_check_assignable(ae->type, v,
                "list pattern element", contextual, xctx);
        }
        impl_assign_value(ae->assignment_target, v, assignment_type,
            p, xctx);
    }

    if (ld->rest) {
        rest = NULL;
        if (!eol) {
            for (;;) {
                v = afw_array_get_next_value(
                    ((const afw_value_array_t *)value)->internal,
                    &iterator, p, xctx);
                if (!v) {
                    break;
                }
                if (!rest) {
                    rest = afw_array_create_generic(p, xctx);
                }
                afw_array_push_value(rest, v, xctx);
            }
        }
        if (!rest) {
            rest = afw_array_create_generic(p, xctx);
        }
        v = afw_value_create_unmanaged_array(rest, p, xctx);
        if (ld->rest_type) {
            afw_value_type_check_assignable(ld->rest_type, v,
                "list pattern rest", contextual, xctx);
        }
        impl_assign_value(ld->rest, v, assignment_type, p, xctx);
    }
}



static void
impl_object_destructure(
    const afw_compile_assignment_target_t *at,
    const afw_compile_object_destructure_t *od,
    const afw_value_t *value,
    afw_compile_internal_assignment_type_t assignment_type,
    const afw_compile_value_contextual_t *contextual,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_compile_assignment_property_t *ap;
    const afw_object_t *object;
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;
    const afw_utf8_t *bound_name;
    const afw_value_t *v;
    const afw_value_t *name_v;
    const afw_object_t *rest;

    object = afw_value_as_object(value, xctx);

    /* Process assignment properties. */
    for (ap = od->assignment_property; ap; ap = ap->next)
    {
        if (ap->is_rename) {
            if (ap->property_name_expr) {
                name_v = afw_value_evaluate(ap->property_name_expr, p, xctx);
                bound_name = afw_value_as_utf8(name_v, p, xctx);
            }
            else {
                bound_name = ap->property_name;
            }
            v = bound_name
                ? afw_object_get_property(object, bound_name, xctx)
                : NULL;
            if (!v) {
                v = ap->assignment_element->default_value;
            }
            if (!v) {
                v = afw_value_undefined;
            }
            if (ap->assignment_element->type && v &&
                !afw_value_is_undefined(v))
            {
                afw_value_type_check_assignable(
                    ap->assignment_element->type, v,
                    "object pattern property", contextual, xctx);
            }
            impl_assign_value(ap->assignment_element->assignment_target, v,
                assignment_type, p, xctx);
        }
        else {
            v = afw_object_get_property(object,
                ap->symbol_reference->symbol->name, xctx);
            if (!v) {
                v = ap->default_value;
            }
            if (!v) {
                v = afw_value_undefined;
            }
            impl_assign_value(&ap->symbol_reference->pub,
                v, assignment_type, p, xctx);
        }
    }

    /* Add other properties to rest. */
    if (od->rest) {
        rest = afw_object_create(p, xctx);
        for (iterator = NULL;;) {
            v = afw_object_get_next_property(object, &iterator, &property_name,
                xctx);
            if (!v) {
                break;
            }

            for (ap = od->assignment_property; ap; ap = ap->next)
            {
                if (ap->is_rename) {
                    if (ap->property_name_expr) {
                        name_v = afw_value_evaluate(
                            ap->property_name_expr, p, xctx);
                        bound_name = afw_value_as_utf8(name_v, p, xctx);
                    }
                    else {
                        bound_name = ap->property_name;
                    }
                    if (bound_name &&
                        afw_utf8_equal(bound_name, property_name))
                    {
                        break;
                    }
                }
                else if (afw_utf8_equal(
                    ap->symbol_reference->symbol->name, property_name))
                {
                    break;
                }
            }
            if (!ap) {
                afw_object_set_property(rest, property_name, v, xctx);
            }
        }
        v = afw_value_create_unmanaged_object(rest, p, xctx);
        if (od->rest_type) {
            afw_value_type_check_assignable(od->rest_type, v,
                "object pattern rest", contextual, xctx);
        }
        impl_assign_value(od->rest, v, assignment_type, p, xctx);
    }
}



static void
impl_assignment_target(
    const afw_value_assignment_target_t *target,
    const afw_value_t *value,
    afw_compile_internal_assignment_type_t assignment_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_compile_assignment_target_t *at =
        target->assignment_target;
    const afw_value_block_symbol_t *symbol;
    const afw_compile_value_contextual_t *contextual = target->contextual;

    switch (at->target_type) {
    case afw_compile_assignment_target_type_list_destructure:
        if (afw_value_is_compiled_value(value)) {
            value = afw_value_evaluate(value, p, xctx);
        }
        impl_list_destructure(at, at->list_destructure, value,
            assignment_type, contextual, p, xctx);
        break;

    case afw_compile_assignment_target_type_object_destructure:
        if (afw_value_is_compiled_value(value)) {
            value = afw_value_evaluate(value, p, xctx);
        }
        impl_object_destructure(at, at->object_destructure, value,
            assignment_type, contextual, p, xctx);
        break;

    case afw_compile_assignment_target_type_symbol_reference:
        symbol = at->symbol_reference->symbol;
        if (symbol->type.kind != afw_value_type_kind_data_type ||
            symbol->type.data_type != afw_data_type_unevaluated)
        {
            value = afw_value_evaluate(value, p, xctx);
        }
        afw_value_type_check_assignable(&symbol->type, value,
            "assignment", contextual, xctx);
        afw_xctx_scope_symbol_set_value(symbol, value, xctx);
        break;

    case afw_compile_assignment_target_type_max_type:
    default:
        AFW_THROW_ERROR_FZ(code, xctx, "Invalid case %d", at->target_type);
    }
}


/*
 * Evaluate value then assign. Object/array clone-on-bind removed for issue
 * #17 (faces via wrap_literal_*).
 */
static const afw_value_t *
impl_assign(
    const afw_value_t *target,
    const afw_value_t *value,
    afw_compile_internal_assignment_type_t assignment_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_assignment_target_t *at;

    value = afw_value_evaluate(value, p, xctx);

    if (assignment_type == afw_compile_assignment_type_use_assignment_targets)
    {
        if (afw_value_is_assignment_target(target))
        {
            at = (const afw_value_assignment_target_t *)target;
            assignment_type = at->assignment_target->assignment_type;
        }
        /*
         * for-of / similar may pass a bare assignment target shape that is
         * already a symbol_reference or reference_by_key (e.g. for (x.y of …)).
         * Those assign via impl_assign_value without an assignment_target wrap.
         */
        else if (afw_value_is_symbol_reference(target) ||
            afw_value_is_reference_by_key(target))
        {
            assignment_type = afw_compile_assignment_type_assign_only;
        }
        else {
            AFW_THROW_ERROR_Z(general,
                "Left-hand side is not a valid assignment target", xctx);
        }
    }

    impl_assign_value(target, value, assignment_type, p, xctx);

    return value;
}



static void
impl_assign_value(
    const afw_value_t *target,
    const afw_value_t *value,
    afw_compile_internal_assignment_type_t assignment_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_assignment_target_t *at;

    if (afw_value_is_assignment_target(target)) {
        at = (const afw_value_assignment_target_t *)target;
        impl_assignment_target(at, value, assignment_type, p, xctx);
    }

    /*
     * If symbol reference, set value in scope. If this is a function definition
     * and not assigning to a function symbol slot, create a closure for it if
     * needed.
     */
    else if (afw_value_is_symbol_reference(target)) {
        const afw_value_symbol_reference_t *t =
            (afw_value_symbol_reference_t *)target;
        if (t->symbol->symbol_type != afw_value_block_symbol_type_function &&
            afw_value_is_script_function_definition(value))
        {
            value = impl_create_closure_if_needed(
                (const afw_value_script_function_definition_t *)value, p, xctx);
        }
        /*
         * Pattern leaves and object-destructure shorthand bind through bare
         * symbol_reference (not assignment_target). Enforce symbol->type here
         * so `const [a: integer] = …` / `{ name }` typed symbols are checked
         * under typeCheck (issue #28).
         */
        if (value && !afw_value_is_undefined(value)) {
            afw_value_type_check_assignable(&t->symbol->type, value,
                "assignment", t->contextual, xctx);
        }
        afw_xctx_scope_symbol_set_value(t->symbol, value, xctx);
    }

    /* Reference by key */
    else if (afw_value_is_reference_by_key(target)) {
        const afw_value_reference_by_key_t *t =
            (afw_value_reference_by_key_t *)target;
        const afw_object_t *object;
        const afw_array_t *list;
        const afw_value_t *key;
        const afw_value_t *aggregate_value;
        const afw_utf8_t *name;

        if (afw_value_is_script_function_definition(value)) {
            value = impl_create_closure_if_needed(
                (const afw_value_script_function_definition_t *)value, p, xctx);
        }

        aggregate_value = afw_value_evaluate(t->aggregate_value, p, xctx);
        key = afw_value_evaluate(t->key, p, xctx);

        if (afw_value_is_object(aggregate_value)) {
            object = ((const afw_value_object_t *)aggregate_value)->internal;
            if (afw_object_is_immutable(object, xctx)) {
                AFW_THROW_ERROR_Z(general, "Target object is immutable", xctx);
            }
            name = afw_value_is_string(key)
                ? &((const afw_value_string_t *)key)->internal
                : afw_value_as_utf8(key, p, xctx);
            afw_object_set_property(object, name, value, xctx);
        }

        else if (afw_value_is_array(aggregate_value)) {
            list = ((const afw_value_array_t *)aggregate_value)->internal;

            if (afw_array_is_immutable(list, xctx)) {
                AFW_THROW_ERROR_Z(general, "Target array is immutable", xctx);
            }

            if (!afw_value_is_integer(key)) {
                AFW_THROW_ERROR_Z(general, "Array index must be integer", xctx);
            }

            afw_array_set_value(list,
                ((const afw_value_integer_t *)key)->internal,
                value, xctx);
        }

        else {
            AFW_THROW_ERROR_Z(general, "Invalid assignment target", xctx);
        }

    }
    
    /* Invalid assignment target. */
    else {
        AFW_THROW_ERROR_Z(general, "Invalid assignment target", xctx);
    }

}



/* Use by for statement assignments. Result of evaluate is ignored. */
static const afw_value_t *
impl_evaluate_one_or_more_values(
    afw_function_execute_t *x,
    afw_size_t parameter_number,
    const afw_value_t *values,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;
    const afw_value_t *value;
    const afw_iterator_t *iterator;

    result = NULL;
    if (afw_value_is_array(values)) {
        iterator = NULL;
        for (;;) {
            value = afw_array_get_next_value(
                ((const afw_value_array_t *)values)->internal,
                &iterator, p, xctx);
            if (!value) {
                break;
            }
            result = afw_value_block_evaluate_statement(x, value, p, xctx);
        }
    }

    else {
        result = afw_value_block_evaluate_statement(x, values, p, xctx);
    }

    return result;
}




/*
 * Adaptive function: assign
 *
 * afw_function_execute_assign
 *
 * See afw_function_bindings.h for more information.
 *
 * Assign a value to the innermost structured block definition of a variable. If
 * the variable is not defined, the variable is defined in the innermost
 * structured block. An error is thrown if not called from an array of values
 * (statements) in a structured function.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function assign(
 *       name: string,
 *       value: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   name - (string) Variable name.
 *
 *   value - (any) This is the value to assign to the variable.
 *
 * Returns:
 *
 *   (any) The value assigned.
 */
const afw_value_t *
afw_function_execute_assign(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;
    const afw_value_t *result;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_IS(2);
    result = impl_assign(x->argv[1], AFW_FUNCTION_ARGV(2),
        afw_compile_assignment_type_assign_only,
        p, xctx);

    return result;
}



/*
 * Adaptive function: break
 *
 * afw_function_execute_break
 *
 * See afw_function_bindings.h for more information.
 *
 * This is a special function that can be called to break out of the body of a
 * loop. If called outside of a loop body, an error is thrown.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function break(
 *       value?: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   value - (optional any) The value to evaluate that the enclosing loop will
 *       return. If not specified, the last evaluated value or a null value will
 *       be returned.
 *
 * Returns:
 *
 *   (any) This function returns from the body of a loop with the last evaluated
 *       value.
 */
const afw_value_t *
afw_function_execute_break(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;
    const afw_value_t *result;

    result = afw_value_undefined;
    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(1);
    afw_xctx_statement_flow_set_type(break, xctx);
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(1)) {
        result = afw_function_evaluate_required_parameter(x, 1, NULL);
        if (afw_value_is_script_function_definition(result)) {
            result = impl_create_closure_if_needed(
                (const afw_value_script_function_definition_t *)result,
                p, xctx);
        }
    }

    return result;
}


/*
 * Adaptive function: const
 *
 * afw_function_execute_const
 *
 * See afw_function_bindings.h for more information.
 *
 * Define one or more statically scoped constants local to the current script
 * block with a permanent value. These constants can be accessed from the
 * current block and inner blocks, but can not be assigned a different value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function const(
 *       name: string[],
 *       value: any,
 *       type?: object // _AdaptiveValueMeta_
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   name - (string[]) The name of one or more constants to defined in the
 *       current block.
 *
 *   value - (any) This is the value of the constant(s).
 *
 *   type - (optional object _AdaptiveValueMeta_) The type of the constant(s).
 *
 * Returns:
 *
 *   (any) The value assigned.
 */
const afw_value_t *
afw_function_execute_const(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;
    const afw_value_t *result;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MIN(2);
    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(3);

    /** @fixme process type. */

    result = impl_assign(x->argv[1], AFW_FUNCTION_ARGV(2),
        afw_compile_assignment_type_const,
        p, xctx);

    return result;
}



/*
 * Adaptive function: continue
 *
 * afw_function_execute_continue
 *
 * See afw_function_bindings.h for more information.
 *
 * This is a special function that can be called in the body of a loop function
 * to test the condition and, if true, start evaluating the body again. If
 * called outside of a loop body, an error is thrown.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function continue(
 *   ): any;
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (any) This function does not return.
 */
const afw_value_t *
afw_function_execute_continue(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_IS(0);
    afw_xctx_statement_flow_set_type(continue, xctx);

    return afw_value_undefined;
}



/*
 * Adaptive function: do_while
 *
 * afw_function_execute_do_while
 *
 * See afw_function_bindings.h for more information.
 *
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function will evaluate an array of values (statements) at least once
 * while a condition is true. See the related functions 'break', 'continue',
 * 'return' and 'throw'.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function do_while(
 *       condition: boolean,
 *       body: array
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   condition - (boolean) While this condition is true, the loop will continue.
 *       This is evaluated in the loop's scope.
 *
 *   body - (array) This is an array of values (statements) that are evaluated
 *       for each iteration of the loop. Each value in body is evaluated in
 *       order until the end of the array or until a 'break', 'continue',
 *       'return' or 'throw' function is encountered.
 *
 * Returns:
 *
 *   (any) The last value evaluated in body or null if the body is empty.
 */
const afw_value_t *
afw_function_execute_do_while(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;
    const afw_value_t *result;
    const afw_value_boolean_t *condition;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_IS(2);
    for (;;) {
        result = afw_value_block_evaluate_statement(x, x->argv[2], p, xctx);
        if (afw_xctx_statement_flow_is_leave(xctx)) {
            break;
        }
        AFW_FUNCTION_EVALUATE_REQUIRED_CONDITION_PARAMETER(condition, 1);
        if (!condition->internal) {
            break;
        }
    }

    /* We don't want break/continue outside of this loop */
    afw_xctx_statement_flow_reset_break_and_continue(xctx);

    return result;
}



/*
 * Adaptive function: for
 *
 * afw_function_execute_for
 *
 * See afw_function_bindings.h for more information.
 *
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function loops while condition is true. If the condition is false for
 * the first iteration, the loop returns a null value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function for(
 *       initial?: array,
 *       condition?: boolean,
 *       increment?: array,
 *       body?: array
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   initial - (optional array) This is an array of values (statements) to
 *       evaluate before the loop starts. The values will normally be a call to
 *       the 'assign' function.
 *
 *   condition - (optional boolean) While this condition is true, the loop will
 *       continue.
 *
 *   increment - (optional array) This is an array of values (statements) to
 *       evaluate after each iteration of the loop. The values will normally be
 *       a call to the 'assign' function.
 *
 *   body - (optional array) This is an array of values (statements) that are
 *       evaluated for each iteration of the loop. Each value in body is
 *       evaluated in order until the end of the array or until a 'break',
 *       'continue', 'return' or 'throw' function is encountered.
 *
 * Returns:
 *
 *   (any) The last value evaluated in body or null if condition evaluates to
 *       false the first time.
 */
const afw_value_t *
afw_function_execute_for(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;
    const afw_value_boolean_t *condition;
    const afw_xctx_scope_t *scope;
    const afw_xctx_scope_t *previous_iterator_scope;
    const afw_value_t *result;
    const afw_value_t *increment;
    const afw_value_t *body;

    previous_iterator_scope = NULL;
    AFW_TRY{

        AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(4);

        if (AFW_FUNCTION_PARAMETER_IS_PRESENT(1)) {
            impl_evaluate_one_or_more_values(x, 1, x->argv[1], p, xctx);
        }

        increment = NULL;
        if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
            increment = x->argv[3];
        }

        body = NULL;
        if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4)) {
            body = x->argv[4];
        }

        for (result= afw_value_undefined;;) {

            if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
                AFW_FUNCTION_EVALUATE_REQUIRED_CONDITION_PARAMETER(condition,
                    2);
                if (!condition->internal) {
                    break;
                }
            }

            if (body) {
                result = afw_value_block_evaluate_statement(x, body, p, xctx);
                if (afw_xctx_statement_flow_is_leave(xctx)) {
                    break;
                }
            }

            if (increment) {
                //scope = previous_iterator_scope; // Get rid of warning.
                if (previous_iterator_scope) {
                    scope = afw_xctx_scope_clone(previous_iterator_scope, xctx);
                    afw_xctx_scope_deactivate(previous_iterator_scope, xctx);
                }
                else {
                    scope = afw_xctx_scope_clone(
                        afw_xctx_scope_current(xctx), xctx);
                }
                previous_iterator_scope = scope;
                afw_xctx_scope_activate(scope, xctx);
                impl_evaluate_one_or_more_values(x, 3, increment, p, xctx);
            }
        }
    }
    AFW_FINALLY{

        /* We don't want break/continue outside of this loop */
        afw_xctx_statement_flow_reset_break_and_continue(xctx);

        /* Release final increment scope. */
        if (previous_iterator_scope) {
            afw_xctx_scope_deactivate(previous_iterator_scope, xctx);
        }      
    }
    AFW_ENDTRY;

    return result;
}



/*
 * Adaptive function: for_of
 *
 * afw_function_execute_for_of
 *
 * See afw_function_bindings.h for more information.
 *
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function will evaluate an array of values (statements) while a condition
 * is true with initial and increment values. The condition is tested at the
 * beginning of the loop. If the condition is false for the first iteration, the
 * loop returns a null value. This supports for-of statement.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function for_of(
 *       name: string[],
 *       value: any,
 *       body?: array
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   name - (string[]) Variable name(s).
 *
 *   value - (any) Any array, object or single value.
 *
 *   body - (optional array) This is an array of values (statements) that are
 *       evaluated for each iteration of the loop. Each value in body is
 *       evaluated in order until the end of the array or until a 'break',
 *       'continue', 'return' or 'throw' function is encountered.
 *
 * Returns:
 *
 *   (any) The last value evaluated in body or null if condition evaluates to
 *       false the first time.
 */
const afw_value_t *
afw_function_execute_for_of(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;
    const afw_value_t *result;
    const afw_value_t *iterable;
    const afw_value_array_t *list;
    const afw_value_string_t *strv;
    const afw_iterator_t *iterator;
    const afw_value_t *value;
    afw_compile_internal_assignment_type_t assignment_type;
    afw_size_t offset;
    afw_code_point_t cp;
    afw_utf8_t one;
    afw_size_t start;
    afw_size_t n;

    result = afw_value_undefined;
    AFW_TRY{

        AFW_FUNCTION_ASSERT_PARAMETER_COUNT_IS(3);

        /* Evaluate head without forcing array (string for-of is special). */
        iterable = afw_function_evaluate_required_parameter(x, 2, NULL);

        assignment_type = afw_compile_assignment_type_use_assignment_targets;

        if (afw_value_is_string(iterable)) {
            strv = (const afw_value_string_t *)iterable;
            for (offset = 0; offset < strv->internal.len; ) {
                start = offset;
                cp = afw_utf8_next_code_point(strv->internal.s, &offset,
                    strv->internal.len, xctx);
                if (cp < 0) {
                    AFW_THROW_ERROR_Z(general,
                        "Invalid UTF-8 in for-of string", xctx);
                }
                /* One-character string for this code point (UTF-8 bytes). */
                n = offset - start;
                if (n == 0 || n > 4) {
                    AFW_THROW_ERROR_Z(general,
                        "Invalid UTF-8 code point length in for-of string",
                        xctx);
                }
                one.s = strv->internal.s + start;
                one.len = n;
                value = afw_value_create_unmanaged_string(&one, p, xctx);

                impl_assign(x->argv[1], value, assignment_type, p, xctx);
                assignment_type = afw_compile_assignment_type_assign_only;
                result = afw_value_block_evaluate_statement(
                    x, x->argv[3], p, xctx);
                if (afw_xctx_statement_flow_is_leave(xctx)) {
                    break;
                }
            }
        }
        else if (afw_value_is_array(iterable)) {
            list = (const afw_value_array_t *)iterable;
            for (iterator = NULL;;) {
                value = afw_array_get_next_value(
                    list->internal, &iterator, p, xctx);
                if (!value) {
                    break;
                }
                impl_assign(x->argv[1], value, assignment_type, p, xctx);
                assignment_type = afw_compile_assignment_type_assign_only;
                result = afw_value_block_evaluate_statement(
                    x, x->argv[3], p, xctx);
                if (afw_xctx_statement_flow_is_leave(xctx)) {
                    break;
                }
            }
        }
        else {
            AFW_THROW_ERROR_Z(general,
                "for-of head must be an array or string",
                xctx);
        }
    }
    AFW_FINALLY{

        /* We don't want break/continue outside of this loop */
        afw_xctx_statement_flow_reset_break_and_continue(xctx);

    }
    AFW_ENDTRY;

    return result;
}



/*
 * Adaptive function: if
 *
 * afw_function_execute_if
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate one of two different values depending on test condition.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function if(
 *       condition: boolean,
 *       then: array,
 *       else?: array
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   condition - (boolean) If true, parameter 'then' is evaluated for result. If
 *       false, parameter 'else' is evaluated.
 *
 *   then - (array) This is the body of a structured block that is evaluated if
 *       'condition' is true. See the 'body' parameter of the 'block' function
 *       for information on how the body is processed.
 *
 *   else - (optional array) This is the body of a structured block that is
 *       evaluated if 'condition' is false. If not specified and condition is
 *       false, a null value is returned. See the 'body' parameter of the
 *       'block' function for information on how the body is processed.
 *
 * Returns:
 *
 *   (any) The result of evaluating 'then' or 'else'.
 */
const afw_value_t *
afw_function_execute_if(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;
    const afw_value_t *result;
    const afw_value_boolean_t *condition;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MIN(2);
    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(3);

    AFW_FUNCTION_EVALUATE_REQUIRED_CONDITION_PARAMETER(condition, 1);
    result = afw_value_undefined;
    if (condition->internal) {
        result = afw_value_block_evaluate_statement(x, x->argv[2], p, xctx);
    }
    else if (x->argc == 3) {
        result = afw_value_block_evaluate_statement(x, x->argv[3], p, xctx);
    }

    return result;
}



/*
 * Adaptive function: let
 *
 * afw_function_execute_let
 *
 * See afw_function_bindings.h for more information.
 *
 * Declare one or more statically scoped variable locations local to the current
 * script block and optionally assign them an initial value. These variables can
 * be accessed and assigned different values from the current block and inner
 * blocks.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function let(
 *       name: string[],
 *       value?: any,
 *       type?: object // _AdaptiveValueMeta_
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   name - (string[]) The name of one or more variables to declared in the
 *       current block.
 *
 *   value - (optional any) This is the initial value of the variable(s). If not
 *       specified, the variable will have a value of undefined.
 *
 *   type - (optional object _AdaptiveValueMeta_) The type of the variable(s).
 *
 * Returns:
 *
 *   (any) The value assigned.
 */
const afw_value_t *
afw_function_execute_let(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;
    const afw_value_t *result;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MIN(1);
    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(3);

    result = impl_assign(x->argv[1], AFW_FUNCTION_ARGV(2),
        afw_compile_assignment_type_let,
        p, xctx);

    return result;
}



/*
 * Adaptive function: rethrow
 *
 * afw_function_execute_rethrow
 *
 * See afw_function_bindings.h for more information.
 *
 * This is a special function that can be called to rethrow an error inside of a
 * catch block. If called outside of a catch body, an error is thrown.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function rethrow(
 *   ): any;
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (any) This function rethrows the current error in a catch block.
 */
const afw_value_t *
afw_function_execute_rethrow(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(0);
    afw_xctx_statement_flow_set_type(rethrow, xctx);

    return afw_value_undefined;
}



/*
 * Adaptive function: return
 *
 * afw_function_execute_return
 *
 * See afw_function_bindings.h for more information.
 *
 * Return from the outermost structured block. If the expression of a lambda
 * function is a block function, this will effectively return from the lambda
 * function. If called outside of a structured block, an error is thrown.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function return(
 *       value?: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   value - (optional any) The value to evaluate that the outermost block will
 *       return. If not specified, the last evaluated value or a null value will
 *       be returned.
 *
 * Returns:
 *
 *   (any) This function returns from the outermost structured block with the
 *       last evaluated value.
 */
const afw_value_t *
afw_function_execute_return(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;
    const afw_value_t *result;

    result = afw_value_undefined;
    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(1);
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(1)) {
        /* NULL (undefined) is okay here. */
        result = afw_function_evaluate_parameter(x, 1, NULL);
        if (afw_value_is_script_function_definition(result)) {
            result = impl_create_closure_if_needed(
                (const afw_value_script_function_definition_t *)result,
                p, xctx);
        }

        /* return statement should not return NULL for undefined. */
        else if (!result) {
            result = afw_value_undefined;
        }
    }
    afw_xctx_statement_flow_set_type(return, xctx);

    return result;
}



/*
 * Adaptive function: wrap_literal_object
 *
 * afw_function_execute_wrap_literal_object
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate an object value, create a memory object wrapper
 * (afw_object_create_wrapper_*) over its instance, and return that wrapper as
 * an object value. Local property sets stay on the face; gets look through to
 * the shared base. Intended for compile/runtime isolation of object literals
 * (issue #17); not normal author surface syntax.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function wrap_literal_object(
 *       object: object
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   object - (object) Object to evaluate and wrap (typically a constant object
 *       literal once the compiler emits isolation).
 *
 * Returns:
 *
 *   (object) A new memory-wrapper object face over the evaluated base.
 */
const afw_value_t *
afw_function_execute_wrap_literal_object(
    afw_function_execute_t *x)
{
    const afw_value_object_t *object;
    const afw_object_t *wrap;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_IS(1);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object, 1, object);

    /*
     * Idempotent: compiler is the normal producer of this call; double wrap
     * (emit + explicit call, or nested call) returns the existing face.
     * Cross-pool re-home is not required for that path.
     */
    if (afw_object_is_memory_wrapper(object->internal)) {
        return (const afw_value_t *)object;
    }

    wrap = afw_object_create_wrapper_unmanaged(object->internal, x->p, x->xctx);
    return wrap->value;
}



/*
 * Adaptive function: wrap_literal_array
 *
 * afw_function_execute_wrap_literal_array
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate an array value, create a memory array wrapper
 * (afw_array_create_wrapper_*) over its instance, and return that wrapper as an
 * array value. Entry mutators stay on the face; nested objects/arrays are
 * promoted on get. Intended for compile/runtime isolation of array literals
 * (issue #17); not normal author surface syntax.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function wrap_literal_array(
 *       array: array
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array - (array) Array to evaluate and wrap (typically a constant array
 *       literal once the compiler emits isolation).
 *
 * Returns:
 *
 *   (array) A new memory-wrapper array face over the evaluated base.
 */
const afw_value_t *
afw_function_execute_wrap_literal_array(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array;
    const afw_array_t *wrap;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_IS(1);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);

    /* Idempotent: already a face — return as-is. */
    if (afw_array_is_memory_wrapper(array->internal)) {
        return (const afw_value_t *)array;
    }

    wrap = afw_array_create_wrapper_unmanaged(array->internal, x->p, x->xctx);
    return wrap->value;
}



/*
 * Adaptive function: switch
 *
 * afw_function_execute_switch
 *
 * See afw_function_bindings.h for more information.
 *
 * Support for switch statement.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function switch(
 *       predicate: (value1: any, value2: any) => boolean,
 *       value1: any,
 *       case_clause_1: any,
 *       case_clause_2: any,
 *       ...case_clause_rest: any[]
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   predicate - ((value1: any, value2: any) => boolean) The predicate is passed
 *       two parameters and must return a boolean. The first parameter passed is
 *       the evaluated value of the value1 parameter and the second is the
 *       value2 from a case clause. This predicate will often be 'eqx' to use
 *       the exactly equal function but can also be any other function such as
 *       'regexp_match' or a lambda function.
 *
 *   value1 - (any) The first parameter passed to the predicate.
 *
 *   case_clause - (2 or more any) This is one or more case clauses which are
 *       pairs of a value2 parameter followed by a statement list or undefined
 *       parameter. One value2 can be undefined to indicate the default case
 *       clause.
 * 
 *       For the first value2 that is undefined or calling the predicate returns
 *       true, the statement list followed by any statement lists of subsequent
 *       case clauses are executed until a break or return is encountered. The
 *       predicate is called with value1 and the case clause's value2.
 *
 * Returns:
 *
 *   (any)
 */
const afw_value_t *
afw_function_execute_switch(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;
    const afw_value_t *result;
    struct {
        const afw_value_t *function;
        const afw_value_t *value1;
        const afw_value_t *value2;
    } args;
    const afw_value_t *functor;
    const afw_value_t * const *end_of_args;
    const afw_value_t * const *pair;
    const afw_value_t * const *default_pair;     
    const afw_value_t *statement;
    const afw_array_t *statement_list;
    const afw_iterator_t *iterator;

    result = afw_value_undefined;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MIN(4);
    if ((x->argc & 1) != 0) {
        AFW_THROW_ERROR_Z(general,
            "Expecting an even number of parameters.", xctx);
    }

    /* Create skeleton functor call. */
    args.function = afw_function_evaluate_function_parameter(
        x->argv[1], p, xctx);
    AFW_FUNCTION_EVALUATE_PARAMETER(args.value1, 2);
    args.value2 = NULL; /* Filled in later. */
    functor = afw_value_call_create(afw_function_execute_contextual(x),
        2, (const afw_value_t **)&args, false, p, xctx);


    /* Find first true case. */
    for (end_of_args = x->argv + x->argc,
        pair = x->argv + 3,
        default_pair = NULL;
        pair < end_of_args;
        pair += 2)
    {
        args.value2 = pair[0];
        if (args.value2 == afw_value_unique_default_case_value) {
            default_pair = pair;
            continue;
        }
        result = afw_value_evaluate(functor, p, xctx);
        if (!afw_value_is_boolean(result)) {
            AFW_THROW_ERROR_Z(general,
                "Expecting functor to return a boolean value",
                xctx);
        }
        if (afw_value_is_boolean_true(result)) {
            break;
        }
    }

    /* If no true case, use default case if supplied. */
    if (pair >= end_of_args) {
        if (default_pair) {
            pair = default_pair;
        }
    }

    /*
     * Evaluate statements in first true and subsequent cases until a
     * break, rethrow or return is encountered.
     */
    for (;pair < end_of_args; pair += 2)
    {
        if (pair[1]) {
            if (!afw_value_is_array(pair[1])) {
                AFW_THROW_ERROR_Z(general,
                    "Expecting an array of statements",
                     xctx);
            }
            statement_list = ((const afw_value_array_t *)pair[1])->internal;
            for (iterator = NULL;;) {
                statement = afw_array_get_next_value(
                    statement_list, &iterator, p, xctx);
                if (!statement) {
                    break;
                }
                result = afw_value_block_evaluate_statement(
                    x, statement, p, xctx);
                if (afw_xctx_statement_flow_is_leave(xctx)) {
                    break;
                }
            }
            if (afw_xctx_statement_flow_is_leave(xctx)) {
                break;
            }
        }
    }

    /* We don't want break/continue outside of this switch */
    afw_xctx_statement_flow_reset_break_and_continue(xctx);

    return result;
}


/*
 * Adaptive function: throw
 *
 * afw_function_execute_throw
 *
 * See afw_function_bindings.h for more information.
 *
 * This throws an error that can be caught by a try/catch block. An error object
 * of object type _AdaptiveError_ will be available in the catch block. Its 'id'
 * property will be set to 'throw'. The other properties set based on the
 * parameters specified and where this function is called.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function throw(
 *       message: string,
 *       additional?: any
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   message - (string) This is the message that will be included in the
 *       _AdaptiveError_ error object available in the catch block.
 *
 *   additional - (optional any) Optional additional information that will be
 *       available as a 'additional' property in the error object.
 *
 * Returns:
 *
 *   (void)
 */
const afw_value_t *
afw_function_execute_throw(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_value_string_t *message;
    const afw_value_t *data;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MIN(1);
    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(2);

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(message, 1, string);
    AFW_FUNCTION_EVALUATE_PARAMETER(data, 2);

    AFW_THROW_ERROR_WITH_DATA_FZ(throw, data, xctx, AFW_UTF8_FMT,
        AFW_UTF8_FMT_ARG(&message->internal));

    return afw_value_undefined;
}



/*
 * Adaptive function: try
 *
 * afw_function_execute_try
 *
 * See afw_function_bindings.h for more information.
 *
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function will evaluate the body statements. If an error is thrown and
 * there is an optional catch, the error will be 'caught' and the associated
 * statements will be evaluated. The optional finally statements are always
 * evaluated after the body and catch statements. See the related functions
 * 'break', 'continue', 'return' and 'throw'.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function try(
 *       body: array,
 *       finally?: array,
 *       catch?: array,
 *       error?: object // _AdaptiveObjectType_
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   body - (array) This is an array of values (statements) that are evaluated.
 *       Each value in body is evaluated in order until the end of the list or
 *       until a 'break', 'continue', 'return' or 'throw' function is
 *       encountered.
 *
 *   finally - (optional array) This is an array of values (statements) that are
 *       evaluated after the try and catch statements even if an error occurs.
 *       Each value in body is evaluated in order until the end of the list or
 *       until a 'break', 'continue', 'return' or 'throw' function is
 *       encountered.
 *
 *   catch - (optional array) This is an array of values (statements) that are
 *       evaluated when an error is thrown while evaluating the body. Each value
 *       in body is evaluated in order until the end of the list or until a
 *       'break', 'continue', 'return' or 'throw' function is encountered.
 *
 *   error - (optional object _AdaptiveObjectType_) The error object thrown.
 *       This is only available in the catch block. See adaptive object type
 *       _AdaptiveObjectType_ for details.
 *
 * Returns:
 *
 *   (any) The last value evaluated in body.
 */
const afw_value_t *
afw_function_execute_try(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;
    const afw_value_t *result;
    const afw_value_t *this_result;
    const afw_object_t *error_object;
    const afw_value_t *error_value;
    const afw_xctx_scope_t *scope_at_entry;
    afw_xctx_statement_flow_t use_type;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MIN(2);
    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(4);

    result = afw_value_undefined;
    use_type = afw_xctx_statement_flow_get(xctx);

    scope_at_entry = afw_xctx_scope_current(xctx);
    AFW_TRY {
        result = afw_value_block_evaluate_statement(x, x->argv[1], p, xctx);
        use_type = afw_xctx_statement_flow_get(xctx);
    }

    AFW_CATCH_UNHANDLED {
        afw_xctx_scope_unwind(scope_at_entry, xctx);
        if AFW_FUNCTION_PARAMETER_IS_PRESENT(3) {
            /*
             * Catch body is always a block when there is a binding (arg 4
             * or Pattern reparse with bind as first statement). Plain catch
             * without a binding evaluates argv[3] as a statement list/block.
             */
            if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4) ||
                (afw_value_is_block(x->argv[3]) &&
                    ((const afw_value_block_t *)x->argv[3])->statement_count >
                        0 &&
                    afw_value_is_assignment_target(
                        ((const afw_value_block_t *)x->argv[3])->
                            statements[0])))
            {
                error_object = afw_error_to_object(&this_THROWN_ERROR, p, xctx);
                error_value = afw_value_create_unmanaged_object(
                    error_object, p, xctx);
                /// @fixme Assignment type is not correct when frames used
 // -------------------------------------------------------------------
 /// @fixme There needs to be a better way to do this. This is a copy of
 /// some of the code from afw_value_block_evaluate_block that had to be
 /// copied here so that the error object can be set in scope after the
 /// scope is created.
                const afw_xctx_scope_t *scope;
                const afw_value_block_t *block =
                    (const afw_value_block_t *)x->argv[3];
                scope = afw_xctx_scope_create(
                    block, afw_xctx_scope_current(xctx), xctx);
                afw_xctx_scope_activate(scope, xctx);
                AFW_TRY{
                    /*
                     * Error bind target resolution (issue #140 Patterns):
                     *
                     * 1) argv[4] string — decompile of identifier catch;
                     *    resolve name in the catch block.
                     * 2) argv[4] assignment_target / symbol_reference —
                     *    original compile (Pattern or identifier target).
                     * 3) No argv[4], first block statement is
                     *    #assignment_target — decompile of Pattern catch
                     *    embeds the Pattern as the first catch statement;
                     *    use it as the bind target and skip evaluating it
                     *    as a normal statement.
                     */
                    const afw_value_t *err_target = NULL;
                    int stmt_start = 0;

                    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4) &&
                        afw_value_is_string(x->argv[4]))
                    {
                        const afw_utf8_t *err_name =
                            &((const afw_value_string_t *)x->argv[4])->
                                internal;
                        afw_value_block_symbol_t *esym;
                        for (esym = block->first_entry; esym;
                            esym = esym->next_entry)
                        {
                            if (afw_utf8_equal(esym->name, err_name)) {
                                err_target =
                                    (const afw_value_t *)
                                    afw_value_symbol_reference_create(
                                        NULL, esym, p, xctx);
                                break;
                            }
                        }
                        if (!err_target) {
                            AFW_THROW_ERROR_FZ(general, xctx,
                                "try catch: error variable " AFW_UTF8_FMT_Q
                                " not found in catch block",
                                AFW_UTF8_FMT_ARG(err_name));
                        }
                    }
                    else if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4)) {
                        err_target = x->argv[4];
                    }
                    else if (block->statement_count > 0 &&
                        afw_value_is_assignment_target(block->statements[0]))
                    {
                        err_target = block->statements[0];
                        stmt_start = 1;
                    }

                    if (err_target) {
                        impl_assign_value(err_target, error_value,
                            afw_compile_assignment_type_let, p, xctx);
                    }
                    this_result = afw_value_undefined;
                    for (int i = stmt_start; i < block->statement_count; i++) {
                        this_result = afw_value_block_evaluate_statement(
                            x, block->statements[i], p, xctx);
                        if (!afw_xctx_statement_flow_is_type(sequential, xctx))
                        {
                            break;
                        }
                    }
                }
                AFW_FINALLY{
                    afw_xctx_scope_deactivate(scope, xctx);
                }
                AFW_ENDTRY;
// --------------------------------------------------------------------
            }
            else {
                this_result = afw_value_block_evaluate_statement(
                    x, x->argv[3], p, xctx);
            }
            if (afw_xctx_statement_flow_is_type(break, xctx) ||
                afw_xctx_statement_flow_is_type(continue, xctx))
            {
                use_type = afw_xctx_statement_flow_get(xctx);
            }
            else if (afw_xctx_statement_flow_is_type(return, xctx)) {
                use_type = afw_xctx_statement_flow_return;
                result = this_result;
            }
            else if (afw_xctx_statement_flow_is_type(rethrow, xctx)) {
                AFW_ERROR_RETHROW;
            }
        }
        else {
            AFW_ERROR_RETHROW;
        }
    }

    AFW_FINALLY {
        afw_xctx_scope_unwind(scope_at_entry, xctx);
        if AFW_FUNCTION_PARAMETER_IS_PRESENT(2) {
            this_result = afw_value_block_evaluate_statement(
                x, x->argv[2], p, xctx);
            if (afw_xctx_statement_flow_is_type(break, xctx) ||
                afw_xctx_statement_flow_is_type(continue, xctx))
            {
                use_type = afw_xctx_statement_flow_get(xctx);
                AFW_ERROR_MARK_CAUGHT;
            }
            else if (afw_xctx_statement_flow_is_type(return, xctx))
            {
                use_type = afw_xctx_statement_flow_return;
                result = this_result;
                AFW_ERROR_MARK_CAUGHT;
            }
            else if (afw_xctx_statement_flow_is_type(rethrow, xctx))
            {
                use_type = afw_xctx_statement_flow_sequential;
            }
        }
    }

    AFW_ENDTRY;

    afw_xctx_statement_flow_set(use_type, xctx);
    return result;
}



/*
 * Adaptive function: while
 *
 * afw_function_execute_while
 *
 * See afw_function_bindings.h for more information.
 *
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function will evaluate an array of values (statements) while a condition
 * is true. The condition is tested at the beginning of the loop. If the
 * condition is false for the first iteration, the loop returns a null value.
 * See the related functions 'break', 'continue', 'return' and 'throw'.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function while(
 *       condition: boolean,
 *       body: array
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   condition - (boolean) While this condition is true, the loop will continue.
 *       This is evaluated in the loop's scope.
 *
 *   body - (array) This is an array of values (statements) that are evaluated
 *       for each iteration of the loop. Each value in body is evaluated in
 *       order until the end of the list or until a 'break', 'continue',
 *       'return' or 'throw' function is encountered.
 *
 * Returns:
 *
 *   (any) The last value evaluated in body or null if condition evaluates to
 *       false the first time.
 */
const afw_value_t *
afw_function_execute_while(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;
    const afw_value_t *result;
    const afw_value_boolean_t *condition;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_IS(2);

    for (result = afw_value_undefined;;) {
        AFW_FUNCTION_EVALUATE_REQUIRED_CONDITION_PARAMETER(condition, 1);
        if (!condition->internal) {
            break;
        }
        result = afw_value_block_evaluate_statement(x, x->argv[2], p, xctx);
        if (afw_xctx_statement_flow_is_leave(xctx))
        {
            break;
        }
    }

    /* We don't want break/continue outside of this loop */
    afw_xctx_statement_flow_reset_break_and_continue(xctx);

    return result;
}
