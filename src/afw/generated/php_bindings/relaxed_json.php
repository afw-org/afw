<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for relaxed_json
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file relaxed_json.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category relaxed_json
 */
class relaxed_json
{
    private $session;

    /**
     * Constructs the relaxed_json class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * compile_relaxed_json()
     *
     * Compile relaxed_json value and return either an unevaluated adaptive
     * value or a string containing the compiler listing.
     *
     * @param relaxed_json $source relaxed_json string to compile
     * @param  $listing If specified, a compiler listing is produced instead
     *                  of an unevaluated compiled value.
     * 
     *                  This parameter can be an integer between 0 and 10 of a
     *                  string that is used for indentation. If 0 is
     *                  specified, no whitespace is added to the resulting
     *                  string. If 1 through 10 is specified, that number of
     *                  spaces is used.
     *
     * @return unevaluated
     */
    public function compile_relaxed_json(, $source, $listing = null)
    {
        $request = $this->$session->request();

        $request->set("function", "compile<relaxed_json>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($listing != null)
            $request->set('listing', $listing);

        return $request->get_result();
    }

    /**
     * relaxed_json()
     *
     * Converts value to data type relaxed_json returning relaxed_json result.
     *
     * @param  $value Value to convert
     *
     * @return relaxed_json Converted value
     */
    public function relaxed_json(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "relaxed_json");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
