// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_lmdb_function_execute_* functions for lmdb
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_lmdb_function_lmdb.c
 * @brief afw_lmdb_function_execute_* functions for lmdb.
 */

#include "afw.h"



/*
 * Adaptive function: reader_check
 *
 * afw_lmdb_function_execute_reader_check
 *
 * See afw_lmdb_function_bindings.h for more information.
 *
 * Check for stale entries in the reader lock table.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function reader_check(
 *       adaptorId: string
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   adaptorId - (string) Adaptor Id.
 *
 * Returns:
 *
 *   (integer) Number of stale slots that were cleared.
 */
const afw_value_t *
afw_lmdb_function_execute_reader_check(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: reader_list
 *
 * afw_lmdb_function_execute_reader_list
 *
 * See afw_lmdb_function_bindings.h for more information.
 *
 * Dump the entries in the reader lock table.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function reader_list(
 *       adaptorId: string
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   adaptorId - (string) Adaptor Id.
 *
 * Returns:
 *
 *   (string) Reader entries.
 */
const afw_value_t *
afw_lmdb_function_execute_reader_list(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
