<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for base64Binary
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file base64Binary.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category base64Binary
 */
class base64Binary
{
    private $session;

    /**
     * Constructs the base64Binary class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * at_least_one_member_of_base64Binary()
     *
     * Returns boolean true if at least one value in base64Binary array1 is in
     * base64Binary array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function at_least_one_member_of_base64Binary(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_base64Binary()
     *
     * Takes any number of base64Binary values and returns an array of array.
     *
     * @param array $values
     *
     * @return array
     */
    public function bag_base64Binary(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<base64Binary>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_base64Binary()
     *
     * This returns the integer number of values in array.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size_base64Binary(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * base64Binary()
     *
     * Converts value to data type base64Binary returning base64Binary result.
     *
     * @param  $value Value to convert
     *
     * @return base64Binary Converted value
     */
    public function base64Binary(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "base64Binary");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * decode_to_string_base64Binary()
     *
     * Decode base64Binary value to string. An error is thrown if decoded
     * value is not valid UTF-8.
     *
     * @param base64Binary $value The base64Binary value to decode.
     *
     * @return string The decoded string.
     */
    public function decode_to_string_base64Binary(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "decode_to_string<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_base64Binary()
     *
     * Determine if base64Binary arg1 is equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use 'eqx'
     * ('===') instead if you want false to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param base64Binary $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_base64Binary(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_base64Binary()
     *
     * Determine if for base64Binary arg1 is equal to the value and data type
     * of arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param base64Binary $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_base64Binary(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_base64Binary()
     *
     * Checks for base64Binary arg1 is greater than or equal to base64Binary
     * arg2 and return the boolean result.
     *
     * @param base64Binary $arg1
     * @param base64Binary $arg2
     *
     * @return boolean
     */
    public function ge_base64Binary(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_base64Binary()
     *
     * Checks for base64Binary arg1 is greater than base64Binary arg2 and
     * return the boolean result.
     *
     * @param base64Binary $arg1
     * @param base64Binary $arg2
     *
     * @return boolean
     */
    public function gt_base64Binary(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * intersection_base64Binary()
     *
     * Returns an array of base64Binary with the values that are common to
     * both array of base64Binary array1 and array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return array
     */
    public function intersection_base64Binary(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_base64Binary()
     *
     * Checks whether value is dataType base64Binary and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_base64Binary(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_base64Binary()
     *
     * Checks whether base64Binary value is in array of base64Binary array and
     * returns the boolean result.
     *
     * @param base64Binary $value
     * @param array $array
     *
     * @return boolean
     */
    public function is_in_base64Binary(, $value, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_base64Binary()
     *
     * Checks for base64Binary arg1 is less than or equal to base64Binary arg2
     * and return the boolean result.
     *
     * @param base64Binary $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_base64Binary(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_base64Binary()
     *
     * Checks for base64Binary arg1 is less that base64Binary arg2 and return
     * the boolean result.
     *
     * @param base64Binary $arg1
     * @param base64Binary $arg2
     *
     * @return boolean
     */
    public function lt_base64Binary(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_base64Binary()
     *
     * Determine if base64Binary arg1 is not equal to the value of arg2
     * converted to the data type of arg1 then return the boolean result. Use
     * 'nex' ('!==') instead if you want true to be returned if arg1 and
     * arg2's data type don't match.
     *
     * @param base64Binary $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_base64Binary(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_base64Binary()
     *
     * Determine if for base64Binary arg1 is not equal to the value or data
     * type of arg2 then return the boolean result. Use 'ne' ('!=') instead if
     * you want arg2 to be converted to the data type of arg1 before
     * comparison.
     *
     * @param base64Binary $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_base64Binary(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only_base64Binary()
     *
     * This converts an array of base64Binary values that contains one value
     * to a single base64Binary value.
     *
     * @param array $array
     *
     * @return base64Binary
     */
    public function one_and_only_base64Binary(, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * set_equals_base64Binary()
     *
     * Returns boolean true if base64Binary array1 and base64Binary array2 are
     * subsets of each other and return the boolean result.
     *
     * @param array $array1
     * @param array $array2
     *
     * @return boolean
     */
    public function set_equals_base64Binary(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_base64Binary()
     *
     * Returns boolean true if the unique values in base64Binary array1 are
     * all in base64Binary array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function subset_base64Binary(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_base64Binary()
     *
     * Converts base64Binary value to string. For array values, the
     * to_string() value for each entry is returned separated with commas.
     *
     * @param base64Binary $value A base64Binary value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_base64Binary(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * union_base64Binary()
     *
     * Returns an array of base64Binary contains all of the unique values in
     * two or more array of base64Binary values.
     *
     * @param array $arrays Two or more arrays.
     *
     * @return array
     */
    public function union_base64Binary(, $arrays)
    {
        $request = $this->$session->request();

        $request->set("function", "union<base64Binary>");

        /* pass along required parameters to the request payload */
        $request->set("arrays", $arrays);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
