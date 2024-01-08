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
     * Returns true if all values in an array pass the predicate test.
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
     * Returns true if any value in an array pass the predicate test.
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
     * filter()
     *
     * This produces an array containing only values from another array that
     * pass a predicate test.
     *
     * @param function $predicate This is a boolean function that is called to
     *                            determine if an array entry should be
     *                            included in the returned array.
     * @param  $values These are the values passed to the predicate with the
     *                 exception that the first array is passed as the single
     *                 current value from the array. At least one array is
     *                 required.
     *
     * @return array This is the resulting filtered array.
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
     * The predicate is called for each value in the first array in values
     * until true is returned, then that value is returned.
     *
     * @param function $predicate This is a boolean function that is called to
     *                            determine if an array entry passes the test.
     * @param  $values These are the values passed to the predicate with the
     *                 exception that the first array is passed as the single
     *                 current value from the array. At least one array is
     *                 required.
     *
     * @return  The first value that passes the test is returned.
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
     * This function creates an array of the results of calling functor with
     * each value of the first array in values
     *
     * @param function $functor
     * @param  $values These are the parameters passed to functor with the
     *                 exception that the first array is passed one value at a
     *                 time. At least one array is required.
     *
     * @return array
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
     * Reduce calls functor for each value in array with two parameters,
     * accumulator and value, and must return a value of any dataType.
     * Parameter accumulator is the reduce() accumulator parameter value on
     * first call and the return value of previous functor() call on
     * subsequent calls. The dataType of the return value should normally be
     * the same as accumulator, but this is not required.
     *
     * @param function $functor This function is called for each value in an
     *                          array. The returned value is passed as the
     *                          accumulator parameter on the next call to
     *                          functor().
     * @param  $accumulator This is an initial accumulator value passed to
     *                      functor(). Normally, the dataType of accumulator
     *                      will be the dataTape for the reduce() return
     *                      value, but this is not required.
     * @param array $array This is an array to be reduced.
     *
     * @return  This is the final return value from functor() or the
     *          accumulator parameter value if array is empty.
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
     * sort()
     *
     * This produces an array with values sorted based on result of
     * compareFunction. The compareFunction is passed two values from the
     * array and must return an integer less than 0 if the first value is less
     * than the second value, 0 if they are equal, and a integer greater than
     * 0 if the first value is greater than the second value.
     *
     * @param function $compareFunction This function is called with two value
     *                                  from array.
     * @param array $array This is the array to sort.
     *
     * @return array This the the resulting sorted array.
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
