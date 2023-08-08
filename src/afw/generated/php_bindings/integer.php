<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for integer
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category integer
 */
class integer
{
    private $session;

    /**
     * Constructs the integer class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * abs_integer()
     *
     * Compute the absolute value of the integer value and return the integer
     * result.
     *
     * @param integer $value
     *
     * @return integer
     */
    public function abs_integer(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "abs<integer>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * add_integer()
     *
     * Add 2 or more integer values and return the integer result.
     *
     * @param integer $values
     *
     * @return integer
     */
    public function add_integer(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "add<integer>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * at_least_one_member_of_integer()
     *
     * Returns boolean true if at least one value in integer list1 is in
     * integer list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return boolean
     */
    public function at_least_one_member_of_integer(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<integer>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_integer()
     *
     * Takes any number of integer values and returns a list of list.
     *
     * @param list $values
     *
     * @return list
     */
    public function bag_integer(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<integer>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_integer()
     *
     * This returns the integer number of values in list.
     *
     * @param list $value
     *
     * @return integer
     */
    public function bag_size_integer(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<integer>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * divide_integer()
     *
     * Divide integer dividend by integer divisor and return the integer
     * quotient.
     *
     * @param integer $dividend
     * @param integer $divisor
     *
     * @return integer
     */
    public function divide_integer(, $dividend, $divisor)
    {
        $request = $this->$session->request();

        $request->set("function", "divide<integer>");

        /* pass along required parameters to the request payload */
        $request->set("dividend", $dividend);
        $request->set("divisor", $divisor);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_integer()
     *
     * Determine if integer arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'eqx'
     * ('===') instead if you want false to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param integer $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_integer(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<integer>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_integer()
     *
     * Determine if for integer arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param integer $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_integer(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<integer>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_integer()
     *
     * Checks for integer arg1 is greater than or equal to integer arg2 and
     * return the boolean result.
     *
     * @param integer $arg1
     * @param integer $arg2
     *
     * @return boolean
     */
    public function ge_integer(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<integer>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_integer()
     *
     * Checks for integer arg1 is greater than integer arg2 and return the
     * boolean result.
     *
     * @param integer $arg1
     * @param integer $arg2
     *
     * @return boolean
     */
    public function gt_integer(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<integer>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * integer()
     *
     * Converts value to data type integer returning integer result.
     *
     * @param  $value Value to convert
     *
     * @return integer Converted value
     */
    public function integer(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "integer");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * intersection_integer()
     *
     * Returns a list of integer with the values that are common to both list
     * of integer list1 and list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return list
     */
    public function intersection_integer(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<integer>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_integer()
     *
     * Checks whether integer value is in list of integer list and returns
     * the boolean result.
     *
     * @param integer $value
     * @param list $list
     *
     * @return boolean
     */
    public function is_in_integer(, $value, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<integer>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_integer()
     *
     * Checks whether value is dataType integer and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_integer(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<integer>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_integer()
     *
     * Checks for integer arg1 is less than or equal to integer arg2 and
     * return the boolean result.
     *
     * @param integer $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_integer(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<integer>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_integer()
     *
     * Checks for integer arg1 is less that integer arg2 and return the
     * boolean result.
     *
     * @param integer $arg1
     * @param integer $arg2
     *
     * @return boolean
     */
    public function lt_integer(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<integer>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * max_integer()
     *
     * Return the integer value that is greater than or equal to the others.
     *
     * @param integer $values
     *
     * @return integer
     */
    public function max_integer(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "max<integer>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * min_integer()
     *
     * Return the integer value that is less than or equal to the others.
     *
     * @param integer $values
     *
     * @return integer
     */
    public function min_integer(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "min<integer>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * mod_integer()
     *
     * Divide integer dividend by integer divisor and return the integer
     * remainder.
     *
     * @param integer $dividend
     * @param integer $divisor
     *
     * @return integer
     */
    public function mod_integer(, $dividend, $divisor)
    {
        $request = $this->$session->request();

        $request->set("function", "mod<integer>");

        /* pass along required parameters to the request payload */
        $request->set("dividend", $dividend);
        $request->set("divisor", $divisor);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * multiply_integer()
     *
     * Multiply 2 or more integer values and return the integer result.
     *
     * @param integer $values
     *
     * @return integer
     */
    public function multiply_integer(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "multiply<integer>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_integer()
     *
     * Determine if integer arg1 is not equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use 'nex'
     * ('!==') instead if you want true to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param integer $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_integer(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<integer>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * negative_integer()
     *
     * Return negative of integer value.
     *
     * @param integer $value
     *
     * @return integer
     */
    public function negative_integer(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "negative<integer>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_integer()
     *
     * Determine if for integer arg1 is not equal to the value or data type
     * of arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param integer $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_integer(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<integer>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only_integer()
     *
     * This converts a list of integer values that contains one value to a
     * single integer value.
     *
     * @param list $list
     *
     * @return integer
     */
    public function one_and_only_integer(, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<integer>");

        /* pass along required parameters to the request payload */
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * set_equals_integer()
     *
     * Returns boolean true if integer list1 and integer list2 are subsets of
     * each other and return the boolean result.
     *
     * @param list $list1
     * @param list $list2
     *
     * @return boolean
     */
    public function set_equals_integer(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<integer>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_integer()
     *
     * Returns boolean true if the unique values in integer list1 are all in
     * integer list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return boolean
     */
    public function subset_integer(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<integer>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subtract_integer()
     *
     * Subtract integer arg2 from integer arg1 and return the integer result.
     *
     * @param integer $arg1
     * @param integer $arg2
     *
     * @return integer
     */
    public function subtract_integer(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "subtract<integer>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_double_integer()
     *
     * Converts integer value to double and returns double result.
     *
     * @param integer $value
     *
     * @return double
     */
    public function to_double_integer(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_double<integer>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_integer()
     *
     * Converts integer value to string. For list values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param integer $value A integer value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_integer(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<integer>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * union_integer()
     *
     * Returns a list of integer contains all of the unique values in two or
     * more list of integer values.
     *
     * @param list $lists Two or more lists.
     *
     * @return list
     */
    public function union_integer(, $lists)
    {
        $request = $this->$session->request();

        $request->set("function", "union<integer>");

        /* pass along required parameters to the request payload */
        $request->set("lists", $lists);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
