// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for higher_order_array
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Returns true if all values in an array pass the predicate test.
 * 
 * @param {function} predicate - This function is called for each value in
 *     the first array in values or until false is returned. If no calls
 *     return false, the result is true.
 * 
 * @param {} values - These are the parameters passed to predicate with the
 *     exception that the first array is passed one value at a time. At least
 *     one array is required.
 * 
 * @returns {boolean}
 */
export function afwAllOf(client : any, predicate : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "all_of";
    _action["predicate"] = predicate;
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Returns true if the result of calling predicate with all of the
 * combination of values from list1 and list2 returns true.
 * 
 * @param {function} predicate - The predicate is passed two parameters, the
 *     first is a value from list1 and the second is a value from list2.
 * 
 * @param {array} list1 -
 * 
 * @param {array} list2 -
 * 
 * @returns {boolean}
 */
export function afwAllOfAll(client : any, predicate : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "all_of_all";
    _action["predicate"] = predicate;
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * This function returns true if the result of calling predicate with all of
 * the combination of values from list1 and any of the values of list2
 * returns true.
 * 
 * @param {function} predicate - The predicate is passed two parameters, the
 *     first is a value from list1 and the second is a value from list2.
 * 
 * @param {array} list1 -
 * 
 * @param {array} list2 -
 * 
 * @returns {boolean}
 */
export function afwAllOfAny(client : any, predicate : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "all_of_any";
    _action["predicate"] = predicate;
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * Returns true if any value in an array pass the predicate test.
 * 
 * @param {function} predicate - This function is called for each value in
 *     the first array in values or until true is returned. If no calls
 *     return true, the result is false.
 * 
 * @param {} values - These are the parameters passed to predicate with the
 *     exception that the first array is passed one value at a time. At least
 *     one array is required.
 * 
 * @returns {boolean}
 */
export function afwAnyOf(client : any, predicate : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "any_of";
    _action["predicate"] = predicate;
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Returns true if the result of calling predicate with all of the
 * combination of values from list2 and any of the values of list1 returns
 * true.
 * 
 * @param {function} predicate - The predicate is passed two parameters, the
 *     first is a value from list1 and the second is a value from list2.
 * 
 * @param {array} list1 -
 * 
 * @param {array} list2 -
 * 
 * @returns {boolean}
 */
export function afwAnyOfAll(client : any, predicate : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "any_of_all";
    _action["predicate"] = predicate;
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * This function returns true if the result of calling predicate with any of
 * the combination of values from list1 and list2 returns true.
 * 
 * @param {function} predicate - The predicate is passed two parameters, the
 *     first is a value from list1 and the second is a value from list2.
 * 
 * @param {array} list1 -
 * 
 * @param {array} list2 -
 * 
 * @returns {boolean}
 */
export function afwAnyOfAny(client : any, predicate : any, list1 : any[], list2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "any_of_any";
    _action["predicate"] = predicate;
    _action["list1"] = list1;
    _action["list2"] = list2;

    return client.perform(_action);
}

/**
 * This produces an array containing only values from another array that pass
 * a predicate test.
 * 
 * @param {function} predicate - This is a boolean function that is called to
 *     determine if an array entry should be included in the returned array.
 * 
 * @param {} values - These are the values passed to the predicate with the
 *     exception that the first array is passed as the single current value
 *     from the array. At least one array is required.
 * 
 * @returns {array} This is the resulting filtered array.
 */
export function afwFilter(client : any, predicate : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "filter";
    _action["predicate"] = predicate;
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * The predicate is called for each value in the first array in values until
 * true is returned, then that value is returned.
 * 
 * @param {function} predicate - This is a boolean function that is called to
 *     determine if an array entry passes the test.
 * 
 * @param {} values - These are the values passed to the predicate with the
 *     exception that the first array is passed as the single current value
 *     from the array. At least one array is required.
 * 
 * @returns {} The first value that passes the test is returned.
 */
export function afwFind(client : any, predicate : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "find";
    _action["predicate"] = predicate;
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * This function creates an array of the results of calling functor with each
 * value of the first array in values
 * 
 * @param {function} functor -
 * 
 * @param {} values - These are the parameters passed to functor with the
 *     exception that the first array is passed one value at a time. At least
 *     one array is required.
 * 
 * @returns {array}
 */
export function afwMap(client : any, functor : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "map";
    _action["functor"] = functor;
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Reduce calls functor for each value in array with two parameters,
 * accumulator and value, and must return a value of any dataType. Parameter
 * accumulator is the reduce() accumulator parameter value on first call and
 * the return value of previous functor() call on subsequent calls. The
 * dataType of the return value should normally be the same as accumulator,
 * but this is not required.
 * 
 * @param {function} functor - This function is called for each value in an
 *     array. The returned value is passed as the accumulator parameter on
 *     the next call to functor().
 * 
 * @param {} accumulator - This is an initial accumulator value passed to
 *     functor(). Normally, the dataType of accumulator will be the dataTape
 *     for the reduce() return value, but this is not required.
 * 
 * @param {array} array - This is an array to be reduced.
 * 
 * @returns {} This is the final return value from functor() or the
 *     accumulator parameter value if array is empty.
 */
export function afwReduce(client : any, functor : any, accumulator : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "reduce";
    _action["functor"] = functor;
    _action["accumulator"] = accumulator;
    _action["array"] = array;

    return client.perform(_action);
}

/**
 * This produces an array with values sorted based on result of
 * compareFunction. The compareFunction is passed two values from the array
 * and must return an integer less than 0 if the first value is less than the
 * second value, 0 if they are equal, and a integer greater than 0 if the
 * first value is greater than the second value.
 * 
 * @param {function} compareFunction - This function is called with two value
 *     from array.
 * 
 * @param {array} array - This is the array to sort.
 * 
 * @returns {array} This the the resulting sorted array.
 */
export function afwSort(client : any, compareFunction : any, array : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "sort";
    _action["compareFunction"] = compareFunction;
    _action["array"] = array;

    return client.perform(_action);
}

