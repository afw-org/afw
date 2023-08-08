<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for dateTime
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category dateTime
 */
class dateTime
{
    private $session;

    /**
     * Constructs the dateTime class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * add_dayTimeDuration_dateTime()
     *
     * Add dateTime arg1 to dayTimeDuration arg2 and return the dateTime
     * result.
     *
     * @param dateTime $arg1
     * @param dayTimeDuration $arg2
     *
     * @return dateTime
     */
    public function add_dayTimeDuration_dateTime(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "add_dayTimeDuration<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * add_yearMonthDuration_dateTime()
     *
     * Add dateTime arg1 to yearMonthDuration arg2 and return the dateTime
     * result.
     *
     * @param dateTime $arg1
     * @param yearMonthDuration $arg2
     *
     * @return dateTime
     */
    public function add_yearMonthDuration_dateTime(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "add_yearMonthDuration<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * at_least_one_member_of_dateTime()
     *
     * Returns boolean true if at least one value in dateTime list1 is in
     * dateTime list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return boolean
     */
    public function at_least_one_member_of_dateTime(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_dateTime()
     *
     * Takes any number of dateTime values and returns a list of list.
     *
     * @param list $values
     *
     * @return list
     */
    public function bag_dateTime(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<dateTime>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_dateTime()
     *
     * This returns the integer number of values in list.
     *
     * @param list $value
     *
     * @return integer
     */
    public function bag_size_dateTime(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * dateTime()
     *
     * Converts value to data type dateTime returning dateTime result.
     *
     * @param  $value Value to convert
     *
     * @return dateTime Converted value
     */
    public function dateTime(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "dateTime");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_dateTime()
     *
     * Determine if dateTime arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'eqx'
     * ('===') instead if you want false to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param dateTime $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_dateTime(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_dateTime()
     *
     * Determine if for dateTime arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param dateTime $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_dateTime(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_dateTime()
     *
     * Checks for dateTime arg1 is greater than or equal to dateTime arg2 and
     * return the boolean result.
     * 
     * If a dateTime value does not include a time-zone value, then the local
     * time-zone value will be assigned.
     *
     * @param dateTime $arg1
     * @param dateTime $arg2
     *
     * @return boolean
     */
    public function ge_dateTime(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_dateTime()
     *
     * Checks for dateTime arg1 is greater than dateTime arg2 and return the
     * boolean result.
     * 
     * If a dateTime value does not include a time-zone value, then the local
     * time-zone value will be assigned.
     *
     * @param dateTime $arg1
     * @param dateTime $arg2
     *
     * @return boolean
     */
    public function gt_dateTime(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * intersection_dateTime()
     *
     * Returns a list of dateTime with the values that are common to both
     * list of dateTime list1 and list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return list
     */
    public function intersection_dateTime(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_dateTime()
     *
     * Checks whether value is dataType dateTime and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_dateTime(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_dateTime()
     *
     * Checks whether dateTime value is in list of dateTime list and returns
     * the boolean result.
     *
     * @param dateTime $value
     * @param list $list
     *
     * @return boolean
     */
    public function is_in_dateTime(, $value, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_dateTime()
     *
     * Checks for dateTime arg1 is less than or equal to dateTime arg2 and
     * return the boolean result.
     * 
     * If a dateTime value does not include a time-zone value, then the local
     * time-zone value will be assigned.
     *
     * @param dateTime $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_dateTime(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_dateTime()
     *
     * Checks for dateTime arg1 is less that dateTime arg2 and return the
     * boolean result.
     * 
     * If a dateTime value does not include a time-zone value, then the local
     * time-zone value will be assigned.
     *
     * @param dateTime $arg1
     * @param dateTime $arg2
     *
     * @return boolean
     */
    public function lt_dateTime(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * max_dateTime()
     *
     * Return the dateTime value that is greater than or equal to the others.
     * 
     * If a dateTime value does not include a time-zone value, then the local
     * time-zone value will be assigned.
     *
     * @param dateTime $values
     *
     * @return dateTime
     */
    public function max_dateTime(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "max<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * min_dateTime()
     *
     * Return the dateTime value that is less than or equal to the others.
     * 
     * If a dateTime value does not include a time-zone value, then the local
     * time-zone value will be assigned.
     *
     * @param dateTime $values
     *
     * @return dateTime
     */
    public function min_dateTime(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "min<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_dateTime()
     *
     * Determine if dateTime arg1 is not equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use 'nex'
     * ('!==') instead if you want true to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param dateTime $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_dateTime(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_dateTime()
     *
     * Determine if for dateTime arg1 is not equal to the value or data type
     * of arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param dateTime $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_dateTime(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only_dateTime()
     *
     * This converts a list of dateTime values that contains one value to a
     * single dateTime value.
     *
     * @param list $list
     *
     * @return dateTime
     */
    public function one_and_only_dateTime(, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * set_equals_dateTime()
     *
     * Returns boolean true if dateTime list1 and dateTime list2 are subsets
     * of each other and return the boolean result.
     *
     * @param list $list1
     * @param list $list2
     *
     * @return boolean
     */
    public function set_equals_dateTime(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_dateTime()
     *
     * Returns boolean true if the unique values in dateTime list1 are all in
     * dateTime list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return boolean
     */
    public function subset_dateTime(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subtract_dayTimeDuration_dateTime()
     *
     * Subtract dayTimeDuration arg2 from dateTime arg1 and return the
     * dateTime result.
     *
     * @param dateTime $arg1
     * @param dayTimeDuration $arg2
     *
     * @return dateTime
     */
    public function subtract_dayTimeDuration_dateTime(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "subtract_dayTimeDuration<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subtract_yearMonthDuration_dateTime()
     *
     * Subtract yearMonthDuration arg2 from dateTime arg1 and return the
     * dateTime result.
     *
     * @param dateTime $arg1
     * @param yearMonthDuration $arg2
     *
     * @return dateTime
     */
    public function subtract_yearMonthDuration_dateTime(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "subtract_yearMonthDuration<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_dateTime()
     *
     * Converts dateTime value to string. For list values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param dateTime $value A dateTime value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_dateTime(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * union_dateTime()
     *
     * Returns a list of dateTime contains all of the unique values in two or
     * more list of dateTime values.
     *
     * @param list $lists Two or more lists.
     *
     * @return list
     */
    public function union_dateTime(, $lists)
    {
        $request = $this->$session->request();

        $request->set("function", "union<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("lists", $lists);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
