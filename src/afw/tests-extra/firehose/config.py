#!/usr/bin/env python3
"""Start a private slapd for this leaf and stop it when the leaf ends.

The directory is 127.0.0.1 on a free port. AFW_WORLD_LDAP_URL is set
for the afwfcgi child. Data lives in the test work directory.
"""

import os
import shutil
import socket
import subprocess
import time

_proc = None


def _port():
    sock = socket.socket()
    sock.bind(("127.0.0.1", 0))
    port = sock.getsockname()[1]
    sock.close()
    return port


def _wait(port, proc, log_path):
    deadline = time.time() + 10
    while time.time() < deadline:
        if proc.poll() is not None:
            tail = ""
            try:
                with open(log_path, "r", encoding="utf-8", errors="replace") as fd:
                    tail = fd.read()[-800:]
            except OSError:
                pass
            raise RuntimeError(
                "slapd exited %s. %s" % (proc.returncode, tail))
        try:
            client = socket.create_connection(("127.0.0.1", port), 0.2)
            client.close()
            return
        except OSError:
            time.sleep(0.1)
    raise RuntimeError("slapd did not accept connections on port %s" % port)


def before_all():
    global _proc
    slapd = shutil.which("slapd")
    slapadd = shutil.which("slapadd")
    if not slapd or not slapadd:
        raise RuntimeError(
            "slapd is not installed; the world LDAP adapter needs the slapd package")
    work = os.getcwd()
    data = os.path.join(work, "ldap-data")
    os.makedirs(data, exist_ok=True)
    port = _port()
    conf_path = os.path.join(work, "slapd.conf")
    with open(conf_path, "w", encoding="utf-8") as fd:
        fd.write(
            "include /etc/ldap/schema/core.schema\n"
            "include /etc/ldap/schema/cosine.schema\n"
            "include /etc/ldap/schema/inetorgperson.schema\n"
            "\n"
            "modulepath /usr/lib/ldap\n"
            "moduleload back_mdb\n"
            "\n"
            "database mdb\n"
            "maxsize 10485760\n"
            "suffix \"dc=world,dc=test\"\n"
            "rootdn \"cn=admin,dc=world,dc=test\"\n"
            "rootpw secret\n"
            "directory %s\n"
            "index objectClass eq\n"
            "index cn eq\n" % data)
    add = subprocess.run(
        [slapadd, "-f", conf_path, "-l", os.path.join(work, "ldap", "seed.ldif")],
        capture_output=True, text=True)
    if add.returncode != 0:
        raise RuntimeError(
            "slapadd failed: %s" % ((add.stderr or add.stdout or "").strip()))
    log_path = os.path.join(work, "slapd.log")
    log = open(log_path, "wb")
    _proc = subprocess.Popen(
        [slapd, "-f", conf_path, "-h", "ldap://127.0.0.1:%d" % port, "-d", "0"],
        stdout=log, stderr=subprocess.STDOUT)
    _wait(port, _proc, log_path)
    os.environ["AFW_WORLD_LDAP_URL"] = "ldap://127.0.0.1:%d" % port


def after_all():
    global _proc
    proc = _proc
    _proc = None
    if proc is None or proc.poll() is not None:
        return
    proc.terminate()
    try:
        proc.wait(timeout=5)
    except subprocess.TimeoutExpired:
        proc.kill()
        proc.wait(timeout=5)
