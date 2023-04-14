// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for x500Name
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Returns boolean true if at least one value in x500Name list1 is in
 * x500Name list2.
 * 
 * @param {list} list1 - The first list.
 * 
 * @param {list} list2 - The second list.
 * 
 * @returns {boolean}
 */
export function afwAtLeastOneMemberOfX500Name(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "at_least_one_member_of<x500Name>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * This returns the integer number of values in list.
 * 
 * @param {list} value -
 * 
 * @returns {integer}
 */
export function afwBagSizeX500Name(client : any, value : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag_size<x500Name>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Takes any number of x500Name values and returns a list of list.
 * 
 * @param {list} values -
 * 
 * @returns {list}
 */
export function afwBagX500Name(client : any, values? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "bag<x500Name>";

    if (values !== undefined)
        _action["values"] = values;

    return client.perform(_action);
}

/**
 * Determine if x500Name arg1 is equal to the value of arg2 converted to the
 * data type of arg1 then return the boolean result. Use "eqx" ("===")
 * instead if you want false to be returned if arg1 and arg2's data type
 * don't match.
 * 
 * @param {x500Name} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqX500Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eq<x500Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for x500Name arg1 is equal to the value and data type of arg2
 * then return the boolean result. Use "eq" ("==") instead if you want arg2
 * to be converted to the data type of arg1 before comparison.
 * 
 * @param {x500Name} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwEqxX500Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "eqx<x500Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for x500Name arg1 is greater than or equal to x500Name arg2 and
 * return the boolean result.
 * 
 * @param {x500Name} arg1 -
 * 
 * @param {x500Name} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGeX500Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ge<x500Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for x500Name arg1 is greater than x500Name arg2 and return the
 * boolean result.
 * 
 * @param {x500Name} arg1 -
 * 
 * @param {x500Name} arg2 -
 * 
 * @returns {boolean}
 */
export function afwGtX500Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "gt<x500Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Returns a list of x500Name with the values that are common to both list of
 * x500Name list1 and list2.
 * 
 * @param {list} list1 - The first list.
 * 
 * @param {list} list2 - The second list.
 * 
 * @returns {list}
 */
export function afwIntersectionX500Name(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "intersection<x500Name>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Checks whether x500Name value is in list of x500Name list and returns the
 * boolean result.
 * 
 * @param {x500Name} value -
 * 
 * @param {list} list -
 * 
 * @returns {boolean}
 */
export function afwIsInX500Name(client : any, value : any, list : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is_in<x500Name>";
    _action["value"] = value;
    _action["list"] = list;

    return client.perform(_action);
}

/**
 * Checks whether value is dataType x500Name and return the boolean result.
 * 
 * @param {} value - Value to check
 * 
 * @returns {boolean}
 */
export function afwIsX500Name(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "is<x500Name>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Checks for x500Name arg1 is less than or equal to x500Name arg2 and return
 * the boolean result.
 * 
 * @param {x500Name} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLeX500Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "le<x500Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Checks for x500Name arg1 is less that x500Name arg2 and return the boolean
 * result.
 * 
 * @param {x500Name} arg1 -
 * 
 * @param {x500Name} arg2 -
 * 
 * @returns {boolean}
 */
export function afwLtX500Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "lt<x500Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Returns true if arg1 matches some terminal sequence of RDNs from arg2 when
 * compared using equal_x500Name.
 * 
 * @param {x500Name} arg1 -
 * 
 * @param {string} arg2 -
 * 
 * @returns {boolean}
 */
export function afwMatchX500Name(client : any, arg1 : any, arg2 : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "match<x500Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if x500Name arg1 is not equal to the value of arg2 converted to
 * the data type of arg1 then return the boolean result. Use "nex" ("!==")
 * instead if you want true to be returned if arg1 and arg2's data type don't
 * match.
 * 
 * @param {x500Name} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNeX500Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "ne<x500Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * Determine if for x500Name arg1 is not equal to the value or data type of
 * arg2 then return the boolean result. Use "ne" ("!=") instead if you want
 * arg2 to be converted to the data type of arg1 before comparison.
 * 
 * @param {x500Name} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {boolean}
 */
export function afwNexX500Name(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nex<x500Name>";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This converts a list of x500Name values that contains one value to a
 * single x500Name value.
 * 
 * @param {list} list -
 * 
 * @returns {x500Name}
 */
export function afwOneAndOnlyX500Name(client : any, list : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "one_and_only<x500Name>";
    _action["list"] = list;

    return client.perform(_action);
}

/**
 * Checks whether x500Name value matches the regular expression regexp and
 * return the boolean result.
 * 
 * @param {x500Name} value -
 * 
 * @param {string} regexp -
 * 
 * @returns {boolean}
 */
export function afwRegexpMatchX500Name(client : any, value : any, regexp : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "regexp_match<x500Name>";
    _action["value"] = value;
    _action["regexp"] = regexp;

    return client.perform(_action);
}

/**
 * Returns boolean true if x500Name list1 and x500Name list2 are subsets of
 * each other and return the boolean result.
 * 
 * @param {list} list1 -
 * 
 * @param {list} list2 -
 * 
 * @returns {boolean}
 */
export function afwSetEqualsX500Name(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "set_equals<x500Name>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Returns boolean true if the unique values in x500Name list1 are all in
 * x500Name list2.
 * 
 * @param {list} list1 - The first list.
 * 
 * @param {list} list2 - The second list.
 * 
 * @returns {boolean}
 */
export function afwSubsetX500Name(client : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "subset<x500Name>";
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Converts x500Name value to string. For list values, the to_string() value
 * for each entry is returned separated with commas.
 * 
 * @param {x500Name} value - A x500Name value.
 * 
 * @returns {string} The string representation of the value.
 */
export function afwToStringX500Name(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_string<x500Name>";
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * Returns a list of x500Name contains all of the unique values in two or
 * more list of x500Name values.
 * 
 * @param {list} lists - Two or more lists.
 * 
 * @returns {list}
 */
export function afwUnionX500Name(client : any, lists : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "union<x500Name>";
    _action["lists"] = lists;

    return client.perform(_action);
}

/**
 * Converts value to data type x500Name returning x500Name result.
 * 
 * @param {} value - Value to convert
 * 
 * @returns {x500Name} Converted value
 */
export function afwX500Name(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "x500Name";
    _action["value"] = value;

    return client.perform(_action);
}

