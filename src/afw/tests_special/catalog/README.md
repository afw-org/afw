# tests_special/catalog (not part of `afwdev test -j`)

Private Adaptive scripts for **`afwdev blast --tests-path`** only.
Lives next to `tests/` but **not** under it, so normal suite discovery ignores them.

```bash
# Attach to docker/dev nginx+afwfcgi (default URL)
afwdev blast -T src/afw/tests_special/catalog -d 15s -c 4 -m 50

# Or managed conf
# afwdev blast -f /path/to/afw.conf -T src/afw/tests_special/catalog -m 100
```

See `designs/afwdev-blast.md` and #149 catalog work.
