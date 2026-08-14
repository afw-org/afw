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
     * @param  $values These are the parameters passed to predicate with the
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
     * @param  $values These are the parameters passed to predicate with the
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
     * Returns true if the result of calling predicate with all of the
     * combination of values from array2 and any of the values of array1
     * returns true.
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
     * Return true if predicate returns true for every entry of the first
     * array in values (index order), or if that array is empty. Entries whose
     * value is undefined are included. Same single-array behavior as all_of.
     *
     * @param function $predicate Called for each value in the first array in
     *                            values or until false is returned.
     * @param  $values Parameters passed to predicate with the first array
     *                 passed one value at a time.
     *
     * @return boolean
     */
    public function every(, $predicate, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "every");

        /* pass along required parameters to the request payload */
        $request->set("predicate", $predicate);
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * filter()
     *
     * Return a new array of entries from the first array in values for which
     * predicate returns true. Every index is considered, including entries
     * whose value is undefined. Order of kept entries is preserved.
     *
     * @param function $predicate This is a boolean function that is called to
     *                            determine if an array entry should be
     *                            included in the returned array.
     * @param  $values These are the values passed to the predicate with the
     *                 exception that the first array is passed as the single
     *                 current value from the array. At least one array is
     *                 required.
     *
     * @return array A new array of the entries that passed the test (possibly
     *               empty).
     */
    public function filter(, $predicate, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "filter");

        /* pass along required parameters to the request payload */
        $request->set("predicate", $predicate);
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * find()
     *
     * Call predicate for each entry of the first array in values, in index
     * order, until it returns true, then return that entry. Entries whose
     * value is undefined are included. If no entry passes, the result is
     * undefined (the same as a found undefined entry; use filter if you need
     * to tell those apart).
     *
     * @param function $predicate This is a boolean function that is called to
     *                            determine if an array entry passes the test.
     * @param  $values These are the values passed to the predicate with the
     *                 exception that the first array is passed as the single
     *                 current value from the array. At least one array is
     *                 required.
     *
     * @return  The first matching entry, or undefined if none match.
     */
    public function find(, $predicate, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "find");

        /* pass along required parameters to the request payload */
        $request->set("predicate", $predicate);
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * map()
     *
     * Call functor once for each entry of the first array in values, in index
     * order from 0 through length minus one, and return a new array of the
     * same length with the results. Entries whose value is undefined
     * (including omitted elements in array literals) are included; the
     * functor receives undefined for those indexes. Additional values
     * parameters, if present, are passed through on every call.
     *
     * @param function $functor
     * @param  $values The first array is walked one entry at a time as the
     *                 first argument to functor. Additional parameters are
     *                 passed on every call. At least one array is required.
     *
     * @return array A new array with one result per entry of the first array.
     */
    public function map(, $functor, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "map");

        /* pass along required parameters to the request payload */
        $request->set("functor", $functor);
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * reduce()
     *
     * Call functor for each entry of array, in index order, with the current
     * accumulator and that entry. The first call uses the accumulator
     * argument; each later call uses the previous return value. Every index
     * is visited, including undefined entries. If array is empty, the
     * accumulator argument is returned without calling functor.
     *
     * @param function $functor This function is called for each value in an
     *                          array. The returned value is passed as the
     *                          accumulator parameter on the next call to
     *                          functor().
     * @param  $accumulator This is an initial accumulator value passed to
     *                      functor(). Normally, the dataType of accumulator
     *                      will be the data type for the reduce() return
     *                      value, but this is not required.
     * @param array $array This is an array to be reduced.
     *
     * @return  The final value returned by functor, or the initial
     *          accumulator if array is empty.
     */
    public function reduce(, $functor, $accumulator, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "reduce");

        /* pass along required parameters to the request payload */
        $request->set("functor", $functor);
        $request->set("accumulator", $accumulator);
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * some()
     *
     * Return true if predicate returns true for any entry of the first array
     * in values (index order). Entries whose value is undefined are included.
     * Empty array yields false. Same single-array behavior as any_of.
     *
     * @param function $predicate Called for each value in the first array in
     *                            values or until true is returned.
     * @param  $values Parameters passed to predicate with the first array
     *                 passed one value at a time.
     *
     * @return boolean
     */
    public function some(, $predicate, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "some");

        /* pass along required parameters to the request payload */
        $request->set("predicate", $predicate);
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * sort()
     *
     * Return a new array with the same entries as array, ordered using
     * compareFunction. The array must have a single element data type (for
     * example all integers or all strings); mixed or empty untyped arrays are
     * not accepted. compareFunction is called with two entries and must
     * return true when the first should sort before the second (boolean), not
     * a numeric sort key.
     *
     * @param function $compareFunction Return true if value1 should be
     *                                  ordered before value2.
     * @param array $array Array to sort. Must be single-type (all entries the
     *                     same data type).
     *
     * @return array A new array with the entries of array in sorted order.
     */
    public function sort(, $compareFunction, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "sort");

        /* pass along required parameters to the request payload */
        $request->set("compareFunction", $compareFunction);
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
