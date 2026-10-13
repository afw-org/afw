// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for x500Name
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_x500Name.c
 * @brief Adaptive function execute implementations for category `x500Name`.
 */

#include "afw_internal.h"



/*
 * Adaptive function: x500Name_match
 *
 * afw_function_execute_x500Name_match
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Returns true if the RDNs of pattern are the last RDNs of value, compared as
 * x500Name equality compares them. For example, "O=Medico Corp,C=US"x500Name
 * matches "cn=John Smith,o=Medico Corp, c=US"x500Name.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function x500Name_match(
 *       pattern: x500Name,
 *       value: x500Name
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   pattern - (x500Name) The RDNs value must end with.
 *
 *   value - (x500Name) The x500Name to check.
 *
 * Returns:
 *
 *   (boolean) True if value ends with the RDNs of pattern.
 */
const afw_value_t *
afw_function_execute_x500Name_match(
    afw_function_execute_t *x)
{
    const afw_value_x500Name_t *pattern;
    const afw_value_x500Name_t *value;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(pattern, 1, x500Name);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(value, 2, x500Name);

    return afw_data_type_x500Name_match(
        &pattern->internal, &value->internal, x->xctx)
        ? afw_boolean_v_true
        : afw_boolean_v_false;
}
