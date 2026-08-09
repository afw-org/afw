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
| **Advanced-test leaf** | Multi-request / long-lived **`afwfcgi`** process with leaf conf. **Experimental** — see below. |
| **`afwdev blast`** | On-demand **random** suite firehose at afwfcgi for a period. **Not** part of `test -j`. See below. |

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
- After `//? source: ...`, the body runs until the next `//?` block or EOF.  
- The runner compares the evaluation result to **`expect`** (Adaptive value
  syntax). Failures are reported per `test:` case.  
- Study working files under `src/afw/tests/` (for example
  `src/afw/tests/language/script/try.as`).

Exact line grammar for test scripts lives in the compiler
(`afw_compile_parse_script.c` EBNF comments). Prefer copying a nearby test
over inventing a new metadata dialect.

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
| **`afw`** (default) | Subprocess **`afw`** for `.as`; hermetic **advanced-test** leaves (spawn `afwfcgi`). |
| **`valgrind`** | `.as` under valgrind; advanced leaves wrap **`afwfcgi`** when supported. |
| **`afwfcgi`** | Replays compatible `.as` against a **live** stack (default URL
  `http://localhost:8080/afw`). Skips tests with a private `afw.conf`.
  Advanced leaves are skipped (hermetic conf does not apply to the live process). |
| **`actions`** | Local Python `Session` + `eval_script` for `.as`. |

File type still wins for some paths: `.py` → Python runner;
`commands_*.txt` → commands runner.

## \*\*\* Experimental \*\*\* advanced-test leaves

**Status: experimental.** Marker names, schema, and runner details may change.
Many early choices (leaf discovery, hermetic `afwfcgi`, FCGI client) are
expected to stay in spirit. Feedback: GitHub issue **#157** and
`designs/afwdev-advanced-test.md`.

Use an advanced-test leaf when you need:

- One long-lived **`afwfcgi`** for several requests  
- Leaf-local conf / objects that must not depend on a pre-started stack  
- Multi-step process-lifetime checks (catalog, adapters, later stress for #2)

### Shape

```text
src/<srcdir>/tests/…/my_leaf/     # any depth under tests/
  advanced-test.yaml              # or advanced-test.json (not both)
  afw.conf                        # required for host afwfcgi
  objects/                        # optional
  step_something.as               # optional; referenced from steps
```

The directory with the marker is a **leaf**: one test unit; children are
assets only (not nested tests).

Minimal YAML:

```yaml
# Experimental — see designs/afwdev-advanced-test.md
host: afwfcgi
description: Short summary for listing / failures

afwfcgi:
  threads: 1

timeout_s: 60

steps:
  - name: first request
    eval: |
      return true;

  - name: script file
    script: step_two.as
```

- **`host`** — v1 supports **`afwfcgi`** only.  
- **`eval`** — inline Adaptive Script (one FCGI perform / request).  
- **`script`** — path relative to the leaf; keep a `test_script` shebang if you
  want per-case `expect` checking.  
- One leaf → one pass/fail (fail-fast on first bad step).  
- Requires **PyYAML** (see package `python-requirements.txt`) and **`afwfcgi`**
  on PATH from an install build.

Examples: `src/afw/tests/advanced/` (smoke, multi-request file adapter,
multi-eval lifetime, JSON marker sample).

## \*\*\* Experimental \*\*\* afwdev blast

**Not part of the normal gate.** On-demand load: randomly eval suite
`.as` sources against afwfcgi for a duration or request count.

```bash
# Typical docker/dev (defaults: url :8080/afw, 5m, concurrency=CPUs)
afwdev blast

# Longer / more parallel / adapter-focused
afwdev blast -d 30m -c 16 --test-pattern 'file_adapter/|model_adapter/|rql/'

# Harness-owned afwfcgi
afwdev blast -f /path/to/afw.conf -m 200
```

Same discovery filters as `test` (`--srcdir-pattern`, `--test-pattern`,
`--tags`). By default **skips fixture-heavy groups** (`Environment=` /
`afw.conf`) so failures usually mean a real problem; use
`--include-fixtures` to blast adapter/env tests too. Continues on Adaptive
failures; stops if the server dies. Design: `designs/afwdev-blast.md`.
afwfcgi SIGTERM/SIGINT graceful stop: issue **#158** (done; hermetic
check under `tests/advanced/afwfcgi_signal_shutdown/`).

## Practical tips

1. **Start from a neighbor** — copy a test in the same area and edit.  
2. **Name for `--test-pattern`** — patterns match file path / basename / leaf
   directory name (advanced markers).  
3. **Keep groups small** — shared conf is fine; do not hide unrelated suites
   in one directory if you want fine-grained parallel runs.  
4. **Default gate** — `afwdev test -j` after meaningful changes; valgrind mode
   is slower and optional for day-to-day.  
5. **Generated tests** — many function/datatype scripts under
   `tests/generated/` are generated; prefer hand tests under clear group
   names for new product behavior.

## Related

- @ref afw_dev_overview — builder docs map  
- @ref afw_dev_extending — extension sketch (includes `afwdev test -j`)  
- @ref afw_dev_compiler_ebnf — compiler EBNF harvest (not test authoring)  
- Maintainer design: `designs/afwdev-advanced-test.md` (experimental)  
- Issue [#157](https://github.com/afw-org/afw/issues/157)  
- CLI help: `afw --help` (`-s test_script`), `afwdev test --help`  
