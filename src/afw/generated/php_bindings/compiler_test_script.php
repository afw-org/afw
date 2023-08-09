<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for compiler_test_script
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category compiler_test_script
 */
class compiler_test_script
{
    private $session;

    /**
     * Constructs the compiler_test_script class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * test_script_runtime_support()
     *
     * This is a function called internally as the result of a test_script
     * compile. This function is not intended to be called directly.
     *
     * @param object $testScriptObject A test script results object with the
     *                                 required evaluation result properties
     *                                 missing. The sources will be evaluated
     *                                 and the corresponding test result
     *                                 properties will be set.
     *
     * @return object The testScriptObject object with test result properties
     *                set.
     */
    public function test_script_runtime_support(, $testScriptObject)
    {
        $request = $this->$session->request();

        $request->set("function", "test_script_runtime_support");

        /* pass along required parameters to the request payload */
        $request->set("testScriptObject", $testScriptObject);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
