// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for lmdb
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Check for stale entries in the reader lock table.
 * 
 * @param {string} adaptorId - Adaptor Id.
 * 
 * @returns {integer} Number of stale slots that were cleared.
 */
export function afwReaderCheck(client : any, adaptorId? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "reader_check";

    if (adaptorId !== undefined)
        _action["adaptorId"] = adaptorId;

    return client.perform(_action);
}

/**
 * Dump the entries in the reader lock table.
 * 
 * @param {string} adaptorId - Adaptor Id.
 * 
 * @returns {string} Reader entries.
 */
export function afwReaderList(client : any, adaptorId? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "reader_list";

    if (adaptorId !== undefined)
        _action["adaptorId"] = adaptorId;

    return client.perform(_action);
}

