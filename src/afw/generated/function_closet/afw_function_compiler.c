// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for compiler
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_compiler.c
 * @brief afw_function_execute_* functions for compiler.
 */

#include "afw.h"



/*
 * Adaptive function: assert
 *
 * afw_function_execute_assert
 *
 * See afw_function_bindings.h for more information.
 *
 * Assert that a value is true. If not, an assertion_failed error is thrown.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function assert(
 *       assertion: boolean,
 *       reason?: string
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   assertion - (boolean) This is the assertion to make. If not true, an
 *       assertion_failed error is thrown.
 *
 *   reason - (optional string) This is an optional reason to include in the
 *       assertion_failed message.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_assert(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: compile_expression_tuple
 *
 * afw_function_execute_compile_expression_tuple
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile a string containing adaptive expression tuple syntax and return
 * either an unevaluated expression tuple adaptive value or a string containing
 * the compiler listing.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function compile_expression_tuple(
 *       expression_tuple: array,
 *       listing?: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   expression_tuple - (array) expression tuple to compile.
 *
 *   listing - (optional any dataType) If specified, a compiler listing is
 *       produced instead of an unevaluated expression tuple value.
 *       
 *       This parameter can be an integer between 0 and 10 of a string that is
 *       used for indentation. If 0 is specified, no whitespace is added to the
 *       resulting string. If 1 through 10 is specified, that number of spaces
 *       is used.
 *
 * Returns:
 *
 *   (any dataType) An unevaluated expression tuple value ready for use by
 *       function evaluate() or a string containing the compiler listing.
 */
const afw_value_t *
afw_function_execute_compile_expression_tuple(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: compile_json
 *
 * afw_function_execute_compile_json
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile a string containing adaptive JSON syntax and return either an
 * unevaluated JSON adaptive value or a string containing the compiler listing.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function compile_json(
 *       json: string,
 *       listing?: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   json - (string) JSON string to compile.
 *
 *   listing - (optional any dataType) If specified, a compiler listing is
 *       produced instead of an unevaluated JSON value.
 *       
 *       This parameter can be an integer between 0 and 10 of a string that is
 *       used for indentation. If 0 is specified, no whitespace is added to the
 *       resulting string. If 1 through 10 is specified, that number of spaces
 *       is used.
 *
 * Returns:
 *
 *   (any dataType) An unevaluated JSON value ready for use by function
 *       evaluate() or a string containing the compiler listing.
 */
const afw_value_t *
afw_function_execute_compile_json(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: compile_relaxed_json
 *
 * afw_function_execute_compile_relaxed_json
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile an adaptive relaxed JSON syntax string and return an adaptive value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function compile_relaxed_json(
 *       json: string,
 *       listing?: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   json - (string) Adaptive relaxed JSON syntax string to compile.
 *
 *   listing - (optional any dataType) If specified, a compiler listing is
 *       produced instead of an unevaluated relaxed JSON value.
 *       
 *       This parameter can be an integer between 0 and 10 of a string that is
 *       used for indentation. If 0 is specified, no whitespace is added to the
 *       resulting string. If 1 through 10 is specified, that number of spaces
 *       is used.
 *
 * Returns:
 *
 *   (any dataType) An unevaluated relaxed JSON value ready for use by function
 *       evaluate() or a string containing the compiler listing.
 */
const afw_value_t *
afw_function_execute_compile_relaxed_json(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: convert_syntax_hybrid_to_expression
 *
 * afw_function_execute_convert_syntax_hybrid_to_expression
 *
 * See afw_function_bindings.h for more information.
 *
 * Convert a string containing adaptive hybrid syntax, which can be an adaptive
 * template or adaptive expression tuple, to adaptive expression syntax.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function convert_syntax_hybrid_to_expression(
 *       hybrid: string,
 *       whitespace?: any
 *   ): expression;
 * ```
 *
 * Parameters:
 *
 *   hybrid - (string) The hybrid to convert.
 *
 *   whitespace - (optional any dataType) Add whitespace for readability if
 *       present and not 0. This parameter can be an integer between 0 and 10
 *       or a string that is used for indentation. If 0 is specified, no
 *       whitespace is added to the resulting string. If 1 through 10 is
 *       specified, that number of spaces is used.
 *
 * Returns:
 *
 *   (expression) The converted value in adaptive expression syntax.
 */
const afw_value_t *
afw_function_execute_convert_syntax_hybrid_to_expression(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: decompile
 *
 * afw_function_execute_decompile
 *
 * See afw_function_bindings.h for more information.
 *
 * Decompile an adaptive value to string.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function decompile(
 *       value: any,
 *       whitespace?: any
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   value - (any dataType) Value to decompile.
 *
 *   whitespace - (optional any dataType) Add whitespace for readability if
 *       present and not 0. This parameter can be an integer between 0 and 10
 *       or a string that is used for indentation. If 0 is specified, no
 *       whitespace is added to the resulting string. If 1 through 10 is
 *       specified, that number of spaces is used.
 *
 * Returns:
 *
 *   (string) Decompiled value.
 */
const afw_value_t *
afw_function_execute_decompile(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: evaluate_expression_tuple
 *
 * afw_function_execute_evaluate_expression_tuple
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile a string containing adaptive expression tuple syntax and then
 * evaluate the result.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function evaluate_expression_tuple(
 *       expression_tuple: string,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   expression_tuple - (string) Expression tuple to compile and evaluate.
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveHybridPropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (any dataType) Evaluated adaptive expression tuple.
 */
const afw_value_t *
afw_function_execute_evaluate_expression_tuple(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: evaluate_value
 *
 * afw_function_execute_evaluate_value
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate an adaptive value.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function evaluate_value(
 *       value: any,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   value - (any dataType)
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveHybridPropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (any dataType) Evaluated adaptive value.
 */
const afw_value_t *
afw_function_execute_evaluate_value(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: evaluate_with_retry
 *
 * afw_function_execute_evaluate_with_retry
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate a value and retry up to a limit if an exception occurs.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function evaluate_with_retry(
 *       value: any,
 *       limit: integer
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   value - (any dataType) Value to evaluated.
 *
 *   limit - (integer) Maximum number to retry if an exception occurs.
 *
 * Returns:
 *
 *   (any dataType) Evaluated value.
 */
const afw_value_t *
afw_function_execute_evaluate_with_retry(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: qualifier
 *
 * afw_function_execute_qualifier
 *
 * See afw_function_bindings.h for more information.
 *
 * This function allows the active variables for a qualifier to be accessed as
 * the properties of an object.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function qualifier(
 *       qualifier: string,
 *       forTesting?: boolean
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   qualifier - (string) This is the qualifier whose variables are to be
 *       accessed as properties of the returned object.
 *
 *   forTesting - (optional boolean) If specified and true, the object returned
 *       will be suitable to pass as the additionalUntrustedQualifiedVariables
 *       parameter of evaluate*() functions. This is intended for testing
 *       purposes and should not be used in production.
 *
 * Returns:
 *
 *   (object) Each property is the name of a variable with the value influenced
 *       by the forTesting property.
 */
const afw_value_t *
afw_function_execute_qualifier(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: qualifiers
 *
 * afw_function_execute_qualifiers
 *
 * See afw_function_bindings.h for more information.
 *
 * This function allows the active qualifiers to be accessed as properties of
 * an object. The value of each of these properties is an object whose
 * properties are the variables for the corresponding qualifier.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function qualifiers(
 *       forTesting?: boolean
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   forTesting - (optional boolean) If specified and true, the object returned
 *       will be suitable to pass as the additionalUntrustedQualifiedVariables
 *       parameter of evaluate*() functions. This is intended for testing
 *       purposes and should not be used in production.
 *
 * Returns:
 *
 *   (object) Each property is the name of a qualifier with a value that is an
 *       object whose properties are the variables of that qualifier. The value
 *       of the variable properties is influenced by the forTesting property.
 */
const afw_value_t *
afw_function_execute_qualifiers(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: safe_evaluate
 *
 * afw_function_execute_safe_evaluate
 *
 * See afw_function_bindings.h for more information.
 *
 * Return the evaluated adaptive value. If an exception occurs, return
 * evaluated error instead.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function safe_evaluate(
 *       value: any,
 *       error: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   value - (any dataType) Value to evaluated.
 *
 *   error - (any dataType) Value to evaluate and return if exception occurs.
 *       If an error occurs evaluating this value, the exception will continue.
 *
 * Returns:
 *
 *   (any dataType) Evaluated adaptive value or error value.
 */
const afw_value_t *
afw_function_execute_safe_evaluate(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: stringify
 *
 * afw_function_execute_stringify
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate and decompile an adaptive value to string. For most values this has
 * the effect of producing a string containing json.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function stringify(
 *       value: any,
 *       replacer?: any,
 *       whitespace?: any
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   value - (any dataType) Value to stringify.
 *
 *   replacer - (optional any dataType) Optional replacer function.
 *
 *   whitespace - (optional any dataType) Add whitespace for readability if
 *       present and not 0. This parameter can be an integer between 0 and 10
 *       or a string that is used for indentation. If 0 is specified, no
 *       whitespace is added to the resulting string. If 1 through 10 is
 *       specified, that number of spaces is used.
 *
 * Returns:
 *
 *   (string) Evaluated and decompiled value.
 */
const afw_value_t *
afw_function_execute_stringify(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: test_expression
 *
 * afw_function_execute_test_expression
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile and evaluate an adaptive expression and compare the results to an
 * expected value. Return object with the test's results.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function test_expression(
 *       id: string,
 *       description: string,
 *       expression: string,
 *       expected: any,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   id - (string) Id of test.
 *
 *   description - (string) Description of test.
 *
 *   expression - (string) Expression to compile and evaluate.
 *
 *   expected - (any dataType) Expected result.
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveHybridPropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (object) Test results.
 */
const afw_value_t *
afw_function_execute_test_expression(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: test_expression_tuple
 *
 * afw_function_execute_test_expression_tuple
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile and evaluate an adaptive expression tuple and compare the results to
 * an expected value. Return object with the test's results.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function test_expression_tuple(
 *       id: string,
 *       description: string,
 *       expression: string,
 *       expected: any,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   id - (string) Id of test.
 *
 *   description - (string) Description of test.
 *
 *   expression - (string) Expression tuple to compile and evaluate.
 *
 *   expected - (any dataType) Expected result.
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveHybridPropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (object) Test results.
 */
const afw_value_t *
afw_function_execute_test_expression_tuple(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: test_hybrid
 *
 * afw_function_execute_test_hybrid
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile and evaluate a string containing adaptive hybrid syntax which can be
 * an adaptive template or adaptive expression tuple and then compare the
 * results to an expected value. Return object with the test's results.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function test_hybrid(
 *       id: string,
 *       description: string,
 *       hybrid: string,
 *       expected: any,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   id - (string) Id of test.
 *
 *   description - (string) Description of test.
 *
 *   hybrid - (string) Hybrid to compile and evaluate.
 *
 *   expected - (any dataType) Expected evaluated result.
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveHybridPropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (object) Test results.
 */
const afw_value_t *
afw_function_execute_test_hybrid(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: test_script
 *
 * afw_function_execute_test_script
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile and evaluate an adaptive script and compare the results to an
 * expected value. Return object with the test's results.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function test_script(
 *       id: string,
 *       description: string,
 *       script: string,
 *       expected: any,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   id - (string) Id of test.
 *
 *   description - (string) Description of test.
 *
 *   script - (string) Script to compile and evaluate.
 *
 *   expected - (any dataType) Expected result.
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveHybridPropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (object) Test results.
 */
const afw_value_t *
afw_function_execute_test_script(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: test_template
 *
 * afw_function_execute_test_template
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile and evaluate an adaptive template and compare the results to an
 * expected value. Return object with the test's results.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function test_template(
 *       id: string,
 *       description: string,
 *       template: string,
 *       expected: any,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   id - (string) Id of test.
 *
 *   description - (string) Description of test.
 *
 *   template - (string) Template to compile and evaluate.
 *
 *   expected - (any dataType) Expected evaluated result.
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveHybridPropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (object) Test results.
 */
const afw_value_t *
afw_function_execute_test_template(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: test_value
 *
 * afw_function_execute_test_value
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate an adaptive value and compare it to an expected value. Return
 * object with the test's results.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function test_value(
 *       id: string,
 *       description: string,
 *       value: string,
 *       expected: any,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   id - (string) Id of test.
 *
 *   description - (string) Description of test.
 *
 *   value - (string) Value to evaluate.
 *
 *   expected - (any dataType) Expected result.
 *
 *   additionalUntrustedQualifiedVariables - (optional object
 *       _AdaptiveHybridPropertiesObjects_) This parameter supplies additional
 *       qualified variables that can be accessed during evaluation. These
 *       variables will not be used by anything that needs to ensure its
 *       qualified variables must come from a trusted source, such as
 *       authorization. This parameter is intended to be used for testing only
 *       and should not be used for anything running in production.
 *
 * Returns:
 *
 *   (object) Test results.
 */
const afw_value_t *
afw_function_execute_test_value(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
