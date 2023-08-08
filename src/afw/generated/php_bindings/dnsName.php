<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for dnsName
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category dnsName
 */
class dnsName
{
    private $session;

    /**
     * Constructs the dnsName class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * bag_dnsName()
     *
     * Takes any number of dnsName values and returns a list of list.
     *
     * @param list $values
     *
     * @return list
     */
    public function bag_dnsName(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<dnsName>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_dnsName()
     *
     * This returns the integer number of values in list.
     *
     * @param list $value
     *
     * @return integer
     */
    public function bag_size_dnsName(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * dnsName()
     *
     * Converts value to data type dnsName returning dnsName result.
     *
     * @param  $value Value to convert
     *
     * @return dnsName Converted value
     */
    public function dnsName(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "dnsName");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_dnsName()
     *
     * Determine if dnsName arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'eqx'
     * ('===') instead if you want false to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param dnsName $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_dnsName(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_dnsName()
     *
     * Determine if for dnsName arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param dnsName $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_dnsName(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_dnsName()
     *
     * Checks for dnsName arg1 is greater than or equal to dnsName arg2 and
     * return the boolean result.
     *
     * @param dnsName $arg1
     * @param dnsName $arg2
     *
     * @return boolean
     */
    public function ge_dnsName(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_dnsName()
     *
     * Checks for dnsName arg1 is greater than dnsName arg2 and return the
     * boolean result.
     *
     * @param dnsName $arg1
     * @param dnsName $arg2
     *
     * @return boolean
     */
    public function gt_dnsName(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_dnsName()
     *
     * Checks whether value is dataType dnsName and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_dnsName(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_dnsName()
     *
     * Checks whether dnsName value is in list of dnsName list and returns
     * the boolean result.
     *
     * @param dnsName $value
     * @param list $list
     *
     * @return boolean
     */
    public function is_in_dnsName(, $value, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_dnsName()
     *
     * Checks for dnsName arg1 is less than or equal to dnsName arg2 and
     * return the boolean result.
     *
     * @param dnsName $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_dnsName(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_dnsName()
     *
     * Checks for dnsName arg1 is less that dnsName arg2 and return the
     * boolean result.
     *
     * @param dnsName $arg1
     * @param dnsName $arg2
     *
     * @return boolean
     */
    public function lt_dnsName(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_dnsName()
     *
     * Determine if dnsName arg1 is not equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use 'nex'
     * ('!==') instead if you want true to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param dnsName $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_dnsName(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_dnsName()
     *
     * Determine if for dnsName arg1 is not equal to the value or data type
     * of arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param dnsName $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_dnsName(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only_dnsName()
     *
     * This converts a list of dnsName values that contains one value to a
     * single dnsName value.
     *
     * @param list $list
     *
     * @return dnsName
     */
    public function one_and_only_dnsName(, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * regexp_match_dnsName()
     *
     * Checks whether dnsName value matches the regular expression regexp and
     * return the boolean result.
     *
     * @param dnsName $value
     * @param string $regexp
     *
     * @return boolean
     */
    public function regexp_match_dnsName(, $value, $regexp)
    {
        $request = $this->$session->request();

        $request->set("function", "regexp_match<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("regexp", $regexp);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_dnsName()
     *
     * Converts dnsName value to string. For list values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param dnsName $value A dnsName value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_dnsName(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<dnsName>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
