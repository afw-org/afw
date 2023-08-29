# -*- coding: utf-8 -*-

class PerformException(Exception):

    """
        Indicates a non-successful status from the perform request.

    """
    def __init__(self, message, data=None):
        super().__init__(message)
        self.data = data
