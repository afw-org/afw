<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for password
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file password.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category password
 */
class password
{
    private $session;

    /**
     * Constructs the password class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * array_password()
     *
     * Construct a new array of password from the given values, the same as
     * array() except that the new array is typed: each value, and each entry
     * of a spread array, must be dataType password, and adding a value of
     * another data type to the array later throws. A value of another data
     * type is an error, never converted. This is usually written
     * `array<password>(`...values).
     *
     * @param $values A value of dataType password, or ... followed by an
     *                 expression that results in an array, every entry of
     *                 which is included.
     *
     * @return array The constructed array of password.
     */
    public function array_password(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "array_password");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * as_password()
     *
     * Returns value unchanged if it is dataType password. Otherwise,
     * including when value is undefined, throws an error. It never converts:
     * to get a password from a value of another data type, write a typed
     * literal or call a to_T conversion function where one exists. This is
     * usually written value as password.
     *
     * @param $value Value to assert
     *
     * @return password value, which is dataType password
     */
    public function as_password(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "as_password");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_password()
     *
     * Determine if password arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param password $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function eq_password(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<password>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_password()
     *
     * Determine if for password arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param password $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function eqx_password(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<password>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_password()
     *
     * Checks for password arg1 is greater than or equal to password arg2 and
     * return the boolean result.
     *
     * @param password $arg1
     * @param password $arg2
     *
     * @return boolean
     */
    public function ge_password(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<password>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_password()
     *
     * Checks for password arg1 is greater than password arg2 and return the
     * boolean result.
     *
     * @param password $arg1
     * @param password $arg2
     *
     * @return boolean
     */
    public function gt_password(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<password>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_password()
     *
     * Checks whether value is dataType password and returns the boolean
     * result. It is false for undefined. This can also be written
     * `is<password>(value`).
     *
     * @param $value Value to check
     *
     * @return boolean
     */
    public function is_password(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is_password");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_password()
     *
     * Checks for password arg1 is less than or equal to password arg2 and
     * return the boolean result.
     *
     * @param password $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function le_password(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<password>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_password()
     *
     * Checks for password arg1 is less that password arg2 and return the
     * boolean result.
     *
     * @param password $arg1
     * @param password $arg2
     *
     * @return boolean
     */
    public function lt_password(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<password>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_password()
     *
     * Determine if password arg1 is not equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use 'nex'
     * ('!==') instead if you want true to be returned if arg1 and arg2's data
     * type don't match.
     *
     * @param password $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function ne_password(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<password>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_password()
     *
     * Determine if for password arg1 is not equal to the value or data type
     * of arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param password $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function nex_password(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<password>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_password()
     *
     * Converts password value to string. For array values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param password $value A password value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_password(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<password>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
