#!/usr/bin/env python3

##
# @file logical.py
# @ingroup afw_bindings_python
# @brief This file contains the Python bindings for the logical category.
#

# Generated by afwdev generate afw

import requests
import json

def and_(session, conditions):
    """
    Logical and

    Evaluates 0 or more boolean conditions returning boolean true if there are
    no conditions and boolean false if any condition evaluate to false. All
    conditions after the first false remain unevaluated.

    Args:
        conditions (bool):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "and",
        "conditions": conditions
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def n_of(session, n, conditions):
    """
    Check for n true values

    integer n specifies the number of boolean conditions that follow that must
    evaluate to true for boolean true to be returned. If n is 0, true is
    returned. Once n conditions evaluate to true, true is returned and the
    remaining conditions remain unevaluated.

    Args:
        n (int):

        conditions (bool):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "n_of",
        "n": n,
        "conditions": conditions
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def not_(session, condition):
    """
    Logical not

    Evaluates boolean condition returning boolean true if condition evaluates
    to false and false if condition evaluates to true.

    Args:
        condition (bool):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "not",
        "condition": condition
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def or_(session, conditions):
    """
    Logical or

    Evaluates 0 or more boolean conditions returning boolean false if there
    are no conditions and boolean true if any condition evaluate to true. All
    conditions after the first true remain unevaluated.

    Args:
        conditions (bool):

    Returns:
        bool:
    """

    request = session.Request()

    action = {
        "function": "or",
        "conditions": conditions
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

