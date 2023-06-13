// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for adaptive script
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_script.c
 * @brief afw_function_execute_* functions for script.
 */

#include "afw_internal.h"



/*
 * Adaptive function: assign
 *
 * afw_function_execute_assign
 *
 * See afw_function_bindings.h for more information.
 *
 * Assign a value to the innermost structured block definition of a variable.
 * If the variable is not defined, the variable is defined in the innermost
 * structured block. An error is thrown if not called from a list of values in
 * a structured function.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function assign(
 *       name: string,
 *       value: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   name - (string) Variable name.
 *
 *   value - (any dataType) This is the value to assign to the variable.
 *
 * Returns:
 *
 *   (any dataType) The value assigned.
 */
const afw_value_t *
afw_function_execute_assign(
    afw_function_execute_t *x)
{
    /* Only allowed in a block value. See afw_value_block.c */
    AFW_THROW_ERROR_Z(general, "Misplaced assign()", x->xctx);
}



/*
 * Adaptive function: break
 *
 * afw_function_execute_break
 *
 * See afw_function_bindings.h for more information.
 *
 * This is a special function that can be called to break out of the body of a
 * loop. If called outside of a loop body, an error is thrown.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function break(
 *       value?: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   value - (optional any dataType) The value to evaluate that the enclosing
 *       loop will return. If not specified, the last evaluated value or a null
 *       value will be returned.
 *
 * Returns:
 *
 *   (any dataType) This function returns from the body of a loop with the last
 *       evaluated value.
 */
const afw_value_t *
afw_function_execute_break(
    afw_function_execute_t *x)
{
    /* Only allowed in a block value. See afw_value_block.c */
    AFW_THROW_ERROR_Z(general, "Misplaced break()", x->xctx);
}



/*
 * Adaptive function: compile<script>
 *
 * afw_function_execute_compile_script
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile script value and return either an unevaluated adaptive value or a
 * string containing the compiler listing.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function compile<script>(
 *       source: script,
 *       listing?: any
 *   ): unevaluated;
 * ```
 *
 * Parameters:
 *
 *   source - (script) script string to compile.
 *
 *   listing - (optional any dataType) If specified, a compiler listing is
 *       produced instead of an unevaluated expression value.
 *       
 *       This parameter can be an integer between 0 and 10 of a string that is
 *       used for indentation. If 0 is specified, no whitespace is added to the
 *       resulting string. If 1 through 10 is specified, that number of spaces
 *       is used.
 *
 * Returns:
 *
 *   (unevaluated)
 */
const afw_value_t *
afw_function_execute_compile_script(
    afw_function_execute_t *x)
{
    const afw_value_script_t *script;
    const afw_value_t *result;
    const afw_utf8_t *listing;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(script, 1, script);

    result = afw_compile_to_value(
        &script->internal, AFW_FUNCTION_SOURCE_LOCATION,
        afw_compile_type_script,
        NULL, NULL, x->p, x->xctx);

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        listing = afw_function_evaluate_whitespace_parameter(x, 2);
        result = afw_value_create_string(
            afw_value_compiler_listing_to_string(result, listing,
                x->p, x->xctx),
            x->p, x->xctx);
    }

    return result;
}



/*
 * Adaptive function: const
 *
 * afw_function_execute_const
 *
 * See afw_function_bindings.h for more information.
 *
 * Define one or more statically scoped constants local to the current script
 * block with a permanent value. These constants can be accessed from the
 * current block and inner blocks, but can not be assigned a different value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function const(
 *       name: (list string),
 *       value: any,
 *       type?: (object _AdaptiveValueMeta_)
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   name - (list string) The name of one or more constants to defined in the
 *       current block.
 *
 *   value - (any dataType) This is the value of the constant(s).
 *
 *   type - (optional object _AdaptiveValueMeta_) The type of the constant(s).
 *
 * Returns:
 *
 *   (any dataType) The value assigned.
 */
const afw_value_t *
afw_function_execute_const(
    afw_function_execute_t *x)
{
    /* Only allowed in a block value. See afw_value_block.c */
    AFW_THROW_ERROR_Z(general, "Misplaced const()", x->xctx);
}



/*
 * Adaptive function: continue
 *
 * afw_function_execute_continue
 *
 * See afw_function_bindings.h for more information.
 *
 * This is a special function that can be called in the body of a loop function
 * to test the condition and, if true, start evaluating the body again. If
 * called outside of a loop body, an error is thrown.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function continue(
 *   
 *   ): any;
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (any dataType) This function does not return.
 */
const afw_value_t *
afw_function_execute_continue(
    afw_function_execute_t *x)
{
    /* Only allowed in a block value. See afw_value_block.c */
    AFW_THROW_ERROR_Z(general, "Misplaced continue()", x->xctx);
}



/*
 * Adaptive function: do_while
 *
 * afw_function_execute_do_while
 *
 * See afw_function_bindings.h for more information.
 *
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function will evaluate a list of values at least once while a condition
 * is true. See the related functions "break", "continue", and "return".
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function do_while(
 *       condition: boolean,
 *       body: list
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   condition - (boolean) While this condition is true, the loop will
 *       continue. This is evaluated in the loop's scope.
 *
 *   body - (list) This is a list of values that are evaluated for each
 *       iteration of the loop. Each value in body is evaluated in order until
 *       the end of the list or until a "break", "continue" or "return"
 *       function is encountered.
 *
 * Returns:
 *
 *   (any dataType) The last value evaluated in body or null if the body is
 *       empty.
 */
const afw_value_t *
afw_function_execute_do_while(
    afw_function_execute_t *x)
{
    const afw_value_t *result;
    afw_value_block_statement_type_t type;

    /* Can be called outside of a block, but usually part of a block value. */
    result = afw_value_block_evaluate_do_while(x,
        &type, x->argc, x->argv, x->p, x->xctx);

    return result;
}



/*
 * Adaptive function: evaluate<script>
 *
 * afw_function_execute_evaluate_script
 *
 * See afw_function_bindings.h for more information.
 *
 * Compile and evaluate script value.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function evaluate<script>(
 *       source: script,
 *       additionalUntrustedQualifiedVariables?: (object _AdaptiveHybridPropertiesObjects_)
 *   ): unevaluated;
 * ```
 *
 * Parameters:
 *
 *   source - (script) script string to compile and evaluate.
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
 *   (unevaluated)
 */
const afw_value_t *
afw_function_execute_evaluate_script(
    afw_function_execute_t *x)
{
    const afw_value_script_t *script;
    const afw_value_t *compiled;
    const afw_value_t *value;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(script, 1, script);

    compiled = afw_compile_to_value(
        &script->internal, AFW_FUNCTION_SOURCE_LOCATION,
        afw_compile_type_script,
        NULL, NULL, x->p, x->xctx);

    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        value = afw_value_evaluate_with_additional_untrusted_qualified_variables(
            compiled, x->argv[2], x->p, x->xctx);
    }
    else {
        value = afw_value_evaluate(compiled, x->p, x->xctx);
    }

    return value;
}



/*
 * Adaptive function: for
 *
 * afw_function_execute_for
 *
 * See afw_function_bindings.h for more information.
 *
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function loops while condition is true. If the condition is false for
 * the first iteration, the loop returns a null value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function for(
 *       initial?: list,
 *       condition?: boolean,
 *       increment?: list,
 *       body?: list
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   initial - (optional list) This is a list of values to evaluate before the
 *       loop starts. The values will normally be a call to the "assign"
 *       function.
 *
 *   condition - (optional boolean) While this condition is true, the loop will
 *       continue.
 *
 *   increment - (optional list) This is a list of values to evaluate after
 *       each iteration of the loop. The values will normally be a call to the
 *       "assign" function.
 *
 *   body - (optional list) This is a list of values that are evaluated for
 *       each iteration of the loop. Each value in body is evaluated in order
 *       until the end of the list or until a "break", "continue" or "return"
 *       function is encountered.
 *
 * Returns:
 *
 *   (any dataType) The last value evaluated in body or null if condition
 *       evaluates to false the first time.
 */
const afw_value_t *
afw_function_execute_for(
    afw_function_execute_t *x)
{
    const afw_value_t *result;
    afw_value_block_statement_type_t type;

    /* Can be called outside of a block, but usually part of a block value. */
    result = afw_value_block_evaluate_for(x,
        &type, x->argc, x->argv, x->p, x->xctx);
 
    return result;
}



/*
 * Adaptive function: foreach
 *
 * afw_function_execute_foreach
 *
 * See afw_function_bindings.h for more information.
 *
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function will evaluate a list of values while a condition is true with
 * initial and increment values. The condition is tested at the beginning of
 * the loop. If the condition is false for the first iteration, the loop
 * returns a null value.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function foreach(
 *       name: (list string),
 *       value: any,
 *       body?: list
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   name - (list string) Variable name(s).
 *
 *   value - (any dataType) Any list, object or single value.
 *
 *   body - (optional list) This is a list of values that are evaluated for
 *       each iteration of the loop. Each value in body is evaluated in order
 *       until the end of the list or until a "break", "continue" or "return"
 *       function is encountered.
 *
 * Returns:
 *
 *   (any dataType) The last value evaluated in body or null if condition
 *       evaluates to false the first time.
 */
const afw_value_t *
afw_function_execute_foreach(
    afw_function_execute_t *x)
{
    const afw_value_t *result;
    afw_value_block_statement_type_t type;

    /* Can be called outside of a block, but usually part of a block value. */
    result = afw_value_block_evaluate_foreach(x,
        &type, x->argc, x->argv, x->p, x->xctx);

    return result;
}



/*
 * Adaptive function: if
 *
 * afw_function_execute_if
 *
 * See afw_function_bindings.h for more information.
 *
 * Evaluate one of two different values depending on test condition.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function if(
 *       condition: boolean,
 *       then: list,
 *       else?: list
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   condition - (boolean) If true, parameter "then" is evaluated for result.
 *       If false, parameter "else" is evaluated.
 *
 *   then - (list) This is the body of a structured block that is evaluated if
 *       "condition" is true. See the "body" parameter of the "block" function
 *       for information on how the body is processed.
 *
 *   else - (optional list) This is the body of a structured block that is
 *       evaluated if "condition" is false. If not specified and condition is
 *       false, a null value is returned. See the "body" parameter of the
 *       "block" function for information on how the body is processed.
 *
 * Returns:
 *
 *   (any dataType) The result of evaluating "then" or "else".
 */
const afw_value_t *
afw_function_execute_if(
    afw_function_execute_t *x)
{
    const afw_value_t *result;
    afw_value_block_statement_type_t type;

    /* Can be called outside of a block, but usually part of a block value. */
    result = afw_value_block_evaluate_if(x,
        &type, x->argc, x->argv, false, x->p, x->xctx);

    return result;
}



/*
 * Adaptive function: loc
 *
 * afw_function_execute_loc
 *
 * See afw_function_bindings.h for more information.
 *
 * Declare one or more statically scoped variable locations local to the
 * current script block and optionally assign them an initial value. These
 * variables can be accessed and assigned different values from the current
 * block and inner blocks.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function loc(
 *       name: (list string),
 *       value?: any,
 *       type?: (object _AdaptiveValueMeta_)
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   name - (list string) The name of one or more variables to declared in the
 *       current block.
 *
 *   value - (optional any dataType) This is the initial value of the
 *       variable(s). If not specified, the variable will have a value of
 *       undefined.
 *
 *   type - (optional object _AdaptiveValueMeta_) The type of the variable(s).
 *
 * Returns:
 *
 *   (any dataType) The value assigned.
 */
const afw_value_t *
afw_function_execute_loc(
    afw_function_execute_t *x)
{
    /* Only allowed in a block value. See afw_value_block.c */
    AFW_THROW_ERROR_Z(general, "Misplaced loc()", x->xctx);
}



/*
 * Adaptive function: return
 *
 * afw_function_execute_return
 *
 * See afw_function_bindings.h for more information.
 *
 * Return from the outermost structured block. If the expression of a lambda
 * function is a block function, this will effectively return from the lambda
 * function. If called outside of a structured block, an error is thrown.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function return(
 *       value?: any
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   value - (optional any dataType) The value to evaluate that the outermost
 *       block will return. If not specified, the last evaluated value or a
 *       null value will be returned.
 *
 * Returns:
 *
 *   (any dataType) This function returns from the outermost structured block
 *       with the last evaluated value.
 */
const afw_value_t *
afw_function_execute_return(
    afw_function_execute_t *x)
{
    /* Only allowed in a block value. See afw_value_block.c */
    AFW_THROW_ERROR_Z(general, "Misplaced return()", x->xctx);
}



/*
 * Adaptive function: while
 *
 * afw_function_execute_while
 *
 * See afw_function_bindings.h for more information.
 *
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function will evaluate a list of values while a condition is true. The
 * condition is tested at the beginning of the loop. If the condition is false
 * for the first iteration, the loop returns a null value. See the related
 * functions "break", "continue", and "return".
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function while(
 *       condition: boolean,
 *       body: list
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   condition - (boolean) While this condition is true, the loop will
 *       continue. This is evaluated in the loop's scope.
 *
 *   body - (list) This is a list of values that are evaluated for each
 *       iteration of the loop. Each value in body is evaluated in order until
 *       the end of the list or until a "break", "continue" or "return"
 *       function is encountered.
 *
 * Returns:
 *
 *   (any dataType) The last value evaluated in body or null if condition
 *       evaluates to false the first time.
 */
const afw_value_t *
afw_function_execute_while(
    afw_function_execute_t *x)
{
    const afw_value_t *result;
    afw_value_block_statement_type_t type;

    /* Can be called outside of a block, but usually part of a block value. */
    result = afw_value_block_evaluate_while(x,
        &type, x->argc, x->argv, x->p, x->xctx);

    return result;
}