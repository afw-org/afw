// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for compiler_expression
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Returns the first value of values that is not null or undefined leaving
 * the remaining values unevaluated.
 * 
 * @param {} values -
 * 
 * @returns {} The first value of values that is not null or undefined.
 */
export function afwNullishCoalescing(client : any, values : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "nullish_coalescing";
    _action["values"] = values;

    return client.perform(_action);
}

/**
 * Returns undefined if arg1 is null or undefined without evaluating arg2,
 * but otherwise returns evaluated value of arg2.
 * 
 * @param {} arg1 -
 * 
 * @param {} arg2 -
 * 
 * @returns {} Undefined value if arg1 is null or undefined but otherwise
 *     evaluated arg2.
 */
export function afwOptionalChaining(client : any, arg1 : any, arg2 : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "optional_chaining";
    _action["arg1"] = arg1;
    _action["arg2"] = arg2;

    return client.perform(_action);
}

/**
 * This is the support function for the void operator which evaluates value
 * and returns undefined.
 * 
 * @param {} value - This is the value to evaluate.
 * 
 * @returns {} This always returns undefined.
 */
export function afwVoidOperator(client : any, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "void_operator";
    _action["value"] = value;

    return client.perform(_action);
}

