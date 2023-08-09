<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for polymorphic
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category polymorphic
 */
class polymorphic
{
    private $session;

    /**
     * Constructs the polymorphic class, using a session.
     *
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
     * @param  $value
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
     * add()
     *
     * Add 2 or more `<dataType>` values and return the `<dataType>` result.
     *
     * @param  $values
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
     * @param  $arg1
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
     * @param  $arg1
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
     * at_least_one_member_of()
     *
     * Returns boolean true if at least one value in `<dataType>` list1 is in
     * `<dataType>` list2.
     *
     * @param array $list1 The first array.
     * @param array $list2 The second array.
     *
     * @return boolean
     */
    public function at_least_one_member_of(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag()
     *
     * Takes any number of `<dataType>` values and returns an array of array.
     *
     * @param array $values
     *
     * @return array
     */
    public function bag(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size()
     *
     * This returns the integer number of values in array.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size");

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
     * @param  $value
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
     * @param  $value The `<dataType>` value to clone.
     *
     * @return  The cloned `<dataType>` value.
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
     * compile()
     *
     * Compile `<dataType>` value and return either an unevaluated adaptive
     * value or a string containing the compiler listing.
     *
     * @param  $source `<dataType>` string to compile
     * @param  $listing If specified, a compiler listing is produced instead
     *                  of an unevaluated expression value.
     *                  
     *                  This parameter can be an integer between 0 and 10 of
     *                  a string that is used for indentation. If 0 is
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
     * decode_to_string()
     *
     * Decode `<dataType>` value to string. An error is thrown if decoded
     * value is not valid UTF-8.
     *
     * @param  $value The `<dataType>` value to decode.
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
     * @param  $dividend
     * @param  $divisor
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
     * @param  $value The `<dataType>` value to encode.
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
     * @param  $value The `<dataType>` value to encode.
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
     * @param  $value
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
     * @param  $arg1
     * @param  $arg2
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
     * @param  $arg1
     * @param  $arg2
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
     * @param  $arg1
     * @param  $arg2
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
     * evaluate()
     *
     * Compile and evaluate `<dataType>` value.
     *
     * @param  $source `<dataType>` string to compile and evaluate
     * @param object $additionalUntrustedQualifiedVariables This parameter
     *                                                      supplies
     *                                                      additional
     *                                                      qualified
     *                                                      variables that
     *                                                      can be accessed
     *                                                      during
     *                                                      evaluation. These
     *                                                      variables will
     *                                                      not be used by
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
     * @return unevaluated
     */
    public function evaluate(, $source, $additionalUntrustedQualifiedVariables = null)
    {
        $request = $this->$session->request();

        $request->set("function", "evaluate");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($additionalUntrustedQualifiedVariables != null)
            $request->set('additionalUntrustedQualifiedVariables', $additionalUntrustedQualifiedVariables);

        return $request->get_result();
    }

    /**
     * floor()
     *
     * Determine the largest integer that is smaller then or equal to the
     * `<dataType>` value and return the `<dataType>` result.
     *
     * @param  $number
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
     * ge()
     *
     * Checks for `<dataType>` arg1 is greater than or equal to `<dataType>`
     * arg2 and return the boolean result.
     *
     * @param  $arg1
     * @param  $arg2
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
     * @param  $arg1
     * @param  $arg2
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
     * in_range()
     *
     * Checks if `<dataType>` is between startTime and endTime, inclusive.
     * Regardless of endTime value, it is always considered to be equal to,
     * but less than 24 hours greater than startTime. If no `<dataType>` zone
     * is specified for `<dataType>,` the default `<dataType>` zone is used.
     * If no `<dataType>` zone is specified for startTime or endTime, the
     * `<dataType>` zone of `<dataType>` is used.
     *
     * @param  $time
     * @param  $startTime
     * @param  $endTime
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
     * @param  $searchString The `<dataType>` to search.
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
     * @param  $value The `<dataType>` value to search.
     * @param string $subString Substring to search for.
     * @param integer $startIndex Optional start index for search if
     *                            different than start of string.
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
     * both array of `<dataType>` list1 and list2.
     *
     * @param array $list1 The first array.
     * @param array $list2 The second array.
     *
     * @return array
     */
    public function intersection(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is()
     *
     * Checks whether value is dataType `<dataType>` and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in()
     *
     * Checks whether `<dataType>` value is in array of `<dataType>` array
     * and returns the boolean result.
     *
     * @param  $value
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
     * @param  $value The `<dataType>` value to search.
     * @param string $subString Substring to search for.
     * @param integer $startIndex Optional start index for search if
     *                            different than start of string.
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
     * Checks for `<dataType>` arg1 is less than or equal to `<dataType>`
     * arg2 and return the boolean result.
     *
     * @param  $arg1
     * @param  $arg2
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
     * This is a polymorphic function where `<dataType>` can be any of the
     * supported data types. Return the integer number of entries in datatype
     * array or codepoints in others.
     *
     * @param  $value Returns the number of entries in an array or code
     *                points in others.
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
     * lt()
     *
     * Checks for `<dataType>` arg1 is less that `<dataType>` arg2 and return
     * the boolean result.
     *
     * @param  $arg1
     * @param  $arg2
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
     * Returns true if arg1 matches some terminal sequence of RDNs from arg2
     * when compared using `equal_<dataType>`.
     *
     * @param  $arg1
     * @param string $arg2
     *
     * @return boolean
     */
    public function match(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "match");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * max()
     *
     * Return the `<dataType>` value that is greater than or equal to the
     * others.
     *
     * @param  $values
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
     * Return the `<dataType>` value that is less than or equal to the
     * others.
     *
     * @param  $values
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
     * @param  $dividend
     * @param  $divisor
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
     * @param  $values
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
     * @param  $arg1
     * @param  $arg2
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
     * @param  $value
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
     * type of arg2 then return the boolean result. Use 'ne' ('!=') instead
     * if you want arg2 to be converted to the data type of arg1 before
     * comparison.
     *
     * @param  $arg1
     * @param  $arg2
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
     * normalize_space()
     *
     * Remove whitespace from the beginning and end of a `<dataType>` value.
     *
     * @param  $string
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
     * @param  $string
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
     * pow()
     *
     * This returns the value of base raised to a power. Multiple exponents
     * can be specified to raise the previous exponent to the power of the
     * latter exponent.
     *
     * @param  $base Base value.
     * @param  $exponent Exponent value.
     *
     * @return  Base raised to the power.
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
     * regexp_index_of()
     *
     * Search `<dataType>` value for a regular expression and return index.
     * If not found, -1 is returned.
     *
     * @param  $value The `<dataType>` value to search.
     * @param string $regexp A regular expression to use for search.
     *
     * @return integer Zero-based index of subString or -1 if not found.
     */
    public function regexp_index_of(, $value, $regexp)
    {
        $request = $this->$session->request();

        $request->set("function", "regexp_index_of");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * regexp_match()
     *
     * Checks whether `<dataType>` value matches the regular expression
     * regexp and return the boolean result.
     *
     * @param  $value
     * @param string $regexp
     *
     * @return boolean
     */
    public function regexp_match(, $value, $regexp)
    {
        $request = $this->$session->request();

        $request->set("function", "regexp_match");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * regexp_replace()
     *
     * Replace matched values for a regular expression in a `<dataType>`
     * value.
     *
     * @param  $value The original `<dataType>` value.
     * @param string $regexp A regular expression to use for search.
     * @param string $replacement The replacement string.
     * @param integer $limit This is the maximum times to replace. The
     *                       default is 1. Specify -1 to replace all
     *                       occurrences.
     *
     * @return  A `<dataType>` value with the matched string(s) replaced.
     */
    public function regexp_replace(, $value, $regexp, $replacement, $limit = null)
    {
        $request = $this->$session->request();

        $request->set("function", "regexp_replace");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);
        $request->set("replacement", $replacement);

        /* pass along any optional parameters to the request payload */
        if ($limit != null)
            $request->set('limit', $limit);

        return $request->get_result();
    }

    /**
     * repeat()
     *
     * Repeat a `<dataType>` value a specified number of times.
     *
     * @param  $value The `<dataType>` value to repeat.
     * @param integer $times The number of times to repeat the value.
     *
     * @return  The repeated `<dataType>` value.
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
     * Replace string(s) in a `<dataType>` value.
     *
     * @param  $value The original `<dataType>` value.
     * @param string $match The string to replace.
     * @param string $replacement The replacement string.
     * @param integer $limit This is the maximum times to replace. The
     *                       default is 1. Specify -1 to replace all
     *                       occurrences.
     *
     * @return  A `<dataType>` value with the matched string(s) replaced.
     */
    public function replace(, $value, $match, $replacement, $limit = null)
    {
        $request = $this->$session->request();

        $request->set("function", "replace");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("match", $match);
        $request->set("replacement", $replacement);

        /* pass along any optional parameters to the request payload */
        if ($limit != null)
            $request->set('limit', $limit);

        return $request->get_result();
    }

    /**
     * round()
     *
     * Determine the integer closest to `<dataType>` value and return the
     * `<dataType>` result.
     *
     * @param  $number
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
     * set_equals()
     *
     * Returns boolean true if `<dataType>` list1 and `<dataType>` list2 are
     * subsets of each other and return the boolean result.
     *
     * @param array $list1
     * @param array $list2
     *
     * @return boolean
     */
    public function set_equals(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * split()
     *
     * Split `<dataType>` value into an array of strings using a separator.
     *
     * @param  $value The `<dataType>` value to split.
     * @param string $separator The separator to use. If this is an empty
     *                          string or separator is not specified, the
     *                          value is split between characters.
     * @param integer $limit This is the maximum number of splits. Any
     *                       remaining part of value is ignored.
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
     * starts_with()
     *
     * Checks whether `<dataType>` value starts with a subString and return
     * the boolean result.
     *
     * @param  $value
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
     * Returns boolean true if the unique values in `<dataType>` list1 are
     * all in `<dataType>` list2.
     *
     * @param array $list1 The first array.
     * @param array $list2 The second array.
     *
     * @return boolean
     */
    public function subset(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * substring()
     *
     * Returns the `<dataType>` substring of value beginning at zero-based
     * position integer startIndex and ending at the position before integer
     * endIndex. Specify -1 or omitting endIndex to return up to end of
     * `<dataType>`.
     *
     * @param  $string
     * @param integer $startIndex
     * @param integer $endIndex
     *
     * @return
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
     * @param  $arg1
     * @param  $arg2
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
     * @param  $arg1
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
     * @param  $arg1
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
     * to_anyURI()
     *
     * Converts `<dataType>` value to anyURI.
     *
     * @param  $value
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
     * to_boolean()
     *
     * Converts `<dataType>` value to boolean.
     *
     * @param  $value
     *
     * @return boolean
     */
    public function to_boolean(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_boolean");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_date()
     *
     * Converts `<dataType>` value to date.
     *
     * @param  $value
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
     * Converts `<dataType>` value to dateTime.
     *
     * @param  $value
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
     * Converts `<dataType>` value to dayTimeDuration.
     *
     * @param  $value
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
     * Converts `<dataType>` value to dnsName.
     *
     * @param  $value
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
     * Converts `<dataType>` value to double and returns double result.
     *
     * @param  $value
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
     * to_integer()
     *
     * Truncate `<dataType>` value to a whole number and returns integer
     * result.
     *
     * @param  $value
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
     * Converts `<dataType>` value to ipAddress.
     *
     * @param  $value
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
     * to_rfc822Name()
     *
     * Converts `<dataType>` value to rfc822Name.
     *
     * @param  $value
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
     * @param  $value A `<dataType>` value.
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
     * Converts `<dataType>` value to time.
     *
     * @param  $value
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
     * to_x500Name()
     *
     * Converts `<dataType>` value to x500Name.
     *
     * @param  $value
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
     * Converts `<dataType>` value to yearMonthDuration.
     *
     * @param  $value
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
     * union()
     *
     * Returns an array of `<dataType>` contains all of the unique values in
     * two or more array of `<dataType>` values.
     *
     * @param array $lists Two or more lists.
     *
     * @return array
     */
    public function union(, $lists)
    {
        $request = $this->$session->request();

        $request->set("function", "union");

        /* pass along required parameters to the request payload */
        $request->set("lists", $lists);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * url_encode()
     *
     * URL encode a value or bag of values.
     *
     * @param  $unencoded URL encode a single value. See the url_encode
     *                    method for the data type of more details.
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
