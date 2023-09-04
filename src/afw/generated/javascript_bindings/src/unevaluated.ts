// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for unevaluated
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Checks whether value is dataType unevaluated and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsUnevaluated(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<unevaluated>";
    _action["value"] = value;

    return client.perform(_action);
}

