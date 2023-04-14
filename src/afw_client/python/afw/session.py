# -*- coding: utf-8 -*-

import requests

from .request import HttpRequest, LocalRequest

import shlex, subprocess
import os, tempfile
import sys

class Session(object):
    """

        A session stores connection properties associated with
        the Adaptive Framework url.  If no url is specified, the
        default is 'local', which uses afw command to locally
        issue the adaptive calls.

    """

    _tmpdir = None
    _filename = None
    _fifo = None    

    def __init__(self, url='local', **kwargs):

        self._config_vars = {}
        self._url = url

        # store away user parameters for the session
        for key in kwargs:
            self._config_vars[key] = kwargs[key]
        
        if url == 'local':
            config = self._config_vars.get('config')
            stdout = self._config_vars.get('stdout', sys.stdout)
            stderr = self._config_vars.get('stderr', sys.stderr)

            # allocate named pipe
            self._tmpdir = tempfile.mkdtemp()
            self._filename = os.path.join(self._tmpdir, 'local')            
            os.mkfifo(self._filename)            

            # Use afw command in "local" mode to execute our command(s)
            command = 'afw -l ' + self._filename
            if config:
                command = command + ' -f ' + config

            args = shlex.split(command)

            # open local session through a subprocess, forwarding stdout/stderr
            self._localSession = subprocess.Popen(
                args, stdin=subprocess.PIPE, stdout=stdout, 
                stderr=stderr, bufsize=1)

            # make sure it is running
            if self._localSession.poll() is None:
                self._fifo = open(self._filename, "r")

            # read the version information and any preliminary text output
            # FIXME verify version
            bytes = int(self._fifo.readline())
            while bytes < 0:
                # skip passed stderr (negative bytes)
                err = self._fifo.read(bytes * -1)
                # write it directly to stderr
                sys.stderr.write(err)

                bytes = int(self._fifo.readline())

            while bytes != 0:
                self._fifo.read(bytes)
                bytes = int(self._fifo.readline())                
                while bytes < 0:
                    # skip passed stderr (negative bytes)
                    err = self._fifo.read(bytes * -1)
                    # write it directly to stderr
                    sys.stderr.write(err)

                    bytes = int(self._fifo.readline())

        else:
            # use the requests Session interface for efficiency
            self._httpSession = requests.Session()

            # always use JSON
            self._httpSession.headers = { 
                'Content-Type': 'application/json',
                'Accept': 'application/json' 
            }
            self._httpSession.verify  = self._config_vars.get('verify')
            self._httpSession.cert    = self._config_vars.get('cert')
            self._httpSession.timeout = self._config_vars.get('timeout')

    def set(self, name, prop):

        self._config_vars[name] = prop

    def Request(self):

        if self._url == 'local':
            return LocalRequest(self)
        else:
            return HttpRequest(self)

    def close(self):
        
        if self._url == 'local':
            if self._fifo:
                # close the named pipe
                self._fifo.close()

            # wait for process to exit
            if self._localSession:
                self._localSession.terminate()
                self._localSession.wait()

            if os.path.exists(self._filename):
                os.remove(self._filename)
                os.rmdir(self._tmpdir)

