# -*- coding: utf-8 -*-

#
# Adaptive Framework
#

"""
Adaptive Framework Library

Adaptive Framework Library is a wrapper for python to encapsulate
the Adaptive Framework RESTful API.  Basic usage:

    >>> import afw
    >>> session = afw.Session(url='https://localhost:8443/', adapter_id='vault', verify=False)
    >>> response = retrieve_objects(session, object_type_id='Users')

"""

__title__ = 'afw'
__author__ = 'Adaptive Framework'
__version__ = '0.9.0'

from .session import Session
from .request import HttpRequest, LocalRequest
