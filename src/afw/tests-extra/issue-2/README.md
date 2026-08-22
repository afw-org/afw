# issue-2 extra tests (opt-in)

Numbered leaves for umbrella [#2](https://github.com/afw-org/afw/issues/2).
Not in default `afwdev test -j`.

```bash
# all #2 extra leaves
afwdev test -T src/afw/tests-extra/issue-2 --show-all

# one leaf
afwdev test -T src/afw/tests-extra/issue-2/01-rss-hard-loops --show-all
afwdev test -T src/afw/tests-extra/issue-2/02-pool-eval-soak
```

| Leaf | What |
|------|------|
| **01-rss-hard-loops** | Hard-loop Adaptive Scripts; `/proc` RSS vs `pool_bytes_in_use`; gdb; `debug:pool` |
| **02-pool-eval-soak** | `afwfcgi` firehose of object/array/nested-eval (heap wrap + slot overwrite) |

Campaign map: [`designs/issue-2-lifetime.md`](../../../designs/issue-2-lifetime.md).
