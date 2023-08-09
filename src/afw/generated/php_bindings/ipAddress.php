<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for ipAddress
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category ipAddress
 */
class ipAddress
{
    private $session;

    /**
     * Constructs the ipAddress class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * bag_ipAddress()
     *
     * Takes any number of ipAddress values and returns an array of array.
     *
     * @param array $values
     *
     * @return array
     */
    public function bag_ipAddress(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<ipAddress>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_ipAddress()
     *
     * This returns the integer number of values in array.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size_ipAddress(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<ipAddress>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_ipAddress()
     *
     * Determine if ipAddress arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'eqx'
     * ('===') instead if you want false to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param ipAddress $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_ipAddress(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<ipAddress>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_ipAddress()
     *
     * Determine if for ipAddress arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param ipAddress $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_ipAddress(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<ipAddress>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_ipAddress()
     *
     * Checks for ipAddress arg1 is greater than or equal to ipAddress arg2
     * and return the boolean result.
     *
     * @param ipAddress $arg1
     * @param ipAddress $arg2
     *
     * @return boolean
     */
    public function ge_ipAddress(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<ipAddress>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_ipAddress()
     *
     * Checks for ipAddress arg1 is greater than ipAddress arg2 and return
     * the boolean result.
     *
     * @param ipAddress $arg1
     * @param ipAddress $arg2
     *
     * @return boolean
     */
    public function gt_ipAddress(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<ipAddress>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ipAddress()
     *
     * Converts value to data type ipAddress returning ipAddress result.
     *
     * @param  $value Value to convert
     *
     * @return ipAddress Converted value
     */
    public function ipAddress(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "ipAddress");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_ipAddress()
     *
     * Checks whether ipAddress value is in array of ipAddress array and
     * returns the boolean result.
     *
     * @param ipAddress $value
     * @param array $array
     *
     * @return boolean
     */
    public function is_in_ipAddress(, $value, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<ipAddress>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_ipAddress()
     *
     * Checks whether value is dataType ipAddress and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_ipAddress(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<ipAddress>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_ipAddress()
     *
     * Checks for ipAddress arg1 is less than or equal to ipAddress arg2 and
     * return the boolean result.
     *
     * @param ipAddress $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_ipAddress(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<ipAddress>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_ipAddress()
     *
     * Checks for ipAddress arg1 is less that ipAddress arg2 and return the
     * boolean result.
     *
     * @param ipAddress $arg1
     * @param ipAddress $arg2
     *
     * @return boolean
     */
    public function lt_ipAddress(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<ipAddress>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_ipAddress()
     *
     * Determine if ipAddress arg1 is not equal to the value of arg2
     * converted to the data type of arg1 then return the boolean result. Use
     * 'nex' ('!==') instead if you want true to be returned if arg1 and
     * arg2's data type don't match.
     *
     * @param ipAddress $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_ipAddress(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<ipAddress>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_ipAddress()
     *
     * Determine if for ipAddress arg1 is not equal to the value or data type
     * of arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param ipAddress $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_ipAddress(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<ipAddress>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only_ipAddress()
     *
     * This converts an array of ipAddress values that contains one value to
     * a single ipAddress value.
     *
     * @param array $array
     *
     * @return ipAddress
     */
    public function one_and_only_ipAddress(, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<ipAddress>");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * regexp_match_ipAddress()
     *
     * Checks whether ipAddress value matches the regular expression regexp
     * and return the boolean result.
     *
     * @param ipAddress $value
     * @param string $regexp
     *
     * @return boolean
     */
    public function regexp_match_ipAddress(, $value, $regexp)
    {
        $request = $this->$session->request();

        $request->set("function", "regexp_match<ipAddress>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_ipAddress()
     *
     * Converts ipAddress value to string. For array values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param ipAddress $value A ipAddress value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_ipAddress(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<ipAddress>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
