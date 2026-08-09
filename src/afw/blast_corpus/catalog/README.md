# blast_corpus (not part of `afwdev test -j`)

Private Adaptive scripts for **`afwdev blast --tests-path`** only.
Not under `tests/`, so normal suite discovery ignores them.

```bash
# Attach to docker/dev nginx+afwfcgi (default URL)
afwdev blast -T src/afw/blast_corpus/catalog -d 15s -c 4 -m 50

# Or managed conf
# afwdev blast -f /path/to/afw.conf -T src/afw/blast_corpus/catalog -m 100
```

See `designs/afwdev-blast.md` and #149 catalog work.
