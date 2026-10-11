// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for indexes
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_indexes.c
 * @brief afw_function_execute_* functions for indexes.
 */

#include "afw.h"



/*
 * Adaptive function: index_create
 *
 * afw_function_execute_index_create
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Create an index definition.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function index_create(
 *       adapterId: string,
 *       key: string,
 *       value?: string,
 *       objectType?: string[],
 *       filter?: string,
 *       options?: string[],
 *       retroactive?: boolean,
 *       test?: boolean
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   adapterId - (string) Id of adapter.
 *
 *   key - (string) Name queries use. Without a value script, the property it
 *       indexes. With a value or filter script, a computed name: a query on it
 *       means what the scripts give, so it must be a name objects don't have
 *       (an object type in objectType must not declare it).
 *
 *   value - (optional string) Adaptive script (expression-like; must return a
 *       value) used to calculate the index value(s). While evaluating,
 *       current::object, current::objectId, current::objectType, and
 *       current::key are available (issue #54). If omitted, the property named
 *       by key is indexed. A property read works as is (current::object.given);
 *       an operator or an array literal needs return (return
 *       current::object.department == "ENG";).
 *
 *   objectType - (optional string[]) Object type id(s) this index may apply to.
 *
 *   filter - (optional string) Adaptive script that must return a boolean to
 *       decide whether this index applies to a particular object. Uses the same
 *       current:: variables as value (issue #54). If omitted, the filter is
 *       always true. A filter needs a value script: an object the filter leaves
 *       out has no value under the computed name.
 *
 *   options - (optional string[]) Indexing options: unique (each value at most
 *       once), case-insensitive-string (compare strings case-insensitively),
 *       sort-reverse, and integer or double (the index's values are numbers, so
 *       a string query value such as a query string's is the number's key when
 *       no object type gives the property's data type; it must agree with a
 *       data type an object type declares).
 *
 *   retroactive - (optional boolean) Retroactively generate indexes for
 *       existing objects.
 *
 *   test - (optional boolean) Test create (don't actually perform).
 *
 * Returns:
 *
 *   (object) Object response from the indexing process.
 */
const afw_value_t *
afw_function_execute_index_create(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: index_list
 *
 * afw_function_execute_index_list
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * List property indexes
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function index_list(
 *       adapterId: string,
 *       objectType?: string
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   adapterId - (string) Id of adapter.
 *
 *   objectType - (optional string) Id of adaptive object type indexes.
 *
 * Returns:
 *
 *   (object) Object response from the index repair process.
 */
const afw_value_t *
afw_function_execute_index_list(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: index_remove
 *
 * afw_function_execute_index_remove
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Remove an index definition.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * This function requires 'execute' access.
 *
 * Declaration:
 *
 * ```
 *   function index_remove(
 *       adapterId: string,
 *       key: string
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   adapterId - (string) Id of adapter.
 *
 *   key - (string) The index key to be removed.
 *
 * Returns:
 *
 *   (object) Object response from the indexing process.
 */
const afw_value_t *
afw_function_execute_index_remove(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
