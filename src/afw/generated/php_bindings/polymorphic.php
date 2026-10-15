<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for polymorphic
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file polymorphic.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category polymorphic
 */
class polymorphic
{
    private $session;

    /**
     * Constructs the polymorphic class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * abs()
     *
     * Compute the absolute value of the `<dataType>` value and return the
     * `<dataType>` result.
     *
     * @param $value
     *
     * @return
     */
    public function abs(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "abs");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * acos()
     *
     * Return the arccosine, in radians from 0 to #pi, of the `<dataType>`
     * value. A value outside -1.0 to 1.0 gives NaN (an error under #compile
     * strictDouble). A NaN argument gives NaN. An integer is not accepted:
     * write to_double(v).
     *
     * @param $value
     *
     * @return
     */
    public function acos(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "acos");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * add()
     *
     * Add 2 or more `<dataType>` values and return the `<dataType>` result.
     *
     * @param $values
     *
     * @return
     */
    public function add(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "add");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * add_dayTimeDuration()
     *
     * Add `<dataType>` arg1 to dayTimeDuration arg2 and return the
     * `<dataType>` result.
     *
     * @param $arg1
     * @param dayTimeDuration $arg2
     *
     * @return
     */
    public function add_dayTimeDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "add_dayTimeDuration");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * add_yearMonthDuration()
     *
     * Add `<dataType>` arg1 to yearMonthDuration arg2 and return the
     * `<dataType>` result.
     *
     * @param $arg1
     * @param yearMonthDuration $arg2
     *
     * @return
     */
    public function add_yearMonthDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "add_yearMonthDuration");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * asin()
     *
     * Return the arcsine, in radians from -#pi/2 to #pi/2, of the
     * `<dataType>` value. A value outside -1.0 to 1.0 gives NaN (an error
     * under #compile strictDouble). A NaN argument gives NaN. An integer is
     * not accepted: write to_double(v).
     *
     * @param $value
     *
     * @return
     */
    public function asin(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "asin");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * at_least_one_member_of()
     *
     * Returns boolean true if at least one value in `<dataType>` array1 is in
     * `<dataType>` array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function at_least_one_member_of(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * atan()
     *
     * Return the arctangent, in radians from -#pi/2 to #pi/2, of the
     * `<dataType>` value. A NaN argument gives NaN. An integer is not
     * accepted: write to_double(v).
     *
     * @param $value
     *
     * @return
     */
    public function atan(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "atan");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * atan2()
     *
     * Return the angle, in radians from -#pi to #pi, from the positive x axis
     * to the point (x, y); y comes first. A NaN argument gives NaN. An
     * integer is not accepted: write to_double(v).
     *
     * @param $y The y coordinate.
     * @param $x The x coordinate.
     *
     * @return
     */
    public function atan2(, $y, $x)
    {
        $request = $this->$session->request();

        $request->set("function", "atan2");

        /* pass along required parameters to the request payload */
        $request->set("y", $y);
        $request->set("x", $x);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * cbrt()
     *
     * Return the cube root of the `<dataType>` value. A NaN argument gives
     * NaN. An integer is not accepted: write to_double(v).
     *
     * @param $value
     *
     * @return
     */
    public function cbrt(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "cbrt");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ceil()
     *
     * Determine the smallest integer that is greater then or equal to the
     * `<dataType>` value and return the `<dataType>` result.
     *
     * @param $value
     *
     * @return
     */
    public function ceil(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "ceil");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * clone()
     *
     * Deep clone a `<dataType>` value.
     *
     * @param $value The `<dataType>` value to clone.
     *
     * @return The cloned `<dataType>` value.
     */
    public function clone(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "clone");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * code_point_at()
     *
     * Return the code point at a zero-based index of the `<dataType>` value
     * as an integer. Indexes count code points, as everywhere in Adaptive
     * Script. Negative indexes count from the end, as at does. If the index
     * is out of range, the result is undefined, as for at.
     *
     * @param $value The `<dataType>` value.
     * @param integer $index Zero-based code point index, or negative from the
     *                       end.
     *
     * @return integer The code point, or undefined if index is out of range.
     */
    public function code_point_at(, $value, $index)
    {
        $request = $this->$session->request();

        $request->set("function", "code_point_at");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("index", $index);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * compile()
     *
     * Compile `<dataType>` value and return either an unevaluated adaptive
     * value or a string containing the compiler listing. The listing is a
     * human-oriented dump (value tree interleaved with source, plus
     * ---Symbols tables) for Fiddle and debugging — not pure JSON (use
     * stringify) and not Adaptive compiled-form text (use decompile).
     *
     * @param $source `<dataType>` string to compile
     * @param $listing If specified, a human compiler listing is produced
     *                  instead of an unevaluated compiled value (tree +
     *                  ---Symbols; not recompilable). Use decompile() for
     *                  Adaptive compiled-form text and stringify() for pure
     *                  JSON of evaluated data.
     * 
     *                  This parameter can be an integer between 0 and 10 or a
     *                  string that is used for indentation. If 0 is
     *                  specified, no whitespace is added to the resulting
     *                  string. If 1 through 10 is specified, that number of
     *                  spaces is used.
     *
     * @return unevaluated
     */
    public function compile(, $source, $listing = null)
    {
        $request = $this->$session->request();

        $request->set("function", "compile");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($listing != null)
            $request->set('listing', $listing);

        return $request->get_result();
    }

    /**
     * cos()
     *
     * Return the cosine of the `<dataType>` value in radians (#pi is π). An
     * infinite value gives NaN (an error under #compile strictDouble). A NaN
     * argument gives NaN. An integer is not accepted: write to_double(v).
     *
     * @param $value
     *
     * @return
     */
    public function cos(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "cos");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * decode_to_string()
     *
     * Decode `<dataType>` value to string. An error is thrown if decoded
     * value is not valid UTF-8.
     *
     * @param $value The `<dataType>` value to decode.
     *
     * @return string The decoded string.
     */
    public function decode_to_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "decode_to_string");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * divide()
     *
     * Divide `<dataType>` dividend by `<dataType>` divisor and return the
     * `<dataType>` quotient.
     *
     * @param $dividend
     * @param $divisor
     *
     * @return
     */
    public function divide(, $dividend, $divisor)
    {
        $request = $this->$session->request();

        $request->set("function", "divide");

        /* pass along required parameters to the request payload */
        $request->set("dividend", $dividend);
        $request->set("divisor", $divisor);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * encode_as_base64Binary()
     *
     * Encode a value as a base64Binary. The effect is to create a
     * base64Binary value with an internal value of the value passed.
     *
     * @param $value The `<dataType>` value to encode.
     *
     * @return base64Binary A base64Binary value.
     */
    public function encode_as_base64Binary(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "encode_as_base64Binary");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * encode_as_hexBinary()
     *
     * Encode a value as a hexBinary. The effect is to create a hexBinary
     * value with an internal value of the value passed.
     *
     * @param $value The `<dataType>` value to encode.
     *
     * @return hexBinary A hexBinary value.
     */
    public function encode_as_hexBinary(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "encode_as_hexBinary");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ends_with()
     *
     * Checks whether `<dataType>` value ends with a `<dataType>` and return
     * the boolean result.
     *
     * @param $value
     * @param string $subString
     *
     * @return boolean
     */
    public function ends_with(, $value, $subString)
    {
        $request = $this->$session->request();

        $request->set("function", "ends_with");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("subString", $subString);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq()
     *
     * Determine if `<dataType>` arg1 is equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use 'eqx'
     * ('===') instead if you want false to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function eq(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_ignore_case()
     *
     * Checks for `<dataType>` arg1 is equal to `<dataType>` arg2 ignoring
     * case and return the boolean result.
     *
     * @param $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function eq_ignore_case(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq_ignore_case");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx()
     *
     * Determine if for `<dataType>` arg1 is equal to the value and data type
     * of arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function eqx(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eval()
     *
     * Compile and evaluate `<dataType>` value.
     *
     * @param $source `<dataType>` to compile and evaluate
     * @param object $additionalUntrustedQualifiedVariables This parameter
     *                                                      supplies
     *                                                      additional
     *                                                      qualified
     *                                                      variables that can
     *                                                      be accessed during
     *                                                      evaluation. These
     *                                                      variables will not
     *                                                      be used by
     *                                                      anything that
     *                                                      needs to ensure
     *                                                      its qualified
     *                                                      variables must
     *                                                      come from a
     *                                                      trusted source,
     *                                                      such as
     *                                                      authorization.
     *                                                      This parameter is
     *                                                      intended to be
     *                                                      used for testing
     *                                                      only and should
     *                                                      not be used for
     *                                                      anything running
     *                                                      in production.
     *
     * @return any
     */
    public function eval(, $source, $additionalUntrustedQualifiedVariables = null)
    {
        $request = $this->$session->request();

        $request->set("function", "eval");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($additionalUntrustedQualifiedVariables != null)
            $request->set('additionalUntrustedQualifiedVariables', $additionalUntrustedQualifiedVariables);

        return $request->get_result();
    }

    /**
     * exp()
     *
     * Return #e raised to the `<dataType>` value. A result too large is
     * Infinity. A NaN argument gives NaN. An integer is not accepted: write
     * to_double(v).
     *
     * @param $value
     *
     * @return
     */
    public function exp(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "exp");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * floor()
     *
     * Determine the largest integer that is smaller then or equal to the
     * `<dataType>` value and return the `<dataType>` result.
     *
     * @param $number
     *
     * @return
     */
    public function floor(, $number)
    {
        $request = $this->$session->request();

        $request->set("function", "floor");

        /* pass along required parameters to the request payload */
        $request->set("number", $number);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * freeze()
     *
     * Set a `<dataType>` value immutable so further mutation throws. If
     * already immutable, has no effect. Returns the same value.
     *
     * @param $value The `<dataType>` value to freeze.
     *
     * @return The same value, now immutable.
     */
    public function freeze(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "freeze");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge()
     *
     * Checks for `<dataType>` arg1 is greater than or equal to `<dataType>`
     * arg2 and return the boolean result.
     *
     * @param $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function ge(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt()
     *
     * Checks for `<dataType>` arg1 is greater than `<dataType>` arg2 and
     * return the boolean result.
     *
     * @param $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function gt(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * hypot()
     *
     * Return the square root of the sum of the squares of the `<dataType>`
     * values, without overflow or underflow in between. At least one value is
     * required; an infinite value makes the result Infinity, even with a NaN
     * value; otherwise a NaN value gives NaN. An integer is not accepted:
     * write to_double(v).
     *
     * @param $values The values.
     *
     * @return
     */
    public function hypot(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "hypot");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * in_range()
     *
     * Checks if `<dataType>` is between startTime and endTime, inclusive.
     * Regardless of endTime value, it is always considered to be equal to,
     * but less than 24 hours greater than startTime. If no `<dataType>` zone
     * is specified for `<dataType>,` the default `<dataType>` zone is used.
     * If no `<dataType>` zone is specified for startTime or endTime, the
     * `<dataType>` zone of `<dataType>` is used.
     *
     * @param $time
     * @param $startTime
     * @param $endTime
     *
     * @return boolean
     */
    public function in_range(, $time, $startTime, $endTime)
    {
        $request = $this->$session->request();

        $request->set("function", "in_range");

        /* pass along required parameters to the request payload */
        $request->set("time", $time);
        $request->set("startTime", $startTime);
        $request->set("endTime", $endTime);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * includes()
     *
     * Checks whether the `<dataType>` value includes a string and return the
     * boolean result.
     *
     * @param $searchString The `<dataType>` to search.
     * @param string $subString Substring to find.
     * @param integer $position Zero-based position in the search string to
     *                          start search.
     *
     * @return boolean Indicates if the substring is contained in the search
     *                 string.
     */
    public function includes(, $searchString, $subString, $position = null)
    {
        $request = $this->$session->request();

        $request->set("function", "includes");

        /* pass along required parameters to the request payload */
        $request->set("searchString", $searchString);
        $request->set("subString", $subString);

        /* pass along any optional parameters to the request payload */
        if ($position != null)
            $request->set('position', $position);

        return $request->get_result();
    }

    /**
     * index_of()
     *
     * Returns the zero-based index into `<dataType>` value of subString. If
     * subString is not found, -1 is returned.
     *
     * @param $value The `<dataType>` value to search.
     * @param string $subString Substring to search for.
     * @param integer $startIndex Optional start index for search if different
     *                            than start of string.
     *
     * @return integer Zero-based index of subString or -1 if not found.
     */
    public function index_of(, $value, $subString, $startIndex = null)
    {
        $request = $this->$session->request();

        $request->set("function", "index_of");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("subString", $subString);

        /* pass along any optional parameters to the request payload */
        if ($startIndex != null)
            $request->set('startIndex', $startIndex);

        return $request->get_result();
    }

    /**
     * intersection()
     *
     * Returns an array of `<dataType>` with the values that are common to
     * both array of `<dataType>` array1 and array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return array
     */
    public function intersection(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in()
     *
     * Checks whether `<dataType>` value is in array of `<dataType>` array and
     * returns the boolean result. Empty array yields false.
     *
     * @param $value
     * @param array $array
     *
     * @return boolean
     */
    public function is_in(, $value, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * last_index_of()
     *
     * Returns the zero-based index into `<dataType>` value of the last
     * occurrence of a subString. If subString is not found, -1 is returned.
     *
     * @param $value The `<dataType>` value to search.
     * @param string $subString Substring to search for.
     * @param integer $startIndex Optional start index for search if different
     *                            than start of string.
     *
     * @return integer Zero-based index of subString or -1 if not found.
     */
    public function last_index_of(, $value, $subString, $startIndex = null)
    {
        $request = $this->$session->request();

        $request->set("function", "last_index_of");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("subString", $subString);

        /* pass along any optional parameters to the request payload */
        if ($startIndex != null)
            $request->set('startIndex', $startIndex);

        return $request->get_result();
    }

    /**
     * le()
     *
     * Checks for `<dataType>` arg1 is less than or equal to `<dataType>` arg2
     * and return the boolean result.
     *
     * @param $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function le(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * length()
     *
     * Return the integer number of entries in a `<dataType>` array or the
     * number of codepoints in a `<dataType>` anyURI or string.
     *
     * @param $value Returns the number of entries in an array or code points
     *                in others.
     *
     * @return integer
     */
    public function length(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "length");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * log()
     *
     * Return the natural logarithm (base #e) of the `<dataType>` value.
     * log(0.0) is -Infinity and a negative value gives NaN; under #compile
     * strictDouble both are errors. A NaN argument gives NaN. An integer is
     * not accepted: write to_double(v).
     *
     * @param $value
     *
     * @return
     */
    public function log(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "log");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * log10()
     *
     * Return the base 10 logarithm of the `<dataType>` value. log10(0.0) is
     * -Infinity and a negative value gives NaN; under #compile strictDouble
     * both are errors. A NaN argument gives NaN. An integer is not accepted:
     * write to_double(v).
     *
     * @param $value
     *
     * @return
     */
    public function log10(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "log10");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * log2()
     *
     * Return the base 2 logarithm of the `<dataType>` value. log2(0.0) is
     * -Infinity and a negative value gives NaN; under #compile strictDouble
     * both are errors. A NaN argument gives NaN. An integer is not accepted:
     * write to_double(v).
     *
     * @param $value
     *
     * @return
     */
    public function log2(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "log2");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt()
     *
     * Checks for `<dataType>` arg1 is less that `<dataType>` arg2 and return
     * the boolean result.
     *
     * @param $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function lt(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * match()
     *
     * Searches `<dataType>` value for the regular expression and returns its
     * first match with the text of its groups, or undefined if there is none.
     * Use test for a boolean result.
     *
     * @param $value The `<dataType>` value to search.
     * @param regexp $regexp The regular expression. A regexp literal (/a+b/
     *                       or "a+b"re), to_regexp(s), or
     *                       `compile<regexp>(r`) is already compiled; any
     *                       other regexp value is compiled for this call.
     *
     * @return object The first match, or undefined if there is none.
     */
    public function match(, $value, $regexp)
    {
        $request = $this->$session->request();

        $request->set("function", "match");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * match_all()
     *
     * Searches `<dataType>` value for the regular expression and returns each
     * match with the text of its groups, left to right. Matches do not
     * overlap; after an empty match the next search starts one code point
     * later. length(match_all(s, re)) counts the matches.
     *
     * @param $value The `<dataType>` value to search.
     * @param regexp $regexp The regular expression. A regexp literal (/a+b/
     *                       or "a+b"re), to_regexp(s), or
     *                       `compile<regexp>(r`) is already compiled; any
     *                       other regexp value is compiled for this call.
     * @param integer $limit The maximum number of matches to return. The
     *                       default, or any negative limit, returns every
     *                       match.
     *
     * @return array The matches, empty if there are none.
     */
    public function match_all(, $value, $regexp, $limit = null)
    {
        $request = $this->$session->request();

        $request->set("function", "match_all");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);

        /* pass along any optional parameters to the request payload */
        if ($limit != null)
            $request->set('limit', $limit);

        return $request->get_result();
    }

    /**
     * max()
     *
     * Return the `<dataType>` value that is greater than or equal to the
     * others.
     *
     * @param $values
     *
     * @return
     */
    public function max(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "max");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * min()
     *
     * Return the `<dataType>` value that is less than or equal to the others.
     *
     * @param $values
     *
     * @return
     */
    public function min(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "min");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * mod()
     *
     * Divide `<dataType>` dividend by `<dataType>` divisor and return the
     * `<dataType>` remainder.
     *
     * @param $dividend
     * @param $divisor
     *
     * @return
     */
    public function mod(, $dividend, $divisor)
    {
        $request = $this->$session->request();

        $request->set("function", "mod");

        /* pass along required parameters to the request payload */
        $request->set("dividend", $dividend);
        $request->set("divisor", $divisor);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * multiply()
     *
     * Multiply 2 or more `<dataType>` values and return the `<dataType>`
     * result.
     *
     * @param $values
     *
     * @return
     */
    public function multiply(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "multiply");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne()
     *
     * Determine if `<dataType>` arg1 is not equal to the value of arg2
     * converted to the data type of arg1 then return the boolean result. Use
     * 'nex' ('!==') instead if you want true to be returned if arg1 and
     * arg2's data type don't match.
     *
     * @param $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function ne(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * negative()
     *
     * Return negative of `<dataType>` value.
     *
     * @param $value
     *
     * @return
     */
    public function negative(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "negative");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex()
     *
     * Determine if for `<dataType>` arg1 is not equal to the value or data
     * type of arg2 then return the boolean result. Use 'ne' ('!=') instead if
     * you want arg2 to be converted to the data type of arg1 before
     * comparison.
     *
     * @param $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function nex(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * normalize()
     *
     * Return the `<dataType>` value in a Unicode normalization form. Every
     * Adaptive string is already NFC, so form "NFC" (the default) returns it
     * as a string; "NFKC" also replaces compatibility characters ("ﬁ" becomes
     * "fi", "①" becomes "1"). "NFD" and "NFKD" are errors: a string in
     * Adaptive Script is always NFC, so a decomposed result can not be a
     * string.
     *
     * @param $value The `<dataType>` value.
     * @param string $form "NFC" (the default) or "NFKC".
     *
     * @return string The normalized value, as a string.
     */
    public function normalize(, $value, $form = null)
    {
        $request = $this->$session->request();

        $request->set("function", "normalize");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        if ($form != null)
            $request->set('form', $form);

        return $request->get_result();
    }

    /**
     * normalize_space()
     *
     * Remove whitespace from the beginning and end of a `<dataType>` value.
     *
     * @param $string
     *
     * @return
     */
    public function normalize_space(, $string)
    {
        $request = $this->$session->request();

        $request->set("function", "normalize_space");

        /* pass along required parameters to the request payload */
        $request->set("string", $string);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * normalize_to_lower_case()
     *
     * Normalize `<dataType>` value to lower case and returns `<dataType>`
     * result.
     *
     * @param $string
     *
     * @return
     */
    public function normalize_to_lower_case(, $string)
    {
        $request = $this->$session->request();

        $request->set("function", "normalize_to_lower_case");

        /* pass along required parameters to the request payload */
        $request->set("string", $string);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only()
     *
     * This converts an array of `<dataType>` values that contains one value
     * to a single `<dataType>` value.
     *
     * @param array $array
     *
     * @return
     */
    public function one_and_only(, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * pad_end()
     *
     * Pad the `<dataType>` value at the end with fillString until it is
     * maxLength code points long. An empty fillString when padding is needed
     * is an error. Where the fill and the value meet they can compose, since
     * every string is NFC, so the result can be shorter than maxLength.
     *
     * @param $value The `<dataType>` value.
     * @param integer $maxLength The length of the result in code points, at
     *                           most 1,000,000. If it is not more than the
     *                           length of value, value is returned (as a
     *                           string).
     * @param string $fillString The text to pad with, repeated and cut to fit
     *                           (by code point). The default is one space. An
     *                           empty fillString is an error when padding is
     *                           needed.
     *
     * @return string The padded value, as a string.
     */
    public function pad_end(, $value, $maxLength, $fillString = null)
    {
        $request = $this->$session->request();

        $request->set("function", "pad_end");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("maxLength", $maxLength);

        /* pass along any optional parameters to the request payload */
        if ($fillString != null)
            $request->set('fillString', $fillString);

        return $request->get_result();
    }

    /**
     * pad_start()
     *
     * Pad the `<dataType>` value at the start with fillString until it is
     * maxLength code points long. An empty fillString when padding is needed
     * is an error. Where the fill and the value meet they can compose, since
     * every string is NFC, so the result can be shorter than maxLength.
     *
     * @param $value The `<dataType>` value.
     * @param integer $maxLength The length of the result in code points, at
     *                           most 1,000,000. If it is not more than the
     *                           length of value, value is returned (as a
     *                           string).
     * @param string $fillString The text to pad with, repeated and cut to fit
     *                           (by code point). The default is one space. An
     *                           empty fillString is an error when padding is
     *                           needed.
     *
     * @return string The padded value, as a string.
     */
    public function pad_start(, $value, $maxLength, $fillString = null)
    {
        $request = $this->$session->request();

        $request->set("function", "pad_start");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("maxLength", $maxLength);

        /* pass along any optional parameters to the request payload */
        if ($fillString != null)
            $request->set('fillString', $fillString);

        return $request->get_result();
    }

    /**
     * pow()
     *
     * This returns the value of base raised to a power. Multiple exponents
     * can be specified to raise the previous exponent to the power of the
     * latter exponent. An integer result is exact: a negative exponent or a
     * result too large for an integer is an error.
     *
     * @param $base Base value.
     * @param $exponent Exponent value.
     *
     * @return Base raised to the power.
     */
    public function pow(, $base, $exponent)
    {
        $request = $this->$session->request();

        $request->set("function", "pow");

        /* pass along required parameters to the request payload */
        $request->set("base", $base);
        $request->set("exponent", $exponent);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * repeat()
     *
     * Repeat a `<dataType>` value a specified number of times.
     *
     * @param $value The `<dataType>` value to repeat.
     * @param integer $times The number of times to repeat the value.
     *
     * @return string Repeated text as string (not re-typed as the input data
     *                type).
     */
    public function repeat(, $value, $times)
    {
        $request = $this->$session->request();

        $request->set("function", "repeat");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("times", $times);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * replace()
     *
     * Replace matches in a `<dataType>` value, left to right, up to limit
     * times. search is a string, which is literal text, or a regexp, which is
     * a pattern. The replacement is a string, or a function called with each
     * match's _AdaptiveRegexpMatch_ object that returns the string to put in.
     * An empty match is a match at a code-point boundary. The default limit
     * replaces once (an empty string inserts at the start). Specify limit -1
     * to replace every match.
     *
     * @param $value The original `<dataType>` value.
     * @param $search What to replace: a string is literal text, and an empty
     *                 string matches at each code-point boundary, including
     *                 the start and the end; a regexp is a pattern. A regexp
     *                 literal (/a+b/ or "a+b"re), to_regexp(s), or
     *                 `compile<regexp>(r`) is already compiled; any other
     *                 regexp value is compiled for this call.
     * @param $replacement The replacement for each match: a string, or a
     *                      function. A function is called with one argument,
     *                      the match's _AdaptiveRegexpMatch_ object ({match,
     *                      index, captures, groups}), and must return a
     *                      string. A string replacement is literal text when
     *                      search is a string. When search is a regexp, $n is
     *                      the text of group n ($0 the whole match), ${name}
     *                      the text of a named group, and a backslash makes
     *                      the next character literal (\$ for a dollar sign);
     *                      any other $ is an error.
     * @param integer $limit The maximum number of matches to replace. The
     *                       default is 1. Specify -1 (or any negative) to
     *                       replace every match.
     *
     * @return string Result text as string (not re-typed as the input data
     *                type).
     */
    public function replace(, $value, $search, $replacement, $limit = null)
    {
        $request = $this->$session->request();

        $request->set("function", "replace");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("search", $search);
        $request->set("replacement", $replacement);

        /* pass along any optional parameters to the request payload */
        if ($limit != null)
            $request->set('limit', $limit);

        return $request->get_result();
    }

    /**
     * replace_all()
     *
     * Replace every match in a `<dataType>` value, left to right; the same as
     * replace(value, search, replacement, -1). search is a string, which is
     * literal text, or a regexp, which is a pattern. The replacement is a
     * string, or a function called with each match's _AdaptiveRegexpMatch_
     * object that returns the string to put in.
     *
     * @param $value The `<dataType>` value.
     * @param $search What to replace: a string is literal text, and an empty
     *                 string matches at each code-point boundary, including
     *                 the start and the end; a regexp is a pattern. A regexp
     *                 literal (/a+b/ or "a+b"re), to_regexp(s), or
     *                 `compile<regexp>(r`) is already compiled; any other
     *                 regexp value is compiled for this call.
     * @param $replacement The replacement for each match: a string, or a
     *                      function. A function is called with one argument,
     *                      the match's _AdaptiveRegexpMatch_ object ({match,
     *                      index, captures, groups}), and must return a
     *                      string. A string replacement is literal text when
     *                      search is a string. When search is a regexp, $n is
     *                      the text of group n ($0 the whole match), ${name}
     *                      the text of a named group, and a backslash makes
     *                      the next character literal (\$ for a dollar sign);
     *                      any other $ is an error.
     *
     * @return string The value with every match replaced, as a string.
     */
    public function replace_all(, $value, $search, $replacement)
    {
        $request = $this->$session->request();

        $request->set("function", "replace_all");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("search", $search);
        $request->set("replacement", $replacement);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * round()
     *
     * Determine the integer closest to `<dataType>` value and return the
     * `<dataType>` result. A value halfway between two integers rounds toward
     * positive infinity: round(2.5) is 3 and round(-2.5) is -2.
     *
     * @param $number
     *
     * @return
     */
    public function round(, $number)
    {
        $request = $this->$session->request();

        $request->set("function", "round");

        /* pass along required parameters to the request payload */
        $request->set("number", $number);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * search()
     *
     * Searches `<dataType>` value for the regular expression and returns the
     * code point index of the first match, or -1 if there is none. Use
     * index_of to find literal text.
     *
     * @param $value The `<dataType>` value to search.
     * @param regexp $regexp The regular expression. A regexp literal (/a+b/
     *                       or "a+b"re), to_regexp(s), or
     *                       `compile<regexp>(r`) is already compiled; any
     *                       other regexp value is compiled for this call.
     *
     * @return integer Zero-based code point index of the first match, or -1.
     */
    public function search(, $value, $regexp)
    {
        $request = $this->$session->request();

        $request->set("function", "search");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * set_equals()
     *
     * Returns boolean true if `<dataType>` array1 and `<dataType>` array2 are
     * subsets of each other and return the boolean result.
     *
     * @param array $array1
     * @param array $array2
     *
     * @return boolean
     */
    public function set_equals(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * sign()
     *
     * Return the sign of the `<dataType>` value: -1, 0 or 1 of the same data
     * type (`sign<double>` keeps the sign of a zero: sign(-0.0) is -0.0). A
     * NaN argument gives NaN.
     *
     * @param $value
     *
     * @return
     */
    public function sign(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "sign");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * sin()
     *
     * Return the sine of the `<dataType>` value in radians (#pi is π). An
     * infinite value gives NaN (an error under #compile strictDouble). A NaN
     * argument gives NaN. An integer is not accepted: write to_double(v).
     *
     * @param $value
     *
     * @return
     */
    public function sin(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "sin");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * split()
     *
     * Split `<dataType>` value into an array of strings. separator is a
     * string, which is literal text, or a regexp, which is a pattern. With a
     * regexp, the text of each group of a match is put in the array after the
     * piece before it (undefined when the group did not take part), and an
     * empty match splits between code points but not at the start or end of
     * the value.
     *
     * @param $value The `<dataType>` value to split.
     * @param $separator Where to split: a string is literal text, a regexp is
     *                    a pattern. If this is an empty string or separator
     *                    is not specified, the value is split between
     *                    characters. A regexp literal (/a+b/ or "a+b"re),
     *                    to_regexp(s), or `compile<regexp>(r`) is already
     *                    compiled; any other regexp value is compiled for
     *                    this call.
     * @param integer $limit The maximum number of entries in the result. Any
     *                       remaining part of value is ignored. The default,
     *                       or any negative limit, is no limit.
     *
     * @return array An array of strings.
     */
    public function split(, $value, $separator = null, $limit = null)
    {
        $request = $this->$session->request();

        $request->set("function", "split");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        if ($separator != null)
            $request->set('separator', $separator);

        if ($limit != null)
            $request->set('limit', $limit);

        return $request->get_result();
    }

    /**
     * sqrt()
     *
     * Return the square root of the `<dataType>` value; sqrt(-0.0) is -0.0. A
     * negative value gives NaN (an error under #compile strictDouble). A NaN
     * argument gives NaN. An integer is not accepted: write to_double(v).
     *
     * @param $value
     *
     * @return
     */
    public function sqrt(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "sqrt");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * starts_with()
     *
     * Checks whether `<dataType>` value starts with a subString and return
     * the boolean result.
     *
     * @param $value
     * @param string $subString
     *
     * @return boolean
     */
    public function starts_with(, $value, $subString)
    {
        $request = $this->$session->request();

        $request->set("function", "starts_with");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("subString", $subString);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset()
     *
     * Returns boolean true if the unique values in `<dataType>` array1 are
     * all in `<dataType>` array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function subset(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * substring()
     *
     * Returns the string substring of value from zero-based code point
     * position integer startIndex up to, but not including, endIndex. A
     * negative index is 0 and an index past the end is the end of value. Omit
     * endIndex to return through the end of value. If endIndex is less than
     * startIndex, the two are swapped, and equal indexes return an empty
     * string. Use slice to count from the end. The result is always string (a
     * slice of anyURI is not an anyURI).
     *
     * @param $string
     * @param integer $startIndex
     * @param integer $endIndex
     *
     * @return string Substring as string (not re-typed as the input data
     *                type).
     */
    public function substring(, $string, $startIndex, $endIndex = null)
    {
        $request = $this->$session->request();

        $request->set("function", "substring");

        /* pass along required parameters to the request payload */
        $request->set("string", $string);
        $request->set("startIndex", $startIndex);

        /* pass along any optional parameters to the request payload */
        if ($endIndex != null)
            $request->set('endIndex', $endIndex);

        return $request->get_result();
    }

    /**
     * subtract()
     *
     * Subtract `<dataType>` arg2 from `<dataType>` arg1 and return the
     * `<dataType>` result.
     *
     * @param $arg1
     * @param $arg2
     *
     * @return
     */
    public function subtract(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "subtract");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subtract_dayTimeDuration()
     *
     * Subtract dayTimeDuration arg2 from `<dataType>` arg1 and return the
     * `<dataType>` result.
     *
     * @param $arg1
     * @param dayTimeDuration $arg2
     *
     * @return
     */
    public function subtract_dayTimeDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "subtract_dayTimeDuration");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subtract_yearMonthDuration()
     *
     * Subtract yearMonthDuration arg2 from `<dataType>` arg1 and return the
     * `<dataType>` result.
     *
     * @param $arg1
     * @param yearMonthDuration $arg2
     *
     * @return
     */
    public function subtract_yearMonthDuration(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "subtract_yearMonthDuration");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * tan()
     *
     * Return the tangent of the `<dataType>` value in radians (#pi is π). An
     * infinite value gives NaN (an error under #compile strictDouble). A NaN
     * argument gives NaN. An integer is not accepted: write to_double(v).
     *
     * @param $value
     *
     * @return
     */
    public function tan(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "tan");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_anyURI()
     *
     * Converts `<dataType>` value to anyURI. A string is read with the anyURI
     * parser, the same parser the typed literal "..."anyURI uses, and text
     * the parser rejects is an error.
     *
     * @param $value
     *
     * @return anyURI
     */
    public function to_anyURI(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_anyURI");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_base64Binary()
     *
     * Converts `<dataType>` value to base64Binary. A string is read with the
     * base64Binary parser, the same parser the typed literal
     * "..."base64Binary uses, and text the parser rejects is an error.
     *
     * @param $value
     *
     * @return base64Binary
     */
    public function to_base64Binary(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_base64Binary");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_date()
     *
     * Converts `<dataType>` value to date. A string is read with the date
     * parser, the same parser the typed literal "..."date uses, and text the
     * parser rejects is an error.
     *
     * @param $value
     *
     * @return date
     */
    public function to_date(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_date");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_dateTime()
     *
     * Converts `<dataType>` value to dateTime. A string is read with the
     * dateTime parser, the same parser the typed literal "..."dateTime uses,
     * and text the parser rejects is an error.
     *
     * @param $value
     *
     * @return dateTime
     */
    public function to_dateTime(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_dateTime");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_dayTimeDuration()
     *
     * Converts `<dataType>` value to dayTimeDuration. A string is read with
     * the dayTimeDuration parser, the same parser the typed literal
     * "..."dayTimeDuration uses, and text the parser rejects is an error.
     *
     * @param $value
     *
     * @return dayTimeDuration
     */
    public function to_dayTimeDuration(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_dayTimeDuration");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_dnsName()
     *
     * Converts `<dataType>` value to dnsName. A string is read with the
     * dnsName parser, the same parser the typed literal "..."dnsName uses,
     * and text the parser rejects is an error.
     *
     * @param $value
     *
     * @return dnsName
     */
    public function to_dnsName(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_dnsName");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_double()
     *
     * Converts `<dataType>` value to double. A string is read as XML Schema
     * double text: digits with an optional sign, decimal point, and exponent
     * ("5", ".5", "5.", "007.5", "1e3"), INF, +INF, -INF, or NaN. Other text,
     * including leading or trailing spaces, is an error. Only text is read
     * this way: an integer value is never silently a double.
     *
     * @param $value
     *
     * @return double
     */
    public function to_double(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_double");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_hexBinary()
     *
     * Converts `<dataType>` value to hexBinary. A string is read with the
     * hexBinary parser, the same parser the typed literal "..."hexBinary
     * uses, and text the parser rejects is an error.
     *
     * @param $value
     *
     * @return hexBinary
     */
    public function to_hexBinary(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_hexBinary");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_ia5String()
     *
     * Converts `<dataType>` value to ia5String. A string is read with the
     * ia5String parser, the same parser the typed literal "..."ia5String
     * uses, and text the parser rejects is an error.
     *
     * @param $value
     *
     * @return ia5String
     */
    public function to_ia5String(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_ia5String");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_integer()
     *
     * Converts `<dataType>` value to integer. A double is truncated toward
     * zero (1.9 is 1 and -1.9 is -1); use round(), floor(), or ceil() first
     * for another choice. A double that is NaN, infinite, or outside the
     * integer range is an error. A string is read as XML Schema integer text:
     * digits with an optional sign, leading zeros allowed ("007" is 7). Text
     * that is not an integer ("1.5", " 1", "1x") is an error.
     *
     * @param $value
     *
     * @return integer
     */
    public function to_integer(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_integer");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_ipAddress()
     *
     * Converts `<dataType>` value to ipAddress. A string is read with the
     * ipAddress parser, the same parser the typed literal "..."ipAddress
     * uses, and text the parser rejects is an error.
     *
     * @param $value
     *
     * @return ipAddress
     */
    public function to_ipAddress(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_ipAddress");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_lower_case()
     *
     * Return the `<dataType>` value in lower case: Unicode full case mapping,
     * the same in every locale ("İ" becomes "i̇"). The result is a string.
     * normalize_to_lower_case maps one code point at a time.
     *
     * @param $value The `<dataType>` value.
     *
     * @return string The value in lower case, as a string.
     */
    public function to_lower_case(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_lower_case");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_objectId()
     *
     * Converts `<dataType>` value to objectId. A string is read with the
     * objectId parser, and text the parser rejects is an error.
     *
     * @param $value
     *
     * @return objectId
     */
    public function to_objectId(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_objectId");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_objectPath()
     *
     * Converts `<dataType>` value to objectPath. A string is read with the
     * objectPath parser, and text the parser rejects is an error.
     *
     * @param $value
     *
     * @return objectPath
     */
    public function to_objectPath(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_objectPath");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_password()
     *
     * Converts `<dataType>` value to password. A string is read with the
     * password parser, the same parser the typed literal "..."password uses,
     * and text the parser rejects is an error.
     *
     * @param $value
     *
     * @return password
     */
    public function to_password(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_password");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_regexp()
     *
     * Converts `<dataType>` value to a compiled regexp, the same value as a
     * regexp literal. A pattern ICU does not accept is a conversion_error.
     *
     * @param $value
     *
     * @return regexp
     */
    public function to_regexp(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_regexp");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_rfc822Name()
     *
     * Converts `<dataType>` value to rfc822Name. A string is read with the
     * rfc822Name parser, the same parser the typed literal "..."rfc822Name
     * uses, and text the parser rejects is an error.
     *
     * @param $value
     *
     * @return rfc822Name
     */
    public function to_rfc822Name(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_rfc822Name");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string()
     *
     * Converts `<dataType>` value to string. For array values, the
     * to_string() value for each entry is returned separated with commas.
     *
     * @param $value A `<dataType>` value.
     *
     * @return string The string representation of the value.
     */
    public function to_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_time()
     *
     * Converts `<dataType>` value to time. A string is read with the time
     * parser, the same parser the typed literal "..."time uses, and text the
     * parser rejects is an error.
     *
     * @param $value
     *
     * @return time
     */
    public function to_time(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_time");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_upper_case()
     *
     * Return the `<dataType>` value in upper case: Unicode full case mapping,
     * the same in every locale, so one code point can become more than one
     * ("ß" becomes "SS"). The result is a string.
     *
     * @param $value The `<dataType>` value.
     *
     * @return string The value in upper case, as a string.
     */
    public function to_upper_case(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_upper_case");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_x500Name()
     *
     * Converts `<dataType>` value to x500Name. A string is read with the
     * x500Name parser, the same parser the typed literal "..."x500Name uses,
     * and text the parser rejects is an error.
     *
     * @param $value
     *
     * @return x500Name
     */
    public function to_x500Name(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_x500Name");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_yearMonthDuration()
     *
     * Converts `<dataType>` value to yearMonthDuration. A string is read with
     * the yearMonthDuration parser, the same parser the typed literal
     * "..."yearMonthDuration uses, and text the parser rejects is an error.
     *
     * @param $value
     *
     * @return yearMonthDuration
     */
    public function to_yearMonthDuration(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_yearMonthDuration");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * trim()
     *
     * Remove white space and line terminators (tab, vertical tab, form feed,
     * space, no-break space, every Unicode space separator, LF, CR, U+2028
     * and U+2029) from the beginning and end of the `<dataType>` value. White
     * space inside the value is kept; normalize_space also collapses it. The
     * result is a string.
     *
     * @param $value The `<dataType>` value.
     *
     * @return string The value without leading and trailing white space, as a
     *                string.
     */
    public function trim(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "trim");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * trim_end()
     *
     * Remove white space and line terminators (tab, vertical tab, form feed,
     * space, no-break space, every Unicode space separator, LF, CR, U+2028
     * and U+2029) from the end of the `<dataType>` value. The result is a
     * string.
     *
     * @param $value The `<dataType>` value.
     *
     * @return string The value without trailing white space, as a string.
     */
    public function trim_end(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "trim_end");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * trim_start()
     *
     * Remove white space and line terminators (tab, vertical tab, form feed,
     * space, no-break space, every Unicode space separator, LF, CR, U+2028
     * and U+2029) from the beginning of the `<dataType>` value. The result is
     * a string.
     *
     * @param $value The `<dataType>` value.
     *
     * @return string The value without leading white space, as a string.
     */
    public function trim_start(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "trim_start");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * trunc()
     *
     * Return the integer part of the `<dataType>` value, removing any
     * fraction (toward zero: trunc(-1.5) is -1.0). The result is a double:
     * write to_integer(trunc(v)) for an integer. A NaN argument gives NaN. An
     * integer is not accepted: write to_double(v).
     *
     * @param $value
     *
     * @return
     */
    public function trunc(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "trunc");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * union()
     *
     * Returns an array of `<dataType>` contains all of the unique values in
     * two or more array of `<dataType>` values.
     *
     * @param array $arrays Two or more arrays.
     *
     * @return array
     */
    public function union(, $arrays)
    {
        $request = $this->$session->request();

        $request->set("function", "union");

        /* pass along required parameters to the request payload */
        $request->set("arrays", $arrays);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * url_encode()
     *
     * URL encode a `<dataType>` value and return the string result: every
     * character is percent-encoded as UTF-8 except A-Z a-z 0-9 ; , / ? : @ &
     * = + $ - _ . ! ~ * ' ( ) #. Characters that give a URI its structure are
     * kept, so this encodes a whole URI, not a single query value or path
     * segment.
     *
     * @param $unencoded The `<dataType>` value to URL encode.
     *
     * @return string URI encoded string.
     */
    public function url_encode(, $unencoded)
    {
        $request = $this->$session->request();

        $request->set("function", "url_encode");

        /* pass along required parameters to the request payload */
        $request->set("unencoded", $unencoded);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
