<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for double
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file double.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category double
 */
class double
{
    private $session;

    /**
     * Constructs the double class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * abs_double()
     *
     * Compute the absolute value of the double value and return the double
     * result.
     *
     * @param double $value
     *
     * @return double
     */
    public function abs_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "abs<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * add_double()
     *
     * Add 2 or more double values and return the double result.
     *
     * @param double $values
     *
     * @return double
     */
    public function add_double(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "add<double>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * at_least_one_member_of_double()
     *
     * Returns boolean true if at least one value in double array1 is in
     * double array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function at_least_one_member_of_double(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<double>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_double()
     *
     * Takes any number of double values and returns an array of array.
     *
     * @param array $values
     *
     * @return array
     */
    public function bag_double(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<double>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_double()
     *
     * This returns the integer number of values in array.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ceil_double()
     *
     * Determine the smallest integer that is greater then or equal to the
     * double value and return the double result.
     *
     * @param double $value
     *
     * @return double
     */
    public function ceil_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "ceil<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * divide_double()
     *
     * Divide double dividend by double divisor and return the double
     * quotient.
     *
     * @param double $dividend
     * @param double $divisor
     *
     * @return double
     */
    public function divide_double(, $dividend, $divisor)
    {
        $request = $this->$session->request();

        $request->set("function", "divide<double>");

        /* pass along required parameters to the request payload */
        $request->set("dividend", $dividend);
        $request->set("divisor", $divisor);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * double()
     *
     * Converts value to data type double returning double result.
     *
     * @param  $value Value to convert
     *
     * @return double Converted value
     */
    public function double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "double");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_double()
     *
     * Determine if double arg1 is equal to the value of arg2 converted to the
     * data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param double $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_double(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<double>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_double()
     *
     * Determine if for double arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param double $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_double(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<double>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * floor_double()
     *
     * Determine the largest integer that is smaller then or equal to the
     * double value and return the double result.
     *
     * @param double $number
     *
     * @return double
     */
    public function floor_double(, $number)
    {
        $request = $this->$session->request();

        $request->set("function", "floor<double>");

        /* pass along required parameters to the request payload */
        $request->set("number", $number);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_double()
     *
     * Checks for double arg1 is greater than or equal to double arg2 and
     * return the boolean result.
     *
     * @param double $arg1
     * @param double $arg2
     *
     * @return boolean
     */
    public function ge_double(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<double>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_double()
     *
     * Checks for double arg1 is greater than double arg2 and return the
     * boolean result.
     *
     * @param double $arg1
     * @param double $arg2
     *
     * @return boolean
     */
    public function gt_double(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<double>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * intersection_double()
     *
     * Returns an array of double with the values that are common to both
     * array of double array1 and array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return array
     */
    public function intersection_double(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<double>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_NaN()
     *
     * Checks if the argument 'number' is not a number(NaN) and returns the
     * boolean result.
     *
     * @param double $number Number to check
     *
     * @return boolean True if the argument 'number' is not a number.
     */
    public function is_NaN(, $number)
    {
        $request = $this->$session->request();

        $request->set("function", "is_NaN");

        /* pass along required parameters to the request payload */
        $request->set("number", $number);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_double()
     *
     * Checks whether value is dataType double and return the boolean result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_finite()
     *
     * Checks if the argument 'number' is finite and returns the boolean
     * result.
     *
     * @param double $number Number to check
     *
     * @return boolean True if the argument 'number' is finite.
     */
    public function is_finite(, $number)
    {
        $request = $this->$session->request();

        $request->set("function", "is_finite");

        /* pass along required parameters to the request payload */
        $request->set("number", $number);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_double()
     *
     * Checks whether double value is in array of double array and returns the
     * boolean result.
     *
     * @param double $value
     * @param array $array
     *
     * @return boolean
     */
    public function is_in_double(, $value, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_double()
     *
     * Checks for double arg1 is less than or equal to double arg2 and return
     * the boolean result.
     *
     * @param double $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_double(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<double>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_double()
     *
     * Checks for double arg1 is less that double arg2 and return the boolean
     * result.
     *
     * @param double $arg1
     * @param double $arg2
     *
     * @return boolean
     */
    public function lt_double(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<double>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * max_double()
     *
     * Return the double value that is greater than or equal to the others.
     *
     * @param double $values
     *
     * @return double
     */
    public function max_double(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "max<double>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * min_double()
     *
     * Return the double value that is less than or equal to the others.
     *
     * @param double $values
     *
     * @return double
     */
    public function min_double(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "min<double>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * multiply_double()
     *
     * Multiply 2 or more double values and return the double result.
     *
     * @param double $values
     *
     * @return double
     */
    public function multiply_double(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "multiply<double>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_double()
     *
     * Determine if double arg1 is not equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
     * instead if you want true to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param double $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_double(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<double>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * negative_double()
     *
     * Return negative of double value.
     *
     * @param double $value
     *
     * @return double
     */
    public function negative_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "negative<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_double()
     *
     * Determine if for double arg1 is not equal to the value or data type of
     * arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param double $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_double(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<double>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only_double()
     *
     * This converts an array of double values that contains one value to a
     * single double value.
     *
     * @param array $array
     *
     * @return double
     */
    public function one_and_only_double(, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<double>");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * pow_double()
     *
     * This returns the value of base raised to a power. Multiple exponents
     * can be specified to raise the previous exponent to the power of the
     * latter exponent.
     *
     * @param double $base Base value.
     * @param double $exponent Exponent value.
     *
     * @return double Base raised to the power.
     */
    public function pow_double(, $base, $exponent)
    {
        $request = $this->$session->request();

        $request->set("function", "pow<double>");

        /* pass along required parameters to the request payload */
        $request->set("base", $base);
        $request->set("exponent", $exponent);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * round_double()
     *
     * Determine the integer closest to double value and return the double
     * result.
     *
     * @param double $number
     *
     * @return double
     */
    public function round_double(, $number)
    {
        $request = $this->$session->request();

        $request->set("function", "round<double>");

        /* pass along required parameters to the request payload */
        $request->set("number", $number);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * set_equals_double()
     *
     * Returns boolean true if double array1 and double array2 are subsets of
     * each other and return the boolean result.
     *
     * @param array $array1
     * @param array $array2
     *
     * @return boolean
     */
    public function set_equals_double(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<double>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_double()
     *
     * Returns boolean true if the unique values in double array1 are all in
     * double array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function subset_double(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<double>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subtract_double()
     *
     * Subtract double arg2 from double arg1 and return the double result.
     *
     * @param double $arg1
     * @param double $arg2
     *
     * @return double
     */
    public function subtract_double(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "subtract<double>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_integer_double()
     *
     * Truncate double value to a whole number and returns integer result.
     *
     * @param double $value
     *
     * @return integer
     */
    public function to_integer_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_integer<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_double()
     *
     * Converts double value to string. For array values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param double $value A double value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * union_double()
     *
     * Returns an array of double contains all of the unique values in two or
     * more array of double values.
     *
     * @param array $arrays Two or more arrays.
     *
     * @return array
     */
    public function union_double(, $arrays)
    {
        $request = $this->$session->request();

        $request->set("function", "union<double>");

        /* pass along required parameters to the request payload */
        $request->set("arrays", $arrays);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
