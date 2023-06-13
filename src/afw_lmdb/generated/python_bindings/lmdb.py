# -*- coding: utf-8 -*-

# Generated by afwdev generate afw_lmdb

import requests
import json

def reader_check(session, adaptorId=None):
    '''
    

    Check for stale entries in the reader lock table.

    Parameters:

        adaptorId (string): Adaptor Id.

    Returns:
    integer: Number of stale slots that were cleared.
    '''

    request = session.Request()

    action = {
        "function": "reader_check"
    }

    if adaptorId != None:
        action['adaptorId'] = adaptorId

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def reader_list(session, adaptorId=None):
    '''
    

    Dump the entries in the reader lock table.

    Parameters:

        adaptorId (string): Adaptor Id.

    Returns:
    string: Reader entries.
    '''

    request = session.Request()

    action = {
        "function": "reader_list"
    }

    if adaptorId != None:
        action['adaptorId'] = adaptorId

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']
