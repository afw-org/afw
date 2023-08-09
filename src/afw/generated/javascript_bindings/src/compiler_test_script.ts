// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for compiler_test_script
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
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

