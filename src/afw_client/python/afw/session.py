# -*- coding: utf-8 -*-

import requests

from .request import HttpRequest, LocalRequest

import shlex, subprocess
import os, tempfile
import sys
import time

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

            # open local session through a subprocess, forwarding stdout/stderr.
            # stdin is binary: local protocol lengths are UTF-8 octet counts.
            self._localSession = subprocess.Popen(
                args, stdin=subprocess.PIPE, stdout=stdout,
                stderr=stderr, bufsize=0)

            # make sure it is running
            if self._localSession.poll() is None:
                #FIXME Temporary fix to allow afw time to start up, but this
                # needs to be fixed in a better way.
                time.sleep(1.0)
                # Binary FIFO: length prefixes are octet counts (see
                # afw_command_local.md). Text mode read(n) counts characters
                # and deadlocks when JSON bodies contain multi-byte UTF-8.
                self._fifo = open(self._filename, "rb", buffering=0)

            # read the version information and any preliminary text output
            # FIXME verify version
            nbytes = self._read_local_length()
            while nbytes < 0:
                # skip past stderr (negative lengths)
                err = self._fifo.read(-nbytes)
                sys.stderr.write(err.decode("utf-8", errors="replace"))
                nbytes = self._read_local_length()

            while nbytes != 0:
                self._fifo.read(nbytes)
                nbytes = self._read_local_length()
                while nbytes < 0:
                    err = self._fifo.read(-nbytes)
                    sys.stderr.write(err.decode("utf-8", errors="replace"))
                    nbytes = self._read_local_length()
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

    def _read_local_length(self):
        """Read a local-mode length line (digits + newline) as an int.

        Protocol lengths are UTF-8 octet counts (afw_command_local.md).
        """
        buf = bytearray()
        while True:
            b = self._fifo.read(1)
            if not b:
                raise EOFError("afw local mode FIFO closed while reading length")
            if b == b"\n":
                break
            buf.extend(b)
        return int(buf.decode("ascii"))

    def _read_exact(self, nbytes):
        """Read exactly nbytes from the FIFO (raw read may return short)."""
        if nbytes == 0:
            return b""
        parts = []
        remaining = nbytes
        while remaining > 0:
            chunk = self._fifo.read(remaining)
            if not chunk:
                got = nbytes - remaining
                raise EOFError(
                    "afw local mode FIFO closed during chunk "
                    "(wanted %d octets, got %d)" % (nbytes, got))
            parts.append(chunk)
            remaining -= len(chunk)
        return b"".join(parts)

    def _read_local_chunk(self):
        """Read one length-prefixed chunk; return (length, payload_bytes).

        length 0 ends a segment; negative length is a stderr chunk.
        """
        nbytes = self._read_local_length()
        if nbytes == 0:
            return 0, b""
        if nbytes < 0:
            return nbytes, self._read_exact(-nbytes)
        return nbytes, self._read_exact(nbytes)

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

