# Issue #38 — Expression property names in object values

**GitHub:** [#38](https://github.com/afw-org/afw/issues/38)  
**Branch:** `issue-#38`  
**Status:** Implemented on branch `issue-#38` (2026-08). User-facing notes in `whats-new.md`.  
**Related (do not block #38):** [#138](https://github.com/afw-org/afw/issues/138) — meta on the wire / `"_meta_"` encoding.

## What ships

In an **object value** (expression-mode `{ … }` only), a property name may be a **bracketed expression**, same string-key idea as existing `obj[expr]` get/set:

```adaptive
{ [expression]: value }
```

Also still valid (unchanged): identifier keys, string-literal keys, `...spread`.

**Not in scope:** property/method shorthand, getters, prototypes, non-identifier **variable** names, destructure computed keys, JSON-strict object text gaining brackets.

## Mental model (AFW only — no ES comparisons in user docs)

| Binding / name | Rule |
|----------------|------|
| Variables, parameters, functions | Lexical **Identifier** only |
| Object **property** names | Strings; identifier form is sugar when the name is a valid identifier |
| `obj.name` / `obj["…"]` / `obj[var]` | Already supported (get and assign via `reference_by_key`) |
| `{ [expr]: v }` | **#38** — construct-time expression key |

Historical intent of brackets: property names that are **not** identifiers (spaces, SQL-ish columns, etc.), not “relax variable naming.”

## Docs

- Describe Adaptive Script only (identifier / string / `[expr]` property names).  
- Do **not** frame as ECMAScript/JS in handbook.  
- Update `features.xml` / `objects-and-arrays.xml` (today they mark `[y]: 3` invalid).  
- EBNF in `afw_compile_parse_Object` comments + harvest as usual.

## Implementation sketch (for the plan)

1. **Parse** (`afw_compile_parse_value.c` / `ObjectValue`): accept `'[' Expression ']' ':' Expression` alongside identifier/string and spread.  
2. **Eval construction:** static names can stay on a compile-time object; expression names (and mix with spread / unevaluated values) need deferred construction—extend or generalize `object_expression` / call graph (ordered entries: static | computed | spread).  
3. **Name evaluation:** same as assignment through `reference_by_key` — string or `as_utf8` / convert-to-string; no special identifier rules for bracket keys.  
4. **Left-to-right** application; last write wins on duplicate names (incl. after eval).  
5. **Literal `_meta_:`** keep **current** peel → `meta_set_meta_object` (parse sideband).  
6. **Computed name that evaluates to `"_meta_"`:** treat as **normal property** (same as `obj["_meta_"] = …`). Do **not** invent new language meta rules; collision on JSON/YAML/UBJSON is **#138**.  
7. **Embedded nested objects:** computed names break “property name known before parsing value” embedding; create non-embedded when name is expression.  
8. **Decompile / listing:** emit `[…]: …`.  
9. **Tests:** non-identifier fixed via string (already); expression keys; template/concat; mix with spread; last-wins; pure JSON mode rejects brackets; regression that `obj[k]` still works.

## Explicit non-decisions for #38

- Content-type dual encoding / kill reserved `"_meta_"` → **#138**.  
- Do not ban `set_property(…, "_meta_", …)`.  
- Destructure still identifier-only property side unless a later issue.

## Implementation plan

### Goal

Ship `{ [expression]: value }` in Adaptive Script **object values** only, with AFW-only docs/tests. No #138 encoding work.

### Current construction paths (baseline)

`afw_compile_parse_Object(..., allow_expression, …)` today:

| Situation | Result |
|-----------|--------|
| All property names static; all values evaluated | Unmanaged **object** (+ optional literal `_meta_` via `meta_set_meta_object`) |
| Static names; some values not yet evaluated | **`object_expression`** — bag of known names, eval values on evaluate |
| Any `...spread` | Call to **`add_properties`** (base object/`object_expression` + spread objects) |

`object_expression` **cannot** hold expression names: eval walks `get_next_property` with fixed names.

`obj[key]` get/set already uses `reference_by_key` + string/`as_utf8` for names.

### Recommended approach

**A. Parse** — When `allow_expression` is true, accept property name forms:

- unqualified **Identifier**
- **String** (existing strict vs relaxed quote rules)
- **`[` Expression `]`** (new; only if `allow_expression`)

Strict JSON / non-expression object parse: **reject** `[` as property name (unchanged “Invalid property name”).

EBNF update `ObjectValue` only (not `Object` / `ObjectLiteral` / pure JSON).

**B. Representation when any name is an expression**

Do **not** force everything through a static-name bag.

Introduce an ordered **entry list** for deferred construction (preferred: extend the value model rather than a one-off compile-only struct that never evaluates cleanly):

Each entry is one of:

1. **Static pair** — `property_name` (utf8) + `value` (may be expression)  
2. **Expression-name pair** — `name_expr` + `value`  
3. **Spread** — `object_expr` (existing `...`)

On evaluate (into current pool):

1. Create mutable unmanaged object.  
2. For each entry left to right:  
   - static: `set_property(name, evaluate(value))`  
   - expression name: evaluate name → string (same as assign path: string or `afw_value_as_utf8` / convert); evaluate value; `set_property`  
   - spread: evaluate object; copy properties onto target (same semantics as `add_properties` source merge)  
3. Last write wins on duplicate names.  
4. Return object value.

**Optimization:** If **no** expression-name entries and **no** spreads, keep **today’s** paths (plain object / `object_expression` / `add_properties` only). That limits risk to the new syntax and mixed cases that need ordered entries.

**Literal `_meta_:`** (static name only): keep peel → install meta on the result object when construction finishes (only when name is the **literal** identifier/string `_meta_` at parse time, not when a computed name evaluates to that string).

**Computed name → `"_meta_"`:** normal `set_property` (documents #138 footgun; no language magic).

**C. Where to put the entry-list value**

Pick one during implement (plan preference first):

1. **Preferred:** New value inf e.g. `object_literal` / `object_construct` with `entries[]` + contextual — evaluate/decompile/listing like `object_expression` / `list_expression`. Register in value registry.  
2. **Alternative:** Lower to a built-in (new or synthetic) that takes a structured args array — more generate churn, easier decompile as call.

Prefer (1) unless entry list fits an existing pattern with less code.

**D. Nested objects / embedding**

When the **parent** property name is an expression name, do not use `parser->property_name` for `AFW_OBJECT_CREATE_ENTITY_OR_EMBEDDED` embedding into a parent bag under a known name:

- Parse value with `property_name` unset / force unmanaged create for that child.  
- Attach only after the name expression is evaluated at eval time.

Static-name nested objects can keep current embedding behavior.

**E. Decompile / compiler listing / pragmas**

- Listing: show expression-name entries as property name expressions + value listing.  
- Decompile: emit `{ [ <name decompile> ]: <value>, ... }` and preserve spreads.  
- If #list_expression-style pragma exists for objects, only add if needed for round-trip tests; don’t invent unless decompile fidelity requires it.

**F. Docs (AFW-only prose)**

- `src/afw/doc/reference/language/features.xml` — remove “invalid (computed name)”; describe expression names.  
- `src/afw/doc/reference/language/objects-and-arrays.xml` — property names: identifier, string, or bracket expression.  
- No ECMAScript comparisons.

**G. Tests** (new or extend under `src/afw/tests/…`)

| Case | Expect |
|------|--------|
| `{ [k]: 1 }` with `k` string var | property set |
| `{ ["a b"]: 1 }` | non-identifier name |
| `` { [`p${n}`]: v } `` | template key |
| Mix static + expression + spread | order + last-wins |
| Expression value + expression name | both eval |
| Nested object under expression name | works; no wrong embed |
| Pure `json` / strict object | `[` name is syntax error |
| `obj[k]=` still works | regression |
| Decompile or listing smoke if easy | optional but good |
| Literal `_meta_: { path: ... }` still meta | no change |
| `{ ["_meta_"]: 1 }` | bag property if set_property allows (no throw) |

**H. Build / verify**

```bash
./afwdev build --cdev
afwdev test -j --srcdir-pattern afw --test-pattern '…'   # narrow while iterating
# before PR: ./afwdev build --fulldev && afwdev test -j --env-mode valgrind
```

Generate only if new function metadata is chosen (path 2); path 1 may be hand C only.

### Work phases

| Phase | Work | Done when |
|-------|------|-----------|
| **1. Parse** | Token `[` as name; parse Expression; colon; value; EBNF | Syntax accepted only in expression mode; rejected in JSON |
| **2. Construct** | Entry list + evaluate; keep fast path for static-only | `{ [k]: v }` evaluates correctly |
| **3. Edges** | Spread mix, nesting, `_meta_` literal vs computed, name string conversion | Tests green |
| **4. Decompile/listing** | Emit/list expression names | Listing/decompile don’t crash; fidelity if practical |
| **5. Docs + tests** | Handbook XML; full test file | Docs match behavior |
| **6. Verify** | `--cdev` + focused tests; fulldev/valgrind before PR | Maintainer bar |

### Out of scope (explicit)

- #138 dual encoding / reserved-key fix  
- Destructure `{ [k]: x }`  
- Property/method shorthand  
- Changing variable identifier rules  

### Risks

| Risk | Mitigation |
|------|------------|
| Breaking static object / spread paths | Keep existing paths when no expression names |
| Meta / embed footguns | Literal `_meta_` only; unmanaged children for expr names |
| Name type chaos | Mirror `reference_by_key` assign conversion |
| Decompile gap | Minimum: no crash; improve fidelity if cheap |

### Success criteria

- Scripts can build objects with expression property names in one object value.  
- Behavior matches `let o={}; o[k]=v` for the key/value pair.  
- Docs AFW-only; tests cover main cases.  
- No requirement to solve #138.

### Implementation status

Done on working tree (hold commit for review):

- `afw_value_object_construct` value inf (entries: static / name_expr / spread)
- Parse `[ Expression ]` names in `ObjectValue`; migrate static/spread to construct when needed
- Tests: `src/afw/tests/language/script/object_expression_names.as` (11 passed)
- Docs: `features.xml`, `objects-and-arrays.xml`
- Fast path unchanged when no expression property names

Commit when maintainer asks.
