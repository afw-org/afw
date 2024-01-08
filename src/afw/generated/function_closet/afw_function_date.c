// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for date
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_date.c
 * @brief afw_function_execute_* functions for date.
 */

#include "afw.h"



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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
