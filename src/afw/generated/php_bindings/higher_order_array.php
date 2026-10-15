<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for higher_order_array
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file higher_order_array.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category higher_order_array
 */
class higher_order_array
{
    private $session;

    /**
     * Constructs the higher_order_array class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * all_of()
     *
     * Return true if predicate returns true for every entry of the first
     * array in values (index order), or if that array is empty. Entries whose
     * value is undefined are included. every() is an alias for the common
     * single-array form.
     *
     * @param function $predicate This function is called for each value in
     *                            the first array in values or until false is
     *                            returned. If no calls return false, the
     *                            result is true.
     * @param $values These are the parameters passed to predicate with the
     *                 exception that the first array is passed one value at a
     *                 time. At least one array is required.
     *
     * @return boolean
     */
    public function all_of(, $predicate, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "all_of");

        /* pass along required parameters to the request payload */
        $request->set("predicate", $predicate);
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * all_of_all()
     *
     * Returns true if the result of calling predicate with all of the
     * combination of values from array1 and array2 returns true.
     *
     * @param function $predicate The predicate is passed two parameters, the
     *                            first is a value from array1 and the second
     *                            is a value from array2.
     * @param array $array1
     * @param array $array2
     *
     * @return boolean
     */
    public function all_of_all(, $predicate, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "all_of_all");

        /* pass along required parameters to the request payload */
        $request->set("predicate", $predicate);
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * all_of_any()
     *
     * This function returns true if the result of calling predicate with all
     * of the combination of values from array1 and any of the values of
     * array2 returns true.
     *
     * @param function $predicate The predicate is passed two parameters, the
     *                            first is a value from array1 and the second
     *                            is a value from array2.
     * @param array $array1
     * @param array $array2
     *
     * @return boolean
     */
    public function all_of_any(, $predicate, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "all_of_any");

        /* pass along required parameters to the request payload */
        $request->set("predicate", $predicate);
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * any_of()
     *
     * Return true if predicate returns true for any entry of the first array
     * in values (index order). Entries whose value is undefined are included.
     * Empty array yields false. some() is an alias for the common
     * single-array form.
     *
     * @param function $predicate This function is called for each value in
     *                            the first array in values or until true is
     *                            returned. If no calls return true, the
     *                            result is false.
     * @param $values These are the parameters passed to predicate with the
     *                 exception that the first array is passed one value at a
     *                 time. At least one array is required.
     *
     * @return boolean
     */
    public function any_of(, $predicate, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "any_of");

        /* pass along required parameters to the request payload */
        $request->set("predicate", $predicate);
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * any_of_all()
     *
     * Returns true if, for at least one value of array1, the predicate
     * returns true with every value of array2 (the predicate gets the value
     * from array1 first).
     *
     * @param function $predicate The predicate is passed two parameters, the
     *                            first is a value from array1 and the second
     *                            is a value from array2.
     * @param array $array1
     * @param array $array2
     *
     * @return boolean
     */
    public function any_of_all(, $predicate, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "any_of_all");

        /* pass along required parameters to the request payload */
        $request->set("predicate", $predicate);
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * any_of_any()
     *
     * This function returns true if the result of calling predicate with any
     * of the combination of values from array1 and array2 returns true.
     *
     * @param function $predicate The predicate is passed two parameters, the
     *                            first is a value from array1 and the second
     *                            is a value from array2.
     * @param array $array1
     * @param array $array2
     *
     * @return boolean
     */
    public function any_of_any(, $predicate, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "any_of_any");

        /* pass along required parameters to the request payload */
        $request->set("predicate", $predicate);
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * every()
     *
     * Return true if predicate returns true for every entry of array (index
     * order), or if array is empty. Entries whose value is undefined are
     * included.
     *
     * @param array $array The array to walk. A string is not walked as an
     *                     array: write [...value] to walk its characters.
     * @param function $predicate Called for each entry of array until it
     *                            returns false. A script function is called
     *                            with (value, index, array); a built-in
     *                            function is called with the value alone,
     *                            since its other parameters have their own
     *                            meanings.
     *
     * @return boolean
     */
    public function every(, $array, $predicate)
    {
        $request = $this->$session->request();

        $request->set("function", "every");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("predicate", $predicate);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * filter()
     *
     * Return a new array of the entries of array for which predicate returns
     * true. Every index is considered, including entries whose value is
     * undefined. Order of kept entries is preserved.
     *
     * @param array $array The array to walk. A string is not walked as an
     *                     array: write [...value] to walk its characters.
     * @param function $predicate Returns true to keep the entry. A script
     *                            function is called with (value, index,
     *                            array); a built-in function is called with
     *                            the value alone, since its other parameters
     *                            have their own meanings.
     *
     * @return array A new array of the entries that passed the test (possibly
     *               empty).
     */
    public function filter(, $array, $predicate)
    {
        $request = $this->$session->request();

        $request->set("function", "filter");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("predicate", $predicate);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * find()
     *
     * Call predicate for each entry of array, in index order, until it
     * returns true, then return that entry. Entries whose value is undefined
     * are included. If no entry passes, the result is undefined (the same as
     * a found undefined entry; use find_index if you need to tell those
     * apart).
     *
     * @param array $array The array to walk. A string is not walked as an
     *                     array: write [...value] to walk its characters.
     * @param function $predicate Returns true for the entry to return. A
     *                            script function is called with (value,
     *                            index, array); a built-in function is called
     *                            with the value alone, since its other
     *                            parameters have their own meanings.
     *
     * @return The first matching entry, or undefined if none match.
     */
    public function find(, $array, $predicate)
    {
        $request = $this->$session->request();

        $request->set("function", "find");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("predicate", $predicate);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * find_index()
     *
     * Call predicate for each entry of array, in index order, until it
     * returns true, then return that entry's index. Entries whose value is
     * undefined are included. If no entry passes, the result is -1.
     *
     * @param array $array The array to walk. A string is not walked as an
     *                     array: write [...value] to walk its characters.
     * @param function $predicate Returns true for the entry whose index to
     *                            return. A script function is called with
     *                            (value, index, array); a built-in function
     *                            is called with the value alone, since its
     *                            other parameters have their own meanings.
     *
     * @return integer The index of the first matching entry, or -1 if none
     *                 match.
     */
    public function find_index(, $array, $predicate)
    {
        $request = $this->$session->request();

        $request->set("function", "find_index");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("predicate", $predicate);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * find_last()
     *
     * Call predicate for each entry of array, from the last to the first,
     * until it returns true, then return that entry. Entries whose value is
     * undefined are included. Only the indexes there at the start are
     * visited, each as it is when it is reached; an index predicate removed
     * reads as undefined. If no entry passes, the result is undefined (the
     * same as a found undefined entry; use find_last_index if you need to
     * tell those apart).
     *
     * @param array $array The array to walk. A string is not walked as an
     *                     array: write [...value] to walk its characters.
     * @param function $predicate Returns true for the entry to return. A
     *                            script function is called with (value,
     *                            index, array); a built-in function is called
     *                            with the value alone, since its other
     *                            parameters have their own meanings.
     *
     * @return The last matching entry, or undefined if none match.
     */
    public function find_last(, $array, $predicate)
    {
        $request = $this->$session->request();

        $request->set("function", "find_last");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("predicate", $predicate);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * find_last_index()
     *
     * Call predicate for each entry of array, from the last to the first,
     * until it returns true, then return that entry's index. Entries whose
     * value is undefined are included. If no entry passes, the result is -1.
     *
     * @param array $array The array to walk. A string is not walked as an
     *                     array: write [...value] to walk its characters.
     * @param function $predicate Returns true for the entry whose index to
     *                            return. A script function is called with
     *                            (value, index, array); a built-in function
     *                            is called with the value alone, since its
     *                            other parameters have their own meanings.
     *
     * @return integer The index of the last matching entry, or -1 if none
     *                 match.
     */
    public function find_last_index(, $array, $predicate)
    {
        $request = $this->$session->request();

        $request->set("function", "find_last_index");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("predicate", $predicate);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * flat_map()
     *
     * Call callbackfn once for each entry of array, in index order, and
     * return a new array of the results, with each result that is an array
     * replaced by its entries (map then flat(1)). A result that is not an
     * array is one entry, so a callback can return zero, one or more entries.
     *
     * @param array $array The array to walk. A string is not walked as an
     *                     array: write [...value] to walk its characters.
     * @param function $callbackfn Called for each entry of array; an array
     *                             result adds its entries, any other result
     *                             adds itself. A script function is called
     *                             with (value, index, array); a built-in
     *                             function is called with the value alone,
     *                             since its other parameters have their own
     *                             meanings.
     *
     * @return array A new array of the results, one level flattened.
     */
    public function flat_map(, $array, $callbackfn)
    {
        $request = $this->$session->request();

        $request->set("function", "flat_map");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("callbackfn", $callbackfn);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * group_by()
     *
     * Call callbackfn once for each entry of array, in index order, and
     * return a new object with a property for each key it returned, whose
     * value is an array of the entries that got that key, in order.
     * Properties are in the order their keys first appeared. The key must be
     * a string; it is not converted (write to_string(v) for another data
     * type).
     *
     * @param array $array The array to walk. A string is not walked as an
     *                     array: write [...value] to walk its characters.
     * @param function $callbackfn Returns the key, a string, of the group the
     *                             entry goes in. A script function is called
     *                             with (value, index, array); a built-in
     *                             function is called with the value alone,
     *                             since its other parameters have their own
     *                             meanings.
     *
     * @return object A new object of arrays, one property per key.
     */
    public function group_by(, $array, $callbackfn)
    {
        $request = $this->$session->request();

        $request->set("function", "group_by");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("callbackfn", $callbackfn);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * map()
     *
     * Call callbackfn once for each entry of array, in index order from 0
     * through length minus one, and return a new array of the same length
     * with the results. Entries whose value is undefined (including omitted
     * elements in array literals) are included; callbackfn receives undefined
     * for those indexes.
     *
     * @param array $array The array to walk. A string is not walked as an
     *                     array: write [...value] to walk its characters.
     * @param function $callbackfn Called for each entry of array; its result
     *                             is the entry of the new array at the same
     *                             index. A script function is called with
     *                             (value, index, array); a built-in function
     *                             is called with the value alone, since its
     *                             other parameters have their own meanings.
     *
     * @return array A new array with one result per entry of array.
     */
    public function map(, $array, $callbackfn)
    {
        $request = $this->$session->request();

        $request->set("function", "map");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("callbackfn", $callbackfn);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * reduce()
     *
     * Call callbackfn for each entry of array, in index order, with the
     * accumulator and that entry; each call's result is the accumulator for
     * the next call. If initialValue is passed, it is the first accumulator
     * and every entry is visited; otherwise the first entry is the first
     * accumulator and the walk starts at the second. Undefined entries are
     * visited. An empty array with no initialValue is an error.
     *
     * @param array $array The array to walk. A string is not walked as an
     *                     array: write [...value] to walk its characters.
     * @param function $callbackfn Called for each entry visited; its result
     *                             is the next accumulator. A script function
     *                             is called with (accumulator, value, index,
     *                             array); a built-in function is called with
     *                             (accumulator, value).
     * @param $initialValue The first accumulator. When it is not passed, the
     *                       first entry of array is.
     *
     * @return The last result of callbackfn, or the initial accumulator if
     *          callbackfn is not called.
     */
    public function reduce(, $array, $callbackfn, $initialValue = null)
    {
        $request = $this->$session->request();

        $request->set("function", "reduce");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("callbackfn", $callbackfn);

        /* pass along any optional parameters to the request payload */
        if ($initialValue != null)
            $request->set('initialValue', $initialValue);

        return $request->get_result();
    }

    /**
     * reduce_right()
     *
     * Call callbackfn for each entry of array, from the last to the first,
     * with the accumulator and that entry; each call's result is the
     * accumulator for the next call. If initialValue is passed, it is the
     * first accumulator and every entry is visited; otherwise the last entry
     * is the first accumulator and the walk starts at the one before it.
     * Undefined entries are visited. Only the indexes there at the start are
     * visited, each as it is when it is reached; an index callbackfn removed
     * is skipped. An empty array with no initialValue is an error.
     *
     * @param array $array The array to walk. A string is not walked as an
     *                     array: write [...value] to walk its characters.
     * @param function $callbackfn Called for each entry visited; its result
     *                             is the next accumulator. A script function
     *                             is called with (accumulator, value, index,
     *                             array); a built-in function is called with
     *                             (accumulator, value).
     * @param $initialValue The first accumulator. When it is not passed, the
     *                       last entry of array is.
     *
     * @return The last result of callbackfn, or the initial accumulator if
     *          callbackfn is not called.
     */
    public function reduce_right(, $array, $callbackfn, $initialValue = null)
    {
        $request = $this->$session->request();

        $request->set("function", "reduce_right");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("callbackfn", $callbackfn);

        /* pass along any optional parameters to the request payload */
        if ($initialValue != null)
            $request->set('initialValue', $initialValue);

        return $request->get_result();
    }

    /**
     * some()
     *
     * Return true if predicate returns true for any entry of array (index
     * order). Entries whose value is undefined are included. An empty array
     * yields false.
     *
     * @param array $array The array to walk. A string is not walked as an
     *                     array: write [...value] to walk its characters.
     * @param function $predicate Called for each entry of array until it
     *                            returns true. A script function is called
     *                            with (value, index, array); a built-in
     *                            function is called with the value alone,
     *                            since its other parameters have their own
     *                            meanings.
     *
     * @return boolean
     */
    public function some(, $array, $predicate)
    {
        $request = $this->$session->request();

        $request->set("function", "some");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);
        $request->set("predicate", $predicate);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * sort()
     *
     * Sort the entries of array in place, ordered by compareFn, or, with no
     * compareFn, as < orders them, and return the same array. The sort is
     * stable: entries that compare equal keep their order. With no compareFn,
     * every entry must be of one data type (a mix is an error, as it is for
     * <); entries are compared as values, not as text, so [10, 9, 1] sorts to
     * [1, 9, 10]. NaN sorts after every other double. With compareFn, the
     * array may hold any mix of data types. Undefined entries go last and are
     * not compared. A frozen array is an error before compareFn is called.
     * Use to_sorted for a sorted copy.
     *
     * @param array $array The array to sort. Must not be frozen.
     * @param function $compareFn Called with two defined entries, once per
     *                            comparison. If omitted, entries are ordered
     *                            as < orders them. Return true, or a negative
     *                            integer, when a sorts before b; false, zero
     *                            or a positive integer otherwise. Entries
     *                            that compare equal keep their order. Any
     *                            other result, a double included, is an
     *                            error.
     *
     * @return array The array, sorted.
     */
    public function sort(, $array, $compareFn = null)
    {
        $request = $this->$session->request();

        $request->set("function", "sort");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        if ($compareFn != null)
            $request->set('compareFn', $compareFn);

        return $request->get_result();
    }

    /**
     * to_sorted()
     *
     * Return a new array with the entries of array ordered by compareFn, or,
     * with no compareFn, as < orders them, leaving array unchanged. The sort
     * is stable: entries that compare equal keep their order. Undefined
     * entries go last and are not compared. With no compareFn, every entry
     * must be of one data type, and NaN sorts after every other double. The
     * new array is an `array<T>` only if array is one.
     *
     * @param array $array The array to sort.
     * @param function $compareFn Called with two defined entries, once per
     *                            comparison. If omitted, entries are ordered
     *                            as < orders them. Return true, or a negative
     *                            integer, when a sorts before b; false, zero
     *                            or a positive integer otherwise. Entries
     *                            that compare equal keep their order. Any
     *                            other result, a double included, is an
     *                            error.
     *
     * @return array A new array with the entries of array in sorted order.
     */
    public function to_sorted(, $array, $compareFn = null)
    {
        $request = $this->$session->request();

        $request->set("function", "to_sorted");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        if ($compareFn != null)
            $request->set('compareFn', $compareFn);

        return $request->get_result();
    }

}

?>
