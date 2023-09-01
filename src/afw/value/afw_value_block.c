// See the 'COPYING' file in the project root for licensing information.
/*
 * Interface afw_value Implementation for block
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file afw_value_block.c
 * @brief Implementation of afw_value interface for block
 */

#include "afw_internal.h"


#define impl_afw_value_optional_release NULL
#define impl_afw_value_get_reference NULL

#define impl_afw_value_get_evaluated_meta \
    afw_value_internal_get_evaluated_meta_default

#define impl_afw_value_get_evaluated_metas \
    afw_value_internal_get_evaluated_metas_default

/* Declares and rti/inf defines for interface afw_value */
#define AFW_IMPLEMENTATION_ID "block"
#define AFW_IMPLEMENTATION_INF_SPECIFIER AFW_DEFINE_CONST_DATA
#define AFW_IMPLEMENTATION_INF_LABEL afw_value_block_inf
#include "afw_value_impl_declares.h"

#define IMPL_TEMP_FIX_ASSIGNS(XX) \
    modified_x.p = p; \
    modified_x.xctx = xctx; \
    modified_x.function = &afw_function_definition_ ## XX; \
    modified_x.argv = argv; \
    modified_x.argc = argc; \

#define IMPL_TEMP_FIX(XX) \
    afw_function_execute_t modified_x; \
    afw_memory_clear(&modified_x); \
    IMPL_TEMP_FIX_ASSIGNS(XX) \
    x = &modified_x




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


static const afw_value_t *
impl_create_closure_if_needed(
    const afw_value_script_function_definition_t *function,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_xctx_scope_t *scope;
    const afw_value_t *result;

    scope = afw_xctx_scope_current(xctx);

    if (function->depth >= scope->block->depth) {
        for (; scope; scope = scope->parent_lexical_scope) {
            if (function->depth == scope->block->depth) {
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
        result = (const afw_value_t *)function;
    }

    return result;
}



static void
impl_list_destructure(
    const afw_compile_assignment_target_t *at,
    const afw_compile_list_destructure_t *ld,
    const afw_value_t *value,
    afw_compile_internal_assignment_type_t assignment_type,
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
            "List destructure can only be performed on a list", xctx);
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
            continue;
        }
        if (eol) {
            v = ae->default_value;
        }
        if (v) {
            impl_assign_value(ae->assignment_target, v, assignment_type,
                p, xctx);
        }
        else {
            //FIXME ???
        }
    }

    if (!eol && ld->rest) {
        for (rest = NULL;;) {
            v = afw_array_get_next_value(
                ((const afw_value_array_t *)value)->internal,
                &iterator, p, xctx);
            if (!v) {
                break;
            }
            if (!rest) {
                rest = afw_array_create_generic(p, xctx);
            }
            afw_array_add_value(rest, v, xctx);
        }
        if (rest) {
            v = afw_value_create_array(rest, p, xctx);
            impl_assign_value(ld->rest, v, assignment_type, p, xctx);
        }
    }
}



static void
impl_object_destructure(
    const afw_compile_assignment_target_t *at,
    const afw_compile_object_destructure_t *od,
    const afw_value_t *value,
    afw_compile_internal_assignment_type_t assignment_type,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_compile_assignment_property_t *ap;
    const afw_object_t *object;
    const afw_iterator_t *iterator;
    const afw_utf8_t *property_name;
    const afw_value_t *v;
    const afw_object_t *rest;

    object = afw_value_as_object(value, xctx);

    /* Process assignment properties. */
    for (ap = od->assignment_property; ap; ap = ap->next)
    {
        if (ap->is_rename) {
            v = afw_object_get_property(object, ap->property_name, xctx);
            if (!v) {
                v = ap->assignment_element->default_value;
            }
            if (v) {
                impl_assign_value(ap->assignment_element->assignment_target, v,
                    assignment_type, p, xctx);
            }
            else {
                //FIXME ???
            }
        }
        else {
            v = afw_object_get_property(object,
                ap->symbol_reference->symbol->name, xctx);
            if (!v) {
                v = ap->default_value;
            }
            if (v) {
                impl_assign_value((const afw_value_t *)ap->symbol_reference,
                    v, assignment_type, p, xctx);
            }
            else {
                //FIXME ???
            }

        }
    }

    /* Add other properties to rest. */
    if (od->rest) {
        rest = afw_object_create_managed(p, xctx);
        for (iterator = NULL;;) {
            v = afw_object_get_next_property(object, &iterator, &property_name,
                xctx);
            if (!v) {
                break;
            }

            for (ap = od->assignment_property; ap; ap = ap->next)
            {
                if (
                    (ap->is_rename && afw_utf8_equal(
                        ap->property_name, property_name))
                    ||
                    (!ap->is_rename && afw_utf8_equal(
                        ap->symbol_reference->symbol->name, property_name))
                )
                {
                    break;
                }
            }
            if (!ap) {
                afw_object_set_property(rest, property_name, v, xctx);
            }
        }
        v = afw_value_create_object(rest, p, xctx);
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

    switch (at->target_type) {
    case afw_compile_assignment_target_type_list_destructure:
        impl_list_destructure(at, at->list_destructure, value,
            assignment_type, p, xctx);
        break;

    case afw_compile_assignment_target_type_object_destructure:
        impl_object_destructure(at, at->object_destructure, value,
            assignment_type, p, xctx);
        break;

    case afw_compile_assignment_target_type_symbol_reference:
        afw_xctx_scope_symbol_set_value(
            at->symbol_reference->symbol, value, xctx);
        break;

    case afw_compile_assignment_target_type_max_type:
    default:
        AFW_THROW_ERROR_FZ(code, xctx, "Invalid case %d", at->target_type);
    }
}


/*
 * Call this without value being evaluated so that the need for a clone can
 * be determined. Evaluated objects and lists need to be cloned so they can
 * potentially be modified. If needed, value will be evaluated.
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

    if (afw_value_is_object(value) || afw_value_is_array(value))
    {
        value = afw_value_clone(value, p, xctx);
    }
    else
    {
        value = afw_value_evaluate(value, p, xctx);
    }

    if (assignment_type == afw_compile_assignment_type_use_assignment_targets)
    {
        if (afw_value_is_assignment_target(target))
        {
            at = (const afw_value_assignment_target_t *)target;
            assignment_type = at->assignment_target->assignment_type;
        }
        else {
            AFW_THROW_ERROR_Z(general, "Internal error", xctx);
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
                AFW_THROW_ERROR_Z(general, "Target list is immutable", xctx);
            }

            if (!afw_value_is_integer(key)) {
                AFW_THROW_ERROR_Z(general, "List index must be integer", xctx);
            }

            afw_array_set_value_by_index(list,
                (afw_size_t)((const afw_value_integer_t *)key)->internal,
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
    afw_value_block_statement_type_t type;

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
            result = afw_value_block_evaluate_statement(x, &type,
                false, false, value, p, xctx);
        }
    }

    else {
        result = afw_value_block_evaluate_statement(x, &type,
            false, false, values, p, xctx);
    }

    return result;
}



AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_block(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    const afw_value_block_t *self,
    afw_boolean_t is_loop,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;
    const afw_compile_value_contextual_t *saved_contextual;
    afw_size_t i;
    const afw_xctx_scope_t *scope;

    /* Push value on evaluation stack. */
    afw_xctx_evaluation_stack_push_value(
        (const afw_value_t *)self, xctx);
    saved_contextual = xctx->error->contextual;
    xctx->error->contextual = self->contextual;
    result = afw_value_undefined;

    scope = afw_xctx_scope_create_and_activate(
        self, afw_xctx_scope_current(xctx), xctx);
    AFW_TRY{
        for (i = 0; i < self->statement_count; i++) {
            result = afw_value_block_evaluate_statement(x, type,
                true, is_loop, self->statements[i], p, xctx);
            if (*type != afw_value_block_statement_type_sequential)
            {
                break;
            }
        }
    }
    AFW_FINALLY{
        afw_xctx_scope_deactivate(scope, xctx);
    }
    AFW_ENDTRY;

    /* Pop value from evaluation stack and return result. */
    afw_xctx_evaluation_stack_pop_value(xctx);
    xctx->error->contextual = saved_contextual;

    return result;
}



AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_for(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_size_t argc,
    const afw_value_t * const * argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    IMPL_TEMP_FIX(for);
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
            impl_evaluate_one_or_more_values(x, 1, argv[1], p, xctx);
        }

        increment = NULL;
        if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
            increment = argv[3];
        }

        body = NULL;
        if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4)) {
            body = argv[4];
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
                result = afw_value_block_evaluate_statement(x, type,
                    true, true, body, p, xctx);
                if (*type == afw_value_block_statement_type_break ||
                    *type == afw_value_block_statement_type_rethrow ||
                    *type == afw_value_block_statement_type_return)
                {
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
        /* We don't want continue/break outside of this loop */
        if (*type == afw_value_block_statement_type_continue || 
            *type == afw_value_block_statement_type_break) 
        {
            *type = afw_value_block_statement_type_sequential;
        }

        /* Release final increment scope. */
        if (previous_iterator_scope) {
            afw_xctx_scope_deactivate(previous_iterator_scope, xctx);
        }      
    }
    AFW_ENDTRY;

    return result;
}


AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_for_of(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_size_t argc,
    const afw_value_t *const *argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    IMPL_TEMP_FIX(for_of);
    const afw_value_t *result = afw_value_undefined;
    const afw_value_array_t *list;
    const afw_iterator_t *iterator;
    const afw_value_t *value;
    afw_compile_internal_assignment_type_t assignment_type;

    AFW_TRY{

        AFW_FUNCTION_ASSERT_PARAMETER_COUNT_IS(3);
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(list, 2, array);

        assignment_type = afw_compile_assignment_type_use_assignment_targets;
        for (iterator = NULL;;) {
            value = afw_array_get_next_value(list->internal, &iterator,
                p, xctx);
            if (!value) {
                break;
            }
            impl_assign(argv[1], value, assignment_type, p, xctx);
            assignment_type = afw_compile_assignment_type_assign_only;
            result = afw_value_block_evaluate_statement(x, type,
                true, true, argv[3], p, xctx);
            if (*type == afw_value_block_statement_type_break ||
                *type == afw_value_block_statement_type_rethrow ||
                *type == afw_value_block_statement_type_return)
            {
                break;
            }
        }
    }
    AFW_FINALLY{
        /* We don't want continue/break outside of this loop */
        if (*type == afw_value_block_statement_type_continue || 
            *type == afw_value_block_statement_type_break) 
        {
            *type = afw_value_block_statement_type_sequential;
        }
    }
    AFW_ENDTRY;

    return result;
}



AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_do_while(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_size_t argc,
    const afw_value_t *const *argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    IMPL_TEMP_FIX(do_while);
    const afw_value_t *result;
    const afw_value_boolean_t *condition;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_IS(2);
    for (;;) {
        result = afw_value_block_evaluate_statement(x, type,
            true, true, argv[2], p, xctx);
        if (*type == afw_value_block_statement_type_break ||
            *type == afw_value_block_statement_type_rethrow ||
            *type == afw_value_block_statement_type_return)
        {
            break;
        }
        AFW_FUNCTION_EVALUATE_REQUIRED_CONDITION_PARAMETER(condition, 1);
        if (!condition->internal) {
            break;
        }
    }

    /* We don't want continue/break outside of this loop */
    if (*type == afw_value_block_statement_type_continue || 
        *type == afw_value_block_statement_type_break) 
    {
        *type = afw_value_block_statement_type_sequential;
    }

    return result;
}


AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_if(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_size_t argc,
    const afw_value_t * const * argv,
    afw_boolean_t is_loop,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    IMPL_TEMP_FIX(if);
    const afw_value_t *result;
    const afw_value_boolean_t *condition;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MIN(2);
    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(3);

    AFW_FUNCTION_EVALUATE_REQUIRED_CONDITION_PARAMETER(condition, 1);
    result = afw_value_undefined;
    if (condition->internal) {
        result = afw_value_block_evaluate_statement(x, type,
            true, is_loop, argv[2], p, xctx);
    }
    else if (x->argc == 3) {
        result = afw_value_block_evaluate_statement(x, type,
            true, is_loop, argv[3], p, xctx);
    }

    return result;
}


AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_switch(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_size_t argc,
    const afw_value_t * const * argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    IMPL_TEMP_FIX(switch);
    struct {
        const afw_value_t *function;
        const afw_value_t *value1;
        const afw_value_t *value2;
    } args;
    const afw_value_t *functor;
    const afw_value_t * const *end_of_args;
    const afw_value_t * const *pair;
    const afw_value_t * const *default_pair;     
    const afw_value_t *result;
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
    functor = afw_value_call_create(NULL,
        2, (const afw_value_t **)&args, false, p, xctx);


    /* Find first true case. */
    for (end_of_args = argv + x->argc,
        pair = argv + 3,
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
        if (afw_value_is_true(result)) {
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
                result = afw_value_block_evaluate_statement(x, type,
                    true, true, statement, p, xctx);
                if (*type == afw_value_block_statement_type_break ||
                    *type == afw_value_block_statement_type_rethrow ||
                    *type == afw_value_block_statement_type_return)
                {
                    break;
                }
            }
            if (*type == afw_value_block_statement_type_break ||
                *type == afw_value_block_statement_type_rethrow ||
                *type == afw_value_block_statement_type_return)
            {
                break;
            }
        }
    }

    return result;
}


AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_throw(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_size_t argc,
    const afw_value_t * const * argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    IMPL_TEMP_FIX(throw);
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


AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_try(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_size_t argc,
    const afw_value_t * const * argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    IMPL_TEMP_FIX(try);
    const afw_value_t *result;
    const afw_value_t *this_result;
    const afw_object_t *error_object;
    const afw_value_t *error_value;
    const afw_xctx_scope_t *scope_at_entry;
    afw_value_block_statement_type_t use_type;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MIN(2);
    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(4);

    result = afw_value_undefined;
    use_type = *type;

    scope_at_entry = afw_xctx_scope_current(xctx);
    AFW_TRY {
        result = afw_value_block_evaluate_statement(x, type,
            true, true, argv[1], p, xctx);
        use_type = *type;
    }

    AFW_CATCH_UNHANDLED {
        afw_xctx_scope_unwind(scope_at_entry, xctx);
        if AFW_FUNCTION_PARAMETER_IS_PRESENT(3) {
            if (AFW_FUNCTION_PARAMETER_IS_PRESENT(4)) {
                error_object = afw_error_to_object(&this_THROWN_ERROR, p, xctx);
                error_value = afw_value_create_object(error_object, p, xctx);
                /// @fixme Assignment type is not correct when frames used
 // -------------------------------------------------------------------
 /// @fixme There needs to be a better way to do this. This is a copy of
 /// some of the code from afw_value_block_evaluate_block that had to be
 /// copied here so that the error object can be set in scope after the
 /// scope is created.
    const afw_xctx_scope_t *scope;
    const afw_value_block_t *block = (const afw_value_block_t *)argv[3];
    scope = afw_xctx_scope_create_and_activate(block, afw_xctx_scope_current(xctx), xctx);
    AFW_TRY{
        impl_assign_value(argv[4], error_value,
            afw_compile_assignment_type_let, p, xctx);
        this_result = afw_value_undefined;        
        for (int i = 0; i < block->statement_count; i++) {
            this_result = afw_value_block_evaluate_statement(x, type,
                true, true, block->statements[i], p, xctx);
            if (*type != afw_value_block_statement_type_sequential)
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
                this_result = afw_value_block_evaluate_statement(x, type,
                    true, true, argv[3], p, xctx);
            }
            if (*type == afw_value_block_statement_type_break ||
                *type == afw_value_block_statement_type_continue)
            {
                use_type = *type;
            }
            else if (*type == afw_value_block_statement_type_return)
            {
                use_type = afw_value_block_statement_type_return;
                result = this_result;
            }
            else if (*type == afw_value_block_statement_type_rethrow)
            {
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
            this_result = afw_value_block_evaluate_statement(x, type,
                true, true, argv[2], p, xctx);
            if (*type == afw_value_block_statement_type_break ||
                *type == afw_value_block_statement_type_continue)
            {
                use_type = *type;
                AFW_ERROR_MARK_CAUGHT;
            }
            else if (*type == afw_value_block_statement_type_return)
            {
                use_type = afw_value_block_statement_type_return;
                result = this_result;
                AFW_ERROR_MARK_CAUGHT;
            }
            else if (*type == afw_value_block_statement_type_rethrow)
            {
                use_type = afw_value_block_statement_type_sequential;
            }
        }
    }

    AFW_ENDTRY;

    *type = use_type;
    return result;
}


AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_while(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_size_t argc,
    const afw_value_t * const * argv,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    IMPL_TEMP_FIX(while);
    const afw_value_t *result;
    const afw_value_boolean_t *condition;

    AFW_FUNCTION_ASSERT_PARAMETER_COUNT_IS(2);

    for (result = afw_value_undefined;;) {
        AFW_FUNCTION_EVALUATE_REQUIRED_CONDITION_PARAMETER(condition, 1);
        if (!condition->internal) {
            break;
        }
        result = afw_value_block_evaluate_statement(x, type,
            true, true, argv[2], p, xctx);
        if (*type == afw_value_block_statement_type_break ||
            *type == afw_value_block_statement_type_rethrow ||
            *type == afw_value_block_statement_type_return)
        {
            break;
        }
    }

    /* We don't want continue/break outside of this loop */
    if (*type == afw_value_block_statement_type_continue || 
        *type == afw_value_block_statement_type_break) 
    {
        *type = afw_value_block_statement_type_sequential;
    }

    return result;
}


AFW_DEFINE_INTERNAL(const afw_value_t *)
afw_value_block_evaluate_statement(
    afw_function_execute_t *x,
    afw_value_block_statement_type_t *type,
    afw_boolean_t allow_return,
    afw_boolean_t is_loop,
    const afw_value_t *statement,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;
    const afw_value_call_t *call;
    const afw_compile_value_contextual_t *saved_contextual;
    const afw_value_function_definition_t *function;

    /** @todo this is temporary. */
    afw_function_execute_t modified_x;
    afw_memory_copy(&modified_x, x);
    modified_x.p = p;
    modified_x.xctx = xctx;
    modified_x.function = NULL;
    x = &modified_x;


    result = afw_value_undefined;
    *type = afw_value_block_statement_type_sequential;

    /* If statement is block, handle special. */
    if (afw_value_is_block(statement)) {
        result = afw_value_block_evaluate_block(x, type,
            (const afw_value_block_t *)statement, is_loop, p, xctx);
    }

    /* If statement is special call, handle special. */
    else if (afw_value_is_any_call(statement) &&
        afw_value_is_function_definition(
        ((const afw_value_call_t *)statement)->function_value))
    {
        call = (const afw_value_call_t *)statement;
        function = (const afw_value_function_definition_t *)
            (call->function_value);
        switch (function->scriptSupportNumber) {

        case AFW_VALUE_SCRIPT_SUPPORT_NUMBER_ASSIGN:
            afw_xctx_evaluation_stack_push_value(statement, xctx);
            saved_contextual = xctx->error->contextual;
            xctx->error->contextual = call->args.contextual;
            modified_x.argc = call->args.argc;
            modified_x.argv = call->args.argv;
            modified_x.function = &afw_function_definition_assign;
 
            AFW_FUNCTION_ASSERT_PARAMETER_COUNT_IS(2);
            result = impl_assign(modified_x.argv[1], AFW_FUNCTION_ARGV(2),
                afw_compile_assignment_type_assign_only,
                p, xctx);

            afw_xctx_evaluation_stack_pop_value(xctx);
            xctx->error->contextual = saved_contextual;
            break;

        case AFW_VALUE_SCRIPT_SUPPORT_NUMBER_BREAK:
            if (!is_loop) {
                AFW_THROW_ERROR_Z(general, "Misplaced break", xctx);
            }
            modified_x.argc = call->args.argc;
            modified_x.argv = call->args.argv;
            modified_x.function = &afw_function_definition_break;
            AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(1);
            *type = afw_value_block_statement_type_break;
            if (AFW_FUNCTION_PARAMETER_IS_PRESENT(1)) {
                result = afw_function_evaluate_required_parameter(
                        &modified_x, 1, NULL);
                if (afw_value_is_script_function_definition(result)) {
                    result = impl_create_closure_if_needed(
                        (const afw_value_script_function_definition_t *)result,
                        p, xctx);
                }
            }
            break;

        case AFW_VALUE_SCRIPT_SUPPORT_NUMBER_CONST:
            afw_xctx_evaluation_stack_push_value(statement, xctx);
            saved_contextual = xctx->error->contextual;
            xctx->error->contextual = call->args.contextual;
            modified_x.argc = call->args.argc;
            modified_x.argv = call->args.argv;
            modified_x.function = &afw_function_definition_const;
            AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MIN(2);
            AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(3);

            /** @fixme process type. */

            result = impl_assign(modified_x.argv[1], AFW_FUNCTION_ARGV(2),
                afw_compile_assignment_type_const,
                p, xctx);

            afw_xctx_evaluation_stack_pop_value(xctx);
            xctx->error->contextual = saved_contextual;
            break;

        case AFW_VALUE_SCRIPT_SUPPORT_NUMBER_CONTINUE:
            if (!is_loop) {
                AFW_THROW_ERROR_Z(general, "Misplaced continue()", xctx);
            }
            modified_x.argc = call->args.argc;
            modified_x.function = &afw_function_definition_continue;
            AFW_FUNCTION_ASSERT_PARAMETER_COUNT_IS(0);

            *type = afw_value_block_statement_type_continue;
            break;

        case AFW_VALUE_SCRIPT_SUPPORT_NUMBER_DO_WHILE:
            afw_xctx_evaluation_stack_push_value(statement, xctx);
            saved_contextual = xctx->error->contextual;
            xctx->error->contextual = call->args.contextual;

            result = afw_value_block_evaluate_do_while(x, type,
                call->args.argc, call->args.argv,
                p, xctx);

            afw_xctx_evaluation_stack_pop_value(xctx);
            xctx->error->contextual = saved_contextual;
            break;

        case AFW_VALUE_SCRIPT_SUPPORT_NUMBER_FOR:
            afw_xctx_evaluation_stack_push_value(statement, xctx);
            saved_contextual = xctx->error->contextual;
            xctx->error->contextual = call->args.contextual;

            result = afw_value_block_evaluate_for(x, type,
                call->args.argc, call->args.argv,
                p, xctx);

            afw_xctx_evaluation_stack_pop_value(xctx);
            xctx->error->contextual = saved_contextual;
            break;

        case AFW_VALUE_SCRIPT_SUPPORT_NUMBER_FOR_OF:
            afw_xctx_evaluation_stack_push_value(statement, xctx);
            saved_contextual = xctx->error->contextual;
            xctx->error->contextual = call->args.contextual;

            result = afw_value_block_evaluate_for_of(x, type,
                call->args.argc, call->args.argv,
                p, xctx);

            afw_xctx_evaluation_stack_pop_value(xctx);
            xctx->error->contextual = saved_contextual;
            break;

        case AFW_VALUE_SCRIPT_SUPPORT_NUMBER_IF:
            afw_xctx_evaluation_stack_push_value(statement, xctx);
            saved_contextual = xctx->error->contextual;
            xctx->error->contextual = call->args.contextual;

            result = afw_value_block_evaluate_if(x, type,
                call->args.argc, call->args.argv, is_loop,
                p, xctx);

            afw_xctx_evaluation_stack_pop_value(xctx);
            xctx->error->contextual = saved_contextual;
            break;

        case AFW_VALUE_SCRIPT_SUPPORT_NUMBER_LET:
            afw_xctx_evaluation_stack_push_value(statement, xctx);
            saved_contextual = xctx->error->contextual;
            xctx->error->contextual = call->args.contextual;
            modified_x.argc = call->args.argc;
            modified_x.argv = call->args.argv;
            modified_x.function = &afw_function_definition_let;
            AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MIN(1);
            AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(3);

            result = impl_assign(modified_x.argv[1], AFW_FUNCTION_ARGV(2),
                afw_compile_assignment_type_let,
                p, xctx);

            afw_xctx_evaluation_stack_pop_value(xctx);
            xctx->error->contextual = saved_contextual;
            break;

        case AFW_VALUE_SCRIPT_SUPPORT_NUMBER_RETHROW:
            /** @fixme check if in catch
            if (!is_in_catch) {
                AFW_THROW_ERROR_Z(general, "Misplaced rethrow", xctx);
            }
             */
            modified_x.argc = call->args.argc;
            modified_x.argv = call->args.argv;
            modified_x.function = &afw_function_definition_rethrow;
            AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(1);
            *type = afw_value_block_statement_type_rethrow;
            break;

        case AFW_VALUE_SCRIPT_SUPPORT_NUMBER_RETURN:
            *type = afw_value_block_statement_type_return;
            modified_x.argc = call->args.argc;
            modified_x.argv = call->args.argv;
            modified_x.function = &afw_function_definition_return;
            if (!allow_return) {
                AFW_THROW_ERROR_Z(general, "Misplaced return()", xctx);
            }
            AFW_FUNCTION_ASSERT_PARAMETER_COUNT_MAX(1);
            if (AFW_FUNCTION_PARAMETER_IS_PRESENT(1)) {
                /* NULL (undefined) is okay here. */
                result = afw_function_evaluate_parameter(&modified_x, 1,
                        NULL);
                if (afw_value_is_script_function_definition(result)) {
                    result = impl_create_closure_if_needed(
                        (const afw_value_script_function_definition_t *)result,
                        p, xctx);
                }
            }
            break;

        case AFW_VALUE_SCRIPT_SUPPORT_NUMBER_THROW:
            afw_xctx_evaluation_stack_push_value(statement, xctx);
            saved_contextual = xctx->error->contextual;
            xctx->error->contextual = call->args.contextual;

            result = afw_value_block_evaluate_throw(x, type,
                call->args.argc, call->args.argv,
                p, xctx);

            afw_xctx_evaluation_stack_pop_value(xctx);
            xctx->error->contextual = saved_contextual;
            break;

        case AFW_VALUE_SCRIPT_SUPPORT_NUMBER_TRY:
            afw_xctx_evaluation_stack_push_value(statement, xctx);
            saved_contextual = xctx->error->contextual;
            xctx->error->contextual = call->args.contextual;

            result = afw_value_block_evaluate_try(x, type,
                call->args.argc, call->args.argv,
                p, xctx);

            afw_xctx_evaluation_stack_pop_value(xctx);
            xctx->error->contextual = saved_contextual;
            break;

        case AFW_VALUE_SCRIPT_SUPPORT_NUMBER_WHILE:
            afw_xctx_evaluation_stack_push_value(statement, xctx);
            saved_contextual = xctx->error->contextual;
            xctx->error->contextual = call->args.contextual;

            result = afw_value_block_evaluate_while(x, type,
                call->args.argc, call->args.argv,
                p, xctx);

            afw_xctx_evaluation_stack_pop_value(xctx);
            xctx->error->contextual = saved_contextual;
            break;

        default:
            result = afw_value_evaluate(statement, p, xctx);
        }
    }

    /* If statement is not special call, just evaluate. */
    else {
        result = afw_value_evaluate(statement, p, xctx);
    }

    return result;
}



AFW_DEFINE(const afw_value_block_t *)
afw_value_block_allocated_and_link(
    const afw_compile_value_contextual_t *contextual,
    afw_value_compiled_value_t *compiled_value,
    afw_size_t *block_count,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_value_block_t *block;

    /* Error if top block has already ended. */
    if (compiled_value->top_block &&
        !compiled_value->current_block)
    {
        /*
         * This is probably caused by parent being passed with top_block
         * but already backed out of current_block.
         */
        AFW_THROW_ERROR_Z(general, "Block block underflow", xctx);

        /** @fixme Remove this code. Saving for now in case error is wrong.
        block = compiled_value->top_block;
        compiled_value->current_block = block;
        return (const afw_value_block_t *)block;
         */
    }
    else {
        block = afw_pool_calloc_type(p, afw_value_block_t, xctx);
        block->inf = &afw_value_block_inf;
        block->contextual = contextual;
    }
    if (!compiled_value->top_block) {
        compiled_value->top_block = block;
    }
    else {
        block->parent_block = compiled_value->current_block;
        block->depth = block->parent_block->depth + 1;
        (*block_count)++;
        block->number = *block_count;
        if (!block->parent_block->first_child_block) {
            block->parent_block->first_child_block = block;
        }
        else {
            block->parent_block->final_child_block->next_sibling_block = block;
        }
        block->parent_block->final_child_block = block;
    }
    compiled_value->current_block = block;

    return (const afw_value_block_t*)block;
}



AFW_DEFINE(const afw_value_t *)
afw_value_block_finalize(
    const afw_value_block_t *block,
    afw_size_t statement_count,
    const afw_value_t * const *statements,
    afw_xctx_t *xctx)
{
    afw_value_block_t *self = (afw_value_block_t *)block;

    self->statement_count = statement_count;
    self->statements = statements;

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
    const afw_value_t *result;
    afw_value_block_statement_type_t type;
    afw_function_execute_t x;

    /** @todo temporary */
    x.p = p;
    x.xctx = xctx;

    /* Evaluate block. */
    result = afw_value_block_evaluate_block(&x, &type, 
        (const afw_value_block_t *)instance, false, p, xctx);

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
    const afw_value_block_t *self =
        (const afw_value_block_t *)instance;
    afw_value_block_symbol_t *symbol;
    afw_size_t i;

    afw_value_compiler_listing_begin_value(writer, instance,
        self->contextual, xctx);

    afw_writer_write_z(writer, " number=", xctx);
    afw_writer_write_size(writer, self->number, xctx);
    afw_writer_write_z(writer, " depth=", xctx);
    afw_writer_write_size(writer, self->depth, xctx);
    afw_writer_write_z(writer, " : [", xctx);
    afw_writer_write_eol(writer, xctx);
    afw_writer_increment_indent(writer, xctx);

    AFW_VALUE_COMPILER_LISTING_IF_NOT_LIMIT_EXCEEDED

    if (self->symbol_count > 0) {
        afw_writer_write_z(writer, "symbols: [", xctx);
        afw_writer_write_eol(writer, xctx);
        afw_writer_increment_indent(writer, xctx);
        for (symbol = self->first_entry; symbol; symbol = symbol->next_entry) {
            afw_writer_write_z(writer, "[", xctx);
            afw_writer_write_size(writer, symbol->index, xctx);
            afw_writer_write_z(writer, "] ", xctx);
            afw_writer_write_utf8(writer,
                afw_value_compiler_listing_symbol_type_name(
                    symbol->symbol_type),
                xctx);
            afw_writer_write_z(writer, " ", xctx);
            afw_value_compiler_listing_name_and_type(writer,
                symbol->name, &symbol->type, xctx);
            afw_writer_write_eol(writer, xctx);
        }
        afw_writer_decrement_indent(writer, xctx);
        afw_writer_write_z(writer, "]", xctx);
        afw_writer_write_eol(writer, xctx);
    }

    if (self->statement_count > 0) {
        afw_writer_write_z(writer, "statements: [", xctx);
        afw_writer_write_eol(writer, xctx);
        afw_writer_increment_indent(writer, xctx);
        for (i = 0; i < self->statement_count; i++) {
            afw_value_produce_compiler_listing(self->statements[i], writer, xctx);
        }
        afw_writer_decrement_indent(writer, xctx);
        afw_writer_write_z(writer, "]", xctx);
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
    /*

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
    const afw_value_block_t *self =
        (const afw_value_block_t *)instance;

    afw_memory_clear(info);
    info->value_inf_id = &instance->inf->rti.implementation_id;
    info->contextual = self->contextual;
    info->optimized_value = instance;

    /* Note: Maybe something can be done for optimized_value_data_type. */
}
