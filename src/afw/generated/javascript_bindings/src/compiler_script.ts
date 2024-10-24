// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for compiler_script
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file compiler_script.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the compiler_script category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Assign a value to the innermost structured block definition of a variable.
 * If the variable is not defined, the variable is defined in the innermost
 * structured block. An error is thrown if not called from an array of values
 * (statements) in a structured function.
 * 
 * @param {string} name - Variable name
 * 
 * @param {} value - This is the value to assign to the variable.
 * 
 * @returns {} The value assigned.
 */
export function afwAssign(client : any, name : string, value : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "assign";
    _action["name"] = name;
    _action["value"] = value;

    return client.perform(_action);
}

/**
 * This is a special function that can be called to break out of the body of a
 * loop. If called outside of a loop body, an error is thrown.
 * 
 * @param {} value - The value to evaluate that the enclosing loop will
 *     return. If not specified, the last evaluated value or a null value will
 *     be returned.
 * 
 * @returns {} This function returns from the body of a loop with the last
 *     evaluated value.
 */
export function afwBreak(client : any, value? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "break";

    if (value !== undefined)
        _action["value"] = value;

    return client.perform(_action);
}

/**
 * Define one or more statically scoped constants local to the current script
 * block with a permanent value. These constants can be accessed from the
 * current block and inner blocks, but can not be assigned a different value.
 * 
 * @param {array} name - The name of one or more constants to defined in the
 *     current block.
 * 
 * @param {} value - This is the value of the constant(s).
 * 
 * @param {object} type - The type of the constant(s).
 * 
 * @returns {} The value assigned.
 */
export function afwConst(client : any, name : any[], value : any, type? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "const";
    _action["name"] = name;
    _action["value"] = value;

    if (type !== undefined)
        _action["type"] = type;

    return client.perform(_action);
}

/**
 * This is a special function that can be called in the body of a loop
 * function to test the condition and, if true, start evaluating the body
 * again. If called outside of a loop body, an error is thrown.
 * 
 * @returns {} This function does not return.
 */
export function afwContinue(client : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "continue";

    return client.perform(_action);
}

/**
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function will evaluate an array of values (statements) at least once
 * while a condition is true. See the related functions 'break', 'continue',
 * 'return' and 'throw'.
 * 
 * @param {boolean} condition - While this condition is true, the loop will
 *     continue. This is evaluated in the loop's scope.
 * 
 * @param {array} body - This is an array of values (statements) that are
 *     evaluated for each iteration of the loop. Each value in body is
 *     evaluated in order until the end of the array or until a 'break',
 *     'continue', 'return' or 'throw' function is encountered.
 * 
 * @returns {} The last value evaluated in body or null if the body is empty.
 */
export function afwDoWhile(client : any, condition : boolean, body : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "do_while";
    _action["condition"] = condition;
    _action["body"] = body;

    return client.perform(_action);
}

/**
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function loops while condition is true. If the condition is false for
 * the first iteration, the loop returns a null value.
 * 
 * @param {array} initial - This is an array of values (statements) to
 *     evaluate before the loop starts. The values will normally be a call to
 *     the 'assign' function.
 * 
 * @param {boolean} condition - While this condition is true, the loop will
 *     continue.
 * 
 * @param {array} increment - This is an array of values (statements) to
 *     evaluate after each iteration of the loop. The values will normally be
 *     a call to the 'assign' function.
 * 
 * @param {array} body - This is an array of values (statements) that are
 *     evaluated for each iteration of the loop. Each value in body is
 *     evaluated in order until the end of the array or until a 'break',
 *     'continue', 'return' or 'throw' function is encountered.
 * 
 * @returns {} The last value evaluated in body or null if condition evaluates
 *     to false the first time.
 */
export function afwFor(client : any, initial? : any[], condition? : boolean, increment? : any[], body? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "for";

    if (initial !== undefined)
        _action["initial"] = initial;

    if (condition !== undefined)
        _action["condition"] = condition;

    if (increment !== undefined)
        _action["increment"] = increment;

    if (body !== undefined)
        _action["body"] = body;

    return client.perform(_action);
}

/**
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function will evaluate an array of values (statements) while a
 * condition is true with initial and increment values. The condition is
 * tested at the beginning of the loop. If the condition is false for the
 * first iteration, the loop returns a null value. This supports for-of
 * statement.
 * 
 * @param {array} name - Variable name(s).
 * 
 * @param {} value - Any array, object or single value.
 * 
 * @param {array} body - This is an array of values (statements) that are
 *     evaluated for each iteration of the loop. Each value in body is
 *     evaluated in order until the end of the array or until a 'break',
 *     'continue', 'return' or 'throw' function is encountered.
 * 
 * @returns {} The last value evaluated in body or null if condition evaluates
 *     to false the first time.
 */
export function afwForOf(client : any, name : any[], value : any, body? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "for_of";
    _action["name"] = name;
    _action["value"] = value;

    if (body !== undefined)
        _action["body"] = body;

    return client.perform(_action);
}

/**
 * Evaluate one of two different values depending on test condition.
 * 
 * @param {boolean} condition - If true, parameter 'then' is evaluated for
 *     result. If false, parameter 'else' is evaluated.
 * 
 * @param {array} then - This is the body of a structured block that is
 *     evaluated if 'condition' is true. See the 'body' parameter of the
 *     'block' function for information on how the body is processed.
 * 
 * @param {array} _else - This is the body of a structured block that is
 *     evaluated if 'condition' is false. If not specified and condition is
 *     false, a null value is returned. See the 'body' parameter of the
 *     'block' function for information on how the body is processed.
 * 
 * @returns {} The result of evaluating 'then' or 'else'
 */
export function afwIf(client : any, condition : boolean, then : any[], _else? : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "if";
    _action["condition"] = condition;
    _action["then"] = then;

    if (_else !== undefined)
        _action["else"] = _else;

    return client.perform(_action);
}

/**
 * Include an external adaptive script, json, or template to be compiled and
 * returned.
 * 
 * @param {string} file - The path of the file to include, which will be
 *     resolved using rootFilePaths.
 * 
 * @param {string} compileType - The compile type, used by the parser to
 *     determine how to compile the data. For example, 'json', 'relaxed_json',
 *     'script', 'template'
 * 
 * @returns {any}
 */
export function afwInclude(client : any, file : string, compileType? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "include";
    _action["file"] = file;

    if (compileType !== undefined)
        _action["compileType"] = compileType;

    return client.perform(_action);
}

/**
 * Declare one or more statically scoped variable locations local to the
 * current script block and optionally assign them an initial value. These
 * variables can be accessed and assigned different values from the current
 * block and inner blocks.
 * 
 * @param {array} name - The name of one or more variables to declared in the
 *     current block.
 * 
 * @param {} value - This is the initial value of the variable(s). If not
 *     specified, the variable will have a value of undefined.
 * 
 * @param {object} type - The type of the variable(s).
 * 
 * @returns {} The value assigned.
 */
export function afwLet(client : any, name : any[], value? : any, type? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "let";
    _action["name"] = name;

    if (value !== undefined)
        _action["value"] = value;

    if (type !== undefined)
        _action["type"] = type;

    return client.perform(_action);
}

/**
 * This is a special function that can be called to rethrow an error inside of
 * a catch block. If called outside of a catch body, an error is thrown.
 * 
 * @returns {} This function rethrows the current error in a catch block.
 */
export function afwRethrow(client : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "rethrow";

    return client.perform(_action);
}

/**
 * Return from the outermost structured block. If the expression of a lambda
 * function is a block function, this will effectively return from the lambda
 * function. If called outside of a structured block, an error is thrown.
 * 
 * @param {} value - The value to evaluate that the outermost block will
 *     return. If not specified, the last evaluated value or a null value will
 *     be returned.
 * 
 * @returns {} This function returns from the outermost structured block with
 *     the last evaluated value.
 */
export function afwReturn(client : any, value? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "return";

    if (value !== undefined)
        _action["value"] = value;

    return client.perform(_action);
}

/**
 * Support for switch statement.
 * 
 * @param {function} predicate - The predicate is passed two parameters and
 *     must return a boolean. The first parameter passed is the evaluated
 *     value of the value1 parameter and the second is the value2 from a case
 *     clause. This predicate will often be 'eqx' to use the exactly equal
 *     function but can also be any other function such as 'regexp_match' or a
 *     lambda function.
 * 
 * @param {} value1 - The first parameter passed to the predicate.
 * 
 * @param {} case_clause - This is one or more case clauses which are pairs of
 *     a value2 parameter followed by a statement list or undefined parameter.
 *     One value2 can be undefined to indicate the default case clause.
 * 
 *     For the first value2 that is undefined or calling the predicate returns
 *     true, the statement list followed by any statement lists of subsequent
 *     case clauses are executed until a break or return is encountered. The
 *     predicate is called with value1 and the case clause's value2.
 * 
 * @returns {}
 */
export function afwSwitch(client : any, predicate : any, value1 : any, case_clause : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "switch";
    _action["predicate"] = predicate;
    _action["value1"] = value1;
    _action["case_clause"] = case_clause;

    return client.perform(_action);
}

/**
 * This throws an error that can be caught by a try/catch block. An error
 * object of object type _AdaptiveError_ will be available in the catch block.
 * Its 'id' property will be set to 'throw'. The other properties set based on
 * the parameters specified and where this function is called.
 * 
 * @param {string} message - This is the message that will be included in the
 *     _AdaptiveError_ error object available in the catch block.
 * 
 * @param {} additional - Optional additional information that will be
 *     available as a 'additional' property in the error object.
 * 
 * @returns {void}
 */
export function afwThrow(client : any, message : string, additional? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "throw";
    _action["message"] = message;

    if (additional !== undefined)
        _action["additional"] = additional;

    return client.perform(_action);
}

/**
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function will evaluate the body statements. If an error is thrown and
 * there is an optional catch, the error will be 'caught' and the associated
 * statements will be evaluated. The optional finally statements are always
 * evaluated after the body and catch statements. See the related functions
 * 'break', 'continue', 'return' and 'throw'.
 * 
 * @param {array} body - This is an array of values (statements) that are
 *     evaluated. Each value in body is evaluated in order until the end of
 *     the list or until a 'break', 'continue', 'return' or 'throw' function
 *     is encountered.
 * 
 * @param {array} _finally - This is an array of values (statements) that are
 *     evaluated after the try and catch statements even if an error occurs.
 *     Each value in body is evaluated in order until the end of the list or
 *     until a 'break', 'continue', 'return' or 'throw' function is
 *     encountered.
 * 
 * @param {array} _catch - This is an array of values (statements) that are
 *     evaluated when an error is thrown while evaluating the body. Each value
 *     in body is evaluated in order until the end of the list or until a
 *     'break', 'continue', 'return' or 'throw' function is encountered.
 * 
 * @param {object} error - The error object thrown. This is only available in
 *     the catch block. See adaptive object type _AdaptiveObjectType_ for
 *     details.
 * 
 * @returns {} The last value evaluated in body.
 */
export function afwTry(client : any, body : any[], _finally? : any[], _catch? : any[], error? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "try";
    _action["body"] = body;

    if (_finally !== undefined)
        _action["finally"] = _finally;

    if (_catch !== undefined)
        _action["catch"] = _catch;

    if (error !== undefined)
        _action["error"] = error;

    return client.perform(_action);
}

/**
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function will evaluate an array of values (statements) while a
 * condition is true. The condition is tested at the beginning of the loop. If
 * the condition is false for the first iteration, the loop returns a null
 * value. See the related functions 'break', 'continue', 'return' and 'throw'.
 * 
 * @param {boolean} condition - While this condition is true, the loop will
 *     continue. This is evaluated in the loop's scope.
 * 
 * @param {array} body - This is an array of values (statements) that are
 *     evaluated for each iteration of the loop. Each value in body is
 *     evaluated in order until the end of the list or until a 'break',
 *     'continue', 'return' or 'throw' function is encountered.
 * 
 * @returns {} The last value evaluated in body or null if condition evaluates
 *     to false the first time.
 */
export function afwWhile(client : any, condition : boolean, body : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "while";
    _action["condition"] = condition;
    _action["body"] = body;

    return client.perform(_action);
}

