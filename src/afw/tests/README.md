# `src/afw/tests` — default gate

This tree is what `afwdev test -j` runs for core. The runner discovers tests
under each source directory's `tests/` folder (`src/*/tests/`).

Add a test by copying a neighbor and editing it. The handbook Developer
Guide **Writing Tests** has the `//?` syntax and expect keys
(`src/afw/doc/guide/developer/writing-tests.xml`).

A few landmarks — not a catalog of every group:

- `generated/` — generated function and datatype scripts; do not hand-edit
- `test262/` — language suite derived from TC39; see that folder's README
- `environments/` — shared fixtures named from a group's `config.py`
- `advanced/` — short orchestrated gate leaves (`orchestration.yaml`)

Longer soak, progressive, and firehose work lives next door in
`src/afw/tests-extra/` and is opt-in (`afwdev test -T …`). The
`orchestration.yaml` schema is `src/afw/tests-extra/SCHEMA.md`.
