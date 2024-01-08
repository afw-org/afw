// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for random
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file random.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the random category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * This returns a specified number of random octets as dataType base64Binary.
 * 
 * @param {integer} numberOfOctets - The number of random octets to generate.
 * 
 * @returns {base64Binary}
 */
export function afwRandomBase64Binary(client : any, numberOfOctets : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "random_base64Binary";
    _action["numberOfOctets"] = numberOfOctets;

    return client.perform(_action);
}

/**
 * Generate a string containing a specified number of random decimal digits.
 * 
 * @param {integer} numberOfDigits - The number (1-18) of digits to generate.
 * 
 * @returns {string}
 */
export function afwRandomDigits(client : any, numberOfDigits : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "random_digits";
    _action["numberOfDigits"] = numberOfDigits;

    return client.perform(_action);
}

/**
 * This returns a specified number of random octets as dataType hexBinary.
 * 
 * @param {integer} numberOfOctets - The number of random octets to generate.
 * 
 * @returns {hexBinary}
 */
export function afwRandomHexBinary(client : any, numberOfOctets : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "random_hexBinary";
    _action["numberOfOctets"] = numberOfOctets;

    return client.perform(_action);
}

/**
 * This returns a random integer between specified values inclusive.
 * 
 * @param {integer} min - Minimum integer inclusive.
 * 
 * @param {integer} max - Maximum integer inclusive.
 * 
 * @returns {integer} A random integer.
 */
export function afwRandomInteger(client : any, min : number, max : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "random_integer";
    _action["min"] = min;
    _action["max"] = max;

    return client.perform(_action);
}

/**
 * This returns a random double between specified values
 * 
 * @param {double} min - Minimum double inclusive. The default is 0.0.
 * 
 * @param {double} max - Maximum double exclusive. The default is 1.0.
 * 
 * @returns {double}
 */
export function afwRandomNumber(client : any, min? : number, max? : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "random_number";

    if (min !== undefined)
        _action["min"] = min;

    if (max !== undefined)
        _action["max"] = max;

    return client.perform(_action);
}

