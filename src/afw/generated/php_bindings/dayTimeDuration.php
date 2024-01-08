<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for dayTimeDuration
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file dayTimeDuration.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category dayTimeDuration
 */
class dayTimeDuration
{
    private $session;

    /**
     * Constructs the dayTimeDuration class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * at_least_one_member_of_dayTimeDuration()
     *
     * Returns boolean true if at least one value in dayTimeDuration array1 is
     * in dayTimeDuration array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function at_least_one_member_of_dayTimeDuration(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_dayTimeDuration()
     *
     * Takes any number of dayTimeDuration values and returns an array of
     * array.
     *
     * @param array $values
     *
     * @return array
     */
    public function bag_dayTimeDuration(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<dayTimeDuration>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_dayTimeDuration()
     *
     * This returns the integer number of values in array.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size_dayTimeDuration(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * dayTimeDuration()
     *
     * Converts value to data type dayTimeDuration returning dayTimeDuration
     * result.
     *
     * @param  $value Value to convert
     *
     * @return dayTimeDuration Converted value
     */
    public function dayTimeDuration(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "dayTimeDuration");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_dayTimeDuration()
     *
     * Determine if dayTimeDuration arg1 is equal to the value of arg2
     * converted to the data type of arg1 then return the boolean result. Use
     * 'eqx' ('===') instead if you want false to be returned if arg1 and
     * arg2's data type don't match.
     *
     * @param dayTimeDuration $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_dayTimeDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_dayTimeDuration()
     *
     * Determine if for dayTimeDuration arg1 is equal to the value and data
     * type of arg2 then return the boolean result. Use 'eq' ('==') instead if
     * you want arg2 to be converted to the data type of arg1 before
     * comparison.
     *
     * @param dayTimeDuration $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_dayTimeDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_dayTimeDuration()
     *
     * Checks for dayTimeDuration arg1 is greater than or equal to
     * dayTimeDuration arg2 and return the boolean result.
     *
     * @param dayTimeDuration $arg1
     * @param dayTimeDuration $arg2
     *
     * @return boolean
     */
    public function ge_dayTimeDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_dayTimeDuration()
     *
     * Checks for dayTimeDuration arg1 is greater than dayTimeDuration arg2
     * and return the boolean result.
     *
     * @param dayTimeDuration $arg1
     * @param dayTimeDuration $arg2
     *
     * @return boolean
     */
    public function gt_dayTimeDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * intersection_dayTimeDuration()
     *
     * Returns an array of dayTimeDuration with the values that are common to
     * both array of dayTimeDuration array1 and array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return array
     */
    public function intersection_dayTimeDuration(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_dayTimeDuration()
     *
     * Checks whether value is dataType dayTimeDuration and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_dayTimeDuration(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_dayTimeDuration()
     *
     * Checks whether dayTimeDuration value is in array of dayTimeDuration
     * array and returns the boolean result.
     *
     * @param dayTimeDuration $value
     * @param array $array
     *
     * @return boolean
     */
    public function is_in_dayTimeDuration(, $value, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_dayTimeDuration()
     *
     * Checks for dayTimeDuration arg1 is less than or equal to
     * dayTimeDuration arg2 and return the boolean result.
     *
     * @param dayTimeDuration $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_dayTimeDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_dayTimeDuration()
     *
     * Checks for dayTimeDuration arg1 is less that dayTimeDuration arg2 and
     * return the boolean result.
     *
     * @param dayTimeDuration $arg1
     * @param dayTimeDuration $arg2
     *
     * @return boolean
     */
    public function lt_dayTimeDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_dayTimeDuration()
     *
     * Determine if dayTimeDuration arg1 is not equal to the value of arg2
     * converted to the data type of arg1 then return the boolean result. Use
     * 'nex' ('!==') instead if you want true to be returned if arg1 and
     * arg2's data type don't match.
     *
     * @param dayTimeDuration $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_dayTimeDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_dayTimeDuration()
     *
     * Determine if for dayTimeDuration arg1 is not equal to the value or data
     * type of arg2 then return the boolean result. Use 'ne' ('!=') instead if
     * you want arg2 to be converted to the data type of arg1 before
     * comparison.
     *
     * @param dayTimeDuration $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_dayTimeDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only_dayTimeDuration()
     *
     * This converts an array of dayTimeDuration values that contains one
     * value to a single dayTimeDuration value.
     *
     * @param array $array
     *
     * @return dayTimeDuration
     */
    public function one_and_only_dayTimeDuration(, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * set_equals_dayTimeDuration()
     *
     * Returns boolean true if dayTimeDuration array1 and dayTimeDuration
     * array2 are subsets of each other and return the boolean result.
     *
     * @param array $array1
     * @param array $array2
     *
     * @return boolean
     */
    public function set_equals_dayTimeDuration(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_dayTimeDuration()
     *
     * Returns boolean true if the unique values in dayTimeDuration array1 are
     * all in dayTimeDuration array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function subset_dayTimeDuration(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_dayTimeDuration()
     *
     * Converts dayTimeDuration value to string. For array values, the
     * to_string() value for each entry is returned separated with commas.
     *
     * @param dayTimeDuration $value A dayTimeDuration value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_dayTimeDuration(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * union_dayTimeDuration()
     *
     * Returns an array of dayTimeDuration contains all of the unique values
     * in two or more array of dayTimeDuration values.
     *
     * @param array $arrays Two or more arrays.
     *
     * @return array
     */
    public function union_dayTimeDuration(, $arrays)
    {
        $request = $this->$session->request();

        $request->set("function", "union<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("arrays", $arrays);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
