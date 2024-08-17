// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_curl_function_execute_* functions for curl
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_curl_function_curl.c
 * @brief afw_curl_function_execute_* functions for curl.
 */

#include <curl/curl.h>
#include "afw.h"

#include "afw_curl_strings.h"

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
    afw_xctx_t *xctx = x->xctx;
    curl_version_info_data *version_info;
    const afw_object_t *object;

    version_info = curl_version_info(CURLVERSION_NOW);
    if (version_info) {
        object = afw_object_create(x->p, xctx);

        afw_object_meta_set_object_type_id(
            object, afw_curl_s__AdaptiveCurlVersionInfo_, xctx);

        afw_object_set_property_as_string_from_utf8_z(object, 
            afw_curl_s_version, version_info->version, xctx);
        afw_object_set_property_as_integer(object, 
            afw_curl_s_version_num, version_info->version_num, xctx);
        afw_object_set_property_as_string_from_utf8_z(object, 
            afw_curl_s_host, version_info->host, xctx);
        afw_object_set_property_as_string_from_utf8_z(object, 
            afw_curl_s_ssl_version, version_info->ssl_version, xctx);

        afw_object_set_property_as_integer(object, 
            afw_curl_s_ssl_version_num, version_info->ssl_version_num, xctx);

        if (version_info->libz_version)
            afw_object_set_property_as_string_from_utf8_z(object, 
                afw_curl_s_libz_version, version_info->libz_version, xctx);

        if (version_info->libssh_version)
            afw_object_set_property_as_string_from_utf8_z(object, 
                afw_curl_s_libssh_version, version_info->libssh_version, xctx);

        /* collect features */


        /* collect protocols */

        return afw_value_create_unmanaged_object(object, x->p, xctx);
    } else {
        AFW_THROW_ERROR_Z(general, "Error getting version information.", xctx);
    }
}
