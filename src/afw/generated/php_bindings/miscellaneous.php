<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for miscellaneous
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file miscellaneous.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category miscellaneous
 */
class miscellaneous
{
    private $session;

    /**
     * Constructs the miscellaneous class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * compare_uri()
     *
     * Compare two URIs.
     *
     * @param string $uri1 First URI for compare. This URI can not contain an
     *                     asterisk ('*')
     * @param string $uri2 Second URI for compare. This URI can contain
     *                     asterisk ('*') for substitution if isValuePath is
     *                     true and currentPath2 is specified.
     * @param boolean $isValuePath The URIs are adaptive value paths. If one
     *                             of the URIs begins with a single slash '/',
     *                             both must, and each URI will be parsed as
     *                             an adaptive value path (example:
     *                             /adapterId/objectType/objectId.propertyNames).
     * @param string $currentPath If isValuePath is true, this is the current
     *                            path that is used to resolve a relative path
     *                            in the URIs. If isValuePath is not true,
     *                            this parameter is ignored.
     *
     * @return boolean Result of comparison.
     */
    public function compare_uri(, $uri1, $uri2, $isValuePath = null, $currentPath = null)
    {
        $request = $this->$session->request();

        $request->set("function", "compare_uri");

        /* pass along required parameters to the request payload */
        $request->set("uri1", $uri1);
        $request->set("uri2", $uri2);

        /* pass along any optional parameters to the request payload */
        if ($isValuePath != null)
            $request->set('isValuePath', $isValuePath);

        if ($currentPath != null)
            $request->set('currentPath', $currentPath);

        return $request->get_result();
    }

    /**
     * debug()
     *
     * Conditionally, based on the detail parameter, write a value as a string
     * to the debug file descriptor (usually stderr).
     *
     * @param  $value This is the value that will be converted to its string
     *                representation and written. An undefined value is
     *                represented by 'undefined'.
     * @param boolean $detail If true, the string will only written if the
     *                        debug:function_active:detail flag is on. If
     *                        false or not specified, the string will only
     *                        written if the debug:function_active flag is on.
     *
     * @return void
     */
    public function debug(, $value, $detail = null)
    {
        $request = $this->$session->request();

        $request->set("function", "debug");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        if ($detail != null)
            $request->set('detail', $detail);

        return $request->get_result();
    }

    /**
     * execution_start_time_local()
     *
     * Return local dateTime when the execution context was created. This will
     * usually be the start of request time.
     *
     *
     * @return dateTime
     */
    public function execution_start_time_local()
    {
        $request = $this->$session->request();

        $request->set("function", "execution_start_time_local");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * execution_start_time_utc()
     *
     * Return UTC dateTime when the execution context was created. This will
     * usually be the start of request time.
     *
     *
     * @return dateTime
     */
    public function execution_start_time_utc()
    {
        $request = $this->$session->request();

        $request->set("function", "execution_start_time_utc");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * generate_uuid()
     *
     * Generate a UUID.
     *
     *
     * @return string
     */
    public function generate_uuid()
    {
        $request = $this->$session->request();

        $request->set("function", "generate_uuid");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_defined()
     *
     * Return true if the value is not undefined. Does not check whether a
     * variable name is bound — use variable_exists for that. null is defined.
     *
     * @param  $value Value to check.
     *
     * @return boolean True if value is not undefined.
     */
    public function is_defined(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is_defined");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_nullish()
     *
     * Return true if the value is null or undefined. Does not check whether a
     * variable name is bound — use variable_exists for that.
     *
     * @param  $value Value to check.
     *
     * @return boolean True if value is null or undefined.
     */
    public function is_nullish(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is_nullish");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * log()
     *
     * Evaluate and convert value to String and log it.
     *
     * @param  $value Value to log.
     *
     * @return void
     */
    public function log(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "log");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * now_local()
     *
     * Return current local dateTime.
     *
     *
     * @return dateTime
     */
    public function now_local()
    {
        $request = $this->$session->request();

        $request->set("function", "now_local");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * now_utc()
     *
     * Return current UTC dateTime.
     *
     *
     * @return dateTime
     */
    public function now_utc()
    {
        $request = $this->$session->request();

        $request->set("function", "now_utc");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * parse_uri()
     *
     * Parse a URI.
     *
     * @param string $uri URI to parse
     * @param boolean $isValuePath The URI is an adaptive value path. If the
     *                             path begins with a single slash '/', the
     *                             URI will be parsed as an adaptive value
     *                             path (example:
     *                             /adapterId/objectType/objectId.propertyNames)
     * @param string $currentPath If isValuePath is true, this is the current
     *                            path that is used to resolve relative paths.
     *                            If isValuePath is not true, this parameter
     *                            is ignored.
     *
     * @return object Object with results of parse.
     */
    public function parse_uri(, $uri, $isValuePath = null, $currentPath = null)
    {
        $request = $this->$session->request();

        $request->set("function", "parse_uri");

        /* pass along required parameters to the request payload */
        $request->set("uri", $uri);

        /* pass along any optional parameters to the request payload */
        if ($isValuePath != null)
            $request->set('isValuePath', $isValuePath);

        if ($currentPath != null)
            $request->set('currentPath', $currentPath);

        return $request->get_result();
    }

    /**
     * perform()
     *
     * Perform actions right away.
     *
     * @param object $request See /afw/_AdaptiveObjectType_/_AdaptiveActions_
     *                        for more information.
     *
     * @return object Response object.
     */
    public function perform(, $request)
    {
        $request = $this->$session->request();

        $request->set("function", "perform");

        /* pass along required parameters to the request payload */
        $request->set("request", $request);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * pool_bytes_in_use()
     *
     * Return the environment running total of outstanding bytes AFW
     * malloc/calloc asked for and has not yet given back (heap/tracker free,
     * or pool destroy). Rounding and prefixes included. Not APR's private
     * usage; see process_rss() for current process RSS.
     *
     *
     * @return integer Sum of all pools' bytes_allocated.
     */
    public function pool_bytes_in_use()
    {
        $request = $this->$session->request();

        $request->set("function", "pool_bytes_in_use");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * process_rss()
     *
     * Return the current resident set size of this process in kilobytes
     * (Linux /proc/self/statm). A hint at APR and OS usage; APR does not
     * return pages to the OS. Compare with pool_bytes_in_use() for AFW
     * asked-for vs process RSS.
     *
     *
     * @return integer Current RSS in kilobytes, or 0 if unavailable.
     */
    public function process_rss()
    {
        $request = $this->$session->request();

        $request->set("function", "process_rss");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * trace()
     *
     * Write a value to a trace log.
     *
     * @param  $value This is the value that will be converted to its string
     *                representation and written the trace log. An undefined
     *                value is represented by 'undefined'.
     * @param boolean $filter If this optional filter is false, nothing will
     *                        be written to the trace log. The default is
     *                        true.
     * @param integer $number This is an optional number between 1 and 8 that
     *                        is appended to 'trace' to identify the trace
     *                        log. The default is 1.
     *
     * @return void
     */
    public function trace(, $value, $filter = null, $number = null)
    {
        $request = $this->$session->request();

        $request->set("function", "trace");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        if ($filter != null)
            $request->set('filter', $filter);

        if ($number != null)
            $request->set('number', $number);

        return $request->get_result();
    }

    /**
     * variable_exists()
     *
     * Return true if the named variable is bound: a lexical symbol in the
     * current scope chain, or a name defined on a visible qualifier frame.
     * Still true when the value is undefined (including an uninitialized let)
     * or null. False only when the name is not bound. Use is_defined /
     * is_nullish for the value.
     *
     * @param string $name Name of variable to check. Optionally
     *                     qualifier::name.
     *
     * @return boolean True if the name is bound.
     */
    public function variable_exists(, $name)
    {
        $request = $this->$session->request();

        $request->set("function", "variable_exists");

        /* pass along required parameters to the request payload */
        $request->set("name", $name);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * variable_get()
     *
     * Return the value of a bound variable. Optional default applies only
     * when the name is not bound — not when the value is undefined. If
     * unbound and no default is given, the result is undefined. The default
     * is the evaluated value at that moment (identity).
     *
     * @param string $name Name of variable to get. Optionally
     *                     qualifier::name.
     * @param  $defaultValue Value to return only if the name is not bound.
     *                       The evaluated value at that moment (identity).
     *
     * @return  Bound variable value, or default / undefined if unbound.
     */
    public function variable_get(, $name, $defaultValue = null)
    {
        $request = $this->$session->request();

        $request->set("function", "variable_get");

        /* pass along required parameters to the request payload */
        $request->set("name", $name);

        /* pass along any optional parameters to the request payload */
        if ($defaultValue != null)
            $request->set('defaultValue', $defaultValue);

        return $request->get_result();
    }

    /**
     * variable_is_not_null()
     *
     * Return true if the named variable is bound and its value is not
     * Adaptive null. Undefined (including an uninitialized let) counts as not
     * null. False if the name is not bound or the value is null. This is not
     * the same as is_defined or not is_nullish.
     *
     * @param string $name Name of variable to check. Optionally
     *                     qualifier::name.
     *
     * @return boolean True if bound and value is not Adaptive null.
     */
    public function variable_is_not_null(, $name)
    {
        $request = $this->$session->request();

        $request->set("function", "variable_is_not_null");

        /* pass along required parameters to the request payload */
        $request->set("name", $name);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
