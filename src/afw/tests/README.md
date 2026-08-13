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
  part of the default run

Longer tests, including firehose, live next door in
`src/afw/tests-extra/` and are only run when you pass
`afwdev test -T …`. The `orchestration.yaml` schema is
`src/afw/tests-extra/SCHEMA.md`.
