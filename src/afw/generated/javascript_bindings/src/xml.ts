// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for xml
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Compile xml value and return either an unevaluated adaptive value or a
 * string containing the compiler listing.
 * 
 * @param {xml} source - xml string to compile
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
export function afwCompileXml(client : any, source : any, listing? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "compile<xml>";
    _action["source"] = source;

    if (listing !== undefined)
        _action["listing"] = listing;

    return client.perform(_action);
}

/**
 * Converts value to data type xml returning xml result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {xml} Converted value
 */
export function afwXml(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "xml";
    _action["value"] = value;

    return client.perform(_action);
}

