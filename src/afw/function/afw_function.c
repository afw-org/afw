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

AFW_DEFINE_INTERNAL(void)
afw_function_internal_prepare_environment(afw_xctx_t *xctx)
{
    afw_function_environment_t *e;

    e = afw_xctx_calloc_type(afw_function_environment_t, xctx);
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

    result = afw_function_evaluate_required_parameter(x, 1, NULL);

    afw_xctx_evaluation_stack_push_parameter_number(1, xctx);
    result = afw_value_convert(result,
        x->function->returns->data_type, false, x->p, xctx);
    afw_xctx_evaluation_stack_pop_parameter_number(xctx);

    return result;
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
        obj = afw_object_create(x->p, xctx);

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
        afw_object_set_property_as_object(
            obj, afw_s_function, x->function->object, xctx);
        argv_array = afw_array_const_create_array_of_values(
            &argv[1], x->argc, obj->p, xctx);
        afw_object_set_property_as_array(
            obj, afw_s_arguments, argv_array, xctx);
        for (argc = 1, parameter = x->function->parameters;
            argc <= x->function->parameters_count;
            argc++, parameter++)
        {
            if (argc == x->function->parameters_count &&
                (*parameter)->minArgs->internal != -1)
            {
                if (argc <= x->argc) {
                    argv_array = afw_array_const_create_array_of_values(
                        &argv[argc], x->argc - argc, obj->p, xctx);
                }
                else {
                    argv_array = afw_array_const_create_array_of_values(
                        &argv[1], 0, obj->p, xctx);
                }
                afw_object_set_property_as_array(
                    obj, &(*parameter)->name->internal, argv_array, xctx);
            }
            else {
                afw_object_set_property(
                    obj,
                    &(*parameter)->name->internal,
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

    /* Push parameter number on evaluation stack. */
    afw_xctx_evaluation_stack_push_parameter_number(parameter_number, xctx);

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

    /* Evaluate result if needed. */
    if (!afw_value_is_defined_and_evaluated(result)) {
        result = afw_value_evaluate(result, x->p, xctx);
        if (afw_value_is_compiled_value(result) &&
            data_type != afw_data_type_unevaluated)
        {
            result = afw_value_evaluate(result, x->p, xctx);
        }
    }

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
            AFW_THROW_ERROR_FZ(general, xctx,
                "Parameter " AFW_SIZE_T_FMT
                " of function " AFW_UTF8_FMT_Q
                " can not be undefined",
                parameter_number,
                AFW_UTF8_FMT_ARG(&x->function->functionId->internal));
        }
        afw_xctx_evaluation_stack_pop_parameter_number(xctx);
        return result;
    }

    /* Get result's data type. */
    result_data_type = afw_value_get_data_type(result, xctx);

    /* Error if result data type doesn't match parameter's defined data type. */
    if (parameter->data_type && result_data_type) {
        if (parameter->data_type != afw_data_type_array && //@fixme don't require
            (parameter->data_type != result_data_type))
        {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Parameter " AFW_SIZE_T_FMT
                " of function " AFW_UTF8_FMT_Q
                " must evaluate to data type " AFW_UTF8_FMT_Q
                " but evaluated to be " AFW_UTF8_FMT_Q,
                parameter_number,
                AFW_UTF8_FMT_ARG(&x->function->functionId->internal),
                AFW_UTF8_FMT_ARG(&parameter->data_type->data_type_id),
                AFW_UTF8_FMT_ARG(&result_data_type->data_type_id));
        }
    }

    /* Convert to requested data type if needed. */
    if (data_type && result_data_type && result_data_type != data_type)
    {
        result = afw_value_convert(result, data_type, false, x->p, xctx);
    }

    /* Pop parameter number from evaluation stack and return result. */
    afw_xctx_evaluation_stack_pop_parameter_number(xctx);
    return result;
}



/* Evaluate a value and cast if necessary without throwing error. */
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
        AFW_THROW_ERROR_FZ(undefined, xctx,
            "Parameter " AFW_SIZE_T_FMT " is undefined value",
            parameter_number);
        afw_xctx_evaluation_stack_pop_parameter_number(xctx);
    }

    /* Return result that will not be NULL. */
    return result;
}




