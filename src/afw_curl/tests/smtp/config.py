#!/usr/bin/env python3

import os
import re
import socket
import threading

# test configuration settings
Environment = "curl"

# Minimal local SMTP stub for smtp_send() regression tests (C1: curl upload
# READFUNCTION heap overflow). Runs for the lifetime of this test group,
# accepting one connection per smtp_send() call. It reads the expected
# payload length out of the MAIL FROM address (e.g. "stub-len-500000@..."),
# so each test case is self-contained without extra coordination, and
# rejects (SMTP 5xx) if the bytes actually received during DATA don't match
# -- which surfaces in the .as test as a thrown error from smtp_send().
_server_socket = None
_server_thread = None
_stop_event = None


def _read_line(conn):
    buf = b""
    while not buf.endswith(b"\r\n"):
        chunk = conn.recv(1)
        if not chunk:
            break
        buf += chunk
    return buf


def _handle_connection(conn):
    conn.settimeout(20)

    def send(s):
        conn.sendall(s.encode())

    send("220 localhost ESMTP stub\r\n")

    expected_len = None
    while True:
        line = _read_line(conn)
        if not line:
            return
        upper = line.upper()
        if upper.startswith(b"MAIL FROM"):
            m = re.search(rb"stub-len-(\d+)@", line)
            if m:
                expected_len = int(m.group(1))
            send("250 OK\r\n")
        elif upper.startswith(b"DATA"):
            send("354 End data with <CR><LF>.<CR><LF>\r\n")
            break
        else:
            send("250 OK\r\n")

    terminator = b"\r\n.\r\n"
    data = b""
    while terminator not in data:
        chunk = conn.recv(65536)
        if not chunk:
            return
        data += chunk
    body = data[:data.index(terminator)]

    if expected_len is not None and len(body) == expected_len:
        send("250 OK: queued\r\n")
    else:
        send("554 Transaction failed: length mismatch (expected {}, got {})\r\n"
             .format(expected_len, len(body)))

    _read_line(conn)  # QUIT
    send("221 Bye\r\n")


def _serve():
    while not _stop_event.is_set():
        try:
            _server_socket.settimeout(0.5)
            conn, _ = _server_socket.accept()
        except socket.timeout:
            continue
        except OSError:
            break
        try:
            _handle_connection(conn)
        except Exception:
            pass
        finally:
            conn.close()


def before_all():
    global _server_socket, _server_thread, _stop_event

    _server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    _server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    _server_socket.bind(("127.0.0.1", 0))
    _server_socket.listen(5)

    # port 0 lets the OS assign a free port; publish it to the .as tests
    os.environ["AFW_CURL_TEST_SMTP_PORT"] = str(_server_socket.getsockname()[1])

    _stop_event = threading.Event()
    _server_thread = threading.Thread(target=_serve, daemon=True)
    _server_thread.start()


def after_all():
    global _server_socket, _server_thread, _stop_event

    if _stop_event:
        _stop_event.set()
    if _server_socket:
        _server_socket.close()
    if _server_thread:
        _server_thread.join(timeout=5)
