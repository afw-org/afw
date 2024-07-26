// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Core Adaptive Function Bindings
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/*  ----------------------------- N O T E -------------------------------------
 *
 * This file is generated by "afwdev generate afw_lmdb".
 *
 * Do not change this file directly or the changes will be lost the next time
 * this file is regenerated.
 *
 * -------------------------------------------------------------------------*/


#ifndef __AFW_LMDB_FUNCTION_BINDINGS_H__
#define __AFW_LMDB_FUNCTION_BINDINGS_H__


/**
 * @file afw_lmdb_function_bindings.h
 * @brief Adaptive Framework core adaptive function bindings header.
 */

#include "afw_lmdb_declare_helpers.h"

/**
 * @addtogroup afw_c_api_public
 * @{
 *
 */

/**
 * @addtogroup afw_c_api_functions Adaptive functions
 *
 * @{
 */

/**
 * @brief Get array of pointers to afw_lmdb_function bindings.
 * @return pointer to array of function value pointers.
 */
AFW_LMDB_DECLARE(const afw_value_function_definition_t **)
afw_lmdb_function_bindings_get();


/** @addtogroup afw_functions_lmdb lmdb functions
 *
 * lmdb adaptive functions.
 *
 * @{
 */

/**
 * @brief Adaptive Function `reader_check`
 * @param x function execute parameter.
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
    afw_function_execute_t *x);

/**
 * @brief Adaptive Function `reader_list`
 * @param x function execute parameter.
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
    afw_function_execute_t *x);

/** @} */

/** @} */
/** @} */


#endif /* __AFW_LMDB_FUNCTION_BINDINGS_H__ */
