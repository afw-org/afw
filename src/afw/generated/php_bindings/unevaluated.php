<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for unevaluated
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file unevaluated.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category unevaluated
 */
class unevaluated
{
    private $session;

    /**
     * Constructs the unevaluated class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * as_unevaluated()
     *
     * Returns value unchanged if it is dataType unevaluated. Otherwise,
     * including when value is undefined, throws an error. It never converts:
     * to get a unevaluated from a value of another data type, write a typed
     * literal or call a to_T conversion function where one exists. This is
     * usually written value as unevaluated.
     *
     * @param $value Value to assert
     *
     * @return unevaluated value, which is dataType unevaluated
     */
    public function as_unevaluated(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "as_unevaluated");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_unevaluated()
     *
     * Checks whether value is dataType unevaluated and returns the boolean
     * result. It is false for undefined. This can also be written
     * `is<unevaluated>(value`).
     *
     * @param $value Value to check
     *
     * @return boolean
     */
    public function is_unevaluated(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is_unevaluated");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
