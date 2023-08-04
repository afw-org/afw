<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for objectId
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category objectId
 */
class objectId
{
    private $session;

    /**
     * Constructs the objectId class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * bag_objectId()
     *
     * Takes any number of objectId values and returns a list of list.
     *
     * @param list $values
     *
     * @return list
     */
    public function bag_objectId(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<objectId>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_objectId()
     *
     * This returns the integer number of values in list.
     *
     * @param list $value
     *
     * @return integer
     */
    public function bag_size_objectId(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<objectId>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_objectId()
     *
     * Determine if objectId arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use "eqx"
     * ("===") instead if you want false to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param objectId $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_objectId(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<objectId>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_objectId()
     *
     * Determine if for objectId arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use "eq" ("==") instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param objectId $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_objectId(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<objectId>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_objectId()
     *
     * Checks for objectId arg1 is greater than or equal to objectId arg2 and
     * return the boolean result.
     *
     * @param objectId $arg1
     * @param objectId $arg2
     *
     * @return boolean
     */
    public function ge_objectId(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<objectId>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_objectId()
     *
     * Checks for objectId arg1 is greater than objectId arg2 and return the
     * boolean result.
     *
     * @param objectId $arg1
     * @param objectId $arg2
     *
     * @return boolean
     */
    public function gt_objectId(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<objectId>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_objectId()
     *
     * Checks whether value is dataType objectId and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_objectId(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<objectId>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_objectId()
     *
     * Checks for objectId arg1 is less than or equal to objectId arg2 and
     * return the boolean result.
     *
     * @param objectId $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_objectId(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<objectId>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_objectId()
     *
     * Checks for objectId arg1 is less that objectId arg2 and return the
     * boolean result.
     *
     * @param objectId $arg1
     * @param objectId $arg2
     *
     * @return boolean
     */
    public function lt_objectId(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<objectId>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_objectId()
     *
     * Determine if objectId arg1 is not equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use "nex"
     * ("!==") instead if you want true to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param objectId $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_objectId(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<objectId>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_objectId()
     *
     * Determine if for objectId arg1 is not equal to the value or data type
     * of arg2 then return the boolean result. Use "ne" ("!=") instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param objectId $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_objectId(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<objectId>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * objectId()
     *
     * Converts value to data type objectId returning objectId result.
     *
     * @param  $value Value to convert
     *
     * @return objectId Converted value
     */
    public function objectId(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "objectId");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_objectId()
     *
     * Converts objectId value to string. For list values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param objectId $value A objectId value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_objectId(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<objectId>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
