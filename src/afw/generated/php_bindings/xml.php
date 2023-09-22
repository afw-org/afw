<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for xml
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category xml
 */
class xml
{
    private $session;

    /**
     * Constructs the xml class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * compile_xml()
     *
     * Compile xml value and return either an unevaluated adaptive value or a
     * string containing the compiler listing.
     *
     * @param xml $source xml string to compile
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
    public function compile_xml(, $source, $listing = null)
    {
        $request = $this->$session->request();

        $request->set("function", "compile<xml>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($listing != null)
            $request->set('listing', $listing);

        return $request->get_result();
    }

    /**
     * xml()
     *
     * Converts value to data type xml returning xml result.
     *
     * @param  $value Value to convert
     *
     * @return xml Converted value
     */
    public function xml(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "xml");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
