// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for higher_order_array
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file higher_order_array.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the higher_order_array category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Return true if predicate returns true for every entry of the first array in
 * values (index order), or if that array is empty. Entries whose value is
 * undefined are included. every() is an alias for the common single-array
 * form.
 * 
 * @param {function} predicate - This function is called for each value in the
 *     first array in values or until false is returned. If no calls return
 *     false, the result is true.
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
 * Returns true if the result of calling predicate with all of the combination
 * of values from array1 and array2 returns true.
 * 
 * @param {function} predicate - The predicate is passed two parameters, the
 *     first is a value from array1 and the second is a value from array2.
 * 
 * @param {array} array1 -
 * 
 * @param {array} array2 -
 * 
 * @returns {boolean}
 */
export function afwAllOfAll(client : any, predicate : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "all_of_all";
    _action["predicate"] = predicate;
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * This function returns true if the result of calling predicate with all of
 * the combination of values from array1 and any of the values of array2
 * returns true.
 * 
 * @param {function} predicate - The predicate is passed two parameters, the
 *     first is a value from array1 and the second is a value from array2.
 * 
 * @param {array} array1 -
 * 
 * @param {array} array2 -
 * 
 * @returns {boolean}
 */
export function afwAllOfAny(client : any, predicate : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "all_of_any";
    _action["predicate"] = predicate;
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Return true if predicate returns true for any entry of the first array in
 * values (index order). Entries whose value is undefined are included. Empty
 * array yields false. some() is an alias for the common single-array form.
 * 
 * @param {function} predicate - This function is called for each value in the
 *     first array in values or until true is returned. If no calls return
 *     true, the result is false.
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
 * Returns true if, for at least one value of array1, the predicate returns
 * true with every value of array2 (the predicate gets the value from array1
 * first).
 * 
 * @param {function} predicate - The predicate is passed two parameters, the
 *     first is a value from array1 and the second is a value from array2.
 * 
 * @param {array} array1 -
 * 
 * @param {array} array2 -
 * 
 * @returns {boolean}
 */
export function afwAnyOfAll(client : any, predicate : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "any_of_all";
    _action["predicate"] = predicate;
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * This function returns true if the result of calling predicate with any of
 * the combination of values from array1 and array2 returns true.
 * 
 * @param {function} predicate - The predicate is passed two parameters, the
 *     first is a value from array1 and the second is a value from array2.
 * 
 * @param {array} array1 -
 * 
 * @param {array} array2 -
 * 
 * @returns {boolean}
 */
export function afwAnyOfAny(client : any, predicate : any, array1 : any[], array2 : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "any_of_any";
    _action["predicate"] = predicate;
    _action["array1"] = array1;
    _action["array2"] = array2;

    return client.perform(_action);
}

/**
 * Return true if predicate returns true for every entry of array (index
 * order), or if array is empty. Entries whose value is undefined are
 * included.
 * 
 * @param {array} array - The array to walk. A string is not walked as an
 *     array: write [...value] to walk its characters.
 * 
 * @param {function} predicate - Called for each entry of array until it
 *     returns false. A script function is called with (value, index, array);
 *     a built-in function is called with the value alone, since its other
 *     parameters have their own meanings.
 * 
 * @returns {boolean}
 */
export function afwEvery(client : any, array : any[], predicate : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "every";
    _action["array"] = array;
    _action["predicate"] = predicate;

    return client.perform(_action);
}

/**
 * Return a new array of the entries of array for which predicate returns
 * true. Every index is considered, including entries whose value is
 * undefined. Order of kept entries is preserved.
 * 
 * @param {array} array - The array to walk. A string is not walked as an
 *     array: write [...value] to walk its characters.
 * 
 * @param {function} predicate - Returns true to keep the entry. A script
 *     function is called with (value, index, array); a built-in function is
 *     called with the value alone, since its other parameters have their own
 *     meanings.
 * 
 * @returns {array} A new array of the entries that passed the test (possibly
 *     empty).
 */
export function afwFilter(client : any, array : any[], predicate : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "filter";
    _action["array"] = array;
    _action["predicate"] = predicate;

    return client.perform(_action);
}

/**
 * Call predicate for each entry of array, in index order, until it returns
 * true, then return that entry. Entries whose value is undefined are
 * included. If no entry passes, the result is undefined (the same as a found
 * undefined entry; use find_index if you need to tell those apart).
 * 
 * @param {array} array - The array to walk. A string is not walked as an
 *     array: write [...value] to walk its characters.
 * 
 * @param {function} predicate - Returns true for the entry to return. A
 *     script function is called with (value, index, array); a built-in
 *     function is called with the value alone, since its other parameters
 *     have their own meanings.
 * 
 * @returns {} The first matching entry, or undefined if none match.
 */
export function afwFind(client : any, array : any[], predicate : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "find";
    _action["array"] = array;
    _action["predicate"] = predicate;

    return client.perform(_action);
}

/**
 * Call predicate for each entry of array, in index order, until it returns
 * true, then return that entry's index. Entries whose value is undefined are
 * included. If no entry passes, the result is -1.
 * 
 * @param {array} array - The array to walk. A string is not walked as an
 *     array: write [...value] to walk its characters.
 * 
 * @param {function} predicate - Returns true for the entry whose index to
 *     return. A script function is called with (value, index, array); a
 *     built-in function is called with the value alone, since its other
 *     parameters have their own meanings.
 * 
 * @returns {integer} The index of the first matching entry, or -1 if none
 *     match.
 */
export function afwFindIndex(client : any, array : any[], predicate : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "find_index";
    _action["array"] = array;
    _action["predicate"] = predicate;

    return client.perform(_action);
}

/**
 * Call predicate for each entry of array, from the last to the first, until
 * it returns true, then return that entry. Entries whose value is undefined
 * are included. Only the indexes there at the start are visited, each as it
 * is when it is reached; an index predicate removed reads as undefined. If no
 * entry passes, the result is undefined (the same as a found undefined entry;
 * use find_last_index if you need to tell those apart).
 * 
 * @param {array} array - The array to walk. A string is not walked as an
 *     array: write [...value] to walk its characters.
 * 
 * @param {function} predicate - Returns true for the entry to return. A
 *     script function is called with (value, index, array); a built-in
 *     function is called with the value alone, since its other parameters
 *     have their own meanings.
 * 
 * @returns {} The last matching entry, or undefined if none match.
 */
export function afwFindLast(client : any, array : any[], predicate : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "find_last";
    _action["array"] = array;
    _action["predicate"] = predicate;

    return client.perform(_action);
}

/**
 * Call predicate for each entry of array, from the last to the first, until
 * it returns true, then return that entry's index. Entries whose value is
 * undefined are included. If no entry passes, the result is -1.
 * 
 * @param {array} array - The array to walk. A string is not walked as an
 *     array: write [...value] to walk its characters.
 * 
 * @param {function} predicate - Returns true for the entry whose index to
 *     return. A script function is called with (value, index, array); a
 *     built-in function is called with the value alone, since its other
 *     parameters have their own meanings.
 * 
 * @returns {integer} The index of the last matching entry, or -1 if none
 *     match.
 */
export function afwFindLastIndex(client : any, array : any[], predicate : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "find_last_index";
    _action["array"] = array;
    _action["predicate"] = predicate;

    return client.perform(_action);
}

/**
 * Call callbackfn once for each entry of array, in index order, and return a
 * new array of the results, with each result that is an array replaced by its
 * entries (map then flat(1)). A result that is not an array is one entry, so
 * a callback can return zero, one or more entries.
 * 
 * @param {array} array - The array to walk. A string is not walked as an
 *     array: write [...value] to walk its characters.
 * 
 * @param {function} callbackfn - Called for each entry of array; an array
 *     result adds its entries, any other result adds itself. A script
 *     function is called with (value, index, array); a built-in function is
 *     called with the value alone, since its other parameters have their own
 *     meanings.
 * 
 * @returns {array} A new array of the results, one level flattened.
 */
export function afwFlatMap(client : any, array : any[], callbackfn : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "flat_map";
    _action["array"] = array;
    _action["callbackfn"] = callbackfn;

    return client.perform(_action);
}

/**
 * Call callbackfn once for each entry of array, in index order, and return a
 * new object with a property for each key it returned, whose value is an
 * array of the entries that got that key, in order. Properties are in the
 * order their keys first appeared. The key must be a string; it is not
 * converted (write to_string(v) for another data type).
 * 
 * @param {array} array - The array to walk. A string is not walked as an
 *     array: write [...value] to walk its characters.
 * 
 * @param {function} callbackfn - Returns the key, a string, of the group the
 *     entry goes in. A script function is called with (value, index, array);
 *     a built-in function is called with the value alone, since its other
 *     parameters have their own meanings.
 * 
 * @returns {object} A new object of arrays, one property per key.
 */
export function afwGroupBy(client : any, array : any[], callbackfn : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "group_by";
    _action["array"] = array;
    _action["callbackfn"] = callbackfn;

    return client.perform(_action);
}

/**
 * Call callbackfn once for each entry of array, in index order from 0 through
 * length minus one, and return a new array of the same length with the
 * results. Entries whose value is undefined (including omitted elements in
 * array literals) are included; callbackfn receives undefined for those
 * indexes.
 * 
 * @param {array} array - The array to walk. A string is not walked as an
 *     array: write [...value] to walk its characters.
 * 
 * @param {function} callbackfn - Called for each entry of array; its result
 *     is the entry of the new array at the same index. A script function is
 *     called with (value, index, array); a built-in function is called with
 *     the value alone, since its other parameters have their own meanings.
 * 
 * @returns {array} A new array with one result per entry of array.
 */
export function afwMap(client : any, array : any[], callbackfn : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "map";
    _action["array"] = array;
    _action["callbackfn"] = callbackfn;

    return client.perform(_action);
}

/**
 * Call callbackfn for each entry of array, in index order, with the
 * accumulator and that entry; each call's result is the accumulator for the
 * next call. If initialValue is passed, it is the first accumulator and every
 * entry is visited; otherwise the first entry is the first accumulator and
 * the walk starts at the second. Undefined entries are visited. An empty
 * array with no initialValue is an error.
 * 
 * @param {array} array - The array to walk. A string is not walked as an
 *     array: write [...value] to walk its characters.
 * 
 * @param {function} callbackfn - Called for each entry visited; its result is
 *     the next accumulator. A script function is called with (accumulator,
 *     value, index, array); a built-in function is called with (accumulator,
 *     value).
 * 
 * @param {} initialValue - The first accumulator. When it is not passed, the
 *     first entry of array is.
 * 
 * @returns {} The last result of callbackfn, or the initial accumulator if
 *     callbackfn is not called.
 */
export function afwReduce(client : any, array : any[], callbackfn : any, initialValue? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "reduce";
    _action["array"] = array;
    _action["callbackfn"] = callbackfn;

    if (initialValue !== undefined)
        _action["initialValue"] = initialValue;

    return client.perform(_action);
}

/**
 * Call callbackfn for each entry of array, from the last to the first, with
 * the accumulator and that entry; each call's result is the accumulator for
 * the next call. If initialValue is passed, it is the first accumulator and
 * every entry is visited; otherwise the last entry is the first accumulator
 * and the walk starts at the one before it. Undefined entries are visited.
 * Only the indexes there at the start are visited, each as it is when it is
 * reached; an index callbackfn removed is skipped. An empty array with no
 * initialValue is an error.
 * 
 * @param {array} array - The array to walk. A string is not walked as an
 *     array: write [...value] to walk its characters.
 * 
 * @param {function} callbackfn - Called for each entry visited; its result is
 *     the next accumulator. A script function is called with (accumulator,
 *     value, index, array); a built-in function is called with (accumulator,
 *     value).
 * 
 * @param {} initialValue - The first accumulator. When it is not passed, the
 *     last entry of array is.
 * 
 * @returns {} The last result of callbackfn, or the initial accumulator if
 *     callbackfn is not called.
 */
export function afwReduceRight(client : any, array : any[], callbackfn : any, initialValue? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "reduce_right";
    _action["array"] = array;
    _action["callbackfn"] = callbackfn;

    if (initialValue !== undefined)
        _action["initialValue"] = initialValue;

    return client.perform(_action);
}

/**
 * Return true if predicate returns true for any entry of array (index order).
 * Entries whose value is undefined are included. An empty array yields false.
 * 
 * @param {array} array - The array to walk. A string is not walked as an
 *     array: write [...value] to walk its characters.
 * 
 * @param {function} predicate - Called for each entry of array until it
 *     returns true. A script function is called with (value, index, array); a
 *     built-in function is called with the value alone, since its other
 *     parameters have their own meanings.
 * 
 * @returns {boolean}
 */
export function afwSome(client : any, array : any[], predicate : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "some";
    _action["array"] = array;
    _action["predicate"] = predicate;

    return client.perform(_action);
}

/**
 * Sort the entries of array in place, ordered by compareFn, or, with no
 * compareFn, as < orders them, and return the same array. The sort is stable:
 * entries that compare equal keep their order. With no compareFn, every entry
 * must be of one data type (a mix is an error, as it is for <); entries are
 * compared as values, not as text, so [10, 9, 1] sorts to [1, 9, 10]. NaN
 * sorts after every other double. With compareFn, the array may hold any mix
 * of data types. Undefined entries go last and are not compared. A frozen
 * array is an error before compareFn is called. Use to_sorted for a sorted
 * copy.
 * 
 * @param {array} array - The array to sort. Must not be frozen.
 * 
 * @param {function} compareFn - Called with two defined entries, once per
 *     comparison. If omitted, entries are ordered as < orders them. Return
 *     true, or a negative integer, when a sorts before b; false, zero or a
 *     positive integer otherwise. Entries that compare equal keep their
 *     order. Any other result, a double included, is an error.
 * 
 * @returns {array} The array, sorted.
 */
export function afwSort(client : any, array : any[], compareFn? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "sort";
    _action["array"] = array;

    if (compareFn !== undefined)
        _action["compareFn"] = compareFn;

    return client.perform(_action);
}

/**
 * Return a new array with the entries of array ordered by compareFn, or, with
 * no compareFn, as < orders them, leaving array unchanged. The sort is
 * stable: entries that compare equal keep their order. Undefined entries go
 * last and are not compared. With no compareFn, every entry must be of one
 * data type, and NaN sorts after every other double. The new array is an
 * `array<T>` only if array is one.
 * 
 * @param {array} array - The array to sort.
 * 
 * @param {function} compareFn - Called with two defined entries, once per
 *     comparison. If omitted, entries are ordered as < orders them. Return
 *     true, or a negative integer, when a sorts before b; false, zero or a
 *     positive integer otherwise. Entries that compare equal keep their
 *     order. Any other result, a double included, is an error.
 * 
 * @returns {array} A new array with the entries of array in sorted order.
 */
export function afwToSorted(client : any, array : any[], compareFn? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "to_sorted";
    _action["array"] = array;

    if (compareFn !== undefined)
        _action["compareFn"] = compareFn;

    return client.perform(_action);
}

