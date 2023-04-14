<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for higher_order_list
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category higher_order_list
 */
class higher_order_list
{
    private $session;

    /**
     * Constructs the higher_order_list class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * all_of()
     *
     * Returns true if all values in a list pass the predicate test.
     *
     * @param function $predicate This function is called for each value in
     *                            the first list in values or until false is
     *                            returned. If no calls return false, the
     *                            result is true.
     * @param  $values These are the parameters passed to predicate with the
     *                 exception that the first list is passed one value at a
     *                 time. At least one list is required.
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
     * combination of values from list1 and list2 returns true.
     *
     * @param function $predicate The predicate is passed two parameters, the
     *                            first is a value from list1 and the second
     *                            is a value from list2.
     * @param list $list1
     * @param list $list2
     *
     * @return boolean
     */
    public function all_of_all(, $predicate, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "all_of_all");

        /* pass along required parameters to the request payload */
        $request->set("predicate", $predicate);
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * all_of_any()
     *
     * This function returns true if the result of calling predicate with all
     * of the combination of values from list1 and any of the values of list2
     * returns true.
     *
     * @param function $predicate The predicate is passed two parameters, the
     *                            first is a value from list1 and the second
     *                            is a value from list2.
     * @param list $list1
     * @param list $list2
     *
     * @return boolean
     */
    public function all_of_any(, $predicate, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "all_of_any");

        /* pass along required parameters to the request payload */
        $request->set("predicate", $predicate);
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * any_of()
     *
     * Returns true if any value in a list pass the predicate test.
     *
     * @param function $predicate This function is called for each value in
     *                            the first list in values or until true is
     *                            returned. If no calls return true, the
     *                            result is false.
     * @param  $values These are the parameters passed to predicate with the
     *                 exception that the first list is passed one value at a
     *                 time. At least one list is required.
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
     * combination of values from list2 and any of the values of list1
     * returns true.
     *
     * @param function $predicate The predicate is passed two parameters, the
     *                            first is a value from list1 and the second
     *                            is a value from list2.
     * @param list $list1
     * @param list $list2
     *
     * @return boolean
     */
    public function any_of_all(, $predicate, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "any_of_all");

        /* pass along required parameters to the request payload */
        $request->set("predicate", $predicate);
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * any_of_any()
     *
     * This function returns true if the result of calling predicate with any
     * of the combination of values from list1 and list2 returns true.
     *
     * @param function $predicate The predicate is passed two parameters, the
     *                            first is a value from list1 and the second
     *                            is a value from list2.
     * @param list $list1
     * @param list $list2
     *
     * @return boolean
     */
    public function any_of_any(, $predicate, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "any_of_any");

        /* pass along required parameters to the request payload */
        $request->set("predicate", $predicate);
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * filter()
     *
     * This produces a list containing only values from another list that
     * pass a predicate test.
     *
     * @param function $predicate This is a boolean function that is called
     *                            to determine if a list entry should be
     *                            included in the returned list.
     * @param  $values These are the values passed to the predicate with the
     *                 exception that the first list is passed as the single
     *                 current value from the list. At least one list is
     *                 required.
     *
     * @return list This is the resulting filtered list.
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
     * The predicate is called for each value in the first list in values
     * until true is returned, then that value is returned.
     *
     * @param function $predicate This is a boolean function that is called
     *                            to determine if a list entry passes the
     *                            test.
     * @param  $values These are the values passed to the predicate with the
     *                 exception that the first list is passed as the single
     *                 current value from the list. At least one list is
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
     * This function creates a list of the results of calling functor with
     * each value of the first list in values
     *
     * @param function $functor
     * @param  $values These are the parameters passed to functor with the
     *                 exception that the first list is passed one value at a
     *                 time. At least one list is required.
     *
     * @return list
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
     * Reduce calls functor for each value in list with two parameters,
     * accumulator and value, and must return a value of any dataType.
     * Parameter accumulator is the reduce() accumulator parameter value on
     * first call and the return value of previous functor() call on
     * subsequent calls. The dataType of the return value should normally be
     * the same as accumulator, but this is not required.
     *
     * @param function $functor This function is called for each value in a
     *                          list. The returned value is passed as the
     *                          accumulator parameter on the next call to
     *                          functor().
     * @param  $accumulator This is an initial accumulator value passed to
     *                      functor(). Normally, the dataType of accumulator
     *                      will be the dataTape for the reduce() return
     *                      value, but this is not required.
     * @param list $list This is a list to be reduced.
     *
     * @return  This is the final return value from functor() or the
     *          accumulator parameter value if list is empty.
     */
    public function reduce(, $functor, $accumulator, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "reduce");

        /* pass along required parameters to the request payload */
        $request->set("functor", $functor);
        $request->set("accumulator", $accumulator);
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * sort()
     *
     * This produces a list with values sorted based on result of
     * compareFunction. The compareFunction is passed two values from the
     * list and must return an integer less than 0 if the first value is less
     * than the second value, 0 if they are equal, and a integer greater than
     * 0 if the first value is greater than the second value.
     *
     * @param function $compareFunction This function is called with two
     *                                  value from list.
     * @param list $list This is the list to sort.
     *
     * @return list This the the resulting sorted list.
     */
    public function sort(, $compareFunction, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "sort");

        /* pass along required parameters to the request payload */
        $request->set("compareFunction", $compareFunction);
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
