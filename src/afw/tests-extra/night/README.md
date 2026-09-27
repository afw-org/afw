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
| `restart` | `/tmp/afw-night-restart` | 1 thread. Restarts file, model, VFS, a log, and a handler. |

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
started`, `is not running`, `is not available`. Anything else fails the
request. `maxFail` is 0, and the leaf still fails if `afwfcgi` exits.
LMDB is not in this leaf. LDAP is not in this leaf. `lmdb-optin` stays
out.

## Threads

`slope` is `threads: "50%"`. On the 2026-09-27 practice that was 16
threads on a 32-CPU machine.

`restart` is `threads: 1`. Do not raise it for an overnight until the
crash below is understood.

On that practice, develop `d79b6365`:

| Restart threads | Result |
|-----------------|--------|
| 1, beside slope | 90.0s, 123,652 requests, 0 failures. Passed. |
| 1, alone | 90.0s, 150,058 requests, 0 failures. Passed. |
| 2 | Segfault during the run. |
| 4 and 16 | Segfault in `libafw` once the harness was connected. The server log stopped flushing around `log-standard` starting. |

A plain `afwfcgi -n 16` with no client stayed up through startup.
`stress-model-restart` has stayed up at 16 threads. The crash is this
combined leaf with two or more server threads. With one server thread,
firehose concurrency is 100% of that thread, so it drops to 1. The
green run restarts and reads, and it does not overlap them. That is
the open question before an overnight of `restart`.

`slope` beside that one-thread restart, same 90s:

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
An add of `inetOrgPerson` is a different bug and is not in the pool.

If `afwfcgi` dies, in-flight clients can sit until the leaf timeout.
The summary is then late. The server log is
`afwfcgi.stderr.log` under that leaf's work directory inside the
tmpdir. Stderr is block-buffered when it is not a terminal, so a crash
can hide the last service line.

## Focused leaves, not this pair

`stress-model-restart` and `stress-file-restart` use the same overlap
returns and `maxFail: 0`. They were not re-run after that edit.
`stress-file-restart-only` still expects a read to succeed across a
restart that swaps the instance in place. `stress-type-restarts` and
`handler-log-properties` are the short sequential checks.
`lmdb-optin` is Jeremy's crash and is not part of this set.
