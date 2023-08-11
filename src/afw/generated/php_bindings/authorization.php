<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for authorization
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category authorization
 */
class authorization
{
    private $session;

    /**
     * Constructs the authorization class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * authorization_check()
     *
     * This function can be used to check if the current subject has a given
     * access to the specified resource in the current environment. The
     * returned object is object type '_AdaptiveAuthorizationResult_'.
     *
     * @param string $requestId This is a request id to associate with the
     *                          authorization request. Variable
     *                          current::requestId can be used to access this
     *                          value in authorization policies.
     * @param string $resourceId This is the URI of the resource being
     *                           checked. Variable current::resourceId can be
     *                           used to access this value in authorization
     *                           policies.
     * @param string $actionId This is the action that on the resource that's
     *                         being checked.  Variable current::actionId can
     *                         be used to access this value in authorization
     *                         policies.
     * @param object $object Variable current::object can be used to access
     *                       this value in authorization policies.
     * @param boolean $enforce This specifies whether an error is thrown if
     *                         access is denied. If false or not specified,
     *                         an error is not thrown and the decision can be
     *                         checked in the returned object.
     *
     * @return object The authorization result.
     */
    public function authorization_check(, $requestId, $resourceId, $actionId, $object = null, $enforce = null)
    {
        $request = $this->$session->request();

        $request->set("function", "authorization_check");

        /* pass along required parameters to the request payload */
        $request->set("requestId", $requestId);
        $request->set("resourceId", $resourceId);
        $request->set("actionId", $actionId);

        /* pass along any optional parameters to the request payload */
        if ($object != null)
            $request->set('object', $object);

        if ($enforce != null)
            $request->set('enforce', $enforce);

        return $request->get_result();
    }

}

?>
