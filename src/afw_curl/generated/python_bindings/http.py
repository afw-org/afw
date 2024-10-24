#!/usr/bin/env python3

##
# @file http.py
# @ingroup afw_bindings_python
# @brief This file contains the Python bindings for the http category.
#

# Generated by afwdev generate afw_curl

import requests
import json

def http_get(session, url, headers=None, options=None):
    """
    

    Makes a HTTP GET Request.

    Args:
        url (str): HTTP Url.

        headers (list): HTTP Headers.

        options (dict): CURL Options

    Returns:
        dict: Returns an object describing the response from the HTTP GET
        request.
    """

    request = session.Request()

    action = {
        "function": "http_get",
        "url": url
    }

    if headers != None:
        action['headers'] = headers

    if options != None:
        action['options'] = options

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def http_post(session, url, payload=None, headers=None, options=None):
    """
    

    Makes a HTTP Post Request.

    Args:
        url (str): HTTP Url.

        payload (str): Data payload to POST to the url.

        headers (list): HTTP Headers.

        options (dict): CURL Options

    Returns:
        dict: Returns an object describing the response from the HTTP POST
        request.
    """

    request = session.Request()

    action = {
        "function": "http_post",
        "url": url
    }

    if payload != None:
        action['payload'] = payload

    if headers != None:
        action['headers'] = headers

    if options != None:
        action['options'] = options

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']
