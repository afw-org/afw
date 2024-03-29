<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for dayTimeDuration
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category dayTimeDuration
 */
class dayTimeDuration
{
    private $session;

    /**
     * Constructs the dayTimeDuration class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * at_least_one_member_of_dayTimeDuration()
     *
     * Returns boolean true if at least one value in dayTimeDuration list1 is
     * in dayTimeDuration list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return boolean
     */
    public function at_least_one_member_of_dayTimeDuration(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_dayTimeDuration()
     *
     * Takes any number of dayTimeDuration values and returns a list of list.
     *
     * @param list $values
     *
     * @return list
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
     * This returns the integer number of values in list.
     *
     * @param list $value
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
     * "eqx" ("===") instead if you want false to be returned if arg1 and
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
     * type of arg2 then return the boolean result. Use "eq" ("==") instead
     * if you want arg2 to be converted to the data type of arg1 before
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
     * Returns a list of dayTimeDuration with the values that are common to
     * both list of dayTimeDuration list1 and list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return list
     */
    public function intersection_dayTimeDuration(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_dayTimeDuration()
     *
     * Checks whether value is dataType dayTimeDuration and return the
     * boolean result.
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
     * Checks whether dayTimeDuration value is in list of dayTimeDuration
     * list and returns the boolean result.
     *
     * @param dayTimeDuration $value
     * @param list $list
     *
     * @return boolean
     */
    public function is_in_dayTimeDuration(, $value, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("list", $list);

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
     * "nex" ("!==") instead if you want true to be returned if arg1 and
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
     * Determine if for dayTimeDuration arg1 is not equal to the value or
     * data type of arg2 then return the boolean result. Use "ne" ("!=")
     * instead if you want arg2 to be converted to the data type of arg1
     * before comparison.
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
     * This converts a list of dayTimeDuration values that contains one value
     * to a single dayTimeDuration value.
     *
     * @param list $list
     *
     * @return dayTimeDuration
     */
    public function one_and_only_dayTimeDuration(, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * set_equals_dayTimeDuration()
     *
     * Returns boolean true if dayTimeDuration list1 and dayTimeDuration
     * list2 are subsets of each other and return the boolean result.
     *
     * @param list $list1
     * @param list $list2
     *
     * @return boolean
     */
    public function set_equals_dayTimeDuration(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_dayTimeDuration()
     *
     * Returns boolean true if the unique values in dayTimeDuration list1 are
     * all in dayTimeDuration list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return boolean
     */
    public function subset_dayTimeDuration(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_dayTimeDuration()
     *
     * Converts dayTimeDuration value to string. For list values, the
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
     * Returns a list of dayTimeDuration contains all of the unique values in
     * two or more list of dayTimeDuration values.
     *
     * @param list $lists Two or more lists.
     *
     * @return list
     */
    public function union_dayTimeDuration(, $lists)
    {
        $request = $this->$session->request();

        $request->set("function", "union<dayTimeDuration>");

        /* pass along required parameters to the request payload */
        $request->set("lists", $lists);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
