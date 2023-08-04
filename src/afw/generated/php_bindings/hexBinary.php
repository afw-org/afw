<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for hexBinary
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category hexBinary
 */
class hexBinary
{
    private $session;

    /**
     * Constructs the hexBinary class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * at_least_one_member_of_hexBinary()
     *
     * Returns boolean true if at least one value in hexBinary list1 is in
     * hexBinary list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return boolean
     */
    public function at_least_one_member_of_hexBinary(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "at_least_one_member_of<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_hexBinary()
     *
     * Takes any number of hexBinary values and returns a list of list.
     *
     * @param list $values
     *
     * @return list
     */
    public function bag_hexBinary(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<hexBinary>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_hexBinary()
     *
     * This returns the integer number of values in list.
     *
     * @param list $value
     *
     * @return integer
     */
    public function bag_size_hexBinary(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * decode_to_string_hexBinary()
     *
     * Decode hexBinary value to string. An error is thrown if decoded value
     * is not valid UTF-8.
     *
     * @param hexBinary $value The hexBinary value to decode.
     *
     * @return string The decoded string.
     */
    public function decode_to_string_hexBinary(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "decode_to_string<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_hexBinary()
     *
     * Determine if hexBinary arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use "eqx"
     * ("===") instead if you want false to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param hexBinary $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_hexBinary(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_hexBinary()
     *
     * Determine if for hexBinary arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use "eq" ("==") instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param hexBinary $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_hexBinary(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_hexBinary()
     *
     * Checks for hexBinary arg1 is greater than or equal to hexBinary arg2
     * and return the boolean result.
     *
     * @param hexBinary $arg1
     * @param hexBinary $arg2
     *
     * @return boolean
     */
    public function ge_hexBinary(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_hexBinary()
     *
     * Checks for hexBinary arg1 is greater than hexBinary arg2 and return
     * the boolean result.
     *
     * @param hexBinary $arg1
     * @param hexBinary $arg2
     *
     * @return boolean
     */
    public function gt_hexBinary(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * hexBinary()
     *
     * Converts value to data type hexBinary returning hexBinary result.
     *
     * @param  $value Value to convert
     *
     * @return hexBinary Converted value
     */
    public function hexBinary(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "hexBinary");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * intersection_hexBinary()
     *
     * Returns a list of hexBinary with the values that are common to both
     * list of hexBinary list1 and list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return list
     */
    public function intersection_hexBinary(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "intersection<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_hexBinary()
     *
     * Checks whether value is dataType hexBinary and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_hexBinary(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_in_hexBinary()
     *
     * Checks whether hexBinary value is in list of hexBinary list and
     * returns the boolean result.
     *
     * @param hexBinary $value
     * @param list $list
     *
     * @return boolean
     */
    public function is_in_hexBinary(, $value, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "is_in<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_hexBinary()
     *
     * Checks for hexBinary arg1 is less than or equal to hexBinary arg2 and
     * return the boolean result.
     *
     * @param hexBinary $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_hexBinary(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_hexBinary()
     *
     * Checks for hexBinary arg1 is less that hexBinary arg2 and return the
     * boolean result.
     *
     * @param hexBinary $arg1
     * @param hexBinary $arg2
     *
     * @return boolean
     */
    public function lt_hexBinary(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_hexBinary()
     *
     * Determine if hexBinary arg1 is not equal to the value of arg2
     * converted to the data type of arg1 then return the boolean result. Use
     * "nex" ("!==") instead if you want true to be returned if arg1 and
     * arg2's data type don't match.
     *
     * @param hexBinary $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_hexBinary(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_hexBinary()
     *
     * Determine if for hexBinary arg1 is not equal to the value or data type
     * of arg2 then return the boolean result. Use "ne" ("!=") instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param hexBinary $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_hexBinary(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * one_and_only_hexBinary()
     *
     * This converts a list of hexBinary values that contains one value to a
     * single hexBinary value.
     *
     * @param list $list
     *
     * @return hexBinary
     */
    public function one_and_only_hexBinary(, $list)
    {
        $request = $this->$session->request();

        $request->set("function", "one_and_only<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("list", $list);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * set_equals_hexBinary()
     *
     * Returns boolean true if hexBinary list1 and hexBinary list2 are
     * subsets of each other and return the boolean result.
     *
     * @param list $list1
     * @param list $list2
     *
     * @return boolean
     */
    public function set_equals_hexBinary(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "set_equals<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * subset_hexBinary()
     *
     * Returns boolean true if the unique values in hexBinary list1 are all
     * in hexBinary list2.
     *
     * @param list $list1 The first list.
     * @param list $list2 The second list.
     *
     * @return boolean
     */
    public function subset_hexBinary(, $list1, $list2)
    {
        $request = $this->$session->request();

        $request->set("function", "subset<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("list1", $list1);
        $request->set("list2", $list2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_hexBinary()
     *
     * Converts hexBinary value to string. For list values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param hexBinary $value A hexBinary value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_hexBinary(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * union_hexBinary()
     *
     * Returns a list of hexBinary contains all of the unique values in two
     * or more list of hexBinary values.
     *
     * @param list $lists Two or more lists.
     *
     * @return list
     */
    public function union_hexBinary(, $lists)
    {
        $request = $this->$session->request();

        $request->set("function", "union<hexBinary>");

        /* pass along required parameters to the request payload */
        $request->set("lists", $lists);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
