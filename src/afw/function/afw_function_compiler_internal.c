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


static inline const afw_value_t *
impl_optional_loop_label(afw_function_execute_t *x, afw_size_t n)
{
    const afw_value_t *v;

    if (!AFW_FUNCTION_PARAMETER_IS_PRESENT(n)) {
        return NULL;
    }
    v = AFW_FUNCTION_ARGV(n);
    if (!afw_value_is_string(v)) {
        v = afw_function_evaluate_required_parameter(x, n,
            afw_data_type_string);
    }
    return v;
}


static inline afw_boolean_t
impl_loop_is_flow_target(const afw_value_t *this_label, afw_xctx_t *xctx)
{
    const afw_value_t *target;

    if (!afw_xctx_statement_flow_is_type(break, xctx) &&
        !afw_xctx_statement_flow_is_type(continue, xctx))
    {
        return false;
    }
    target = xctx->statement_flow_label;
    if (!target) {
        return true;
    }
    if (!this_label) {
        return false;
    }
    if (this_label == target) {
        return true;
    }
    if (afw_value_is_string(this_label) && afw_value_is_string(target)) {
        return afw_utf8_equal(
            &((const afw_value_string_t *)this_label)->internal,
            &((const afw_value_string_t *)target)->internal);
    }
    return false;
}


static inline void
impl_loop_consume_if_target(const afw_value_t *this_label, afw_xctx_t *xctx)
{
    if (impl_loop_is_flow_target(this_label, xctx)) {
        afw_xctx_statement_flow_reset_break_and_continue(xctx);
    }
}


/* True if this loop should stop iterating (propagate or consume break). */
static inline afw_boolean_t
impl_loop_should_exit(const afw_value_t *this_label, afw_xctx_t *xctx)
{
    if (afw_xctx_statement_flow_is_type(return, xctx) ||
        afw_xctx_statement_flow_is_type(rethrow, xctx))
    {
        return true;
    }
    if (afw_xctx_statement_flow_is_type(break, xctx)) {
        impl_loop_consume_if_target(this_label, xctx);
        return true;
    }
    if (afw_xctx_statement_flow_is_type(continue, xctx)) {
        if (impl_loop_is_flow_target(this_label, xctx)) {
            impl_loop_consume_if_target(this_label, xctx);
            return false;
        }
        return true;
    }
    return false;
}


/* Keep previous if this iteration/body did not complete (void). */
static inline const afw_value_t *
impl_update_empty(
    const afw_value_t *previous,
    const afw_value_t *incoming)
{
    if (!incoming || afw_value_is_void(incoming)) {
        return previous;
    }
    return incoming;
}


/*
 * UpdateEmpty for statement built-ins (while/for/try/switch): the
 * body's last non-void bubbles to the parent list, including
 * undefined. Only void does not wipe. return/rethrow keep their
 * value. if is ternary and already returns then/else directly.
 */
static inline const afw_value_t *
impl_statement_result_or_void(
    const afw_value_t *result,
    afw_xctx_t *xctx)
{
    if (afw_xctx_statement_flow_is_type(return, xctx) ||
        afw_xctx_statement_flow_is_type(rethrow, xctx))
    {
        return result;
    }
    if (!result || afw_value_is_void(result)) {
        return afw_value_void;
    }
    return result;
}


/* try: void except return/rethrow. Loops use impl_keep_loop_last. */
static inline const afw_value_t *
impl_keep_if_return(
    const afw_value_t *result,
    const afw_value_t *body_result,
    afw_xctx_t *xctx)
{
    if (afw_xctx_statement_flow_is_type(return, xctx) ||
        afw_xctx_statement_flow_is_type(rethrow, xctx))
    {
        return body_result;
    }
    return result;
}


/* Loop body already slot_stored this last. */
static inline const afw_value_t *
impl_keep_loop_last(
    const afw_value_t *result,
    const afw_value_t *body_result,
    afw_xctx_t *xctx)
{
    if (afw_xctx_statement_flow_is_type(return, xctx) ||
        afw_xctx_statement_flow_is_type(rethrow, xctx))
    {
        return body_result;
    }
    if (body_result &&
        body_result == xctx->script_result &&
        !afw_value_is_void(body_result))
    {
        return body_result;
    }
    return result;
}


/* Formal expects array of values: leaf array, T[], or tuple (#153). */
static inline afw_boolean_t
impl_script_formal_expects_array_sequence(const afw_value_type_t *type)
{
    if (!type) {
        return false;
    }
    if (type->kind == afw_value_type_kind_data_type &&
        type->data_type == afw_data_type_array)
    {
        return true;
    }
    if (type->kind == afw_value_type_kind_array ||
        type->kind == afw_value_type_kind_tuple)
    {
        return true;
    }
    return false;
}


/*
 * Evaluate a script-function formal (call_script_function only).
 *
 * When runtime typeCheck is on for contextual's unit: strict assignability.
 * Otherwise: leaf data_type convert (legacy). Not used by Adaptive built-ins.
 *
 * Array-shaped formals accept utf8 code-point sequences via
 * afw_value_as_array_sequence (#153), same language rule as built-ins.
 */
const afw_value_t *
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
    afw_boolean_t wants_array_sequence;

    /* Leaf convert only; composites use type_check when enabled. */
    want_dt = NULL;
    if (type && type->kind == afw_value_type_kind_data_type) {
        want_dt = type->data_type;
    }
    wants_array_sequence = impl_script_formal_expects_array_sequence(type);

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

    result = afw_value_evaluate_and_park(value, parameter_number, p, xctx);
    if (result && afw_value_is_compiled_value(result) &&
        want_dt != afw_data_type_unevaluated)
    {
        result = afw_value_evaluate(result, p, xctx);
    }

    /* #153: materialize utf8 sequences before check/convert. */
    if (wants_array_sequence) {
        result = afw_value_as_array_sequence(result, p, xctx);
    }

    if (AFW_VALUE_TYPE_CHECK_RUNTIME_ENABLED(contextual, xctx)) {
        afw_value_type_check_assignable(type, result, "parameter",
            contextual, xctx);
    }
    else if (result && want_dt && want_dt != afw_data_type_any &&
        afw_value_get_data_type(result, xctx) != want_dt)
    {
        result = afw_value_convert(result, want_dt, true, p, xctx);
    }

    return result;
}


/* Pattern parameters and shared destructure bind paths. */
void
afw_function_script_assign_pattern(
    const afw_value_t *target,
    const afw_value_t *value,
    afw_compile_internal_assignment_type_t assignment_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    /*
     * Store-time bind for script functions is clone_or_reference on the
     * script_function inf (slot_store). Faces isolate object/array literals.
     */
    if (value && !afw_value_is_undefined(value)) {
        value = afw_value_evaluate_and_park(value, 1, p, xctx);
    }
    impl_assign_value(target, value, assignment_type, p, xctx);
}


/*
 * Pattern leaves bind through a bare symbol_reference, which stores the
 * value as-is. Parameter defaults already evaluate before bind; Pattern
 * element/property defaults must too, or `const [a = make()] = []` stores
 * the call node and make() runs on every use.
 */
static inline const afw_value_t *
impl_evaluate_pattern_default(
    const afw_value_t *default_value,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    if (!default_value) {
        return afw_value_undefined;
    }
    return afw_value_evaluate_and_park(default_value, 1, p, xctx);
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
    const afw_iterator_old_t *iterator;
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
            v = impl_evaluate_pattern_default(ae->default_value, p, xctx);
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
                    rest = afw_array_create_unmanaged(p, xctx);
                }
                afw_array_push_value(rest, v, xctx);
            }
        }
        if (!rest) {
            rest = afw_array_create_unmanaged(p, xctx);
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
    const afw_iterator_old_t *iterator;
    const afw_value_t *property_name;
    const afw_value_t *bound_name;
    const afw_value_t **resolved_names;
    const afw_value_t *v;
    const afw_value_t *name_v;
    const afw_object_t *rest;
    afw_size_t nprops;
    afw_size_t i;

    object = afw_value_as_object(value, xctx);

    /*
     * Remember each bound name (evaluate computed keys once). The rest
     * walk must not re-evaluate property_name_expr.
     */
    resolved_names = NULL;
    if (od->rest) {
        nprops = 0;
        for (ap = od->assignment_property; ap; ap = ap->next) {
            nprops++;
        }
        if (nprops > 0) {
            resolved_names = afw_pool_calloc(p,
                sizeof(const afw_value_t *) * nprops, xctx);
        }
    }

    /* Process assignment properties. */
    i = 0;
    for (ap = od->assignment_property; ap; ap = ap->next, i++)
    {
        if (ap->is_rename) {
            if (ap->property_name_expr) {
                name_v = afw_value_evaluate_and_park(
                    ap->property_name_expr, 1, p, xctx);
                bound_name = afw_object_require_string_property_name(
                    name_v, xctx);
            }
            else {
                bound_name = ap->property_name;
            }
            if (resolved_names) {
                resolved_names[i] = bound_name;
            }
            v = bound_name
                ? afw_object_get_property(object, bound_name, xctx)
                : NULL;
            if (!v) {
                v = impl_evaluate_pattern_default(
                    ap->assignment_element->default_value, p, xctx);
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
            if (resolved_names) {
                resolved_names[i] =
                    &ap->symbol_reference->symbol->name->pub;
            }
            v = afw_object_get_property(object,
                &ap->symbol_reference->symbol->name->pub, xctx);
            if (!v) {
                v = impl_evaluate_pattern_default(
                    ap->default_value, p, xctx);
            }
            impl_assign_value(&ap->symbol_reference->pub,
                v, assignment_type, p, xctx);
        }
    }

    /* Add other properties to rest. */
    if (od->rest) {
        rest = afw_object_create_unmanaged_new_p(p, xctx);
        for (iterator = NULL;;) {
            v = afw_object_get_next_property(object, &iterator, &property_name,
                xctx);
            if (!v) {
                break;
            }

            for (i = 0, ap = od->assignment_property;
                ap;
                ap = ap->next, i++)
            {
                bound_name = resolved_names
                    ? resolved_names[i] : NULL;
                if (bound_name &&
                    afw_value_equal(bound_name, property_name, xctx))
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
        /*
         * const may only be set when establishing the binding (const/let
         * define path). Plain assign_only must not overwrite a const.
         */
        if (assignment_type == afw_compile_assignment_type_assign_only &&
            symbol->symbol_type == afw_value_block_symbol_type_const)
        {
            AFW_THROW_ERROR_FZ(read_only, xctx,
                "Cannot assign to const variable \"" AFW_UTF8_FMT "\"",
                AFW_UTF8_FMT_ARG(&symbol->name->internal));
        }
        if (symbol->type.kind != afw_value_type_kind_data_type ||
            symbol->type.data_type != afw_data_type_unevaluated)
        {
            value = afw_value_evaluate_and_park(value, 1, p, xctx);
        }
        afw_value_type_check_assignable(&symbol->type, value,
            "assignment", contextual, xctx);
        afw_xctx_scope_symbol_set_value(symbol, value, xctx);
        break;

    case afw_compile_assignment_target_type_max_type:
    default:
        AFW_THROW_ERROR_FZ(coding_error, xctx, "Invalid case %d", at->target_type);
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

    value = afw_value_evaluate_and_park(value, 1, p, xctx);

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
        if (assignment_type == afw_compile_assignment_type_assign_only &&
            t->symbol->symbol_type == afw_value_block_symbol_type_const)
        {
            AFW_THROW_ERROR_FZ(read_only, xctx,
                "Cannot assign to const variable \"" AFW_UTF8_FMT "\"",
                AFW_UTF8_FMT_ARG(&t->symbol->name->internal));
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

        aggregate_value = afw_value_evaluate_and_park(
            t->aggregate_value, 1, p, xctx);
        key = afw_value_evaluate_and_park(t->key, 1, p, xctx);
        aggregate_value = afw_value_as_assignable(aggregate_value, xctx);

        if (afw_value_is_object(aggregate_value)) {
            object = ((const afw_value_object_t *)aggregate_value)->internal;
            if (afw_object_is_immutable(object, xctx)) {
                afw_value_release(aggregate_value, xctx);
                AFW_THROW_ERROR_Z(general, "Target object is immutable", xctx);
            }
            key = afw_object_require_string_property_name(key, xctx);
            afw_object_set_property(object, key, value, xctx);
        }

        else if (afw_value_is_array(aggregate_value)) {
            list = ((const afw_value_array_t *)aggregate_value)->internal;

            if (afw_array_is_immutable(list, xctx)) {
                afw_value_release(aggregate_value, xctx);
                AFW_THROW_ERROR_Z(general, "Target array is immutable", xctx);
            }

            if (!afw_value_is_integer(key)) {
                afw_value_release(aggregate_value, xctx);
                AFW_THROW_ERROR_Z(general, "Array index must be integer", xctx);
            }

            afw_array_set_value(list,
                ((const afw_value_integer_t *)key)->internal,
                value, xctx);
        }

        else if (afw_value_has_iterator(aggregate_value)) {
            afw_value_release(aggregate_value, xctx);
            /* Utf8 code-point sequences are immutable (#153). */
            AFW_THROW_ERROR_Z(general,
                "Cannot assign into a utf8 code-point sequence", xctx);
        }

        else {
            afw_value_release(aggregate_value, xctx);
            AFW_THROW_ERROR_Z(general, "Invalid assignment target", xctx);
        }
        afw_value_release(aggregate_value, xctx);

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
    const afw_iterator_old_t *iterator;

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
 * for (let/const …) compile wraps the call in a one-statement block that
 * holds the loop-local names. Assign-for has no such wrapper; do not clone
 * the enclosing script/function scope.
 */
static afw_boolean_t
impl_is_c_style_for_let_wrapper(const afw_xctx_scope_t *scope)
{
    const afw_value_t *stmt;
    const afw_value_call_built_in_function_t *call;
    const afw_utf8_t *id;

    if (!scope || !scope->block ||
        scope->block->symbol_count == 0 ||
        scope->block->statement_count != 1)
    {
        return false;
    }
    stmt = scope->block->statements[0];
    if (!stmt || !afw_value_is_call_built_in_function(stmt)) {
        return false;
    }
    call = (const afw_value_call_built_in_function_t *)stmt;
    if (!call->function || !call->function->functionId) {
        return false;
    }
    id = &call->function->functionId->internal;
    return afw_utf8_equal(id, afw_s_for);
}


/* for init/increment are assignment IR, not statement-list last_return. */
static void
impl_evaluate_for_increment(
    afw_function_execute_t *x,
    afw_size_t parameter_number,
    const afw_value_t *values,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    impl_evaluate_one_or_more_values(x, parameter_number, values, p, xctx);
}



/*
 * Adaptive function: assign
 *
 * afw_function_execute_assign
 *
 * See afw_function_bindings_internal.h for more information.
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
    {
        const afw_value_t *value;

        AFW_FUNCTION_EVALUATE_PARAMETER(value, 2);
        result = impl_assign(x->argv[1], value,
            afw_compile_assignment_type_assign_only,
            p, xctx);
    }

    /* last_return is the statement list, not assign() itself
     * (for init/increment must not write). */
    return result;
}



/*
 * Adaptive function: break
 *
 * afw_function_execute_break
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This is a special function that can be called to break out of the body of a
 * loop or switch. If a label is supplied, break the loop with that label. If
 * called outside of a loop or switch body, an error is thrown.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function break(
 *       label?: string
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   label - (optional string) Optional loop label. If omitted, break the
 *       innermost loop or switch.
 *
 * Returns:
 *
 *   (void) Does not complete. Leaves the body of a loop or switch.
 */
const afw_value_t *
afw_function_execute_break(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(1);
    xctx->statement_flow_label = impl_optional_loop_label(x, 1);
    afw_xctx_statement_flow_set_type(break, xctx);

    return afw_value_void;
}


/*
 * Adaptive function: const
 *
 * afw_function_execute_const
 *
 * See afw_function_bindings_internal.h for more information.
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
 *   ): void;
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
 *   (void) Does not complete. A const statement does not override the running
 *       result.
 */
const afw_value_t *
afw_function_execute_const(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MIN(2);
    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(3);

    /** @fixme process type. */

    impl_assign(x->argv[1], AFW_FUNCTION_ARGV(2),
        afw_compile_assignment_type_const,
        p, xctx);

    return afw_value_void;
}



/*
 * Adaptive function: continue
 *
 * afw_function_execute_continue
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This is a special function that can be called in the body of a loop function
 * to test the condition and, if true, start evaluating the body again. If a
 * label is supplied, continue the loop with that label. If called outside of a
 * loop body, an error is thrown.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function continue(
 *       label?: string
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   label - (optional string) Optional loop label. If omitted, continue the
 *       innermost loop.
 *
 * Returns:
 *
 *   (void) Does not complete. Continues the enclosing loop.
 */
const afw_value_t *
afw_function_execute_continue(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(1);
    xctx->statement_flow_label = impl_optional_loop_label(x, 1);
    afw_xctx_statement_flow_set_type(continue, xctx);

    return afw_value_void;
}



/*
 * Adaptive function: do_while
 *
 * afw_function_execute_do_while
 *
 * See afw_function_bindings_internal.h for more information.
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
 *       body: array,
 *       label?: string
 *   ): void;
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
 *   label - (optional string) Optional loop label for break/continue Identifier
 *       (issue #62).
 *
 * Returns:
 *
 *   (void) Does not complete. Nested assignment still writes the running
 *       result.
 */
const afw_value_t *
afw_function_execute_do_while(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;
    const afw_value_t *result;
    const afw_value_boolean_t *condition;

    const afw_value_t *this_label;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MIN(2);
    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(3);
    this_label = impl_optional_loop_label(x, 3);
    result = afw_value_void;
    for (;;) {
        result = impl_keep_loop_last(result,
            afw_value_block_evaluate_statement(x, x->argv[2], p, xctx),
            xctx);
        if (impl_loop_should_exit(this_label, xctx)) {
            break;
        }
        AFW_FUNCTION_EVALUATE_REQUIRED_CONDITION_PARAMETER(condition, 1);
        if (!condition->internal) {
            break;
        }
    }

    impl_loop_consume_if_target(this_label, xctx);

    return impl_statement_result_or_void(result, xctx);
}



/*
 * Adaptive function: for
 *
 * afw_function_execute_for
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function loops while condition is true. If the condition is false for
 * the first iteration, the loop does not complete (void).
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
 *       body?: array,
 *       label?: string
 *   ): void;
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
 *   label - (optional string) Optional loop label for break/continue Identifier
 *       (issue #62).
 *
 * Returns:
 *
 *   (void) Does not complete. Nested assignment still writes the running
 *       result.
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

    const afw_value_t *this_label;
    afw_boolean_t clone_each;

    previous_iterator_scope = NULL;
    this_label = NULL;
    clone_each = impl_is_c_style_for_let_wrapper(
        afw_xctx_scope_current(xctx));
    AFW_TRY{

        AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(5);
        this_label = impl_optional_loop_label(x, 5);

        if (AFW_FUNCTION_PARAMETER_IS_PRESENT(1)) {
            impl_evaluate_for_increment(x, 1, x->argv[1], p, xctx);
        }

        increment = NULL;
        if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
            increment = x->argv[3];
        }

        body = NULL;
        if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4)) {
            body = x->argv[4];
        }

        for (result = afw_value_void;;) {

            if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
                AFW_FUNCTION_EVALUATE_REQUIRED_CONDITION_PARAMETER(condition,
                    2);
                if (!condition->internal) {
                    break;
                }
            }

            if (body) {
                /* for is void. Nested assignment writes the slot. */
                result = impl_keep_loop_last(result,
                    afw_value_block_evaluate_statement(x, body, p, xctx),
                    xctx);
                if (impl_loop_should_exit(this_label, xctx)) {
                    break;
                }
            }

            if (increment) {
                /*
                 * Per-iteration clone is only for for-let/const (the
                 * wrapper block that opened this call). Assign-for mutates
                 * existing names; cloning that enclosing scope after a
                 * skipped 0-symbol `{ }` body leaves the originals stale.
                 */
                if (clone_each) {
                    if (previous_iterator_scope) {
                        scope = afw_xctx_scope_clone(
                            previous_iterator_scope, xctx);
                        afw_xctx_scope_deactivate(
                            previous_iterator_scope, xctx);
                        afw_xctx_scope_release(
                            previous_iterator_scope, xctx);
                    }
                    else {
                        scope = afw_xctx_scope_clone(
                            afw_xctx_scope_current(xctx), xctx);
                    }
                    previous_iterator_scope = scope;
                    afw_xctx_scope_activate(scope, xctx);
                }
                impl_evaluate_for_increment(x, 3, increment, p, xctx);
            }
        }
    }
    AFW_FINALLY{

        impl_loop_consume_if_target(this_label, xctx);

        /* Creator release of the last clone; pop if still current. */
        if (previous_iterator_scope) {
            if (afw_xctx_scope_current(xctx) ==
                previous_iterator_scope)
            {
                afw_xctx_scope_deactivate(
                    previous_iterator_scope, xctx);
            }
            afw_xctx_scope_release(previous_iterator_scope, xctx);
        }
    }
    AFW_ENDTRY;

    return impl_statement_result_or_void(result, xctx);
}



/*
 * Adaptive function: for_of
 *
 * afw_function_execute_for_of
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function will evaluate an array of values (statements) while a condition
 * is true with initial and increment values. The condition is tested at the
 * beginning of the loop. If the condition is false for the first iteration, the
 * loop does not complete (void). This supports for-of statement.
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
 *       body?: array,
 *       label?: string
 *   ): void;
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
 *   label - (optional string) Optional loop label for break/continue Identifier
 *       (issue #62).
 *
 * Returns:
 *
 *   (void) Does not complete. Nested assignment still writes the running
 *       result.
 */
const afw_value_t *
afw_function_execute_for_of(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;
    const afw_value_t *result;
    const afw_value_t *iterable;
    const afw_value_t *value;
    const afw_value_t *for_of_target;
    const afw_xctx_scope_t *scope;
    const afw_xctx_scope_t *previous_iterator_scope;
    afw_compile_internal_assignment_type_t assignment_type;
    afw_compile_internal_assignment_type_t head_type;
    afw_iterator_t iterator;
    const afw_value_t *this_label;
    afw_boolean_t clone_each;
    afw_boolean_t first;

    result = afw_value_void;
    this_label = NULL;
    previous_iterator_scope = NULL;
    clone_each = false;
    first = true;
    AFW_TRY{

        AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MIN(3);
        AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(4);
        this_label = impl_optional_loop_label(x, 4);

        /*
         * Evaluate head without forcing array. Keyless afw_iterator covers
         * array and utf8-backed types (code points as managed strings) — #153.
         */
        iterable = afw_function_evaluate_required_parameter(x, 2, NULL);

        assignment_type = afw_compile_assignment_type_use_assignment_targets;
        for_of_target = x->argv[1];
        if (afw_value_is_assignment_target(for_of_target)) {
            const afw_value_assignment_target_t *at =
                (const afw_value_assignment_target_t *)for_of_target;

            head_type = at->assignment_target->assignment_type;
            /* let/const open a loop-local block; clone it per iteration. */
            if (head_type == afw_compile_assignment_type_let ||
                head_type == afw_compile_assignment_type_const)
            {
                clone_each = true;
            }
        }

        if (!afw_value_has_iterator(iterable)) {
            AFW_THROW_ERROR_Z(general,
                "for-of head must be an array or utf8 code-point sequence",
                xctx);
        }

        afw_value_initialize_iterator(iterable, &iterator, xctx);
        while ((value = afw_iterator_get_next(&iterator, p, xctx)) != NULL) {
            if (clone_each && !first) {
                if (previous_iterator_scope) {
                    scope = afw_xctx_scope_clone(
                        previous_iterator_scope, xctx);
                    afw_xctx_scope_deactivate(
                        previous_iterator_scope, xctx);
                    afw_xctx_scope_release(
                        previous_iterator_scope, xctx);
                }
                else {
                    scope = afw_xctx_scope_clone(
                        afw_xctx_scope_current(xctx), xctx);
                }
                previous_iterator_scope = scope;
                afw_xctx_scope_activate(scope, xctx);
                /* Still let/const define on the clone, not assign_only. */
            }
            impl_assign(x->argv[1], value, assignment_type, p, xctx);
            if (!clone_each) {
                assignment_type =
                    afw_compile_assignment_type_assign_only;
            }
            result = impl_keep_loop_last(result,
                afw_value_block_evaluate_statement(
                    x, x->argv[3], p, xctx),
                xctx);
            first = false;
            if (impl_loop_should_exit(this_label, xctx)) {
                break;
            }
        }
    }
    AFW_FINALLY{

        impl_loop_consume_if_target(this_label, xctx);
        if (previous_iterator_scope) {
            if (afw_xctx_scope_current(xctx) ==
                previous_iterator_scope)
            {
                afw_xctx_scope_deactivate(
                    previous_iterator_scope, xctx);
            }
            afw_xctx_scope_release(previous_iterator_scope, xctx);
        }

    }
    AFW_ENDTRY;

    return impl_statement_result_or_void(result, xctx);
}



/*
 * Adaptive function: if
 *
 * afw_function_execute_if
 *
 * See afw_function_bindings_internal.h for more information.
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
 *       false, the if does not complete (void). See the 'body' parameter of the
 *       'block' function for information on how the body is processed.
 *
 * Returns:
 *
 *   (any) The result of evaluating 'then' or 'else'. Also the ternary operator.
 *       If the condition is false and else is omitted, does not complete
 *       (void).
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
    result = afw_value_void;
    if (condition->internal) {
        result = afw_value_block_evaluate_statement(x, x->argv[2], p, xctx);
    }
    else if (x->argc == 3) {
        result = afw_value_block_evaluate_statement(x, x->argv[3], p, xctx);
    }

    /*
     * if is also the ternary operator. Return then/else. No else and
     * false is void (does not complete).
     */
    return result;
}



/*
 * Adaptive function: let
 *
 * afw_function_execute_let
 *
 * See afw_function_bindings_internal.h for more information.
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
 *   ): void;
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
 *   (void) Does not complete. A let statement does not override the running
 *       result.
 */
const afw_value_t *
afw_function_execute_let(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MIN(1);
    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(3);

    impl_assign(x->argv[1], AFW_FUNCTION_ARGV(2),
        afw_compile_assignment_type_let,
        p, xctx);

    return afw_value_void;
}



/*
 * Adaptive function: rethrow
 *
 * afw_function_execute_rethrow
 *
 * See afw_function_bindings_internal.h for more information.
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
 *   ): void;
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (void) Does not complete. Rethrows the current error in a catch block.
 */
const afw_value_t *
afw_function_execute_rethrow(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(0);
    afw_xctx_statement_flow_set_type(rethrow, xctx);

    return afw_value_void;
}



/*
 * Adaptive function: return
 *
 * afw_function_execute_return
 *
 * See afw_function_bindings_internal.h for more information.
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
    const afw_value_t *result;

    result = afw_value_undefined;
    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(1);
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(1)) {
        /* NULL (undefined) is okay here. */
        result = afw_function_evaluate_parameter(x, 1, NULL);
        /* return statement should not return NULL for undefined. */
        if (!result) {
            result = afw_value_undefined;
        }
    }
    result = afw_value_as_assignable(result, xctx);
    afw_xctx_statement_flow_set_type(return, xctx);
    return result;
}



/*
 * Adaptive function: wrap_literal_object
 *
 * afw_function_execute_wrap_literal_object
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Evaluate an object value and clone_or_reference it (object_hold: memory face
 * over the instance). Remaining explicit wrap_literal_object() calls; the
 * compiler no longer emits this. Not normal author surface.
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
 *   object - (object) Object to evaluate and hold (typically a constant object
 *       literal).
 *
 * Returns:
 *
 *   (object) A holdable memory-wrapper object face over the evaluated base.
 */
const afw_value_t *
afw_function_execute_wrap_literal_object(
    afw_function_execute_t *x)
{
    const afw_value_object_t *object;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_IS(1);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(object, 1, object);

    return afw_value_object_hold((const afw_value_t *)object, x->p, x->xctx);
}



/*
 * Adaptive function: wrap_literal_array
 *
 * afw_function_execute_wrap_literal_array
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Evaluate an array value and clone_or_reference it (array_hold: memory face
 * over the instance). Remaining explicit wrap_literal_array() calls; the
 * compiler no longer emits this. Not normal author surface.
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
 *   array - (array) Array to evaluate and hold (typically a constant array
 *       literal).
 *
 * Returns:
 *
 *   (array) A holdable memory-wrapper array face over the evaluated base.
 */
const afw_value_t *
afw_function_execute_wrap_literal_array(
    afw_function_execute_t *x)
{
    const afw_value_array_t *array;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_IS(1);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(array, 1, array);

    return afw_value_array_hold((const afw_value_t *)array, x->p, x->xctx);
}



/*
 * Adaptive function: switch
 *
 * afw_function_execute_switch
 *
 * See afw_function_bindings_internal.h for more information.
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
 *   ): void;
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
 *   (void) Does not complete. Nested assignment still writes the running
 *       result.
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
    const afw_iterator_old_t *iterator;

    result = afw_value_void;

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
        result = afw_value_function_return_value_consume(
            afw_value_evaluate(functor, p, xctx), p, xctx);
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
                if (!afw_xctx_statement_flow_is_type(sequential, xctx)) {
                    break;
                }
            }
            if (!afw_xctx_statement_flow_is_type(sequential, xctx)) {
                break;
            }
        }
    }

    /*
     * Unlabeled break stays in the switch. Labeled break/continue and
     * unlabeled continue belong to an enclosing loop (issue #62).
     */
    if (afw_xctx_statement_flow_is_type(break, xctx) &&
        !xctx->statement_flow_label)
    {
        afw_xctx_statement_flow_reset_break_and_continue(xctx);
    }

    return impl_statement_result_or_void(result, xctx);
}


/*
 * Adaptive function: throw
 *
 * afw_function_execute_throw
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This throws an error that can be caught by a try/catch block. An error object
 * of object type _AdaptiveError_ will be available in the catch block. Its 'id'
 * property is 'throw' unless the optional id parameter is supplied. Optional
 * data is available as the 'data' property. The other properties are set based
 * on the parameters specified and where this function is called.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function throw(
 *       message: string,
 *       data?: any,
 *       id?: string
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   message - (string) This is the message that will be included in the
 *       _AdaptiveError_ error object available in the catch block.
 *
 *   data - (optional any) Optional data that will be available as the 'data'
 *       property of the _AdaptiveError_ object in the catch block.
 *
 *   id - (optional string) Optional error id (mnemonic) to use instead of
 *       'throw'. Must be a name allowed on script throw (request-facing HTTP
 *       names such as not_found, denied, gone, too_many_requests). Sets the id
 *       property of the catch object and the HTTP status if the error is not
 *       caught.
 *
 * Returns:
 *
 *   (void)
 *
 * Errors thrown:
 *
 *   throw - Default when id is omitted
 *   not_found - id is not_found
 *   denied - id is denied
 *   authentication_required - id is authentication_required
 *   conflict - id is conflict
 *   bad_request - id is bad_request
 *   read_only - id is read_only
 *   payload_too_large - id is payload_too_large
 *   query_too_complex - id is query_too_complex
 *   method_not_allowed - id is method_not_allowed
 *   gone - id is gone
 *   too_many_requests - id is too_many_requests
 *   unprocessable_content - id is unprocessable_content
 *   moved_permanently - id is moved_permanently
 *   argument_error - id is not a string or is not allowed on throw
 */
const afw_value_t *
afw_function_execute_throw(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_value_string_t *message;
    const afw_value_string_t *id_value;
    const afw_value_t *data;
    afw_error_code_t code;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MIN(1);
    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(3);

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(message, 1, string);
    AFW_FUNCTION_EVALUATE_PARAMETER(data, 2);
    id_value = NULL;
    if (x->argc >= 3) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(
            id_value, 3, string);
    }

    code = afw_error_code_throw;
    if (id_value) {
        if (!afw_error_code_from_id(&id_value->internal, &code) ||
            !afw_error_id_allowed_on_script_throw(&id_value->internal))
        {
            AFW_THROW_ERROR_FZ(argument_error, xctx,
                "id " AFW_UTF8_FMT_Q " is not allowed on throw",
                AFW_UTF8_FMT_ARG(&id_value->internal));
        }
    }

    afw_error_set_fz(code, AFW__FILE_LINE__, xctx,
        AFW_UTF8_FMT, AFW_UTF8_FMT_ARG(&message->internal));
    xctx->error->data = data;
    longjmp(((xctx)->current_try->throw_jmp_buf), code);

    return afw_value_void;
}



/*
 * Adaptive function: try
 *
 * afw_function_execute_try
 *
 * See afw_function_bindings_internal.h for more information.
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
 *   ): void;
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
 *   (void) Does not complete. Nested assignment still writes the running
 *       result.
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

    result = afw_value_void;
    use_type = afw_xctx_statement_flow_get(xctx);

    scope_at_entry = afw_xctx_scope_current(xctx);
    AFW_TRY {
        this_result = afw_value_block_evaluate_statement(
            x, x->argv[1], p, xctx);
        use_type = afw_xctx_statement_flow_get(xctx);
        result = impl_keep_if_return(result, this_result, xctx);
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
                /* Hand-built try() can set argv[4] without a catch block. */
                if (!afw_value_is_block(x->argv[3])) {
                    AFW_THROW_ERROR_Z(argument_error,
                        "try catch body must be a block when an error bind "
                        "is present",
                        xctx);
                }
                /*
                 * Bind the error after the catch frame exists, then the
                 * same statement-list last_return rules as evaluate_block.
                 * Do not call evaluate_block: it would create the scope
                 * again. Empty catch writes nothing (UpdateEmpty).
                 */
                const afw_xctx_scope_t *scope;
                const afw_pool_t *eval_p;
                const afw_error_t *caught_error = &this_THROWN_ERROR;
                const afw_value_block_t *block =
                    (const afw_value_block_t *)x->argv[3];
                scope = NULL;
                this_result = afw_value_void;
                AFW_TRY{
                    scope = afw_xctx_scope_create(
                        block, afw_xctx_scope_current(xctx), xctx);
                    afw_xctx_scope_activate(scope, xctx);
                    eval_p = scope->p;
                    error_object = afw_error_to_object(
                        caught_error, eval_p, xctx);
                    error_value = afw_value_create_unmanaged_object(
                        error_object, eval_p, xctx);
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
                    afw_size_t stmt_start = 0;

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
                            if (afw_utf8_equal(&esym->name->internal,
                                err_name)) {
                                err_target =
                                    (const afw_value_t *)
                                    afw_value_symbol_reference_create(
                                        NULL, esym, eval_p, xctx);
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
                            afw_compile_assignment_type_let, eval_p, xctx);
                    }
                    this_result = afw_value_block_evaluate_statements(
                        x, block, stmt_start, eval_p, xctx);
                    if (!afw_value_is_void(this_result)) {
                        afw_xctx_script_result_set(this_result, xctx);
                    }
                }
                AFW_FINALLY{
                    if (scope) {
                        if (afw_xctx_scope_current(xctx) == scope) {
                            afw_xctx_scope_deactivate(scope, xctx);
                        }
                        afw_xctx_scope_release(scope, xctx);
                    }
                }
                AFW_ENDTRY;
                if (!afw_value_is_void(this_result)) {
                    this_result = afw_xctx_script_result_get(xctx);
                }
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
            else {
                /* Catch completed: void. Nested assignment wrote the slot. */
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
    return impl_statement_result_or_void(result, xctx);
}



/*
 * Adaptive function: while
 *
 * afw_function_execute_while
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function will evaluate an array of values (statements) while a condition
 * is true. The condition is tested at the beginning of the loop. If the
 * condition is false for the first iteration, the loop does not complete
 * (void). See the related functions 'break', 'continue', 'return' and 'throw'.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function while(
 *       condition: boolean,
 *       body: array,
 *       label?: string
 *   ): void;
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
 *   label - (optional string) Optional loop label for break/continue Identifier
 *       (issue #62).
 *
 * Returns:
 *
 *   (void) Does not complete. Nested assignment still writes the running
 *       result.
 */
const afw_value_t *
afw_function_execute_while(
    afw_function_execute_t *x)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_pool_t *p = x->p;
    const afw_value_t *result;
    const afw_value_boolean_t *condition;

    const afw_value_t *this_label;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MIN(2);
    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(3);
    this_label = impl_optional_loop_label(x, 3);

    for (result = afw_value_void;;) {
        AFW_FUNCTION_EVALUATE_REQUIRED_CONDITION_PARAMETER(condition, 1);
        if (!condition->internal) {
            break;
        }
        result = impl_keep_loop_last(result,
            afw_value_block_evaluate_statement(x, x->argv[2], p, xctx),
            xctx);
        if (impl_loop_should_exit(this_label, xctx))
        {
            break;
        }
    }

    impl_loop_consume_if_target(this_label, xctx);

    return impl_statement_result_or_void(result, xctx);
}
