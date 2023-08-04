<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for logical
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category logical
 */
class logical
{
    private $session;

    /**
     * Constructs the logical class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * and()
     *
     * Evaluates 0 or more boolean conditions returning boolean true if there
     * are no conditions and boolean false if any condition evaluate to
     * false. All conditions after the first false remain unevaluated.
     *
     * @param boolean $conditions
     *
     * @return boolean
     */
    public function and(, $conditions)
    {
        $request = $this->$session->request();

        $request->set("function", "and");

        /* pass along required parameters to the request payload */
        $request->set("conditions", $conditions);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * n_of()
     *
     * integer n specifies the number of boolean conditions that follow that
     * must evaluate to true for boolean true to be returned. If n is 0, true
     * is returned. Once n conditions evaluate to true, true is returned and
     * the remaining conditions remain unevaluated.
     *
     * @param integer $n
     * @param boolean $conditions
     *
     * @return boolean
     */
    public function n_of(, $n, $conditions)
    {
        $request = $this->$session->request();

        $request->set("function", "n_of");

        /* pass along required parameters to the request payload */
        $request->set("n", $n);
        $request->set("conditions", $conditions);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * not()
     *
     * Evaluates boolean condition returning boolean true if condition
     * evaluates to false and false if condition evaluates to true.
     *
     * @param boolean $condition
     *
     * @return boolean
     */
    public function not(, $condition)
    {
        $request = $this->$session->request();

        $request->set("function", "not");

        /* pass along required parameters to the request payload */
        $request->set("condition", $condition);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * or()
     *
     * Evaluates 0 or more boolean conditions returning boolean false if
     * there are no conditions and boolean true if any condition evaluate to
     * true. All conditions after the first true remain unevaluated.
     *
     * @param boolean $conditions
     *
     * @return boolean
     */
    public function or(, $conditions)
    {
        $request = $this->$session->request();

        $request->set("function", "or");

        /* pass along required parameters to the request payload */
        $request->set("conditions", $conditions);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
