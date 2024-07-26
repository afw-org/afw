#!/usr/bin/env python3

##
# @file lmdb.py
# @ingroup afw_bindings_python
# @brief This file contains the Python bindings for the lmdb category.
#

# Generated by afwdev generate afw_lmdb

import requests
import json

def reader_check(session, adapterId=None):
    """
    

    Check for stale entries in the reader lock table.

    Args:
        adapterId (str): Adapter Id.

    Returns:
        int: Number of stale slots that were cleared.
    """

    request = session.Request()

    action = {
        "function": "reader_check"
    }

    if adapterId != None:
        action['adapterId'] = adapterId

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def reader_list(session, adapterId=None):
    """
    

    Dump the entries in the reader lock table.

    Args:
        adapterId (str): Adapter Id.

    Returns:
        str: Reader entries.
    """

    request = session.Request()

    action = {
        "function": "reader_list"
    }

    if adapterId != None:
        action['adapterId'] = adapterId

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

