# Pragma and `#` design (Pattern B)

**Status:** Active on `issue-#28-pragma-cleanup` (and follow-ons).  
**Code home:** `src/afw/compile/afw_compile_parse_pragma.c` (file header mirrors this).  
**Related:** compiler-internal `#` forms — `afw_compile_parse_compiler_internal.c`; flags — `afw_flag.c` / `generate/strings/strings.txt`.

## Two surfaces that share the `#Name` token

| Kind | Parse | Audience | Role |
|------|--------|----------|------|
| **Pragma** | `afw_compile_parse_pragma.c` | Script authors | Per-compile **policy** |
| **Compiler-internal** | `afw_compile_parse_compiler_internal.c` | Toolchain only | **decompile → recompile** (`#block`, `#script_function`, …) |

Lex: `pound_identifier` for any `#Name`. Product docs should not teach compiler-internal forms as normal script.

## Pattern B — author pragmas

The only planned author pragma family is **compile policy**:

```text
#compile <operand>… ;
```

| Layer | Role |
|-------|------|
| **Flags** (`compile:…`, env / `flag_set` / host) | **Defaults** |
| **`#compile`** | **Override for this compile unit** |

Rules:

1. Operands are **short names** of `compile:*` flags (the part after `compile:`), same spelling as the registry — plus special **`noTypeCheck`** (clears type-check cluster; does not clear `noOptimize`).
2. At **compile start**, process flags are **snapshotted** into `compiled_value->compile_policy` (`afw_compile_policy_init_from_flags`).
3. `#compile` **mutates only that policy** — never `afw_flag_set` / process flags. Sibling compiles and the rest of the request keep their defaults.
4. Unmentioned knobs keep the snapshot; mentioned names are forced for this unit (except `noTypeCheck`).
5. Type checks use **`AFW_VALUE_TYPE_CHECK_*(contextual, xctx)`** macros: if `contextual->compiled_value` then unit policy, else process flags (`afw_flag_is_active`). No ambient xctx policy.
6. New script-overridable compile flags → new **allow-listed operands**, not new top-level `#foo` directives.
7. If a future need is **not** compile policy, add a different `#DirectiveName` — do not invent parallel vocabularies (`#typecheck` was retired for this reason).

### Mid-unit `#compile` (“from here on”)

`#compile` may appear **anywhere a statement is allowed**. It updates the **current unit’s** `compile_policy` **as of that point in the source**:

- **Compile-time** checks for constructs **parsed after** the pragma use the new policy.
- Constructs **already parsed** before the pragma are not re-checked under the new policy.
- **Runtime** checks for values in that unit use the unit policy as last mutated (full unit policy object, not a per-statement stack).

This is intentional. Example:

```text
// process flags may be off; unit starts from snapshot (e.g. off)
const a: integer = "x";     // not checked if policy still off

#compile typeCheck;

const b: integer = "y";     // checked — fails compile
```

```text
#compile typeCheck;
const a: integer = 1;       // checked

#compile noTypeCheck;
const b: integer = "ok";    // allowed after noTypeCheck in this unit
```

### Flags vs unit policy (common confusion)

| Expectation | What actually happens |
|-------------|------------------------|
| `flag_set(["compile:typeCheck"], true)` then compile a string | New unit **snapshots** flags **at that compile**; checking applies unless `#compile noTypeCheck` in that source. |
| `#compile typeCheck` in a script | Only **that unit’s** policy; process flags **unchanged**. Later `compile<script>` without pragma still uses process defaults. |
| Flag on but source has `#compile noTypeCheck` | Unit type-check cluster off — “nothing happened” relative to the flag is correct for **that unit**. |
| Process flags off but source has `#compile typeCheck` | Unit still checks — override is per-unit, not “flags only.” |
| Flag set **during** evaluation of an outer script after inner was compiled | Too late for that inner unit’s snapshot; set flags **before** `compile…` or use `#compile` **inside** the unit under test. |

**Author-facing default story remains flags-first** (handbook). `#compile` is the per-unit / mid-unit override; document both so flag_set users are not surprised.

## Current `#compile` operands (flags they override)

Flag registry text lives in `src/afw/generate/strings/strings.txt` (`a_flag_compile_*_description`); each notes that `#compile` may override the default for one unit.

| Operand | Flag overridden | Notes |
|---------|-----------------|--------|
| `typeCheck` | `compile:typeCheck` | Full compile + runtime checking |
| `typeCheckCompileOnly` | `compile:typeCheckCompileOnly` | Last wins vs `typeCheck` if both appear |
| `noImplicitAny` | `compile:noImplicitAny` | Only meaningful when checking is active |
| `strictNullChecks` | `compile:strictNullChecks` | |
| `strict` | `compile:strict` | Includes typeCheck + noImplicitAny + strictNullChecks |
| `noOptimize` | `compile:noOptimize` | Independent of type-check cluster |
| `noTypeCheck` | *(clears type-check cluster)* | Forces typeCheck, typeCheckCompileOnly, noImplicitAny, strictNullChecks, and strict **off** on unit policy. Does **not** clear `noOptimize`. (Replaces bare `off`.) |

At least one operand required. Spaces separate tokens; no commas. Case-sensitive.

Examples:

```text
#compile typeCheck;
#compile typeCheck noImplicitAny;
#compile typeCheckCompileOnly;
#compile strict;
#compile noTypeCheck;
```

## Retired

- `#typecheck` / `#typeCheck`
- Synonyms: `on`, `true`, `full`, `false`, `compileOnly` (use `typeCheckCompileOnly`), case-folds, optional commas
- `#compile off;` — use **`#compile noTypeCheck;`** (same type-check cluster clear)

## Docs split (later handbook work)

| Surface | Content |
|---------|---------|
| Language reference (authors) | Flags as defaults; short `#compile` override note |
| Advanced / EBNF | Full accept set; compiler-internal as decompile forms only |
| This pad + pragma.c header | Maintainer source of truth for the pattern |

## Open / follow-up

- Handbook language-ref note for `#compile` (flags still primary; mid-unit “from here on” + flag vs unit table above).
- **Decompile / `#…` inventory:** [`decompile-compiler-internal-inventory.md`](decompile-compiler-internal-inventory.md) + `src/afw/tests/compiler/decompile_accept/`.
- Curl nested script functors still pass NULL contextual (process flags only) — see [`compile-contextual-audit.md`](compile-contextual-audit.md). **Not** planned as pragma-able flag metadata; keep a hand allow-list of `#compile` operands.

**Closed for this surface**

- No registry “pragma-able” flag metadata — operands stay an explicit allow-list in `afw_compile_parse_pragma.c`.
- `noOptimize` stays a valid `#compile` operand; optimize work is a separate issue (flag + unit policy remain useful for tests/debug).
