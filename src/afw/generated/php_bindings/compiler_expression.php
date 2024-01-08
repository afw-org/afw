<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for compiler_expression
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file compiler_expression.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category compiler_expression
 */
class compiler_expression
{
    private $session;

    /**
     * Constructs the compiler_expression class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * nullish_coalescing()
     *
     * Returns the first value of values that is not null or undefined leaving
     * the remaining values unevaluated.
     *
     * @param  $values
     *
     * @return  The first value of values that is not null or undefined.
     */
    public function nullish_coalescing(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "nullish_coalescing");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * optional_chaining()
     *
     * Returns undefined if arg1 is null or undefined without evaluating arg2,
     * but otherwise returns evaluated value of arg2.
     *
     * @param  $arg1
     * @param  $arg2
     *
     * @return  Undefined value if arg1 is null or undefined but otherwise
     *          evaluated arg2.
     */
    public function optional_chaining(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "optional_chaining");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * void_operator()
     *
     * This is the support function for the void operator which evaluates
     * value and returns undefined.
     *
     * @param  $value This is the value to evaluate.
     *
     * @return  This always returns undefined.
     */
    public function void_operator(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "void_operator");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
