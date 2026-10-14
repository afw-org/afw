// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for integer
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_integer.c
 * @brief Adaptive function execute implementations for category `integer`.
 */

#include "afw_internal.h"



/*
 * Adaptive function: abs<integer>
 *
 * afw_function_execute_abs_integer
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Compute the absolute value of the integer value and return the integer
 * result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function abs<integer>(
 *       value: integer
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   value - (integer)
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_abs_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, integer);

    if (arg->internal == AFW_INTEGER_MIN) {
        AFW_THROW_ERROR_Z(argument_error, "Integer abs overflow", x->xctx);
    }

    return (arg->internal >= 0)
        ? &arg->pub
        : afw_value_create_unmanaged_integer(-(arg->internal), x->p, x->xctx);
}



/*
 * Adaptive function: add<integer>
 *
 * afw_function_execute_add_integer
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Add 2 or more integer values and return the integer result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function add<integer>(
 *       values_1: integer,
 *       values_2: integer,
 *       ...values_rest: integer[]
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   values - (2 or more integer)
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_add_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg;
    afw_integer_t sum;
    afw_size_t n;

    for (sum = 0, n = 1; n <= x->argc; n++) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, n, integer);
        if ((arg->internal > 0 && (sum > AFW_INTEGER_MAX - arg->internal)) ||
            (arg->internal < 0 && (sum < AFW_INTEGER_MIN - arg->internal)))
        {
            AFW_THROW_ERROR_Z(argument_error, "Integer add overflow", x->xctx);
        }
        sum += arg->internal;
    }

    return afw_value_create_unmanaged_integer(sum, x->p, x->xctx);
}



/*
 * Adaptive function: divide<integer>
 *
 * afw_function_execute_divide_integer
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Divide integer dividend by integer divisor and return the integer quotient.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function divide<integer>(
 *       dividend: integer,
 *       divisor: integer
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   dividend - (integer)
 *
 *   divisor - (integer)
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_divide_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg1;
    const afw_value_integer_t *arg2;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg1, 1, integer);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg2, 2, integer);

    if (arg2->internal == 0) {
        AFW_THROW_ERROR_Z(argument_error, "Integer divide by zero error", x->xctx);
    }
    /* The quotient does not fit, and the CPU traps (SIGFPE). */
    if (arg1->internal == AFW_INTEGER_MIN && arg2->internal == -1) {
        AFW_THROW_ERROR_Z(argument_error, "Integer divide overflow", x->xctx);
    }

    return afw_value_create_unmanaged_integer(
        arg1->internal / arg2->internal,
        x->p, x->xctx);
}



/*
 * Adaptive function: mod<integer>
 *
 * afw_function_execute_mod_integer
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Divide integer dividend by integer divisor and return the integer remainder.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function mod<integer>(
 *       dividend: integer,
 *       divisor: integer
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   dividend - (integer)
 *
 *   divisor - (integer)
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_mod_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg1;
    const afw_value_integer_t *arg2;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg1, 1, integer);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg2, 2, integer);

    if (arg2->internal == 0) {
        AFW_THROW_ERROR_Z(argument_error, "Integer divide by zero error", x->xctx);
    }
    /* The remainder is 0, but the CPU traps (SIGFPE) computing it. */
    if (arg2->internal == -1) {
        return afw_value_create_unmanaged_integer(0, x->p, x->xctx);
    }

    return afw_value_create_unmanaged_integer(
        arg1->internal % arg2->internal,
        x->p, x->xctx);
}



/*
 * Adaptive function: multiply<integer>
 *
 * afw_function_execute_multiply_integer
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Multiply 2 or more integer values and return the integer result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function multiply<integer>(
 *       values_1: integer,
 *       values_2: integer,
 *       ...values_rest: integer[]
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   values - (2 or more integer)
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_multiply_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg;
    afw_integer_t result, b;
    afw_size_t n;

    for (result = 1, n = 1; n <= x->argc; n++) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, n, integer);
        b = arg->internal;
        /*
         * Check before multiplying: signed overflow is undefined, so
         * a check after the multiply is not guaranteed to run.
         */
        if (result > 0 ?
            (b > 0 ? result > AFW_INTEGER_MAX / b
                : b < AFW_INTEGER_MIN / result) :
            (b > 0 ? result < AFW_INTEGER_MIN / b
                : (result != 0 && b < AFW_INTEGER_MAX / result)))
        {
            AFW_THROW_ERROR_Z(argument_error, "Integer multiply overflow", x->xctx);
        }
        result *= b;
    }

    return afw_value_create_unmanaged_integer(result, x->p, x->xctx);
}



/*
 * Adaptive function: pow<integer>
 *
 * afw_function_execute_pow_integer
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This returns the value of base raised to a power. Multiple exponents can be
 * specified to raise the previous exponent to the power of the latter exponent.
 * An integer result is exact: a negative exponent or a result too large for an
 * integer is an error.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function pow<integer>(
 *       base: integer,
 *       exponent_1: integer,
 *       ...exponent_rest: integer[]
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   base - (integer) Base value.
 *
 *   exponent - (1 or more integer) Exponent value.
 *
 * Returns:
 *
 *   (integer) Base raised to the power.
 */
const afw_value_t *
afw_function_execute_pow_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg;
    afw_integer_t exponent;
    afw_integer_t base;
    afw_integer_t result;
    afw_size_t i;

    /* Right to left: pow(a, b, c) is a raised to (b raised to c). */
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, x->argc, integer);
    exponent = arg->internal;
    for (i = x->argc - 1; i >= 1; i--) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, i, integer);
        base = arg->internal;
        if (exponent < 0) {
            AFW_THROW_ERROR_Z(argument_error,
                "Integer pow with a negative exponent", x->xctx);
        }
        /* These never overflow, however large the exponent. */
        if (base == 0 || base == 1) {
            result = (exponent == 0) ? 1 : base;
        }
        else if (base == -1) {
            result = (exponent % 2 == 0) ? 1 : -1;
        }
        /* |base| >= 2, so this overflows in at most 64 steps. */
        else {
            for (result = 1; exponent > 0; exponent--) {
                if (base > 0
                    ? (result > 0
                        ? result > AFW_INTEGER_MAX / base
                        : result < AFW_INTEGER_MIN / base)
                    : (result > 0
                        ? result > AFW_INTEGER_MIN / base
                        : result < AFW_INTEGER_MAX / base))
                {
                    AFW_THROW_ERROR_Z(argument_error,
                        "Integer pow overflow", x->xctx);
                }
                result *= base;
            }
        }
        exponent = result;
    }
    return afw_value_create_unmanaged_integer(exponent, x->p, x->xctx);
}



/*
 * Adaptive function: subtract<integer>
 *
 * afw_function_execute_subtract_integer
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Subtract integer arg2 from integer arg1 and return the integer result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function subtract<integer>(
 *       arg1: integer,
 *       arg2: integer
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (integer)
 *
 *   arg2 - (integer)
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_subtract_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg1;
    const afw_value_integer_t *arg2;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg1, 1, integer);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg2, 2, integer);


    /*
     * Check without negating arg2: -AFW_INTEGER_MIN overflows, and
     * signed overflow is undefined. MAX + a negative and MIN + a
     * positive cannot overflow.
     */
    if ((arg2->internal < 0 && arg1->internal > AFW_INTEGER_MAX + arg2->internal) ||
        (arg2->internal > 0 && arg1->internal < AFW_INTEGER_MIN + arg2->internal))
    {
        AFW_THROW_ERROR_Z(argument_error, "Integer subtract overflow", x->xctx);
    }

    return afw_value_create_unmanaged_integer(
        arg1->internal - arg2->internal,
        x->p, x->xctx);
}



/*
 * Adaptive function: to_double<integer>
 *
 * afw_function_execute_to_double_integer
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Converts integer arg to double and returns double result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters.
 *
 * Parameters:
 *    arg - (integer)
 *
 * Returns:
 *    (double)
 */
const afw_value_t *
afw_function_execute_to_double_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, integer);

    return afw_value_create_unmanaged_double(
        (afw_double_t)arg->internal,
        x->p, x->xctx);
}



/*
 * Adaptive function: negative<integer>
 *
 * afw_function_execute_negative_integer
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return negative of integer value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function negative<integer>(
 *       value: integer
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   value - (integer)
 *
 * Returns:
 *
 *   (integer)
 */
const afw_value_t *
afw_function_execute_negative_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, integer);

    if (arg->internal == AFW_INTEGER_MIN) {
        AFW_THROW_ERROR_Z(argument_error, "Integer negate overflow", x->xctx);
    }

    return afw_value_create_unmanaged_integer(-arg->internal, x->p, x->xctx);
}



/*
 * Adaptive function: sign<integer>
 *
 * afw_function_execute_sign_integer
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Return the sign of the integer value, as ECMAScript Math.sign does: -1, 0 or
 * 1 of the same data type (`sign<double>` keeps the sign of a zero: sign(-0.0)
 * is -0.0). A NaN argument is an error.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function sign<integer>(
 *       value: integer
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   value - (integer)
 *
 * Returns:
 *
 *   (integer)
 *
 * Errors thrown:
 *
 *   argument_error - the value is NaN
 */
const afw_value_t *
afw_function_execute_sign_integer(
    afw_function_execute_t *x)
{
    const afw_value_integer_t *arg;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg, 1, integer);
    return (arg->internal > 0)
        ? afw_integer_v_one
        : (arg->internal < 0)
        ? afw_value_create_unmanaged_integer(-1, x->p, x->xctx)
        : afw_integer_v_zero;
}
