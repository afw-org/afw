# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def authorization_check(session, requestId, resourceId, actionId, object=None, enforce=None, context=None):
    '''
    Perform authorization check

    This function can be used to check if the current subject has a given
    access to the specified resource in the current environment. The returned
    object is object type '_AdaptiveAuthorizationResult_'. The context
    parameter can be used to provide additional contest that will be
    available to the authorization policies as qualified variables. This
    additional context can override any context including subject for testing
    purposes.

    Parameters:

        requestId (string): This is a request id to associate with the
        authorization request. Variable current::requestId can be used to
        access this value in authorization policies.

        resourceId (string): This is the URI of the resource being checked.
        Variable current::resourceId can be used to access this value in
        authorization policies.

        actionId (string): This is the action that on the resource that's
        being checked.  Variable current::actionId can be used to access this
        value in authorization policies.

        object (object): Variable current::object can be used to access this
        value in authorization policies.

        enforce (boolean): This specifies whether an error is thrown if
        access is denied. If false or not specified, an error is not thrown
        and the decision can be checked in the returned object.

        context (object): This specifies additional context information
        available to the authorization policies via qualified variables.

    Returns:
    object: The authorization result.
    '''

    request = session.Request()

    action = {
        "function": "authorization_check",
        "requestId": requestId,
        "resourceId": resourceId,
        "actionId": actionId
    }

    if object != None:
        action['object'] = object

    if enforce != None:
        action['enforce'] = enforce

    if context != None:
        action['context'] = context

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

