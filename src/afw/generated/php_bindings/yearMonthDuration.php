<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for yearMonthDuration
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category yearMonthDuration
 */
class yearMonthDuration
{
    private $session;

    /**
     * Constructs the yearMonthDuration class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * at_least_one_member_of_yearMonthDuration()
     *
     * Returns boolean true if at least one value in yearMonthDuration list1
     * is in yearMonthDuration list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return boolean
     */
    public function at_least_one_member_of_yearMonthDuration(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_size_yearMonthDuration()
     *
     * This returns the integer number of values in list.
     *
     * @param list $value
     *
     * @return integer
     */
    public function bag_size_yearMonthDuration(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_yearMonthDuration()
     *
     * Takes any number of yearMonthDuration values and returns a list of
     * list.
     *
     * @param list $values
     *
     * @return list
     */
    public function bag_yearMonthDuration(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<yearMonthDuration>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * eq_yearMonthDuration()
     *
     * Determine if yearMonthDuration arg1 is equal to the value of arg2
     * converted to the data type of arg1 then return the boolean result. Use
     * "eqx" ("===") instead if you want false to be returned if arg1 and
     * arg2's data type don't match.
     *
     * @param yearMonthDuration $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_yearMonthDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_yearMonthDuration()
     *
     * Determine if for yearMonthDuration arg1 is equal to the value and data
     * type of arg2 then return the boolean result. Use "eq" ("==") instead
     * if you want arg2 to be converted to the data type of arg1 before
     * comparison.
     *
     * @param yearMonthDuration $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_yearMonthDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_yearMonthDuration()
     *
     * Checks for yearMonthDuration arg1 is greater than or equal to
     * yearMonthDuration arg2 and return the boolean result.
     *
     * @param yearMonthDuration $arg1
     * @param yearMonthDuration $arg2
     *
     * @return boolean
     */
    public function ge_yearMonthDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_yearMonthDuration()
     *
     * Checks for yearMonthDuration arg1 is greater than yearMonthDuration
     * arg2 and return the boolean result.
     *
     * @param yearMonthDuration $arg1
     * @param yearMonthDuration $arg2
     *
     * @return boolean
     */
    public function gt_yearMonthDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * intersection_yearMonthDuration()
     *
     * Returns a list of yearMonthDuration with the values that are common to
     * both list of yearMonthDuration list1 and list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return list
     */
    public function intersection_yearMonthDuration(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_yearMonthDuration()
     *
     * Checks whether yearMonthDuration value is in list of yearMonthDuration
     * list and returns the boolean result.
     *
     * @param yearMonthDuration $value
     * @param list $list
     *
     * @return boolean
     */
    public function is_in_yearMonthDuration(, $value, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_yearMonthDuration()
     *
     * Checks whether value is dataType yearMonthDuration and return the
     * boolean result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_yearMonthDuration(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_yearMonthDuration()
     *
     * Checks for yearMonthDuration arg1 is less than or equal to
     * yearMonthDuration arg2 and return the boolean result.
     *
     * @param yearMonthDuration $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_yearMonthDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_yearMonthDuration()
     *
     * Checks for yearMonthDuration arg1 is less that yearMonthDuration arg2
     * and return the boolean result.
     *
     * @param yearMonthDuration $arg1
     * @param yearMonthDuration $arg2
     *
     * @return boolean
     */
    public function lt_yearMonthDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_yearMonthDuration()
     *
     * Determine if yearMonthDuration arg1 is not equal to the value of arg2
     * converted to the data type of arg1 then return the boolean result. Use
     * "nex" ("!==") instead if you want true to be returned if arg1 and
     * arg2's data type don't match.
     *
     * @param yearMonthDuration $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_yearMonthDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_yearMonthDuration()
     *
     * Determine if for yearMonthDuration arg1 is not equal to the value or
     * data type of arg2 then return the boolean result. Use "ne" ("!=")
     * instead if you want arg2 to be converted to the data type of arg1
     * before comparison.
     *
     * @param yearMonthDuration $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_yearMonthDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only_yearMonthDuration()
     *
     * This converts a list of yearMonthDuration values that contains one
     * value to a single yearMonthDuration value.
     *
     * @param list $list
     *
     * @return yearMonthDuration
     */
    public function one_and_only_yearMonthDuration(, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * set_equals_yearMonthDuration()
     *
     * Returns boolean true if yearMonthDuration list1 and yearMonthDuration
     * list2 are subsets of each other and return the boolean result.
     *
     * @param list $list1
     * @param list $list2
     *
     * @return boolean
     */
    public function set_equals_yearMonthDuration(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_yearMonthDuration()
     *
     * Returns boolean true if the unique values in yearMonthDuration list1
     * are all in yearMonthDuration list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return boolean
     */
    public function subset_yearMonthDuration(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_yearMonthDuration()
     *
     * Converts yearMonthDuration value to string. For list values, the
     * to_string() value for each entry is returned separated with commas.
     *
     * @param yearMonthDuration $value A yearMonthDuration value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_yearMonthDuration(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * union_yearMonthDuration()
     *
     * Returns a list of yearMonthDuration contains all of the unique values
     * in two or more list of yearMonthDuration values.
     *
     * @param list $lists Two or more lists.
     *
     * @return list
     */
    public function union_yearMonthDuration(, $lists)
    {
        $request = $this->$session->request();

        $request->set("function", "union<yearMonthDuration>");

        /* pass along required parameters to the request payload */
        $request->set("lists", $lists);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * yearMonthDuration()
     *
     * Converts value to data type yearMonthDuration returning
     * yearMonthDuration result.
     *
     * @param  $value Value to convert
     *
     * @return yearMonthDuration Converted value
     */
    public function yearMonthDuration(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "yearMonthDuration");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
