<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for list
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category list
 */
class list
{
    private $session;

    /**
     * Constructs the list class, using a session.
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
     * @param list $target Target list. This list must not be immutable.
     * @param list $source Source list(s).
     *
     * @return list The modified target list.
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
     * bag_list()
     *
     * Takes any number of list values and returns a list of list.
     *
     * @param list $values
     *
     * @return list
     */
    public function bag_list(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<list>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_list()
     *
     * This returns the integer number of values in list.
     *
     * @param list $value
     *
     * @return integer
     */
    public function bag_size_list(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<list>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * clone_list()
     *
     * Deep clone a list value.
     *
     * @param list $value The list value to clone.
     *
     * @return list The cloned list value.
     */
    public function clone_list(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "clone<list>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_list()
     *
     * Determine if list arg1 is equal to the value of arg2 converted to the
     * data type of arg1 then return the boolean result. Use "eqx" ("===")
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param list $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_list(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<list>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_list()
     *
     * Determine if for list arg1 is equal to the value and data type of arg2
     * then return the boolean result. Use "eq" ("==") instead if you want
     * arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param list $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_list(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<list>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_list()
     *
     * Checks for list arg1 is greater than or equal to list arg2 and return
     * the boolean result.
     *
     * @param list $arg1
     * @param list $arg2
     *
     * @return boolean
     */
    public function ge_list(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<list>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_list()
     *
     * Checks for list arg1 is greater than list arg2 and return the boolean
     * result.
     *
     * @param list $arg1
     * @param list $arg2
     *
     * @return boolean
     */
    public function gt_list(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<list>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * includes_list()
     *
     * Checks whether or not a list contains any value.
     *
     * @param list $list The list to search.
     * @param  $searchElement Element to find.
     * @param integer $fromIndex Index in the list to start search.
     *
     * @return boolean Indicates if the element is found in list.
     */
    public function includes_list(, $list, $searchElement, $fromIndex = null)
    {
        $request = $this->$session->request();

        $request->set("function", "includes<list>");

        /* pass along required parameters to the request payload */
        $request->set("list", $list);
        $request->set("searchElement", $searchElement);

        /* pass along any optional parameters to the request payload */
        if ($fromIndex != null)
            $request->set('fromIndex', $fromIndex);

        return $request->get_result();
    }

    /**
     * is_list()
     *
     * Checks whether value is dataType list and return the boolean result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_list(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<list>");

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
     * @param list $value A list of values of any data type.
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
     * le_list()
     *
     * Checks for list arg1 is less than or equal to list arg2 and return the
     * boolean result.
     *
     * @param list $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_list(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<list>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * length_list()
     *
     * This is a polymorphic function where list can be any of the supported
     * data types. Return the integer number of entries in datatype list or
     * codepoints in others.
     *
     * @param list $value Returns the number of entries in a list or code
     *                    points in others.
     *
     * @return integer
     */
    public function length_list(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "length<list>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * list()
     *
     * Construct a list with 0 or more elements.
     *
     * @param  $values A value can refer to any adaptable value belonging to
     *                 any data type or a list expression. In the case of a
     *                 list expression, indicated by "..." followed by an
     *                 expression that results in a list, every element
     *                 within that list is included in the newly created
     *                 list.
     *
     * @return list The constructed list.
     */
    public function list(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "list");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_list()
     *
     * Checks for list arg1 is less that list arg2 and return the boolean
     * result.
     *
     * @param list $arg1
     * @param list $arg2
     *
     * @return boolean
     */
    public function lt_list(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<list>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_list()
     *
     * Determine if list arg1 is not equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use "nex"
     * ("!==") instead if you want true to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param list $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_list(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<list>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_list()
     *
     * Determine if for list arg1 is not equal to the value or data type of
     * arg2 then return the boolean result. Use "ne" ("!=") instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param list $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_list(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<list>");

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
     * @param list $list A list to reverse.
     *
     * @return list A list with elements reversed.
     */
    public function reverse(, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "reverse");

        /* pass along required parameters to the request payload */
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * slice()
     *
     * This function extracts a consecutive slice of values from a list.
     *
     * @param list $list The list to slice.
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
     * @return list A list containing the selected values. If all of the
     *              values are the same data type, the list will be a list of
     *              that data type.
     */
    public function slice(, $list, $startIndex = null, $endIndex = null)
    {
        $request = $this->$session->request();

        $request->set("function", "slice");

        /* pass along required parameters to the request payload */
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        if ($startIndex != null)
            $request->set('startIndex', $startIndex);

        if ($endIndex != null)
            $request->set('endIndex', $endIndex);

        return $request->get_result();
    }

    /**
     * to_string_list()
     *
     * Converts list value to string. For list values, the to_string() value
     * for each entry is returned separated with commas.
     *
     * @param list $value A list value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_list(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<list>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
