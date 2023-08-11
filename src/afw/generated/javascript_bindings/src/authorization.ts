// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for authorization
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * This function can be used to check if the current subject has a given
 * access to the specified resource in the current environment.
 * 
 * @param {string} requestId - This is a request id to associate with the
 *     authorization request. Variable current::requestId can be used to
 *     access this value in authorization policies.
 * 
 * @param {string} resourceId - This is the URI of the resource being
 *     checked. Variable current::resourceId can be used to access this value
 *     in authorization policies.
 * 
 * @param {string} actionId - This is the action that on the resource that's
 *     being checked.  Variable current::actionId can be used to access this
 *     value in authorization policies.
 * 
 * @param {object} object - Variable current::object can be used to access
 *     this value in authorization policies.
 * 
 * @param {boolean} enforce - This specifies whether an error is thrown if
 *     access is denied. If false or not specified, an error is not thrown
 *     and the decision can be checked in the returned object.
 * 
 * @returns {object} The authorization result.
 */
export function afwAuthorizationCheck(client : any, requestId : string, resourceId : string, actionId : string, object? : object, enforce? : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "authorization_check";
    _action["requestId"] = requestId;
    _action["resourceId"] = resourceId;
    _action["actionId"] = actionId;

    if (object !== undefined)
        _action["object"] = object;

    if (enforce !== undefined)
        _action["enforce"] = enforce;

    return client.perform(_action);
}

