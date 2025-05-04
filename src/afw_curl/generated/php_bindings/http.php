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
     * http_delete()
     *
     * Makes a HTTP DELETE Request.
     *
     * @param string $url HTTP Url.
     * @param array $headers HTTP Headers.
     * @param object $options CURL Options
     *
     * @return object Returns an object describing the response from the HTTP
     *                delete request.
     */
    public function http_delete(, $url, $headers = null, $options = null)
    {
        $request = $this->$session->request();

        $request->set("function", "http_delete");

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
     * http_get()
     *
     * Makes a HTTP GET Request.
     *
     * @param string $url HTTP Url.
     * @param array $headers HTTP Headers.
     * @param object $options CURL Options
     * @param function $headerCallback The optional callback function to read
     *                                 the headers.
     * @param  $headerUserData The user data to pass to the header callback
     *                         function.
     * @param function $bodyCallback The optional callback function to read
     *                               the body.
     * @param  $bodyUserData The user data to pass to the body callback
     *                       function.
     *
     * @return object Returns an object describing the response from the HTTP
     *                GET request.
     */
    public function http_get(, $url, $headers = null, $options = null, $headerCallback = null, $headerUserData = null, $bodyCallback = null, $bodyUserData = null)
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

        if ($headerCallback != null)
            $request->set('headerCallback', $headerCallback);

        if ($headerUserData != null)
            $request->set('headerUserData', $headerUserData);

        if ($bodyCallback != null)
            $request->set('bodyCallback', $bodyCallback);

        if ($bodyUserData != null)
            $request->set('bodyUserData', $bodyUserData);

        return $request->get_result();
    }

    /**
     * http_head()
     *
     * Makes a HTTP HEAD Request.
     *
     * @param string $url HTTP Url.
     * @param array $headers HTTP Headers.
     * @param object $options CURL Options
     *
     * @return object Returns an object describing the response from the HTTP
     *                head request.
     */
    public function http_head(, $url, $headers = null, $options = null)
    {
        $request = $this->$session->request();

        $request->set("function", "http_head");

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
     * http_options()
     *
     * Makes a HTTP OPTIONS Request.
     *
     * @param string $url HTTP Url.
     * @param array $headers HTTP Headers.
     * @param object $options CURL Options
     *
     * @return object Returns an object describing the response from the HTTP
     *                OPTIONS request.
     */
    public function http_options(, $url, $headers = null, $options = null)
    {
        $request = $this->$session->request();

        $request->set("function", "http_options");

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
     * http_patch()
     *
     * Makes a HTTP PATCH Request.
     *
     * @param string $url HTTP Url.
     * @param string $payload Data payload for PATCH.
     * @param array $headers HTTP Headers.
     * @param object $options CURL Options
     *
     * @return object Returns an object describing the response from the HTTP
     *                PATCH request.
     */
    public function http_patch(, $url, $payload = null, $headers = null, $options = null)
    {
        $request = $this->$session->request();

        $request->set("function", "http_patch");

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

    /**
     * http_post()
     *
     * Makes a HTTP POST Request.
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

    /**
     * http_put()
     *
     * Makes a HTTP PUT Request.
     *
     * @param string $url HTTP Url.
     * @param string $payload Data payload for PUT.
     * @param array $headers HTTP Headers.
     * @param object $options CURL Options
     *
     * @return object Returns an object describing the response from the HTTP
     *                PUT request.
     */
    public function http_put(, $url, $payload = null, $headers = null, $options = null)
    {
        $request = $this->$session->request();

        $request->set("function", "http_put");

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
