# `src/afw/tests`

This is what `afwdev test -j` runs for core. The runner looks under each
source directory's `tests/` folder (`src/*/tests/`).

Add a test by copying an existing one and changing it. The handbook
Developer Guide **Writing Tests** (Guides → Developer → Writing Tests)
describes the `//?` syntax and expect keys. The source for that page is
`src/afw/doc/guide/developer/writing-tests.xml`.

A few places that are easy to misunderstand:

- `generated/` — generated function and datatype scripts; do not edit them
- `test262/` — language tests derived from TC39; see that folder's README
- `environments/` — shared fixtures named from a group's `config.py`
- `advanced/` — short orchestrated tests (`orchestration.yaml`) that are
  part of the default run. A few groups are a thin Python `run()` plus
  a checked-in `*_probe.c` (pool wrap, C-array view index, UTF-8 ICU
  bound). Use that when Adaptive Script cannot reach the hole. Call
  `run_c_probe()` from `_afwdev.test.c_probe` — do not copy a `cc`
  line. The `.c` is not part of the cmake library build.
  `afwdev test --env-mode valgrind` wraps `afw` **and** those probe
  binaries (suite suppressions cover libunwind noise in
  `afw_os_backtrace` on a throw). Standalone valgrind without that
  file can still report that noise; that is not the hole. Handbook
  **Writing Tests** has the recipe.

Longer tests, including firehose, live next door in
`src/afw/tests-extra/` and are only run when you pass
`afwdev test -T …`. The `orchestration.yaml` schema is
`src/afw/tests-extra/SCHEMA.md`.
