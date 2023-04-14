<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for null
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category null
 */
class null
{
    private $session;

    /**
     * Constructs the null class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * bag_null()
     *
     * Takes any number of null values and returns a list of list.
     *
     * @param list $values
     *
     * @return list
     */
    public function bag_null(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<null>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_null()
     *
     * This returns the integer number of values in list.
     *
     * @param list $value
     *
     * @return integer
     */
    public function bag_size_null(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<null>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_null()
     *
     * Checks whether value is dataType null and return the boolean result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_null(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<null>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * null()
     *
     * Converts value to data type null returning null result.
     *
     * @param  $value Value to convert
     *
     * @return null Converted value
     */
    public function null(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "null");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_null()
     *
     * Converts null value to string. For list values, the to_string() value
     * for each entry is returned separated with commas.
     *
     * @param null $value A null value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_null(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<null>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
