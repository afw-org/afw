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
     * bag_password()
     *
     * Takes any number of password values and returns an array of array.
     *
     * @param array $values
     *
     * @return array
     */
    public function bag_password(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<password>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_password()
     *
     * This returns the integer number of values in array.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size_password(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<password>");

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
     * @param  $arg2
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
     * @param  $arg2
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
     * Checks whether value is dataType password and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_password(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<password>");

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
     * @param  $arg2
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
     * @param  $arg2
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
     * @param  $arg2
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
     * password()
     *
     * Converts value to data type password returning password result.
     *
     * @param  $value Value to convert
     *
     * @return password Converted value
     */
    public function password(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "password");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

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
