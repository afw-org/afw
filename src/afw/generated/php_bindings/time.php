<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for time
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category time
 */
class time
{
    private $session;

    /**
     * Constructs the time class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * at_least_one_member_of_time()
     *
     * Returns boolean true if at least one value in time list1 is in time
     * list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return boolean
     */
    public function at_least_one_member_of_time(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<time>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_size_time()
     *
     * This returns the integer number of values in list.
     *
     * @param list $value
     *
     * @return integer
     */
    public function bag_size_time(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<time>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_time()
     *
     * Takes any number of time values and returns a list of list.
     *
     * @param list $values
     *
     * @return list
     */
    public function bag_time(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<time>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * eq_time()
     *
     * Determine if time arg1 is equal to the value of arg2 converted to the
     * data type of arg1 then return the boolean result. Use "eqx" ("===")
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param time $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_time(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<time>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_time()
     *
     * Determine if for time arg1 is equal to the value and data type of arg2
     * then return the boolean result. Use "eq" ("==") instead if you want
     * arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param time $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_time(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<time>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_time()
     *
     * Checks for time arg1 is greater than or equal to time arg2 and return
     * the boolean result.
     * 
     * It is illegal to compare a time that includes a time-zone value with
     * one that does not. In such cases, the `in_range<time>` function should
     * be used.
     *
     * @param time $arg1
     * @param time $arg2
     *
     * @return boolean
     */
    public function ge_time(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<time>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_time()
     *
     * Checks for time arg1 is greater than time arg2 and return the boolean
     * result.
     * 
     * It is illegal to compare a time that includes a time-zone value with
     * one that does not. In such cases, the `in_range<time>` function should
     * be used.
     *
     * @param time $arg1
     * @param time $arg2
     *
     * @return boolean
     */
    public function gt_time(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<time>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * in_range_time()
     *
     * Checks if time is between startTime and endTime, inclusive. Regardless
     * of endTime value, it is always considered to be equal to, but less
     * than 24 hours greater than startTime. If no time zone is specified for
     * time, the default time zone is used. If no time zone is specified for
     * startTime or endTime, the time zone of time is used.
     *
     * @param time $time
     * @param time $startTime
     * @param time $endTime
     *
     * @return boolean
     */
    public function in_range_time(, $time, $startTime, $endTime)
    {
        $request = $this->$session->request();

        $request->set("function", "in_range<time>");

        /* pass along required parameters to the request payload */
        $request->set("time", $time);
        $request->set("startTime", $startTime);
        $request->set("endTime", $endTime);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * intersection_time()
     *
     * Returns a list of time with the values that are common to both list of
     * time list1 and list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return list
     */
    public function intersection_time(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<time>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_time()
     *
     * Checks whether time value is in list of time list and returns the
     * boolean result.
     *
     * @param time $value
     * @param list $list
     *
     * @return boolean
     */
    public function is_in_time(, $value, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<time>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_time()
     *
     * Checks whether value is dataType time and return the boolean result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_time(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<time>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_time()
     *
     * Checks for time arg1 is less than or equal to time arg2 and return the
     * boolean result.
     * 
     * It is illegal to compare a time that includes a time-zone value with
     * one that does not. In such cases, the `in_range<time>` function should
     * be used.
     *
     * @param time $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_time(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<time>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_time()
     *
     * Checks for time arg1 is less that time arg2 and return the boolean
     * result.
     * 
     * It is illegal to compare a time that includes a time-zone value with
     * one that does not. In such cases, the `in_range<time>` function should
     * be used.
     *
     * @param time $arg1
     * @param time $arg2
     *
     * @return boolean
     */
    public function lt_time(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<time>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * max_time()
     *
     * Return the time value that is greater than or equal to the others.
     * 
     * It is illegal to compare a time that includes a time-zone value with
     * one that does not. In such cases, the `in_range<time>` function should
     * be used.
     *
     * @param time $values
     *
     * @return time
     */
    public function max_time(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "max<time>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * min_time()
     *
     * Return the time value that is less than or equal to the others.
     * 
     * It is illegal to compare a time that includes a time-zone value with
     * one that does not. In such cases, the `in_range<time>` function should
     * be used.
     *
     * @param time $values
     *
     * @return time
     */
    public function min_time(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "min<time>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_time()
     *
     * Determine if time arg1 is not equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use "nex"
     * ("!==") instead if you want true to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param time $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_time(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<time>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_time()
     *
     * Determine if for time arg1 is not equal to the value or data type of
     * arg2 then return the boolean result. Use "ne" ("!=") instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param time $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_time(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<time>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only_time()
     *
     * This converts a list of time values that contains one value to a
     * single time value.
     *
     * @param list $list
     *
     * @return time
     */
    public function one_and_only_time(, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<time>");

        /* pass along required parameters to the request payload */
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * set_equals_time()
     *
     * Returns boolean true if time list1 and time list2 are subsets of each
     * other and return the boolean result.
     *
     * @param list $list1
     * @param list $list2
     *
     * @return boolean
     */
    public function set_equals_time(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<time>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_time()
     *
     * Returns boolean true if the unique values in time list1 are all in
     * time list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return boolean
     */
    public function subset_time(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<time>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * time()
     *
     * Converts value to data type time returning time result.
     *
     * @param  $value Value to convert
     *
     * @return time Converted value
     */
    public function time(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "time");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_time()
     *
     * Converts time value to string. For list values, the to_string() value
     * for each entry is returned separated with commas.
     *
     * @param time $value A time value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_time(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<time>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * union_time()
     *
     * Returns a list of time contains all of the unique values in two or
     * more list of time values.
     *
     * @param list $lists Two or more lists.
     *
     * @return list
     */
    public function union_time(, $lists)
    {
        $request = $this->$session->request();

        $request->set("function", "union<time>");

        /* pass along required parameters to the request payload */
        $request->set("lists", $lists);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
