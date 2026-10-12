// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for unevaluated
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file unevaluated.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the unevaluated category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Returns value unchanged if it is dataType unevaluated. Otherwise, including
 * when value is undefined, throws an error. It never converts: to get a
 * unevaluated from a value of another data type, write a typed literal or
 * call a to_ function. This is usually written value as unevaluated.
 * 
 * @param {} value - Value to assert
 * 
 * @returns {unevaluated} value, which is dataType unevaluated
 */
export function afwAsUnevaluated(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "as_unevaluated";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType unevaluated and returns the boolean
 * result. This can also be written `is<unevaluated>(value`).
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsUnevaluated(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_unevaluated";
    _action["value"] = value;

    return client.perform(_action);
}

