// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for compiler_internal
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_function_compiler_internal.c
 * @brief afw_function_execute_* functions for compiler_internal.
 */

#include "afw.h"



/*
 * Adaptive function: assign
 *
 * afw_function_execute_assign
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Assign a value to the innermost structured block definition of a variable. If
 * the variable is not defined, the variable is defined in the innermost
 * structured block. An error is thrown if not called from an array of values
 * (statements) in a structured function.
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
 *   value - (any) This is the value to assign to the variable.
 *
 * Returns:
 *
 *   (any) The value assigned.
 */
const afw_value_t *
afw_function_execute_assign(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: break
 *
 * afw_function_execute_break
 *
 * See afw_function_bindings_internal.h for more information.
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
 *   value - (optional any) The value to evaluate that the enclosing loop will
 *       return. If not specified, the last evaluated value or a null value will
 *       be returned.
 *
 * Returns:
 *
 *   (any) This function returns from the body of a loop with the last evaluated
 *       value.
 */
const afw_value_t *
afw_function_execute_break(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: const
 *
 * afw_function_execute_const
 *
 * See afw_function_bindings_internal.h for more information.
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
 *       name: string[],
 *       value: any,
 *       type?: object // _AdaptiveValueMeta_
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   name - (string[]) The name of one or more constants to defined in the
 *       current block.
 *
 *   value - (any) This is the value of the constant(s).
 *
 *   type - (optional object _AdaptiveValueMeta_) The type of the constant(s).
 *
 * Returns:
 *
 *   (any) The value assigned.
 */
const afw_value_t *
afw_function_execute_const(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: continue
 *
 * afw_function_execute_continue
 *
 * See afw_function_bindings_internal.h for more information.
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
 *   ): any;
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (any) This function does not return.
 */
const afw_value_t *
afw_function_execute_continue(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: do_while
 *
 * afw_function_execute_do_while
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function will evaluate an array of values (statements) at least once
 * while a condition is true. See the related functions 'break', 'continue',
 * 'return' and 'throw'.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function do_while(
 *       condition: boolean,
 *       body: array
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   condition - (boolean) While this condition is true, the loop will continue.
 *       This is evaluated in the loop's scope.
 *
 *   body - (array) This is an array of values (statements) that are evaluated
 *       for each iteration of the loop. Each value in body is evaluated in
 *       order until the end of the array or until a 'break', 'continue',
 *       'return' or 'throw' function is encountered.
 *
 * Returns:
 *
 *   (any) The last value evaluated in body or null if the body is empty.
 */
const afw_value_t *
afw_function_execute_do_while(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: for
 *
 * afw_function_execute_for
 *
 * See afw_function_bindings_internal.h for more information.
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
 *       initial?: array,
 *       condition?: boolean,
 *       increment?: array,
 *       body?: array
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   initial - (optional array) This is an array of values (statements) to
 *       evaluate before the loop starts. The values will normally be a call to
 *       the 'assign' function.
 *
 *   condition - (optional boolean) While this condition is true, the loop will
 *       continue.
 *
 *   increment - (optional array) This is an array of values (statements) to
 *       evaluate after each iteration of the loop. The values will normally be
 *       a call to the 'assign' function.
 *
 *   body - (optional array) This is an array of values (statements) that are
 *       evaluated for each iteration of the loop. Each value in body is
 *       evaluated in order until the end of the array or until a 'break',
 *       'continue', 'return' or 'throw' function is encountered.
 *
 * Returns:
 *
 *   (any) The last value evaluated in body or null if condition evaluates to
 *       false the first time.
 */
const afw_value_t *
afw_function_execute_for(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: for_of
 *
 * afw_function_execute_for_of
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function will evaluate an array of values (statements) while a condition
 * is true with initial and increment values. The condition is tested at the
 * beginning of the loop. If the condition is false for the first iteration, the
 * loop returns a null value. This supports for-of statement.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function for_of(
 *       name: string[],
 *       value: any,
 *       body?: array
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   name - (string[]) Variable name(s).
 *
 *   value - (any) Any array, object or single value.
 *
 *   body - (optional array) This is an array of values (statements) that are
 *       evaluated for each iteration of the loop. Each value in body is
 *       evaluated in order until the end of the array or until a 'break',
 *       'continue', 'return' or 'throw' function is encountered.
 *
 * Returns:
 *
 *   (any) The last value evaluated in body or null if condition evaluates to
 *       false the first time.
 */
const afw_value_t *
afw_function_execute_for_of(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: if
 *
 * afw_function_execute_if
 *
 * See afw_function_bindings_internal.h for more information.
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
 *       then: array,
 *       else?: array
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   condition - (boolean) If true, parameter 'then' is evaluated for result. If
 *       false, parameter 'else' is evaluated.
 *
 *   then - (array) This is the body of a structured block that is evaluated if
 *       'condition' is true. See the 'body' parameter of the 'block' function
 *       for information on how the body is processed.
 *
 *   else - (optional array) This is the body of a structured block that is
 *       evaluated if 'condition' is false. If not specified and condition is
 *       false, a null value is returned. See the 'body' parameter of the
 *       'block' function for information on how the body is processed.
 *
 * Returns:
 *
 *   (any) The result of evaluating 'then' or 'else'.
 */
const afw_value_t *
afw_function_execute_if(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: let
 *
 * afw_function_execute_let
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Declare one or more statically scoped variable locations local to the current
 * script block and optionally assign them an initial value. These variables can
 * be accessed and assigned different values from the current block and inner
 * blocks.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function let(
 *       name: string[],
 *       value?: any,
 *       type?: object // _AdaptiveValueMeta_
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   name - (string[]) The name of one or more variables to declared in the
 *       current block.
 *
 *   value - (optional any) This is the initial value of the variable(s). If not
 *       specified, the variable will have a value of undefined.
 *
 *   type - (optional object _AdaptiveValueMeta_) The type of the variable(s).
 *
 * Returns:
 *
 *   (any) The value assigned.
 */
const afw_value_t *
afw_function_execute_let(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: rethrow
 *
 * afw_function_execute_rethrow
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This is a special function that can be called to rethrow an error inside of a
 * catch block. If called outside of a catch body, an error is thrown.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function rethrow(
 *   ): any;
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (any) This function rethrows the current error in a catch block.
 */
const afw_value_t *
afw_function_execute_rethrow(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: return
 *
 * afw_function_execute_return
 *
 * See afw_function_bindings_internal.h for more information.
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
 *   value - (optional any) The value to evaluate that the outermost block will
 *       return. If not specified, the last evaluated value or a null value will
 *       be returned.
 *
 * Returns:
 *
 *   (any) This function returns from the outermost structured block with the
 *       last evaluated value.
 */
const afw_value_t *
afw_function_execute_return(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: switch
 *
 * afw_function_execute_switch
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Support for switch statement.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function switch(
 *       predicate: (value1: any, value2: any) => boolean,
 *       value1: any,
 *       case_clause_1: any,
 *       case_clause_2: any,
 *       ...case_clause_rest: any[]
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   predicate - ((value1: any, value2: any) => boolean) The predicate is passed
 *       two parameters and must return a boolean. The first parameter passed is
 *       the evaluated value of the value1 parameter and the second is the
 *       value2 from a case clause. This predicate will often be 'eqx' to use
 *       the exactly equal function but can also be any other function such as
 *       'regexp_match' or a lambda function.
 *
 *   value1 - (any) The first parameter passed to the predicate.
 *
 *   case_clause - (2 or more any) This is one or more case clauses which are
 *       pairs of a value2 parameter followed by a statement list or undefined
 *       parameter. One value2 can be undefined to indicate the default case
 *       clause.
 * 
 *       For the first value2 that is undefined or calling the predicate returns
 *       true, the statement list followed by any statement lists of subsequent
 *       case clauses are executed until a break or return is encountered. The
 *       predicate is called with value1 and the case clause's value2.
 *
 * Returns:
 *
 *   (any)
 */
const afw_value_t *
afw_function_execute_switch(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: throw
 *
 * afw_function_execute_throw
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This throws an error that can be caught by a try/catch block. An error object
 * of object type _AdaptiveError_ will be available in the catch block. Its 'id'
 * property is 'throw' unless the optional id parameter is supplied. Optional
 * data is available as the 'data' property. The other properties are set based
 * on the parameters specified and where this function is called.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function throw(
 *       message: string,
 *       data?: any,
 *       id?: string
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   message - (string) This is the message that will be included in the
 *       _AdaptiveError_ error object available in the catch block.
 *
 *   data - (optional any) Optional data that will be available as the 'data'
 *       property of the _AdaptiveError_ object in the catch block.
 *
 *   id - (optional string) Optional error id (mnemonic) to use instead of
 *       'throw'. Must be a name allowed on script throw (request-facing HTTP
 *       names such as not_found, denied, gone, too_many_requests). Sets the id
 *       property of the catch object and the HTTP status if the error is not
 *       caught.
 *
 * Returns:
 *
 *   (void)
 *
 * Errors thrown:
 *
 *   throw - Default when id is omitted
 *   not_found - id is not_found
 *   denied - id is denied
 *   authentication_required - id is authentication_required
 *   conflict - id is conflict
 *   bad_request - id is bad_request
 *   read_only - id is read_only
 *   payload_too_large - id is payload_too_large
 *   query_too_complex - id is query_too_complex
 *   method_not_allowed - id is method_not_allowed
 *   gone - id is gone
 *   too_many_requests - id is too_many_requests
 *   unprocessable_content - id is unprocessable_content
 *   moved_permanently - id is moved_permanently
 *   argument_error - id is not a string or is not allowed on throw
 */
const afw_value_t *
afw_function_execute_throw(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: try
 *
 * afw_function_execute_try
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function will evaluate the body statements. If an error is thrown and
 * there is an optional catch, the error will be 'caught' and the associated
 * statements will be evaluated. The optional finally statements are always
 * evaluated after the body and catch statements. See the related functions
 * 'break', 'continue', 'return' and 'throw'.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function try(
 *       body: array,
 *       finally?: array,
 *       catch?: array,
 *       error?: object // _AdaptiveObjectType_
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   body - (array) This is an array of values (statements) that are evaluated.
 *       Each value in body is evaluated in order until the end of the list or
 *       until a 'break', 'continue', 'return' or 'throw' function is
 *       encountered.
 *
 *   finally - (optional array) This is an array of values (statements) that are
 *       evaluated after the try and catch statements even if an error occurs.
 *       Each value in body is evaluated in order until the end of the list or
 *       until a 'break', 'continue', 'return' or 'throw' function is
 *       encountered.
 *
 *   catch - (optional array) This is an array of values (statements) that are
 *       evaluated when an error is thrown while evaluating the body. Each value
 *       in body is evaluated in order until the end of the list or until a
 *       'break', 'continue', 'return' or 'throw' function is encountered.
 *
 *   error - (optional object _AdaptiveObjectType_) The error object thrown.
 *       This is only available in the catch block. See adaptive object type
 *       _AdaptiveObjectType_ for details.
 *
 * Returns:
 *
 *   (any) The last value evaluated in body.
 */
const afw_value_t *
afw_function_execute_try(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: while
 *
 * afw_function_execute_while
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * This creates a new structured block with a new nested variable scope.
 * 
 * This function will evaluate an array of values (statements) while a condition
 * is true. The condition is tested at the beginning of the loop. If the
 * condition is false for the first iteration, the loop returns a null value.
 * See the related functions 'break', 'continue', 'return' and 'throw'.
 *
 * This function is pure, so it will always return the same result
 * given exactly the same parameters and has no side effects.
 *
 * Declaration:
 *
 * ```
 *   function while(
 *       condition: boolean,
 *       body: array
 *   ): any;
 * ```
 *
 * Parameters:
 *
 *   condition - (boolean) While this condition is true, the loop will continue.
 *       This is evaluated in the loop's scope.
 *
 *   body - (array) This is an array of values (statements) that are evaluated
 *       for each iteration of the loop. Each value in body is evaluated in
 *       order until the end of the list or until a 'break', 'continue',
 *       'return' or 'throw' function is encountered.
 *
 * Returns:
 *
 *   (any) The last value evaluated in body or null if condition evaluates to
 *       false the first time.
 */
const afw_value_t *
afw_function_execute_while(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: wrap_literal_array
 *
 * afw_function_execute_wrap_literal_array
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Evaluate an array value, create a memory array wrapper
 * (afw_array_create_wrapper_*) over its instance, and return that wrapper as an
 * array value. Entry mutators stay on the face; nested objects/arrays are
 * promoted on get. Intended for compile/runtime isolation of array literals
 * (issue #17); not normal author surface syntax.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function wrap_literal_array(
 *       array: array
 *   ): array;
 * ```
 *
 * Parameters:
 *
 *   array - (array) Array to evaluate and wrap (typically a constant array
 *       literal once the compiler emits isolation).
 *
 * Returns:
 *
 *   (array) A new memory-wrapper array face over the evaluated base.
 */
const afw_value_t *
afw_function_execute_wrap_literal_array(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}



/*
 * Adaptive function: wrap_literal_object
 *
 * afw_function_execute_wrap_literal_object
 *
 * See afw_function_bindings_internal.h for more information.
 *
 * Evaluate an object value, create a memory object wrapper
 * (afw_object_create_wrapper_*) over its instance, and return that wrapper as
 * an object value. Local property sets stay on the face; gets look through to
 * the shared base. Intended for compile/runtime isolation of object literals
 * (issue #17); not normal author surface syntax.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function wrap_literal_object(
 *       object: object
 *   ): object;
 * ```
 *
 * Parameters:
 *
 *   object - (object) Object to evaluate and wrap (typically a constant object
 *       literal once the compiler emits isolation).
 *
 * Returns:
 *
 *   (object) A new memory-wrapper object face over the evaluated base.
 */
const afw_value_t *
afw_function_execute_wrap_literal_object(
    afw_function_execute_t *x)
{
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
