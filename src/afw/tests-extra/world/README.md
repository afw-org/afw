# world

One `afwfcgi` that stands in for a busy server. Add adapters here over time.

Checked-in run: two minutes, half the CPUs. The generator script writes 24 people through the file adapter and 24 notes through the model, then the firehose reads the seeds and that batch and adds and deletes its own notes and people so the tree does not grow.

```bash
afwdev test -T src/afw/tests-extra/world
```

For a longer run, raise `duration_s` and `timeout_s` in `orchestration.yaml`. `timeout_s` must stay above `duration_s`.

Now: file adapter `data`, model `model` mapped onto it, lmdb adapter `lmdb`, vfs adapter `vfs`, ldap adapter `ldap`, and the built-in `afw` catalog. `curl` still waits for a peer. Do not put `adapter-model` stop/restart in this firehose until #382 is fixed.

LDAP is a private `slapd` on `127.0.0.1` and a free port. `config.py` starts it in `before_all` and stops it in `after_all`. The database is a fresh directory in the test work directory, loaded from `ldap/seed.ldif`. The leaf needs the `slapd` package. It does not use a directory on port 389. The firehose reads `cn=Ada`. An add of `inetOrgPerson` is rejected by slapd because the adapter does not send the required `sn` attribute.
