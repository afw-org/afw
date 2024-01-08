<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for integer
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file integer.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category integer
 */
class integer
{
    private $session;

    /**
     * Constructs the integer class, using a session.
     */
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
     * Returns boolean true if at least one value in integer array1 is in
     * integer array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function at_least_one_member_of_integer(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<integer>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_integer()
     *
     * Takes any number of integer values and returns an array of array.
     *
     * @param array $values
     *
     * @return array
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
     * This returns the integer number of values in array.
     *
     * @param array $value
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
     * the data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
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
     * Returns an array of integer with the values that are common to both
     * array of integer array1 and array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return array
     */
    public function intersection_integer(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<integer>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_integer()
     *
     * Checks whether integer value is in array of integer array and returns
     * the boolean result.
     *
     * @param integer $value
     * @param array $array
     *
     * @return boolean
     */
    public function is_in_integer(, $value, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<integer>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_integer()
     *
     * Checks whether value is dataType integer and return the boolean result.
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
     * ('!==') instead if you want true to be returned if arg1 and arg2's data
     * type don't match.
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
     * Determine if for integer arg1 is not equal to the value or data type of
     * arg2 then return the boolean result. Use 'ne' ('!=') instead if you
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
     * This converts an array of integer values that contains one value to a
     * single integer value.
     *
     * @param array $array
     *
     * @return integer
     */
    public function one_and_only_integer(, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<integer>");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * set_equals_integer()
     *
     * Returns boolean true if integer array1 and integer array2 are subsets
     * of each other and return the boolean result.
     *
     * @param array $array1
     * @param array $array2
     *
     * @return boolean
     */
    public function set_equals_integer(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<integer>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_integer()
     *
     * Returns boolean true if the unique values in integer array1 are all in
     * integer array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function subset_integer(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<integer>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

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
     * Converts integer value to string. For array values, the to_string()
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
     * Returns an array of integer contains all of the unique values in two or
     * more array of integer values.
     *
     * @param array $arrays Two or more arrays.
     *
     * @return array
     */
    public function union_integer(, $arrays)
    {
        $request = $this->$session->request();

        $request->set("function", "union<integer>");

        /* pass along required parameters to the request payload */
        $request->set("arrays", $arrays);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
