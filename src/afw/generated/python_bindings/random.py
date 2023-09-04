# -*- coding: utf-8 -*-

# Generated by afwdev generate afw

import requests
import json

def random_base64Binary(session, numberOfOctets):
    """
    Generate random octets (base64Binary)

    This returns a specified number of random octets as dataType base64Binary.

    Args:
        numberOfOctets (int): The number of random octets to generate.

    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "random_base64Binary",
        "numberOfOctets": numberOfOctets
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def random_digits(session, numberOfDigits):
    """
    Generate random decimal digits

    Generate a string containing a specified number of random decimal digits.

    Args:
        numberOfDigits (int): The number (1-18) of digits to generate.

    Returns:
        str:
    """

    request = session.Request()

    action = {
        "function": "random_digits",
        "numberOfDigits": numberOfDigits
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def random_hexBinary(session, numberOfOctets):
    """
    Generate random octets (hexBinary)

    This returns a specified number of random octets as dataType hexBinary.

    Args:
        numberOfOctets (int): The number of random octets to generate.

    Returns:
        object:
    """

    request = session.Request()

    action = {
        "function": "random_hexBinary",
        "numberOfOctets": numberOfOctets
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def random_integer(session, min, max):
    """
    Generate a random integer between specified values

    This returns a random integer between specified values inclusive.

    Args:
        min (int): Minimum integer inclusive.

        max (int): Maximum integer inclusive.

    Returns:
        int: A random integer.
    """

    request = session.Request()

    action = {
        "function": "random_integer",
        "min": min,
        "max": max
    }

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

def random_number(session, min=None, max=None):
    """
    Generate a random number between specified values

    This returns a random double between specified values

    Args:
        min (float): Minimum double inclusive. The default is 0.0.

        max (float): Maximum double exclusive. The default is 1.0.

    Returns:
        float:
    """

    request = session.Request()

    action = {
        "function": "random_number"
    }

    if min != None:
        action['min'] = min

    if max != None:
        action['max'] = max

    request.add_action(action)

    response = request.perform()
    if response.get('status') == 'error':
        raise Exception(response.get('error'))

    return response['actions'][0]['result']

