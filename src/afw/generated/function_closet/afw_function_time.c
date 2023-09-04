// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for time
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_time.c
 * @brief afw_function_execute_* functions for time.
 */

#include "afw.h"



/*
 * Adaptive function: in_range<time>
 *
 * afw_function_execute_in_range_time
 *
 * See afw_function_bindings.h for more information.
 *
 * Checks if time is between startTime and endTime, inclusive. Regardless of
 * endTime value, it is always considered to be equal to, but less than 24 hours
 * greater than startTime. If no time zone is specified for time, the default
 * time zone is used. If no time zone is specified for startTime or endTime, the
 * time zone of time is used.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function in_range<time>(
 *       time: time,
 *       startTime: time,
 *       endTime: time
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   time - (time)
 *
 *   startTime - (time)
 *
 *   endTime - (time)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_in_range_time(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
