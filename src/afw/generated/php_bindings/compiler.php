<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for compiler
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category compiler
 */
class compiler
{
    private $session;

    /**
     * Constructs the compiler class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * assert()
     *
     * Assert that a value is true. If not, an assertion_failed error is
     * thrown.
     *
     * @param boolean $assertion This is the assertion to make. If not true,
     *                           an assertion_failed error is thrown.
     * @param string $reason This is an optional reason to include in the
     *                       assertion_failed message.
     *
     * @return void
     */
    public function assert(, $assertion, $reason = null)
    {
        $request = $this->$session->request();

        $request->set("function", "assert");

        /* pass along required parameters to the request payload */
        $request->set("assertion", $assertion);

        /* pass along any optional parameters to the request payload */
        if ($reason != null)
            $request->set('reason', $reason);

        return $request->get_result();
    }

    /**
     * compile_relaxed_json()
     *
     * Compile an adaptive relaxed JSON syntax string and return an adaptive
     * value.
     *
     * @param string $json Adaptive relaxed JSON syntax string to compile.
     * @param  $listing If specified, a compiler listing is produced instead
     *                  of an unevaluated relaxed JSON value.
     * 
     *                  This parameter can be an integer between 0 and 10 of a
     *                  string that is used for indentation. If 0 is
     *                  specified, no whitespace is added to the resulting
     *                  string. If 1 through 10 is specified, that number of
     *                  spaces is used.
     *
     * @return  An unevaluated relaxed JSON value ready for use by function
     *          evaluate() or a string containing the compiler listing.
     */
    public function compile_relaxed_json(, $json, $listing = null)
    {
        $request = $this->$session->request();

        $request->set("function", "compile_relaxed_json");

        /* pass along required parameters to the request payload */
        $request->set("json", $json);

        /* pass along any optional parameters to the request payload */
        if ($listing != null)
            $request->set('listing', $listing);

        return $request->get_result();
    }

    /**
     * decompile()
     *
     * Decompile an adaptive value to string.
     *
     * @param  $value Value to decompile.
     * @param  $whitespace Add whitespace for readability if present and not
     *                     0. This parameter can be an integer between 0 and
     *                     10 or a string that is used for indentation. If 0
     *                     is specified, no whitespace is added to the
     *                     resulting string. If 1 through 10 is specified,
     *                     that number of spaces is used.
     *
     * @return string Decompiled value.
     */
    public function decompile(, $value, $whitespace = null)
    {
        $request = $this->$session->request();

        $request->set("function", "decompile");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        if ($whitespace != null)
            $request->set('whitespace', $whitespace);

        return $request->get_result();
    }

    /**
     * evaluate_value()
     *
     * Evaluate an adaptive value.
     *
     * @param  $value
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
     * @return  Evaluated adaptive value.
     */
    public function evaluate_value(, $value, $additionalUntrustedQualifiedVariables = null)
    {
        $request = $this->$session->request();

        $request->set("function", "evaluate_value");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        if ($additionalUntrustedQualifiedVariables != null)
            $request->set('additionalUntrustedQualifiedVariables', $additionalUntrustedQualifiedVariables);

        return $request->get_result();
    }

    /**
     * evaluate_with_retry()
     *
     * Evaluate a value and retry up to a limit if an exception occurs.
     *
     * @param  $value Value to evaluated
     * @param integer $limit Maximum number to retry if an exception occurs.
     *
     * @return  Evaluated value.
     */
    public function evaluate_with_retry(, $value, $limit)
    {
        $request = $this->$session->request();

        $request->set("function", "evaluate_with_retry");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("limit", $limit);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * qualifier()
     *
     * This function allows the active variables for a qualifier to be
     * accessed as the properties of an object.
     *
     * @param string $qualifier This is the qualifier whose variables are to
     *                          be accessed as properties of the returned
     *                          object.
     * @param boolean $forTesting If specified and true, the object returned
     *                            will be suitable to pass as the
     *                            additionalUntrustedQualifiedVariables
     *                            parameter of evaluate*() functions. This is
     *                            intended for testing purposes and should not
     *                            be used in production.
     *
     * @return object Each property is the name of a variable with the value
     *                influenced by the forTesting property.
     */
    public function qualifier(, $qualifier, $forTesting = null)
    {
        $request = $this->$session->request();

        $request->set("function", "qualifier");

        /* pass along required parameters to the request payload */
        $request->set("qualifier", $qualifier);

        /* pass along any optional parameters to the request payload */
        if ($forTesting != null)
            $request->set('forTesting', $forTesting);

        return $request->get_result();
    }

    /**
     * qualifiers()
     *
     * This function allows the active qualifiers to be accessed as properties
     * of an object. The value of each of these properties is an object whose
     * properties are the variables for the corresponding qualifier.
     *
     * @param boolean $forTesting If specified and true, the object returned
     *                            will be suitable to pass as the
     *                            additionalUntrustedQualifiedVariables
     *                            parameter of evaluate*() functions. This is
     *                            intended for testing purposes and should not
     *                            be used in production.
     *
     * @return object Each property is the name of a qualifier with a value
     *                that is an object whose properties are the variables of
     *                that qualifier. The value of the variable properties is
     *                influenced by the forTesting property.
     */
    public function qualifiers(, $forTesting = null)
    {
        $request = $this->$session->request();

        $request->set("function", "qualifiers");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($forTesting != null)
            $request->set('forTesting', $forTesting);

        return $request->get_result();
    }

    /**
     * safe_evaluate()
     *
     * Return the evaluated adaptive value. If an exception occurs, return
     * evaluated error instead.
     *
     * @param  $value Value to evaluated
     * @param  $error Value to evaluate and return if exception occurs. If an
     *                error occurs evaluating this value, the exception will
     *                continue.
     *
     * @return  Evaluated adaptive value or error value.
     */
    public function safe_evaluate(, $value, $error)
    {
        $request = $this->$session->request();

        $request->set("function", "safe_evaluate");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);
        $request->set("error", $error);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * stringify()
     *
     * Evaluate and decompile an adaptive value to string. For most values
     * this has the effect of producing a string containing json.
     *
     * @param  $value Value to stringify.
     * @param  $replacer Optional replacer function.
     * @param  $whitespace Add whitespace for readability if present and not
     *                     0. This parameter can be an integer between 0 and
     *                     10 or a string that is used for indentation. If 0
     *                     is specified, no whitespace is added to the
     *                     resulting string. If 1 through 10 is specified,
     *                     that number of spaces is used.
     *
     * @return string Evaluated and decompiled value.
     */
    public function stringify(, $value, $replacer = null, $whitespace = null)
    {
        $request = $this->$session->request();

        $request->set("function", "stringify");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        if ($replacer != null)
            $request->set('replacer', $replacer);

        if ($whitespace != null)
            $request->set('whitespace', $whitespace);

        return $request->get_result();
    }

    /**
     * test_script()
     *
     * Compile and evaluate an adaptive script and compare the results to an
     * expected value. Return object with the test's results.
     *
     * @param string $id Id of test
     * @param string $description Description of test
     * @param string $script Script to compile and evaluate.
     * @param  $expected Expected result.
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
     * @return object Test results.
     */
    public function test_script(, $id, $description, $script, $expected = null, $additionalUntrustedQualifiedVariables = null)
    {
        $request = $this->$session->request();

        $request->set("function", "test_script");

        /* pass along required parameters to the request payload */
        $request->set("id", $id);
        $request->set("description", $description);
        $request->set("script", $script);

        /* pass along any optional parameters to the request payload */
        if ($expected != null)
            $request->set('expected', $expected);

        if ($additionalUntrustedQualifiedVariables != null)
            $request->set('additionalUntrustedQualifiedVariables', $additionalUntrustedQualifiedVariables);

        return $request->get_result();
    }

    /**
     * test_template()
     *
     * Compile and evaluate an adaptive template and compare the results to an
     * expected value. Return object with the test's results.
     *
     * @param string $id Id of test
     * @param string $description Description of test
     * @param string $template Template to compile and evaluate.
     * @param  $expected Expected evaluated result.
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
     * @return object Test results.
     */
    public function test_template(, $id, $description, $template, $expected = null, $additionalUntrustedQualifiedVariables = null)
    {
        $request = $this->$session->request();

        $request->set("function", "test_template");

        /* pass along required parameters to the request payload */
        $request->set("id", $id);
        $request->set("description", $description);
        $request->set("template", $template);

        /* pass along any optional parameters to the request payload */
        if ($expected != null)
            $request->set('expected', $expected);

        if ($additionalUntrustedQualifiedVariables != null)
            $request->set('additionalUntrustedQualifiedVariables', $additionalUntrustedQualifiedVariables);

        return $request->get_result();
    }

    /**
     * test_value()
     *
     * Evaluate an adaptive value and compare it to an expected value. Return
     * object with the test's results.
     *
     * @param string $id Id of test
     * @param string $description Description of test
     * @param string $value Value to evaluate
     * @param  $expected Expected result.
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
     * @return object Test results.
     */
    public function test_value(, $id, $description, $value, $expected = null, $additionalUntrustedQualifiedVariables = null)
    {
        $request = $this->$session->request();

        $request->set("function", "test_value");

        /* pass along required parameters to the request payload */
        $request->set("id", $id);
        $request->set("description", $description);
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        if ($expected != null)
            $request->set('expected', $expected);

        if ($additionalUntrustedQualifiedVariables != null)
            $request->set('additionalUntrustedQualifiedVariables', $additionalUntrustedQualifiedVariables);

        return $request->get_result();
    }

}

?>
