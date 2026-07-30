<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for compiler
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file compiler.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category compiler
 */
class compiler
{
    private $session;

    /**
     * Constructs the compiler class, using a session.
     */
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
     * compile_from_file()
     *
     * Load an external adaptive script, json, or template to be compiled and
     * returned.
     *
     * @param string $file The path of the file to include, resolved using
     *                     rootFilePaths (longest matching prefix; host path
     *                     must remain under that root).
     * @param string $compileType The compile type, used by the parser to
     *                            determine how to compile the data. For
     *                            example, 'json', 'relaxed_json', 'script',
     *                            'template'
     *
     * @return any
     */
    public function compile_from_file(, $file, $compileType = null)
    {
        $request = $this->$session->request();

        $request->set("function", "compile_from_file");

        /* pass along required parameters to the request payload */
        $request->set("file", $file);

        /* pass along any optional parameters to the request payload */
        if ($compileType != null)
            $request->set('compileType', $compileType);

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
     * eval_from_file()
     *
     * Load an external adaptive script, json, or template to be compiled and
     * evaluate.
     *
     * @param string $file The path of the file to include, resolved using
     *                     rootFilePaths (longest matching prefix; host path
     *                     must remain under that root).
     * @param string $compileType The compile type, used by the parser to
     *                            determine how to compile the data. For
     *                            example, 'json', 'relaxed_json', 'script',
     *                            'template'
     *
     * @return any
     */
    public function eval_from_file(, $file, $compileType = null)
    {
        $request = $this->$session->request();

        $request->set("function", "eval_from_file");

        /* pass along required parameters to the request payload */
        $request->set("file", $file);

        /* pass along any optional parameters to the request payload */
        if ($compileType != null)
            $request->set('compileType', $compileType);

        return $request->get_result();
    }

    /**
     * evaluate()
     *
     * Evaluate an adaptive value.
     *
     * @param any $value
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
     * @return any Evaluated adaptive value.
     */
    public function evaluate(, $value, $additionalUntrustedQualifiedVariables = null)
    {
        $request = $this->$session->request();

        $request->set("function", "evaluate");

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
     * Returns a new memory object whose properties are the active variables
     * for the given qualifier (issue #9). Built from the current xctx
     * qualifier stack via contribute callbacks; not a live view. Each call
     * creates a fresh object. Intended for debugging, tooling, and tests —
     * not for hot production paths that only need qualifier::name access.
     * 
     * Warning: snapshots can be large. Qualifiers such as environment:: or
     * request:: may contribute many properties (and some values can
     * themselves be large objects). qualifiers() nests a full snapshot per
     * active qualifier name and multiplies that cost. Prefer qualifier::name
     * for normal work; use these functions sparingly and avoid holding or
     * repeatedly rebuilding large snapshots in long-running scripts.
     * 
     * All matching visible stack entries for the qualifier name contribute
     * into one object (most recent first; later entries only fill property
     * names not already set). Get (qualifier::name) uses the same
     * first-defining-frame rule per name (newest → older; first non-null
     * get_cb wins, including present undefined/null values). Default
     * visibility matches normal qualifier::name access right now. Optional
     * includeUntrusted is only meaningful while the xctx is secure: set true
     * so the snapshot includes the same frames you would see with :: if you
     * were less secure (trusted and untrusted). When already not secure, the
     * flag changes nothing.
     *
     * @param string $qualifier This is the qualifier whose variables are to
     *                          be accessed as properties of the returned
     *                          object.
     * @param boolean $includeUntrusted Default false: snapshot matches what
     *                                  qualifier::name can access in the
     *                                  current xctx (while secure, untrusted
     *                                  stack frames with secure=false are
     *                                  omitted). Set true while secure to use
     *                                  the same visibility as running less
     *                                  secure — trusted and untrusted frames
     *                                  (not untrusted-only). When the xctx is
     *                                  not secure, true and false are the
     *                                  same because :: already sees untrusted
     *                                  frames. Does not change hot-path get;
     *                                  only this snapshot. Useful for
     *                                  debugging secure evaluation and for
     *                                  building objects to re-inject as
     *                                  evaluate()'s
     *                                  additionalUntrustedQualifiedVariables.
     *
     * @return  When the qualifier has at least one matching visible stack
     *          entry, each property is a variable name for that qualifier
     *          (values from contribute, most recent entry wins per name).
     *          Fresh object on every call (may be empty if nothing was
     *          contributed). When no matching visible entry exists for that
     *          qualifier name, the result is undefined (nullish), not an
     *          empty object.
     */
    public function qualifier(, $qualifier, $includeUntrusted = null)
    {
        $request = $this->$session->request();

        $request->set("function", "qualifier");

        /* pass along required parameters to the request payload */
        $request->set("qualifier", $qualifier);

        /* pass along any optional parameters to the request payload */
        if ($includeUntrusted != null)
            $request->set('includeUntrusted', $includeUntrusted);

        return $request->get_result();
    }

    /**
     * qualifiers()
     *
     * Returns a new memory object whose properties are active qualifier
     * names; each value is an object of that qualifier's variables (issue
     * #9). Built from the current xctx qualifier stack; each call creates a
     * fresh object. Intended for debugging, tooling, and tests — not for hot
     * production paths that only need qualifier::name access.
     * 
     * Warning: the result can be very large. Each property is a full snapshot
     * of that qualifier (see qualifier()), so environment, request,
     * application, current, and others can all appear as nested objects with
     * many properties. Prefer qualifier::name or qualifier(name) when you
     * need one bag; avoid repeated qualifiers() calls or retaining the result
     * in long-running work.
     * 
     * Each nested variables object is the multi-entry snapshot for that name
     * (all matching visible stack entries contribute; most recent wins per
     * property). A qualifier name is omitted if it is not active (same as
     * qualifier(name) being nullish); never invent an empty nested object for
     * an inactive name. Default visibility matches normal qualifier::name
     * access right now. Optional includeUntrusted is only meaningful while
     * the xctx is secure: set true so each nested snapshot uses the same
     * frame visibility as running less secure (trusted and untrusted). When
     * already not secure, the flag changes nothing.
     *
     * @param boolean $includeUntrusted Default false: only qualifiers/frames
     *                                  visible to qualifier::name in the
     *                                  current xctx. Set true while secure to
     *                                  match less-secure :: visibility
     *                                  (include untrusted frames). When not
     *                                  secure, true and false are the same.
     *                                  Does not change hot-path get. The
     *                                  result shape (qualifier → variables
     *                                  object) is suitable to pass as
     *                                  evaluate()'s
     *                                  additionalUntrustedQualifiedVariables
     *                                  when that is the intent.
     *
     * @return object Each property is an active qualifier name with a value
     *                that is a variables snapshot object for that qualifier.
     *                Inactive names are omitted. Fresh object on every call.
     */
    public function qualifiers(, $includeUntrusted = null)
    {
        $request = $this->$session->request();

        $request->set("function", "qualifiers");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($includeUntrusted != null)
            $request->set('includeUntrusted', $includeUntrusted);

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
