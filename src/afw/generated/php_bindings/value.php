<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for value
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category value
 */
class value
{
    private $session;

    /**
     * Constructs the value class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * meta()
     *
     * This function returns an object containing meta information for an
     * adaptive value.
     *
     * @param  $value This is the adaptive value whose meta object is to be
     *                returned.
     *
     * @return object An object containing the meta information for the
     *                adaptive value. The objectType of this object is
     *                normally _AdaptiveValueMeta_ or one of its descendants.
     */
    public function meta(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "meta");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * metas()
     *
     * This function returns an array of the result of calling meta() for each
     * value in an array or object adaptive value.
     *
     * @param  $value This is an adaptive array or object.
     *
     * @return object An array containing the result of calling meta() of each
     *                value in array or object adaptive value.
     */
    public function metas(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "metas");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
