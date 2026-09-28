# Night

Two opt-in `afwfcgi` processes. Run them together when the question is
whether a long server stays flat, and whether restarts crash or count
expected overlap as failure. They are not in `afwdev test -j`.

From the package root:

```bash
src/afw/tests-extra/night/practice.sh          # both leaves, 90s
src/afw/tests-extra/night/practice.sh 600      # both leaves, 10 minutes
src/afw/tests-extra/night/overnight.sh         # both leaves, 7 hours
```

`overnight.sh` is `practice.sh 25200`. Do not start it until that run
is the plan. A number argument copies each leaf, sets `duration_s` to
that many seconds, and sets `timeout_s` to `duration_s` plus 150.
`timeout_s` has to stay above the firehose or the harness stops the
leaf early.

The script starts both `afwdev test` processes at once. Each gets its
own `--tmpdir`. The default temp directory is `/tmp`, and every
`afwdev test` deletes `$tmpdir/afwdev_test_output`. Two runs on `/tmp`
delete each other's server.

| Leaf | `--tmpdir` | Server |
|------|------------|--------|
| `slope` | `/tmp/afw-night-slope` | Half the CPUs. No `service_restart`. |
| `restart` | `/tmp/afw-night-restart` | 4 threads. Restarts file, model, VFS, a log, and a handler. |

Logs:

| File | What |
|------|------|
| `/tmp/afw-night-slope/practice.log` | Slope harness summary |
| `/tmp/afw-night-restart/practice.log` | Restart harness summary |
| `/tmp/afw-night-slope-metrics/metrics.tsv` | `process::` and server samples from slope |
| `/tmp/afw-night-slope-metrics/rss-first.txt` | First `process::rss` (bytes) |
| `/tmp/afw-night-slope-metrics/rss-last.txt` | Last `process::rss` (bytes) |

Slope's `config.py` clears those three metrics files at the start of
the leaf. `rss-check` fails the leaf if RSS grew 64 MiB or more from
the first sample. The script's exit status is non-zero if either leaf
exits non-zero. A pass is exit 0, `fail=0` on both firehose lines, and
the RSS check still inside the leaf.

One leaf by itself:

```bash
mkdir -p /tmp/afw-night-slope /tmp/afw-night-slope-metrics
afwdev test --tmpdir /tmp/afw-night-slope \
    -T src/afw/tests-extra/night/slope

mkdir -p /tmp/afw-night-restart
afwdev test --tmpdir /tmp/afw-night-restart \
    -T src/afw/tests-extra/night/restart
```

The parent directory passed to `--tmpdir` has to exist. `practice.sh`
creates the two it uses.

## What each leaf is for

`slope` is the leak watch. The pool is the busy firehose (file, model,
LMDB use, VFS, an LDAP read, catalog, HTTP) plus the #379 shapes
(cheap loop, catalog touch, adapter metrics) and a metrics sample.
`maxFail` is 0. Do not put `service_restart` in this pool. A model-swap
firehose has climbed about a gigabyte a minute; that is why restarts
stay in the other process. A flat night here is the long server we
want. It does not by itself close #379. That issue names
`07-firehose-blast-style` and `07b-firehose-catalog-pool` as the same
workload run longer. Those scripts are in this pool. The leaf is still
a different mix.

`restart` holds a snapshot of the authorization handler and the log
across three restarts, then the firehose restarts file, model, VFS, the
log, and reads them. Overlap the service layer already reports returns
success: `cannot be restarted`, `can not be stopped`, `can not be
started`, `is not running`, `is not available`. A read that finds its
adapter stopped starts it. When another start or stop is in flight,
that throws `can not be started`, and the read counts it as down.
Since #413, a read whose start lost the race to another start uses
the running adapter. It no longer throws `can not be started.  Service
is running` (#411). The other statuses (`starting`, `finishing active
work`, `restarting`) still throw and still count as down.
Anything else fails the request. `maxFail` is 0, and the leaf still fails if `afwfcgi` exits.
LMDB is not in this leaf. LDAP is not in this leaf. `lmdb-optin` stays
out.

## Threads

`slope` is `threads: "50%"`. On the 2026-09-27 practice that was 16
threads on a 32-CPU machine.

`restart` is `threads: 4`. With one server thread, firehose
concurrency is 100% of that thread, so it drops to 1. Restarts and
reads then never overlap, and the leaf cannot find a timing crash.

On the 2026-09-27 practice, develop `d79b6365` segfaulted at 2 threads
and more. A start published the new service before it held a
reference, so a stop or restart on another thread could free it
first. #389 (`263c4426`) holds the reference before publishing. Its
parent still segfaulted at 16 threads. #389 did not. See #403.

On develop `2d6098cb`, with the reads counting `can not be started` as
down, each 90s:

| Restart threads | Result |
|-----------------|--------|
| 2 | 230,475 requests, 0 failures. Passed. |
| 4 | 235,903 requests, 0 failures. Passed. |
| 16 | 193,996 requests, 0 failures. Passed. |

On develop `c27794c4` (#413), `restart` at 4 threads passed with
222,796 requests and 0 failures. Probe copies of `stress-model-restart`
(16 threads) and `stress-file-restart` (96 threads) whose reads fail on
`Service is running` went from 53 and 172 failures before #413 to 0.

`afwfcgi.stderr.log` ending at `Service 'log-standard' starting.` is
not a crash. After that log starts, the server logs to stdout.

`slope` beside a one-thread restart, same 90s, on `d79b6365`:

| | |
|--|--|
| Requests | 222,575, 0 failures, about 2,469/s |
| Threads | 16, max concurrent 16/16 |
| `process::rss` | 31.3 MiB to 40.0 MiB (up 8.8 MiB) |

An earlier slope-only 90s was the same shape: 226,150 requests, 0
failures, RSS up 8.3 MiB. Both were under the 64 MiB cap.

## Needs

`slope` starts a private `slapd` (`config.py`, same as `firehose`).
The `slapd` package has to be installed. The HTTP front maps
`build/js/apps`, `build/docs`, and `src/afw/tests-extra`. Those
directories have to exist. LDAP in this leaf is a read of `cn=Ada`.
The add is `src/afw/tests-extra/ldap-add`.

If `afwfcgi` dies, the harness stops the clients. The server's stderr
is `afwfcgi.stderr.log` and its stdout (where log type standard
writes) is `afwfcgi.stdout.log`, both in that leaf's work directory.
afwdev line-buffers those streams when `stdbuf` is on PATH. The files
stay until the next `afwdev test` for that temp directory. A firehose
that saw request errors also leaves `diag/firehose-errors.txt` there.
The console shows a short sample. When any request error was counted,
including a run that stayed under its threshold and passed, the
summary ends with a Detail line naming the temp directory. A clean
pass does not.

## Focused leaves, not this pair

`stress-model-restart` and `stress-file-restart` use the same overlap
returns and `maxFail: 0`. Their reads also count `can not be started`
as down.
`stress-file-restart-only` still expects a read to succeed across a
restart that swaps the instance in place. `stress-type-restarts` and
`handler-log-properties` are the short sequential checks.
`service-restart-conf` checks that a restart whose new conf can not be
used throws and leaves the service running (#413).
`lmdb-optin` is Jeremy's crash and is not part of this set.
