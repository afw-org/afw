<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for anyURI
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file anyURI.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category anyURI
 */
class anyURI
{
    private $session;

    /**
     * Constructs the anyURI class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * anyURI()
     *
     * Converts value to data type anyURI returning anyURI result.
     *
     * @param  $value Value to convert
     *
     * @return anyURI Converted value
     */
    public function anyURI(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "anyURI");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * at_least_one_member_of_anyURI()
     *
     * Returns boolean true if at least one value in anyURI array1 is in
     * anyURI array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function at_least_one_member_of_anyURI(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_anyURI()
     *
     * Takes any number of anyURI values and returns an array of array.
     *
     * @param array $values
     *
     * @return array
     */
    public function bag_anyURI(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<anyURI>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_anyURI()
     *
     * This returns the integer number of values in array.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size_anyURI(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ends_with_anyURI()
     *
     * Checks whether anyURI value ends with a anyURI and return the boolean
     * result.
     *
     * @param anyURI $value
     * @param string $subString
     *
     * @return boolean
     */
    public function ends_with_anyURI(, $value, $subString)
    {
        $request = $this->$session->request();

        $request->set("function", "ends_with<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("subString", $subString);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_anyURI()
     *
     * Determine if anyURI arg1 is equal to the value of arg2 converted to the
     * data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param anyURI $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_anyURI(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_anyURI()
     *
     * Determine if for anyURI arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param anyURI $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_anyURI(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_anyURI()
     *
     * Checks for anyURI arg1 is greater than or equal to anyURI arg2 and
     * return the boolean result.
     *
     * @param anyURI $arg1
     * @param anyURI $arg2
     *
     * @return boolean
     */
    public function ge_anyURI(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_anyURI()
     *
     * Checks for anyURI arg1 is greater than anyURI arg2 and return the
     * boolean result.
     *
     * @param anyURI $arg1
     * @param anyURI $arg2
     *
     * @return boolean
     */
    public function gt_anyURI(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * includes_anyURI()
     *
     * Checks whether the anyURI value includes a string and return the
     * boolean result.
     *
     * @param anyURI $searchString The anyURI to search.
     * @param string $subString Substring to find.
     * @param integer $position Zero-based position in the search string to
     *                          start search.
     *
     * @return boolean Indicates if the substring is contained in the search
     *                 string.
     */
    public function includes_anyURI(, $searchString, $subString, $position = null)
    {
        $request = $this->$session->request();

        $request->set("function", "includes<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("searchString", $searchString);
        $request->set("subString", $subString);

        /* pass along any optional parameters to the request payload */
        if ($position != null)
            $request->set('position', $position);

        return $request->get_result();
    }

    /**
     * index_of_anyURI()
     *
     * Returns the zero-based index into anyURI value of subString. If
     * subString is not found, -1 is returned.
     *
     * @param anyURI $value The anyURI value to search.
     * @param string $subString Substring to search for.
     * @param integer $startIndex Optional start index for search if different
     *                            than start of string.
     *
     * @return integer Zero-based index of subString or -1 if not found.
     */
    public function index_of_anyURI(, $value, $subString, $startIndex = null)
    {
        $request = $this->$session->request();

        $request->set("function", "index_of<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("subString", $subString);

        /* pass along any optional parameters to the request payload */
        if ($startIndex != null)
            $request->set('startIndex', $startIndex);

        return $request->get_result();
    }

    /**
     * intersection_anyURI()
     *
     * Returns an array of anyURI with the values that are common to both
     * array of anyURI array1 and array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return array
     */
    public function intersection_anyURI(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_anyURI()
     *
     * Checks whether value is dataType anyURI and return the boolean result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_anyURI(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_anyURI()
     *
     * Checks whether anyURI value is in array of anyURI array and returns the
     * boolean result.
     *
     * @param anyURI $value
     * @param array $array
     *
     * @return boolean
     */
    public function is_in_anyURI(, $value, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * last_index_of_anyURI()
     *
     * Returns the zero-based index into anyURI value of the last occurrence
     * of a subString. If subString is not found, -1 is returned.
     *
     * @param anyURI $value The anyURI value to search.
     * @param string $subString Substring to search for.
     * @param integer $startIndex Optional start index for search if different
     *                            than start of string.
     *
     * @return integer Zero-based index of subString or -1 if not found.
     */
    public function last_index_of_anyURI(, $value, $subString, $startIndex = null)
    {
        $request = $this->$session->request();

        $request->set("function", "last_index_of<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("subString", $subString);

        /* pass along any optional parameters to the request payload */
        if ($startIndex != null)
            $request->set('startIndex', $startIndex);

        return $request->get_result();
    }

    /**
     * le_anyURI()
     *
     * Checks for anyURI arg1 is less than or equal to anyURI arg2 and return
     * the boolean result.
     *
     * @param anyURI $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_anyURI(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * length_anyURI()
     *
     * This is a polymorphic function where anyURI can be any of the supported
     * data types. Return the integer number of entries in datatype array or
     * codepoints in others.
     *
     * @param anyURI $value Returns the number of entries in an array or code
     *                      points in others.
     *
     * @return integer
     */
    public function length_anyURI(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "length<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_anyURI()
     *
     * Checks for anyURI arg1 is less that anyURI arg2 and return the boolean
     * result.
     *
     * @param anyURI $arg1
     * @param anyURI $arg2
     *
     * @return boolean
     */
    public function lt_anyURI(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_anyURI()
     *
     * Determine if anyURI arg1 is not equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
     * instead if you want true to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param anyURI $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_anyURI(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_anyURI()
     *
     * Determine if for anyURI arg1 is not equal to the value or data type of
     * arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param anyURI $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_anyURI(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only_anyURI()
     *
     * This converts an array of anyURI values that contains one value to a
     * single anyURI value.
     *
     * @param array $array
     *
     * @return anyURI
     */
    public function one_and_only_anyURI(, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * regexp_index_of_anyURI()
     *
     * Search anyURI value for a regular expression and return index. If not
     * found, -1 is returned.
     *
     * @param anyURI $value The anyURI value to search.
     * @param string $regexp A regular expression to use for search.
     *
     * @return integer Zero-based index of subString or -1 if not found.
     */
    public function regexp_index_of_anyURI(, $value, $regexp)
    {
        $request = $this->$session->request();

        $request->set("function", "regexp_index_of<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * regexp_match_anyURI()
     *
     * Checks whether anyURI value matches the regular expression regexp and
     * return the boolean result.
     *
     * @param anyURI $value
     * @param string $regexp
     *
     * @return boolean
     */
    public function regexp_match_anyURI(, $value, $regexp)
    {
        $request = $this->$session->request();

        $request->set("function", "regexp_match<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * regexp_replace_anyURI()
     *
     * Replace matched values for a regular expression in a anyURI value.
     *
     * @param anyURI $value The original anyURI value.
     * @param string $regexp A regular expression to use for search.
     * @param string $replacement The replacement string.
     * @param integer $limit This is the maximum times to replace. The default
     *                       is 1. Specify -1 to replace all occurrences.
     *
     * @return anyURI A anyURI value with the matched string(s) replaced.
     */
    public function regexp_replace_anyURI(, $value, $regexp, $replacement, $limit = null)
    {
        $request = $this->$session->request();

        $request->set("function", "regexp_replace<anyURI>");

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
     * repeat_anyURI()
     *
     * Repeat a anyURI value a specified number of times.
     *
     * @param anyURI $value The anyURI value to repeat.
     * @param integer $times The number of times to repeat the value.
     *
     * @return anyURI The repeated anyURI value.
     */
    public function repeat_anyURI(, $value, $times)
    {
        $request = $this->$session->request();

        $request->set("function", "repeat<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("times", $times);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * replace_anyURI()
     *
     * Replace string(s) in a anyURI value.
     *
     * @param anyURI $value The original anyURI value.
     * @param string $match The string to replace.
     * @param string $replacement The replacement string.
     * @param integer $limit This is the maximum times to replace. The default
     *                       is 1. Specify -1 to replace all occurrences.
     *
     * @return anyURI A anyURI value with the matched string(s) replaced.
     */
    public function replace_anyURI(, $value, $match, $replacement, $limit = null)
    {
        $request = $this->$session->request();

        $request->set("function", "replace<anyURI>");

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
     * set_equals_anyURI()
     *
     * Returns boolean true if anyURI array1 and anyURI array2 are subsets of
     * each other and return the boolean result.
     *
     * @param array $array1
     * @param array $array2
     *
     * @return boolean
     */
    public function set_equals_anyURI(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * split_anyURI()
     *
     * Split anyURI value into an array of strings using a separator.
     *
     * @param anyURI $value The anyURI value to split.
     * @param string $separator The separator to use. If this is an empty
     *                          string or separator is not specified, the
     *                          value is split between characters.
     * @param integer $limit This is the maximum number of splits. Any
     *                       remaining part of value is ignored.
     *
     * @return array An array of strings.
     */
    public function split_anyURI(, $value, $separator = null, $limit = null)
    {
        $request = $this->$session->request();

        $request->set("function", "split<anyURI>");

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
     * starts_with_anyURI()
     *
     * Checks whether anyURI value starts with a subString and return the
     * boolean result.
     *
     * @param anyURI $value
     * @param string $subString
     *
     * @return boolean
     */
    public function starts_with_anyURI(, $value, $subString)
    {
        $request = $this->$session->request();

        $request->set("function", "starts_with<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("subString", $subString);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_anyURI()
     *
     * Returns boolean true if the unique values in anyURI array1 are all in
     * anyURI array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function subset_anyURI(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * substring_anyURI()
     *
     * Returns the anyURI substring of value beginning at zero-based position
     * integer startIndex and ending at the position before integer endIndex.
     * Specify -1 or omitting endIndex to return up to end of anyURI.
     *
     * @param anyURI $string
     * @param integer $startIndex
     * @param integer $endIndex
     *
     * @return anyURI
     */
    public function substring_anyURI(, $string, $startIndex, $endIndex = null)
    {
        $request = $this->$session->request();

        $request->set("function", "substring<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("string", $string);
        $request->set("startIndex", $startIndex);

        /* pass along any optional parameters to the request payload */
        if ($endIndex != null)
            $request->set('endIndex', $endIndex);

        return $request->get_result();
    }

    /**
     * to_string_anyURI()
     *
     * Converts anyURI value to string. For array values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param anyURI $value A anyURI value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_anyURI(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * union_anyURI()
     *
     * Returns an array of anyURI contains all of the unique values in two or
     * more array of anyURI values.
     *
     * @param array $arrays Two or more arrays.
     *
     * @return array
     */
    public function union_anyURI(, $arrays)
    {
        $request = $this->$session->request();

        $request->set("function", "union<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("arrays", $arrays);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * url_encode_anyURI()
     *
     * URL encode a value or bag of values.
     *
     * @param anyURI $unencoded URL encode a single value. See the url_encode
     *                          method for the data type of more details.
     *
     * @return string URI encoded string.
     */
    public function url_encode_anyURI(, $unencoded)
    {
        $request = $this->$session->request();

        $request->set("function", "url_encode<anyURI>");

        /* pass along required parameters to the request payload */
        $request->set("unencoded", $unencoded);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
