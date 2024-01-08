<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for string
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file string.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category string
 */
class string
{
    private $session;

    /**
     * Constructs the string class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * add_string()
     *
     * Add (concatenate) a string with 1 or more values of any data type
     * converted to their string value and return the string result.
     *
     * @param string $string
     * @param  $values
     *
     * @return string
     */
    public function add_string(, $string, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "add<string>");

        /* pass along required parameters to the request payload */
        $request->set("string", $string);
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * at_least_one_member_of_string()
     *
     * Returns boolean true if at least one value in string array1 is in
     * string array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function at_least_one_member_of_string(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<string>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_size_string()
     *
     * This returns the integer number of values in array.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_string()
     *
     * Takes any number of string values and returns an array of array.
     *
     * @param array $values
     *
     * @return array
     */
    public function bag_string(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<string>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * concat()
     *
     * Convert two or more values of any data type to string and return the
     * concatenated result. A value with an undefined value is represented by
     * 'undefined'.
     *
     * @param  $values Value to convert
     *
     * @return string The concatenated string values.
     */
    public function concat(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "concat");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * encode_as_base64Binary_string()
     *
     * Encode a value as a base64Binary. The effect is to create a
     * base64Binary value with an internal value of the value passed.
     *
     * @param string $value The string value to encode.
     *
     * @return base64Binary A base64Binary value.
     */
    public function encode_as_base64Binary_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "encode_as_base64Binary<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * encode_as_hexBinary_string()
     *
     * Encode a value as a hexBinary. The effect is to create a hexBinary
     * value with an internal value of the value passed.
     *
     * @param string $value The string value to encode.
     *
     * @return hexBinary A hexBinary value.
     */
    public function encode_as_hexBinary_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "encode_as_hexBinary<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ends_with_string()
     *
     * Checks whether string value ends with a string and return the boolean
     * result.
     *
     * @param string $value
     * @param string $subString
     *
     * @return boolean
     */
    public function ends_with_string(, $value, $subString)
    {
        $request = $this->$session->request();

        $request->set("function", "ends_with<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("subString", $subString);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_ignore_case_string()
     *
     * Checks for string arg1 is equal to string arg2 ignoring case and return
     * the boolean result.
     *
     * @param string $arg1
     * @param string $arg2
     *
     * @return boolean
     */
    public function eq_ignore_case_string(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq_ignore_case<string>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_string()
     *
     * Determine if string arg1 is equal to the value of arg2 converted to the
     * data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param string $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_string(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<string>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_string()
     *
     * Determine if for string arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param string $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_string(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<string>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_string()
     *
     * Checks for string arg1 is greater than or equal to string arg2 and
     * return the boolean result.
     *
     * @param string $arg1
     * @param string $arg2
     *
     * @return boolean
     */
    public function ge_string(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<string>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_string()
     *
     * Checks for string arg1 is greater than string arg2 and return the
     * boolean result.
     *
     * @param string $arg1
     * @param string $arg2
     *
     * @return boolean
     */
    public function gt_string(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<string>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * includes_string()
     *
     * Checks whether the string value includes a string and return the
     * boolean result.
     *
     * @param string $searchString The string to search.
     * @param string $subString Substring to find.
     * @param integer $position Zero-based position in the search string to
     *                          start search.
     *
     * @return boolean Indicates if the substring is contained in the search
     *                 string.
     */
    public function includes_string(, $searchString, $subString, $position = null)
    {
        $request = $this->$session->request();

        $request->set("function", "includes<string>");

        /* pass along required parameters to the request payload */
        $request->set("searchString", $searchString);
        $request->set("subString", $subString);

        /* pass along any optional parameters to the request payload */
        if ($position != null)
            $request->set('position', $position);

        return $request->get_result();
    }

    /**
     * index_of_string()
     *
     * Returns the zero-based index into string value of subString. If
     * subString is not found, -1 is returned.
     *
     * @param string $value The string value to search.
     * @param string $subString Substring to search for.
     * @param integer $startIndex Optional start index for search if different
     *                            than start of string.
     *
     * @return integer Zero-based index of subString or -1 if not found.
     */
    public function index_of_string(, $value, $subString, $startIndex = null)
    {
        $request = $this->$session->request();

        $request->set("function", "index_of<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("subString", $subString);

        /* pass along any optional parameters to the request payload */
        if ($startIndex != null)
            $request->set('startIndex', $startIndex);

        return $request->get_result();
    }

    /**
     * intersection_string()
     *
     * Returns an array of string with the values that are common to both
     * array of string array1 and array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return array
     */
    public function intersection_string(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<string>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_string()
     *
     * Checks whether string value is in array of string array and returns the
     * boolean result.
     *
     * @param string $value
     * @param array $array
     *
     * @return boolean
     */
    public function is_in_string(, $value, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_string()
     *
     * Checks whether value is dataType string and return the boolean result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * last_index_of_string()
     *
     * Returns the zero-based index into string value of the last occurrence
     * of a subString. If subString is not found, -1 is returned.
     *
     * @param string $value The string value to search.
     * @param string $subString Substring to search for.
     * @param integer $startIndex Optional start index for search if different
     *                            than start of string.
     *
     * @return integer Zero-based index of subString or -1 if not found.
     */
    public function last_index_of_string(, $value, $subString, $startIndex = null)
    {
        $request = $this->$session->request();

        $request->set("function", "last_index_of<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("subString", $subString);

        /* pass along any optional parameters to the request payload */
        if ($startIndex != null)
            $request->set('startIndex', $startIndex);

        return $request->get_result();
    }

    /**
     * le_string()
     *
     * Checks for string arg1 is less than or equal to string arg2 and return
     * the boolean result.
     *
     * @param string $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_string(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<string>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * length_string()
     *
     * This is a polymorphic function where string can be any of the supported
     * data types. Return the integer number of entries in datatype array or
     * codepoints in others.
     *
     * @param string $value Returns the number of entries in an array or code
     *                      points in others.
     *
     * @return integer
     */
    public function length_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "length<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_string()
     *
     * Checks for string arg1 is less that string arg2 and return the boolean
     * result.
     *
     * @param string $arg1
     * @param string $arg2
     *
     * @return boolean
     */
    public function lt_string(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<string>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * max_string()
     *
     * Return the string value that is greater than or equal to the others.
     *
     * @param string $values
     *
     * @return string
     */
    public function max_string(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "max<string>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * min_string()
     *
     * Return the string value that is less than or equal to the others.
     *
     * @param string $values
     *
     * @return string
     */
    public function min_string(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "min<string>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_string()
     *
     * Determine if string arg1 is not equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
     * instead if you want true to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param string $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_string(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<string>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_string()
     *
     * Determine if for string arg1 is not equal to the value or data type of
     * arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param string $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_string(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<string>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * normalize_space_string()
     *
     * Remove whitespace from the beginning and end of a string value.
     *
     * @param string $string
     *
     * @return string
     */
    public function normalize_space_string(, $string)
    {
        $request = $this->$session->request();

        $request->set("function", "normalize_space<string>");

        /* pass along required parameters to the request payload */
        $request->set("string", $string);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * normalize_to_lower_case_string()
     *
     * Normalize string value to lower case and returns string result.
     *
     * @param string $string
     *
     * @return string
     */
    public function normalize_to_lower_case_string(, $string)
    {
        $request = $this->$session->request();

        $request->set("function", "normalize_to_lower_case<string>");

        /* pass along required parameters to the request payload */
        $request->set("string", $string);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only_string()
     *
     * This converts an array of string values that contains one value to a
     * single string value.
     *
     * @param array $array
     *
     * @return string
     */
    public function one_and_only_string(, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<string>");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * regexp_index_of_string()
     *
     * Search string value for a regular expression and return index. If not
     * found, -1 is returned.
     *
     * @param string $value The string value to search.
     * @param string $regexp A regular expression to use for search.
     *
     * @return integer Zero-based index of subString or -1 if not found.
     */
    public function regexp_index_of_string(, $value, $regexp)
    {
        $request = $this->$session->request();

        $request->set("function", "regexp_index_of<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * regexp_match_string()
     *
     * Checks whether string value matches the regular expression regexp and
     * return the boolean result.
     *
     * @param string $value
     * @param string $regexp
     *
     * @return boolean
     */
    public function regexp_match_string(, $value, $regexp)
    {
        $request = $this->$session->request();

        $request->set("function", "regexp_match<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * regexp_replace_string()
     *
     * Replace matched values for a regular expression in a string value.
     *
     * @param string $value The original string value.
     * @param string $regexp A regular expression to use for search.
     * @param string $replacement The replacement string.
     * @param integer $limit This is the maximum times to replace. The default
     *                       is 1. Specify -1 to replace all occurrences.
     *
     * @return string A string value with the matched string(s) replaced.
     */
    public function regexp_replace_string(, $value, $regexp, $replacement, $limit = null)
    {
        $request = $this->$session->request();

        $request->set("function", "regexp_replace<string>");

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
     * repeat_string()
     *
     * Repeat a string value a specified number of times.
     *
     * @param string $value The string value to repeat.
     * @param integer $times The number of times to repeat the value.
     *
     * @return string The repeated string value.
     */
    public function repeat_string(, $value, $times)
    {
        $request = $this->$session->request();

        $request->set("function", "repeat<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("times", $times);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * replace_string()
     *
     * Replace string(s) in a string value.
     *
     * @param string $value The original string value.
     * @param string $match The string to replace.
     * @param string $replacement The replacement string.
     * @param integer $limit This is the maximum times to replace. The default
     *                       is 1. Specify -1 to replace all occurrences.
     *
     * @return string A string value with the matched string(s) replaced.
     */
    public function replace_string(, $value, $match, $replacement, $limit = null)
    {
        $request = $this->$session->request();

        $request->set("function", "replace<string>");

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
     * set_equals_string()
     *
     * Returns boolean true if string array1 and string array2 are subsets of
     * each other and return the boolean result.
     *
     * @param array $array1
     * @param array $array2
     *
     * @return boolean
     */
    public function set_equals_string(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<string>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * split_string()
     *
     * Split string value into an array of strings using a separator.
     *
     * @param string $value The string value to split.
     * @param string $separator The separator to use. If this is an empty
     *                          string or separator is not specified, the
     *                          value is split between characters.
     * @param integer $limit This is the maximum number of splits. Any
     *                       remaining part of value is ignored.
     *
     * @return array An array of strings.
     */
    public function split_string(, $value, $separator = null, $limit = null)
    {
        $request = $this->$session->request();

        $request->set("function", "split<string>");

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
     * starts_with_string()
     *
     * Checks whether string value starts with a subString and return the
     * boolean result.
     *
     * @param string $value
     * @param string $subString
     *
     * @return boolean
     */
    public function starts_with_string(, $value, $subString)
    {
        $request = $this->$session->request();

        $request->set("function", "starts_with<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("subString", $subString);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * string()
     *
     * Convert one or more values of any data type to string and return the
     * concatenated result. A value with an undefined value is represented by
     * 'undefined'.
     *
     * @param  $values Value to convert
     *
     * @return string The concatenated string values.
     */
    public function string(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "string");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_string()
     *
     * Returns boolean true if the unique values in string array1 are all in
     * string array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function subset_string(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<string>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * substring_string()
     *
     * Returns the string substring of value beginning at zero-based position
     * integer startIndex and ending at the position before integer endIndex.
     * Specify -1 or omitting endIndex to return up to end of string.
     *
     * @param string $string
     * @param integer $startIndex
     * @param integer $endIndex
     *
     * @return string
     */
    public function substring_string(, $string, $startIndex, $endIndex = null)
    {
        $request = $this->$session->request();

        $request->set("function", "substring<string>");

        /* pass along required parameters to the request payload */
        $request->set("string", $string);
        $request->set("startIndex", $startIndex);

        /* pass along any optional parameters to the request payload */
        if ($endIndex != null)
            $request->set('endIndex', $endIndex);

        return $request->get_result();
    }

    /**
     * to_anyURI_string()
     *
     * Converts string value to anyURI.
     *
     * @param string $value
     *
     * @return anyURI
     */
    public function to_anyURI_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_anyURI<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_boolean_string()
     *
     * Converts string value to boolean.
     *
     * @param string $value
     *
     * @return boolean
     */
    public function to_boolean_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_boolean<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_dateTime_string()
     *
     * Converts string value to dateTime.
     *
     * @param string $value
     *
     * @return dateTime
     */
    public function to_dateTime_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_dateTime<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_date_string()
     *
     * Converts string value to date.
     *
     * @param string $value
     *
     * @return date
     */
    public function to_date_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_date<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_dayTimeDuration_string()
     *
     * Converts string value to dayTimeDuration.
     *
     * @param string $value
     *
     * @return dayTimeDuration
     */
    public function to_dayTimeDuration_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_dayTimeDuration<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_dnsName_string()
     *
     * Converts string value to dnsName.
     *
     * @param string $value
     *
     * @return dnsName
     */
    public function to_dnsName_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_dnsName<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_double_string()
     *
     * Converts string value to double and returns double result.
     *
     * @param string $value
     *
     * @return double
     */
    public function to_double_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_double<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_integer_string()
     *
     * Truncate string value to a whole number and returns integer result.
     *
     * @param string $value
     *
     * @return integer
     */
    public function to_integer_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_integer<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_ipAddress_string()
     *
     * Converts string value to ipAddress.
     *
     * @param string $value
     *
     * @return ipAddress
     */
    public function to_ipAddress_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_ipAddress<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_rfc822Name_string()
     *
     * Converts string value to rfc822Name.
     *
     * @param string $value
     *
     * @return rfc822Name
     */
    public function to_rfc822Name_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_rfc822Name<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_string()
     *
     * Converts string value to string. For array values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param string $value A string value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_time_string()
     *
     * Converts string value to time.
     *
     * @param string $value
     *
     * @return time
     */
    public function to_time_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_time<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_x500Name_string()
     *
     * Converts string value to x500Name.
     *
     * @param string $value
     *
     * @return x500Name
     */
    public function to_x500Name_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_x500Name<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_yearMonthDuration_string()
     *
     * Converts string value to yearMonthDuration.
     *
     * @param string $value
     *
     * @return yearMonthDuration
     */
    public function to_yearMonthDuration_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_yearMonthDuration<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * union_string()
     *
     * Returns an array of string contains all of the unique values in two or
     * more array of string values.
     *
     * @param array $arrays Two or more arrays.
     *
     * @return array
     */
    public function union_string(, $arrays)
    {
        $request = $this->$session->request();

        $request->set("function", "union<string>");

        /* pass along required parameters to the request payload */
        $request->set("arrays", $arrays);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * url_decode()
     *
     * URL decode a value or bag of values.
     *
     * @param string $encoded URL decode a single string or a bag of string.
     *
     * @return string A string or bag of strings.
     */
    public function url_decode(, $encoded)
    {
        $request = $this->$session->request();

        $request->set("function", "url_decode");

        /* pass along required parameters to the request payload */
        $request->set("encoded", $encoded);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * url_encode_string()
     *
     * URL encode a value or bag of values.
     *
     * @param string $unencoded URL encode a single value. See the url_encode
     *                          method for the data type of more details.
     *
     * @return string URI encoded string.
     */
    public function url_encode_string(, $unencoded)
    {
        $request = $this->$session->request();

        $request->set("function", "url_encode<string>");

        /* pass along required parameters to the request payload */
        $request->set("unencoded", $unencoded);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
