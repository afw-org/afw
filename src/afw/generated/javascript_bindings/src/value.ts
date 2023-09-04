// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for value
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * This function returns an object containing meta information for an adaptive
 * value.
 * 
 * @param {} value - This is the adaptive value whose meta object is to be
 *     returned.
 * 
 * @returns {object} An object containing the meta information for the
 *     adaptive value. The objectType of this object is normally
 *     _AdaptiveValueMeta_ or one of its descendants.
 */
export function afwMeta(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "meta";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * This function returns an array of the result of calling meta() for each
 * value in an array or object adaptive value.
 * 
 * @param {} value - This is an adaptive array or object.
 * 
 * @returns {object} An array containing the result of calling meta() of each
 *     value in array or object adaptive value.
 */
export function afwMetas(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "metas";
    _action["value"] = value;

    return client.perform(_action);
}

