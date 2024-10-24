<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for compiler_script
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file compiler_script.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category compiler_script
 */
class compiler_script
{
    private $session;

    /**
     * Constructs the compiler_script class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * assign()
     *
     * Assign a value to the innermost structured block definition of a
     * variable. If the variable is not defined, the variable is defined in
     * the innermost structured block. An error is thrown if not called from
     * an array of values (statements) in a structured function.
     *
     * @param string $name Variable name
     * @param  $value This is the value to assign to the variable.
     *
     * @return  The value assigned.
     */
    public function assign(, $name, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "assign");

        /* pass along required parameters to the request payload */
        $request->set("name", $name);
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * break()
     *
     * This is a special function that can be called to break out of the body
     * of a loop. If called outside of a loop body, an error is thrown.
     *
     * @param  $value The value to evaluate that the enclosing loop will
     *                return. If not specified, the last evaluated value or a
     *                null value will be returned.
     *
     * @return  This function returns from the body of a loop with the last
     *          evaluated value.
     */
    public function break(, $value = null)
    {
        $request = $this->$session->request();

        $request->set("function", "break");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($value != null)
            $request->set('value', $value);

        return $request->get_result();
    }

    /**
     * const()
     *
     * Define one or more statically scoped constants local to the current
     * script block with a permanent value. These constants can be accessed
     * from the current block and inner blocks, but can not be assigned a
     * different value.
     *
     * @param array $name The name of one or more constants to defined in the
     *                    current block.
     * @param  $value This is the value of the constant(s).
     * @param object $type The type of the constant(s).
     *
     * @return  The value assigned.
     */
    public function const(, $name, $value, $type = null)
    {
        $request = $this->$session->request();

        $request->set("function", "const");

        /* pass along required parameters to the request payload */
        $request->set("name", $name);
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        if ($type != null)
            $request->set('type', $type);

        return $request->get_result();
    }

    /**
     * continue()
     *
     * This is a special function that can be called in the body of a loop
     * function to test the condition and, if true, start evaluating the body
     * again. If called outside of a loop body, an error is thrown.
     *
     *
     * @return  This function does not return.
     */
    public function continue()
    {
        $request = $this->$session->request();

        $request->set("function", "continue");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * do_while()
     *
     * This creates a new structured block with a new nested variable scope.
     * 
     * This function will evaluate an array of values (statements) at least
     * once while a condition is true. See the related functions 'break',
     * 'continue', 'return' and 'throw'.
     *
     * @param boolean $condition While this condition is true, the loop will
     *                           continue. This is evaluated in the loop's
     *                           scope.
     * @param array $body This is an array of values (statements) that are
     *                    evaluated for each iteration of the loop. Each value
     *                    in body is evaluated in order until the end of the
     *                    array or until a 'break', 'continue', 'return' or
     *                    'throw' function is encountered.
     *
     * @return  The last value evaluated in body or null if the body is empty.
     */
    public function do_while(, $condition, $body)
    {
        $request = $this->$session->request();

        $request->set("function", "do_while");

        /* pass along required parameters to the request payload */
        $request->set("condition", $condition);
        $request->set("body", $body);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * for()
     *
     * This creates a new structured block with a new nested variable scope.
     * 
     * This function loops while condition is true. If the condition is false
     * for the first iteration, the loop returns a null value.
     *
     * @param array $initial This is an array of values (statements) to
     *                       evaluate before the loop starts. The values will
     *                       normally be a call to the 'assign' function.
     * @param boolean $condition While this condition is true, the loop will
     *                           continue.
     * @param array $increment This is an array of values (statements) to
     *                         evaluate after each iteration of the loop. The
     *                         values will normally be a call to the 'assign'
     *                         function.
     * @param array $body This is an array of values (statements) that are
     *                    evaluated for each iteration of the loop. Each value
     *                    in body is evaluated in order until the end of the
     *                    array or until a 'break', 'continue', 'return' or
     *                    'throw' function is encountered.
     *
     * @return  The last value evaluated in body or null if condition
     *          evaluates to false the first time.
     */
    public function for(, $initial = null, $condition = null, $increment = null, $body = null)
    {
        $request = $this->$session->request();

        $request->set("function", "for");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($initial != null)
            $request->set('initial', $initial);

        if ($condition != null)
            $request->set('condition', $condition);

        if ($increment != null)
            $request->set('increment', $increment);

        if ($body != null)
            $request->set('body', $body);

        return $request->get_result();
    }

    /**
     * for_of()
     *
     * This creates a new structured block with a new nested variable scope.
     * 
     * This function will evaluate an array of values (statements) while a
     * condition is true with initial and increment values. The condition is
     * tested at the beginning of the loop. If the condition is false for the
     * first iteration, the loop returns a null value. This supports for-of
     * statement.
     *
     * @param array $name Variable name(s).
     * @param  $value Any array, object or single value.
     * @param array $body This is an array of values (statements) that are
     *                    evaluated for each iteration of the loop. Each value
     *                    in body is evaluated in order until the end of the
     *                    array or until a 'break', 'continue', 'return' or
     *                    'throw' function is encountered.
     *
     * @return  The last value evaluated in body or null if condition
     *          evaluates to false the first time.
     */
    public function for_of(, $name, $value, $body = null)
    {
        $request = $this->$session->request();

        $request->set("function", "for_of");

        /* pass along required parameters to the request payload */
        $request->set("name", $name);
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        if ($body != null)
            $request->set('body', $body);

        return $request->get_result();
    }

    /**
     * if()
     *
     * Evaluate one of two different values depending on test condition.
     *
     * @param boolean $condition If true, parameter 'then' is evaluated for
     *                           result. If false, parameter 'else' is
     *                           evaluated.
     * @param array $then This is the body of a structured block that is
     *                    evaluated if 'condition' is true. See the 'body'
     *                    parameter of the 'block' function for information on
     *                    how the body is processed.
     * @param array $else This is the body of a structured block that is
     *                    evaluated if 'condition' is false. If not specified
     *                    and condition is false, a null value is returned.
     *                    See the 'body' parameter of the 'block' function for
     *                    information on how the body is processed.
     *
     * @return  The result of evaluating 'then' or 'else'
     */
    public function if(, $condition, $then, $else = null)
    {
        $request = $this->$session->request();

        $request->set("function", "if");

        /* pass along required parameters to the request payload */
        $request->set("condition", $condition);
        $request->set("then", $then);

        /* pass along any optional parameters to the request payload */
        if ($else != null)
            $request->set('else', $else);

        return $request->get_result();
    }

    /**
     * include()
     *
     * Include an external adaptive script to be executed in the current
     * context.
     *
     * @param string $script The name of the script to include
     * @param string $compileType The compile type, used by the parser to
     *                            determine how to compile the data. For
     *                            example, 'json', 'relaxed_json', 'script',
     *                            'template'
     *
     * @return any
     */
    public function include(, $script, $compileType = null)
    {
        $request = $this->$session->request();

        $request->set("function", "include");

        /* pass along required parameters to the request payload */
        $request->set("script", $script);

        /* pass along any optional parameters to the request payload */
        if ($compileType != null)
            $request->set('compileType', $compileType);

        return $request->get_result();
    }

    /**
     * let()
     *
     * Declare one or more statically scoped variable locations local to the
     * current script block and optionally assign them an initial value. These
     * variables can be accessed and assigned different values from the
     * current block and inner blocks.
     *
     * @param array $name The name of one or more variables to declared in the
     *                    current block.
     * @param  $value This is the initial value of the variable(s). If not
     *                specified, the variable will have a value of undefined.
     * @param object $type The type of the variable(s).
     *
     * @return  The value assigned.
     */
    public function let(, $name, $value = null, $type = null)
    {
        $request = $this->$session->request();

        $request->set("function", "let");

        /* pass along required parameters to the request payload */
        $request->set("name", $name);

        /* pass along any optional parameters to the request payload */
        if ($value != null)
            $request->set('value', $value);

        if ($type != null)
            $request->set('type', $type);

        return $request->get_result();
    }

    /**
     * rethrow()
     *
     * This is a special function that can be called to rethrow an error
     * inside of a catch block. If called outside of a catch body, an error is
     * thrown.
     *
     *
     * @return  This function rethrows the current error in a catch block.
     */
    public function rethrow()
    {
        $request = $this->$session->request();

        $request->set("function", "rethrow");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * return()
     *
     * Return from the outermost structured block. If the expression of a
     * lambda function is a block function, this will effectively return from
     * the lambda function. If called outside of a structured block, an error
     * is thrown.
     *
     * @param  $value The value to evaluate that the outermost block will
     *                return. If not specified, the last evaluated value or a
     *                null value will be returned.
     *
     * @return  This function returns from the outermost structured block with
     *          the last evaluated value.
     */
    public function return(, $value = null)
    {
        $request = $this->$session->request();

        $request->set("function", "return");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($value != null)
            $request->set('value', $value);

        return $request->get_result();
    }

    /**
     * switch()
     *
     * Support for switch statement.
     *
     * @param function $predicate The predicate is passed two parameters and
     *                            must return a boolean. The first parameter
     *                            passed is the evaluated value of the value1
     *                            parameter and the second is the value2 from
     *                            a case clause. This predicate will often be
     *                            'eqx' to use the exactly equal function but
     *                            can also be any other function such as
     *                            'regexp_match' or a lambda function.
     * @param  $value1 The first parameter passed to the predicate.
     * @param  $case_clause This is one or more case clauses which are pairs
     *                      of a value2 parameter followed by a statement list
     *                      or undefined parameter. One value2 can be
     *                      undefined to indicate the default case clause.
     * 
     *                      For the first value2 that is undefined or calling
     *                      the predicate returns true, the statement list
     *                      followed by any statement lists of subsequent case
     *                      clauses are executed until a break or return is
     *                      encountered. The predicate is called with value1
     *                      and the case clause's value2.
     *
     * @return
     */
    public function switch(, $predicate, $value1, $case_clause)
    {
        $request = $this->$session->request();

        $request->set("function", "switch");

        /* pass along required parameters to the request payload */
        $request->set("predicate", $predicate);
        $request->set("value1", $value1);
        $request->set("case_clause", $case_clause);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * throw()
     *
     * This throws an error that can be caught by a try/catch block. An error
     * object of object type _AdaptiveError_ will be available in the catch
     * block. Its 'id' property will be set to 'throw'. The other properties
     * set based on the parameters specified and where this function is
     * called.
     *
     * @param string $message This is the message that will be included in the
     *                        _AdaptiveError_ error object available in the
     *                        catch block.
     * @param  $additional Optional additional information that will be
     *                     available as a 'additional' property in the error
     *                     object.
     *
     * @return void
     */
    public function throw(, $message, $additional = null)
    {
        $request = $this->$session->request();

        $request->set("function", "throw");

        /* pass along required parameters to the request payload */
        $request->set("message", $message);

        /* pass along any optional parameters to the request payload */
        if ($additional != null)
            $request->set('additional', $additional);

        return $request->get_result();
    }

    /**
     * try()
     *
     * This creates a new structured block with a new nested variable scope.
     * 
     * This function will evaluate the body statements. If an error is thrown
     * and there is an optional catch, the error will be 'caught' and the
     * associated statements will be evaluated. The optional finally
     * statements are always evaluated after the body and catch statements.
     * See the related functions 'break', 'continue', 'return' and 'throw'.
     *
     * @param array $body This is an array of values (statements) that are
     *                    evaluated. Each value in body is evaluated in order
     *                    until the end of the list or until a 'break',
     *                    'continue', 'return' or 'throw' function is
     *                    encountered.
     * @param array $finally This is an array of values (statements) that are
     *                       evaluated after the try and catch statements even
     *                       if an error occurs. Each value in body is
     *                       evaluated in order until the end of the list or
     *                       until a 'break', 'continue', 'return' or 'throw'
     *                       function is encountered.
     * @param array $catch This is an array of values (statements) that are
     *                     evaluated when an error is thrown while evaluating
     *                     the body. Each value in body is evaluated in order
     *                     until the end of the list or until a 'break',
     *                     'continue', 'return' or 'throw' function is
     *                     encountered.
     * @param object $error The error object thrown. This is only available in
     *                      the catch block. See adaptive object type
     *                      _AdaptiveObjectType_ for details.
     *
     * @return  The last value evaluated in body.
     */
    public function try(, $body, $finally = null, $catch = null, $error = null)
    {
        $request = $this->$session->request();

        $request->set("function", "try");

        /* pass along required parameters to the request payload */
        $request->set("body", $body);

        /* pass along any optional parameters to the request payload */
        if ($finally != null)
            $request->set('finally', $finally);

        if ($catch != null)
            $request->set('catch', $catch);

        if ($error != null)
            $request->set('error', $error);

        return $request->get_result();
    }

    /**
     * while()
     *
     * This creates a new structured block with a new nested variable scope.
     * 
     * This function will evaluate an array of values (statements) while a
     * condition is true. The condition is tested at the beginning of the
     * loop. If the condition is false for the first iteration, the loop
     * returns a null value. See the related functions 'break', 'continue',
     * 'return' and 'throw'.
     *
     * @param boolean $condition While this condition is true, the loop will
     *                           continue. This is evaluated in the loop's
     *                           scope.
     * @param array $body This is an array of values (statements) that are
     *                    evaluated for each iteration of the loop. Each value
     *                    in body is evaluated in order until the end of the
     *                    list or until a 'break', 'continue', 'return' or
     *                    'throw' function is encountered.
     *
     * @return  The last value evaluated in body or null if condition
     *          evaluates to false the first time.
     */
    public function while(, $condition, $body)
    {
        $request = $this->$session->request();

        $request->set("function", "while");

        /* pass along required parameters to the request payload */
        $request->set("condition", $condition);
        $request->set("body", $body);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
