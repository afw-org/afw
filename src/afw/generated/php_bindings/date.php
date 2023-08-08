<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for date
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category date
 */
class date
{
    private $session;

    /**
     * Constructs the date class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * add_yearMonthDuration_date()
     *
     * Add date arg1 to yearMonthDuration arg2 and return the date result.
     *
     * @param date $arg1
     * @param yearMonthDuration $arg2
     *
     * @return date
     */
    public function add_yearMonthDuration_date(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "add_yearMonthDuration<date>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * at_least_one_member_of_date()
     *
     * Returns boolean true if at least one value in date list1 is in date
     * list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return boolean
     */
    public function at_least_one_member_of_date(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<date>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_date()
     *
     * Takes any number of date values and returns a list of list.
     *
     * @param list $values
     *
     * @return list
     */
    public function bag_date(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<date>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_date()
     *
     * This returns the integer number of values in list.
     *
     * @param list $value
     *
     * @return integer
     */
    public function bag_size_date(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<date>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * date()
     *
     * Converts value to data type date returning date result.
     *
     * @param  $value Value to convert
     *
     * @return date Converted value
     */
    public function date(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "date");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_date()
     *
     * Determine if date arg1 is equal to the value of arg2 converted to the
     * data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param date $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_date(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<date>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_date()
     *
     * Determine if for date arg1 is equal to the value and data type of arg2
     * then return the boolean result. Use 'eq' ('==') instead if you want
     * arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param date $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_date(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<date>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_date()
     *
     * Checks for date arg1 is greater than or equal to date arg2 and return
     * the boolean result.
     * 
     * If a date value does not include a time-zone value, then the local
     * time-zone value will be assigned.
     *
     * @param date $arg1
     * @param date $arg2
     *
     * @return boolean
     */
    public function ge_date(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<date>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_date()
     *
     * Checks for date arg1 is greater than date arg2 and return the boolean
     * result.
     * 
     * If a date value does not include a time-zone value, then the local
     * time-zone value will be assigned.
     *
     * @param date $arg1
     * @param date $arg2
     *
     * @return boolean
     */
    public function gt_date(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<date>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * intersection_date()
     *
     * Returns a list of date with the values that are common to both list of
     * date list1 and list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return list
     */
    public function intersection_date(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<date>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_date()
     *
     * Checks whether value is dataType date and return the boolean result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_date(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<date>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_date()
     *
     * Checks whether date value is in list of date list and returns the
     * boolean result.
     *
     * @param date $value
     * @param list $list
     *
     * @return boolean
     */
    public function is_in_date(, $value, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<date>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_date()
     *
     * Checks for date arg1 is less than or equal to date arg2 and return the
     * boolean result.
     * 
     * If a date value does not include a time-zone value, then the local
     * time-zone value will be assigned.
     *
     * @param date $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_date(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<date>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_date()
     *
     * Checks for date arg1 is less that date arg2 and return the boolean
     * result.
     * 
     * If a date value does not include a time-zone value, then the local
     * time-zone value will be assigned.
     *
     * @param date $arg1
     * @param date $arg2
     *
     * @return boolean
     */
    public function lt_date(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<date>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * max_date()
     *
     * Return the date value that is greater than or equal to the others..
     * 
     * If a date value does not include a time-zone value, then the local
     * time-zone value will be assigned.
     *
     * @param date $values
     *
     * @return date
     */
    public function max_date(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "max<date>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * min_date()
     *
     * Return the date value that is less than or equal to the others..
     * 
     * If a date value does not include a time-zone value, then the local
     * time-zone value will be assigned.
     *
     * @param date $values
     *
     * @return date
     */
    public function min_date(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "min<date>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_date()
     *
     * Determine if date arg1 is not equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'nex'
     * ('!==') instead if you want true to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param date $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_date(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<date>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_date()
     *
     * Determine if for date arg1 is not equal to the value or data type of
     * arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param date $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_date(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<date>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only_date()
     *
     * This converts a list of date values that contains one value to a
     * single date value.
     *
     * @param list $list
     *
     * @return date
     */
    public function one_and_only_date(, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<date>");

        /* pass along required parameters to the request payload */
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * set_equals_date()
     *
     * Returns boolean true if date list1 and date list2 are subsets of each
     * other and return the boolean result.
     *
     * @param list $list1
     * @param list $list2
     *
     * @return boolean
     */
    public function set_equals_date(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<date>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_date()
     *
     * Returns boolean true if the unique values in date list1 are all in
     * date list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return boolean
     */
    public function subset_date(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<date>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subtract_yearMonthDuration_date()
     *
     * Subtract yearMonthDuration arg2 from date arg1 and return the date
     * result.
     *
     * @param date $arg1
     * @param yearMonthDuration $arg2
     *
     * @return date
     */
    public function subtract_yearMonthDuration_date(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "subtract_yearMonthDuration<date>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_date()
     *
     * Converts date value to string. For list values, the to_string() value
     * for each entry is returned separated with commas.
     *
     * @param date $value A date value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_date(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<date>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * union_date()
     *
     * Returns a list of date contains all of the unique values in two or
     * more list of date values.
     *
     * @param list $lists Two or more lists.
     *
     * @return list
     */
    public function union_date(, $lists)
    {
        $request = $this->$session->request();

        $request->set("function", "union<date>");

        /* pass along required parameters to the request payload */
        $request->set("lists", $lists);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
