// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW runtime functions support
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function.c
 * @brief Built-in Adaptive function execute helpers.
 *
 * AFW_FUNCTION_EVALUATE_* → afw_function_evaluate_parameter /
 * evaluate_required_parameter. Definition-driven leaf formals; no script
 * typeCheck. Script formals and IR ops: afw_function_compiler_internal.c.
 */

#include "afw_internal.h"
#include <stdio.h>

/*
 * Call-site contextual for nested call_create from execute_*. Public so
 * extensions need not see afw_value_call_built_in_function_t layout.
 */
AFW_DEFINE(const afw_compile_value_contextual_t *)
afw_function_execute_contextual(const afw_function_execute_t *x)
{
    if (!x || !x->self) {
        return NULL;
    }
    return x->self->args.contextual;
}

void
afw_function_internal_prepare_environment(afw_xctx_t *xctx)
{
    afw_function_environment_t *e;

    e = afw_pool_calloc_type(xctx->env->p, afw_function_environment_t, xctx);
    ((afw_environment_t *)xctx->env)->function_environment = e;

    e->add_operator_function =
        afw_environment_get_function(afw_s_add, xctx);

    e->subtract_operator_function =
        afw_environment_get_function(afw_s_subtract, xctx);

    e->multiply_operator_function =
        afw_environment_get_function(afw_s_multiply, xctx);

    e->divide_operator_function =
        afw_environment_get_function(afw_s_divide, xctx);

    e->modulus_operator_function =
        afw_environment_get_function(afw_s_mod, xctx);

    e->exponentiation_operator_function =
        afw_environment_get_function(afw_s_pow, xctx);

    e->negative_operator_function =
        afw_environment_get_function(afw_s_negative, xctx);

    e->and_operator_function =
        afw_environment_get_function(afw_s_and, xctx);

    e->or_operator_function =
        afw_environment_get_function(afw_s_or, xctx);

    e->unary_not_operator_function =
        afw_environment_get_function(afw_s_not, xctx);

    e->equal_to_operator_function =
        afw_environment_get_function(afw_s_eq, xctx);

    e->not_equal_to_operator_function =
        afw_environment_get_function(afw_s_ne, xctx);

    e->less_than_operator_function =
        afw_environment_get_function(afw_s_lt, xctx);

    e->less_than_or_equal_to_function =
        afw_environment_get_function(afw_s_le, xctx);

    e->greater_than_operator_function =
        afw_environment_get_function(afw_s_gt, xctx);

    e->greater_than_or_equal_to_function =
        afw_environment_get_function(afw_s_ge, xctx);
}



/*  Adaptive function: convert arg to <datatype> */
const afw_value_t *
afw_function_execute_convert(
    afw_function_execute_t *x)
{
    const afw_value_t *result;
    afw_xctx_t *xctx = x->xctx;

    result = (x->argc >= 1) ? x->argv[1] : NULL;
    result = afw_value_evaluate(result, x->p, xctx);
    if (!result) {
        AFW_THROW_ERROR_Z(undefined_value,
            "Parameter 1 is undefined value", xctx);
    }
    return afw_value_convert(result,
        x->function->returns->data_type, false, x->p, xctx);
}


/* Execute function if caller has 'execute' access. */
const afw_value_t *
afw_function_execute_requiresExecuteAccess_wrapper(
    afw_function_execute_t *x)
{
    const afw_value_t *result = NULL;
    afw_xctx_t *xctx = x->xctx;
    const afw_object_t *obj = NULL;
    afw_function_execute_t *temp_x;
    const afw_value_t **argv;
    afw_size_t argc;
    const afw_array_t *argv_array;
    const afw_value_function_parameter_t * const *parameter;

    AFW_TRY {
        /* Make an object to pass to authorization check. */
        obj = afw_object_create_unmanaged_new_p(x->p, xctx);

        /*
         * Use the object's pool for a temporary copy of x and an evaluated
         * version of argv. These evaluated args will be place in the object
         * passed to the authorization check and also passed to
         * x->function->execute_implementation. This is done so that the args
         * are evaluated only once.
         *
         * This is not compatible with functions that only evaluate args when
         * needed link and/or and for polymorphic functions that need to
         * evaluate the first arg to determine the correct function to call.
         * afwdev generate should throw an error if this is attempted.
         */
        temp_x = afw_pool_calloc_type(obj->p, afw_function_execute_t, xctx);
        afw_memory_copy(temp_x, x);
        argv = afw_pool_malloc(
            obj->p,
            x->argc * sizeof(afw_value_t *) + sizeof(afw_value_t *),
            xctx);
        temp_x->argv = argv;
        argv[0] = x->argv[0];
        for (argc = 1; argc <= x->argc; argc++) {
            argv[argc] = afw_value_evaluate(x->argv[argc], x->p, xctx);
        }

        /* Set properties in object to be available in authorization check. */
        afw_object_set_property_as_object_internal(
            obj, afw_v_function, x->function->object, xctx);
        argv_array = afw_array_create_unmanaged_from_values(
            NULL, &argv[1], x->argc, obj->p, xctx);
        afw_object_set_property_as_array_internal(
            obj, afw_v_arguments, argv_array, xctx);
        for (argc = 1, parameter = x->function->parameters;
            argc <= x->function->parameters_count;
            argc++, parameter++)
        {
            if (argc == x->function->parameters_count &&
                (*parameter)->minArgs->internal != -1)
            {
                if (argc <= x->argc) {
                    argv_array = afw_array_create_unmanaged_from_values(
                        NULL, &argv[argc], x->argc - argc, obj->p, xctx);
                }
                else {
                    argv_array = afw_array_create_unmanaged_from_values(
                        NULL, &argv[1], 0, obj->p, xctx);
                }
                afw_object_set_property_as_array_internal(
                    obj, &(*parameter)->name->pub, argv_array, xctx);
            }
            else {
                afw_object_set_property(
                    obj,
                    &(*parameter)->name->pub,
                    (argc <= x->argc) ? argv[argc] : NULL,
                    xctx);
            }
        }
        
        /* Check authorization and throw error if not allowed. */
        afw_authorization_check(
            true,
            NULL,
            (const afw_value_t *)x->function->functionResourceId,
            obj->value,
            afw_authorization_action_id_execute,
            x->p,
            xctx);

        /* Call function implementation with temp_x. */
        result = x->function->execute_implementation(temp_x);
    }
    AFW_FINALLY {

        /* If obj was create, release which will also release its pool. */
        if (obj) {
            afw_object_release(obj, xctx);
        }
    }
    AFW_ENDTRY;

    /* Return result if error was not thrown. */
    return result;
}


/* Evaluate function parameter. */
/* Note: only called by higher_order_array at the moment */
AFW_DEFINE(const afw_value_t *)
afw_function_evaluate_function_parameter(
    const afw_value_t *function_arg,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_t *result;
    afw_utf8_t qualifier;
    afw_utf8_t name;

    result = afw_value_evaluate(function_arg, p, xctx);

    /* Strings are looked up dynamically. Note: this may become deprecated. */
    if (afw_value_is_string(result)) {
        afw_compile_split_qualified_name(
            &((const afw_value_string_t *)result)->internal,
            &qualifier, &name, xctx);

        /* Built-in function */
        result = (const afw_value_t *)
            afw_environment_get_qualified_function(
                &qualifier, &name, xctx);
    }

    return result;
}



/*
 * Built-in formal: eval + definition metadata + optional convert to caller's
 * requested leaf data_type. Not script typeCheck (see compiler_internal).
 */
AFW_DEFINE(const afw_value_t *)
afw_function_evaluate_parameter(
    afw_function_execute_t *x,
    afw_size_t parameter_number,
    const afw_data_type_t *data_type)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_value_t *result;
    const afw_data_type_t *result_data_type;
    const afw_value_function_parameter_t *parameter;

    /* Formal metadata from Adaptive function definition. */
    parameter = x->function->parameters[
            (
                (parameter_number <= x->function->parameters_count)
                ? parameter_number
                : x->function->parameters_count
            )
            - 1
        ];

    /* Get possibly unevaluated result from argv. */
    result = ((parameter_number <= x->argc) ? x->argv[parameter_number] : NULL);

    result = afw_value_evaluate(result, x->p, xctx);

    /* If result is undefined, return NULL. Fuss if required. */
    if (afw_value_is_undefined(result)) {
        /*
         * Always use NULL for undefined in this case so that caller can use
         * NULL to check if a parameter is not present.
         */
        result = NULL;
        if (
            parameter_number <=
                x->function->numberOfRequiredParameters->internal && 
            !afw_value_is_boolean_true(parameter->optional) &&
            !afw_value_is_boolean_true(parameter->canBeUndefined))
        {
            AFW_THROW_ERROR_FZ(undefined_value, xctx,
                "Parameter " AFW_SIZE_T_FMT
                " of function '%ku' can not be undefined",
                parameter_number,
                &x->function->functionId->internal);
        }
        return result;
    }

    /*
     * Caller requested array (EVALUATE_* … array): materialize keyless-
     * iterator sequences (utf8 code points) as a temporary array (#153).
     * Do not key off parameter->data_type alone — polymorphic bag rest
     * formals are typed array in metadata but take scalar bag members
     * (XACML bag-of-one), not code-point sequences.
     */
    if (data_type == afw_data_type_array) {
        result = afw_value_convert_to_array_sequence(result, x->p, xctx);
    }

    /* Get result's data type. */
    result_data_type = afw_value_get_data_type(result, xctx);

    /* Error if result data type doesn't match parameter's defined data type. */
    if (parameter->data_type && result_data_type) {
        if (parameter->data_type != afw_data_type_array && //@fixme don't require
            (parameter->data_type != result_data_type))
        {
            AFW_THROW_ERROR_FZ(argument_error, xctx,
                "Parameter " AFW_SIZE_T_FMT
                " of function '%ku' must evaluate to data type '%ku' but evaluated to be '%ku'",
                parameter_number,
                &x->function->functionId->internal,
                &parameter->data_type->data_type_id,
                &result_data_type->data_type_id);
        }
    }

    /* Convert to requested data type if needed. */
    if (data_type && result_data_type && result_data_type != data_type)
    {
        result = afw_value_convert(result, data_type, false, x->p, xctx);
    }

    return result;
}



/* Evaluate a value and convert if necessary without throwing error. */
AFW_DEFINE(const afw_value_t *)
afw_function_evaluate_required_parameter(
    afw_function_execute_t *x,
    afw_size_t parameter_number,
    const afw_data_type_t *data_type)
{
    afw_xctx_t *xctx = x->xctx;
    const afw_value_t *result;

    /* Evaluate parameter. */
    result = afw_function_evaluate_parameter(x, parameter_number, data_type);

    /* If result is NULL, throw error with parameter # on evaluation stack. */
    if (!result) {
        afw_xctx_evaluation_stack_push_parameter_number(
            parameter_number, xctx);
        AFW_THROW_ERROR_FZ(undefined_value, xctx,
            "Parameter " AFW_SIZE_T_FMT " is undefined value",
            parameter_number);
    }

    /* Return result that will not be NULL. */
    return result;
}


#define IMPL_EVAL_UNIT_SEEN_MAX 64


static afw_boolean_t
impl_function_defined_in_unit(
    const afw_value_script_function_definition_t *function,
    const afw_value_compiled_value_t *unit)
{
    const afw_value_block_t *block;

    if (!function || !unit) {
        return false;
    }
    for (block = function->enclosing_block; block;
        block = block->parent_block)
    {
        if (block == unit->top_block) {
            return true;
        }
    }
    return false;
}


/* 0 add and walk, 1 already seen skip, 2 list full keep. */
static int
impl_eval_unit_seen_add(
    const void **seen,
    afw_size_t *count,
    const void *p)
{
    afw_size_t i;

    for (i = 0; i < *count; i++) {
        if (seen[i] == p) {
            return 1;
        }
    }
    if (*count >= IMPL_EVAL_UNIT_SEEN_MAX) {
        return 2;
    }
    seen[(*count)++] = p;
    return 0;
}


static void
impl_eval_unit_set_unexpected(
    char *unexpected,
    afw_size_t unexpected_size,
    const char *label,
    const char *detail)
{
    if (!unexpected || unexpected_size == 0) {
        return;
    }
    snprintf(unexpected, unexpected_size,
        "Internal error: %s %s", label, detail);
}


/*
 * Named `function a(); return a` leaves inner in a frame slot of
 * the eval unit root. Factory return last-releases on caller->p
 * and may leave last_statement_non_void_value pointing at inner. Unlink those
 * pointers without release (the slot hold is stolen with the
 * pin) so free of the inner header is not a later UAF.
 */
static void
impl_eval_unlink_inner_from_scopes(
    afw_value_closure_binding_t *inner,
    afw_xctx_t *xctx)
{
    const afw_pool_scope_t *s;
    afw_pool_scope_t *mut;
    afw_size_t i;

    (void)xctx;
    for (s = inner->enclosing_lexical_scope; s;
        s = s->parent_lexical_scope)
    {
        mut = (afw_pool_scope_t *)s;
        for (i = 0; i < mut->symbol_count; i++) {
            if (mut->frame_slots[i] == &inner->pub) {
                mut->frame_slots[i] = afw_value_undefined;
            }
        }
        if (mut->last_statement_non_void_value == &inner->pub) {
            mut->last_statement_non_void_value = afw_value_void;
        }
    }
}


/* 0 referenced or nothing, 2 could not (unexpected). */
static int
impl_eval_pin_binding(
    afw_value_closure_binding_t *binding,
    const afw_value_t *compiled,
    char *unexpected,
    afw_size_t unexpected_size,
    const char *label,
    afw_xctx_t *xctx)
{
    if (binding->compiled_value == compiled) {
        return 0;
    }
    if (binding->compiled_value) {
        impl_eval_unit_set_unexpected(unexpected, unexpected_size,
            label, "closure_binding already keeps a compiled value");
        return 2;
    }
    afw_value_get_reference(compiled, xctx);
    binding->compiled_value = compiled;
    return 0;
}


/*
 * Walk a nested result. get_reference each closure from this unit.
 * 0 means dest p last-release of the compile birth hold is enough.
 * 2 means an unbound definition, seen-list full, or a binding that
 * already keeps another unit.
 */
static int
impl_value_pin_nested_unit(
    const afw_value_t *value,
    const afw_value_t *compiled,
    const afw_value_compiled_value_t *unit,
    const void **seen,
    afw_size_t *seen_count,
    char *unexpected,
    afw_size_t unexpected_size,
    const char *label,
    afw_xctx_t *xctx)
{
    int seen_st;
    int st;
    int child_st;

    if (!value || afw_value_is_undefined(value) || afw_value_is_void(value)) {
        return 0;
    }
    if (afw_value_is_closure_binding(value)) {
        afw_value_closure_binding_t *binding;

        binding = (afw_value_closure_binding_t *)value;
        if (!impl_function_defined_in_unit(
            binding->script_function_definition, unit))
        {
            return 0;
        }
        return impl_eval_pin_binding(binding, compiled, unexpected,
            unexpected_size, label, xctx);
    }
    if (afw_value_is_script_function_definition(value)) {
        if (impl_function_defined_in_unit(
            (const afw_value_script_function_definition_t *)value,
            unit))
        {
            return 2;
        }
        return 0;
    }
    st = 0;
    if (afw_value_is_object(value)) {
        const afw_object_t *obj;
        const afw_iterator_old_t *iterator;
        const afw_value_t *name;
        const afw_value_t *prop;

        obj = ((const afw_value_object_t *)value)->internal;
        if (!obj) {
            return 0;
        }
        seen_st = impl_eval_unit_seen_add(seen, seen_count, obj);
        if (seen_st == 1) {
            return 0;
        }
        if (seen_st == 2) {
            return 2;
        }
        for (iterator = NULL;;) {
            prop = afw_object_get_next_property(obj, &iterator, &name, xctx);
            if (!prop) {
                break;
            }
            child_st = impl_value_pin_nested_unit(prop, compiled, unit,
                seen, seen_count, unexpected, unexpected_size, label,
                xctx);
            if (child_st == 2) {
                st = 2;
            }
        }
        return st;
    }
    if (afw_value_is_array(value)) {
        const afw_array_t *list;
        const afw_iterator_old_t *iterator;
        const afw_value_t *entry;

        list = ((const afw_value_array_t *)value)->internal;
        if (!list) {
            return 0;
        }
        seen_st = impl_eval_unit_seen_add(seen, seen_count, list);
        if (seen_st == 1) {
            return 0;
        }
        if (seen_st == 2) {
            return 2;
        }
        for (iterator = NULL;;) {
            entry = afw_array_get_next_value(list, &iterator, xctx);
            if (!entry) {
                break;
            }
            child_st = impl_value_pin_nested_unit(entry, compiled, unit,
                seen, seen_count, unexpected, unexpected_size, label,
                xctx);
            if (child_st == 2) {
                st = 2;
            }
        }
        return st;
    }
    return 0;
}


AFW_DEFINE(const afw_value_t *)
afw_function_eval_reference_escaped_unit(
    const afw_value_t *compiled,
    const afw_value_t *value,
    char *unexpected,
    afw_size_t unexpected_size,
    const char *label,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_value_compiled_value_t *unit;
    afw_value_closure_binding_t *binding;
    afw_value_closure_binding_t *inner;
    const void *seen[IMPL_EVAL_UNIT_SEEN_MAX];
    afw_size_t seen_count;
    afw_boolean_t transferred;

    if (unexpected && unexpected_size > 0) {
        unexpected[0] = 0;
    }
    transferred = false;
    if (value && xctx->error_processing_count == 0 &&
        afw_value_is_compiled_value(compiled))
    {
        unit = (const afw_value_compiled_value_t *)compiled;
        if (afw_value_is_closure_binding(value)) {
            binding = (afw_value_closure_binding_t *)value;
            if (impl_function_defined_in_unit(
                binding->script_function_definition, unit))
            {
                if (binding->compiled_value) {
                    impl_eval_unit_set_unexpected(unexpected,
                        unexpected_size, label,
                        "closure_binding already keeps a compiled value");
                }
                else if (binding->reference_count == 0) {
                    impl_eval_unit_set_unexpected(unexpected,
                        unexpected_size, label,
                        "closure_binding has no scope reference to transfer");
                }
                else {
                    /*
                     * Return a new binding at RC 0 that keeps the
                     * unit. The inner evaluate result pins a child
                     * of dest p, so dest-p last-release of that
                     * inner cannot run until the pin drops. Hand
                     * the pin and the unit to the new header, drop
                     * dest-p last-release of the inner, and free
                     * the inner header.
                     */
                    inner = binding;
                    binding = (afw_value_closure_binding_t *)
                        afw_value_closure_binding_create(
                            inner->script_function_definition,
                            inner->enclosing_lexical_scope,
                            p, xctx);
                    afw_value_get_reference(compiled, xctx);
                    binding->compiled_value = compiled;
                    /*
                     * compiled_value evaluate last-releases dest p.
                     * A script-function return also last-releases
                     * caller->p (eval unit root or make()'s caller).
                     * Named `function a(); return a` still holds
                     * inner in a frame slot: unlink then free.
                     */
                    afw_pool_deregister_value_at_cleanup(
                        &inner->pub, p, xctx);
                    {
                        const afw_pool_scope_t *s;

                        for (s = inner->enclosing_lexical_scope; s;
                            s = s->parent_lexical_scope)
                        {
                            if (s->p && s->p != p) {
                                afw_pool_deregister_value_at_cleanup(
                                    &inner->pub, s->p, xctx);
                            }
                        }
                    }
                    impl_eval_unlink_inner_from_scopes(inner, xctx);
                    afw_pool_free_memory_type(inner->p, inner,
                        afw_value_closure_binding_t, xctx);
                    value = &binding->pub;
                    transferred = true;
                }
            }
        }
        else if (afw_value_is_script_function_definition(value) &&
            impl_function_defined_in_unit(
                (const afw_value_script_function_definition_t *)
                value, unit))
        {
            impl_eval_unit_set_unexpected(unexpected, unexpected_size,
                label,
                "result is an unbound script function in its compile unit");
        }
        else {
            seen_count = 0;
            impl_value_pin_nested_unit(value, compiled, unit,
                seen, &seen_count, unexpected, unexpected_size,
                label, xctx);
        }
    }
    if (value && !transferred) {
        /* Evaluate already registered last-release on dest p. */
        value = afw_pool_scope_get_assignable_for_p_lifetime(
            value, p, xctx);
    }
    return value;
}




