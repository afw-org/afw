Writing tests {#afw_dev_writing_tests}
=============

@brief How a developer adds tests that `afwdev test` will run.

## Audience

Package, extension, and core authors who need a **regression** for behavior
they care about. This is not a full Adaptive Script tutorial.

## Where tests live

Under each source directory:

```text
src/<srcdir>/tests/
  some_group/           # ordinary test group (one or more files)
    foo.as
    bar.as
    afw.conf            # optional; used when the group needs conf
    objects/            # optional fixtures
    config.py           # optional Tags, Environment, before/after hooks
  environments/         # optional shared fixtures (named; see config.py)
    models/
      afw.conf
      …
```

`afwdev test` walks `src/*/tests/` (filtered by `--srcdir-pattern` /
`--test-pattern`). A **group** is usually a directory of related files that
should not be split across parallel workers when they share conf or data.

## Day-to-day loop

```bash
# From package root — after code/generate changes
./afwdev build --cdev -j

# Run tests (installed afw / afwfcgi on PATH)
afwdev test -j
afwdev test --srcdir-pattern afw --test-pattern 'rql/.*'
afwdev test --list --test-pattern smoke
```

Use `./afwdev` when the build must refresh `afwdev` itself; use `afwdev` on
PATH afterward for `test`, `validate`, and similar.

## Choose a test kind

| Kind | When to use |
|------|-------------|
| **Adaptive test script** (`.as`, `--syntax test_script`) | Default. Language, functions, adapters under optional `afw.conf`. |
| **Python** (`.py` with `run()`) | Need host process control, raw env octets, bindings that are awkward in script. |
| **`commands_*.txt`** | Drive shell-ish command sequences (special runner). |
| **Orchestrated-test leaf** | Multi-request / long-lived **`afwfcgi`** with **`orchestration.yaml`**. See below. |

Prefer a **test_script** unless you need process lifetime, a private server, or
host-only APIs.

## Adaptive test scripts

### Skeleton

```adaptive
#!/usr/bin/env -S afw --syntax test_script
//? testScript: example.as
//? description: What this file covers
//? sourceType: script
//?
//? test: case-1
//? description: short case title
//? expect: true
//? source: ...

return true;

//?
//? test: case-2
//? description: another case
//? expect: 0
//? source: ...

// body …
return 0;
```

Notes:

- Shebang must mention **`afw`** and **`test_script`** (as above or
  `afw --syntax test_script`).  
- Metadata lines start with **`//?`**. Keys such as `test`, `description`,
  `expect`, `source`, `skip` are the common ones.  
- **Same-line values** (`//? expect: 0`, `//? description: short title`):
  leading and trailing **whitespace is trimmed**, so you do not need to
  fuss over spaces or tabs after the value.  
- **Multi-line values** use `//? key: ...` then the text on the following
  lines until the next `//?` (or end of file). That form keeps the text
  more literally (including newlines inside the block). A blank line before
  the next `//?` is the usual way to keep a final newline on the last
  content line.  
- **File values** use `//? key: <<< relative/path` (path relative to the
  directory of the `.as` file). The file contents become the key’s value
  **exactly** (no trim). Paths must be relative (no `..` segments). Useful
  for long expects or sources. A value that **starts with** `<<<` is always
  this form — do not begin free-text descriptions with those characters.  
- After `//? source: ...` or `//? source: <<< …`, the body is the multi-line
  or file form above.  
- **`expect:`** is Adaptive source for the **return value** you want (for
  example `0`, `true`, `"ok"`, `anyURI("…")`), not free text. The runner
  compiles and evaluates it, then compares to the case result. Use
  `error` or `error:…message…` when the case should fail. A `<<<` file for
  `expect` must contain that Adaptive source (for example `"hello"` with
  quotes if you want a string return value).  
- **`expect-stdout:`** / **`expect-stderr:`** (optional) assert the **text**
  written to the Adaptive `stdout` / `stderr` streams during the case
  (`print` / `println` / stream writes). Values are **literal strings**, not
  Adaptive source — same forms as other keys (same-line trim, `...`, `<<<`).
  Comparison is exact utf-8. Use a hyphen in the key (`expect-stdout`), never
  a colon inside the name (`expect:stdout` is wrong: `:` separates key from
  value). When present, the harness captures into an in-memory buffer so
  output does not pollute the `afw` process result channel. Omit the key to
  ignore that stream.  
- Failures are reported per `test:` case.

### Orchestrated leaves (`orchestration.yaml`)

Hermetic multi-request / firehose / REST work lives in leaves with
`orchestration.yaml` (see `src/afw/orchestrated-tests/README.md` and gate
examples under `src/afw/tests/advanced/`). Same vocabulary where it applies:
`expect`, `expect-stdout`, `expect-stderr`, plus `expectResponse` /
`expectStatus` for wire bodies.

**Recording `expectResponse` goldens:**

```bash
afwdev test --capture-goldens -T path/to/leaf
# or: AFWDEV_CAPTURE_GOLDENS=1 afwdev test -T path/to/leaf
```

Writes actual response bytes to each `expectResponse: <<< rel/path`, then
re-run without capture for the gate.  
- Study working files under `src/afw/tests/` (for example
  `src/afw/tests/language/script/try.as` and
  `src/afw/tests/compiler/test_script_file_value/`). Prefer copying a nearby
  test over inventing a new metadata dialect.

### Conf and fixtures

If the script needs adapters or other conf:

- Put **`afw.conf`** in the same directory (or use a shared
  `tests/environments/<name>/` and set `Environment` in `config.py`).  
- Default **`--env-mode afw`** runs `afw` with that conf when the group
  provides it.  
- Keep fixtures small and hermetic; `afwdev test` copies group files into a
  temp work directory.

### Optional `config.py`

A group may define:

- **`Tags`** — list of strings; `afwdev test --tags <regex>` filters groups.  
- **`Environment`** — name of a directory under `tests/environments/`.  
- **`before_all` / `after_all` / `before_each` / `after_each`** — hooks.

See existing `config.py` files under `src/afw/tests/` for patterns.

## How `afwdev test` runs things (`--env-mode`)

| Mode | Typical use |
|------|-------------|
| **`afw`** (default) | Subprocess **`afw`** for `.as`; hermetic **orchestrated-test** leaves (spawn `afwfcgi`). |
| **`valgrind`** | `.as` under valgrind; orchestrated leaves wrap **`afwfcgi`** when supported. |
| **`afwfcgi`** | Replays compatible `.as` against a **live** stack (default URL
  `http://localhost:8080/afw`). Skips tests with a private `afw.conf`.
  Orchestrated leaves are skipped (hermetic conf does not apply to the live process). |
| **`actions`** | Local Python `Session` + `eval_script` for `.as`. |

File type still wins for some paths: `.py` → Python runner;
`commands_*.txt` → commands runner.

## Orchestrated tests

Use an **orchestrated-test** leaf when you need a long-lived **`afwfcgi`**,
leaf-local conf, multi-request process checks, optional **REST** feeds,
**Accept** overrides (e.g. `application/x-afw`), or a **firehose** schedule
(replaces the retired **`afwdev blast`** subcommand).

Schema and extra scenario sketches: `src/afw/orchestrated-tests/` (also
`afwdev test -T src/afw/orchestrated-tests/...`). Gate examples:
`src/afw/tests/advanced/`.

### Shape

```text
src/<srcdir>/tests/…/my_leaf/
  orchestration.yaml          # or orchestration.json (not both)
  afw.conf
  objects/                    # optional fixtures
  tests/ or *.as              # payloads (sourcePath / <<<)
```

```yaml
version: 1
host: afwfcgi
description: Short summary
afwfcgi:
  threads: 1
timeout_s: 60
feed:                         # defaults for every test
  kind: action
  accept: application/json
tests:
  - name: first
    sourceType: script
    source: |
      return true;
    expect: true
  - name: progressive
    feed:
      accept: application/x-afw   # override default Accept only
    sourceType: script
    sourcePath: tests/to_response.as
# optional schedule: sequential | parallel | firehose | repeat
```

- **`feed`** at document level = defaults; per-test **`feed`** overrides fields.  
- **`sourceType` + `source` / `sourcePath` / `<<< path`** — same idea as test_script evaluate.  
- **`expect`** — optional JSON-ish compare of action result; test_script payloads use inner `passed`.  
- **`schedule.firehose`** — duration / concurrency / maxRequests / fromTests (load).  
- Requires **PyYAML** and **`afwfcgi`** on PATH from an install build.

## Practical tips

1. **Start from a neighbor** — copy a test in the same area and edit.  
2. **Name for `--test-pattern`** — patterns match file path / basename / leaf
   directory name (orchestration markers).  
3. **Keep groups small** — shared conf is fine; do not hide unrelated suites
   in one directory if you want fine-grained parallel runs.  
4. **Default gate** — `afwdev test -j` after meaningful changes; valgrind mode
   is slower and optional for day-to-day.  
5. **Generated tests** — many function/datatype scripts under
   `tests/generated/` are generated; prefer hand tests under clear group
   names for new product behavior.  
6. **Load / soak** — orchestrated leaves with firehose under
   `src/afw/orchestrated-tests/` or `-T` roots; do not put heavy firehose in
   the default `tests/` gate.

## Related

- @ref afw_dev_overview — builder docs map  
- @ref afw_dev_extending — extension sketch (includes `afwdev test -j`)  
- @ref afw_dev_compiler_ebnf — compiler EBNF harvest (not test authoring)  
- Maintainer: `src/afw/orchestrated-tests/SCHEMA.md`  
- Issue [#157](https://github.com/afw-org/afw/issues/157)  
- CLI help: `afw --help` (`-s test_script`), `afwdev test --help`  
