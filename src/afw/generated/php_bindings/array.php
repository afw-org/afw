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
     * array_array()
     *
     * Construct a new array of array from the given values, the same as
     * array() except that the new array is typed: each value, and each entry
     * of a spread array, must be dataType array, and adding a value of
     * another data type to the array later throws. A value of another data
     * type is an error, never converted. This is usually written
     * `array<array>(`...values).
     *
     * @param $values A value of dataType array, or ... followed by an
     *                 expression that results in an array, every entry of
     *                 which is included.
     *
     * @return array The constructed array of array.
     */
    public function array_array(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "array_array");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * as_array()
     *
     * Returns value unchanged if it is dataType array. Otherwise, including
     * when value is undefined, throws an error. It never converts: to get a
     * array from a value of another data type, write a typed literal or call
     * a to_T conversion function where one exists. This is usually written
     * value as array.
     *
     * @param $value Value to assert
     *
     * @return array value, which is dataType array
     */
    public function as_array(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "as_array");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * at()
     *
     * Return the value at a zero-based index in an array, or the code point
     * at a zero-based index in a string as a one-code-point string. Negative
     * indexes count from the end (-1 is the last). If the index is out of
     * range, the result is undefined. Bracket indexing a[index] and s[index]
     * use the same out-of-range result (undefined).
     *
     * @param $array Array, or string, to index.
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
     * value, or undefined when value is not passed. Useful when you want a
     * known length up front before assigning entries; assigning one past the
     * end appends, further is an error. A computed array is
     * create_array(n)->map(function(v, i) { ... }). Length must be a
     * non-negative integer and must not exceed 1,000,000. This is a
     * length-based constructor, not a conversion function (see also
     * array(...), which builds from elements).
     *
     * @param integer $length Number of entries (0 or more, up to the
     *                        maximum).
     * @param $value The value of every entry. A function is stored as the
     *                entry, not called; write create_array(n)->map(fn) for
     *                computed entries. If not passed, every entry is
     *                undefined.
     *
     * @return array A new array of the requested length; each entry is value,
     *               or undefined.
     */
    public function create_array(, $length, $value = null)
    {
        $request = $this->$session->request();

        $request->set("function", "create_array");

        /* pass along required parameters to the request payload */
        $request->set("length", $length);

        /* pass along any optional parameters to the request payload */
        if ($value != null)
            $request->set('value', $value);

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
     * fill()
     *
     * Set the entries of array from start up to (not including) end to value,
     * in place, and return the same array. Negative indexes count from the
     * end and indexes out of range are clamped, as for slice. The length of
     * the array does not change. A frozen array is an error. A function
     * passed as value is stored, not called.
     *
     * @param array $array The array to fill. Must not be frozen.
     * @param $value The value to store in each entry.
     * @param integer $start The first index to fill. The default is 0.
     *                       Negative counts from the end.
     * @param integer $end One more than the last index to fill. The default
     *                     is the length. Negative counts from the end.
     *
     * @return array The array, filled.
     */
    public function fill(, $array, $value, $start = null, $end = null)
    {
        $request = $this->$session->request();

        $request->set("function", "fill");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        if ($start != null)
            $request->set('start', $start);

        if ($end != null)
            $request->set('end', $end);

        return $request->get_result();
    }

    /**
     * flat()
     *
     * Return a new array with the entries of array, where an entry that is an
     * array is replaced by its entries, down to depth levels. The default
     * depth is 1; a negative depth flattens every level; 0 copies the array.
     * Undefined entries are kept (Adaptive arrays have no holes).
     *
     * @param array $array The array to flatten.
     * @param integer $depth How many levels of nested arrays to flatten. The
     *                       default is 1; a negative depth flattens every
     *                       level.
     *
     * @return array A new, flattened array.
     */
    public function flat(, $array, $depth = null)
    {
        $request = $this->$session->request();

        $request->set("function", "flat");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        if ($depth != null)
            $request->set('depth', $depth);

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
     * Checks whether an array contains searchElement: the same as
     * index_of(array, searchElement, fromIndex) >= 0. Values are compared by
     * value (equal arrays and objects match) and NaN is found.
     *
     * @param array $array The array to search.
     * @param $searchElement Element to find.
     * @param integer $fromIndex Index to start the search at. Negative counts
     *                           from the end. At or past the end, nothing is
     *                           found. Default 0.
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
     * index_of_array()
     *
     * Return the index of the first entry of array equal to searchElement,
     * searching forward from fromIndex, or -1. Values are compared as
     * includes compares them: by value (equal arrays and objects match), and
     * NaN is found, so includes(a, v) is always index_of(a, v) >= 0.
     *
     * @param array $array The array to search.
     * @param $searchElement The value to find.
     * @param integer $fromIndex Index to start the search at. Negative counts
     *                           from the end. At or past the end, nothing is
     *                           found. Default 0.
     *
     * @return integer The index found, or -1.
     */
    public function index_of_array(, $array, $searchElement, $fromIndex = null)
    {
        $request = $this->$session->request();

        $request->set("function", "index_of<array>");

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
     * Checks whether value is dataType array and returns the boolean result.
     * It is false for undefined. This can also be written `is<array>(value`).
     *
     * @param $value Value to check
     *
     * @return boolean
     */
    public function is_array(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is_array");

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
     * last_index_of_array()
     *
     * Return the index of the last entry of array equal to searchElement,
     * searching backward from fromIndex, or -1. Values are compared as
     * includes compares them: by value (equal arrays and objects match), and
     * NaN is found.
     *
     * @param array $array The array to search.
     * @param $searchElement The value to find.
     * @param integer $fromIndex Index to start the search at, searching
     *                           backward. Negative counts from the end.
     *                           Default: the last index.
     *
     * @return integer The index found, or -1.
     */
    public function last_index_of_array(, $array, $searchElement, $fromIndex = null)
    {
        $request = $this->$session->request();

        $request->set("function", "last_index_of<array>");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("searchElement", $searchElement);

        /* pass along any optional parameters to the request payload */
        if ($fromIndex != null)
            $request->set('fromIndex', $fromIndex);

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
     * new length of the array. The array must not be immutable (for example
     * after freeze).
     *
     * @param array $array Target array. Must not be immutable.
     * @param $values Values to append in order.
     *
     * @return integer The new length of the array.
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
     * Reverse the order of the entries of array in place and return the same
     * array. A frozen array is an error. Use to_reversed for a reversed copy.
     *
     * @param array $array The array to reverse. Must not be frozen.
     *
     * @return array The array, reversed.
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
     * This function extracts a consecutive slice of values from an array, or
     * of code points from a string. For a string, or another value stored as
     * UTF-8, the result is a string.
     *
     * @param $array The array, or the string, to slice.
     * @param integer $startIndex This is the zero based starting index. Use
     *                            negative number to index from the end. If
     *                            not specified, an index of 0 is assumed.
     * @param integer $endIndex This is the zero based ending index. If
     *                          positive, this is one more than the index of
     *                          the last value or code point to include. If
     *                          negative, the index is from the end. If not
     *                          specified, the slice is from startIndex up to
     *                          and including the end.
     *
     * @return For an array, a new array containing the selected values; it is
     *          an `array<T>` only if array is one, so values of another data
     *          type can be added to it. For a string, a string containing the
     *          selected code points.
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
     * to_reversed()
     *
     * Return a new array with the entries of array in reverse order, leaving
     * array unchanged. The new array is an `array<T>` only if array is one.
     *
     * @param array $array The array to copy reversed.
     *
     * @return array A new array with the entries of array reversed.
     */
    public function to_reversed(, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "to_reversed");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_spliced()
     *
     * Return a new array that is array with the changes splice would make,
     * leaving array unchanged: deleteCount values removed at startIndex and
     * values inserted there. Negative startIndex counts from the end. If
     * deleteCount is omitted, all values from startIndex to the end are
     * removed.
     *
     * @param array $array The array to copy.
     * @param integer $startIndex Zero-based start index, or negative from the
     *                            end.
     * @param integer $deleteCount Number of values to remove. If omitted,
     *                             remove through the end of the array.
     *                             Negative is treated as zero.
     * @param $values Values to insert at startIndex after removals.
     *
     * @return array A new array with the changes made.
     */
    public function to_spliced(, $array, $startIndex, $deleteCount = null, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "to_spliced");

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
     * preserving the relative order of the inserted values. Returns the new
     * length of the array. The array must not be immutable (for example after
     * freeze).
     *
     * @param array $array Target array. Must not be immutable.
     * @param $values Values to insert at the front, in order.
     *
     * @return integer The new length of the array.
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

    /**
     * with_at()
     *
     * Return a new array that is array with the value at index replaced by
     * value, leaving array unchanged (it is named with_at because with is a
     * reserved word). Negative index counts from the end, as in at. An index
     * out of range is an error.
     *
     * @param array $array The array to copy.
     * @param integer $index Zero-based index, or negative from the end.
     * @param $value The value to put at index.
     *
     * @return array A new array with the value at index replaced.
     */
    public function with_at(, $array, $index, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "with_at");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("index", $index);
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
