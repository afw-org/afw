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
 * See afw_function_bindings.h for more information.
 *
 * Create an index definition.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function index_create(
 *       adapterId: string,
 *       key: string,
 *       value?: string,
 *       objectType?: string,
 *       filter?: string,
 *       options?: string,
 *       retroactive?: boolean,
 *       test?: boolean
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   adapterId - (string) Id of adapter.
 *
 *   key - (string) Name of the property index to be created.
 *
 *   value - (optional string) Expression to calculate the index value(s).
 *
 *   objectType - (optional string) Object Type(s) this index may apply to.
 *
 *   filter - (optional string) Expression to determine if this index applies to
 *       a particular object.
 *
 *   options - (optional string) Indexing options.
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
 * See afw_function_bindings.h for more information.
 *
 * List property indexes
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
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
 * See afw_function_bindings.h for more information.
 *
 * Remove an index definition.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
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
