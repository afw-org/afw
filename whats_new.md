# What’s new on `mgg-develop`

This note is for **AFW users** (script authors, model authors, operators, and people editing Adaptive object JSON). It covers changes on **`mgg-develop`** since it branched from **`develop`** (merge-base `f945f97c`, mid‑July 2026).

Internal agent rules, Cursor docs, and pure test-infrastructure work are omitted unless they affect runtime or tooling you use day to day.

---

## Highlights

| Area | What changed |
|------|----------------|
| **File streams** | Working `open_file` with hardened `rootFilePaths`; stream errors **throw** |
| **Model adapters** | `mappedAdapterId` is **optional** for pure-script models |
| **`afw` CLI** | Optional interactive line editing and command history |
| **JSON Schema** | Cleaner editor schemas for Adaptive object types |

---

## File streams (`open_file` and friends)

**Issue #103 / PR #120**

Adaptive Script file I/O is finished for beta: open, read, write, flush, and close through the stream category, with safe path resolution.

### Using `open_file`

Configure logical path prefixes on the application as `rootFilePaths` (type `_AdaptiveRootFilePaths_`). Each property name is a **prefix**; its value is the host directory.

```json
"rootFilePaths": {
  "data": "/var/afw/data",
  "includes": "/var/afw/includes"
}
```

Then in script:

```adaptive
const sn = open_file("out", "data/report.txt", "w");
write(sn, "hello");
flush(sn);
close(sn);

const sn2 = open_file("in", "data/report.txt", "r");
const s = read(sn2, 100);
close(sn2);
```

### Path rules

- A logical path must **equal** a prefix or begin with `prefix/` (no leading `/` on the logical path in the modern form: use `data/foo`, not `/data/foo`).
- Among matching prefixes, the **longest** wins (e.g. `includes` beats `include` for `includes/a.as`).
- After resolve, the host path must stay under that root; `..` and other escapes are rejected.
- The same resolution is used by `compile_from_file` and `eval_from_file`.

### Error handling (breaking change)

- `open_file` and `stream` **throw** on failure (invalid path, open failure, stream already open, unknown streamId). They no longer return `-1`.
- **`get_stream_error` was removed.** Use `try` / `catch` and the thrown `_AdaptiveError_` (`e.message`, and where applicable `e.rv` / `e.rvDecoded` for errno-based I/O failures).
- Unfinished APIs were removed from the public surface for a leaner beta: **`open_uri`**, **`open_response`**.

Modes cover text and binary (`r`, `w`, `a`, `r+`, … and `rb`, `wb`, …). `open_file` requires execute access.

---

## Pure-script model adapters

**Issue #109 / PR #119**

A model adapter still loads an `_AdaptiveModel_` from `modelLocationAdapterId` / `modelId`. What changed is the backend requirement:

- **Hybrid (unchanged):** set `mappedAdapterId` and map-and-forward (or fall through with `current::useDefaultProcessing` / `model_default_*`).
- **Pure-script (new):** implement every operation you use in model object-type `on*` scripts and **omit `mappedAdapterId`**. No dummy file/LMDB backend is required.

If an operation needs default processing and `mappedAdapterId` is missing, AFW fails with a clear error (implement the `on*` hook or set `mappedAdapterId`).

In scripts, `current::mappedAdapterId` is nullish when unset (it does not throw).

`modelId` and `modelLocationAdapterId` remain required for both hybrid and pure-script models.

---

## Interactive `afw`: line editing and history

**Issue #30 / PR #117**

When you run `afw` interactively on a real terminal (no file, no pipe, not `--local` chunk mode), builds linked with **libedit** offer:

- Emacs-style line editing
- Command history stored in **`~/.afw_history`** (when `HOME` is set)

Pipes, file input, and non-TTY use are unchanged. Builds without libedit still succeed; interactive history/editing simply is not available (the usage text says so).

To enable at build time on Debian/Ubuntu:

```bash
sudo apt-get install libedit-dev
```

Other distros: `libedit-dev` / `libedit-devel` as appropriate. See `src/afw/doc/building_on_linux.md` (and the macOS build notes).

---

## JSON Schema for Adaptive object types

**Issue #3 / PR #116**

Package-generated schemas under `generated/schemas/afw/` are improved for **editors** (e.g. VS Code completion, hovers, light validation) and for `afwdev validate`:

- No more invalid “`$ref` mixed with sibling keywords” shapes that break validators.
- Entity schemas promote type/properties to the document root for better editor apply behavior.
- Inheritance merges parent `propertyTypes` with **child overrides** so nested models validate correctly.
- `defaultValue` / possible values map more cleanly to JSON Schema `default` / `enum`.

If you edit Adaptive object JSON under `generate/objects/` (or rely on schema-backed validation), regenerate so your tree picks up the new schemas (`./afwdev build --cdev -j` or your usual generate path). Do not hand-edit `generated/schemas/`.

---

## Reliability notes

### Default values from `property_get` / `variable_get`

The clone-on-return fix for mutable defaults (issue **#110**) landed on `develop` before this branch. On `mgg-develop`, dedicated Adaptive Script regression tests lock that behavior in: a default object or array returned for a missing property/variable is isolated so later mutations do not poison other calls (important in long-running hosts and model `on*` handlers).

### Tests under `src/*/tests`

Tracked suites under `src/*/tests` are permanent regression assets. `afwdev test` uses temporary work directories; it does not mean those sources are disposable.

---

## Breaking / migration checklist

1. **Stream errors:** replace any `open_file` / `stream` checks for `-1` and any use of **`get_stream_error`** with `try` / `catch`.
2. **Removed functions:** stop calling **`open_uri`**, **`open_response`**, **`get_stream_error`** if you had experimental callers.
3. **`rootFilePaths`:** prefer logical paths like `my/file.txt` with a matching prefix property; rely on longest-prefix match and stay inside the host root.
4. **Model conf:** pure-script adapters may drop `mappedAdapterId`; hybrid adapters keep it. If you omit it, every used op must be implemented in `on*`.
5. **Schemas:** regenerate before depending on updated editor/validate behavior.

---

## Related issues and PRs

| Topic | Issue | PR |
|-------|-------|-----|
| File streams | #103 | #120 |
| Optional `mappedAdapterId` | #109 | #119 |
| Default-clone regressions | #110 | #118 (tests) |
| Interactive libedit | #30 | #117 |
| JSON Schema `$ref` / OT projection | #3 | #116 |

Branch tip at the time of this note includes documentation commit #121 as well.

---

## How this was produced

Diff basis: `git log develop..mgg-develop` and the corresponding code/metadata changes on `mgg-develop`. For full commit history, see those PRs on the repository hosting Adaptive Framework.
