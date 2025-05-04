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
 * @param {function} headerCallback - The optional callback function to read
 *     the headers.
 * 
 * @param {} headerUserData - The user data to pass to the header callback
 *     function.
 * 
 * @param {function} bodyCallback - The optional callback function to read the
 *     body.
 * 
 * @param {} bodyUserData - The user data to pass to the body callback
 *     function.
 * 
 * @returns {object} Returns an object describing the response from the HTTP
 *     GET request.
 */
export function afwHttpGet(client : any, url : string, headers? : any[], options? : object, headerCallback? : any, headerUserData? : any, bodyCallback? : any, bodyUserData? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "http_get";
    _action["url"] = url;

    if (headers !== undefined)
        _action["headers"] = headers;

    if (options !== undefined)
        _action["options"] = options;

    if (headerCallback !== undefined)
        _action["headerCallback"] = headerCallback;

    if (headerUserData !== undefined)
        _action["headerUserData"] = headerUserData;

    if (bodyCallback !== undefined)
        _action["bodyCallback"] = bodyCallback;

    if (bodyUserData !== undefined)
        _action["bodyUserData"] = bodyUserData;

    return client.perform(_action);
}

/**
 * Makes a HTTP HEAD Request.
 * 
 * @param {string} url - HTTP Url.
 * 
 * @param {array} headers - HTTP Headers.
 * 
 * @param {object} options - CURL Options
 * 
 * @returns {object} Returns an object describing the response from the HTTP
 *     head request.
 */
export function afwHttpHead(client : any, url : string, headers? : any[], options? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "http_head";
    _action["url"] = url;

    if (headers !== undefined)
        _action["headers"] = headers;

    if (options !== undefined)
        _action["options"] = options;

    return client.perform(_action);
}

/**
 * Makes a HTTP OPTIONS Request.
 * 
 * @param {string} url - HTTP Url.
 * 
 * @param {array} headers - HTTP Headers.
 * 
 * @param {object} options - CURL Options
 * 
 * @returns {object} Returns an object describing the response from the HTTP
 *     OPTIONS request.
 */
export function afwHttpOptions(client : any, url : string, headers? : any[], options? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "http_options";
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

