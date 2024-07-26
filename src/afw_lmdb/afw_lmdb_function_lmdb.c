// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_lmdb_function_execute_* functions for lmdb
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_lmdb_function_lmdb.c
 * @brief afw_lmdb_function_execute_* functions for lmdb.
 */

#include "afw.h"
#include "afw_adapter.h"

#include "afw_lmdb_internal.h"

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
 *       adapterId: string
 *   ): integer;
 * ```
 *
 * Parameters:
 *
 *   adapterId - (string) Adapter Id.
 *
 * Returns:
 *
 *   (integer) Number of stale slots that were cleared.
 */
const afw_value_t *
afw_lmdb_function_execute_reader_check(
    afw_function_execute_t *x)
{
    const afw_value_string_t *adapterId;
    const afw_adapter_session_t     *session;
    int                             deadReaders;
    int                             rc;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adapterId, 1, string);

    session = afw_adapter_session_get_cached(&adapterId->internal, false,
        x->xctx);

    rc = afw_lmdb_internal_reader_check(session->adapter, &deadReaders,
        x->xctx);
    if (rc) {
        AFW_THROW_ERROR_Z(general, "Error in reader_check()", x->xctx);
    }

    return afw_value_create_unmanaged_integer(deadReaders, x->p, x->xctx);
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
 *       adapterId: string
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   adapterId - (string) Adapter Id.
 *
 * Returns:
 *
 *   (string) Reader entries.
 */
const afw_value_t *
afw_lmdb_function_execute_reader_list(
    afw_function_execute_t *x)
{
    const afw_value_string_t *adapterId;
    const afw_adapter_session_t     *session;
    const afw_utf8_t                *list;
    int                             rc;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(adapterId, 1, string);

    session = afw_adapter_session_get_cached(&adapterId->internal, false,
        x->xctx);

    rc = afw_lmdb_internal_reader_list(session->adapter, &list, x->p, x->xctx);
    if (rc < 0) {
        AFW_THROW_ERROR_Z(general, "Error in reader_list()", x->xctx);
    }

    return afw_value_create_unmanaged_string(list, x->p, x->xctx);
}
