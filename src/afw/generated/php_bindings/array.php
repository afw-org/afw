<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for array
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category array
 */
class array
{
    private $session;

    /**
     * Constructs the array class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * add_entries()
     *
     * Add the entries of one or more lists to another.
     *
     * @param array $target Target list. This list must not be immutable.
     * @param array $source Source list(s).
     *
     * @return array The modified target list.
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
     * Construct an array with 0 or more elements.
     *
     * @param  $values A value can refer to any adaptable value belonging to
     *                 any data type or an array expression. In the case of
     *                 an array expression, indicated by '...' followed by an
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
     * bag_array()
     *
     * Takes any number of array values and returns a list of list.
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
     * This returns the integer number of values in list.
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
     * eq_array()
     *
     * Determine if array arg1 is equal to the value of arg2 converted to the
     * data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param array $arg1
     * @param  $arg2
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
     * Determine if for array arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param array $arg1
     * @param  $arg2
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
     * ge_array()
     *
     * Checks for array arg1 is greater than or equal to array arg2 and
     * return the boolean result.
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
     * Checks for array arg1 is greater than array arg2 and return the
     * boolean result.
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
     * Checks whether or not a list contains any value.
     *
     * @param array $array The list to search.
     * @param  $searchElement Element to find.
     * @param integer $fromIndex Index in the list to start search.
     *
     * @return boolean Indicates if the element is found in list.
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
     * @param  $value Value to check
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
     * Concatenate the string values of the elements of a list with a
     * separator.
     *
     * @param array $value A list of values (statements) of any data type.
     * @param string $separator The separator to use. If not specified, a
     *                          comma (,) is used.
     *
     * @return string Joined list values.
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
     * @param  $arg2
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
     * This is a polymorphic function where array can be any of the supported
     * data types. Return the integer number of entries in datatype list or
     * codepoints in others.
     *
     * @param array $value Returns the number of entries in a list or code
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
     * the data type of arg1 then return the boolean result. Use 'nex'
     * ('!==') instead if you want true to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param array $arg1
     * @param  $arg2
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
     * @param  $arg2
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
     * reverse()
     *
     * Reverse the order of the elements in a list. If the list is typed, the
     * resulting list will be the same type.
     *
     * @param array $array A list to reverse.
     *
     * @return array A list with elements reversed.
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
     * slice()
     *
     * This function extracts a consecutive slice of values from a list.
     *
     * @param array $array The list to slice.
     * @param integer $startIndex This is the zero based starting index. Use
     *                            negative number to index from the end of
     *                            the list. If not specified, an index of 0
     *                            is assumed.
     * @param integer $endIndex This is the zero based ending index. If
     *                          positive, this is one more than the index of
     *                          the last value to include in the list. If
     *                          negative, the index is from the end of the
     *                          list. If not specified, the slice is from
     *                          startIndex up to and including the end of the
     *                          list.
     *
     * @return array A list containing the selected values. If all of the
     *               values are the same data type, the list will be a list
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
     * to_string_array()
     *
     * Converts array value to string. For list values, the to_string() value
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

}

?>
