# -*- coding: utf-8 -*-


class AFWException(Exception):
    """
        The base AFW Exception that all other exception classes extend.

    """

class PerformException(AFWException):

    """
        Indicates a non-successful status from the perform request.

    """

class ActionException(AFWException):

    """
        Indicates a non-successful status from the action.

    """

