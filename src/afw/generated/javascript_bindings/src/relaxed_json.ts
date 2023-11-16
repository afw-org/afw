// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for relaxed_json
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


/**
 * @file relaxed_json.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the relaxed_json category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Compile relaxed_json value and return either an unevaluated adaptive value
 * or a string containing the compiler listing.
 * 
 * @param {relaxed_json} source - relaxed_json string to compile
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
export function afwCompileRelaxedJson(client : any, source : any, listing? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "compile<relaxed_json>";
    _action["source"] = source;

    if (listing !== undefined)
        _action["listing"] = listing;

    return client.perform(_action);
}

/**
 * Converts value to data type relaxed_json returning relaxed_json result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {relaxed_json} Converted value
 */
export function afwRelaxedJson(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "relaxed_json";
    _action["value"] = value;

    return client.perform(_action);
}

