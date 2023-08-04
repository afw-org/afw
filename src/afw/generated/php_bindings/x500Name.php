<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for x500Name
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category x500Name
 */
class x500Name
{
    private $session;

    /**
     * Constructs the x500Name class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * at_least_one_member_of_x500Name()
     *
     * Returns boolean true if at least one value in x500Name list1 is in
     * x500Name list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return boolean
     */
    public function at_least_one_member_of_x500Name(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_size_x500Name()
     *
     * This returns the integer number of values in list.
     *
     * @param list $value
     *
     * @return integer
     */
    public function bag_size_x500Name(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_x500Name()
     *
     * Takes any number of x500Name values and returns a list of list.
     *
     * @param list $values
     *
     * @return list
     */
    public function bag_x500Name(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<x500Name>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * eq_x500Name()
     *
     * Determine if x500Name arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use "eqx"
     * ("===") instead if you want false to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param x500Name $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_x500Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_x500Name()
     *
     * Determine if for x500Name arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use "eq" ("==") instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param x500Name $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_x500Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_x500Name()
     *
     * Checks for x500Name arg1 is greater than or equal to x500Name arg2 and
     * return the boolean result.
     *
     * @param x500Name $arg1
     * @param x500Name $arg2
     *
     * @return boolean
     */
    public function ge_x500Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_x500Name()
     *
     * Checks for x500Name arg1 is greater than x500Name arg2 and return the
     * boolean result.
     *
     * @param x500Name $arg1
     * @param x500Name $arg2
     *
     * @return boolean
     */
    public function gt_x500Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * intersection_x500Name()
     *
     * Returns a list of x500Name with the values that are common to both
     * list of x500Name list1 and list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return list
     */
    public function intersection_x500Name(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_x500Name()
     *
     * Checks whether x500Name value is in list of x500Name list and returns
     * the boolean result.
     *
     * @param x500Name $value
     * @param list $list
     *
     * @return boolean
     */
    public function is_in_x500Name(, $value, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_x500Name()
     *
     * Checks whether value is dataType x500Name and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_x500Name(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_x500Name()
     *
     * Checks for x500Name arg1 is less than or equal to x500Name arg2 and
     * return the boolean result.
     *
     * @param x500Name $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_x500Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_x500Name()
     *
     * Checks for x500Name arg1 is less that x500Name arg2 and return the
     * boolean result.
     *
     * @param x500Name $arg1
     * @param x500Name $arg2
     *
     * @return boolean
     */
    public function lt_x500Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * match_x500Name()
     *
     * Returns true if arg1 matches some terminal sequence of RDNs from arg2
     * when compared using equal_x500Name.
     *
     * @param x500Name $arg1
     * @param string $arg2
     *
     * @return boolean
     */
    public function match_x500Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "match<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_x500Name()
     *
     * Determine if x500Name arg1 is not equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use "nex"
     * ("!==") instead if you want true to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param x500Name $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_x500Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_x500Name()
     *
     * Determine if for x500Name arg1 is not equal to the value or data type
     * of arg2 then return the boolean result. Use "ne" ("!=") instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param x500Name $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_x500Name(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only_x500Name()
     *
     * This converts a list of x500Name values that contains one value to a
     * single x500Name value.
     *
     * @param list $list
     *
     * @return x500Name
     */
    public function one_and_only_x500Name(, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * regexp_match_x500Name()
     *
     * Checks whether x500Name value matches the regular expression regexp
     * and return the boolean result.
     *
     * @param x500Name $value
     * @param string $regexp
     *
     * @return boolean
     */
    public function regexp_match_x500Name(, $value, $regexp)
    {
        $request = $this->$session->request();

        $request->set("function", "regexp_match<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * set_equals_x500Name()
     *
     * Returns boolean true if x500Name list1 and x500Name list2 are subsets
     * of each other and return the boolean result.
     *
     * @param list $list1
     * @param list $list2
     *
     * @return boolean
     */
    public function set_equals_x500Name(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_x500Name()
     *
     * Returns boolean true if the unique values in x500Name list1 are all in
     * x500Name list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return boolean
     */
    public function subset_x500Name(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_x500Name()
     *
     * Converts x500Name value to string. For list values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param x500Name $value A x500Name value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_x500Name(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * union_x500Name()
     *
     * Returns a list of x500Name contains all of the unique values in two or
     * more list of x500Name values.
     *
     * @param list $lists Two or more lists.
     *
     * @return list
     */
    public function union_x500Name(, $lists)
    {
        $request = $this->$session->request();

        $request->set("function", "union<x500Name>");

        /* pass along required parameters to the request payload */
        $request->set("lists", $lists);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * x500Name()
     *
     * Converts value to data type x500Name returning x500Name result.
     *
     * @param  $value Value to convert
     *
     * @return x500Name Converted value
     */
    public function x500Name(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "x500Name");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
