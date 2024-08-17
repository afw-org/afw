<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for http
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file http.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category http
 */
class http
{
    private $session;

    /**
     * Constructs the http class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * http_get()
     *
     * Makes a HTTP GET Request.
     *
     * @param string $url HTTP Url.
     * @param array $headers HTTP Headers.
     * @param object $options CURL Options
     *
     * @return object Returns an object describing the response from the HTTP
     *                GET request.
     */
    public function http_get(, $url, $headers = null, $options = null)
    {
        $request = $this->$session->request();

        $request->set("function", "http_get");

        /* pass along required parameters to the request payload */
        $request->set("url", $url);

        /* pass along any optional parameters to the request payload */
        if ($headers != null)
            $request->set('headers', $headers);

        if ($options != null)
            $request->set('options', $options);

        return $request->get_result();
    }

    /**
     * http_post()
     *
     * Makes a HTTP Post Request.
     *
     * @param string $url HTTP Url.
     * @param string $payload Data payload to POST to the url.
     * @param array $headers HTTP Headers.
     * @param object $options CURL Options
     *
     * @return object Returns an object describing the response from the HTTP
     *                POST request.
     */
    public function http_post(, $url, $payload = null, $headers = null, $options = null)
    {
        $request = $this->$session->request();

        $request->set("function", "http_post");

        /* pass along required parameters to the request payload */
        $request->set("url", $url);

        /* pass along any optional parameters to the request payload */
        if ($payload != null)
            $request->set('payload', $payload);

        if ($headers != null)
            $request->set('headers', $headers);

        if ($options != null)
            $request->set('options', $options);

        return $request->get_result();
    }

}

?>
