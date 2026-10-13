// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for rfc822Name
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_rfc822Name.c
 * @brief Adaptive function execute implementations for category `rfc822Name`.
 */

#include "afw_internal.h"



/*
 * Adaptive function: rfc822Name_match
 *
 * afw_function_execute_rfc822Name_match
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Returns true if value matches pattern, a complete or partial rfc822Name.
 * 
 * If pattern is a complete mailbox, such as "Anderson@sun.com", it matches that
 * mailbox: the local part exactly and the domain part ignoring case.
 * 
 * If pattern is a domain, such as "sun.com", it matches every mailbox at
 * exactly that domain (ignoring case), not at its subdomains.
 * 
 * If pattern is a domain that starts with ".", such as ".east.sun.com", it
 * matches every mailbox in a subdomain of it, such as
 * "Anderson@a.east.sun.com", but not at "east.sun.com" itself.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function rfc822Name_match(
 *       pattern: string,
 *       value: rfc822Name
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   pattern - (string) A mailbox, a domain, or a domain that starts with ".".
 *
 *   value - (rfc822Name) The rfc822Name to check.
 *
 * Returns:
 *
 *   (boolean) True if value matches pattern.
 */
const afw_value_t *
afw_function_execute_rfc822Name_match(
    afw_function_execute_t *x)
{
    const afw_value_string_t *pattern;
    const afw_value_rfc822Name_t *value;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(pattern, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(value, 2, rfc822Name);

    return afw_data_type_rfc822Name_match(
        &pattern->internal, &value->internal, x->xctx)
        ? afw_boolean_v_true
        : afw_boolean_v_false;
}
