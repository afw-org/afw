<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for script
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category script
 */
class script
{
    private $session;

    /**
     * Constructs the script class, using a session.
     *
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
     * a list of values (statements) in a structured function.
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
     * bag_script()
     *
     * Takes any number of script values and returns a list of list.
     *
     * @param list $values
     *
     * @return list
     */
    public function bag_script(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<script>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_script()
     *
     * This returns the integer number of values in list.
     *
     * @param list $value
     *
     * @return integer
     */
    public function bag_size_script(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<script>");

        /* pass along required parameters to the request payload */
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
     * compile_script()
     *
     * Compile script value and return either an unevaluated adaptive value
     * or a string containing the compiler listing.
     *
     * @param script $source script string to compile
     * @param  $listing If specified, a compiler listing is produced instead
     *                  of an unevaluated expression value.
     *                  
     *                  This parameter can be an integer between 0 and 10 of
     *                  a string that is used for indentation. If 0 is
     *                  specified, no whitespace is added to the resulting
     *                  string. If 1 through 10 is specified, that number of
     *                  spaces is used.
     *
     * @return unevaluated
     */
    public function compile_script(, $source, $listing = null)
    {
        $request = $this->$session->request();

        $request->set("function", "compile<script>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($listing != null)
            $request->set('listing', $listing);

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
     * @param list $name The name of one or more constants to defined in the
     *                   current block.
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
     * This function will evaluate a list of values (statements) at least
     * once while a condition is true. See the related functions "break",
     * "continue", "return" and "throw".
     *
     * @param boolean $condition While this condition is true, the loop will
     *                           continue. This is evaluated in the loop's
     *                           scope.
     * @param list $body This is a list of values (statements) that are
     *                   evaluated for each iteration of the loop. Each value
     *                   in body is evaluated in order until the end of the
     *                   list or until a "break", "continue", "return" or
     *                   "throw" function is encountered.
     *
     * @return  The last value evaluated in body or null if the body is
     *          empty.
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
     * eq_script()
     *
     * Determine if script arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use "eqx"
     * ("===") instead if you want false to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param script $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_script(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<script>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_script()
     *
     * Determine if for script arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use "eq" ("==") instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param script $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_script(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<script>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * evaluate_script()
     *
     * Compile and evaluate script value.
     *
     * @param script $source script string to compile and evaluate
     * @param object $additionalUntrustedQualifiedVariables This parameter
     *                                                      supplies
     *                                                      additional
     *                                                      qualified
     *                                                      variables that
     *                                                      can be accessed
     *                                                      during
     *                                                      evaluation. These
     *                                                      variables will
     *                                                      not be used by
     *                                                      anything that
     *                                                      needs to ensure
     *                                                      its qualified
     *                                                      variables must
     *                                                      come from a
     *                                                      trusted source,
     *                                                      such as
     *                                                      authorization.
     *                                                      This parameter is
     *                                                      intended to be
     *                                                      used for testing
     *                                                      only and should
     *                                                      not be used for
     *                                                      anything running
     *                                                      in production.
     *
     * @return unevaluated
     */
    public function evaluate_script(, $source, $additionalUntrustedQualifiedVariables = null)
    {
        $request = $this->$session->request();

        $request->set("function", "evaluate<script>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($additionalUntrustedQualifiedVariables != null)
            $request->set('additionalUntrustedQualifiedVariables', $additionalUntrustedQualifiedVariables);

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
     * @param list $initial This is a list of values (statements) to evaluate
     *                      before the loop starts. The values will normally
     *                      be a call to the "assign" function.
     * @param boolean $condition While this condition is true, the loop will
     *                           continue.
     * @param list $increment This is a list of values (statements) to
     *                        evaluate after each iteration of the loop. The
     *                        values will normally be a call to the "assign"
     *                        function.
     * @param list $body This is a list of values (statements) that are
     *                   evaluated for each iteration of the loop. Each value
     *                   in body is evaluated in order until the end of the
     *                   list or until a "break", "continue", "return" or
     *                   "throw" function is encountered.
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
     * foreach()
     *
     * This creates a new structured block with a new nested variable scope.
     * 
     * This function will evaluate a list of values (statements) while a
     * condition is true with initial and increment values. The condition is
     * tested at the beginning of the loop. If the condition is false for the
     * first iteration, the loop returns a null value.
     *
     * @param list $name Variable name(s).
     * @param  $value Any list, object or single value.
     * @param list $body This is a list of values (statements) that are
     *                   evaluated for each iteration of the loop. Each value
     *                   in body is evaluated in order until the end of the
     *                   list or until a "break", "continue", "return" or
     *                   "throw" function is encountered.
     *
     * @return  The last value evaluated in body or null if condition
     *          evaluates to false the first time.
     */
    public function foreach(, $name, $value, $body = null)
    {
        $request = $this->$session->request();

        $request->set("function", "foreach");

        /* pass along required parameters to the request payload */
        $request->set("name", $name);
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        if ($body != null)
            $request->set('body', $body);

        return $request->get_result();
    }

    /**
     * ge_script()
     *
     * Checks for script arg1 is greater than or equal to script arg2 and
     * return the boolean result.
     *
     * @param script $arg1
     * @param script $arg2
     *
     * @return boolean
     */
    public function ge_script(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<script>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_script()
     *
     * Checks for script arg1 is greater than script arg2 and return the
     * boolean result.
     *
     * @param script $arg1
     * @param script $arg2
     *
     * @return boolean
     */
    public function gt_script(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<script>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * if()
     *
     * Evaluate one of two different values depending on test condition.
     *
     * @param boolean $condition If true, parameter "then" is evaluated for
     *                           result. If false, parameter "else" is
     *                           evaluated.
     * @param list $then This is the body of a structured block that is
     *                   evaluated if "condition" is true. See the "body"
     *                   parameter of the "block" function for information on
     *                   how the body is processed.
     * @param list $else This is the body of a structured block that is
     *                   evaluated if "condition" is false. If not specified
     *                   and condition is false, a null value is returned.
     *                   See the "body" parameter of the "block" function for
     *                   information on how the body is processed.
     *
     * @return  The result of evaluating "then" or "else"
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
     * is_script()
     *
     * Checks whether value is dataType script and return the boolean result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_script(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<script>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_script()
     *
     * Checks for script arg1 is less than or equal to script arg2 and return
     * the boolean result.
     *
     * @param script $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_script(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<script>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * let()
     *
     * Declare one or more statically scoped variable locations local to the
     * current script block and optionally assign them an initial value.
     * These variables can be accessed and assigned different values from the
     * current block and inner blocks.
     *
     * @param list $name The name of one or more variables to declared in the
     *                   current block.
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
     * lt_script()
     *
     * Checks for script arg1 is less that script arg2 and return the boolean
     * result.
     *
     * @param script $arg1
     * @param script $arg2
     *
     * @return boolean
     */
    public function lt_script(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<script>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_script()
     *
     * Determine if script arg1 is not equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use "nex"
     * ("!==") instead if you want true to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param script $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_script(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<script>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_script()
     *
     * Determine if for script arg1 is not equal to the value or data type of
     * arg2 then return the boolean result. Use "ne" ("!=") instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param script $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_script(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<script>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * rethrow()
     *
     * This is a special function that can be called to rethrow an error
     * inside of a catch block. If called outside of a catch body, an error
     * is thrown.
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
     * @return  This function returns from the outermost structured block
     *          with the last evaluated value.
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
     * script()
     *
     * Converts value to data type script returning script result.
     *
     * @param  $value Value to convert
     *
     * @return script Converted value
     */
    public function script(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "script");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
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
     *                            "eqx" to use the exactly equal function but
     *                            can also be any other function such as
     *                            "regexp_match" or a lambda function.
     * @param  $value1 The first parameter passed to the predicate.
     * @param  $case_clause This is one or more case clauses which are pairs
     *                      of a value2 parameter followed by a statement
     *                      list or undefined parameter. One value2 can be
     *                      undefined to indicate the default case clause.
     *                      
     *                      For the first value2 that is undefined or calling
     *                      the predicate returns true, the statement list
     *                      followed by any statement lists of subsequent
     *                      case clauses are executed until a break or return
     *                      is encountered. The predicate is called with
     *                      value1 and the case clause's value2.
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
     * block. Its "id" property will be set to "throw". The other properties
     * set based on the parameters specified and where this function is
     * called.
     *
     * @param string $message This is the message that will be included in
     *                        the _AdaptiveError_ error object available in
     *                        the catch block.
     * @param  $additional Optional additional information that will be
     *                     available as a "additional" property in the error
     *                     object.
     *
     * @return null
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
     * and there is an optional catch, the error will be "caught" and the
     * associated statements will be evaluated. The optional finally
     * statements are always evaluated after the body and catch statements.
     * See the related functions "break", "continue", "return" and "throw".
     *
     * @param list $body This is a list of values (statements) that are
     *                   evaluated. Each value in body is evaluated in order
     *                   until the end of the list or until a "break",
     *                   "continue", "return" or "throw" function is
     *                   encountered.
     * @param list $finally This is a list of values (statements) that are
     *                      evaluated after the try and catch statements even
     *                      if an error occurs. Each value in body is
     *                      evaluated in order until the end of the list or
     *                      until a "break", "continue", "return" or "throw"
     *                      function is encountered.
     * @param list $catch This is a list of values (statements) that are
     *                    evaluated when an error is thrown while evaluating
     *                    the body. Each value in body is evaluated in order
     *                    until the end of the list or until a "break",
     *                    "continue", "return" or "throw" function is
     *                    encountered.
     * @param object $error The error object thrown. This is only available
     *                      in the catch block. See adaptive object type
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
     * This function will evaluate a list of values (statements) while a
     * condition is true. The condition is tested at the beginning of the
     * loop. If the condition is false for the first iteration, the loop
     * returns a null value. See the related functions "break", "continue",
     * "return" and "throw".
     *
     * @param boolean $condition While this condition is true, the loop will
     *                           continue. This is evaluated in the loop's
     *                           scope.
     * @param list $body This is a list of values (statements) that are
     *                   evaluated for each iteration of the loop. Each value
     *                   in body is evaluated in order until the end of the
     *                   list or until a "break", "continue", "return" or
     *                   "throw" function is encountered.
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
