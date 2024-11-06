// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for http
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file http.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the http category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Makes a HTTP DELETE Request.
 * 
 * @param {string} url - HTTP Url.
 * 
 * @param {array} headers - HTTP Headers.
 * 
 * @param {object} options - CURL Options
 * 
 * @returns {object} Returns an object describing the response from the HTTP
 *     delete request.
 */
export function afwHttpDelete(client : any, url : string, headers? : any[], options? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "http_delete";
    _action["url"] = url;

    if (headers !== undefined)
        _action["headers"] = headers;

    if (options !== undefined)
        _action["options"] = options;

    return client.perform(_action);
}

/**
 * Makes a HTTP GET Request.
 * 
 * @param {string} url - HTTP Url.
 * 
 * @param {array} headers - HTTP Headers.
 * 
 * @param {object} options - CURL Options
 * 
 * @returns {object} Returns an object describing the response from the HTTP
 *     GET request.
 */
export function afwHttpGet(client : any, url : string, headers? : any[], options? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "http_get";
    _action["url"] = url;

    if (headers !== undefined)
        _action["headers"] = headers;

    if (options !== undefined)
        _action["options"] = options;

    return client.perform(_action);
}

/**
 * Makes a HTTP PATCH Request.
 * 
 * @param {string} url - HTTP Url.
 * 
 * @param {string} payload - Data payload for PATCH.
 * 
 * @param {array} headers - HTTP Headers.
 * 
 * @param {object} options - CURL Options
 * 
 * @returns {object} Returns an object describing the response from the HTTP
 *     PATCH request.
 */
export function afwHttpPatch(client : any, url : string, payload? : string, headers? : any[], options? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "http_patch";
    _action["url"] = url;

    if (payload !== undefined)
        _action["payload"] = payload;

    if (headers !== undefined)
        _action["headers"] = headers;

    if (options !== undefined)
        _action["options"] = options;

    return client.perform(_action);
}

/**
 * Makes a HTTP POST Request.
 * 
 * @param {string} url - HTTP Url.
 * 
 * @param {string} payload - Data payload to POST to the url.
 * 
 * @param {array} headers - HTTP Headers.
 * 
 * @param {object} options - CURL Options
 * 
 * @returns {object} Returns an object describing the response from the HTTP
 *     POST request.
 */
export function afwHttpPost(client : any, url : string, payload? : string, headers? : any[], options? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "http_post";
    _action["url"] = url;

    if (payload !== undefined)
        _action["payload"] = payload;

    if (headers !== undefined)
        _action["headers"] = headers;

    if (options !== undefined)
        _action["options"] = options;

    return client.perform(_action);
}

/**
 * Makes a HTTP PUT Request.
 * 
 * @param {string} url - HTTP Url.
 * 
 * @param {string} payload - Data payload for PUT.
 * 
 * @param {array} headers - HTTP Headers.
 * 
 * @param {object} options - CURL Options
 * 
 * @returns {object} Returns an object describing the response from the HTTP
 *     PUT request.
 */
export function afwHttpPut(client : any, url : string, payload? : string, headers? : any[], options? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "http_put";
    _action["url"] = url;

    if (payload !== undefined)
        _action["payload"] = payload;

    if (headers !== undefined)
        _action["headers"] = headers;

    if (options !== undefined)
        _action["options"] = options;

    return client.perform(_action);
}

