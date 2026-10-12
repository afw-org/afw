<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for null
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file null.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category null
 */
class null
{
    private $session;

    /**
     * Constructs the null class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * array_null()
     *
     * Construct a new array of null from the given values, the same as
     * array() except that the new array is typed: each value, and each entry
     * of a spread array, must be dataType null, and adding a value of another
     * data type to the array later throws. A value of another data type is an
     * error, never converted. This is usually written
     * `array<null>(`...values).
     *
     * @param $values A value of dataType null, or ... followed by an
     *                 expression that results in an array, every entry of
     *                 which is included.
     *
     * @return array The constructed array of null.
     */
    public function array_null(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "array_null");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * as_null()
     *
     * Returns value unchanged if it is dataType null. Otherwise, including
     * when value is undefined, throws an error. It never converts: to get a
     * null from a value of another data type, write a typed literal or call a
     * to_T conversion function where one exists. This is usually written
     * value as null.
     *
     * @param $value Value to assert
     *
     * @return null value, which is dataType null
     */
    public function as_null(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "as_null");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_null()
     *
     * Takes any number of null values and returns an array of null.
     *
     * @param array $values
     *
     * @return array
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
     * This returns the integer number of values in array.
     *
     * @param array $value
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
     * Checks whether value is dataType null and returns the boolean result.
     * It is false for undefined. This can also be written `is<null>(value`).
     *
     * @param $value Value to check
     *
     * @return boolean
     */
    public function is_null(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is_null");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_null()
     *
     * Converts null value to string. For array values, the to_string() value
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
