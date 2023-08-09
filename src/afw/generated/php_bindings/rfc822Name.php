<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for rfc822Name
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category rfc822Name
 */
class rfc822Name
{
    private $session;

    /**
     * Constructs the rfc822Name class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * at_least_one_member_of_rfc822Name()
     *
     * Returns boolean true if at least one value in rfc822Name list1 is in
     * rfc822Name list2.
     *
     * @param array $list1 The first array.
     * @param array $list2 The second array.
     *
     * @return boolean
     */
    public function at_least_one_member_of_rfc822Name(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_rfc822Name()
     *
     * Takes any number of rfc822Name values and returns an array of array.
     *
     * @param array $values
     *
     * @return array
     */
    public function bag_rfc822Name(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<rfc822Name>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_rfc822Name()
     *
     * This returns the integer number of values in array.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size_rfc822Name(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_rfc822Name()
     *
     * Determine if rfc822Name arg1 is equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use 'eqx'
     * ('===') instead if you want false to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param rfc822Name $arg1
     * @param  $arg2
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
     * Determine if for rfc822Name arg1 is equal to the value and data type
     * of arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param rfc822Name $arg1
     * @param  $arg2
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
     * ge_rfc822Name()
     *
     * Checks for rfc822Name arg1 is greater than or equal to rfc822Name arg2
     * and return the boolean result.
     *
     * @param rfc822Name $arg1
     * @param rfc822Name $arg2
     *
     * @return boolean
     */
    public function ge_rfc822Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_rfc822Name()
     *
     * Checks for rfc822Name arg1 is greater than rfc822Name arg2 and return
     * the boolean result.
     *
     * @param rfc822Name $arg1
     * @param rfc822Name $arg2
     *
     * @return boolean
     */
    public function gt_rfc822Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<rfc822Name>");

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
     * array of rfc822Name list1 and list2.
     *
     * @param array $list1 The first array.
     * @param array $list2 The second array.
     *
     * @return array
     */
    public function intersection_rfc822Name(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_rfc822Name()
     *
     * Checks whether rfc822Name value is in array of rfc822Name array and
     * returns the boolean result.
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
     * Checks whether value is dataType rfc822Name and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_rfc822Name(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_rfc822Name()
     *
     * Checks for rfc822Name arg1 is less than or equal to rfc822Name arg2
     * and return the boolean result.
     *
     * @param rfc822Name $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_rfc822Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_rfc822Name()
     *
     * Checks for rfc822Name arg1 is less that rfc822Name arg2 and return the
     * boolean result.
     *
     * @param rfc822Name $arg1
     * @param rfc822Name $arg2
     *
     * @return boolean
     */
    public function lt_rfc822Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * match_rfc822Name()
     *
     * Returns true if arg1 matches some terminal sequence of RDNs from arg2
     * when compared using equal_rfc822Name.
     *
     * @param rfc822Name $arg1
     * @param string $arg2
     *
     * @return boolean
     */
    public function match_rfc822Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "match<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

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
     * @param  $arg2
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
     * Determine if for rfc822Name arg1 is not equal to the value or data
     * type of arg2 then return the boolean result. Use 'ne' ('!=') instead
     * if you want arg2 to be converted to the data type of arg1 before
     * comparison.
     *
     * @param rfc822Name $arg1
     * @param  $arg2
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
     * rfc822Name()
     *
     * Converts value to data type rfc822Name returning rfc822Name result.
     *
     * @param  $value Value to convert
     *
     * @return rfc822Name Converted value
     */
    public function rfc822Name(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "rfc822Name");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * set_equals_rfc822Name()
     *
     * Returns boolean true if rfc822Name list1 and rfc822Name list2 are
     * subsets of each other and return the boolean result.
     *
     * @param array $list1
     * @param array $list2
     *
     * @return boolean
     */
    public function set_equals_rfc822Name(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_rfc822Name()
     *
     * Returns boolean true if the unique values in rfc822Name list1 are all
     * in rfc822Name list2.
     *
     * @param array $list1 The first array.
     * @param array $list2 The second array.
     *
     * @return boolean
     */
    public function subset_rfc822Name(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

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
     * Returns an array of rfc822Name contains all of the unique values in
     * two or more array of rfc822Name values.
     *
     * @param array $lists Two or more lists.
     *
     * @return array
     */
    public function union_rfc822Name(, $lists)
    {
        $request = $this->$session->request();

        $request->set("function", "union<rfc822Name>");

        /* pass along required parameters to the request payload */
        $request->set("lists", $lists);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
