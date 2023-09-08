<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for xpathExpression
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category xpathExpression
 */
class xpathExpression
{
    private $session;

    /**
     * Constructs the xpathExpression class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * bag_size_xpathExpression()
     *
     * This returns the integer number of values in array.
     *
     * @param array $value
     *
     * @return integer
     */
    public function bag_size_xpathExpression(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<xpathExpression>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_xpathExpression()
     *
     * Takes any number of xpathExpression values and returns an array of
     * array.
     *
     * @param array $values
     *
     * @return array
     */
    public function bag_xpathExpression(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<xpathExpression>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * compile_xpathExpression()
     *
     * Compile xpathExpression value and return either an unevaluated adaptive
     * value or a string containing the compiler listing.
     *
     * @param xpathExpression $source xpathExpression string to compile
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
    public function compile_xpathExpression(, $source, $listing = null)
    {
        $request = $this->$session->request();

        $request->set("function", "compile<xpathExpression>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($listing != null)
            $request->set('listing', $listing);

        return $request->get_result();
    }

    /**
     * eq_xpathExpression()
     *
     * Determine if xpathExpression arg1 is equal to the value of arg2
     * converted to the data type of arg1 then return the boolean result. Use
     * 'eqx' ('===') instead if you want false to be returned if arg1 and
     * arg2's data type don't match.
     *
     * @param xpathExpression $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_xpathExpression(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<xpathExpression>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_xpathExpression()
     *
     * Determine if for xpathExpression arg1 is equal to the value and data
     * type of arg2 then return the boolean result. Use 'eq' ('==') instead if
     * you want arg2 to be converted to the data type of arg1 before
     * comparison.
     *
     * @param xpathExpression $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_xpathExpression(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<xpathExpression>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * evaluate_xpathExpression()
     *
     * Compile and evaluate xpathExpression value.
     *
     * @param xpathExpression $source xpathExpression string to compile and
     *                                evaluate
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
    public function evaluate_xpathExpression(, $source, $additionalUntrustedQualifiedVariables = null)
    {
        $request = $this->$session->request();

        $request->set("function", "evaluate<xpathExpression>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($additionalUntrustedQualifiedVariables != null)
            $request->set('additionalUntrustedQualifiedVariables', $additionalUntrustedQualifiedVariables);

        return $request->get_result();
    }

    /**
     * ge_xpathExpression()
     *
     * Checks for xpathExpression arg1 is greater than or equal to
     * xpathExpression arg2 and return the boolean result.
     *
     * @param xpathExpression $arg1
     * @param xpathExpression $arg2
     *
     * @return boolean
     */
    public function ge_xpathExpression(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<xpathExpression>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_xpathExpression()
     *
     * Checks for xpathExpression arg1 is greater than xpathExpression arg2
     * and return the boolean result.
     *
     * @param xpathExpression $arg1
     * @param xpathExpression $arg2
     *
     * @return boolean
     */
    public function gt_xpathExpression(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<xpathExpression>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_xpathExpression()
     *
     * Checks whether value is dataType xpathExpression and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_xpathExpression(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<xpathExpression>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_xpathExpression()
     *
     * Checks for xpathExpression arg1 is less than or equal to
     * xpathExpression arg2 and return the boolean result.
     *
     * @param xpathExpression $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_xpathExpression(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<xpathExpression>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_xpathExpression()
     *
     * Checks for xpathExpression arg1 is less that xpathExpression arg2 and
     * return the boolean result.
     *
     * @param xpathExpression $arg1
     * @param xpathExpression $arg2
     *
     * @return boolean
     */
    public function lt_xpathExpression(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<xpathExpression>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_xpathExpression()
     *
     * Determine if xpathExpression arg1 is not equal to the value of arg2
     * converted to the data type of arg1 then return the boolean result. Use
     * 'nex' ('!==') instead if you want true to be returned if arg1 and
     * arg2's data type don't match.
     *
     * @param xpathExpression $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_xpathExpression(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<xpathExpression>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_xpathExpression()
     *
     * Determine if for xpathExpression arg1 is not equal to the value or data
     * type of arg2 then return the boolean result. Use 'ne' ('!=') instead if
     * you want arg2 to be converted to the data type of arg1 before
     * comparison.
     *
     * @param xpathExpression $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_xpathExpression(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<xpathExpression>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_xpathExpression()
     *
     * Converts xpathExpression value to string. For array values, the
     * to_string() value for each entry is returned separated with commas.
     *
     * @param xpathExpression $value A xpathExpression value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_xpathExpression(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<xpathExpression>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * xpathExpression()
     *
     * Converts value to data type xpathExpression returning xpathExpression
     * result.
     *
     * @param  $value Value to convert
     *
     * @return xpathExpression Converted value
     */
    public function xpathExpression(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "xpathExpression");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * xpath_node_count()
     *
     * The number of nodes in a node-set.
     *
     * @param xpathExpression $nodeset
     *
     * @return integer
     */
    public function xpath_node_count(, $nodeset)
    {
        $request = $this->$session->request();

        $request->set("function", "xpath_node_count");

        /* pass along required parameters to the request payload */
        $request->set("nodeset", $nodeset);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * xpath_node_eq()
     *
     * Checks for xpathExpression arg1 is equal to xpathExpression arg2 and
     * return the boolean result.
     *
     * @param xpathExpression $arg1
     * @param xpathExpression $arg2
     *
     * @return boolean
     */
    public function xpath_node_eq(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "xpath_node_eq");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * xpath_node_match()
     *
     * Returns true if any of the nodes matched by nodeset1 are equal to any
     * of the nodes matched by nodeset2 or their corresponding children.
     *
     * @param xpathExpression $nodeset2
     * @param xpathExpression $arg2
     *
     * @return boolean
     */
    public function xpath_node_match(, $nodeset2, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "xpath_node_match");

        /* pass along required parameters to the request payload */
        $request->set("nodeset2", $nodeset2);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
