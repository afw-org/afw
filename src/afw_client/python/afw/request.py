# -*- coding: utf-8 -*-

import requests
import json
import sys


class HttpRequest(object):
    """
    """

    def __init__(self, session):

        self._session = session
        self._payload = {
            'actions': []
        }
        
        self._payload['adapterId'] = self._session._config_vars.get('adapterId')
        

    def perform(self):

        """

        def perform(self):

        The fundamental AFW function is perform, which requires 
        a POST payload to specify the function and arguments to
        execute by the server.  It looks like:

        {
            "adapterId": <adapterId>,
            "actions": [
                {
                    "function": <function_to_execute>,
                    "param1": <param1>,
                    "param2": <param2>,
                    ...
                },
                ...
            ]
        }

        On success, this will return the following JSON:

        {
            "result": {
                "actions": [
                    {
                        "result": <result of action>
                        "status": <status of action>
                    },
                    ...
                ],
                "status": <status of actions>,
                "error": <optional error message>
            },
            "resultDataType": "object",
            "status": "success"
        }

        """

        r = self._session._httpSession.post(
            self._session._url,
            data=json.dumps(self._payload)
        )

        # raise default exception for 502 (Bad Gateway)
        if r.status_code == 502:
            r.raise_for_status()

        # try to get and decode the JSON response, if possible
        response = {}
        response_fmt = {}
        try:
            response = r.json()

            # format out the JSON, so it's readable in the exception
            response_fmt = json.dumps(response, sort_keys=True, indent=4)
        except ValueError:
            pass

        if response.get('status') != 'success':
            raise Exception(response_fmt)

        return response

    def add_action(self, action):

        self._payload['actions'].append(action)

    def set(self, prop, value):

        self._payload[prop] = value


class LocalRequest(object):

    """
    """

    def __init__(self, session):

        self._session = session
        self._payload = {
            'actions': []
        }
        
        self._payload['adapterId'] = self._session._config_vars.get('adapterId')
        

    def perform(self):        

        """

        def perform(self):

        The fundamental AFW function is perform, which requires 
        a POST payload to specify the function and arguments to
        execute by the server.  It looks like:

        function(arg1, arg2, ...)

        On success, this will return the following JSON:

        <content-length>
        content...

        """
        

        cmd = 'perform(' + json.dumps(self._payload) + ')'

        # write the length of our request
        self._session._localSession.stdin.write((str(len(cmd)) + '\n'))

        # write the command
        self._session._localSession.stdin.write(cmd)

        # denote end of response with length (0)
        self._session._localSession.stdin.write('0\n')

        # flush input so it can be read
        self._session._localSession.stdin.flush()

        # read the response
        response = ''

        bytes = int(self._session._fifo.readline())
        while bytes < 0:
            # skip passed stderr (negative bytes)
            err = self._fifo.read(bytes * -1)
            # write it directly to stderr
            sys.stderr.write(err)

            bytes = int(self._fifo.readline())     

        while bytes != 0:
            response = response + self._session._fifo.read(bytes)            
            bytes = int(self._session._fifo.readline())
            while bytes < 0:
                # skip passed stderr (negative bytes)
                err = self._fifo.read(bytes * -1)
                # write it directly to stderr
                sys.stderr.write(err)

                bytes = int(self._fifo.readline())

        json_response = json.loads(response)        
        if json_response.get('status') != 'success':
            raise Exception(json_response.get('error'))
        
        return json_response.get('result')

    def add_action(self, action):

        self._payload['actions'].append(action)

    def set(self, prop, value):

        self._payload[prop] = value


