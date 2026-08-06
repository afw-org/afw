# Issue #17 — mutable object faces (shared instance problem)

**Audience:** maintainers / assistants. **Not** handbook.  
**GitHub:** [#17](https://github.com/afw-org/afw/issues/17) — **closed** (merged).  
**Landed:** [PR #150](https://github.com/afw-org/afw/pull/150) → `mgg-develop` (2026-08-06, merge `dd318e4f`).  
**User-facing framing:** [`whats-new.md`](../whats-new.md) — *Mutable object faces (issue #17)*.

---

## Product problem (one theme)

Callers often expect an object value to be **theirs to mutate**. AFW often hands back a **shared instance**:

| Area | Example |
|------|---------|
| Script **literals** | Same `{…}` / `[…]` bag across evals / re-entry |
| **Bind / assign** | Shared bag unless isolated (was clone; now faces on literals/returns) |
| **Defaults** (#110) | `property_get` / `variable_get` mutable defaults |
| **Built-in returns** | `get_object`, retrieve, journal get, … |
| **Arrays** | Same class of problem as objects |

**Goal:** platform supplies a **mutable face** (look-through wrapper) so the common path does not poison shared bases or the next evaluation.

- **`const`** — binding-level only (not deep freeze)  
- **`freeze`** — explicit immutability  
- **`clone()`** — deep copy / escape hatch when still needed  

---

## Landed approach

### 1. Memory faces

- **Objects:** `afw_object_create_wrapper_*` — local props first, look-through base, sets stay on face.  
- **Arrays:** `afw_array_create_wrapper_*` — local ring; nested structured values re-faced on materialize / promote.  
- **Nested hard edge:** always put a **new** face over the nested **instance as given** (do **not** peel to ultimate base — preserves face-ring content, e.g. model `onGetProperty` `let l=[]; add_entries…`). Typed `map` uses `get_next_internal`, which promotes like `get_next_value`.

### 2. Isolation helpers + compiler emit

- `wrap_literal_object` / `wrap_literal_array` (`compiler_internal`).  
- Compiler emits wrap on **script/template** constant object/array literals; **not** on pure `json` / `relaxed_json` conf paths.  
- Nested structure under faces: promote-on-get / array materialize (not a second full-tree wrap emit).

### 3. Bind / assign (no clone-on-bind)

**Audit (2026-08):** `impl_assign` and `afw_function_script_assign_pattern` **evaluate only** — **no** `afw_value_clone` for objects or arrays.

```c
/* Objects and arrays: no clone-on-bind — issue #17 faces */
```

Temporary “arrays still clone” was removed when object clone-on-bind dropped; array faces cover multi-call isolation. **Rest pattern** `const [a, ...rest] = arr` still **builds a new array** of remaining entry values — that is **destructure semantics**, not bind clone (must allocate; empty rest is `[]`).

### 4. Defaults (#110)

`afw_value_isolate_mutable_default` — always a **new** face over object/array default base (not deep clone of whole graph).

### 5. Built-in returns (script-facing)

| Path | Policy |
|------|--------|
| Adapter `get_object*` | Face (except reconcilable get — keep entity for reconcile) |
| Materializing `retrieve_objects*` | Face each object |
| `retrieve_*_to_callback` | Face before user CB |
| `retrieve_*_to_response` / `_to_stream` | **No** face (encode path) |
| Journal get / next / by_cursor / consumer / after_cursor / advance | Face response object |
| Journal CRUD “receipt” entries | Fresh memory, not store rows — no face required |
| YAML parse | **No** parse-time face; conf/store plain; adapter/journal faces on script get |

### 6. YAML (same branch, beta hygiene)

- Fixed `afw_yaml_to_object` / raw_to_object for file `contentType: yaml`.  
- Plain scalar typing, empty `[]`/`{}`, parser delete.  
- Tests under `src/afw_yaml/tests/`.

---

## Tests (representative)

| Suite | Covers |
|-------|--------|
| `object_literal_wrapper.as` / `array_literal_wrapper.as` | Literals, multi-call, nested hard edge, property_get/variable_get defaults + map |
| `issue17_faces_regression.as` | Cross-path mix |
| File adapter / journal tests | get/retrieve faces; journal consumer peers seeded on journal adapter |
| `src/afw_yaml/tests/` | allow output + to_object |

---

## Not goals of #17

- Deep freeze trees via `const`  
- Write-through faces into adapter stores  
- Sparse array holes (**#39**, separate)  
- Runtime / `afw` catalog liveness & materialize cost — **[#149](https://github.com/afw-org/afw/issues/149)** under **#2** ([`runtime-catalog-lifetime.md`](runtime-catalog-lifetime.md))

---

## Ship status

**Done** — PR **#150** merged to `mgg-develop`. Post-merge checklist complete (2026-08-06).

### Close checklist

- [x] PR merged to `mgg-develop`  
- [x] `beta-backlog.md` #17 line → done  
- [x] whats-new Highlights / section status for landed line  
- [x] Comment on GitHub **#17** with ship summary  
- [x] Rename issue title to mutable faces / shared instances  
- [x] Close **#17**  

### Related (not #17)

- **#149** runtime catalogs under **#2** — [`runtime-catalog-lifetime.md`](runtime-catalog-lifetime.md)  
- **#39** array holes — separate  

### Code map (for archaeology)

- Faces: `afw_object_memory.c`, `afw_array_memory.c`  
- Adapter/journal: `afw_function_adapter.c`, `afw_function_journal.c`, `afw_file_journal.c` (advance start)  
- Defaults: `afw_value_isolate_mutable_default`  
- Emit: `afw_compile_parse_value.c`, `wrap_literal_*`  
- Tests: `object_literal_wrapper.as`, `array_literal_wrapper.as`, `issue17_faces_regression.as`, file_journal, `src/afw_yaml/tests/`
---

## Author rules of thumb

| Want | Use |
|------|-----|
| Mutate without poison (common path) | Face (platform) — drop defensive `clone()` |
| True independent deep graph | `clone()` |
| Immutability | `freeze` (not `const` alone) |
| Persist adapter data | add / modify / replace / update — not face sets |
| Reconcile after edit | reconcilable get + platform path; face may not be the entity |

---

## Process notes

- Step-by-step; commit when asked.  
- Design pads under `designs/` for maintainers; whats-new for users.  
