// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for indexes
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file indexes.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the indexes category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Create an index definition.
 * 
 * @param {string} adapterId - Id of adapter.
 * 
 * @param {string} key - Name of the property index to be created.
 * 
 * @param {string} value - Adaptive script (expression-like; must return a
 *     value) used to calculate the index value(s). While evaluating,
 *     current::object, current::objectId, current::objectType, and
 *     current::key are available (issue #54). If omitted, the property named
 *     by key is indexed.
 * 
 * @param {array} objectType - Object type id(s) this index may apply to.
 * 
 * @param {string} filter - Adaptive script that must return a boolean to
 *     decide whether this index applies to a particular object. Uses the same
 *     current:: variables as value (issue #54). If omitted, the filter is
 *     always true.
 * 
 * @param {array} options - Indexing options.
 * 
 * @param {boolean} retroactive - Retroactively generate indexes for existing
 *     objects.
 * 
 * @param {boolean} test - Test create (don't actually perform).
 * 
 * @returns {object} Object response from the indexing process
 */
export function afwIndexCreate(client : any, key : string, adapterId? : string, value? : string, objectType? : any[], filter? : string, options? : any[], retroactive? : boolean, test? : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "index_create";
    _action["key"] = key;

    if (adapterId !== undefined)
        _action["adapterId"] = adapterId;

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
 * @param {string} adapterId - Id of adapter.
 * 
 * @param {string} objectType - Id of adaptive object type indexes
 * 
 * @returns {object} Object response from the index repair process
 */
export function afwIndexList(client : any, adapterId? : string, objectType? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "index_list";

    if (adapterId !== undefined)
        _action["adapterId"] = adapterId;

    if (objectType !== undefined)
        _action["objectType"] = objectType;

    return client.perform(_action);
}

/**
 * Remove an index definition.
 * 
 * @param {string} adapterId - Id of adapter.
 * 
 * @param {string} key - The index key to be removed.
 * 
 * @returns {object} Object response from the indexing process
 */
export function afwIndexRemove(client : any, key : string, adapterId? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "index_remove";
    _action["key"] = key;

    if (adapterId !== undefined)
        _action["adapterId"] = adapterId;

    return client.perform(_action);
}

