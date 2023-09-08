<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for regexp
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category regexp
 */
class regexp
{
    private $session;

    /**
     * Constructs the regexp class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * compile_regexp()
     *
     * Compile regexp value and return either an unevaluated adaptive value or
     * a string containing the compiler listing.
     *
     * @param regexp $source regexp string to compile
     * @param  $listing If specified, a compiler listing is produced instead
     *                  of an unevaluated compiled value.
     * 
     *                  This parameter can be an integer between 0 and 10 of a
     *                  string that is used for indentation. If 0 is
     *                  specified, no whitespace is added to the resulting
     *                  string. If 1 through 10 is specified, that number of
     *                  spaces is used.
     *
     * @return unevaluated
     */
    public function compile_regexp(, $source, $listing = null)
    {
        $request = $this->$session->request();

        $request->set("function", "compile<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($listing != null)
            $request->set('listing', $listing);

        return $request->get_result();
    }

    /**
     * eq_regexp()
     *
     * Determine if regexp arg1 is equal to the value of arg2 converted to the
     * data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param regexp $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_regexp(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_regexp()
     *
     * Determine if for regexp arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param regexp $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_regexp(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_regexp()
     *
     * Checks for regexp arg1 is greater than or equal to regexp arg2 and
     * return the boolean result.
     *
     * @param regexp $arg1
     * @param regexp $arg2
     *
     * @return boolean
     */
    public function ge_regexp(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_regexp()
     *
     * Checks for regexp arg1 is greater than regexp arg2 and return the
     * boolean result.
     *
     * @param regexp $arg1
     * @param regexp $arg2
     *
     * @return boolean
     */
    public function gt_regexp(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_regexp()
     *
     * Checks for regexp arg1 is less than or equal to regexp arg2 and return
     * the boolean result.
     *
     * @param regexp $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_regexp(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_regexp()
     *
     * Checks for regexp arg1 is less that regexp arg2 and return the boolean
     * result.
     *
     * @param regexp $arg1
     * @param regexp $arg2
     *
     * @return boolean
     */
    public function lt_regexp(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_regexp()
     *
     * Determine if regexp arg1 is not equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
     * instead if you want true to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param regexp $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_regexp(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_regexp()
     *
     * Determine if for regexp arg1 is not equal to the value or data type of
     * arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param regexp $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_regexp(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * regexp()
     *
     * Converts value to data type regexp returning regexp result.
     *
     * @param  $value Value to convert
     *
     * @return regexp Converted value
     */
    public function regexp(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "regexp");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
