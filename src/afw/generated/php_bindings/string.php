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
     * Add (concatenate) 2 or more string values and return the string result.
     * Every value must be a string, the same as the + operator: to convert
     * values while concatenating, use concat() or string().
     *
     * @param string $values
     *
     * @return string
     */
    public function add_string(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "add<string>");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * array_string()
     *
     * Construct a new array of string from the given values, the same as
     * array() except that the new array is typed: each value, and each entry
     * of a spread array, must be dataType string, and adding a value of
     * another data type to the array later throws. A value of another data
     * type is an error, never converted. This is usually written
     * `array<string>(`...values).
     *
     * @param $values A value of dataType string, or ... followed by an
     *                 expression that results in an array, every entry of
     *                 which is included.
     *
     * @return array The constructed array of string.
     */
    public function array_string(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "array_string");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * as_string()
     *
     * Returns value unchanged if it is dataType string. Otherwise, including
     * when value is undefined, throws an error. It never converts: to get a
     * string from a value of another data type, write a typed literal or call
     * a to_T conversion function where one exists. This is usually written
     * value as string.
     *
     * @param $value Value to assert
     *
     * @return string value, which is dataType string
     */
    public function as_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "as_string");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

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
     * code_point_at_string()
     *
     * Return the code point at a zero-based index of the string value as an
     * integer, as ECMAScript s.codePointAt(i) does. Indexes count code
     * points, as everywhere in Adaptive Script (not UTF-16 units). Negative
     * indexes count from the end, as at does. If the index is out of range,
     * the result is undefined, as for at.
     *
     * @param string $value The string value.
     * @param integer $index Zero-based code point index, or negative from the
     *                       end.
     *
     * @return integer The code point, or undefined if index is out of range.
     */
    public function code_point_at_string(, $value, $index)
    {
        $request = $this->$session->request();

        $request->set("function", "code_point_at<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("index", $index);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * concat()
     *
     * concat follows its first value. If it is an array, return a new array
     * of the entries of every value in order, as ECMAScript array concat
     * does, except that every value must be an array (write [...a, v] or
     * push(a, v) to add a single value). Otherwise convert every value of any
     * data type to string and return the concatenated result; a value with an
     * undefined value is represented by 'undefined'.
     *
     * @param $values Arrays to concatenate, or values to convert to strings
     *                 and concatenate.
     *
     * @return A new array if the first value is an array, otherwise the
     *          concatenated string.
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
     * @param $arg2
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
     * @param $arg2
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
     * eval_string()
     *
     * Compile and evaluate string value as adaptive script.
     *
     * @param string $source string to compile and evaluate
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
    public function eval_string(, $source, $additionalUntrustedQualifiedVariables = null)
    {
        $request = $this->$session->request();

        $request->set("function", "eval<string>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($additionalUntrustedQualifiedVariables != null)
            $request->set('additionalUntrustedQualifiedVariables', $additionalUntrustedQualifiedVariables);

        return $request->get_result();
    }

    /**
     * from_code_point()
     *
     * Return a string of the code points passed, in order, as ECMAScript
     * String.fromCodePoint(...) does. A code point must be from 0 to 0x10FFFF
     * and not a surrogate (U+D800 to U+DFFF): a string can not hold a lone
     * surrogate. The result is NFC, like every string.
     *
     * @param integer $codePoints The code points.
     *
     * @return string The string of those code points.
     */
    public function from_code_point(, $codePoints)
    {
        $request = $this->$session->request();

        $request->set("function", "from_code_point");

        /* pass along required parameters to the request payload */
        $request->set("codePoints", $codePoints);

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
     * boolean result. Empty array yields false.
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
     * Checks whether value is dataType string and returns the boolean result.
     * It is false for undefined. This can also be written
     * `is<string>(value`).
     *
     * @param $value Value to check
     *
     * @return boolean
     */
    public function is_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is_string");

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
     * @param $arg2
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
     * Return the integer number of entries in a string array or the number of
     * codepoints in a string anyURI or string.
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
     * match_all_string()
     *
     * Searches string value for the regular expression and returns each match
     * with the text of its groups, left to right, as ECMAScript's
     * s.matchAll(re) does. Matches do not overlap; after an empty match the
     * next search starts one code point later. length(match_all(s, re))
     * counts the matches.
     *
     * @param string $value The string value to search.
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
    public function match_all_string(, $value, $regexp, $limit = null)
    {
        $request = $this->$session->request();

        $request->set("function", "match_all<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);

        /* pass along any optional parameters to the request payload */
        if ($limit != null)
            $request->set('limit', $limit);

        return $request->get_result();
    }

    /**
     * match_string()
     *
     * Searches string value for the regular expression and returns its first
     * match with the text of its groups, or undefined if there is none, as
     * ECMAScript's s.match(re) does without the g flag. Use test for a
     * boolean result.
     *
     * @param string $value The string value to search.
     * @param regexp $regexp The regular expression. A regexp literal (/a+b/
     *                       or "a+b"re), to_regexp(s), or
     *                       `compile<regexp>(r`) is already compiled; any
     *                       other regexp value is compiled for this call.
     *
     * @return object The first match, or undefined if there is none.
     */
    public function match_string(, $value, $regexp)
    {
        $request = $this->$session->request();

        $request->set("function", "match<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);

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
     * @param $arg2
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
     * @param $arg2
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
     * normalize_string()
     *
     * Return the string value in a Unicode normalization form, as ECMAScript
     * s.normalize(form) does. Every Adaptive string is already NFC, so form
     * "NFC" (the default) returns it as a string; "NFKC" also replaces
     * compatibility characters ("ﬁ" becomes "fi", "①" becomes "1"). "NFD" and
     * "NFKD" are errors: a string in Adaptive Script is always NFC, so a
     * decomposed result can not be a string.
     *
     * @param string $value The string value.
     * @param string $form "NFC" (the default) or "NFKC".
     *
     * @return string The normalized value, as a string.
     */
    public function normalize_string(, $value, $form = null)
    {
        $request = $this->$session->request();

        $request->set("function", "normalize<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        if ($form != null)
            $request->set('form', $form);

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
     * pad_end_string()
     *
     * Pad the string value at the end with fillString until it is maxLength
     * code points long, as ECMAScript s.padEnd() does. Lengths are code
     * points, not UTF-16 units. Unlike ECMAScript, an empty fillString when
     * padding is needed is an error, not a quiet return of a shorter value.
     *
     * @param string $value The string value.
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
    public function pad_end_string(, $value, $maxLength, $fillString = null)
    {
        $request = $this->$session->request();

        $request->set("function", "pad_end<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("maxLength", $maxLength);

        /* pass along any optional parameters to the request payload */
        if ($fillString != null)
            $request->set('fillString', $fillString);

        return $request->get_result();
    }

    /**
     * pad_start_string()
     *
     * Pad the string value at the start with fillString until it is maxLength
     * code points long, as ECMAScript s.padStart() does. Lengths are code
     * points, not UTF-16 units. Unlike ECMAScript, an empty fillString when
     * padding is needed is an error, not a quiet return of a shorter value.
     *
     * @param string $value The string value.
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
    public function pad_start_string(, $value, $maxLength, $fillString = null)
    {
        $request = $this->$session->request();

        $request->set("function", "pad_start<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("maxLength", $maxLength);

        /* pass along any optional parameters to the request payload */
        if ($fillString != null)
            $request->set('fillString', $fillString);

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
     * @return string Repeated text as string (not re-typed as the input data
     *                type).
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
     * replace_all_string()
     *
     * Replace every match in a string value, left to right, as ECMAScript
     * s.replaceAll(search, r) does; the same as replace(value, search,
     * replacement, -1). search is a string, which is literal text, or a
     * regexp, which is a pattern (no g flag is needed). The replacement is a
     * string, or a function called with each match's _AdaptiveRegexpMatch_
     * object that returns the string to put in.
     *
     * @param string $value The string value.
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
    public function replace_all_string(, $value, $search, $replacement)
    {
        $request = $this->$session->request();

        $request->set("function", "replace_all<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("search", $search);
        $request->set("replacement", $replacement);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * replace_string()
     *
     * Replace matches in a string value, left to right, up to limit times, as
     * ECMAScript's s.replace(search, r) does. search is a string, which is
     * literal text, or a regexp, which is a pattern. The replacement is a
     * string, or a function called with each match's _AdaptiveRegexpMatch_
     * object that returns the string to put in. An empty match is a match at
     * a code-point boundary. The default limit replaces once (an empty string
     * inserts at the start). Specify limit -1 to replace every match.
     *
     * @param string $value The original string value.
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
    public function replace_string(, $value, $search, $replacement, $limit = null)
    {
        $request = $this->$session->request();

        $request->set("function", "replace<string>");

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
     * search_string()
     *
     * Searches string value for the regular expression and returns the code
     * point index of the first match, or -1 if there is none, as ECMAScript's
     * s.search(re) does. Use index_of to find literal text.
     *
     * @param string $value The string value to search.
     * @param regexp $regexp The regular expression. A regexp literal (/a+b/
     *                       or "a+b"re), to_regexp(s), or
     *                       `compile<regexp>(r`) is already compiled; any
     *                       other regexp value is compiled for this call.
     *
     * @return integer Zero-based code point index of the first match, or -1.
     */
    public function search_string(, $value, $regexp)
    {
        $request = $this->$session->request();

        $request->set("function", "search<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);

        /* pass along any optional parameters to the request payload */
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
     * Split string value into an array of strings, as ECMAScript's
     * s.split(separator) does. separator is a string, which is literal text,
     * or a regexp, which is a pattern. With a regexp, the text of each group
     * of a match is put in the array after the piece before it (undefined
     * when the group did not take part), and an empty match splits between
     * code points but not at the start or end of the value.
     *
     * @param string $value The string value to split.
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
     * Conversion function for string: convert one or more values of any data
     * type to string and return the concatenated result. With one argument
     * this is convert-to-string; with more arguments each is converted then
     * concatenated in order (no separator). A value with an undefined value
     * is represented by 'undefined'.
     *
     * @param $values Value to convert
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
     * Returns the string substring of value from zero-based code point
     * position integer startIndex up to, but not including, endIndex, as
     * ECMAScript substring does. A negative index is 0 and an index past the
     * end is the end of value. Omit endIndex to return through the end of
     * value. If endIndex is less than startIndex, the two are swapped, and
     * equal indexes return an empty string. Use slice to count from the end.
     * The result is always string (a slice of anyURI is not an anyURI).
     *
     * @param string $string
     * @param integer $startIndex
     * @param integer $endIndex
     *
     * @return string Substring as string (not re-typed as the input data
     *                type).
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
     * Converts string value to anyURI. A string is read with the anyURI
     * parser, the same parser the typed literal "..."anyURI uses, and text
     * the parser rejects is an error.
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
     * to_base64Binary_string()
     *
     * Converts string value to base64Binary. A string is read with the
     * base64Binary parser, the same parser the typed literal
     * "..."base64Binary uses, and text the parser rejects is an error.
     *
     * @param string $value
     *
     * @return base64Binary
     */
    public function to_base64Binary_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_base64Binary<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_dateTime_string()
     *
     * Converts string value to dateTime. A string is read with the dateTime
     * parser, the same parser the typed literal "..."dateTime uses, and text
     * the parser rejects is an error.
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
     * Converts string value to date. A string is read with the date parser,
     * the same parser the typed literal "..."date uses, and text the parser
     * rejects is an error.
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
     * Converts string value to dayTimeDuration. A string is read with the
     * dayTimeDuration parser, the same parser the typed literal
     * "..."dayTimeDuration uses, and text the parser rejects is an error.
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
     * Converts string value to dnsName. A string is read with the dnsName
     * parser, the same parser the typed literal "..."dnsName uses, and text
     * the parser rejects is an error.
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
     * Converts string value to double. A string is read as XML Schema double
     * text: digits with an optional sign, decimal point, and exponent ("5",
     * ".5", "5.", "007.5", "1e3"), INF, +INF, -INF, or NaN. Other text,
     * including leading or trailing spaces, is an error. Only text is read
     * this way: an integer value is never silently a double.
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
     * to_hexBinary_string()
     *
     * Converts string value to hexBinary. A string is read with the hexBinary
     * parser, the same parser the typed literal "..."hexBinary uses, and text
     * the parser rejects is an error.
     *
     * @param string $value
     *
     * @return hexBinary
     */
    public function to_hexBinary_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_hexBinary<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_ia5String_string()
     *
     * Converts string value to ia5String. A string is read with the ia5String
     * parser, the same parser the typed literal "..."ia5String uses, and text
     * the parser rejects is an error.
     *
     * @param string $value
     *
     * @return ia5String
     */
    public function to_ia5String_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_ia5String<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_integer_string()
     *
     * Converts string value to integer. A double is truncated toward zero
     * (1.9 is 1 and -1.9 is -1); use round(), floor(), or ceil() first for
     * another choice. A double that is NaN, infinite, or outside the integer
     * range is an error. A string is read as XML Schema integer text: digits
     * with an optional sign, leading zeros allowed ("007" is 7). Text that is
     * not an integer ("1.5", " 1", "1x") is an error.
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
     * Converts string value to ipAddress. A string is read with the ipAddress
     * parser, the same parser the typed literal "..."ipAddress uses, and text
     * the parser rejects is an error.
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
     * to_lower_case_string()
     *
     * Return the string value in lower case, as ECMAScript s.toLowerCase()
     * does: Unicode full case mapping, the same in every locale ("İ" becomes
     * "i̇"). The result is a string. normalize_to_lower_case maps one code
     * point at a time.
     *
     * @param string $value The string value.
     *
     * @return string The value in lower case, as a string.
     */
    public function to_lower_case_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_lower_case<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_objectId_string()
     *
     * Converts string value to objectId. A string is read with the objectId
     * parser, and text the parser rejects is an error.
     *
     * @param string $value
     *
     * @return objectId
     */
    public function to_objectId_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_objectId<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_objectPath_string()
     *
     * Converts string value to objectPath. A string is read with the
     * objectPath parser, and text the parser rejects is an error.
     *
     * @param string $value
     *
     * @return objectPath
     */
    public function to_objectPath_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_objectPath<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_password_string()
     *
     * Converts string value to password. A string is read with the password
     * parser, the same parser the typed literal "..."password uses, and text
     * the parser rejects is an error.
     *
     * @param string $value
     *
     * @return password
     */
    public function to_password_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_password<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_regexp_string()
     *
     * Converts string value to a compiled regexp, the same value as a regexp
     * literal. A pattern ICU does not accept is a conversion_error.
     *
     * @param string $value
     *
     * @return regexp
     */
    public function to_regexp_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_regexp<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_rfc822Name_string()
     *
     * Converts string value to rfc822Name. A string is read with the
     * rfc822Name parser, the same parser the typed literal "..."rfc822Name
     * uses, and text the parser rejects is an error.
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
     * Converts string value to time. A string is read with the time parser,
     * the same parser the typed literal "..."time uses, and text the parser
     * rejects is an error.
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
     * to_upper_case_string()
     *
     * Return the string value in upper case, as ECMAScript s.toUpperCase()
     * does: Unicode full case mapping, the same in every locale, so one code
     * point can become more than one ("ß" becomes "SS"). The result is a
     * string.
     *
     * @param string $value The string value.
     *
     * @return string The value in upper case, as a string.
     */
    public function to_upper_case_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_upper_case<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_x500Name_string()
     *
     * Converts string value to x500Name. A string is read with the x500Name
     * parser, the same parser the typed literal "..."x500Name uses, and text
     * the parser rejects is an error.
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
     * Converts string value to yearMonthDuration. A string is read with the
     * yearMonthDuration parser, the same parser the typed literal
     * "..."yearMonthDuration uses, and text the parser rejects is an error.
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
     * trim_end_string()
     *
     * Remove white space and line terminators as ECMAScript counts them (tab,
     * vertical tab, form feed, space, no-break space, U+FEFF, every Unicode
     * space separator, LF, CR, U+2028 and U+2029) from the end of the string
     * value, as ECMAScript s.trimEnd() does. The result is a string.
     *
     * @param string $value The string value.
     *
     * @return string The value without trailing white space, as a string.
     */
    public function trim_end_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "trim_end<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * trim_start_string()
     *
     * Remove white space and line terminators as ECMAScript counts them (tab,
     * vertical tab, form feed, space, no-break space, U+FEFF, every Unicode
     * space separator, LF, CR, U+2028 and U+2029) from the beginning of the
     * string value, as ECMAScript s.trimStart() does. The result is a string.
     *
     * @param string $value The string value.
     *
     * @return string The value without leading white space, as a string.
     */
    public function trim_start_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "trim_start<string>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * trim_string()
     *
     * Remove white space and line terminators as ECMAScript counts them (tab,
     * vertical tab, form feed, space, no-break space, U+FEFF, every Unicode
     * space separator, LF, CR, U+2028 and U+2029) from the beginning and end
     * of the string value, as ECMAScript s.trim() does. White space inside
     * the value is kept; normalize_space also collapses it. The result is a
     * string.
     *
     * @param string $value The string value.
     *
     * @return string The value without leading and trailing white space, as a
     *                string.
     */
    public function trim_string(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "trim<string>");

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
     * URL decode a string.
     *
     * @param string $encoded String to URL decode.
     *
     * @return string The URL-decoded string.
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
     * URL encode a string value and return the string result, as ECMAScript
     * encodeURI does: every character is percent-encoded as UTF-8 except A-Z
     * a-z 0-9 ; , / ? : @ & = + $ - _ . ! ~ * ' ( ) #. Characters that give a
     * URI its structure are kept, so this encodes a whole URI, not a single
     * query value or path segment.
     *
     * @param string $unencoded The string value to URL encode.
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
