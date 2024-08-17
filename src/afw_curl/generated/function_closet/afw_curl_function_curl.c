// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_curl_function_execute_* functions for curl
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_curl_function_curl.c
 * @brief afw_curl_function_execute_* functions for curl.
 */

#include "afw.h"



/*
 * Adaptive function: curl_version_info
 *
 * afw_curl_function_execute_curl_version_info
 *
 * See afw_curl_function_bindings.h for more information.
 *
 * Returns run-time libcurl version info.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function curl_version_info(
 *   
 *   ): (object _AdaptiveCurlVersionInfo_);
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (object _AdaptiveCurlVersionInfo_) Returns an object describing the cURL
 *       version information.
 */
const afw_value_t *
afw_curl_function_execute_curl_version_info(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
