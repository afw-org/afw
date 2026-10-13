<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for dnsName
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file dnsName.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category dnsName
 */
class dnsName
{
    private $session;

    /**
     * Constructs the dnsName class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * array_dnsName()
     *
     * Construct a new array of dnsName from the given values, the same as
     * array() except that the new array is typed: each value, and each entry
     * of a spread array, must be dataType dnsName, and adding a value of
     * another data type to the array later throws. A value of another data
     * type is an error, never converted. This is usually written
     * `array<dnsName>(`...values).
     *
     * @param $values A value of dataType dnsName, or ... followed by an
     *                 expression that results in an array, every entry of
     *                 which is included.
     *
     * @return array The constructed array of dnsName.
     */
    public function array_dnsName(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "array_dnsName");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * as_dnsName()
     *
     * Returns value unchanged if it is dataType dnsName. Otherwise, including
     * when value is undefined, throws an error. It never converts: to get a
     * dnsName from a value of another data type, write a typed literal or
     * call a to_T conversion function where one exists. This is usually
     * written value as dnsName.
     *
     * @param $value Value to assert
     *
     * @return dnsName value, which is dataType dnsName
     */
    public function as_dnsName(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "as_dnsName");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_dnsName()
     *
     * Determine if dnsName arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param dnsName $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function eq_dnsName(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_dnsName()
     *
     * Determine if for dnsName arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param dnsName $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function eqx_dnsName(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_dnsName()
     *
     * Checks whether value is dataType dnsName and returns the boolean
     * result. It is false for undefined. This can also be written
     * `is<dnsName>(value`).
     *
     * @param $value Value to check
     *
     * @return boolean
     */
    public function is_dnsName(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is_dnsName");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_dnsName()
     *
     * Checks whether dnsName value is in array of dnsName array and returns
     * the boolean result. Empty array yields false.
     *
     * @param dnsName $value
     * @param array $array
     *
     * @return boolean
     */
    public function is_in_dnsName(, $value, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_dnsName()
     *
     * Determine if dnsName arg1 is not equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use 'nex'
     * ('!==') instead if you want true to be returned if arg1 and arg2's data
     * type don't match.
     *
     * @param dnsName $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function ne_dnsName(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_dnsName()
     *
     * Determine if for dnsName arg1 is not equal to the value or data type of
     * arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param dnsName $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function nex_dnsName(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only_dnsName()
     *
     * This converts an array of dnsName values that contains one value to a
     * single dnsName value.
     *
     * @param array $array
     *
     * @return dnsName
     */
    public function one_and_only_dnsName(, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * regexp_match_dnsName()
     *
     * Checks whether the regular expression matches anywhere in dnsName value
     * and returns the boolean result. Anchor with ^ and \z to match the whole
     * value.
     *
     * @param dnsName $value
     * @param regexp $regexp The regular expression. A regexp literal (/a+b/
     *                       or "a+b"re), to_regexp(s), or
     *                       `compile<regexp>(r`) is already compiled; any
     *                       other regexp value is compiled for this call.
     *
     * @return boolean
     */
    public function regexp_match_dnsName(, $value, $regexp)
    {
        $request = $this->$session->request();

        $request->set("function", "regexp_match<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_dnsName()
     *
     * Converts dnsName value to string. For array values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param dnsName $value A dnsName value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_dnsName(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
