// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for x500Name
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_x500Name.c
 * @brief afw_function_execute_* functions for x500Name.
 */

#include "afw_internal.h"



/*
 * Adaptive function: match<x500Name>
 *
 * afw_function_execute_match_x500Name
 *
 * See afw_function_bindings.h for more information.
 *
 * Returns true if arg1 matches some terminal sequence of RDNs from arg2 when
 * compared using equal_x500Name.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function match<x500Name>(
 *       arg1: x500Name,
 *       arg2: string
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   arg1 - (x500Name)
 *
 *   arg2 - (string)
 *
 * Returns:
 *
 *   (boolean)
 */
const afw_value_t *
afw_function_execute_match_x500Name(
    afw_function_execute_t *x)
{
    /** @fixme Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
