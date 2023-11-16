<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for unevaluated
 *
 * Copyright (c) 2010-2023 Clemson University
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
     * is_unevaluated()
     *
     * Checks whether value is dataType unevaluated and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_unevaluated(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<unevaluated>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
