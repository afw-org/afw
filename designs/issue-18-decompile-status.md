# Issue #18 — decompile / recompile / PragmaValues (status)

**Branch:** `issue-#18` (tracks `origin/issue-#18`)  
**Tip (as of 2026-07-31):** `3a7ad36d` — *fix: polish compiler listing for Fiddle* (includes prior stringify pure JSON + replacer, decompile fidelity, crypto TRY/FINALLY ride-alongs).  
**User-facing notes:** `whats_new.md` — section *stringify, decompile, compiler listing, and binary text*.  
**Verify:** decompile/pragma/listing/stringify tests green under `--cdev`; fulldev+valgrind was green at fidelity tip `71bcfc70` (re-run before PR if desired).

## Goal (locked)

- **IR / behavior fidelity**, not original source listing recovery.
- Primary check: `d1 = decompile(compile(src)); d2 = decompile(compile(d1)); assert(d1 == d2)`.
- Eval of recompiled IR must match original where we care (try/throw, switch default, etc.).
- Decompile prints functional Adaptive-ish / `#implementation_id(...)` form; pragmas re-parse the same IR.

## Print / inspect paths (do not conflate)

| Path | Role | Docs |
|------|------|------|
| **stringify** | Pure JSON of **evaluated** data (+ optional replacer) | `whats_new`, `stringify.json` |
| **decompile** | Adaptive **IR text** (recompilable when forms are supported) | `whats_new`, `decompile.json` |
| **compile listing** | Human tree + `---Symbols` (Fiddle); not recompilable | `whats_new`, `compile` polymorphic listing param |
| **decode_to_string** / **string(binary)** | UTF-8 octets vs base64 printable | `whats_new`, crypto README |

**Pragma / `#…` recompile** is advanced; this file is the maintainer home. User summary is in `whats_new` (short). Full handbook pages still TBD.

## Out of scope on this branch

| Item | Where parked |
|------|----------------|
| Compile-time **optimize** / constant fold pure calls | `designs/compile-optimize-notes.md` |
| Function **parameter-list destructure sugar** | same file, “Related #18 follow-ups” |
| Fake recompile of **runtime** `#closure_binding` | known **reject** with clear error |
| Recompile of **C-side** `#function_thunk` | known **reject** with clear error |

## Landed (major)

PragmaValues / decompile shapes (non-exhaustive):

- `#block`, unwrap empty outer block
- `#assignment_target("const"|"let"|…, Pattern)` — symbol **or** list/object Pattern
- `#list_expression`, `#script_function`, `#template_definition`
- `#statements(...)` for IR-node arrays
- `#switch_default` for switch default arm identity
- Types on symbols from **compiler-owned** storage only (`symbol->type`)
- Known rejects for `#closure_binding` / `#function_thunk`
- **stringify** pure JSON + replacer (function or property-name array)
- **listing** polish: object_expression without evaluate; `switch_default` label; `...rest` on params

## Tests (compiler)

| File | Role |
|------|------|
| `src/afw/tests/compiler/decompile.as` | decompile surface / shapes |
| `src/afw/tests/compiler/pragma.as` | pragma parse / round-trip |
| `src/afw/tests/compiler/decompile_fidelity.as` | `d1 == d2` + eval fidelity |
| `src/afw/tests/compiler/stringify.as` | pure JSON + replacer |
| `src/afw/tests/compiler/listing.as` | Fiddle listing markers / regressions |

Narrow: `afwdev test -j --srcdir-pattern afw --test-pattern 'compiler/(decompile|pragma|stringify|listing).*'`

## Core code touch points

| Area | Paths (hand-edit) |
|------|-------------------|
| Decompile | `src/afw/value/afw_value_decompile.c` (+ per-kind decompile) |
| Pragma parse | `src/afw/compile/afw_compile_parse_pragma.c` |
| Listing | `src/afw/value/afw_value_compiler_listing.c`, `object_expression`, `script_function` |
| Stringify | `src/afw/function/afw_function_compiler.c`, `json/afw_json_from_value.c` |
| Assignment target / Pattern | `src/afw/compile/afw_compile_parse_assignment_target.c` |

Do not hand-edit `generated/`.

## Footguns (do not re-break)

1. **`symbol->type`** — not the wrong assignment-target union arm for types on decompile.
2. **`#closure_binding` / `#function_thunk`** — known compile rejects.
3. **Switch default** — `#switch_default` (decompile) / `switch_default` (listing label).
4. **IR in lists** — `#statements(...)` when elements are IR nodes.
5. **Listing must not evaluate** object_expression properties (free vars).
6. **stringify ≠ decompile** — JSON of evaluated data vs IR text.

## Suggested next moves

1. **PR** `issue-#18` → mgg-develop / develop (after policy + fulldev/valgrind if multi-area PR).
2. **Handbook** pages for the three print paths + optional advanced pragma note (see chat).
3. More fidelity coverage if gaps appear.
4. Later branches: optimize, param destructure sugar, type-check prep.
