<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for objectPath
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category objectPath
 */
class objectPath
{
    private $session;

    /**
     * Constructs the objectPath class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * bag_objectPath()
     *
     * Takes any number of objectPath values and returns an array of array.
     *
     * @param array $values
     *
     * @return array
     */
    public function bag_objectPath(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<objectPath>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_objectPath()
     *
     * This returns the integer number of values in array.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size_objectPath(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<objectPath>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_objectPath()
     *
     * Determine if objectPath arg1 is equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use 'eqx'
     * ('===') instead if you want false to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param objectPath $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_objectPath(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<objectPath>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_objectPath()
     *
     * Determine if for objectPath arg1 is equal to the value and data type
     * of arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param objectPath $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_objectPath(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<objectPath>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_objectPath()
     *
     * Checks for objectPath arg1 is greater than or equal to objectPath arg2
     * and return the boolean result.
     *
     * @param objectPath $arg1
     * @param objectPath $arg2
     *
     * @return boolean
     */
    public function ge_objectPath(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<objectPath>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_objectPath()
     *
     * Checks for objectPath arg1 is greater than objectPath arg2 and return
     * the boolean result.
     *
     * @param objectPath $arg1
     * @param objectPath $arg2
     *
     * @return boolean
     */
    public function gt_objectPath(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<objectPath>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_objectPath()
     *
     * Checks whether value is dataType objectPath and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_objectPath(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<objectPath>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_objectPath()
     *
     * Checks for objectPath arg1 is less than or equal to objectPath arg2
     * and return the boolean result.
     *
     * @param objectPath $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_objectPath(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<objectPath>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_objectPath()
     *
     * Checks for objectPath arg1 is less that objectPath arg2 and return the
     * boolean result.
     *
     * @param objectPath $arg1
     * @param objectPath $arg2
     *
     * @return boolean
     */
    public function lt_objectPath(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<objectPath>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_objectPath()
     *
     * Determine if objectPath arg1 is not equal to the value of arg2
     * converted to the data type of arg1 then return the boolean result. Use
     * 'nex' ('!==') instead if you want true to be returned if arg1 and
     * arg2's data type don't match.
     *
     * @param objectPath $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_objectPath(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<objectPath>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_objectPath()
     *
     * Determine if for objectPath arg1 is not equal to the value or data
     * type of arg2 then return the boolean result. Use 'ne' ('!=') instead
     * if you want arg2 to be converted to the data type of arg1 before
     * comparison.
     *
     * @param objectPath $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_objectPath(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<objectPath>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * objectPath()
     *
     * Converts value to data type objectPath returning objectPath result.
     *
     * @param  $value Value to convert
     *
     * @return objectPath Converted value
     */
    public function objectPath(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "objectPath");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_objectPath()
     *
     * Converts objectPath value to string. For array values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param objectPath $value A objectPath value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_objectPath(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<objectPath>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
