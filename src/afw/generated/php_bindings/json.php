<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for json
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file json.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category json
 */
class json
{
    private $session;

    /**
     * Constructs the json class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * compile_json()
     *
     * Compile json value and return either an unevaluated adaptive value or a
     * string containing the compiler listing.
     *
     * @param json $source json string to compile
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
    public function compile_json(, $source, $listing = null)
    {
        $request = $this->$session->request();

        $request->set("function", "compile<json>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($listing != null)
            $request->set('listing', $listing);

        return $request->get_result();
    }

    /**
     * json()
     *
     * Converts value to data type json returning json result.
     *
     * @param  $value Value to convert
     *
     * @return json Converted value
     */
    public function json(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "json");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
