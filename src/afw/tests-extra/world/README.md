# world

One `afwfcgi` that stands in for a busy server. Add adapters here over time.

Checked-in run: two minutes, half the CPUs. The generator script writes 24 people through the file adapter and 24 notes through the model, then the firehose reads the seeds and that batch and adds and deletes its own notes and people so the tree does not grow.

```bash
afwdev test -T src/afw/tests-extra/world
```

For a longer run, raise `duration_s` and `timeout_s` in `orchestration.yaml`. `timeout_s` must stay above `duration_s`.

Now: file adapter `data`, model `model` mapped onto it, and the built-in `afw` catalog. `curl` and `ldap` wait for a peer. lmdb and vfs are next. Do not put `adapter-model` stop/restart in this firehose until #382 is fixed.
