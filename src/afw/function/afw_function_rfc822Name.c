// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for rfc822Name
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_rfc822Name.c
 * @brief afw_function_execute_* functions for rfc822Name.
 */

#include "afw_internal.h"



/*
 * Adaptive function: match<rfc822Name>
 *
 * afw_function_execute_match_rfc822Name
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns true if arg1 matches some terminal sequence of RDNs from arg2 when
 * compared using equal_rfc822Name.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function match<rfc822Name>(
 *       arg1: rfc822Name,
 *       arg2: string
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (rfc822Name)
 *
 *   arg2 - (string)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_match_rfc822Name(
    afw_function_execute_t *x)
{
    /** @fixme Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
