// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for xpathExpression
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * This returns the integer number of values in list.
 * 
 * @param {array} value -
 * 
 * @returns {integer}
 */
export function afwBagSizeXpathExpression(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<xpathExpression>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Takes any number of xpathExpression values and returns a list of list.
 * 
 * @param {array} values -
 * 
 * @returns {array}
 */
export function afwBagXpathExpression(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<xpathExpression>";

    if (values !== undefined)
        _action["values"] = values;

    return client.perform(_action);
}

/**
 * Compile xpathExpression value and return either an unevaluated adaptive
 * value or a string containing the compiler listing.
 * 
 * @param {xpathExpression} source - xpathExpression string to compile
 * 
 * @param {} listing - If specified, a compiler listing is produced instead
 *     of an unevaluated expression value.
 *     
 *     This parameter can be an integer between 0 and 10 of a string that is
 *     used for indentation. If 0 is specified, no whitespace is added to the
 *     resulting string. If 1 through 10 is specified, that number of spaces
 *     is used.
 * 
 * @returns {unevaluated}
 */
export function afwCompileXpathExpression(client : any, source : any, listing? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "compile<xpathExpression>";
    _action["source"] = source;

    if (listing !== undefined)
        _action["listing"] = listing;

    return client.perform(_action);
}

/**
 * Determine if xpathExpression arg1 is equal to the value of arg2 converted
 * to the data type of arg1 then return the boolean result. Use 'eqx' ('===')
 * instead if you want false to be returned if arg1 and arg2's data type
 * don't match.
 * 
 * @param {xpathExpression} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqXpathExpression(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<xpathExpression>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for xpathExpression arg1 is equal to the value and data type
 * of arg2 then return the boolean result. Use 'eq' ('==') instead if you
 * want arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {xpathExpression} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxXpathExpression(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<xpathExpression>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Compile and evaluate xpathExpression value.
 * 
 * @param {xpathExpression} source - xpathExpression string to compile and
 *     evaluate
 * 
 * @param {object} additionalUntrustedQualifiedVariables - This parameter
 *     supplies additional qualified variables that can be accessed during
 *     evaluation. These variables will not be used by anything that needs to
 *     ensure its qualified variables must come from a trusted source, such
 *     as authorization. This parameter is intended to be used for testing
 *     only and should not be used for anything running in production.
 * 
 * @returns {unevaluated}
 */
export function afwEvaluateXpathExpression(client : any, source : any, additionalUntrustedQualifiedVariables? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "evaluate<xpathExpression>";
    _action["source"] = source;

    if (additionalUntrustedQualifiedVariables !== undefined)
        _action["additionalUntrustedQualifiedVariables"] = additionalUntrustedQualifiedVariables;

    return client.perform(_action);
}

/**
 * Checks for xpathExpression arg1 is greater than or equal to
 * xpathExpression arg2 and return the boolean result.
 * 
 * @param {xpathExpression} arg1 -
 * 
 * @param {xpathExpression} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeXpathExpression(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<xpathExpression>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for xpathExpression arg1 is greater than xpathExpression arg2 and
 * return the boolean result.
 * 
 * @param {xpathExpression} arg1 -
 * 
 * @param {xpathExpression} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtXpathExpression(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<xpathExpression>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType xpathExpression and return the boolean
 * result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsXpathExpression(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<xpathExpression>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for xpathExpression arg1 is less than or equal to xpathExpression
 * arg2 and return the boolean result.
 * 
 * @param {xpathExpression} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeXpathExpression(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<xpathExpression>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for xpathExpression arg1 is less that xpathExpression arg2 and
 * return the boolean result.
 * 
 * @param {xpathExpression} arg1 -
 * 
 * @param {xpathExpression} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtXpathExpression(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<xpathExpression>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if xpathExpression arg1 is not equal to the value of arg2
 * converted to the data type of arg1 then return the boolean result. Use
 * 'nex' ('!==') instead if you want true to be returned if arg1 and arg2's
 * data type don't match.
 * 
 * @param {xpathExpression} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeXpathExpression(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<xpathExpression>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for xpathExpression arg1 is not equal to the value or data
 * type of arg2 then return the boolean result. Use 'ne' ('!=') instead if
 * you want arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {xpathExpression} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexXpathExpression(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<xpathExpression>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Converts xpathExpression value to string. For list values, the to_string()
 * value for each entry is returned separated with commas.
 * 
 * @param {xpathExpression} value - A xpathExpression value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringXpathExpression(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<xpathExpression>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Converts value to data type xpathExpression returning xpathExpression
 * result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {xpathExpression} Converted value
 */
export function afwXpathExpression(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "xpathExpression";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * The number of nodes in a node-set.
 * 
 * @param {xpathExpression} nodeset -
 * 
 * @returns {integer}
 */
export function afwXpathNodeCount(client : any, nodeset : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "xpath_node_count";
    _action["nodeset"] = nodeset;

    return client.perform(_action);
}

/**
 * Checks for xpathExpression arg1 is equal to xpathExpression arg2 and
 * return the boolean result.
 * 
 * @param {xpathExpression} arg1 -
 * 
 * @param {xpathExpression} arg2 -
 * 
 * @returns {boolean}
 */
export function afwXpathNodeEq(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "xpath_node_eq";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Returns true if any of the nodes matched by nodeset1 are equal to any of
 * the nodes matched by nodeset2 or their corresponding children.
 * 
 * @param {xpathExpression} nodeset2 -
 * 
 * @param {xpathExpression} arg2 -
 * 
 * @returns {boolean}
 */
export function afwXpathNodeMatch(client : any, nodeset2 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "xpath_node_match";
    _action["nodeset2"] = nodeset2;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

