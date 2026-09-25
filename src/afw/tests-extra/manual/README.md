# manual

The same server as `firehose`, with no load. Open the printed URL and use the admin app. A heartbeat line every 30 seconds shows request count, concurrent requests, threads, pool bytes, and RSS.

```bash
afwdev test -T src/afw/tests-extra/manual
```

Stop with Ctrl-C or `POST /stop` on that port. A second Ctrl-C stops immediately. Port 8080 stays on `/var/run/afw.sock`.

Conf, objects, and the private `slapd` setup are the `firehose` leaf. This directory only holds the schedule.
