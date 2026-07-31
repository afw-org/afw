# Issue #18 — decompile / recompile / PragmaValues (status)

**Branch:** `issue-#18` (tracks `origin/issue-#18`)  
**Tip (as of 2026-07-31):** `71bcfc70` — *issue #18: decompile recompile value-tree fidelity*  
**Working tree:** clean; pushed.  
**Verify:** `./afwdev build --fulldev` OK; `afwdev test -j --env-mode valgrind` OK (20/20 dirs, ~3290 passed).

## Goal (locked)

- **IR / behavior fidelity**, not original source listing recovery.
- Primary check: `d1 = decompile(compile(src)); d2 = decompile(compile(d1)); assert(d1 == d2)`.
- Eval of recompiled IR must match original where we care (try/throw, switch default, etc.).
- Decompile prints functional Adaptive-ish / `#implementation_id(...)` form; pragmas re-parse the same IR.

## Out of scope on this branch

| Item | Where parked |
|------|----------------|
| Compile-time **optimize** / constant fold pure calls | `designs/compile-optimize-notes.md` |
| Function **parameter-list destructure sugar** (ES-style) | same file, “Related #18 follow-ups” |
| Fake recompile of **runtime** `#closure_binding` (live enclosing scope) | known **reject** with clear error |
| Recompile of **C-side** `#function_thunk` | known **reject** with clear error |

## Landed (major)

PragmaValues / decompile shapes (non-exhaustive):

- `#block`, unwrap empty outer block
- `#assignment_target("const"|"let"|…, Pattern)` — symbol **or** list/object Pattern (holes, rest, rename, defaults, nesting)
- `#list_expression`, `#script_function`, `#template_definition`
- `#statements(...)` for IR-node arrays (not plain data list)
- `#switch_default` so default arm survives unique-null / recompile
- Types on symbols from **compiler-owned** storage only (`symbol->type`); do **not** read the wrong union arm of assignment-target / `variable_type`
- Nested patterns decompile as nested `[`/`{` only (no nested `#assignment_target`)
- Pattern whitespace / type annotation decompile cleaned up
- try/catch: `e` binding + string throw message story aligned for fidelity
- rest params, multi-param `script_function`, `const(f)` without spurious target wrapper

## Tests (compiler)

| File | Role |
|------|------|
| `src/afw/tests/compiler/decompile.as` | decompile surface / shapes |
| `src/afw/tests/compiler/pragma.as` | pragma parse / round-trip |
| `src/afw/tests/compiler/decompile_fidelity.as` | `d1 == d2` + eval fidelity |

Narrow: `afwdev test -j --srcdir-pattern afw --test-pattern 'compiler/(decompile|pragma).*'`

## Core code touch points

| Area | Paths (hand-edit) |
|------|-------------------|
| Decompile | `src/afw/value/afw_value_decompile.c` (+ per-kind decompile in value impls as needed) |
| Pragma parse | `src/afw/compile/afw_compile_parse_pragma.c` |
| Assignment target / Pattern | `src/afw/compile/afw_compile_parse_assignment_target.c` |
| script_function / call / block | related `value/` and compile paths |
| Array IR write | `src/afw/data_type/afw_data_type.c` (`#statements`) |
| Crypto valgrind (rode along) | `afw_pool_calloc` working buffers; suppressions for AES-GCM false positives |

Do not hand-edit `generated/`.

## Footguns (do not re-break)

1. **`symbol_reference` / assignment_target union:** type lives on `symbol->type` after `variable_reference_create(..., type)`. The union arm that holds `variable_type` is **not** valid storage for decompile type print — garbage if misread.
2. **`#closure_binding` / `#function_thunk`:** always **compile-time reject** with intentional messages; not “unknown pragma”. Display-only decompile of runtime closures is OK; do not invent a scope-less recompile.
3. **Switch default:** unique-null default needs `#switch_default` (or equivalent marker) so recompile keeps the arm.
4. **IR in lists:** data-type array decompile uses `#statements(...)` when elements are IR nodes; plain `array()` is wrong for that case.
5. **Crypto buffers:** use `afw_pool_calloc` (not bare alloc without clear) for OpenSSL AES-GCM working memory; valgrind suppressions exist for known OpenSSL noise — prefer real clear when we own the buffer.
6. **Scan / signature:** null-check `script->signature` before parameter-block scan paths.

## Suggested next moves (when resuming)

Pick with user; none started:

1. **PR** `issue-#18` → develop/mgg-develop (after branch policy check).
2. More **fidelity coverage** (templates, qualifiers, more control flow) if gaps appear in real use.
3. **Type-check** / typed-script prep that depends on stable type-on-symbol decompile (related but not the same as optimize).
4. Only later: separate branch for **optimize** (see notes file) and/or **param destructure sugar**.

## Related chat decisions

- User: autonomous progress on #18; **not** optimize on this branch.
- User agreed: destructure Patterns in `#assignment_target` now; param-list sugar later.
- Fidelity = same value-tree / behavior IR via decompile string equality, not pretty-print source.
- fulldev + valgrind requested and passed before push.
