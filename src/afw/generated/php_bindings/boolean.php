<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for boolean
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category boolean
 */
class boolean
{
    private $session;

    /**
     * Constructs the boolean class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * at_least_one_member_of_boolean()
     *
     * Returns boolean true if at least one value in boolean array1 is in
     * boolean array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function at_least_one_member_of_boolean(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_boolean()
     *
     * Takes any number of boolean values and returns an array of array.
     *
     * @param array $values
     *
     * @return array
     */
    public function bag_boolean(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<boolean>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_boolean()
     *
     * This returns the integer number of values in array.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size_boolean(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * boolean()
     *
     * Converts value to data type boolean returning boolean result.
     *
     * @param  $value Value to convert
     *
     * @return boolean Converted value
     */
    public function boolean(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "boolean");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_boolean()
     *
     * Determine if boolean arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'eqx'
     * ('===') instead if you want false to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param boolean $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_boolean(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_boolean()
     *
     * Determine if for boolean arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param boolean $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_boolean(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_boolean()
     *
     * Checks for boolean arg1 is greater than or equal to boolean arg2 and
     * return the boolean result.
     *
     * @param boolean $arg1
     * @param boolean $arg2
     *
     * @return boolean
     */
    public function ge_boolean(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_boolean()
     *
     * Checks for boolean arg1 is greater than boolean arg2 and return the
     * boolean result.
     *
     * @param boolean $arg1
     * @param boolean $arg2
     *
     * @return boolean
     */
    public function gt_boolean(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * intersection_boolean()
     *
     * Returns an array of boolean with the values that are common to both
     * array of boolean array1 and array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return array
     */
    public function intersection_boolean(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_boolean()
     *
     * Checks whether value is dataType boolean and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_boolean(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_boolean()
     *
     * Checks whether boolean value is in array of boolean array and returns
     * the boolean result.
     *
     * @param boolean $value
     * @param array $array
     *
     * @return boolean
     */
    public function is_in_boolean(, $value, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_boolean()
     *
     * Checks for boolean arg1 is less than or equal to boolean arg2 and
     * return the boolean result.
     *
     * @param boolean $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_boolean(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_boolean()
     *
     * Checks for boolean arg1 is less that boolean arg2 and return the
     * boolean result.
     *
     * @param boolean $arg1
     * @param boolean $arg2
     *
     * @return boolean
     */
    public function lt_boolean(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_boolean()
     *
     * Determine if boolean arg1 is not equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use 'nex'
     * ('!==') instead if you want true to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param boolean $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_boolean(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_boolean()
     *
     * Determine if for boolean arg1 is not equal to the value or data type
     * of arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param boolean $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_boolean(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only_boolean()
     *
     * This converts an array of boolean values that contains one value to a
     * single boolean value.
     *
     * @param array $array
     *
     * @return boolean
     */
    public function one_and_only_boolean(, $array)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("array", $array);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * set_equals_boolean()
     *
     * Returns boolean true if boolean array1 and boolean array2 are subsets
     * of each other and return the boolean result.
     *
     * @param array $array1
     * @param array $array2
     *
     * @return boolean
     */
    public function set_equals_boolean(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_boolean()
     *
     * Returns boolean true if the unique values in boolean array1 are all in
     * boolean array2.
     *
     * @param array $array1 The first array.
     * @param array $array2 The second array.
     *
     * @return boolean
     */
    public function subset_boolean(, $array1, $array2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("array1", $array1);
        $request->set("array2", $array2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_boolean()
     *
     * Converts boolean value to string. For array values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param boolean $value A boolean value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_boolean(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * union_boolean()
     *
     * Returns an array of boolean contains all of the unique values in two
     * or more array of boolean values.
     *
     * @param array $arrays Two or more arrays.
     *
     * @return array
     */
    public function union_boolean(, $arrays)
    {
        $request = $this->$session->request();

        $request->set("function", "union<boolean>");

        /* pass along required parameters to the request payload */
        $request->set("arrays", $arrays);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
