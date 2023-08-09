// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for compiler_test_script
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_compiler_test_script.c
 * @brief afw_function_execute_* functions for compiler_test_script.
 */

#include "afw.h"



/*
 * Adaptive function: test_script_runtime_support
 *
 * afw_function_execute_test_script_runtime_support
 *
 * See afw_function_bindings.h for more information.
 *
 * This is a function called internally as the result of a test_script compile.
 * This function is not intended to be called directly.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function test_script_runtime_support(
 *       testScriptObject: (object _AdaptiveTestScriptResult_)
 *   ): (object _AdaptiveTestScriptResult_);
 * ```
 *
 * Parameters:
 *
 *   testScriptObject - (object _AdaptiveTestScriptResult_) A test script
 *       results object with the required evaluation result properties missing.
 *       The sources will be evaluated and the corresponding test result
 *       properties will be set.
 *
 * Returns:
 *
 *   (object _AdaptiveTestScriptResult_) The testScriptObject object with test
 *       result properties set.
 */
const afw_value_t *
afw_function_execute_test_script_runtime_support(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
