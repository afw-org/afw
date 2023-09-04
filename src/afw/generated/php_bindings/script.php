<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for script
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category script
 */
class script
{
    private $session;

    /**
     * Constructs the script class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * bag_script()
     *
     * Takes any number of script values and returns an array of array.
     *
     * @param array $values
     *
     * @return array
     */
    public function bag_script(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<script>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_script()
     *
     * This returns the integer number of values in array.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size_script(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<script>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * compile_script()
     *
     * Compile script value and return either an unevaluated adaptive value or
     * a string containing the compiler listing.
     *
     * @param script $source script string to compile
     * @param  $listing If specified, a compiler listing is produced instead
     *                  of an unevaluated expression value.
     * 
     *                  This parameter can be an integer between 0 and 10 of a
     *                  string that is used for indentation. If 0 is
     *                  specified, no whitespace is added to the resulting
     *                  string. If 1 through 10 is specified, that number of
     *                  spaces is used.
     *
     * @return unevaluated
     */
    public function compile_script(, $source, $listing = null)
    {
        $request = $this->$session->request();

        $request->set("function", "compile<script>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($listing != null)
            $request->set('listing', $listing);

        return $request->get_result();
    }

    /**
     * eq_script()
     *
     * Determine if script arg1 is equal to the value of arg2 converted to the
     * data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param script $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_script(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<script>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_script()
     *
     * Determine if for script arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param script $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_script(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<script>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * evaluate_script()
     *
     * Compile and evaluate script value.
     *
     * @param script $source script string to compile and evaluate
     * @param object $additionalUntrustedQualifiedVariables This parameter
     *                                                      supplies
     *                                                      additional
     *                                                      qualified
     *                                                      variables that can
     *                                                      be accessed during
     *                                                      evaluation. These
     *                                                      variables will not
     *                                                      be used by
     *                                                      anything that
     *                                                      needs to ensure
     *                                                      its qualified
     *                                                      variables must
     *                                                      come from a
     *                                                      trusted source,
     *                                                      such as
     *                                                      authorization.
     *                                                      This parameter is
     *                                                      intended to be
     *                                                      used for testing
     *                                                      only and should
     *                                                      not be used for
     *                                                      anything running
     *                                                      in production.
     *
     * @return unevaluated
     */
    public function evaluate_script(, $source, $additionalUntrustedQualifiedVariables = null)
    {
        $request = $this->$session->request();

        $request->set("function", "evaluate<script>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($additionalUntrustedQualifiedVariables != null)
            $request->set('additionalUntrustedQualifiedVariables', $additionalUntrustedQualifiedVariables);

        return $request->get_result();
    }

    /**
     * ge_script()
     *
     * Checks for script arg1 is greater than or equal to script arg2 and
     * return the boolean result.
     *
     * @param script $arg1
     * @param script $arg2
     *
     * @return boolean
     */
    public function ge_script(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<script>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_script()
     *
     * Checks for script arg1 is greater than script arg2 and return the
     * boolean result.
     *
     * @param script $arg1
     * @param script $arg2
     *
     * @return boolean
     */
    public function gt_script(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<script>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_script()
     *
     * Checks whether value is dataType script and return the boolean result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_script(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<script>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_script()
     *
     * Checks for script arg1 is less than or equal to script arg2 and return
     * the boolean result.
     *
     * @param script $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_script(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<script>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_script()
     *
     * Checks for script arg1 is less that script arg2 and return the boolean
     * result.
     *
     * @param script $arg1
     * @param script $arg2
     *
     * @return boolean
     */
    public function lt_script(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<script>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_script()
     *
     * Determine if script arg1 is not equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
     * instead if you want true to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param script $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_script(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<script>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_script()
     *
     * Determine if for script arg1 is not equal to the value or data type of
     * arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param script $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_script(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<script>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * script()
     *
     * Converts value to data type script returning script result.
     *
     * @param  $value Value to convert
     *
     * @return script Converted value
     */
    public function script(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "script");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
