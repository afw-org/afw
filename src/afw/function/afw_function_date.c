// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for Date
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_date.c
 * @brief afw_function_execute_* functions for date.
 */

#include "afw_internal.h"



/*
 * Adaptive function: add_yearMonthDuration<date>
 *
 * afw_function_execute_add_yearMonthDuration_date
 *
 * See afw_function_bindings.h for more information.
 *
 * Add date arg1 to yearMonthDuration arg2 and return the date result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function add_yearMonthDuration<date>(
 *       arg1: date,
 *       arg2: yearMonthDuration
 *   ): date;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (date)
 *
 *   arg2 - (yearMonthDuration)
 *
 * Returns:
 *
 *   (date)
 */
const afw_value_t *
afw_function_execute_add_yearMonthDuration_date(
    afw_function_execute_t *x)
{
    const afw_value_date_t *arg1;
    const afw_value_yearMonthDuration_t *arg2;
    afw_value_date_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg1, 1, date);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg2,
        2, yearMonthDuration);

    result = afw_value_allocate_date(x->p, x->xctx);
    result->internal = arg1->internal;
    afw_date_add_yearMonthDuration(&result->internal, &arg2->internal,
        x->xctx);

    return (const afw_value_t *)result;
}



/*
 * Adaptive function: subtract_yearMonthDuration<date>
 *
 * afw_function_execute_subtract_yearMonthDuration_date
 *
 * See afw_function_bindings.h for more information.
 *
 * Subtract yearMonthDuration arg2 from date arg1 and return the date result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function subtract_yearMonthDuration<date>(
 *       arg1: date,
 *       arg2: yearMonthDuration
 *   ): date;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (date)
 *
 *   arg2 - (yearMonthDuration)
 *
 * Returns:
 *
 *   (date)
 */
const afw_value_t *
afw_function_execute_subtract_yearMonthDuration_date(
    afw_function_execute_t *x)
{
    const afw_value_date_t *arg1;
    const afw_value_yearMonthDuration_t *arg2;
    afw_value_date_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg1, 1, date);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg2, 2,
        yearMonthDuration);

    result = afw_value_allocate_date(x->p, x->xctx);
    result->internal = arg1->internal;
    afw_date_subtract_yearMonthDuration(&result->internal, &arg2->internal,
        x->xctx);

    return (const afw_value_t *)result;
}
