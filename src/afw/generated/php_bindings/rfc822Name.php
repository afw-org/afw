<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for rfc822Name
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file rfc822Name.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category rfc822Name
 */
class rfc822Name
{
    private $session;

    /**
     * Constructs the rfc822Name class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * array_rfc822Name()
     *
     * Construct a new array of rfc822Name from the given values, the same as
     * array() except that the new array is typed: each value, and each entry
     * of a spread array, must be dataType rfc822Name, and adding a value of
     * another data type to the array later throws. A value of another data
     * type is an error, never converted. This is usually written
     * `array<rfc822Name>(`...values).
     *
     * @param $values A value of dataType rfc822Name, or ... followed by an
     *                 expression that results in an array, every entry of
     *                 which is included.
     *
     * @return array The constructed array of rfc822Name.
     */
    public function array_rfc822Name(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "array_rfc822Name");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * as_rfc822Name()
     *
     * Returns value unchanged if it is dataType rfc822Name. Otherwise,
     * including when value is undefined, throws an error. It never converts:
     * to get a rfc822Name from a value of another data type, write a typed
     * literal or call a to_T conversion function where one exists. This is
     * usually written value as rfc822Name.
     *
     * @param $value Value to assert
     *
     * @return rfc822Name value, which is dataType rfc822Name
     */
    public function as_rfc822Name(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "as_rfc822Name");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * at_least_one_member_of_rfc822Name()
     *
     * Returns boolean true if at least one value in rfc822Name array1 is in
     * rfc822Name array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function at_least_one_member_of_rfc822Name(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_rfc822Name()
     *
     * Determine if rfc822Name arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param rfc822Name $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function eq_rfc822Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_rfc822Name()
     *
     * Determine if for rfc822Name arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param rfc822Name $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function eqx_rfc822Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * intersection_rfc822Name()
     *
     * Returns an array of rfc822Name with the values that are common to both
     * array of rfc822Name array1 and array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return array
     */
    public function intersection_rfc822Name(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_rfc822Name()
     *
     * Checks whether rfc822Name value is in array of rfc822Name array and
     * returns the boolean result. Empty array yields false.
     *
     * @param rfc822Name $value
     * @param array $array
     *
     * @return boolean
     */
    public function is_in_rfc822Name(, $value, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_rfc822Name()
     *
     * Checks whether value is dataType rfc822Name and returns the boolean
     * result. It is false for undefined. This can also be written
     * `is<rfc822Name>(value`).
     *
     * @param $value Value to check
     *
     * @return boolean
     */
    public function is_rfc822Name(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is_rfc822Name");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_rfc822Name()
     *
     * Determine if rfc822Name arg1 is not equal to the value of arg2
     * converted to the data type of arg1 then return the boolean result. Use
     * 'nex' ('!==') instead if you want true to be returned if arg1 and
     * arg2's data type don't match.
     *
     * @param rfc822Name $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function ne_rfc822Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_rfc822Name()
     *
     * Determine if for rfc822Name arg1 is not equal to the value or data type
     * of arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param rfc822Name $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function nex_rfc822Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only_rfc822Name()
     *
     * This converts an array of rfc822Name values that contains one value to
     * a single rfc822Name value.
     *
     * @param array $array
     *
     * @return rfc822Name
     */
    public function one_and_only_rfc822Name(, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * regexp_match_rfc822Name()
     *
     * Checks whether rfc822Name value matches the regular expression regexp
     * and return the boolean result.
     *
     * @param rfc822Name $value
     * @param string $regexp
     *
     * @return boolean
     */
    public function regexp_match_rfc822Name(, $value, $regexp)
    {
        $request = $this->$session->request();

        $request->set("function", "regexp_match<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * rfc822Name_match()
     *
     * Returns true if value matches pattern, a complete or partial
     * rfc822Name.
     * 
     * If pattern is a complete mailbox, such as "Anderson@sun.com", it
     * matches that mailbox: the local part exactly and the domain part
     * ignoring case.
     * 
     * If pattern is a domain, such as "sun.com", it matches every mailbox at
     * exactly that domain (ignoring case), not at its subdomains.
     * 
     * If pattern is a domain that starts with ".", such as ".east.sun.com",
     * it matches every mailbox in a subdomain of it, such as
     * "Anderson@a.east.sun.com", but not at "east.sun.com" itself.
     *
     * @param string $pattern A mailbox, a domain, or a domain that starts
     *                        with ".".
     * @param rfc822Name $value The rfc822Name to check.
     *
     * @return boolean True if value matches pattern.
     */
    public function rfc822Name_match(, $pattern, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "rfc822Name_match");

        /* pass along required parameters to the request payload */
        $request->set("pattern", $pattern);
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * set_equals_rfc822Name()
     *
     * Returns boolean true if rfc822Name array1 and rfc822Name array2 are
     * subsets of each other and return the boolean result.
     *
     * @param array $array1
     * @param array $array2
     *
     * @return boolean
     */
    public function set_equals_rfc822Name(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_rfc822Name()
     *
     * Returns boolean true if the unique values in rfc822Name array1 are all
     * in rfc822Name array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function subset_rfc822Name(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_rfc822Name()
     *
     * Converts rfc822Name value to string. For array values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param rfc822Name $value A rfc822Name value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_rfc822Name(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * union_rfc822Name()
     *
     * Returns an array of rfc822Name contains all of the unique values in two
     * or more array of rfc822Name values.
     *
     * @param array $arrays Two or more arrays.
     *
     * @return array
     */
    public function union_rfc822Name(, $arrays)
    {
        $request = $this->$session->request();

        $request->set("function", "union<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("arrays", $arrays);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
