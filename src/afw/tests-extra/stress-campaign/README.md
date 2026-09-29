# stress-campaign

Long-running **afwfcgi** beat-down loop for maintainers. Not in `afwdev test -j`.

Runs mixed `tests-extra` leaves (night practice, restart stress, issue-2 RSS
lab, named firehoses), captures failures under `/tmp/afw-stress-campaign/`,
and appends a JSON-lines record for each cycle.

```bash
# From package root — foreground (Ctrl-C stops after the current leaf)
src/afw/tests-extra/stress-campaign/run-cycle.sh

# Background (~9h typical)
nohup src/afw/tests-extra/stress-campaign/run-cycle.sh \
    >> /tmp/afw-stress-campaign/master.log 2>&1 &
echo $! > /tmp/afw-stress-campaign/run-cycle.pid
```

Environment:

| Variable | Default | Meaning |
|----------|---------|---------|
| `AFW_STRESS_PRACTICE_S` | `600` | `night/practice.sh` duration argument |
| `AFW_STRESS_FIREHOSE_S` | `300` | Patched `duration_s` for 07/07b leaves |
| `AFW_STRESS_ISSUE2_S` | `120` | `AFW_ISSUE2_DURATION_S` for RSS hard loops |
| `AFW_STRESS_LOG_ROOT` | `/tmp/afw-stress-campaign` | Artifact root |
| `AFW_STRESS_SLEEP_S` | `15` | Pause between cycles |
| `AFW_STRESS_PARALLEL` | `1` | Run practice + stress leaves concurrently |
| `AFW_STRESS_SERVER_THREADS` | `50%` | `afwfcgi.threads` on staged leaves |
| `AFW_STRESS_CLIENT_CPUS` | `50%` | Firehose `clientProcesses` |
| `AFW_STRESS_FIREHOSE_CONCURRENCY` | `100%` | Firehose `concurrency` |
| `AFW_STRESS_RESTART_THREADS` | `8` | `night/restart` only (practice staging) |

On failure, see `cycles/<id>/` for logs, `diag/` copies, and `record.jsonl`.
Use `report-failure.sh <cycle-dir>` to print a GitHub-issue-shaped summary.
