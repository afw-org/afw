# Pragma and `#` design (Pattern B)

**Status:** Active on `issue-#28-pragma-cleanup` (and follow-ons).  
**Code home:** `src/afw/compile/afw_compile_parse_pragma.c` (file header mirrors this).  
**Related:** compiler-private `#` forms — `afw_compile_parse_compiler_private.c`; flags — `afw_flag.c` / `generate/strings/strings.txt`.

## Two surfaces that share the `#Name` token

| Kind | Parse | Audience | Role |
|------|--------|----------|------|
| **Pragma** | `afw_compile_parse_pragma.c` | Script authors | Per-compile **policy** |
| **Compiler-private** | `afw_compile_parse_compiler_private.c` | Toolchain only | **decompile → recompile** (`#block`, `#script_function`, …) |

Lex: `pragma_identifier` for any `#Name`. Product docs should not teach compiler-private forms as normal script.

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

1. Operands are **short names** of `compile:*` flags (the part after `compile:`), same spelling as the registry — plus special **`off`**.
2. At **compile start**, process flags are **snapshotted** into `compiled_value->compile_policy` (`afw_compile_policy_init_from_flags`).
3. `#compile` **mutates only that policy** — never `afw_flag_set` / process flags. Sibling compiles and the rest of the request keep their defaults.
4. Unmentioned knobs keep the snapshot; mentioned names are forced for this unit (except `off`).
5. Type checks use **`AFW_VALUE_TYPE_CHECK_*(contextual, xctx)`** macros: if `contextual->compiled_value` then unit policy, else process flags (`afw_flag_is_active`). No ambient xctx policy.
6. New script-overridable compile flags → new **allow-listed operands**, not new top-level `#foo` directives.
7. If a future need is **not** compile policy, add a different `#DirectiveName` — do not invent parallel vocabularies (`#typecheck` was retired for this reason).

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
| `off` | *(clears type-check cluster)* | Forces typeCheck, typeCheckCompileOnly, noImplicitAny, strictNullChecks, and strict **off**. Does **not** clear `noOptimize`. |

At least one operand required. Spaces separate tokens; no commas. Case-sensitive.

Examples:

```text
#compile typeCheck;
#compile typeCheck noImplicitAny;
#compile typeCheckCompileOnly;
#compile strict;
#compile off;
```

## Retired

- `#typecheck` / `#typeCheck`
- Synonyms: `on`, `true`, `full`, `false`, `compileOnly` (use `typeCheckCompileOnly`), case-folds, optional commas

## Docs split (later handbook work)

| Surface | Content |
|---------|---------|
| Language reference (authors) | Flags as defaults; short `#compile` override note |
| Advanced / EBNF | Full accept set; compiler-private as decompile forms only |
| This pad + pragma.c header | Maintainer source of truth for the pattern |

## Open / follow-up

- Explicit allow-list metadata on flag registration (“pragma-able”) if the set grows.
- Whether `noOptimize` stays pragma-able long term (host-only vs script).
- Handbook language-ref note for `#compile` (flags still primary for authors).
- Optional: more call sites still pass NULL contextual by design (higher-order) → flags only.
