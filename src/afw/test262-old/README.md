# test262 snapshot (temporary)

Copy of `src/afw/tests/test262/` **immediately before** the #62
`expect: undefined` sweep (`181b82be` on `issue-#62-script-language`).

**Not run** by `afwdev test` (discovery is `src/<srcdir>/tests/` only).
For Jeremy (and anyone else) to diff against the current converted suite
without digging through git. Delete this directory when that comparison
is no longer needed.

The sweep appended `return;` on cases whose converter default was
`expect: undefined` (original test262 only cares that the case does not
throw). Live suite: `src/afw/tests/test262/`.
