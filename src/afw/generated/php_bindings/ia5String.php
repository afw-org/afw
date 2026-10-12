<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for ia5String
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file ia5String.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category ia5String
 */
class ia5String
{
    private $session;

    /**
     * Constructs the ia5String class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * array_ia5String()
     *
     * Construct a new array of ia5String from the given values, the same as
     * array() except that the new array is typed: each value, and each entry
     * of a spread array, must be dataType ia5String, and adding a value of
     * another data type to the array later throws. A value of another data
     * type is an error, never converted. This is usually written
     * `array<ia5String>(`...values).
     *
     * @param $values A value of dataType ia5String, or ... followed by an
     *                 expression that results in an array, every entry of
     *                 which is included.
     *
     * @return array The constructed array of ia5String.
     */
    public function array_ia5String(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "array_ia5String");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * as_ia5String()
     *
     * Returns value unchanged if it is dataType ia5String. Otherwise,
     * including when value is undefined, throws an error. It never converts:
     * to get a ia5String from a value of another data type, write a typed
     * literal or call a to_T conversion function where one exists. This is
     * usually written value as ia5String.
     *
     * @param $value Value to assert
     *
     * @return ia5String value, which is dataType ia5String
     */
    public function as_ia5String(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "as_ia5String");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_ia5String()
     *
     * Takes any number of ia5String values and returns an array of ia5String.
     *
     * @param ia5String $values
     *
     * @return array
     */
    public function bag_ia5String(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<ia5String>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_ia5String()
     *
     * This returns the integer number of values in array.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size_ia5String(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<ia5String>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_ia5String()
     *
     * Determine if ia5String arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param ia5String $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function eq_ia5String(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<ia5String>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_ia5String()
     *
     * Determine if for ia5String arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param ia5String $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function eqx_ia5String(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<ia5String>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_ia5String()
     *
     * Checks for ia5String arg1 is greater than or equal to ia5String arg2
     * and return the boolean result.
     *
     * @param ia5String $arg1
     * @param ia5String $arg2
     *
     * @return boolean
     */
    public function ge_ia5String(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<ia5String>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_ia5String()
     *
     * Checks for ia5String arg1 is greater than ia5String arg2 and return the
     * boolean result.
     *
     * @param ia5String $arg1
     * @param ia5String $arg2
     *
     * @return boolean
     */
    public function gt_ia5String(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<ia5String>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_ia5String()
     *
     * Checks whether value is dataType ia5String and returns the boolean
     * result. It is false for undefined. This can also be written
     * `is<ia5String>(value`).
     *
     * @param $value Value to check
     *
     * @return boolean
     */
    public function is_ia5String(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is_ia5String");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_ia5String()
     *
     * Checks for ia5String arg1 is less than or equal to ia5String arg2 and
     * return the boolean result.
     *
     * @param ia5String $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function le_ia5String(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<ia5String>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_ia5String()
     *
     * Checks for ia5String arg1 is less that ia5String arg2 and return the
     * boolean result.
     *
     * @param ia5String $arg1
     * @param ia5String $arg2
     *
     * @return boolean
     */
    public function lt_ia5String(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<ia5String>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_ia5String()
     *
     * Determine if ia5String arg1 is not equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use 'nex'
     * ('!==') instead if you want true to be returned if arg1 and arg2's data
     * type don't match.
     *
     * @param ia5String $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function ne_ia5String(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<ia5String>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_ia5String()
     *
     * Determine if for ia5String arg1 is not equal to the value or data type
     * of arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param ia5String $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function nex_ia5String(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<ia5String>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_ia5String()
     *
     * Converts ia5String value to string. For array values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param ia5String $value A ia5String value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_ia5String(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<ia5String>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
