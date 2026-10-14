<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for double
 *
 * Copyright (c) 2010-2024 Clemson University
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
     * acos_double()
     *
     * Return the arccosine, in radians from 0 to #pi, of the double value, as
     * ECMAScript Math.acos does. A value outside -1.0 to 1.0 is an error. A
     * result that would be NaN, or a NaN argument, is an error. An integer is
     * not accepted: write to_double(v).
     *
     * @param double $value
     *
     * @return double
     */
    public function acos_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "acos<double>");

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
     * array_double()
     *
     * Construct a new array of double from the given values, the same as
     * array() except that the new array is typed: each value, and each entry
     * of a spread array, must be dataType double, and adding a value of
     * another data type to the array later throws. A value of another data
     * type is an error, never converted. This is usually written
     * `array<double>(`...values).
     *
     * @param $values A value of dataType double, or ... followed by an
     *                 expression that results in an array, every entry of
     *                 which is included.
     *
     * @return array The constructed array of double.
     */
    public function array_double(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "array_double");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * as_double()
     *
     * Returns value unchanged if it is dataType double. Otherwise, including
     * when value is undefined, throws an error. It never converts: to get a
     * double from a value of another data type, write a typed literal or call
     * a to_T conversion function where one exists. This is usually written
     * value as double.
     *
     * @param $value Value to assert
     *
     * @return double value, which is dataType double
     */
    public function as_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "as_double");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * asin_double()
     *
     * Return the arcsine, in radians from -#pi/2 to #pi/2, of the double
     * value, as ECMAScript Math.asin does. A value outside -1.0 to 1.0 is an
     * error. A result that would be NaN, or a NaN argument, is an error. An
     * integer is not accepted: write to_double(v).
     *
     * @param double $value
     *
     * @return double
     */
    public function asin_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "asin<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

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
     * atan2_double()
     *
     * Return the angle, in radians from -#pi to #pi, from the positive x axis
     * to the point (x, y), as ECMAScript Math.atan2(y, x) does; y comes
     * first. A result that would be NaN, or a NaN argument, is an error. An
     * integer is not accepted: write to_double(v).
     *
     * @param double $y The y coordinate.
     * @param double $x The x coordinate.
     *
     * @return double
     */
    public function atan2_double(, $y, $x)
    {
        $request = $this->$session->request();

        $request->set("function", "atan2<double>");

        /* pass along required parameters to the request payload */
        $request->set("y", $y);
        $request->set("x", $x);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * atan_double()
     *
     * Return the arctangent, in radians from -#pi/2 to #pi/2, of the double
     * value, as ECMAScript Math.atan does. A result that would be NaN, or a
     * NaN argument, is an error. An integer is not accepted: write
     * to_double(v).
     *
     * @param double $value
     *
     * @return double
     */
    public function atan_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "atan<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * cbrt_double()
     *
     * Return the cube root of the double value, as ECMAScript Math.cbrt does.
     * A result that would be NaN, or a NaN argument, is an error. An integer
     * is not accepted: write to_double(v).
     *
     * @param double $value
     *
     * @return double
     */
    public function cbrt_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "cbrt<double>");

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
     * cos_double()
     *
     * Return the cosine of the double value in radians, as ECMAScript
     * Math.cos does (#pi is π). An infinite value is an error. A result that
     * would be NaN, or a NaN argument, is an error. An integer is not
     * accepted: write to_double(v).
     *
     * @param double $value
     *
     * @return double
     */
    public function cos_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "cos<double>");

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
     * eq_double()
     *
     * Determine if double arg1 is equal to the value of arg2 converted to the
     * data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param double $arg1
     * @param $arg2
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
     * @param $arg2
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
     * exp_double()
     *
     * Return #e raised to the double value, as ECMAScript Math.exp does. A
     * result that would be NaN, or a NaN argument, is an error. An integer is
     * not accepted: write to_double(v).
     *
     * @param double $value
     *
     * @return double
     */
    public function exp_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "exp<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

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
     * hypot_double()
     *
     * Return the square root of the sum of the squares of the double values,
     * as ECMAScript Math.hypot does, without overflow or underflow in
     * between. At least one value is required (ECMAScript's hypot() is 0); an
     * infinite value makes the result Infinity. A result that would be NaN,
     * or a NaN argument, is an error. An integer is not accepted: write
     * to_double(v).
     *
     * @param double $values The values.
     *
     * @return double
     */
    public function hypot_double(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "hypot<double>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

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
     * Checks whether value is dataType double and returns the boolean result.
     * It is false for undefined. This can also be written
     * `is<double>(value`).
     *
     * @param $value Value to check
     *
     * @return boolean
     */
    public function is_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is_double");

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
     * boolean result. Empty array yields false.
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
     * @param $arg2
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
     * log10_double()
     *
     * Return the base 10 logarithm of the double value, as ECMAScript
     * Math.log10 does. log10(0.0) is -Infinity; a negative value is an error.
     * A result that would be NaN, or a NaN argument, is an error. An integer
     * is not accepted: write to_double(v).
     *
     * @param double $value
     *
     * @return double
     */
    public function log10_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "log10<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * log2_double()
     *
     * Return the base 2 logarithm of the double value, as ECMAScript
     * Math.log2 does. log2(0.0) is -Infinity; a negative value is an error. A
     * result that would be NaN, or a NaN argument, is an error. An integer is
     * not accepted: write to_double(v).
     *
     * @param double $value
     *
     * @return double
     */
    public function log2_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "log2<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * log_double()
     *
     * Return the natural logarithm (base #e) of the double value, as
     * ECMAScript Math.log does. log(0.0) is -Infinity; a negative value is an
     * error. A result that would be NaN, or a NaN argument, is an error. An
     * integer is not accepted: write to_double(v).
     *
     * @param double $value
     *
     * @return double
     */
    public function log_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "log<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

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
     * Return the double value that is greater than or equal to the others. If
     * any value is NaN, the result is NaN, as ECMAScript Math.max does.
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
     * Return the double value that is less than or equal to the others. If
     * any value is NaN, the result is NaN, as ECMAScript Math.min does.
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
     * mod_double()
     *
     * Divide double dividend by double divisor and return the double
     * remainder.
     *
     * @param double $dividend
     * @param double $divisor
     *
     * @return double
     */
    public function mod_double(, $dividend, $divisor)
    {
        $request = $this->$session->request();

        $request->set("function", "mod<double>");

        /* pass along required parameters to the request payload */
        $request->set("dividend", $dividend);
        $request->set("divisor", $divisor);

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
     * @param $arg2
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
     * @param $arg2
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
     * sign_double()
     *
     * Return the sign of the double value, as ECMAScript Math.sign does: -1,
     * 0 or 1 of the same data type (`sign<double>` keeps the sign of a zero:
     * sign(-0.0) is -0.0). A NaN argument is an error.
     *
     * @param double $value
     *
     * @return double
     */
    public function sign_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "sign<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * sin_double()
     *
     * Return the sine of the double value in radians, as ECMAScript Math.sin
     * does (#pi is π). An infinite value is an error. A result that would be
     * NaN, or a NaN argument, is an error. An integer is not accepted: write
     * to_double(v).
     *
     * @param double $value
     *
     * @return double
     */
    public function sin_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "sin<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * sqrt_double()
     *
     * Return the square root of the double value, as ECMAScript Math.sqrt
     * does; sqrt(-0.0) is -0.0. A negative value is an error. A result that
     * would be NaN, or a NaN argument, is an error. An integer is not
     * accepted: write to_double(v).
     *
     * @param double $value
     *
     * @return double
     */
    public function sqrt_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "sqrt<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

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
     * tan_double()
     *
     * Return the tangent of the double value in radians, as ECMAScript
     * Math.tan does (#pi is π). An infinite value is an error. A result that
     * would be NaN, or a NaN argument, is an error. An integer is not
     * accepted: write to_double(v).
     *
     * @param double $value
     *
     * @return double
     */
    public function tan_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "tan<double>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_integer_double()
     *
     * Converts double value to integer. A double is truncated toward zero
     * (1.9 is 1 and -1.9 is -1); use round(), floor(), or ceil() first for
     * another choice. A double that is NaN, infinite, or outside the integer
     * range is an error. A string is read as integer text, and text that is
     * not an integer ("1.5", " 1", "1x") is an error.
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
     * trunc_double()
     *
     * Return the integer part of the double value, removing any fraction, as
     * ECMAScript Math.trunc does (toward zero: trunc(-1.5) is -1.0). The
     * result is a double: write to_integer(trunc(v)) for an integer. A result
     * that would be NaN, or a NaN argument, is an error. An integer is not
     * accepted: write to_double(v).
     *
     * @param double $value
     *
     * @return double
     */
    public function trunc_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "trunc<double>");

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
