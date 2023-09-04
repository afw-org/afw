// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for logical
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Evaluates 0 or more boolean conditions returning boolean true if there are
 * no conditions and boolean false if any condition evaluate to false. All
 * conditions after the first false remain unevaluated.
 * 
 * @param {boolean} conditions -
 * 
 * @returns {boolean}
 */
export function afwAnd(client : any, conditions : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "and";
    _action["conditions"] = conditions;

    return client.perform(_action);
}

/**
 * integer n specifies the number of boolean conditions that follow that must
 * evaluate to true for boolean true to be returned. If n is 0, true is
 * returned. Once n conditions evaluate to true, true is returned and the
 * remaining conditions remain unevaluated.
 * 
 * @param {integer} n -
 * 
 * @param {boolean} conditions -
 * 
 * @returns {boolean}
 */
export function afwNOf(client : any, n : number, conditions : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "n_of";
    _action["n"] = n;
    _action["conditions"] = conditions;

    return client.perform(_action);
}

/**
 * Evaluates boolean condition returning boolean true if condition evaluates
 * to false and false if condition evaluates to true.
 * 
 * @param {boolean} condition -
 * 
 * @returns {boolean}
 */
export function afwNot(client : any, condition : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "not";
    _action["condition"] = condition;

    return client.perform(_action);
}

/**
 * Evaluates 0 or more boolean conditions returning boolean false if there are
 * no conditions and boolean true if any condition evaluate to true. All
 * conditions after the first true remain unevaluated.
 * 
 * @param {boolean} conditions -
 * 
 * @returns {boolean}
 */
export function afwOr(client : any, conditions : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "or";
    _action["conditions"] = conditions;

    return client.perform(_action);
}

