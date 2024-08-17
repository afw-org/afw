// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for curl
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file curl.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the curl category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Returns run-time libcurl version info.
 * 
 * @returns {object} Returns an object describing the cURL version
 *     information.
 */
export function afwCurlVersionInfo(client : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "curl_version_info";

    return client.perform(_action);
}

