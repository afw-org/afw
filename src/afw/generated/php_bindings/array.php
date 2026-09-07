<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for array
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file array.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category array
 */
class array
{
    private $session;

    /**
     * Constructs the array class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * add_entries()
     *
     * Append every entry of each source array onto the end of target, in
     * order. target must be mutable. Entries are copied by value reference;
     * nested objects and arrays are not deeply cloned.
     *
     * @param array $target Target array. This array must not be immutable.
     * @param array $source Source array(s).
     *
     * @return array The modified target array.
     */
    public function add_entries(, $target, $source)
    {
        $request = $this->$session->request();

        $request->set("function", "add_entries");

        /* pass along required parameters to the request payload */
        $request->set("target", $target);
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * array()
     *
     * Construct a new array from the given values (not a conversion
     * function). Each argument becomes one element, in order. If a value is
     * written as ...expression and the expression is an array, each of its
     * entries is included in order. An empty call produces an empty array. A
     * non-spread array argument is nested as a single element (array([1,2])
     * is [[1,2]]); use spread or a list literal to flatten. For a length of
     * undefined slots use create_array(n).
     *
     * @param $values A value can refer to any adaptable value belonging to
     *                 any data type or an array expression. In the case of an
     *                 array expression, indicated by '...' followed by an
     *                 expression that results in an array, every element
     *                 within that array is included in the newly created
     *                 array.
     *
     * @return array The constructed array.
     */
    public function array(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "array");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * at()
     *
     * Return the value at a zero-based index in an array. Negative indexes
     * count from the end (-1 is the last element). If the index is out of
     * range, the result is undefined. Bracket indexing a[index] uses the same
     * out-of-range result (undefined).
     *
     * @param array $array Array to index.
     * @param integer $index Zero-based index, or negative from the end.
     *
     * @return The value at the index, or undefined if out of range.
     */
    public function at(, $array, $index)
    {
        $request = $this->$session->request();

        $request->set("function", "at");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("index", $index);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_array()
     *
     * Takes any number of array values and returns an array of array.
     *
     * @param array $values
     *
     * @return array
     */
    public function bag_array(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<array>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_array()
     *
     * This returns the integer number of values in array.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size_array(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<array>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * clone_array()
     *
     * Deep clone a array value.
     *
     * @param array $value The array value to clone.
     *
     * @return array The cloned array value.
     */
    public function clone_array(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "clone<array>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * create_array()
     *
     * Create a new mutable array of the given length where every entry is
     * undefined. Useful when you want a known length up front before
     * assigning or filling entries. Length must be a non-negative integer and
     * must not exceed 1,000,000. This is a length-based constructor, not a
     * conversion function (see also array(...), which builds from elements).
     *
     * @param integer $length Number of undefined elements (0 or more, up to
     *                        the maximum).
     *
     * @return array A new array of the requested length; each entry is
     *               undefined.
     */
    public function create_array(, $length)
    {
        $request = $this->$session->request();

        $request->set("function", "create_array");

        /* pass along required parameters to the request payload */
        $request->set("length", $length);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_array()
     *
     * Determine if array arg1 is equal to the value of arg2 converted to the
     * data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param array $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function eq_array(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<array>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_array()
     *
     * Determine if for array arg1 is equal to the value and data type of arg2
     * then return the boolean result. Use 'eq' ('==') instead if you want
     * arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param array $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function eqx_array(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<array>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * freeze_array()
     *
     * Set a array value immutable so further mutation throws. If already
     * immutable, has no effect. Returns the same value.
     *
     * @param array $value The array value to freeze.
     *
     * @return array The same value, now immutable.
     */
    public function freeze_array(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "freeze<array>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_array()
     *
     * Checks for array arg1 is greater than or equal to array arg2 and return
     * the boolean result.
     *
     * @param array $arg1
     * @param array $arg2
     *
     * @return boolean
     */
    public function ge_array(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<array>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_array()
     *
     * Checks for array arg1 is greater than array arg2 and return the boolean
     * result.
     *
     * @param array $arg1
     * @param array $arg2
     *
     * @return boolean
     */
    public function gt_array(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<array>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * includes_array()
     *
     * Checks whether or not an array contains any value.
     *
     * @param array $array The array to search.
     * @param $searchElement Element to find.
     * @param integer $fromIndex Index in the array to start search.
     *
     * @return boolean Indicates if the element is found in array.
     */
    public function includes_array(, $array, $searchElement, $fromIndex = null)
    {
        $request = $this->$session->request();

        $request->set("function", "includes<array>");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("searchElement", $searchElement);

        /* pass along any optional parameters to the request payload */
        if ($fromIndex != null)
            $request->set('fromIndex', $fromIndex);

        return $request->get_result();
    }

    /**
     * is_array()
     *
     * Checks whether value is dataType array and return the boolean result.
     *
     * @param $value Value to check
     *
     * @return boolean
     */
    public function is_array(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<array>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * join()
     *
     * Concatenate the string values of the elements of an array with a
     * separator.
     *
     * @param array $value An array of values (statements) of any data type.
     * @param string $separator The separator to use. If not specified, a
     *                          comma (,) is used.
     *
     * @return string Joined array values.
     */
    public function join(, $value, $separator = null)
    {
        $request = $this->$session->request();

        $request->set("function", "join");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        if ($separator != null)
            $request->set('separator', $separator);

        return $request->get_result();
    }

    /**
     * le_array()
     *
     * Checks for array arg1 is less than or equal to array arg2 and return
     * the boolean result.
     *
     * @param array $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function le_array(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<array>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * length_array()
     *
     * Return the integer number of entries in a array array or the number of
     * codepoints in a array anyURI or string.
     *
     * @param array $value Returns the number of entries in an array or code
     *                     points in others.
     *
     * @return integer
     */
    public function length_array(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "length<array>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_array()
     *
     * Checks for array arg1 is less that array arg2 and return the boolean
     * result.
     *
     * @param array $arg1
     * @param array $arg2
     *
     * @return boolean
     */
    public function lt_array(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<array>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_array()
     *
     * Determine if array arg1 is not equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
     * instead if you want true to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param array $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function ne_array(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<array>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_array()
     *
     * Determine if for array arg1 is not equal to the value or data type of
     * arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param array $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function nex_array(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<array>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * pop()
     *
     * Remove the last value from a mutable array and return it. If the array
     * is empty, returns undefined.
     *
     * @param array $array Target array. Must not be immutable.
     *
     * @return The removed value, or undefined if the array was empty.
     */
    public function pop(, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "pop");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * push()
     *
     * Append one or more values to the end of a mutable array. Returns the
     * same array after modification. The array must not be immutable (for
     * example after freeze).
     *
     * @param array $array Target array. Must not be immutable.
     * @param $values Values to append in order.
     *
     * @return array The modified array.
     */
    public function push(, $array, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "push");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * reverse()
     *
     * Reverse the order of the elements in an array. If the array is typed,
     * the resulting array will be the same type.
     *
     * @param array $array An array to reverse.
     *
     * @return array An array with elements reversed.
     */
    public function reverse(, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "reverse");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * shift()
     *
     * Remove the first value from a mutable array and return it. If the array
     * is empty, returns undefined.
     *
     * @param array $array Target array. Must not be immutable.
     *
     * @return The removed value, or undefined if the array was empty.
     */
    public function shift(, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "shift");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * slice()
     *
     * This function extracts a consecutive slice of values from an array.
     *
     * @param array $array The array to slice.
     * @param integer $startIndex This is the zero based starting index. Use
     *                            negative number to index from the end of the
     *                            array. If not specified, an index of 0 is
     *                            assumed.
     * @param integer $endIndex This is the zero based ending index. If
     *                          positive, this is one more than the index of
     *                          the last value to include in the array. If
     *                          negative, the index is from the end of the
     *                          array. If not specified, the slice is from
     *                          startIndex up to and including the end of the
     *                          array.
     *
     * @return array An array containing the selected values. If all of the
     *               values are the same data type, the array will be an array
     *               of that data type.
     */
    public function slice(, $array, $startIndex = null, $endIndex = null)
    {
        $request = $this->$session->request();

        $request->set("function", "slice");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        if ($startIndex != null)
            $request->set('startIndex', $startIndex);

        if ($endIndex != null)
            $request->set('endIndex', $endIndex);

        return $request->get_result();
    }

    /**
     * splice()
     *
     * Remove zero or more values starting at an index from a mutable array
     * and optionally insert new values at that index. Returns an array of the
     * removed values. Negative startIndex counts from the end. If deleteCount
     * is omitted, all values from startIndex to the end are removed.
     *
     * @param array $array Target array. Must not be immutable.
     * @param integer $startIndex Zero-based start index, or negative from the
     *                            end.
     * @param integer $deleteCount Number of values to remove. If omitted,
     *                             remove through the end of the array.
     *                             Negative is treated as zero.
     * @param $values Values to insert at startIndex after removals.
     *
     * @return array Array of removed values, in original order.
     */
    public function splice(, $array, $startIndex, $deleteCount = null, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "splice");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("startIndex", $startIndex);

        /* pass along any optional parameters to the request payload */
        if ($deleteCount != null)
            $request->set('deleteCount', $deleteCount);

        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * to_string_array()
     *
     * Converts array value to string. For array values, the to_string() value
     * for each entry is returned separated with commas.
     *
     * @param array $value A array value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_array(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<array>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * unshift()
     *
     * Insert one or more values at the beginning of a mutable array,
     * preserving the relative order of the inserted values. Returns the
     * modified array.
     *
     * @param array $array Target array. Must not be immutable.
     * @param $values Values to insert at the front, in order.
     *
     * @return array The modified array.
     */
    public function unshift(, $array, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "unshift");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

}

?>
