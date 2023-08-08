<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for template
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category template
 */
class template
{
    private $session;

    /**
     * Constructs the template class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * bag_size_template()
     *
     * This returns the integer number of values in list.
     *
     * @param list $value
     *
     * @return integer
     */
    public function bag_size_template(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<template>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * bag_template()
     *
     * Takes any number of template values and returns a list of list.
     *
     * @param list $values
     *
     * @return list
     */
    public function bag_template(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<template>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * compile_template()
     *
     * Compile template value and return either an unevaluated adaptive value
     * or a string containing the compiler listing.
     *
     * @param template $source template string to compile
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
    public function compile_template(, $source, $listing = null)
    {
        $request = $this->$session->request();

        $request->set("function", "compile<template>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($listing != null)
            $request->set('listing', $listing);

        return $request->get_result();
    }

    /**
     * eq_template()
     *
     * Determine if template arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'eqx'
     * ('===') instead if you want false to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param template $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eq_template(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<template>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_template()
     *
     * Determine if for template arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param template $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function eqx_template(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<template>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * evaluate_template()
     *
     * Compile and evaluate template value.
     *
     * @param template $source template string to compile and evaluate
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
    public function evaluate_template(, $source, $additionalUntrustedQualifiedVariables = null)
    {
        $request = $this->$session->request();

        $request->set("function", "evaluate<template>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($additionalUntrustedQualifiedVariables != null)
            $request->set('additionalUntrustedQualifiedVariables', $additionalUntrustedQualifiedVariables);

        return $request->get_result();
    }

    /**
     * ge_template()
     *
     * Checks for template arg1 is greater than or equal to template arg2 and
     * return the boolean result.
     *
     * @param template $arg1
     * @param template $arg2
     *
     * @return boolean
     */
    public function ge_template(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<template>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_template()
     *
     * Checks for template arg1 is greater than template arg2 and return the
     * boolean result.
     *
     * @param template $arg1
     * @param template $arg2
     *
     * @return boolean
     */
    public function gt_template(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<template>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_template()
     *
     * Checks whether value is dataType template and return the boolean
     * result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_template(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<template>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_template()
     *
     * Checks for template arg1 is less than or equal to template arg2 and
     * return the boolean result.
     *
     * @param template $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function le_template(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<template>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_template()
     *
     * Checks for template arg1 is less that template arg2 and return the
     * boolean result.
     *
     * @param template $arg1
     * @param template $arg2
     *
     * @return boolean
     */
    public function lt_template(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<template>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_template()
     *
     * Determine if template arg1 is not equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use 'nex'
     * ('!==') instead if you want true to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param template $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function ne_template(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<template>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_template()
     *
     * Determine if for template arg1 is not equal to the value or data type
     * of arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param template $arg1
     * @param  $arg2
     *
     * @return boolean
     */
    public function nex_template(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<template>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * template()
     *
     * Converts value to data type template returning template result.
     *
     * @param  $value Value to convert
     *
     * @return template Converted value
     */
    public function template(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "template");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_template()
     *
     * Converts template value to string. For list values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param template $value A template value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_template(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<template>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
