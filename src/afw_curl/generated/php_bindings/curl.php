<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for curl
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file curl.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category curl
 */
class curl
{
    private $session;

    /**
     * Constructs the curl class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * curl_easy_options()
     *
     * Returns option names that are available with the installed version of
     * libcurl.
     *
     *
     * @return array Returns an array of strings containing the cURL option
     *               names that are available with the installed version of
     *               libcurl.
     */
    public function curl_easy_options()
    {
        $request = $this->$session->request();

        $request->set("function", "curl_easy_options");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * curl_version_info()
     *
     * Returns run-time libcurl version info.
     *
     *
     * @return object Returns an object describing the cURL version
     *                information.
     */
    public function curl_version_info()
    {
        $request = $this->$session->request();

        $request->set("function", "curl_version_info");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
