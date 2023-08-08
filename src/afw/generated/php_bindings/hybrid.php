<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for hybrid
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category hybrid
 */
class hybrid
{
    private $session;

    /**
     * Constructs the hybrid class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * bag_hybrid()
     *
     * Takes any number of hybrid values and returns a list of list.
     *
     * @param array $values
     *
     * @return array
     */
    public function bag_hybrid(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<hybrid>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_hybrid()
     *
     * This returns the integer number of values in list.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size_hybrid(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<hybrid>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * compile_hybrid()
     *
     * Compile hybrid value and return either an unevaluated adaptive value
     * or a string containing the compiler listing.
     *
     * @param hybrid $source hybrid string to compile
     * @param  $listing If specified, a compiler listing is produced instead
     *                  of an unevaluated expression value.
     *                  
     *                  This parameter can be an integer between 0 and 10 of
     *                  a string that is used for indentation. If 0 is
     *                  specified, no whitespace is added to the resulting
     *                  string. If 1 through 10 is specified, that number of
     *                  spaces is used.
     *
     * @return unevaluated
     */
    public function compile_hybrid(, $source, $listing = null)
    {
        $request = $this->$session->request();

        $request->set("function", "compile<hybrid>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($listing != null)
            $request->set('listing', $listing);

        return $request->get_result();
    }

    /**
     * eq_hybrid()
     *
     * Determine if hybrid arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'eqx'
     * ('===') instead if you want false to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param hybrid $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_hybrid(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<hybrid>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_hybrid()
     *
     * Determine if for hybrid arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param hybrid $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_hybrid(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<hybrid>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * evaluate_hybrid()
     *
     * Compile and evaluate hybrid value.
     *
     * @param hybrid $source hybrid string to compile and evaluate
     * @param object $additionalUntrustedQualifiedVariables This parameter
     *                                                      supplies
     *                                                      additional
     *                                                      qualified
     *                                                      variables that
     *                                                      can be accessed
     *                                                      during
     *                                                      evaluation. These
     *                                                      variables will
     *                                                      not be used by
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
    public function evaluate_hybrid(, $source, $additionalUntrustedQualifiedVariables = null)
    {
        $request = $this->$session->request();

        $request->set("function", "evaluate<hybrid>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($additionalUntrustedQualifiedVariables != null)
            $request->set('additionalUntrustedQualifiedVariables', $additionalUntrustedQualifiedVariables);

        return $request->get_result();
    }

    /**
     * ge_hybrid()
     *
     * Checks for hybrid arg1 is greater than or equal to hybrid arg2 and
     * return the boolean result.
     *
     * @param hybrid $arg1
     * @param hybrid $arg2
     *
     * @return boolean
     */
    public function ge_hybrid(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<hybrid>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_hybrid()
     *
     * Checks for hybrid arg1 is greater than hybrid arg2 and return the
     * boolean result.
     *
     * @param hybrid $arg1
     * @param hybrid $arg2
     *
     * @return boolean
     */
    public function gt_hybrid(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<hybrid>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * hybrid()
     *
     * Converts value to data type hybrid returning hybrid result.
     *
     * @param  $value Value to convert
     *
     * @return hybrid Converted value
     */
    public function hybrid(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "hybrid");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_hybrid()
     *
     * Checks whether value is dataType hybrid and return the boolean result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_hybrid(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<hybrid>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_hybrid()
     *
     * Checks for hybrid arg1 is less than or equal to hybrid arg2 and return
     * the boolean result.
     *
     * @param hybrid $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_hybrid(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<hybrid>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_hybrid()
     *
     * Checks for hybrid arg1 is less that hybrid arg2 and return the boolean
     * result.
     *
     * @param hybrid $arg1
     * @param hybrid $arg2
     *
     * @return boolean
     */
    public function lt_hybrid(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<hybrid>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_hybrid()
     *
     * Determine if hybrid arg1 is not equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use 'nex'
     * ('!==') instead if you want true to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param hybrid $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_hybrid(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<hybrid>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_hybrid()
     *
     * Determine if for hybrid arg1 is not equal to the value or data type of
     * arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param hybrid $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_hybrid(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<hybrid>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_hybrid()
     *
     * Converts hybrid value to string. For list values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param hybrid $value A hybrid value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_hybrid(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<hybrid>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
