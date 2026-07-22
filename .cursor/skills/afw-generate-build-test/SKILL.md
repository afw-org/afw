---
name: afw-generate-build-test
description: >-
  Runs the AFW C/Python regenerate, build, install, and test loop after metadata
  or interface changes. Use when the user asks to generate, rebuild with --cdev
  or --fulldev, verify bindings, run afwdev test/validate (including --env-mode
  valgrind), or check that generated/ was not hand-edited.
---

# Generate → build → test

## When to use

- Changed anything under `src/*/generate/` or hand C / afwdev Python
- Need fresh bindings before compile/test
- Verifying a C or afwdev change end-to-end

## Checklist

```
Progress:
- [ ] 1. Confirm edits are under generate/ or hand sources (not generated/)
- [ ] 2. Optional: validate metadata if JSON/XML changed
- [ ] 3. Day-to-day edit loop: ./afwdev build --cdev -j
- [ ] 4. Day-to-day: afwdev test -j (or targeted --test-pattern)
- [ ] 5. Before commit/push (docs, multi-area, finish pass):
         ./afwdev build --fulldev -j
- [ ] 6. Full verify before a PR (maintainer default; also when user asks for
         full build/test):
         ./afwdev build --fulldev -j
         afwdev test -j --env-mode valgrind
- [ ] 7. Confirm success from command output
```

## Preferred commands

From package root:

```bash
# Optional: validate specific objects
afwdev validate --pattern 'src/afw/generate/objects/_AdaptiveFunctionGenerate_/*.json'

# C/Python day-to-day: generate + cmake + install core/extensions/afwdev
# Does NOT build JS app or docs. Use ./afwdev because this may refresh afwdev itself.
./afwdev build --cdev -j

# Script tests (installed afwdev after --cdev)
afwdev test -j
afwdev test --srcdir-pattern afw --test-pattern 'rql/.*'
afwdev test --srcdir-pattern afw --tags rql
afwdev test --srcdir-pattern afw --list
afwdev test --srcdir-pattern afw --bail 1

# Full package dev install (docs, multi-area, finish pass, before PR):
#   --fulldev = --all --generate --clean --install --scan
#   --all alone does NOT generate or install
./afwdev build --fulldev -j

# Full verify before a PR (maintainer default):
#   then all tests under valgrind (much slower — not every edit cycle)
./afwdev build --fulldev -j
afwdev test -j --env-mode valgrind
```

`--srcdir-pattern` uses fnmatch against srcdir names from `afw-package.json` (e.g. `afw`, `afw_lmdb`, `*`).

**`./afwdev` vs `afwdev`:** use `./afwdev` for builds that install/refresh the `afwdev` command; use `afwdev` from PATH afterward for `test` / `validate`.

## Do not edit generated/

If a fix seems to require changing `src/*/generated/**`:

1. Stop — find the corresponding input under `generate/` or the generator under `src/afw_dev/_afwdev/generate/`.
2. Fix the source of truth, then `./afwdev build --cdev -j`.
3. Committed `generated/` may exist for grep/review; it is not the edit surface.

## Related

- Skill `add-adaptive-function` for new function/data-type workflows
- `@AGENTS.md` for layout and hand-edit vs generated table
