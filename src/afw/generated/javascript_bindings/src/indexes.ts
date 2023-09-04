// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for indexes
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Create an index definition.
 * 
 * @param {string} adaptorId - Id of adaptor.
 * 
 * @param {string} key - Name of the property index to be created.
 * 
 * @param {string} value - Expression to calculate the index value(s).
 * 
 * @param {string} objectType - Object Type(s) this index may apply to.
 * 
 * @param {string} filter - Expression to determine if this index applies to a
 *     particular object.
 * 
 * @param {string} options - Indexing options.
 * 
 * @param {boolean} retroactive - Retroactively generate indexes for existing
 *     objects.
 * 
 * @param {boolean} test - Test create (don't actually perform).
 * 
 * @returns {object} Object response from the indexing process
 */
export function afwIndexCreate(client : any, key : string, adaptorId? : string, value? : string, objectType? : string, filter? : string, options? : string, retroactive? : boolean, test? : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "index_create";
    _action["key"] = key;

    if (adaptorId !== undefined)
        _action["adaptorId"] = adaptorId;

    if (value !== undefined)
        _action["value"] = value;

    if (objectType !== undefined)
        _action["objectType"] = objectType;

    if (filter !== undefined)
        _action["filter"] = filter;

    if (options !== undefined)
        _action["options"] = options;

    if (retroactive !== undefined)
        _action["retroactive"] = retroactive;

    if (test !== undefined)
        _action["test"] = test;

    return client.perform(_action);
}

/**
 * List property indexes
 * 
 * @param {string} adaptorId - Id of adaptor.
 * 
 * @param {string} objectType - Id of adaptive object type indexes
 * 
 * @returns {object} Object response from the index repair process
 */
export function afwIndexList(client : any, adaptorId? : string, objectType? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "index_list";

    if (adaptorId !== undefined)
        _action["adaptorId"] = adaptorId;

    if (objectType !== undefined)
        _action["objectType"] = objectType;

    return client.perform(_action);
}

/**
 * Remove an index definition.
 * 
 * @param {string} adaptorId - Id of adaptor.
 * 
 * @param {string} key - The index key to be removed.
 * 
 * @returns {object} Object response from the indexing process
 */
export function afwIndexRemove(client : any, key : string, adaptorId? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "index_remove";
    _action["key"] = key;

    if (adaptorId !== undefined)
        _action["adaptorId"] = adaptorId;

    return client.perform(_action);
}

