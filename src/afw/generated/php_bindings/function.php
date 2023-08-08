<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for function
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category function
 */
class function
{
    private $session;

    /**
     * Constructs the function class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * bag_function()
     *
     * Takes any number of function values and returns a list of list.
     *
     * @param array $values
     *
     * @return array
     */
    public function bag_function(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<function>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_function()
     *
     * This returns the integer number of values in list.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size_function(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<function>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_function()
     *
     * Determine if function arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'eqx'
     * ('===') instead if you want false to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param function $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_function(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<function>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_function()
     *
     * Determine if for function arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param function $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_function(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<function>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * function()
     *
     * Converts value to data type function returning function result.
     *
     * @param  $value Value to convert
     *
     * @return function Converted value
     */
    public function function(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "function");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_function()
     *
     * Checks for function arg1 is greater than or equal to function arg2 and
     * return the boolean result.
     *
     * @param function $arg1
     * @param function $arg2
     *
     * @return boolean
     */
    public function ge_function(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<function>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_function()
     *
     * Checks for function arg1 is greater than function arg2 and return the
     * boolean result.
     *
     * @param function $arg1
     * @param function $arg2
     *
     * @return boolean
     */
    public function gt_function(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<function>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_function()
     *
     * Checks whether value is dataType function and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_function(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<function>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_function()
     *
     * Checks for function arg1 is less than or equal to function arg2 and
     * return the boolean result.
     *
     * @param function $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_function(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<function>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_function()
     *
     * Checks for function arg1 is less that function arg2 and return the
     * boolean result.
     *
     * @param function $arg1
     * @param function $arg2
     *
     * @return boolean
     */
    public function lt_function(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<function>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_function()
     *
     * Determine if function arg1 is not equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use 'nex'
     * ('!==') instead if you want true to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param function $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_function(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<function>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_function()
     *
     * Determine if for function arg1 is not equal to the value or data type
     * of arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param function $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_function(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<function>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
