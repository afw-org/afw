// See the 'COPYING' file in the project root for licensing information.
/*
 * AFW runtime functions support
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function.c
 * @brief AFW runtime functions support  
 *
 */

#include "afw_internal.h"

AFW_DEFINE_INTERNAL(void)
afw_function_internal_prepare_environment(afw_xctx_t *xctx)
{
    afw_function_environment_t *e;

    e = afw_xctx_calloc_type(afw_function_environment_t, xctx);
    ((afw_environment_t *)xctx->env)->function_environment = e;

    e->add_operator_function =
        afw_environment_get_function(&afw_s_add, xctx);

    e->subtract_operator_function =
        afw_environment_get_function(&afw_s_subtract, xctx);

    e->multiply_operator_function =
        afw_environment_get_function(&afw_s_multiply, xctx);

    e->divide_operator_function =
        afw_environment_get_function(&afw_s_divide, xctx);

    e->modulus_operator_function =
        afw_environment_get_function(&afw_s_mod, xctx);

    e->exponentiation_operator_function =
        afw_environment_get_function(&afw_s_pow, xctx);

    e->negative_operator_function =
        afw_environment_get_function(&afw_s_negative, xctx);

    e->and_operator_function =
        afw_environment_get_function(&afw_s_and, xctx);

    e->or_operator_function =
        afw_environment_get_function(&afw_s_or, xctx);

    e->unary_not_operator_function =
        afw_environment_get_function(&afw_s_not, xctx);

    e->equal_to_operator_function =
        afw_environment_get_function(&afw_s_eq, xctx);

    e->not_equal_to_operator_function =
        afw_environment_get_function(&afw_s_ne, xctx);

    e->less_than_operator_function =
        afw_environment_get_function(&afw_s_lt, xctx);

    e->less_than_or_equal_to_function =
        afw_environment_get_function(&afw_s_le, xctx);

    e->greater_than_operator_function =
        afw_environment_get_function(&afw_s_gt, xctx);

    e->greater_than_or_equal_to_function =
        afw_environment_get_function(&afw_s_ge, xctx);
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



/* Evaluate function parameter. */
/* Note: only called by higher_order_list at the moment */
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



/* Evaluate a value and cast if necessary without throwing error. */
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

    /* Get function parameter from function definition.  */
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
    }

    /* If result is undefined, return NULL. Fuss if required. */
    if (afw_value_is_undefined(result)) {
        if (
            parameter_number <= x->function->numberOfRequiredParameters && 
            !parameter->optional &&
            !parameter->canBeUndefined)
        {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Parameter " AFW_SIZE_T_FMT
                " of function " AFW_UTF8_FMT
                " can not be undefined",
                parameter_number,
                AFW_UTF8_FMT_ARG(&x->function->functionId));
        }
        afw_xctx_evaluation_stack_pop_parameter_number(xctx);
        return result;
    }

    /* Get result's data type. */
    result_data_type = afw_value_get_data_type(result, xctx);

    /* Error if result data type doesn't match parameter's defined data type. */
    if (parameter->data_type && result_data_type) {
        if (parameter->data_type != afw_data_type_list && //@fixme don't require
            (parameter->data_type != result_data_type))
        {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Parameter " AFW_SIZE_T_FMT
                " of function " AFW_UTF8_FMT
                " must evaluate to data type " AFW_UTF8_FMT
                " but evaluated to be " AFW_UTF8_FMT,
                parameter_number,
                AFW_UTF8_FMT_ARG(&x->function->functionId),
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



/* Evaluate a parameter with dataTypeParameter. */
/* Note: This is only called by impl_call_script_function at the moment. */
AFW_DEFINE(const afw_value_t*)
afw_function_evaluate_parameter_with_type(
    const afw_value_t* value,
    afw_size_t parameter_number,
    const afw_value_type_t *type,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    const afw_value_t *result;

    /* Make sure value is undecorated. */
    AFW_VALUE_UNDECORATE(value);

    /** @fixme add support for dataTypeParameter. */

    /* Just return if no evaluation or conversion needed. */
    if (afw_value_is_defined_and_evaluated(value) &&
        (!type || !type->data_type || type->data_type == afw_data_type_any ||
            afw_value_get_data_type(value, xctx) == type->data_type))
    {
        return value;
    }

    /* Push parameter number on evaluation stack. */
    afw_xctx_evaluation_stack_push_parameter_number(parameter_number, xctx);

    /* Evaluate value. */
    result = afw_value_evaluate(value, p, xctx);

    /* Convert to requested data type if needed. */
    if (result && type && type->data_type &&
        afw_value_get_data_type(value, xctx) != type->data_type)
    {
        result = afw_value_convert(result, type->data_type, true, p, xctx);
    }

    /* Pop parameter number from evaluation stack and return result. */
    afw_xctx_evaluation_stack_pop_parameter_number(xctx);
    return result;
}

