<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for dateTime
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file dateTime.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category dateTime
 */
class dateTime
{
    private $session;

    /**
     * Constructs the dateTime class, using a session.
     */
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
     * Returns boolean true if at least one value in dateTime array1 is in
     * dateTime array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function at_least_one_member_of_dateTime(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_dateTime()
     *
     * Takes any number of dateTime values and returns an array of array.
     *
     * @param array $values
     *
     * @return array
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
     * This returns the integer number of values in array.
     *
     * @param array $value
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
     * the data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
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
     * Returns an array of dateTime with the values that are common to both
     * array of dateTime array1 and array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return array
     */
    public function intersection_dateTime(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

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
     * Checks whether dateTime value is in array of dateTime array and returns
     * the boolean result.
     *
     * @param dateTime $value
     * @param array $array
     *
     * @return boolean
     */
    public function is_in_dateTime(, $value, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("array", $array);

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
     * ('!==') instead if you want true to be returned if arg1 and arg2's data
     * type don't match.
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
     * This converts an array of dateTime values that contains one value to a
     * single dateTime value.
     *
     * @param array $array
     *
     * @return dateTime
     */
    public function one_and_only_dateTime(, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * set_equals_dateTime()
     *
     * Returns boolean true if dateTime array1 and dateTime array2 are subsets
     * of each other and return the boolean result.
     *
     * @param array $array1
     * @param array $array2
     *
     * @return boolean
     */
    public function set_equals_dateTime(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_dateTime()
     *
     * Returns boolean true if the unique values in dateTime array1 are all in
     * dateTime array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function subset_dateTime(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

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
     * Converts dateTime value to string. For array values, the to_string()
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
     * Returns an array of dateTime contains all of the unique values in two
     * or more array of dateTime values.
     *
     * @param array $arrays Two or more arrays.
     *
     * @return array
     */
    public function union_dateTime(, $arrays)
    {
        $request = $this->$session->request();

        $request->set("function", "union<dateTime>");

        /* pass along required parameters to the request payload */
        $request->set("arrays", $arrays);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
