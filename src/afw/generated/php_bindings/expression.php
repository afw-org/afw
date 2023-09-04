<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for expression
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category expression
 */
class expression
{
    private $session;

    /**
     * Constructs the expression class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * bag_expression()
     *
     * Takes any number of expression values and returns an array of array.
     *
     * @param array $values
     *
     * @return array
     */
    public function bag_expression(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<expression>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_expression()
     *
     * This returns the integer number of values in array.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size_expression(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<expression>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * compile_expression()
     *
     * Compile expression value and return either an unevaluated adaptive
     * value or a string containing the compiler listing.
     *
     * @param expression $source expression string to compile
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
    public function compile_expression(, $source, $listing = null)
    {
        $request = $this->$session->request();

        $request->set("function", "compile<expression>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($listing != null)
            $request->set('listing', $listing);

        return $request->get_result();
    }

    /**
     * eq_expression()
     *
     * Determine if expression arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param expression $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_expression(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<expression>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_expression()
     *
     * Determine if for expression arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param expression $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_expression(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<expression>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * evaluate_expression()
     *
     * Compile and evaluate expression value.
     *
     * @param expression $source expression string to compile and evaluate
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
    public function evaluate_expression(, $source, $additionalUntrustedQualifiedVariables = null)
    {
        $request = $this->$session->request();

        $request->set("function", "evaluate<expression>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($additionalUntrustedQualifiedVariables != null)
            $request->set('additionalUntrustedQualifiedVariables', $additionalUntrustedQualifiedVariables);

        return $request->get_result();
    }

    /**
     * expression()
     *
     * Converts value to data type expression returning expression result.
     *
     * @param  $value Value to convert
     *
     * @return expression Converted value
     */
    public function expression(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "expression");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_expression()
     *
     * Checks for expression arg1 is greater than or equal to expression arg2
     * and return the boolean result.
     *
     * @param expression $arg1
     * @param expression $arg2
     *
     * @return boolean
     */
    public function ge_expression(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<expression>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_expression()
     *
     * Checks for expression arg1 is greater than expression arg2 and return
     * the boolean result.
     *
     * @param expression $arg1
     * @param expression $arg2
     *
     * @return boolean
     */
    public function gt_expression(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<expression>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_expression()
     *
     * Checks whether value is dataType expression and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_expression(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<expression>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_expression()
     *
     * Checks for expression arg1 is less than or equal to expression arg2 and
     * return the boolean result.
     *
     * @param expression $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_expression(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<expression>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_expression()
     *
     * Checks for expression arg1 is less that expression arg2 and return the
     * boolean result.
     *
     * @param expression $arg1
     * @param expression $arg2
     *
     * @return boolean
     */
    public function lt_expression(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<expression>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_expression()
     *
     * Determine if expression arg1 is not equal to the value of arg2
     * converted to the data type of arg1 then return the boolean result. Use
     * 'nex' ('!==') instead if you want true to be returned if arg1 and
     * arg2's data type don't match.
     *
     * @param expression $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_expression(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<expression>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_expression()
     *
     * Determine if for expression arg1 is not equal to the value or data type
     * of arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param expression $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_expression(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<expression>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_expression()
     *
     * Converts expression value to string. For array values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param expression $value A expression value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_expression(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<expression>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
