# Issue #17 — mutable object faces (shared instance problem)

**Audience:** maintainers / assistants. **Not** handbook.  
**GitHub:** [#17](https://github.com/afw-org/afw/issues/17) (title may widen beyond “object literals immutable”).  
**Branch:** `issue-#17-object-literals-immutable` (off `mgg-develop`).  
**User-facing framing:** [`whats-new.md`](../whats-new.md) — *Mutable object faces (issue #17, in progress)*.

---

## Product problem (one theme)

Callers often expect an object value to be **theirs to mutate**. AFW often hands back a **shared instance**:

| Area | Example |
|------|---------|
| Script **literals** | Same `{…}` bag across evals / re-entry |
| **Bind / assign** | Shared bag unless cloned today |
| **Defaults** (#110) | `property_get` / `variable_get` mutable defaults |
| **Built-in returns** | `get_object`, retrieve, … — ES authors often `clone()` by hand |
| **Arrays** | Same class of problem; later slices |

**Goal:** platform supplies a **mutable face** (look-through wrapper) so the common path does not poison shared bases or the next evaluation.

- **`const`** — binding-level only (not deep freeze)  
- **`freeze`** — explicit immutability  
- **`clone()`** — deep copy / escape hatch when still needed  

Whats-new speaks this theme (mutable faces / shared instances), not only “literal IR.” Issue rename is fine later.

---

## Approach (current)

### 1. Memory face (landed)

`afw_object_create_wrapper_*` (`afw_object.h` / `afw_object_memory.c`):

- Local properties first, then look-through to `wrapped` base  
- Sets stay on the face  
- Nested **objects** promoted on get (arrays not yet)  

### 2. Isolation step as a normal function (landed)

**`wrap_literal_object`** (`compiler_internal`):

- Evaluate object arg → `create_wrapper_unmanaged` → return face as normal object value  
- Metadata + execute in `afw_function_compiler_internal.c`  
- Tests: `object_literal_wrapper.as` (function tests on; auto-isolate placeholder skipped)  

No special value kind. Compiler will emit a **call** to this function, like other compiler_internal helpers.

### 3. Compiler emit (next major step; not done)

**Where:** `afw_compile_parse_Object` in `afw_compile_parse_value.c` (value `{…}` funnel; nested recurses here).

**Not:** type-syntax `ObjectTypeLiteral`; YAML has its own parser (see below).

**Embedding:** `parser->embedding_object` / `property_name` — entity (top for embedding) when parent is NULL at entry.

**Gate with `compile_type`:** wrap on script/template-ish paths; **not** on `json` / `relaxed_json` (conf and pure data stay plain objects).

**v1 rule of thumb:** topmost/entity constant objects on script paths; nested via promote-on-get.

---

## Other call sites (same theme, later)

| Site | Notes |
|------|--------|
| **Assign / Pattern bind** | Today clones evaluated object/array — revisit after emit (may become redundant) |
| **Defaults (#110)** | Clone mutable defaults — keep until wrap-equivalent |
| **Built-in returns** | Prefer wrap face for ES-friendly “mine to mutate”; policy for write-back / cost |
| **YAML** | Separate parser (`afw_yaml_to_value.c`); not `parse_Object` — later if needed |
| **User `clone()`** | Keep |

---

## Process

- Step-by-step; implement/commit only when asked.  
- One thing at a time when possible.  
- Tests before user-visible commits.  

---

## Sequence (flexible)

1. ~~Memory `create_wrapper_*`~~  
2. ~~`wrap_literal_object` + tests~~  
3. ~~Baseline: remove object clone-on-bind (arrays still clone)~~ — see below  
4. Compiler emit + auto-isolate tests  
5. Revisit assign/pattern **array** clone  
6. Arrays isolation  
7. Built-in returns  
8. Defaults  
9. YAML if needed  
10. Issue rename + finalize whats-new on `mgg-develop`  

---

## Baseline: object clone-on-bind removed (working tree)

**Change:** `impl_assign` + `afw_function_script_assign_pattern` clone **arrays only**; objects only evaluate.

**`afwdev test -j --srcdir-pattern afw`** (after `--cdev`):

| | |
|--|--|
| Passed | 3183 |
| Failed | **1** |
| Skipped | 182 |

### Failures (baseline without object clone-on-bind)

| Location | Sub-tests / notes |
|----------|-------------------|
| `miscellaneous/property_get.as` | `property_get-issue-110-onGetObject-shape` |
| `language/script/object_literal_wrapper.as` | **13 red** isolation cases (below); **3 green** explicit `wrap_literal_object` |

**Isolation cases in `object_literal_wrapper.as` (all expect fail until real fix):**

- `shared-literal-function-two-calls` — const + mutate, two calls  
- `shared-literal-let-two-calls` — let variant  
- `shared-literal-return-mutate` — `return {}` then mutate  
- `shared-literal-nested-object` — nested `{ child: { n: 0 } }`  
- `shared-literal-nested-assign-empty` — `y.z = {}` residual  
- `shared-literal-param-default` — `function f(o = {})`  
- `shared-literal-param-pattern-default` — `{ bag } = { bag: { n: 0 } }`  
- `shared-literal-object-pattern-bind` — `const { x } = { x: { n: 0 } }`  
- `shared-literal-loop-calls` — loop residual property  
- `shared-literal-lambda-two-calls` — lambda body  
- `shared-literal-empty-property-exists` — empty `{}` + extra prop  
- `shared-literal-compile-evaluate-twice` — compile once, evaluate twice  
- `shared-literal-compiled-function-two-calls` — model **on*** style  

**Pitfall:** pure top-level script returning only a number may **constant-fold**; prefer return object or call a function twice.

### Takeaway

These reds are the intentional gate for object-literal isolation (not array bind-clone, still temporary).  

---

## Related

- #110 — default clone · #2 / `memory-management.md` · #22 ES expectations · `afw-function.mdc` (mutable defaults)  
