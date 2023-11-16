// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for json
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file json.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the json category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Compile json value and return either an unevaluated adaptive value or a
 * string containing the compiler listing.
 * 
 * @param {json} source - json string to compile
 * 
 * @param {} listing - If specified, a compiler listing is produced instead of
 *     an unevaluated compiled value.
 * 
 *     This parameter can be an integer between 0 and 10 of a string that is
 *     used for indentation. If 0 is specified, no whitespace is added to the
 *     resulting string. If 1 through 10 is specified, that number of spaces
 *     is used.
 * 
 * @returns {unevaluated}
 */
export function afwCompileJson(client : any, source : any, listing? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "compile<json>";
    _action["source"] = source;

    if (listing !== undefined)
        _action["listing"] = listing;

    return client.perform(_action);
}

/**
 * Converts value to data type json returning json result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {json} Converted value
 */
export function afwJson(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "json";
    _action["value"] = value;

    return client.perform(_action);
}

