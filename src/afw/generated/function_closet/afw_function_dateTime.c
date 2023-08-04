// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for dateTime
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_dateTime.c
 * @brief afw_function_execute_* functions for dateTime.
 */

#include "afw.h"



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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
