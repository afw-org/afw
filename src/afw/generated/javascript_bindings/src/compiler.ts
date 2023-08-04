// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for compiler
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Assert that a value is true. If not, an assertion_failed error is thrown.
 * 
 * @param {boolean} assertion - This is the assertion to make. If not true,
 *     an assertion_failed error is thrown.
 * 
 * @param {string} reason - This is an optional reason to include in the
 *     assertion_failed message.
 * 
 * @returns {null}
 */
export function afwAssert(client : any, assertion : boolean, reason? : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "assert";
    _action["assertion"] = assertion;

    if (reason !== undefined)
        _action["reason"] = reason;

    return client.perform(_action);
}

/**
 * Compile a string containing adaptive expression tuple syntax and return
 * either an unevaluated expression tuple adaptive value or a string
 * containing the compiler listing.
 * 
 * @param {list} expression_tuple - expression tuple to compile.
 * 
 * @param {} listing - If specified, a compiler listing is produced instead
 *     of an unevaluated expression tuple value.
 *     
 *     This parameter can be an integer between 0 and 10 of a string that is
 *     used for indentation. If 0 is specified, no whitespace is added to the
 *     resulting string. If 1 through 10 is specified, that number of spaces
 *     is used.
 * 
 * @returns {} An unevaluated expression tuple value ready for use by
 *     function evaluate() or a string containing the compiler listing.
 */
export function afwCompileExpressionTuple(client : any, expression_tuple : any[], listing? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "compile_expression_tuple";
    _action["expression_tuple"] = expression_tuple;

    if (listing !== undefined)
        _action["listing"] = listing;

    return client.perform(_action);
}

/**
 * Compile a string containing adaptive JSON syntax and return either an
 * unevaluated JSON adaptive value or a string containing the compiler
 * listing.
 * 
 * @param {string} json - JSON string to compile.
 * 
 * @param {} listing - If specified, a compiler listing is produced instead
 *     of an unevaluated JSON value.
 *     
 *     This parameter can be an integer between 0 and 10 of a string that is
 *     used for indentation. If 0 is specified, no whitespace is added to the
 *     resulting string. If 1 through 10 is specified, that number of spaces
 *     is used.
 * 
 * @returns {} An unevaluated JSON value ready for use by function evaluate()
 *     or a string containing the compiler listing.
 */
export function afwCompileJson(client : any, json : string, listing? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "compile_json";
    _action["json"] = json;

    if (listing !== undefined)
        _action["listing"] = listing;

    return client.perform(_action);
}

/**
 * Compile an adaptive relaxed JSON syntax string and return an adaptive
 * value.
 * 
 * @param {string} json - Adaptive relaxed JSON syntax string to compile.
 * 
 * @param {} listing - If specified, a compiler listing is produced instead
 *     of an unevaluated relaxed JSON value.
 *     
 *     This parameter can be an integer between 0 and 10 of a string that is
 *     used for indentation. If 0 is specified, no whitespace is added to the
 *     resulting string. If 1 through 10 is specified, that number of spaces
 *     is used.
 * 
 * @returns {} An unevaluated relaxed JSON value ready for use by function
 *     evaluate() or a string containing the compiler listing.
 */
export function afwCompileRelaxedJson(client : any, json : string, listing? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "compile_relaxed_json";
    _action["json"] = json;

    if (listing !== undefined)
        _action["listing"] = listing;

    return client.perform(_action);
}

/**
 * Convert a string containing adaptive hybrid syntax, which can be an
 * adaptive template or adaptive expression tuple, to adaptive expression
 * syntax.
 * 
 * @param {string} hybrid - The hybrid to convert.
 * 
 * @param {} whitespace - Add whitespace for readability if present and not
 *     0. This parameter can be an integer between 0 and 10 or a string that
 *     is used for indentation. If 0 is specified, no whitespace is added to
 *     the resulting string. If 1 through 10 is specified, that number of
 *     spaces is used.
 * 
 * @returns {expression} The converted value in adaptive expression syntax.
 */
export function afwConvertSyntaxHybridToExpression(client : any, hybrid : string, whitespace? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "convert_syntax_hybrid_to_expression";
    _action["hybrid"] = hybrid;

    if (whitespace !== undefined)
        _action["whitespace"] = whitespace;

    return client.perform(_action);
}

/**
 * Decompile an adaptive value to string.
 * 
 * @param {} value - Value to decompile.
 * 
 * @param {} whitespace - Add whitespace for readability if present and not
 *     0. This parameter can be an integer between 0 and 10 or a string that
 *     is used for indentation. If 0 is specified, no whitespace is added to
 *     the resulting string. If 1 through 10 is specified, that number of
 *     spaces is used.
 * 
 * @returns {string} Decompiled value.
 */
export function afwDecompile(client : any, value : any, whitespace? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "decompile";
    _action["value"] = value;

    if (whitespace !== undefined)
        _action["whitespace"] = whitespace;

    return client.perform(_action);
}

/**
 * Compile a string containing adaptive expression tuple syntax and then
 * evaluate the result.
 * 
 * @param {string} expression_tuple - Expression tuple to compile and
 *     evaluate.
 * 
 * @param {object} additionalUntrustedQualifiedVariables - This parameter
 *     supplies additional qualified variables that can be accessed during
 *     evaluation. These variables will not be used by anything that needs to
 *     ensure its qualified variables must come from a trusted source, such
 *     as authorization. This parameter is intended to be used for testing
 *     only and should not be used for anything running in production.
 * 
 * @returns {} Evaluated adaptive expression tuple.
 */
export function afwEvaluateExpressionTuple(client : any, expression_tuple : string, additionalUntrustedQualifiedVariables? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "evaluate_expression_tuple";
    _action["expression_tuple"] = expression_tuple;

    if (additionalUntrustedQualifiedVariables !== undefined)
        _action["additionalUntrustedQualifiedVariables"] = additionalUntrustedQualifiedVariables;

    return client.perform(_action);
}

/**
 * Evaluate an adaptive value.
 * 
 * @param {} value -
 * 
 * @param {object} additionalUntrustedQualifiedVariables - This parameter
 *     supplies additional qualified variables that can be accessed during
 *     evaluation. These variables will not be used by anything that needs to
 *     ensure its qualified variables must come from a trusted source, such
 *     as authorization. This parameter is intended to be used for testing
 *     only and should not be used for anything running in production.
 * 
 * @returns {} Evaluated adaptive value.
 */
export function afwEvaluateValue(client : any, value : any, additionalUntrustedQualifiedVariables? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "evaluate_value";
    _action["value"] = value;

    if (additionalUntrustedQualifiedVariables !== undefined)
        _action["additionalUntrustedQualifiedVariables"] = additionalUntrustedQualifiedVariables;

    return client.perform(_action);
}

/**
 * Evaluate a value and retry up to a limit if an exception occurs.
 * 
 * @param {} value - Value to evaluated
 * 
 * @param {integer} limit - Maximum number to retry if an exception occurs.
 * 
 * @returns {} Evaluated value.
 */
export function afwEvaluateWithRetry(client : any, value : any, limit : number) : any {

    let _action : IAnyObject = {};

    _action["function"] = "evaluate_with_retry";
    _action["value"] = value;
    _action["limit"] = limit;

    return client.perform(_action);
}

/**
 * This function allows the active variables for a qualifier to be accessed
 * as the properties of an object.
 * 
 * @param {string} qualifier - This is the qualifier whose variables are to
 *     be accessed as properties of the returned object.
 * 
 * @param {boolean} forTesting - If specified and true, the object returned
 *     will be suitable to pass as the additionalUntrustedQualifiedVariables
 *     parameter of evaluate*() functions. This is intended for testing
 *     purposes and should not be used in production.
 * 
 * @returns {object} Each property is the name of a variable with the value
 *     influenced by the forTesting property.
 */
export function afwQualifier(client : any, qualifier : string, forTesting? : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "qualifier";
    _action["qualifier"] = qualifier;

    if (forTesting !== undefined)
        _action["forTesting"] = forTesting;

    return client.perform(_action);
}

/**
 * This function allows the active qualifiers to be accessed as properties of
 * an object. The value of each of these properties is an object whose
 * properties are the variables for the corresponding qualifier.
 * 
 * @param {boolean} forTesting - If specified and true, the object returned
 *     will be suitable to pass as the additionalUntrustedQualifiedVariables
 *     parameter of evaluate*() functions. This is intended for testing
 *     purposes and should not be used in production.
 * 
 * @returns {object} Each property is the name of a qualifier with a value
 *     that is an object whose properties are the variables of that
 *     qualifier. The value of the variable properties is influenced by the
 *     forTesting property.
 */
export function afwQualifiers(client : any, forTesting? : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "qualifiers";

    if (forTesting !== undefined)
        _action["forTesting"] = forTesting;

    return client.perform(_action);
}

/**
 * Return the evaluated adaptive value. If an exception occurs, return
 * evaluated error instead.
 * 
 * @param {} value - Value to evaluated
 * 
 * @param {} error - Value to evaluate and return if exception occurs. If an
 *     error occurs evaluating this value, the exception will continue.
 * 
 * @returns {} Evaluated adaptive value or error value.
 */
export function afwSafeEvaluate(client : any, value : any, error : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "safe_evaluate";
    _action["value"] = value;
    _action["error"] = error;

    return client.perform(_action);
}

/**
 * Evaluate and decompile an adaptive value to string. For most values this
 * has the effect of producing a string containing json.
 * 
 * @param {} value - Value to stringify.
 * 
 * @param {} replacer - Optional replacer function.
 * 
 * @param {} whitespace - Add whitespace for readability if present and not
 *     0. This parameter can be an integer between 0 and 10 or a string that
 *     is used for indentation. If 0 is specified, no whitespace is added to
 *     the resulting string. If 1 through 10 is specified, that number of
 *     spaces is used.
 * 
 * @returns {string} Evaluated and decompiled value.
 */
export function afwStringify(client : any, value : any, replacer? : any, whitespace? : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "stringify";
    _action["value"] = value;

    if (replacer !== undefined)
        _action["replacer"] = replacer;

    if (whitespace !== undefined)
        _action["whitespace"] = whitespace;

    return client.perform(_action);
}

/**
 * Compile and evaluate an adaptive expression and compare the results to an
 * expected value. Return object with the test's results.
 * 
 * @param {string} id - Id of test
 * 
 * @param {string} description - Description of test
 * 
 * @param {string} expression - Expression to compile and evaluate.
 * 
 * @param {} expected - Expected result.
 * 
 * @param {object} additionalUntrustedQualifiedVariables - This parameter
 *     supplies additional qualified variables that can be accessed during
 *     evaluation. These variables will not be used by anything that needs to
 *     ensure its qualified variables must come from a trusted source, such
 *     as authorization. This parameter is intended to be used for testing
 *     only and should not be used for anything running in production.
 * 
 * @returns {object} Test results.
 */
export function afwTestExpression(client : any, id : string, description : string, expression : string, expected : any, additionalUntrustedQualifiedVariables? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "test_expression";
    _action["id"] = id;
    _action["description"] = description;
    _action["expression"] = expression;
    _action["expected"] = expected;

    if (additionalUntrustedQualifiedVariables !== undefined)
        _action["additionalUntrustedQualifiedVariables"] = additionalUntrustedQualifiedVariables;

    return client.perform(_action);
}

/**
 * Compile and evaluate an adaptive expression tuple and compare the results
 * to an expected value. Return object with the test's results.
 * 
 * @param {string} id - Id of test
 * 
 * @param {string} description - Description of test
 * 
 * @param {string} expression - Expression tuple to compile and evaluate.
 * 
 * @param {} expected - Expected result.
 * 
 * @param {object} additionalUntrustedQualifiedVariables - This parameter
 *     supplies additional qualified variables that can be accessed during
 *     evaluation. These variables will not be used by anything that needs to
 *     ensure its qualified variables must come from a trusted source, such
 *     as authorization. This parameter is intended to be used for testing
 *     only and should not be used for anything running in production.
 * 
 * @returns {object} Test results.
 */
export function afwTestExpressionTuple(client : any, id : string, description : string, expression : string, expected : any, additionalUntrustedQualifiedVariables? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "test_expression_tuple";
    _action["id"] = id;
    _action["description"] = description;
    _action["expression"] = expression;
    _action["expected"] = expected;

    if (additionalUntrustedQualifiedVariables !== undefined)
        _action["additionalUntrustedQualifiedVariables"] = additionalUntrustedQualifiedVariables;

    return client.perform(_action);
}

/**
 * Compile and evaluate a string containing adaptive hybrid syntax which can
 * be an adaptive template or adaptive expression tuple and then compare the
 * results to an expected value. Return object with the test's results.
 * 
 * @param {string} id - Id of test
 * 
 * @param {string} description - Description of test
 * 
 * @param {string} hybrid - Hybrid to compile and evaluate.
 * 
 * @param {} expected - Expected evaluated result.
 * 
 * @param {object} additionalUntrustedQualifiedVariables - This parameter
 *     supplies additional qualified variables that can be accessed during
 *     evaluation. These variables will not be used by anything that needs to
 *     ensure its qualified variables must come from a trusted source, such
 *     as authorization. This parameter is intended to be used for testing
 *     only and should not be used for anything running in production.
 * 
 * @returns {object} Test results.
 */
export function afwTestHybrid(client : any, id : string, description : string, hybrid : string, expected : any, additionalUntrustedQualifiedVariables? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "test_hybrid";
    _action["id"] = id;
    _action["description"] = description;
    _action["hybrid"] = hybrid;
    _action["expected"] = expected;

    if (additionalUntrustedQualifiedVariables !== undefined)
        _action["additionalUntrustedQualifiedVariables"] = additionalUntrustedQualifiedVariables;

    return client.perform(_action);
}

/**
 * Compile and evaluate an adaptive script and compare the results to an
 * expected value. Return object with the test's results.
 * 
 * @param {string} id - Id of test
 * 
 * @param {string} description - Description of test
 * 
 * @param {string} script - Script to compile and evaluate.
 * 
 * @param {} expected - Expected result.
 * 
 * @param {object} additionalUntrustedQualifiedVariables - This parameter
 *     supplies additional qualified variables that can be accessed during
 *     evaluation. These variables will not be used by anything that needs to
 *     ensure its qualified variables must come from a trusted source, such
 *     as authorization. This parameter is intended to be used for testing
 *     only and should not be used for anything running in production.
 * 
 * @returns {object} Test results.
 */
export function afwTestScript(client : any, id : string, description : string, script : string, expected : any, additionalUntrustedQualifiedVariables? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "test_script";
    _action["id"] = id;
    _action["description"] = description;
    _action["script"] = script;
    _action["expected"] = expected;

    if (additionalUntrustedQualifiedVariables !== undefined)
        _action["additionalUntrustedQualifiedVariables"] = additionalUntrustedQualifiedVariables;

    return client.perform(_action);
}

/**
 * This is a function called internally as the result of a test_script
 * compile. This function is not intended to be called directly.
 * 
 * @param {object} testScriptObject - A test script results object with the
 *     required evaluation result properties missing. The sources will be
 *     evaluated and the corresponding test result properties will be set.
 * 
 * @returns {object} The testScriptObject object with test result properties
 *     set.
 */
export function afwTestScriptRuntimeSupport(client : any, testScriptObject : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "test_script_runtime_support";
    _action["testScriptObject"] = testScriptObject;

    return client.perform(_action);
}

/**
 * Compile and evaluate an adaptive template and compare the results to an
 * expected value. Return object with the test's results.
 * 
 * @param {string} id - Id of test
 * 
 * @param {string} description - Description of test
 * 
 * @param {string} template - Template to compile and evaluate.
 * 
 * @param {} expected - Expected evaluated result.
 * 
 * @param {object} additionalUntrustedQualifiedVariables - This parameter
 *     supplies additional qualified variables that can be accessed during
 *     evaluation. These variables will not be used by anything that needs to
 *     ensure its qualified variables must come from a trusted source, such
 *     as authorization. This parameter is intended to be used for testing
 *     only and should not be used for anything running in production.
 * 
 * @returns {object} Test results.
 */
export function afwTestTemplate(client : any, id : string, description : string, template : string, expected : any, additionalUntrustedQualifiedVariables? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "test_template";
    _action["id"] = id;
    _action["description"] = description;
    _action["template"] = template;
    _action["expected"] = expected;

    if (additionalUntrustedQualifiedVariables !== undefined)
        _action["additionalUntrustedQualifiedVariables"] = additionalUntrustedQualifiedVariables;

    return client.perform(_action);
}

/**
 * Evaluate an adaptive value and compare it to an expected value. Return
 * object with the test's results.
 * 
 * @param {string} id - Id of test
 * 
 * @param {string} description - Description of test
 * 
 * @param {string} value - Value to evaluate
 * 
 * @param {} expected - Expected result.
 * 
 * @param {object} additionalUntrustedQualifiedVariables - This parameter
 *     supplies additional qualified variables that can be accessed during
 *     evaluation. These variables will not be used by anything that needs to
 *     ensure its qualified variables must come from a trusted source, such
 *     as authorization. This parameter is intended to be used for testing
 *     only and should not be used for anything running in production.
 * 
 * @returns {object} Test results.
 */
export function afwTestValue(client : any, id : string, description : string, value : string, expected : any, additionalUntrustedQualifiedVariables? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "test_value";
    _action["id"] = id;
    _action["description"] = description;
    _action["value"] = value;
    _action["expected"] = expected;

    if (additionalUntrustedQualifiedVariables !== undefined)
        _action["additionalUntrustedQualifiedVariables"] = additionalUntrustedQualifiedVariables;

    return client.perform(_action);
}

