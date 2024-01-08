// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for dateTime
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_dateTime.c
 * @brief afw_function_execute_* functions for dateTime.
 */

#include "afw_internal.h"



/*
 * Adaptive function: add_dayTimeDuration<dateTime>
 *
 * afw_function_execute_add_dayTimeDuration_dateTime
 *
 * See afw_function_bindings.h for more information.
 *
 * Add dateTime arg1 to dayTimeDuration arg2 and return the dateTime result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function add_dayTimeDuration<dateTime>(
 *       arg1: dateTime,
 *       arg2: dayTimeDuration
 *   ): dateTime;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (dateTime)
 *
 *   arg2 - (dayTimeDuration)
 *
 * Returns:
 *
 *   (dateTime)
 */
const afw_value_t *
afw_function_execute_add_dayTimeDuration_dateTime(
    afw_function_execute_t *x)
{
    const afw_value_dateTime_t *arg1;
    const afw_value_dayTimeDuration_t *arg2;
    afw_value_dateTime_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg1, 1, dateTime);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg2,
        2, dayTimeDuration);

    result = afw_value_allocate_unmanaged_dateTime(x->p, x->xctx);
    result->internal = arg1->internal;
    afw_dateTime_add_dayTimeDuration(&result->internal, &arg2->internal,
        x->xctx);

    return &result->pub;
}



/*
 * Adaptive function: add_yearMonthDuration<dateTime>
 *
 * afw_function_execute_add_yearMonthDuration_dateTime
 *
 * See afw_function_bindings.h for more information.
 *
 * Add dateTime arg1 to yearMonthDuration arg2 and return the dateTime result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function add_yearMonthDuration<dateTime>(
 *       arg1: dateTime,
 *       arg2: yearMonthDuration
 *   ): dateTime;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (dateTime)
 *
 *   arg2 - (yearMonthDuration)
 *
 * Returns:
 *
 *   (dateTime)
 */
const afw_value_t *
afw_function_execute_add_yearMonthDuration_dateTime(
    afw_function_execute_t *x)
{
    const afw_value_dateTime_t *arg1;
    const afw_value_yearMonthDuration_t *arg2;
    afw_value_dateTime_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg1, 1, dateTime);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg2,
        2, yearMonthDuration);

    result = afw_value_allocate_unmanaged_dateTime(x->p, x->xctx);
    result->internal = arg1->internal;
    afw_dateTime_add_yearMonthDuration(&result->internal, &arg2->internal,
        x->xctx);

    return &result->pub;
}



/*
 * Adaptive function: subtract_dayTimeDuration<dateTime>
 *
 * afw_function_execute_subtract_dayTimeDuration_dateTime
 *
 * See afw_function_bindings.h for more information.
 *
 * Subtract dayTimeDuration arg2 from dateTime arg1 and return the dateTime
 * result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function subtract_dayTimeDuration<dateTime>(
 *       arg1: dateTime,
 *       arg2: dayTimeDuration
 *   ): dateTime;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (dateTime)
 *
 *   arg2 - (dayTimeDuration)
 *
 * Returns:
 *
 *   (dateTime)
 */
const afw_value_t *
afw_function_execute_subtract_dayTimeDuration_dateTime(
    afw_function_execute_t *x)
{
    const afw_value_dateTime_t *arg1;
    const afw_value_dayTimeDuration_t *arg2;
    afw_value_dateTime_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg1, 1, dateTime);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg2,
        2, dayTimeDuration);

    result = afw_value_allocate_unmanaged_dateTime(x->p, x->xctx);
    result->internal = arg1->internal;
    afw_dateTime_subtract_dayTimeDuration(&result->internal, &arg2->internal,
        x->xctx);

    return &result->pub;
}



/*
 * Adaptive function: subtract_yearMonthDuration<dateTime>
 *
 * afw_function_execute_subtract_yearMonthDuration_dateTime
 *
 * See afw_function_bindings.h for more information.
 *
 * Subtract yearMonthDuration arg2 from dateTime arg1 and return the dateTime
 * result.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function subtract_yearMonthDuration<dateTime>(
 *       arg1: dateTime,
 *       arg2: yearMonthDuration
 *   ): dateTime;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (dateTime)
 *
 *   arg2 - (yearMonthDuration)
 *
 * Returns:
 *
 *   (dateTime)
 */
const afw_value_t *
afw_function_execute_subtract_yearMonthDuration_dateTime(
    afw_function_execute_t *x)
{
    const afw_value_dateTime_t *arg1;
    const afw_value_yearMonthDuration_t *arg2;
    afw_value_dateTime_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg1, 1, dateTime);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(arg2,
        2, yearMonthDuration);

    result = afw_value_allocate_unmanaged_dateTime(x->p, x->xctx);
    result->internal = arg1->internal;
    afw_dateTime_subtract_yearMonthDuration(&result->internal, &arg2->internal,
        x->xctx);

    return &result->pub;
}
