afw_server_fcgi
===============

## Synopsis

Adaptive Frame FastCGI Support (`afwfcgi` command).

## Signals (POSIX)

* **SIGTERM** / **SIGINT** — set `env->terminating`, call
  `FCGX_ShutdownPending()`, then wake each request thread (`pthread_kill`
  with SIGUSR1) so a blocking FastCGI `accept` returns (with
  `FCGI_FAIL_ACCEPT_ON_INTR`). Threads finish the current request if any,
  leave the accept loop, main joins them and exits.
* After join, the listen fd is closed; if `-p` was a **Unix domain path**
  (not TCP `:<port>`), that path is **unlinked**.
* No in-process drain timer; the parent (systemd, Docker, afwdev, etc.)
  may still SIGKILL if needed.

## CLI

* `-p` path — Unix socket path or TCP `:port` (default `:9345`)
* `-f` configuration file (default `afw.conf`)
* `-n` request thread count
* `-e` extension, `-t` conf content type
* `--help`, `--version`

## Visual Studio Command Arguments (historical)

* -p = port (default is :9345)
* -f = configuration file (default is: afw.conf)
* In Visual Studio, right click and select properties
  * In properties, select Configuration Properties --> Debugging --> Command Arguments:

      -p :9345 -f "C:\Users\__username__\Documents\GitHub\afw.conf"