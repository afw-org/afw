# Memory management (issue #2) — working notes

**Audience:** maintainers and assistants.  
**Not user docs.** Related always-on runtime rules: [`.cursor/rules/afw-value-memory.mdc`](../.cursor/rules/afw-value-memory.mdc). Beta checklist pointer: [`beta-backlog.md`](../beta-backlog.md). GitHub: [#2 Memory management](https://github.com/afw-org/afw/issues/2).

## Purpose of this file

Living design / discussion notes for long-running AFW process memory: pools, value lifetimes, object/array ↔ value identity, escape (closures, retained results), and what we will implement later.

**End goal of the discussion:** a coherent **AFW memory-management story** — useful for implementing **#2** now, and as durable context for **humans and assistants** on later work (not only this issue). Prefer clear narrative and invariants over a pile of undigested archaeology.

**Related (not the same work as #17 faces):** GitHub **[#149](https://github.com/afw-org/afw/issues/149)** — runtime env metadata / registry views (accessor correctness, per-type lifetime). Architecture: [`runtime-objects-and-environment.md`](runtime-objects-and-environment.md); discovery: [`runtime-catalog-lifetime.md`](runtime-catalog-lifetime.md).

### Adaptive values are a big deal

**`afw_value` (adaptive values) are central to AFW** — not a minor helper type. Script/eval is a graph of values; compile returns a value; functions, literals, objects-as-data, arrays, closures, and scope variables are (or carry) values. Lifetime and escape for long-running work hang on **value inf policy** (`optional_evaluate`, `optional_release`, `clone_or_reference`) with pools/subpools underneath. When in doubt for #2 and script MM: **think in values first**; hide pool/object nastiness behind the value. (Stated again in other conversations; recorded here so it is not only chat memory.)

- **Do** record decisions, open questions, rejected ideas, and archaeology.
- **Do not** treat this as committed API or a merge plan for old branches.
- Code changes come only after discussion produces clear invariants and a phased plan.

**Mode (2026-07+):** step-wise with the partner workflow below. No large implementation until the current step’s plan is agreed and execute is requested.

### Partner workflow for #2 (complex work)

Order of phases is the one we would use by hand (see **Phased plan**). For **each** phase:

1. **Discuss** the step (context from this file + code + old-branch ideas).  
2. **Plan** that step only (concrete, reviewable).  
3. **Tweak** until both are happy; **execute only when told**.  
4. When both satisfied with the result: **commit and push**, then reassess “where are we?” before the next step.

While doing a step, keep an eye on **later phases** so choices stay aligned (e.g. phase 0 bindings should not paint us into a corner for containers/`->value`/assign). Draw on the full memory-management discussion.

---

## Historical framing (from discussion)

Early AFW mostly served **REST-like request** traffic. Lifetime of work memory was **the request**. It was acceptable to rely heavily on **garbage collection when the request ended** (destroy the request-session xctx / its pool). That **ultimate per-request cleanup still applies** in `afwfcgi` and is the baseline that made short-lived processing safe without perfect managed escape on every temporary.

What changes for **long-running** work is anything that must **survive a request** (or live for process/env life): registries, permanent values, escaped closures, retained results, env-level objects. Those cannot depend on request-end bulk free alone — that is the #2 pressure. Pools discussion next; server/request framing is the outer shell.

---

## Problem statement

Short scripts / single requests are production-proven via **hierarchical pools** and bulk free when request xctx (or process) ends. **Long-running hosts** (multi-request server, retained closures/results) need correct **managed escape**: get_reference / optional_release / clone paths so values that outlive a scope/request neither leak nor use-after-free.

Complementary, not substitutes:

| Track | Role |
|-------|------|
| **#2** (this) | Lifetimes, refcount/pool ownership, value identity, OOM resilience story |
| **#49** / maxObjects | Author-visible caps on materializing retrieves |
| **#127** | Progressive release after stream/to_response write |
| **#35** `after2-*` | Escaped closures trustworthiness (depends on #2) |
| **#9** snapshots | Large fresh memory objects; stress #2 if retained |

---

## Working method

1. Use stale branch **`use-value-in-array-and-object`** as **archaeology / intent**, not something to merge.
2. Combine that with **current tree** (`mgg-develop` / `issue-#2`), **afw-value-memory** rules, and **live discussion**.
3. Prefer **invariants** over “port commit X.”
4. Implement later in **small vertical slices**, not a big-bang generator flip.

**Discussion style:** Hop around AFW areas freely. Speaker will try to name the **context** (pool, object, scope, adapter, …). This file is rearranged over time so both humans and assistants can find notes by topic — not a fixed agenda. Cross-cutting decisions still land in **Decisions** / **Invariants** when we settle them.

---

## Archaeology (stale branch)

**Branch:** `origin/use-value-in-array-and-object`  
**Tip:** `0c34bd11` (2024-10-09) — *More use of object->value*  
**Merge-base with develop (when reviewed):** `8a987ded` (2024-08)

### Already on develop / current tree (foundation)

Rough themes from ~135 older `#2` commits and related PRs:

- Pool/subpool model, reparent on destroy, managed vs unmanaged naming
- `afw_object_create()` uses subpool for managed objects
- `afw_value_*_t` inf/value union
- Public **`object->value`** and **`array->value`** fields; implementations set them (PR **#84**)
- Memory object/array **embed** a value and point `pub.value` at it
- Managed create / `optional_release` / `clone_or_reference` machinery (PR **#77** and later)
- Later: **managed_slice** (utf8/memory), free-at-0 refcount semantics, value-memory cursor rules

### Unmerged tip (4 commits, Aug–Oct 2024) — ideas only

Intent: stop double-wrapping objects; use the instance’s own value.

| Idea | Notes |
|------|--------|
| `create_managed/unmanaged_object` return `internal->value` | Was gated behind FIXME / `False and` until all objects set `value` |
| Managed path: `afw_object_get_reference` then return `->value` | Identity + real object lifetime |
| Drop `afw_value_allocate_unmanaged_object` for object type | Call sites use `object->value` |
| Managed release/get_reference special-case object | Branch mixed `afw_object_release` and `pool_get_reference(object->p)` — **must pick a single model** |
| Env function/parameter objects get permanent `value` | Prerequisite for strict “always have value” |
| Clone / utf8 paths use `object->value` | Avoid stack-local wrapper values |
| Test: `_AdaptiveEnvironmentRegistry_/current` has no meta errors | Smoke for “walk everything as values” |
| Runtime bugfix: `pub.value = value` not `&value` | **Still present** in current `afw_runtime.c` — easy later fix |

**Array half of the branch name:** struct + memory/const already set `array->value`; tip never specialized generator for arrays. Same gap pattern as objects: ~many `create_unmanaged_array` still allocate wrappers.

**Do not merge the branch.** Re-apply agreed design on current generators (managed_slice, etc. have moved on).

---

## Current tree snapshot (as of discussion start)

- Memory objects/arrays: embedded value + `pub.value` set.
- `afw_value_create_unmanaged_object` / `_array` still **allocate separate wrappers** in generated bindings (dominant call path today).
- Managed optional_release for object currently frees the **wrapper** only (refcount on managed value), not necessarily the underlying object.
- Short scripts: pool bulk free papers over incomplete escape.
- Long-running: double-wrap + incomplete release is the core pain the tip was addressing.

---

## Cross-cutting (filled as we settle things)

### Design lenses (reference map — not an agenda)

Useful labels when tying subsystem notes together. We do **not** have to cover them in order.

| Lens | What it means |
|------|----------------|
| **Identity** | object/array ↔ `->value`; one instance vs wrapper values |
| **Lifetime** | pool/subpool, object release, value refcount — who owns what |
| **API** | `create_managed_*` / `create_unmanaged_*` / `allocate_*` meaning |
| **Escape** | outliving a scope/request: closures, retained results, snapshots |
| **Verify** | tests, valgrind multi-request, when missing-value is an error |

### Value Lifetime Model (target end-state)

**Source:** parallel brainstorming conversation (2026-07-24).  
**Role:** intended **end-state** for Adaptive value memory management — **not** a rewrite of the phased plan (1d still next for create identity; 2 still assign/scope). Implementation remains incremental; this section answers “what correct looks like.”

**Naming:** code today uses `afw_value_optional_release` / `clone_or_reference`. Below, **release** means that optional_release path (macros may be named `afw_value_release` later or already alias).

#### Core invariant

1. **`afw_value_clone_or_reference()` always returns a managed or permanent value. It never returns an unmanaged value.**
2. Any value **stored** in a **scope slot**, **managed object property**, **managed array element**, or kept as an **evaluation result** that must outlive pure pool-borrow must be **permanent or managed**.

Unmanaged values remain valid as **temporaries** owned by a pool (classic C / request scratch). Escaping them into long-lived slots goes through `clone_or_reference` first.

#### Value kinds and policy

| Kind | `clone_or_reference` | release |
|------|----------------------|---------|
| **Permanent** | return self | no-op |
| **Managed** | hold (RC++) **or** clone into dest pool, per inf | decrement; free resources at 0 |
| **Unmanaged scalar** | **always** produce a **managed clone** in the destination pool | original: no-op (pool owns original) |
| **Unmanaged object/array** | create a **managed wrapper value** in dest pool that points at the real unmanaged instance, **takes a reference on `instance->p`**, and is itself a normal managed value | wrapper release drops that pool pin (+ free wrapper header); original dual face / instance unchanged |
| **Managed slice** | only on **managed or permanent** bases; slice of unmanaged → **error** | existing slice + containing rules |

**Unmanaged object/array wrappers (target):**

- Multiple independent managed wrappers for the **same** unmanaged instance are **allowed**.
- Wrapper is a **separate value face** from any dual face on the original instance (not the same pointer as `obj->value` unless deliberately unified later).
- Proposed helpers (names adjustable): `afw_object_create_managed_wrapper` / `afw_array_create_managed_wrapper` — live in object/array impl files; called from **unmanaged** value inf’s `clone_or_reference`.

**Future (note only):** reference-counted large `afw_utf8_t` / `afw_memory_t` impls will be **managed** and thus **sliceable**, enabling cheap cross-scope assign of large strings without deep copy.

#### Containers

**Scope slots (managed container of values)**

- On assign: `release(old)` → `stored = clone_or_reference(new, scope->p, xctx)` → store `stored`.
- On final scope release (scope RC → 0): **walk all slots**, `release` each, then destroy the scope subpool.

**Managed objects / arrays**

- Own a **private subpool** (current memory managed create).
- `clone_or_reference` on the **container value** → reference the object/array (`afw_object_get_reference` or array equivalent when it exists).
- release on container value → object/array release; when **container** RC hits 0: **walk properties/elements and release each**, destroy private subpool, free value header **only if not embedded dual face**.
- **Property/element store:** `release(old)` if present → store incoming pointer **as-is (lazy)**.
- **Property/element get:** if stored value is still **unmanaged**, promote with `clone_or_reference(stored, container->p, xctx)`, **cache** promoted value back into the slot, return it. Subsequent gets are cheap.

**Unmanaged objects / arrays**

- Classic behavior: store/return raw pointers; lifetime = pool hierarchy. **No** promotion, **no** per-value release of contents on “object release” (there is none beyond pool destroy).

#### Evaluation result

- Result of evaluate lives in the **pool supplied by the caller** (`p`).
- **Verify:** compiled_value evaluate path should ensure the final result is permanent or managed in that evaluate pool (via `clone_or_reference` if needed). Existing tests + valgrind give confidence this is largely already correct; confirm when wiring phase 2.

#### Closures

- Hold a reference on the **captured scope**. Scope RC + **slot-release walk** keep captured values alive.
- Existing closure behavior expected to keep working; residual improvements are **follow-on** after core lifetime pieces.

#### Additional target notes

- **Plan to restore classic-style pools** (no per-block free; free is a no-op) for common short-lived / request-oriented cases. Newer parent/**subpool** impl is primarily for **script evaluation and long-running** cases.
- **Adaptive values are themselves immutable.** Object and array **values** hold an immutable reference to a (possibly **mutable**) instance. Mutation of the instance is visible through all references to that instance.
- **Dual-face rules remain:** especially **never free an embedded dual face**.
- **Error paths** (OOM during wrapper creation or `clone_or_reference`) must not leave unbalanced pool references or partially promoted state.

#### Highest-priority implementation pieces (for long-running script safety)

1. Scope-slot walk on final scope release (+ assign release old / clone_or_reference new).  
2. Managed container release walk of properties/elements.  
3. Unmanaged → managed **wrapper** on `clone_or_reference` (object/array).  

Phase **1** (faces, create identity) remains foundation so dual face and managed holds are honest; phase **2** is where this target becomes load-bearing.

#### Relation to earlier 1d draft

1d (create prefers `internal->value`) still stands for **avoid routine double-wrap of create APIs**. This target model adds: **escape path** is always managed/permanent via `clone_or_reference`, including **wrapper** for unmanaged containers (possibly multiple wrappers). Dual-face identity and managed wrapper are different tools — do not conflate when coding.

### Invariants (draft + target)

_Many draft items below are superseded or refined by **Value Lifetime Model (target)** above. Keep historical drafts for archaeology; prefer target section when implementing._

1. **(draft / 1b′)** Every constructed object/array sets `->value` before it is published to any API that can observe it.
2. **(superseded nuance)** Create identity vs managed wrapper: see target model + 1d — dual face for instance face; **managed wrapper** for unmanaged escape.
3. **(target)** Managed container value hold = object/array get_reference; never free embedded dual face.
4. **(draft)** Evaluation temporaries that do not escape may use scope/`xctx` pools; stored/escaped values must be permanent or managed (target core invariant).
5. **(draft)** Shared permanent/const mutables returned as defaults still require clone-into-call-pool (existing #110 class rule).
6. **(target)** Managed object/array on container RC → 0: release each property/element value, then destroy private subpool.
7. **(target)** Scope: subpool of `xctx->p`; assign via clone_or_reference; final release walks slots then subpool.
8. **(draft / historical)** Short-lived expression/script/model work may use a single outer pool for the whole unit; long-running loops/recursion need scope subpools + managed escape.
9. **(draft / pattern)** Some containers manage lifetime by holding a reference on their own pool/subpool — still valid under target for managed containers.
10. **(target)** Assign: value methods hide pool/object nastiness; callers use clone_or_reference / release only.
11. **(target / phase 2)** `afw_xctx_scope_release` final teardown: walk `symbol_values[]` + release each, then subpool. **Today:** pool-only. `afw_xctx.h` evaluation_result comments partly aspirational.
12. **(target)** `clone_or_reference` never returns unmanaged.
13. **(target)** Adaptive values immutable as values; instance behind object/array may be mutable and shared.

### Decisions

| Date | Decision | Notes |
|------|----------|--------|
| 2026-07-22 | Do **not** merge `use-value-in-array-and-object` | Source of ideas only |
| 2026-07-22 | Discussion first; little/no code until invariants and phases are clear | This file is the memory pad |
| 2026-07-22 | Hop around by AFW area; rearrange this file by topic as we go | No fixed discussion order |
| 2026-07-23 | #2 “done” is step-by-step; we’ll know when we get there | No fixed beta checklist yet |
| 2026-07-23 | Implementation order roughly: managed containers / `->value` → assign `clone_or_reference` → scope/symbol release correctness → accounting later | Adjust as we go |
| 2026-07-23 | **Value** layer owns lifetime policy; **object/array** (main value containers) must be consistent across impls; hide nastiness inside implementations | Pool is storage under containers |
| 2026-07-23 | `compiled_value` today dies with its compile pool; may need pool/value refcount if scripts `compile()` and return/escape that value (closures, return results) | Open — note, don’t implement yet |
| 2026-07-23 | **Adaptive values (`afw_value`) are a big deal in AFW** — central abstraction for eval, assign, escape, and MM | Elevate in this doc; not optional detail |
| 2026-07-23 | **Record both do and do-not** in this pad when we settle something | Decisions + Non-goals / explicit rejects |
| 2026-07-23 | Object/array: keep **dual create surface** — object/array creates for C; value creates when the Adaptive value face is needed | Not a value-only rewrite of AFW C |
| 2026-07-23 | **Do not** reintroduce generated “create empty managed object/array **as a value**” helpers | Abandoned #2 branch residue; not in tree; do not bring back |
| 2026-07-23 | Permanent empty **array of \<dataType\>** (`impl_empty_array_of_*`) stays | Unrelated to empty-object-value creates |
| 2026-07-23 | Objects/arrays (managed **and** unmanaged) will hold property/element **values of mixed lifetimes** | Rely on value inf `clone_or_reference` / `optional_release`; keep thinking as we implement — not only container→value identity |
| 2026-07-23 | Prefer **all** value-kind lifetime nastiness in `clone_or_reference` / `optional_release` (not in every container) | Includes non–data-type values; many still have NULL methods today |
| 2026-07-23 | Prefer **ship new paths early** when they do not break existing behavior; save big-bang switches for scope/assign | Object/array identity work can land incrementally |
| 2026-07-23 | Highest break risk: **scope lifetime + assign** (e.g. `afw_function_compiler_script.c`), not most object/array plumbing | xctx/clone wiring left conservative on purpose during large #2/scope work |
| 2026-07-24 | **Scope teardown intent:** on final `afw_xctx_scope_release`, release **each frame variable** (`optional_release` on `symbol_values[]`) then subpool — not pool-only forever | Design target (phase 2); not in mainline today; abandoned branch may have had it |
| 2026-07-24 | **Target model: `clone_or_reference` never returns unmanaged** — always permanent or managed | Core invariant; see **Value Lifetime Model (target)** |
| 2026-07-24 | **Stored slots** (scope, managed property/element, durable eval result) hold only permanent or managed values | Unmanaged = pool temporary / pre-promotion |
| 2026-07-24 | Unmanaged scalar escape → **managed clone** in dest pool | |
| 2026-07-24 | Unmanaged object/array escape → **managed wrapper** + pin `instance->p`; multiple wrappers OK; ≠ dual face | Helpers: `*_create_managed_wrapper` (names TBD) |
| 2026-07-24 | Managed container RC→0: **walk** properties/elements and release each, then destroy private subpool | Dual-face free rules still apply |
| 2026-07-24 | Property store **lazy** (store as-is after release old); property get **promotes** unmanaged via clone_or_reference into `container->p` and caches | |
| 2026-07-24 | Unmanaged containers keep classic pool-only content lifetime (no promote/release walk) | |
| 2026-07-24 | Adaptive **values** are immutable; object/array values reference a (possibly mutable) shared instance | |
| 2026-07-24 | Slices only of managed/permanent; large managed string/memory + slice for cheap assign is **later** | |
| 2026-07-24 | Classic-style pools (free no-op) for short-lived/request; subpools for script/long-running | Direction, not immediate code |
| 2026-07-24 | OOM / error paths must not leave unbalanced pool pins or half-promoted slots | |
| 2026-07-24 | Target model does **not** replace phased plan order; highest-priority pieces for long-run safety listed in target section | Continue 1d when ready |
| 2026-08-06 | **`afw_pool_release` returns** `const afw_pool_t *` — pool if still alive, **NULL** if this call destroyed it | Callers can free side holds without reading internal RC; void callers still valid |
| 2026-08-06 | **Managed object face** (`create_wrapper_*`): pin `wrapped` once at create; `release(wrapped)` only when face pool destroy (`pool_release` → NULL). Unmanaged face borrows | Object look-through needs base for face life; save `wrapped` before pool release |
| 2026-08-06 | **Array faces** not the same bug today (pool-owned instance, noop `release`, materialize ring). When arrays get real managed RC, mirror object pin + other #2 array work | Do not invent array pin early |
| 2026-08-06 | Removed unfinished **`create_composite`** / **`properties_callback`**; keep **`create_merged`**, **`aggregate_external`**, views / option composite | Faces are product look-through; multi-base aggregate is different |

### Open questions (need maintainer perspective when back)

**Phase 0 / bindings** — largely settled in 0a–0d; residual only if coding surfaces gaps.

**Containers / assign (phases 1–2)** — partly answered by **Value Lifetime Model (target)**; remaining:

- Exact API names for managed wrappers; array get_reference equivalent if still missing.
- 1d create matrix vs wrapper model: when create returns dual face vs when only clone_or_reference wraps.
- NULL `->value` hard error timing on create.
- Whether evaluate path already guarantees managed/permanent result (verify under valgrind when phase 2 lands).
- Closure residuals after core slot walk (follow-on).
- Classic pool restore vs subpool: which create paths use which (timing).

**Later**

- **`compiled_value` escaped** (script `compile` + return): pool RC / managed unit — design when we get there.
- Hard error on missing `object->value` / `array->value` — when to flip?
- Request memory limit: charge only request-session pool tree, or escaped managed too?
- `#35` after2-* unskip bar vs step-by-step #2 — no fixed bar yet.
- Large managed string/memory + slice (cheap cross-scope assign).

### Non-goals (near term)

- **Immediately** replacing hierarchical pools or ripping out APR (possible later; see APR section under Pools)
- Closing #49 / #127 inside #2 (related, tracked elsewhere)
- Hand-editing `generated/` bindings (change generator + regenerate)
- Full OOM/stack handling productization (**#64** adjacent) — noted as future; needs accounting first
- **Replacing** `afw_object_create*` / `afw_array_create*` with value-only constructors for ordinary C use (most object/array work never uses value methods)
- **Generated helpers** that create an empty managed object or array and return only `afw_value_t *` (old #2 experiments; **explicitly out** — see Decisions)
- Forcing every site that builds an object to go through Adaptive value create

### Phase 0 — `data_type_bindings` audit

**Sources:** `src/afw/generate/objects/_AdaptiveDataTypeGenerate_/*.json` (35 types), `_afwdev/generate/data_type_bindings.py`, sample generated `afw_data_type_*_binding.*`.  
**Never hand-edit** `generated/` — change JSON or generator, then rebuild.

#### Phase 0 plan slices

| Slice | Intent | Status |
|-------|--------|--------|
| **0a** | Type × lifetime matrix + generator decision tree | **done** |
| **0b** | Generator/header comments match behavior (clone_or_reference, RC=0, pointer managed) | **done** |
| **0c** | Semantic consistency only if clear (null create, unevaluated, …) | **done** (null + boolean create → permanents; unevaluated/function parked; allocate_* revisit later) |
| **0d** | Phase 1 handoff notes (object/array → `->value`) | **done** |
| **0e** | Verify after code changes | N/A for 0d (docs only); use when phase 1 codes |

**Phase 0 boundary:** bindings **correctness + docs**, not object/array create flip (that is phase **1**).  
**Phase 0 complete** once 0d is accepted — ready for phase 1 discuss/plan.

---

#### 0a — Generator decision tree

Metadata fields that drive emission (`_AdaptiveDataTypeGenerate_`):

| Field | Role |
|-------|------|
| **`special`** | If true: **permanent inf only** — no unmanaged/managed/managed_slice infs, no create/allocate APIs. |
| **`cType`** | Internal payload type; chooses embed vs pointer storage and managed_slice eligibility. |
| **`directReturn`** | If true, create/get APIs take/return `cType` by value (incl. pointers); else `const cType *` and memcpy into header. |
| **`scalar`** | Data-model flag (not a separate create path in the generator today). |

`_supports_managed_slice(cType)` is true only for **`afw_utf8_t`** and **`afw_memory_t`**.

```text
if special:
    permanent inf only
    (no create_managed / create_unmanaged / allocate_unmanaged / slice)
else:
    permanent + unmanaged + managed infs
    if cType in (afw_utf8_t, afw_memory_t):
        + managed_slice inf + create_managed_*_slice
    create_managed_*:
        if cType == afw_utf8_t:  own bytes after header (xctx_calloc)
        elif cType == afw_memory_t: own bytes after header
        elif directReturn:  store internal as-is (xctx_malloc); RC=0 explicit
            # pointer or small by-value types (bool, int, double, void*, object*, …)
        else:  memcpy *internal into header (xctx_calloc); RC 0 via calloc
    create_unmanaged_* / allocate_unmanaged_*: pool header, no RC
    managed optional_release: free header at RC 0 else decrement
    managed get_reference: bump RC, return same instance
    unmanaged clone_or_reference: return instance as-is  (comment historically wrong)
    permanent clone_or_reference: return instance as-is
    managed_slice: RC on containing managed; release frees slice header + containing policy
```

**No `create_permanent_*`** — permanents come from `strings.py` / `const_objects.py` / bindings (id values, empty arrays) / hand `afw_value.h`.

**Common contracts (all non-special managed):**

- **RC starts at 0** on create (calloc path zeros; malloc path sets `reference_count = 0`). First `optional_release` without a prior get_reference **frees the header**.
- **Managed release frees the value header only** — not nested object/array/function payloads.
- **`allocate_managed_*` does not exist** (historical cleanup); only `allocate_unmanaged_*`.

---

#### 0a — Create-path families (by generator branch)

| Family | cType pattern | directReturn | Managed create stores | Slice? | Types |
|--------|---------------|--------------|----------------------|--------|--------|
| **A. Special** | any | — | n/a (no create) | no | any, unknown, undefined, void |
| **B. utf8 owning** | `afw_utf8_t` | false | copy octets after header | **yes** | string, anyURI, dnsName, ia5String, ipAddress, json, objectId, objectPath, password, regexp, relaxed_json, rfc822Name, script, template, x500Name, xpathExpression |
| **C. memory owning** | `afw_memory_t` | false | copy bytes after header | **yes** | base64Binary, hexBinary |
| **D. Struct embed** | struct (`afw_date_t`, …) | false | memcpy struct into header | no | date, dateTime, dayTimeDuration, time, yearMonthDuration |
| **E. Small direct** | non-pointer scalar | **true** | by-value in header (`malloc` + RC=0) | no | boolean, integer, double |
| **F. Pointer direct** | `const T *` or `void *` | **true** | **bare pointer** in header; no clone of referent | no | object, array, function, unevaluated, **null** (`void *`) |

Family **F** is the structural problem for long-running MM: managed RC covers the **wrapper only**. Object/array need phase **1** (`->value` + container RC). Function / unevaluated / null are related edge cases (document; change only if clearly wrong in 0c).

---

#### 0a — Full type matrix (35)

Legend: **P** permanent inf · **U** unmanaged · **M** managed · **S** managed_slice · **Cm/Cu/Au** create_managed / create_unmanaged / allocate_unmanaged.

| dataType | special | scalar | directReturn | cType | P | U | M | S | Cm/Cu/Au | Family / notes |
|----------|---------|--------|--------------|-------|---|---|---|---|----------|----------------|
| any | yes | no | yes | `const afw_value_t *` | ✓ | — | — | — | — | A type-system |
| unknown | yes | no | yes | `const afw_value_t *` | ✓ | — | — | — | — | A type-system |
| undefined | yes | yes | yes | `void *` | ✓ | — | — | — | — | A singleton (`afw_value_undefined`) |
| void | yes | yes | yes | `void *` | ✓ | — | — | — | — | A type-system (return “no value”) |
| string | | yes | | `afw_utf8_t` | ✓ | ✓ | ✓ | ✓ | ✓ | B |
| anyURI | | yes | | `afw_utf8_t` | ✓ | ✓ | ✓ | ✓ | ✓ | B |
| dnsName | | yes | | `afw_utf8_t` | ✓ | ✓ | ✓ | ✓ | ✓ | B |
| ia5String | | yes | | `afw_utf8_t` | ✓ | ✓ | ✓ | ✓ | ✓ | B |
| ipAddress | | yes | | `afw_utf8_t` | ✓ | ✓ | ✓ | ✓ | ✓ | B |
| json | | yes | | `afw_utf8_t` | ✓ | ✓ | ✓ | ✓ | ✓ | B |
| objectId | | yes | | `afw_utf8_t` | ✓ | ✓ | ✓ | ✓ | ✓ | B |
| objectPath | | yes | | `afw_utf8_t` | ✓ | ✓ | ✓ | ✓ | ✓ | B |
| password | | yes | | `afw_utf8_t` | ✓ | ✓ | ✓ | ✓ | ✓ | B |
| regexp | | yes | | `afw_utf8_t` | ✓ | ✓ | ✓ | ✓ | ✓ | B |
| relaxed_json | | yes | | `afw_utf8_t` | ✓ | ✓ | ✓ | ✓ | ✓ | B |
| rfc822Name | | yes | | `afw_utf8_t` | ✓ | ✓ | ✓ | ✓ | ✓ | B |
| script | | yes | | `afw_utf8_t` | ✓ | ✓ | ✓ | ✓ | ✓ | B |
| template | | yes | | `afw_utf8_t` | ✓ | ✓ | ✓ | ✓ | ✓ | B |
| x500Name | | yes | | `afw_utf8_t` | ✓ | ✓ | ✓ | ✓ | ✓ | B |
| xpathExpression | | yes | | `afw_utf8_t` | ✓ | ✓ | ✓ | ✓ | ✓ | B |
| base64Binary | | yes | | `afw_memory_t` | ✓ | ✓ | ✓ | ✓ | ✓ | C |
| hexBinary | | yes | | `afw_memory_t` | ✓ | ✓ | ✓ | ✓ | ✓ | C |
| date | | yes | | `afw_date_t` | ✓ | ✓ | ✓ | — | ✓ | D |
| dateTime | | yes | | `afw_dateTime_t` | ✓ | ✓ | ✓ | — | ✓ | D |
| dayTimeDuration | | yes | | `afw_dayTimeDuration_t` | ✓ | ✓ | ✓ | — | ✓ | D |
| time | | yes | | `afw_time_t` | ✓ | ✓ | ✓ | — | ✓ | D |
| yearMonthDuration | | yes | | `afw_yearMonthDuration_t` | ✓ | ✓ | ✓ | — | ✓ | D |
| boolean | | yes | **yes** | `afw_boolean_t` | ✓ | ✓ | ✓ | — | ✓ | E; **create_* → permanent true/false** (intentional dual) |
| integer | | yes | **yes** | `afw_integer_t` | ✓ | ✓ | ✓ | — | ✓ | E; prefer `afw_integer_v_zero/one` |
| double | | yes | **yes** | `double` | ✓ | ✓ | ✓ | — | ✓ | E |
| object | | no | **yes** | `const afw_object_t *` | ✓ | ✓ | ✓ | — | ✓ | **F — phase 1** double-wrap |
| array | | no | **yes** | `const afw_array_t *` | ✓ | ✓ | ✓ | — | ✓ | **F — phase 1** double-wrap |
| function | | no | **yes** | `const afw_value_t *` | ✓ | ✓ | ✓ | — | ✓ | F; env uses permanent function values |
| unevaluated | | no | **yes** | `const afw_value_t *` | ✓ | ✓ | ✓ | — | ✓ | F; keep create APIs (compile graphs); not special |
| null | | yes | **yes** | `void *` | ✓ | ✓ | ✓ | — | ✓ | F; **create_*_null → `afw_value_null`** (0c); allocate still pool header |

Empty cells for special/scalar/directReturn mean **false** / absent.

---

#### 0a — What already makes sense

| Area | Assessment |
|------|------------|
| Four lifetime policies | Present and wired for non-special types |
| Special types | Permanent-only fits type-system / singleton roles |
| B/C owning managed | Value owns bytes; slice on containing RC looks coherent |
| D/E by-value managed | Header owns payload copy; good long-running story |
| No create_permanent_* | Correct |
| allocate_managed_* gone | Correct; allocate_unmanaged remains |
| Old `False and` object path | Gone from generator |

#### 0a — Gaps / anomalies (for 0b–0d and phase 1)

| # | Issue | Phase |
|---|--------|-------|
| 1 | **object/array** create still allocate wrapper; release does not release container; ignore `->value` | **1** (document target in 0d) |
| 2 | Managed create almost **unused** in hand C (unmanaged + pool bulk free dominates) | 1–2 use |
| 3 | Family **F** pointer managed: RC = header only | 1 for object/array; note function/unevaluated |
| 4 | Managed **RC starts at 0** — documented in generator + headers (**0b**) | keep contract |
| 5 | **null** not special; **create_managed/unmanaged_null → `afw_value_null`** (0c); allocate still non-singleton if used | done |
| 6 | **unevaluated** full create APIs | **parked** — type for compile results; not special |
| 7 | Unmanaged/permanent/managed clone_or_reference comments matched behavior (**0b**) | done |
| 8 | Pointer managed: create Doxygen notes header-only RC / no referent clone (**0b**) | phase 1 for object/array |

#### Relative to old branch (ideas for phase 1 only)

| Old tip | Current tree |
|---------|----------------|
| create_*_object → `internal->value` | Still allocate wrapper |
| managed get_reference → object | Only bumps value-header RC |
| managed release → object_release | Only free value header |
| drop allocate_unmanaged_object | Still generated |
| throw if missing `->value` | N/A |

#### What’s “complete enough” for phase 0 vs later

| In phase 0 | Later |
|------------|--------|
| Matrix (0a) + comment/contract fixes (0b) | Flip object/array to `->value` + container RC |
| Clear small consistency only if safe (0c) | Migrate call sites; assign; escape |
| Phase 1 handoff notes (**0d done**) | Scope release; OOM |

#### 0d — Phase 1 handoff (object / array / `->value`)

**Purpose:** Freeze intent so phase 1 coding does not re-litigate. Old branch `use-value-in-array-and-object` = ideas only, not merge.

##### Invariants

1. **Two faces, one instance.** `afw_object` / `afw_array` methods stay the C workhorse; Adaptive script/eval uses **`afw_value`**. Public instance fields include `p` and **`value`** (lifetime of the Adaptive value = lifetime of the instance — already in interface docs).
2. **One Adaptive value per instance when `->value` is set.** Prefer that pointer; do not allocate a second value header that only points at the same object/array (**double-wrap** is the bug).
3. **Dual create surface (keep both):**

| API class | Returns | Audience |
|-----------|---------|----------|
| `afw_object_create*` / `afw_array_create*` | object/array | Default C construction (most call sites never use value methods) |
| `afw_value_create_*_object` / `*_array` | value | Value graph: functions, eval, set_property of object/array values |
| Instance `->value` | value face | Bridge; filled by impls (memory, many others); audit remaining NULL |

4. **Value create is about an existing instance**, not a second factory for “empty managed object as value only.”

##### Target behavior (value create path)

| Situation | Intended behavior |
|-----------|-------------------|
| Instance has `->value`, managed or permanent | **Return `->value`**; get_reference / container policy as needed — **no new wrapper** |
| Instance has `->value`, unmanaged storage | Prefer return `->value` when lifetime matches; else see wrap |
| **Catch-22:** unmanaged object/array, caller wants **managed** value | Limited **wrap** allowed: managed value header whose RC holds object/pool reference so storage outlives escape — lifetime adapter, not routine double-wrap of already-managed instances |
| `->value` is NULL | Policy to harden in phase 1: inventory first; prefer **require fill** for memory path; hard error vs temporary wrap TBD when coding |
| Permanent / const objects | Already have permanent values; do not break env/const registration |

**Release / clone (single model):** map value `optional_release` / `clone_or_reference` to **object/array** get_reference/release (or the pool policy the impl already uses) — **one** story, not mixed “header free only” + ad hoc object_release. Generic managed scalar “RC starts at 0 / free header” **does not apply** to object/array once special-cased.

##### Permanent empty array **values** vs compile `[]` / `{}`

| Mechanism | Role |
|-----------|------|
| **`impl_value_empty_array_of_<dataType>`** / `data_type->empty_array_value` | Permanent typed empty array **value** on each data type. Used by Adaptive functions (e.g. higher-order array, polymorphic) for cheap typed empty results. **Keep.** |
| **`impl_empty_array_of_<dataType>`** | Matching permanent empty **array** instance. **Keep.** |
| Compile **`[]` / `{}`** | Scripts usually **mutate** (add elements/properties). Permanent const empty was more trouble than worth → compile builds **fresh mutable** empties (`afw_compile_parse_value.c` and related). Imperfect “dance” may exist; **out of scope** for #2 phase 0/1. Revisit later under compile notes if needed (#110 if shared mutables return as defaults). |
| Permanent empty **object** singleton | **Not** generated today. Only reconsider with immutability / clone-default story. |

##### Order of work (phase 1)

1. Inventory object/array **infs**: who sets `pub.value`, who leaves NULL.  
2. Special-case **object** (then **array**) in value create / release / get_reference (generator and/or hand helpers).  
3. Ensure memory (and core) paths always fill `->value`.  
4. Migrate hot **value** call sites off redundant `create_unmanaged_object` when instance already has value.  
5. Tests: identity (same instance → same value pointer); multi-request / valgrind.  
6. **Do not** require rewriting all C that only holds `afw_object_t *`.

##### Explicitly do not (phase 1 and near term)

| Do not | Why |
|--------|-----|
| Merge old `use-value-in-array-and-object` wholesale | Ideas only |
| Reintroduce generated empty managed object/array **value** creates | Abandoned; no residual in `data_type_bindings.py` (checked 2026-07-23); use object create + `->value` |
| Force all object construction through value create | Dual surface |
| Rewrite compile `[]`/`{}` mutable empty dance | Separate compile topic |
| “Always permanent const `[]`/`{}`” without revisiting mutation | Tried; not worth it as default |
| Pull function/unevaluated into first object slice | Related family F; later |
| Solve allocate_* globally | Noted for later revisit |
| Assign/scope escape / OOM | Phases 2–4 |

##### Success criteria (phase 1 “enough”)

- `afw_value_create_*_object` (and array when in scope) prefer instance `->value` for normal memory/managed cases.  
- No routine double-wrap of instances that already have a value.  
- Managed escape/release tied to container policy for those cases.  
- Dual create APIs still documented and used as above.  
- Core tests + valgrind smoke green.

##### Risks

- Double free if both header RC and object release fire.  
- Missing `->value` on some infs.  
- Unmanaged memory object still embedding **managed** value inf (current memory create) — reconcile when coding.  
- #110: permanent/shared mutables as defaults must clone when returned mutable.  
- Array behind object (list heritage); expect second vertical slice.  
- **Mixed lifetimes inside containers:** properties/elements may be permanent, managed, unmanaged, slice, … on both managed and unmanaged objects/arrays. Container identity (`->value`) is only half the story; set_property / array add / clone / release must use each value’s lifetime methods correctly (phases 1–2+). Expect ongoing design checks, not a one-time fix.

##### Mixed value lifetimes inside objects/arrays (ongoing)

As MM work proceeds, real objects and arrays will keep holding **values with all kinds of lifetimes** (permanent, managed, unmanaged, slice, nested object/array values, **and non–data-type values** such as closures, calls, compiled graphs, …) whether the **container** itself is managed or unmanaged.

**Intended architecture:** keep the nastiness in **`afw_value` methods**, especially:

- **`clone_or_reference`** — store / escape / outlive a pool  
- **`optional_release`** — drop a held value  

Containers (set_property, array add, scope assign, …) should call those and **not** special-case every value kind. Data-type bindings already generate permanent/managed/slice/unmanaged clone+release; **hand value kinds** need the same discipline over time.

**Still think through** when coding:

- Unmanaged container holding a **managed** value (or the reverse).  
- Permanent / shared values as properties (#110).  
- Nested object/array values and double-wrap vs `->value`.  
- Container clone deep-walking via `clone_or_reference`.  
- Release order vs subpool bulk free.  
- **Non-DT values** with clone/release still **NULL** (most expression/compile kinds today).  
- Call sites that still avoid `afw_value_clone_or_reference` (e.g. xctx FIXME).

Phase 1 focuses on **container as Adaptive value**; mixed-content + full value-kind clone/release spans **1–2+** (and #35 closures).

##### Value implementations inventory (beyond data types)

Discovery: `afw_value_impl_declares.h` + `AFW_IMPLEMENTATION_ID` (same multi-include caveats as object). ~**132** include sites / **52** files (2026-07-23 pass).

**A. Generated data-type value infs** (~115 sites in `*_binding.c`)

| Kind | clone_or_reference (typical) | optional_release |
|------|------------------------------|------------------|
| permanent_* | as-is | NULL |
| unmanaged (bare id) | as-is | NULL |
| managed_* | bump RC | free header at RC 0 |
| managed_slice_* | new slice + containing RC | slice + containing |

**B. Hand value kinds** (`src/afw/value/*.c`, ~17 ids)

| IMPLEMENTATION_ID | clone / release today | Notes for property/array storage |
|-------------------|----------------------|----------------------------------|
| **closure_binding** | **Real** RC + scope get_reference/release | Escape model; nastiness stays in value methods |
| compiled_value | both **NULL** | Compile pool; escape open |
| block, call, call_*, list/object_expression | both **NULL** | Graph nodes; usually evaluate before long hold |
| symbol_reference, qualified_variable_reference, reference_by_key, assignment_target | both **NULL** | Scope/structure refs |
| function_definition, function_thunk, script_function, template_definition | both **NULL** | Often env/compile-owned |

Many hand kinds `#define impl_afw_value_clone_or_reference NULL` (and release). Call macros invoke the inf slot — **NULL is unsafe to call**. Goal: implement real methods on kinds that can be stored/escaped, rather than teaching every container about that kind.

**C. Permanent object/array values** (const_objects, `empty_array_value`, function meta) use permanent DT infs — covered by A + generate.

**D. Target container pattern:**

```text
held = afw_value_clone_or_reference(incoming, /* pool/policy */, xctx);
// on replace/remove: afw_value_optional_release(old, xctx);
```

Special cases (closure scope, managed object container RC once 1b lands, compiled_value later, …) live **inside** that value’s clone/release.

##### Open for phase 1 plan (not blockers for 0d)

- Hard error vs temporary wrap when `->value` is NULL.  
- Exact wrap mechanics for unmanaged → managed value (pool RC vs object get_reference).  
- Whether unmanaged create of value ever still allocates a header when `->value` exists but lifetime mismatch.

#### Phase 1 plan (multi-step; objects first)

**Shape:** spine (value create + memory object) then sweep impls and hot value call sites.  
**Not one PR.** Dual create surface from 0d stands: keep `afw_object_create*` / `afw_array_create*`; fix value path and `->value` fill-in.

| Step | Intent | Status |
|------|--------|--------|
| **1a** | **Inventory** object/array impls, `->value`, create counts | **done** |
| **1b′** | **Finish `instance->value` on all object *and* array impls** with correct lifetime inf (value required; no create-policy special cases yet) | **done** (`ff5bbbf5`) |
| **1c** | Memory managed/unmanaged **options** fully aligned with value face + container-aware release/clone (if not finished in 1b′) | **done** |
| **1d** | **Value create policy** for object (then array): face identity + managed wrapper for unmanaged; dual surface unchanged | **plan revised** (target model; discuss 1d.0) |
| **1e** | Hot call-site cleanup only | pending |
| ~~1f array mirror~~ | Folded into **1b′** / **1d** | — |

**Out of phase 1:** compile `[]`/`{}` dance, allocate_* global revisit, assign/escape, function/unevaluated container special-case.

**Superseded experiment:** permanent-only `create_*_object` identity (uncommitted 1b draft) — **do not land** before 1b′; avoid temporary create branches we reverse later.

##### 1b′ plan — `object->value` / `array->value` complete and consistent

**Goal:** After 1b′, every live **object** and **array** instance has:

1. **`pub.value != NULL`**  
2. **`value` points at the Adaptive value for that instance** (embedded or paired; `value.internal` ↔ instance for object/array data types)  
3. **Value inf matches ownership story** for that instance (permanent / managed / unmanaged — intentional, documented per impl)

**Why before create policy (old 1b):** create can then assume the face exists and only ask *which lifetime*, not *whether value is missing*. Avoids NULL/permanent-only special cases that must be torn out later.

**Non-goals for 1b′:**

- Changing `afw_value_create_managed/unmanaged_object|array` matrix (that is **1d**)  
- Full assign/scope / clone_or_reference call-site rollout  
- Compile `[]`/`{}`  
- Perfect deep property lifetime (mixed contents still use value methods later)

---

###### Lifetime inf guidance (instance setup)

| Instance kind | Typical value inf | Notes |
|---------------|-------------------|--------|
| Const / static / generate const_objects | `permanent_object_inf` / permanent array | Already mutual; verify all paths |
| Empty typed array (`empty_array_of_*`) | `permanent_array_inf` | Already OK |
| Memory object **managed** (default `afw_object_create`) | `managed_object_inf` | Already set; keep |
| Memory object **unmanaged** | Prefer **unmanaged** face once agreed — *today often still managed inf* | Align in 1b′ or early 1c; do not leave a silent lie if create will trust the face |
| Memory array | `managed_array_inf` (current) | Same managed/unmanaged alignment as object if options exist |
| Runtime indirect (`afw_runtime_object_create_indirect*`) | unmanaged value in pool | Ensure always `pub.value` set (mostly yes) |
| Meta object (`afw_object_meta_object_s`) | own embedded value + `pub.value` | Same dual-face rule; see meta track |

**Invariant after create of any impl:** `instance->value != NULL` and `/* object */ ((afw_value_object_t *)instance->value)->internal == instance` (or array equivalent), unless a rare documented exception (prefer none).

---

###### Work packages (execute in order)

**1b′.0 — Freeze rules in pad (this section)**  
Agree inf table above (especially unmanaged memory object face). Tweak before coding if needed.

**1b′.1 — Object: fix known gaps (1a hit list)**  

| Target | Issue | Action |
|--------|--------|--------|
| `value_meta` | Side `meta_object_value` but **`pub.value` not set** | Set `self->pub.value = &self->meta_object_value.pub` at create; inf already unmanaged |
| `afw_runtime_const_meta` | Inf-only file | Audit **all** instance constructors that use this inf; ensure each sets `value` (const_objects / runtime helpers) |
| Any other `object_impl_declares` without value | 1a | Grep + fix |
| Skeleton closet | template only | No production fix required |

Also re-grep for `afw_object_impl_declares.h` and for object factories **without** declares (runtime) so nothing is missed.

**1b′.2 — Meta objects**  

- Inventory: `afw_object_meta.c`, `afw_object_meta_accessor.c`, `get_nonempty_meta_object`, const meta on const objects.  
- Ensure every meta **object instance** used as `meta.meta_object` has non-NULL `->value` with correct inf.  
- Helpers stay in `afw_object_meta.h`; no API redesign.

**1b′.3 — Array: same pass**  

| Target | Action |
|--------|--------|
| memory, const_array, view_of_c_array, meta_values_* | Confirm `pub.value` always set (1a: mostly yes) |
| Any gap or wrong inf | Fix |
| Permanent empty arrays | Confirm still permanent + mutual (no regression) |

**1b′.4 — Memory managed vs unmanaged face (if not done in 1b′.1)**  

- When `AFW_OBJECT_MEMORY_OPTION_unmanaged`, set value inf to **unmanaged_object** (or agreed policy) and document.  
- Managed option keeps **managed_object**.  
- Same idea for arrays if they have parallel options.  
- **Do not** yet change create_*_value matrix; only instance birth.

**1b′.5 — Smoke / invariants**  

- Optional debug assert helper (or test-only): `object->value && object_value->internal == object` (and array).  
- `./afwdev build --cdev`; `afwdev test -j` (and valgrind if release/inf changes bite).  
- Manual: const object, `afw_object_create`, `afw_object_create_unmanaged`, empty array permanent, one meta path.

**1b′.6 — Pad update**  

- Mark each impl row in 1a table: value OK / fixed.  
- Note remaining exceptions (hope: none).  
- Explicit: create policy still **1d**.

---

###### Suggested commit slices (when executing)

1. Object gaps (value_meta + any NULL)  
2. Meta object value fill  
3. Array gaps (if any)  
4. Memory unmanaged face alignment (if separate)  
5. Pad / assert notes  

Each slice: build + tests; no create_*_object behavior change until 1d.

---

###### Exit criteria for 1b′

- [ ] Every production object/array impl (and meta object instances) sets non-NULL `->value` at construction.  
- [ ] Value inf matches agreed ownership table for managed/unmanaged/permanent.  
- [ ] Mutual identity: value’s internal points at the instance (object/array data type values).  
- [ ] Tests green; no create-policy experiment required for green.  
- [ ] 1a inventory table updated; **ready for 1d** without NULL branches.

---

##### 1d plan — value create policy (object then array) — **discuss**

**Status:** revised plan for discussion (not coding).  
**Depends on:** 1b′ + 1c **done**.  
**Compass:** **Value Lifetime Model (target)** (parallel brainstorm) **overrides** the earlier 1d draft where they conflict.  
**Not a full target landing:** 1d is still a **create-API** slice. Assign/scope walks, property promote-on-get, and unmanaged inf `clone_or_reference` rollout are **phase 2+** unless a thin shared helper is needed for create_managed.

###### What the target model changed about 1d thinking

| Earlier 1d draft (partially wrong / incomplete) | Target-aligned 1d |
|-------------------------------------------------|-------------------|
| Treat create identity as the whole escape story | **Create** = get a value face for an instance (borrow or owning). **Escape** = `clone_or_reference` (never returns unmanaged). |
| “Always return dual face” for every create path | Return dual face when it is the **right face** for that create kind; use a **managed wrapper** when caller needs managed escape over an **unmanaged** instance |
| Catch-22 wrap ≈ today’s heap header + `object_get_reference` | For **unmanaged** instances, `object_get_reference` is a **no-op** today → false hold. Target: wrapper **pins `instance->p`** (pool RC). Dual face stays separate; **multiple wrappers OK** |
| create_unmanaged returning managed dual face is “the” fix for double-wrap | Still correct for **stopping double-wrap** of managed instances; it is a **borrow of dual face**, not a managed create. Callers must not treat API name as “inf is unmanaged” |

###### Goal (1d only)

1. Stop **routine double-wrap** when `internal->value` already exists.  
2. Make **`create_managed_*` / `create_unmanaged_*`** for object (then array) face-aware and ownership-honest.  
3. Align **unmanaged instance + managed create** with the target **managed wrapper** idea (pool pin), not a fake object_get_ref.  
4. Leave dual create surface (`afw_object_create*` / `afw_array_create*`) unchanged.

###### Reality check (call sites)

| API | ~hand C | Role |
|-----|---------|------|
| `create_unmanaged_object` | ~97 | Dominant “wrap instance as value” path |
| `create_managed_object` | ~0 | Rare; still define correctly for escape / tests |
| `create_unmanaged_array` | ~60 | Same pattern |
| `create_managed_array` | ~0 | Same |

Hot bug today: managed memory object already has dual face → `create_unmanaged_object` still allocates a **second** unmanaged header in `p`.

###### Two tools (do not conflate)

| Tool | What it is | Typical use |
|------|------------|-------------|
| **Dual face** (`instance->value`) | Embedded (or permanent) value for the instance | Identity for that instance’s Adaptive face; managed dual face holds via object/array (1c) |
| **Managed wrapper** | Separate managed value header pointing at instance; **pins `instance->p`** for unmanaged instances | Escape / managed create when dual face is unmanaged or missing as an owning face |

`clone_or_reference` on unmanaged object/array infs (target) should eventually call the same wrapper helper. **1d** may introduce the helper for `create_managed_*` first; wire unmanaged inf’s `clone_or_reference` fully in **phase 2** if we want smaller 1d.

###### Decision matrix (target-aligned)

Assume production instances: non-NULL `internal`, non-NULL `internal->value` after 1b′.  
**Legend:** face = `internal->value`; P/M/U = permanent / managed / unmanaged **value inf**.

##### `afw_value_create_unmanaged_object(internal, p, xctx)`

| Face | Action | Ownership |
|------|--------|-----------|
| **P** | return `face` | Borrow; release no-op |
| **U** | return `face` | Borrow; release no-op; lifetime = instance pool hierarchy. **OK if face storage is not in `p`** — unmanaged create is not “allocate header in p,” it is “value with no value-RC” |
| **M** | return `face` | **Borrow of managed dual face** — **no** get_ref. Stops double-wrap. Caller of *unmanaged* create must **not** `optional_release` unless they separately got a hold. |
| NULL face | **Throw** | 1b′ complete for core; throw finds stragglers |
| NULL internal | **Throw** | Match 1c managed create |

API name = **create path** (no pool pin, no value own), **not** “result inf is unmanaged.”

##### `afw_value_create_managed_object(internal, xctx)`

| Face | Action | Ownership |
|------|--------|-----------|
| **P** | return `face` | No get_ref |
| **M** | `afw_object_get_reference(internal)` then return `face` | Owning hold on dual face; pairs with 1c managed release. **No second header.** |
| **U** | **Managed wrapper** in dest pool (`xctx->p` or agreed pool): points at instance; **`afw_pool_get_reference(internal->p)`** (not object_get_ref no-op); managed inf; value RC 0 / create own rules | Owning managed value; release drops pool pin + frees wrapper header. **Not** dual face. Multiple wrappers OK. |
| NULL face / internal | **Throw** | |

Prefer a shared helper e.g. `afw_object_create_managed_wrapper(internal, p, xctx)` used by create_managed (U) and later by unmanaged `clone_or_reference`.

##### Array (parallel)

Same matrix. Memory arrays use **unmanaged** dual face (1c) → `create_managed_array` almost always **wrapper + pin `array->p`**.  
No `afw_array_get_reference`: managed dual face rare; wrapper path is pool pin only (same as unmanaged object wrapper).  
If a true managed array dual face exists later, identity + array hold when API exists.

###### Explicitly out of 1d

| Out | Phase |
|-----|--------|
| Scope assign + slot walk | **2** |
| Managed container property walk on RC→0 | **2/3** |
| Property get promote-on-read | **2** |
| Unmanaged scalar `clone_or_reference` → managed clone (all DTs) | **2** (or with scalar work) |
| Mass replace call sites with `obj->value` | **1e** |
| Compile `[]`/`{}`, classic pool restore | later |
| Large managed string/slice | later |

###### Ownership protocol (1d)

1. **create_unmanaged** → non-owning face borrow (P/U/M dual face).  
2. **create_managed** + M dual face → owning object hold.  
3. **create_managed** + U instance → owning **wrapper** (pool pin).  
4. Never free embedded dual face (1c).  
5. OOM creating wrapper: no pool pin left unbalanced.

###### Work packages

| Slice | Work | Exit |
|-------|------|------|
| **1d.0** | Agree this plan (overrides old 1d draft); freeze matrix | Discuss sign-off |
| **1d.1** | `create_unmanaged_object`: identity for P/U/M face; throw if !internal / !value; build + test | Green; no double-wrap for managed memory objects |
| **1d.2** | `create_managed_object`: P/M identity; U → managed wrapper helper + pool pin; release path correct for wrapper | Green |
| **1d.3** | Array create unmanaged/managed same rules | Green |
| **1d.4** | Generator comments + pad: create vs clone_or_reference vs wrapper | Docs match |
| **1d.5** | Optional: skim grep for `optional_release` after create_unmanaged; fix only true hazards | Safe enough for 1e |
| **1d.6** | Mark 1d done; note phase-2 hook: unmanaged inf `clone_or_reference` → same wrapper helper | Ready for **1e** or **2** |

Micro-land if needed: 1d.1 permanent-only first (known green), then U/M identity.

###### Tests / proof

- `afwdev test -j` full suite.  
- Spot: managed `afw_object_create` → `create_unmanaged_object` pointer **eq** `obj->value`.  
- Spot: unmanaged object → `create_managed_object` → distinct wrapper; release does not free instance early; pool pin balanced.  
- Watch: over-release if someone optional_releases create_unmanaged result that is managed dual face.

###### Old open questions — answered by target (overrides prior)

| Q | Answer for 1d |
|---|----------------|
| create_unmanaged + managed face → identity? | **Yes** — borrow dual face; stops double-wrap |
| NULL `->value`? | **Throw** |
| Face not in caller `p`? | Identity still OK for unmanaged create |
| Managed create + permanent? | Return face as-is |
| create_managed + unmanaged array/object? | **Wrapper + pin `->p`**, not dual face, not no-op get_ref |
| Object then array? | Prefer object first if bisection helps; generator can do both once object green |
| 1e in 1d? | Generator-only unless grep finds hard over-release |

###### Success criteria

- [ ] No routine double-wrap when face exists (unmanaged create returns face).  
- [ ] create_managed: P/M → face (+ hold for M); U → managed wrapper with real pool pin.  
- [ ] NULL internal/face → throw.  
- [ ] Dual object/array create APIs unchanged.  
- [ ] Tests green; pad marks 1d done; phase 2 still owns assign/scope/clone_or_reference completeness.

###### Risks

- create_unmanaged → managed dual face + mistaken optional_release → object over-release (grep).  
- Implementing wrapper without shared helper → duplicate logic when phase 2 wires clone_or_reference.  
- Scope still stores unmanaged creates until phase 2 — **accepted incremental debt**; 1d does not fix assign.  
- Array pool pin only — correct under target until array get_reference exists.

###### Relationship to experiments / 1c

- Permanent-only identity: proven subset of 1d.1.  
- Naive always-return-face without 1c: SEGV; **1c enables** M dual face identity on managed create.  
- Target model: **do not** treat dual-face return as substitute for managed wrapper on unmanaged instances.

---

###### Risks (1b′)

- Touching value_meta/meta may surface callers that assumed missing `pub.value`.  
- Changing unmanaged memory to unmanaged value inf may reveal callers that assumed managed_object inf on all memory objects — good to find now.  
- Const meta consumers outside generate — easy to miss; grep for `afw_runtime_inf_const_meta` / `const_meta_object`.

##### Rollout posture (how we turn things on)

- **xctx / clone_or_reference:** conservative or incomplete call sites (including FIXMEs) were intentional so scopes and other #2 work could land without requiring the full value-lifetime matrix to be live everywhere. That is **not** “clone_or_reference is abandoned.”
- **Goal:** get **new correct paths called as soon as they are safe**, without breaking current production behavior. Much of **object/array** work (fill `->value`, value-create return existing value when present, permanent paths) can ship **incrementally** and stay green.
- **Likely hard switch later:** when **scope lifetime** and **assign** paths fully rely on clone/release (notably script/compiler assign surfaces such as `afw_function_compiler_script.c`). That is when incomplete value-kind methods and xctx wiring become load-bearing.
- **Dev technique (for later):** optional `ifdef` (or similar) around a “new lifetime regime” so local builds can define a symbol while developing, then remove the gate before commit — only when a feature really needs a kill switch; prefer always-on safe deltas when possible.

##### 1a — How to find implementations

AFW interface impls almost always pull in a generated **`afw_<interface>_impl_declares.h`** after setting **`AFW_IMPLEMENTATION_ID`** (and related defines). That is the reliable discovery hook:

```text
grep -rln 'afw_object_impl_declares.h' src --include='*.c'   # files with ≥1 object impl
grep -rln 'afw_array_impl_declares.h'  src --include='*.c'   # files with ≥1 array impl
# Prefer counting / listing each include site, not only unique files:
grep -rn 'afw_object_impl_declares.h' src --include='*.c'
```

Typical preamble:

```c
#define AFW_IMPLEMENTATION_ID "memory"
#include "afw_object_impl_declares.h"
```

**Inventory caveats (do not under-count):**

1. **One `.c` file can implement multiple interfaces** (e.g. object + setter, or object + array helpers in the same unit). Grep for each `*_impl_declares.h` of interest, not only object.
2. **The same `*_impl_declares.h` can be included more than once in one file**, with **`#define` / `#undef` of `AFW_IMPLEMENTATION_ID` (and related macros) between includes**, when that file provides **multiple implementations of the same interface** (different infs / implementation ids). File-level unique grep undercounts; walk each include site and its surrounding defines.
3. Also check: `pub.value =` assignments, runtime/const_objects paths, and `afw_value_create_*_object` / `*_array` call sites.  
4. `afw_object.h` / `afw_array.h` = public creates and helpers — **not** the full impl list.

**1a deliverable:** table per **implementation** (path, `AFW_IMPLEMENTATION_ID`, which interface, value filled Y/N, notes) — one row per include/id, not one row per file — + NULL-value hit list + rough create_*_object call counts. Pad only unless gaps need code in a later step.

##### 1a results (2026-07-23)

**Method:** `grep` for `afw_object_impl_declares.h` / `afw_array_impl_declares.h` include sites; `AFW_IMPLEMENTATION_ID`; `pub.value` assign **or** static `afw_object_t` / `afw_array_t` initializer (3rd field is `value`); value inf symbols; non-generated `afw_value_create_*_object/array` counts. Skeleton closet files ignored.

**How `value` is filled (do not miss #2):**

| Pattern | Example |
|---------|---------|
| Runtime assign | `self->pub.value = (const afw_value_t *)&self->value;` |
| Static struct init | `afw_object_t` / aggregate: `.inf, .p, .value, .meta` — e.g. env registry `impl_current_object` |
| External factory | `afw_runtime_object_create_indirect_using_inf` allocates value + sets `obj->pub.value` |
| Gap | Object exists but `pub.value` never set; value may live only as a side struct (e.g. value_meta) |

**No file today multi-includes `afw_object_impl_declares.h` twice** (multiple object infs in one `.c`). Several files **do** multi-interface: object + `afw_object_setter_impl_declares.h` (memory, meta, property_meta, value_meta).

###### Object implementations (`afw_object_impl_declares.h`)

| IMPLEMENTATION_ID | Source | `value` on instance? | Embedded / paired value inf (typical) | Notes |
|-------------------|--------|----------------------|----------------------------------------|--------|
| `memory` | `object/afw_object_memory.c` | **Y** (create + embedded) | `managed_object_inf` always | Workhorse; also object_setter. Unmanaged option still uses **managed** value inf. Faces via `wrapped`. |
| `afw_object_view` | `object/afw_object_view.c` | **Y** | managed (+ unmanaged symbol present) | ParentPaths / object-option composite (not the removed `create_composite` API) |
| `afw_object_aggregate_external` | `object/afw_object_aggregate_external.c` | **Y** | managed_object | Live multi-object read merge; used by `afw_command` local server request props |
| `afw_object_const_key_value` | `object/afw_object_const_key_value.c` | **Y** | managed_object | Const-ish data, managed value inf |
| `object_meta` | `object/afw_object_meta.c` | **Y** | managed_object | + setter |
| `afw_object_meta_accessor` | `object/afw_object_meta_accessor.c` | **Y** | managed_object | |
| `object_impl_property_meta` | `object/afw_object_impl_property_meta.c` | **Y** | managed (+ unmanaged ref) | + setter |
| `environment_variables` | `environment/afw_environment_variables_object.c` | **Y** | managed_object | |

**Removed (2026-08, cleanup branch):** unfinished `composite` (`afw_object_create_composite`) and `properties_callback` (`afw_object_create_properties_callback`) — no callers; product mutable look-through is memory faces (`create_wrapper_*`). Keep **`afw_object_create_merged`** (eager copy-merge; used by actions).
| `fcgi_request_properties` | `afw_server_fcgi/..._properties_object.c` | **Y** | managed_object | Outside `src/afw/` core |
| `afw_environment_registry` | `environment/afw_environment_registry_object.c` | **Y** (static init, not `pub.value=`) | **permanent_object_inf** | Permanent singleton path; easy to miss if only grepping assigns |
| `afw_runtime_const_meta` | `runtime/afw_runtime_const_meta.c` | **Not on create in this file** | inf only; instances elsewhere | Const meta **inf**; instance `value` must be set by whoever builds the meta object (const_objects / runtime). Audit consumers in 1d. |
| `value_meta` | `value/afw_value_meta.c` | **N on `pub`** | Builds `meta_object_value` with **unmanaged_object_inf** but does **not** set `self->pub.value` | Callers use `&self->meta_object_value.pub` as the value; instance `->value` gap for 1d |

**Related (not object_impl_declares but object instances):**

| Path | Role |
|------|------|
| `runtime/afw_runtime.c` `afw_runtime_object_create_indirect_using_inf` | Sets `pub.value` to pool-allocated **unmanaged_object** value |
| `generated/afw_const_objects.c` (+ peers) | Permanent const objects + permanent object values |
| Extensions | Only **fcgi** properties object found with object_impl_declares under `src/afw_*` in this pass; adapters may use memory/runtime objects rather than custom object infs |

###### Array implementations (`afw_array_impl_declares.h`)

| IMPLEMENTATION_ID | Source | `value`? | Value inf (typical) | Notes |
|-------------------|--------|----------|---------------------|--------|
| `memory` | `array/afw_array_memory.c` | **Y** | managed_array | Workhorse |
| `afw_array_const_array_of_values` | `array/afw_array_const_array.c` | **Y** | (const path) | |
| `afw_array_view_of_c_array` | `array/afw_array_view_of_c_array.c` | **Y** | points at passed/own value | Used by permanent empty arrays |
| `afw_value_meta_values_list` | `value/afw_value_meta_values_list.c` | **Y** | | Meta as array |
| `afw_value_meta_values_object` | `value/afw_value_meta_values_object.c` | **Y** | | |

###### Two different “managed” stories (do not conflate)

**A. Memory object instance options** (`afw_object_create_with_options` in `afw_object.h`):

| Option / macro | Meaning |
|----------------|---------|
| **`AFW_OBJECT_MEMORY_OPTION_managed` (value `0`)** | **Default.** Subpool of `p`; object `get_reference` / `release` control that pool’s lifetime. |
| **`AFW_OBJECT_MEMORY_OPTION_unmanaged`** | No object-level RC / no release of associated pool via object methods. |
| **`AFW_OBJECT_MEMORY_OPTION_managed_cede_p`** | Object takes ownership of passed `p` (not a new subpool); RC still via object release. |
| **`afw_object_create(p,xctx)`** | → `create_with_options(**managed**, …)` |
| **`afw_object_create_unmanaged`** | → unmanaged |
| **`afw_object_create_cede_p`** | → managed_cede_p |
| **`afw_object_create_embedded`** | Child in embedder’s pool / entity lifetime |

Rough **hand C** call counts (exclude `generated/`; macros count as uses):

| Call | ~Count | Lifetime class |
|------|--------|----------------|
| `afw_object_create` | **~72** | **Managed object** (default option 0) |
| `afw_object_create_unmanaged` | **~48** | Unmanaged object |
| `afw_object_create_embedded` | **~36** | Embedded (entity-managed) |
| `afw_object_create_cede_p` | **~4** | Managed, cede pool |
| `afw_object_create_with_options` direct | rare | explicit mask |

So **managed memory objects are common** — they are **not** created via `afw_value_create_managed_object`.

**B. Adaptive value wrappers** (data type object create APIs):

| API | Approx count (hand C) | Notes |
|-----|------------------------|--------|
| `afw_value_create_unmanaged_object` | **~97** | Dominates value wrapping; function/ heavy |
| `afw_value_create_managed_object` | **~0–5** | Essentially unused as a call site pattern |
| `afw_value_create_unmanaged_array` | **~60** | |
| `afw_value_create_managed_array` | **~0** | |

Generated bindings still implement value managed create as **header + bare pointer** (double-wrap when instance already has `->value`).

**1a correction:** saying “managed create = 0” was only true for **`afw_value_create_managed_object`**. **`afw_object_create` is managed-by-default** and is widely used. Phase 1 must connect: managed **object** instance (often already has embedded `managed_object` value) ↔ value path that today often **re-wraps unmanaged**.

###### 1a conclusions (for 1b+)

1. **Most core object/array impls already set `value`** — phase 1 is less “add the field” and more “value create must use it” + a few **gaps**.  
2. **Gaps / careful cases:** `value_meta` (value not on `pub.value`); `runtime_const_meta` (inf-only file); always check **static initializers** (registry).  
3. **Memory object always embeds `managed_object_inf`** even when options say **unmanaged** object — reconcile object options vs value inf in **1c**.  
4. **Managed objects are common** (`afw_object_create`); **managed value wrappers are not** (`afw_value_create_managed_object` ≈ unused). Hot path: create managed/unmanaged **object**, then often `afw_value_create_unmanaged_object` — double-wrap risk even for managed instances.  
5. **Objects first** still right; array inventory is small and mostly already filled.  
6. **1b safe interim:** special-case value create when `instance->value != NULL`; if NULL, keep current wrap (or assert later) so gaps do not break before 1d.

#### Null / undefined / address identity (from discussion — not “just special”)

This is subtler than “mark null special in generate.”

**Manual permanent values in `afw_value.h` / `afw_value.c`** (predate or sit beside `dataType::` in strings.py):

| Symbol | Role |
|--------|------|
| **`afw_value_null`** | Adaptive **null** (permanent_null_inf). **Not** C NULL. |
| **`afw_value_undefined`** | Preferred representation of Adaptive **undefined**; C **NULL** is also undefined for historical reasons. |
| **`afw_value_unique_default_case_value`** | Same *shape* as null (permanent_null_inf) but a **different address** — switch `default` marker; **identity is the pointer**. |

Some of these were added **before** `boolean::` / `integer::` support in `strings.txt`. Overlap with generate is fine: keeping the critical sentinels **declared and documented in `afw_value.h`** is still worthwhile — they show up in **Doxygen value docs**, and they are important enough to **highlight** next to evaluate/nullish macros rather than only buried in generated `afw_strings.h`. Typed static const values from generate (`afw_boolean_v_true`, `afw_integer_v_zero`, …) stay in `afw_strings.*`; **null / undefined / unique-default** stay first-class in the public value API header.

Macros: **`afw_value_is_undefined`** = `!ptr \|\| ptr == afw_value_undefined`; **`afw_value_is_nullish`** = undefined **or** data type null. Always use macros — C NULL can mean “not applicable” in some APIs, not only undefined.

**Address compare is a real C-side pattern** in AFW: “is this *exactly* this singleton?” (null, undefined, unique default case, permanent booleans via `afw_boolean_v_*`, etc.). Multiple values can be “semantically similar” but **different addresses** for that reason. Creating a *new* null/unmanaged null breaks identity checks.

**Historical roots:**

- **XACML v3** first implementation predates/alongside libafw goals; many “odd” data types and long function names exist so a future **XACML extension** can map XACML types/functions onto AFW without inventing a second type system. Almost every XACML v3 function/type has an Adaptive counterpart.
- **ECMAScript-like** Adaptive Script added another world: undefined vs null vs empty object; ES itself is sloppy about null/undefined. Type-checking arguments and TypeScript-ish paths made a **single null instance** painful vs “data type is null” vs C NULL.
- Clashing worlds: **don’t make it worse**; accept macros/checks; XACML extension later may need special “emit the one true null” wiring.

**Implication for phase 0 / generate:**  
Do **not** naively multiply null instances via `create_managed_null` / random unmanaged nulls. Prefer **`afw_value_null` / `afw_value_undefined`**. Whether `null` JSON is `special` is secondary to **preserving singleton identity and is_undefined/is_nullish discipline**. Permanent null from generate and manual `afw_value_null` must not fork into competing identities without a plan.

**`special: true` data types only (authoritative: `generate/objects/_AdaptiveDataTypeGenerate_`):** four types — **any, undefined, unknown, void**. **null is not special.**

| dataType | brief | description (essence) |
|----------|--------|------------------------|
| **any** | Unrestricted type | Any value assignable; actual DT is that of the value. Prefer **unknown** or a concrete type: **any** only detects type errors at **evaluation**. |
| **undefined** | An undefined value | **Special** type for the **single internal value** representing undefined. **Not a valid data type for any other purpose.** |
| **unknown** | Type-safe unknown value | Any value assignable; actual DT is that of the value. Prefer over **any** because most type-check errors can be caught at **compile**. |
| **void** | No value | Common **function return** meaning no value. Variable of type void can only hold **undefined** (or **null** if `compile::strictNullChecks` is not set). |

Generator behavior for special: permanent-only infs, no create_managed/unmanaged. Fits **undefined** (singleton) and “not a normal value payload” kinds (**any/unknown/void** as type-system notions). **null** remains a normal data type with full bindings + separate manual **`afw_value_null`** singleton.

**Compiler / TypeScript-like checking (future work, issue ~#28):**  
See **Future: compile-time type checking** below for a full stash of notes. Short: special types support **TS-style annotations**; **syntax is largely parsed; enforcement is not finished.**

#### Open for discussion before a phase 0 *plan*

1. ~~Should **null** be special?~~ — See null/undefined section; answer deferred; identity + macros first.  
2. For phase 0, only **audit + small generator fixes**, or also start object/array create special-cases (that may be phase 1)?  
3. Is **RC start at 0** still the intended contract for all managed creates?  
4. Any types that should **never** have `create_managed_*` even if not special (e.g. function)?  
5. Should unmanaged `clone_or_reference` comments be fixed as a drive-by in phase 0?  
6. Step **−1** permanent `afw_v_*` cleanup — agree scope?

---

### Future: compile-time type checking (~1 month target, issue **#28**)

**Status today:** annotation **syntax + storage** largely present; **enforcement** incomplete. Do not assume `: Type` is checked at compile time. Tests often use `: any` for lenience.

**Goal later:** finish/enforce TypeScript-like checking in the Adaptive Script compiler (assignability, args, returns, flags).

#### Useful map when we pick this up

| Area | Where / what |
|------|----------------|
| **Type grammar** | EBNF in `afw_compile_parse_expression.c`: `DataType`, `Type`, `OptionalType`, `OptionalReturnType`, object property types, `meta` OT (partial) |
| **Parse Type** | `afw_compile_parse_Type` — resolves name via `afw_environment_get_data_type`; params for array/of, object OT id, function signature, script/template return, unevaluated nested type |
| **Default type** | Missing `: Type` → **`any`** (`afw_compile_parse_OptionalType`) |
| **void return** | `OptionalReturnType` allows bare `void` → NULL type (no value) |
| **type aliases** | `type Name = Type;` — `impl_parse_TypeStatement` in `afw_compile_parse_script.c` |
| **Type AST** | `afw_value_type_t` / list type in `afw_value_internal.h`; hung on symbols/params (`variable_type`, return types, etc.) |
| **Special data types** | `any`, `unknown`, `void`, `undefined` — `special: true` in `_AdaptiveDataTypeGenerate_`; permanent-only in bindings; type-system / sentinel roles (descriptions in those JSON files) |
| **any vs unknown** | **any** = type errors mainly at **eval**; **unknown** = prefer for **compile-time** checks (metadata descriptions) |
| **null / undefined** | Manual singletons `afw_value_null` / `afw_value_undefined`; macros `afw_value_is_undefined` / `is_nullish`; C NULL ≈ undefined; Adaptive null is typed; address identity matters; XACML + ES clash — don’t invent extra nulls |
| **void type vs void op** | Type `void` (returns) ≠ unary **`void` expression** (`void_operator` function) |
| **Flags already registered** | **`compile:noImplicitAny`** (flag env index); descriptions in strings / flag registration — wire into checker when enforcing |
| **strictNullChecks** | Mentioned on **void** data type description (variable of void only holds undefined, or null if flag not set) — find/finish when enforcing nullish |
| **Runtime arg checks** | Some function path already cares about undefined/null (polymorphic, etc.); compile-time should align with runtime macros/semantics |
| **XACML heritage** | Many data types + long function names for future XACML extension mapping — type checker must stay compatible with full data-type set, not only “script primitives” |
| **Related issues** | **#28** compile-time types; language umbrella **#62**; nullish / exists **#131**; memory/MM is separate but value types share `afw_value` / data_type registries |

#### Likely work themes (not a plan yet)

1. Assignability rules: concrete types, any, unknown, null, undefined, void, unions if any.  
2. Enforce annotations on let/const, params, returns, destructuring.  
3. Honor `noImplicitAny` / nullish flags.  
4. Align with `afw_value_is_undefined` / nullish / singleton identity.  
5. Keep XACML-facing types first-class in the type lattice.  
6. Tests: today many use `: any`; add enforcement suites when checker exists.

#### Do not break while doing #2 MM

- Do not remove special types or change their generate `special` without typechecker awareness.  
- Do not casually add create_managed for any/unknown/void/undefined.  
- Prefer permanent/singleton null/undefined discipline (helps both MM and types).

---

### Phased plan (working order — step by step)

| Phase | Intent | Status |
|-------|--------|--------|
| **Discuss** | Memory story pad (`designs/memory-management.md`); invariants; no big code yet | **paused** (good foundation) |
| **−1** | **Prefer permanent `afw_v_*` (and typed permanent values) over allocate/create when the string/scalar already exists from generate** — cleanup call sites left over from before strings.py emitted values | **−1a + −1b + −1c done** |
| **0** | **Audit `data_type_bindings.py` + generated bindings** — correct, complete, match permanent/managed/managed_slice/unmanaged model; finish gaps from recent work; use old branch tip as ideas (object/array create → `->value`, release via container) not as merge | **0a–0d done** — phase 0 complete |
| **1** | Managed **object/array** containers + `->value` identity (consistent impls; hide nastiness) | **1a + 1b′ + 1c done** → **1d** next |
| **2** | Assign / scope: **`clone_or_reference`** so variable-held values own needed lifetime | pending |
| **3** | Scope/symbol release correctness; escape (closures, returned compile results) | pending |
| **4** | Accounting / graceful OOM / limits (later) | pending |

Phases adjust as we go; “done for #2” is know-when-we-get-there.

---

## By area (memory notes as we discuss)

_Add/reorder subsections as topics come up. Each can record: how it works today, intended model, gaps, open questions. Link lenses (Identity / Lifetime / …) where useful._

### Pools and xctx

#### `afw_pool` interface

- Defined in interface XML (`afw_pool`); call surface is the usual macros (`afw_pool_calloc`, `afw_pool_release`, …).
- Mental model: **hierarchical pool** — allocate fast; bulk free on destroy/release. Most AFW values/objects/scopes allocate from pools.
- Methods (high level): `release` / `get_reference` (refcount → destroy at 0), `destroy` (force; use carefully), `get_apr_pool` (lazy public APR pool for APR APIs), `calloc` / `malloc`, **`free_memory_internal`** (see below), cleanup register/deregister.
- Helpers: `afw_pool_create(parent)`, `afw_pool_create_subpool(parent)`, `afw_pool_create_thread`, **`afw_pool_free_memory`** (public free helper), typed calloc/malloc macros.

#### Freeing individual blocks (`free_memory_internal` / `afw_pool_free_memory`)

**History:** Classic pool model was **allocate until release/destroy** — bulk free only. There was **no** individual free path.

**Now:** interface method **`free_memory_internal`** exists so an impl *can* return a block early. Naming is intentional:

- **`_internal` suffix** — not the preferred call site for application/extension code.
- Interface description: if the impl **does not support** free, the call **does nothing** (safe no-op).
- Callers should use **`afw_pool_free_memory(address, xctx)`** (`afw_pool.h`), which:
  1. Reads the **memory prefix** in front of the address (owning pool + size),
  2. Invokes **`free_memory_internal` on that pool** — so you do not need to know which pool instance allocated it.

Current full/subpool impls do implement free (return to free list / untrack). The API still allows “ignore” so code can free when useful (e.g. managed value headers, temporary buffers) without requiring every pool kind to reclaim.

**Primary free story remains pool release/destroy** (and subpool return-to-parent). Individual free is optional, secondary, and always go through **`afw_pool_free_memory`**, not the method macro by hand.

#### APR relationship (past / present / possible future)

**Past / present:** AFW pools started close to **how `apr_pool` works** — hierarchical bulk free, allocate until destroy. Full AFW pools still own an **`apr_pool_t`** and often grow via **`apr_palloc`**; destroy maps to **`apr_pool_destroy`**. Subpools already diverge (track + return to parent free list; parent’s APR pool).

**Possible future (not committed):** change the **afw_pool** implementation so **primary allocation does not go through APR at all**. An **`apr_pool_t` would be created only if something calls `get_apr_pool`** (lazy public APR pool for APR APIs that still need one). That would make AFW free to track sizes, free lists, and limits without APR’s pool model.

**APR may remain in the process anyway:** many other AFW call sites use **non-pool APR** (tables, hashes, arrays, threads, …). Even if pool storage is pure AFW, **`get_apr_pool` on demand** may still matter wherever we cannot replace those uses. Full APR removal is a separate, larger question.

**Why bring this up (OOM / graceful kill):** want enough visibility to know **how much memory an xctx / thread / request** is using so a host can **fail or kill that request** instead of taking down the server. That needs AFW-owned accounting (bytes_allocated already exists on pool self — incomplete story for process-wide limits). Classic **APR on allocation failure** was harsh: intentional hard failure (e.g. segfault / abort style “we have big problems”) rather than a clean Adaptive error — exact APR version behavior may have changed; AFW’s wrapper path tries `AFW_THROW_ERROR_Z(memory, …)` if `apr_palloc` returns NULL, but that may never run if APR aborts first.

Related: issue **#64** (stack overflow / OOM handling), umbrella **#2** (long-running memory + OOM resilience). Productizing graceful OOM is not the same as finishing managed values, but pool implementation and accounting are prerequisites for it.

#### Per-thread memory: what is / isn’t knowable

**Honest answer:** the OS does **not** give a clean, portable “this thread’s heap is N bytes” number. Heap is **process-wide** (or allocator-arena-wide); threads share the address space. What exists:

| Approach | What you get | Limits |
|----------|--------------|--------|
| **Process RSS / rusage** | Whole process (AFW already surfaces some of this via `afw_os` / systemInfo: maxrss, etc.) | Not per request thread |
| **Linux `/proc/self/status` VmRSS, smaps** | Process (or mapping) size | Not attributed to a pthread |
| **`getrusage(RUSAGE_THREAD)`** (Linux) | Per-thread **CPU** time (and some fields); **not** reliable per-thread heap | ru_maxrss is not “this thread allocated N” |
| **glibc mallinfo / malloc_info** | Allocator process totals | Not per thread; thread caches blur numbers |
| **jemalloc/tcmalloc stats** | Arena / thread-cache stats if you adopt that allocator | Extra dependency; still not “AFW request graph” |
| **AFW pool `bytes_allocated`** (already on `afw_pool_internal_self_t`, debug prints) | Bytes handed out via **afw_pool_***alloc on that pool (and subpool tracking) | Only AFW-pool traffic; misses raw malloc/APR outside pools; need aggregation policy (xctx pool + children + managed escaped?) |
| **Custom counters on alloc path** | Exact “this xctx/request charged N” if every adaptive alloc goes through instrumented pools | Best path for **graceful kill**; requires owning the alloc path (ties to APR decoupling) |

**Practical AFW direction:** don’t wait for the kernel to invent thread heap accounting. **Charge memory to xctx / request-session pool trees** (and optionally managed escape into longer-lived pools). Limit **that** counter; on exceed, throw Adaptive `memory` / abort the request and release the request-session xctx. Process RSS remains a coarse outer watchdog.

**Windows (side note):** native Windows host has been effectively abandoned; WSL is the pragmatic path if needed. One historical pain: **DLL import of data symbols** — Windows uses the standard pointer-to-pointer import thunk model, so you cannot treat many `extern` data symbols the same as on Linux ELF (where cross-DSO data access is more direct). That made sharing `afw_s_*` / permanent const data across extension DSOs awkward (related historical note on issue #2 about extensions and `afw_s_*`). Linux-first is consistent with dropping that tax.

#### Threads vs processes (why requests share an environment)

**Design choice:** production hosts (`afwfcgi`) use **threads** (and request-session xctx pools), **not** one OS process per request.

**Why not process-per-request isolation:** AFW carries a **huge amount of static/const data** (generated strings, const objects, function bindings, data-type infs, …) and a **single process-wide environment** where everything is **registered once** (`afw_generated_register`, function bindings, adapters, conf, …). Requests constantly **read that environment**. Duplicating env + permanent tables **per process** would be:

- Memory-heavy (zillions of copies of the same `.so`/const graph and registries),
- Painful for consistency (same registrations, same adapters, same conf),
- Awkward to share mutable env-level state (adapter anchors, services, …) without complex IPC.

So the model is: **one environment, one copy of permanent/const machinery, multithreaded access**, with **per-request (and per-scope) pools** for work memory. Isolation is **pool/xctx lifetime**, not process isolation. That is exactly why long-running / OOM / managed-escape correctness matters inside one process — you cannot rely on “OS kills the worker process and reclaims everything” as the only GC for bad requests.

*(Not solving multi-process env sharing here; just recording the constraint that drove threads + shared env.)*
- Implementation file: `src/afw/pool/afw_pool.c` — **four** infs in one file (see below). Shared self: `afw_pool_internal_self_t` (parent/children, apr_p, refcount, cleanups, free lists, optional `thread`).

#### Four implementations (same file)

| Inf id | Thread-safe? | Kind | Role |
|--------|--------------|------|------|
| **`pool`** | no (single-thread / thread-specific) | full pool | Original basic pool; owns an `apr_pool_t` |
| **`multithreaded`** | yes (locks) | full pool | Thread-safe wrapper around **`pool`** methods |
| **`subpool`** | no | subpool | Allocates from parent; tracks blocks; return-to-parent on free/destroy — **discuss next** |
| **`multithreaded_subpool`** | yes | subpool | Lock wrapper around **`subpool`** — **discuss next** |

Inf flags in `implementation_specific`: `is_multithreaded`, `is_subpool`.

#### Original pair: `pool` + `multithreaded` (this pass)

**History / intent:** Early AFW needed (1) a pool usable on a **single thread** without lock cost, and (2) a **multithreaded** pool when multiple threads might touch the same pool (especially the environment base pool). Both were and largely still are **APR-backed**: each full pool gets its **own** `apr_pool_t` via `apr_pool_create` (child APR pool under parent’s APR pool when there is a parent).

**Create paths:**

| API | What you get |
|-----|----------------|
| `afw_pool_internal_create_base_pool()` | Root **multithreaded** pool; no parent; `apr_pool_create(NULL)`; env `pool_number` 1. Only from env create. |
| `afw_pool_create(parent)` | Child **full** pool. Inf chosen by `parent->thread`: if parent is thread-specific → `pool` inf; else → `multithreaded` inf. Own `apr_pool_t` under parent’s APR pool (`impl_create`). |
| `afw_pool_create_thread` | Thread-specific **`pool`** (single-thread inf), `self->thread` set; used from `afw_thread_create`. Only way to start a thread-specific lineage. |

#### Component pools (pre-script “own a pool to free all at once”)

Long before Adaptive Script and **subpools**, AFW already used **full child pools** for anything that should be **created and torn down as a unit** while the process/env stays up.

| | Component pool (e.g. adapter) | Script **subpool** (scope) |
|--|------------------------------|----------------------------|
| API | `afw_pool_create(env->p)` (or other full parent) | `afw_pool_create_subpool(xctx->p)` |
| Kind | **Full** pool — own `apr_pool_t` | **Subpool** impl — tracks blocks in **parent** |
| Threading under env | Parent is env → **multithreaded** inf; malloc/calloc/release take **`multithreaded_pool_lock`** | Follows parent (xctx line often thread-specific) |
| Purpose | Whole component lifetime; destroy/release pool = drop **all** that component’s memory | Scope enter/exit without destroying whole xctx pool |
| Era | Pre–long-running script MM | Added for long-running script scopes |

**Environment:** `env->p` is the **multithreaded** base pool (any thread may allocate).

**Adapters (canonical example):** create path uses a pool with **parent `xctx->env->p`** — e.g. core afw adapter registration:

```c
p = afw_pool_create(xctx->env->p, xctx);
// conf object + adapter instance live in p; adapter->p = p
```

`afw_adapter_impl_create_cede_p(..., p, ...)` allocates the adapter instance **in that pool**. Because parent is the env multithreaded pool, **`afw_pool_create` selects `multithreaded` inf** — locking on alloc/free is in those methods (`IMPL_MULTITHREADED_LOCK_*`), not a separate ad-hoc lock in adapter code.

Same **“own full pool, parent env (or other long-lived pool)”** idea shows up for other conf/runtime components (auth handlers, services, etc. — same bulk-free pattern). Not every temporary uses this; only things you want to **release as a whole** while env continues.

**Terminology:** Do **not** call adapter pools “subpools” in the new-impl sense. They are **child full pools** of `env->p`. “Subpool” in this document means the **subpool / multithreaded_subpool** implementations used for scopes and managed object storage.

#### Lifetime patterns (not one size fits all)

Several different “managed by pool” stories coexist; **validate case by case**.

| Pattern | What dies together | Typical mechanism | Examples |
|---------|-------------------|-------------------|----------|
| **Scope subpool** | Scope-local work for one activation | Always **subpool** of `xctx->p`; release when scope RC → 0 | `afw_xctx_scope_create` |
| **Unit / component full pool** | Whole unit’s storage at once | Own **full** pool (or managed lifetime via **pool get_reference/release** instead of a separate counter) | Adapters (`env->p` child); **compiled_value**’s `p` holding AST/literals; other “create a pool for this thing, free the whole thing” call sites during eval |
| **Compile temps** | Only while compiling | Separate temp/shared pool released when compile finishes; durable graph stays on the unit pool | Parser/shared `temp_p` vs `compiled_value->p` |
| **Container with escaping parts** | Container ≠ all property/element values | Container may use own subpool for structure; **values** may need **optional_release / clone_or_reference** so they can **outlive** the object/array | Memory object properties; array elements shared across scopes |

**Vague but still useful memory:** some container-like things **don’t keep their own refcount** — they **add a reference on their pool** (or subpool) and treat pool RC as the lifetime of “the whole thing.” Memory objects already lean this way (`get_reference` → `afw_pool_get_reference(instance->p)`). That model fits **compiled_value** well *if* “all important stuff is in one pool and goes away at once.” It is **not** enough alone when **object property values** (or array elements) must **hang around after the container is gone** — those are value-level escape, not unit-pool free.

**During script eval:** callees may create **regular (full) pools** for a whole operation that ends in one shot (parse, temporary workspace, component). That is fine and **different** from **scope subpools**, which are the tight per-block/loop lifetime. Do not force every pool to be a subpool; do not assume every container is only pool-lifetime.

**Process:** pick the pattern per case, keep public APIs calm (`optional_release` / `clone_or_reference` / object release), hide pool vs value RC in the impl, test the escape story. We’ll get it right step by step.

**Single-thread `pool` (core logic):**

- `malloc`/`calloc`: size prefix (`afw_pool_internal_memory_prefix_t`: size + owning `p`), then `impl_alloc_memory` — free-list first-fit (reuse) or `apr_palloc` on `self->apr_p`.
- `free_memory_internal`: return block to free list (`impl_free_memory`; adjacent combine).
- `destroy`: run cleanups → release children → unlink from parent → **`apr_pool_destroy(self->apr_p)`** (bulk free of everything from that APR pool).
- `release` / `get_reference`: refcount; destroy at 0.
- **Not** safe across threads; callers on thread-specific pools must stay on that thread (`afw_pool.h` invariant).

**`multithreaded` (wrapper):**

- Same underlying behavior: methods call the **`pool`** implementations under  
  `AFW_LOCK_BEGIN(xctx->env->multithreaded_pool_lock)` … `AFW_LOCK_END`.
- So: **one process-wide multithreaded pool lock** (not per-pool), registered at env create.
- Env base pool uses this inf so any thread can allocate from `env->p`.

**Still “mostly APR wrappers”:** ownership and bulk free are APR pool lifecycle; AFW adds refcount, parent/child tree, optional free-list reuse, prefixes, cleanups, and the lock layer for multi. Individual free is possible but secondary to destroy-at-scope/request.

**Parent/child:** children linked on parent; creating a child `get_reference`s parent; destroy/release walks children. Multithreaded child-list mutations take the multi lock when parent is multi.

#### How this ties to server/request (shell)

- **Environment / base pool:** `afw_pool_internal_create_base_pool()` → `env->p` — **multithreaded** full pool, process life.
- **Thread pools:** `afw_pool_create_thread` → single-thread **`pool`** with `thread` set; worker xctxs hang under that lineage via `afw_pool_create` / xctx create.
- **Child xctx pool:** `afw_xctx_create` → `afw_pool_create(parent->p)` (full pool, inherits single vs multi from parent). `afw_xctx_release` → `afw_pool_destroy` on that pool = **request GC** when session xctx is released.
- **Subpools** (scopes, managed objects, etc.) use the other pair — **next discussion**.

#### Subpools (long-running scripts / scopes)

**Why they exist:** Early AFW was mostly **short-lived** adaptive expressions: create a pool, evaluate, take the answer, release the pool. Short scripts and model `on*` runs can still do that (one pool for the whole run). **Long-running scripts** re-enter scopes in loops/recursion; variables go in and out of scope; memory would **grow without bound** if every scope’s storage only died with the whole xctx/request pool. Subpools fix that: **per-scope bulk free** while the evaluation still uses a stable parent pool (usually **`xctx->p`**).

**Also:** assignment can copy a variable **across scopes**. Scalars can often be **cloned**; **objects/arrays** need **reference counting** (share container, not deep-copy every time). That is the managed container + `clone_or_reference` story — **partially implemented**.

**Mechanics (`subpool` / `multithreaded_subpool`):**

- No own APR pool for bulk storage: `apr_p` is the **parent’s**; bytes come from parent free-list / `apr_palloc`.
- Each allocation is **tracked** on the subpool (`first_allocated_memory` linked prefixes).
- On **subpool destroy** (pool refcount hit 0 via release): cleanups → release children → **reparent still-referenced child pools to parent** (scopes / assignment) → **walk tracked allocations and return each block to the parent free chain** (`impl_free_memory`) — designed to be **fast** bulk return, not a slow walk of Adaptive values. (Not a separate per-byte “value refcount”; the **pool** has refcount; managed **values** have their own RC story.)
- Externally same API as a full pool (`calloc`/`malloc`/`release`/…).

**Eval model (intended):**

```text
compiled unit evaluate
  xctx->p          = regular pool (request-session / thread / call pool)
  each scope       = afw_pool_create_subpool(xctx->p)   # see afw_xctx_scope_create
  scope struct + symbol_values[] allocated in that subpool
  value storage may live in parent or be managed elsewhere;
  when scope ends → afw_pool_release(scope->p) frees scope-local tracking
  if something was referenced into a longer-lived pool/scope,
    refs keep parent memory / child pools alive past allocating scope
```

Comment in subpool destroy explicitly mentions **scopes + variable assignment** reparenting children with remaining refs.

**Scope code today (`afw_xctx_scope_create`):** always `p = afw_pool_create_subpool(xctx->p)`; scope lives in that subpool; release when refcount ≤ 1.  
**`afw_xctx_scope_clone`:** copies `symbol_values` pointers with **`@fixme change these to value references when that's done`** — open #2 wiring.

#### Script assign path — `afw_function_compiler_script.c`

Compiled script nodes often evaluate through **compiler_script** execute helpers (let/const/assign, control flow, …).

**`impl_assign` / `impl_assign_value`** (central for “value moves into a variable”):

- Comment: call **before** full evaluate so clone need can be decided; objects/arrays may need clone if they could be mutated.
- **Today:** if object or array → **`afw_value_clone(value, p, xctx)`**; else → **`afw_value_evaluate(value, p, xctx)`**; then write into assignment target / `afw_xctx_scope_symbol_set_value`.
- **Intended (remembered direction):** once managed object/array + data-type value infs are solid, use **`clone_or_reference()`** on the value so:
  - permanent/unmanaged → as-is or clone into pool as needed  
  - managed containers → **get_reference** (RC) instead of always deep clone  
- First step was **managed object/array implementations** depending on **data-type value** managed infs; assign’s `clone_or_reference` is later on that stack — **not fully done**.

**Design goal (scopes + assign):** every variable slot holds an **`afw_value`**. After assign, **the value’s lifetime policy must keep alive anything that value still needs** (embedded object/array, slices’ containers, closed-over scope, compiled unit pool, …). Scope code should not track side tables of “extra pools to hold.” That is what **`clone_or_reference` / `optional_release`** (and pool-backed container release behind them) are for. **`afw_value` is the abstraction**; pools/subpools are the storage/lifetime engine underneath. If those two layers are enough, assign/scope stay simple.

Closures: assigning script functions can create **`closure_binding`** (scope get_reference) via `impl_create_closure_if_needed` — example of “important stuff” pulled under the value’s lifetime.

### Server and request hosts (`afw_server` / `afw_request`)

**Interfaces** (core): `afw_server`, `afw_request` — transport-agnostic. Handlers/adapters in libafw.

**FCGI impl** (`src/afw_server_fcgi`, `afwfcgi`): production multi-request host. Other host: `afw --local`. Same outer story: env once; work xctx per unit of work where applicable.

#### Process bootstrap (env pool = process life)

```text
main
  AFW_ENVIRONMENT_CREATE(xctx, …)     → base pool + base xctx (env->p / env->base_xctx)
  afw_server_fcgi_generated_register
  conf load/configure
  afw_server_fcgi_internal_create(…)  → server on env xctx; process-env object once
  afw_request_handler_director_create
  afw_server_run(server, director)    → thread accept loops
  AFW_FINALLY: afw_environment_release(xctx)   # incomplete today
```

Server create also: process-env variables object once, `environment::` on server xctx, FCGI socket, `_AdaptiveServer_/current`, trace flags. Server struct allocated with `afw_xctx_calloc` on **env** xctx.

#### Per-thread loop → per-request session (request pool)

```text
impl_afw_server_request_thread_start (thread xctx)
  FCGX_InitRequest on thread
  loop FCGX_Accept_r:
    request_session_xctx = afw_xctx_create(REQUEST_SESSION, request_count, thread_xctx)
      # new pool child of thread xctx pool
    impl_process_request(..., request_session_xctx)
      create_request on session xctx
      push request:: ; push environment:: only (no re-register process-env — #71)
      afw_request_handler_process(director, request, session_xctx)
      AFW_FINALLY: afw_request_release  # thin; memory with xctx
    AFW_FINALLY: afw_xctx_release(request_session_xctx)  # DESTROYS REQUEST POOL
```

**Intentional bulk GC:** end of request = destroy request-session xctx pool → no residual request memory *if* nothing incorrectly escaped into env/thread pools.

**Not freed per request:** env pool (registries, conf, permanent/const, process-env object), thread-level allocs (e.g. reused `FCGX_Request`), anything that deliberately escaped with longer ownership.

#### Request object notes

- Create: `afw_xctx_calloc` on **session** xctx; properties + `request::`; method/URI fields use `xctx->p`.
- `impl_afw_request_release`: resources release with execution context — not fine-grained free of the request struct.
- Transport only for body I/O (`FCGX_*`).

#### Implication for #2

- **Request-scoped work can keep using pool lifetime** (historical model still correct).
- **#2 matters when values must outlive the request-session xctx**, or when long work *within* a request needs release before pool destroy, or multi-request correctness of shared/escaped data.
- Incomplete managed release is invisible if everything dies with the request pool; multi-request and escaped closures expose it.

### Values (`afw_value` interface)

#### Interface and role

- **`afw_value`** is an interface. Public handle is always `const afw_value_t *` — **not** one C struct body; many layouts share the face (`inf` first / pub union).
- **Everything that is “evaluated” (or is an evaluable node in the script graph) is an instance of `afw_value`.** Other AFW types (`afw_object_t`, `afw_array_t`, pools, xctx, …) are **not** values, but objects/arrays **carry** a related `->value`, and data-type payloads often **point at** object/array internals.
- Prefer **`afw_value_evaluate()`** and create helpers over calling optional methods that may be NULL.

#### Macro `afw_value_evaluate(value, p, xctx)` (`afw_value.h`)

```c
#define afw_value_evaluate(value, p, xctx) \
(((value) && (value)->inf->optional_evaluate) \
    ? (value)->inf->optional_evaluate(value, p, xctx) \
    : value)
```

- If `value` is non-NULL **and** the inf has **`optional_evaluate`**, call it (result may allocate using **`p`**).
- Otherwise return **`value` as-is** (NULL stays NULL; already-evaluated / permanent / managed / unmanaged data-type values typically have **NULL** `optional_evaluate` → identity).
- So “evaluate” is cheap for evaluated data-type values and is the single entry point for unevaluated graph nodes (calls, refs, blocks, …).

#### Important interface methods (memory-relevant)

| Method | Role |
|--------|------|
| **`optional_release`** | Present when memory is **managed**; NULL for permanent/unmanaged/const-in-.so |
| **`clone_or_reference`** | Escape helper: permanent/unmanaged → as-is; managed → bump ref or clone into `p` (and related cases for internals with get_reference) |
| **`optional_evaluate`** | Unevaluated / AST-ish nodes → evaluated value (may allocate in `p`) |
| **`get_data_type` / inf data_type** | Typing |
| **`create_iterator`**, listing, decompile, **`get_info`** | Introspection / tooling |

Inf chooses **lifetime policy** — this is central to #2.

#### Two big families of implementations

**1. Hand implementations — `src/afw/value/`**  
Compiler / script graph and non-scalar kinds (not one-per-data-type generated):

Examples: `compiled_value`, `block`, `call`, `call_built_in_function`, `call_script_function`, `symbol_reference`, `qualified_variable_reference`, `reference_by_key`, `list_expression`, `object_expression`, `script_function`, `function_definition`, `function_thunk`, **`closure_binding`**, assignment_target, template_definition, meta helpers, …

These are how compile/eval is represented. Lifetime often tied to **compiled_value’s pool**, scopes, or (for closures) **scope get_reference** — #35 / long-running escape stress.

**2. Generated per data type — afwdev `data_type_bindings.py`**  
Metadata: `generate/objects/_AdaptiveDataTypeGenerate_/*.json`.  
Emits per type (e.g. integer, string, object, array, …):

| Inf | Lifetime |
|-----|----------|
| `afw_value_permanent_<dt>_inf` | Built-in / env life; no refcount; clone_or_reference as-is; no create_permanent_* API |
| `afw_value_managed_<dt>_inf` | Refcount; `create_managed_*`; **optional_release** frees/decrements |
| `afw_value_managed_slice_<dt>_inf` | utf8/memory only: view into containing managed; release/ref on container |
| `afw_value_unmanaged_<dt>_inf` | Pool / programmer owned; optional_release NULL; die with pool |

Structs: `afw_value_<dt>_t` (pub + internal), `afw_value_<dt>_managed_t` (+ reference_count), slice variant when applicable. Special types (null, undefined, …) may be permanent-only.

**Object/array data types** sit at the junction with containers: internal is `const afw_object_t *` / `const afw_array_t *`; memory objects embed a value with managed object inf; create_* still often double-wrap (open #2 work).

#### How this fits the story so far

```text
Request ends → destroy request-session pool
  → bulk free of unmanaged values and pool-local storage

Managed values / managed containers that escaped
  → need optional_release / clone_or_reference / object-array release
  → not fixed by request GC alone

Permanent values
  → never released; must not hold request-only children without cloning
```

Closure_binding and compiled_value are the hand-side peers of “managed escape” for script graphs.

### Objects and arrays (interfaces, many impls, managed hint)

#### Interfaces

- **`afw_object`** and **`afw_array`** are core interfaces (many implementations), same pattern as `afw_pool` / `afw_server`.
- Public instance layout always has **`p`** (pool containing the instance, or NULL if const) and **`value`** (adaptive value for this instance; same lifetime as the instance).
- Interface docs already say: depending on impl, `p` may be a **subpool** of the create-time parent; if so, **`release()` releases that subpool** (bulk free of the container’s storage).

#### Many implementations (not exhaustive)

**Objects** (examples): `memory` (incl. faces/`wrapped`), view, meta / meta_accessor, const_key_value, aggregate_external, runtime indirect, FCGI request properties, env variables object, …

**Arrays** (examples): `memory`, const array of values, view_of_c_array, …

Different impls mean different lifetime stories: pure views over external data, const permanent, callback-backed, vs **memory** containers that own storage.

#### Own-pool pattern (memory objects — clearest today)

**Memory object managed create** (`afw_object_create` → option `managed`):

1. `p = afw_pool_create_subpool(parent_p)` — object gets **its own subpool**.
2. Object (and typical property storage) allocated from that subpool.
3. `get_reference` → `afw_pool_get_reference(instance->p)` (or parent entity if embedded).
4. `release` → `afw_pool_release(instance->p)` when not unmanaged/embedded — **destroying the subpool frees the whole object graph hanging off that pool**.

**Unmanaged** (`afw_object_create_unmanaged`): no dedicated subpool; lives in caller’s `p`; `release`/`get_reference` are **no-ops** for refcount — lifetime is the enclosing pool (request GC story).

**Embedded** memory objects: share embedder’s pool; managed-by-entity — release/get_reference forward to entity.

So: **some object impls already “own a pool” so you can free a whole object (and its pool-local content) in one release.** That is container-as-lifetime, pool flavor.

**Memory arrays today:** create takes a caller `p`; `release` is currently a no-op (“storage is in the pool provided”). Array interface still documents the subpool possibility; managed-array-as-owning-container is less complete than memory objects (or not flipped the same way yet).

#### Managed as containers (direction / hint — not fully done)

Where we are heading for **managed** (and #2):

- Managed objects/arrays behave like **containers**: when the container is released, it should **release references to contained values** (not only free a subpool of raw bytes).
- That matters when properties/elements are **managed values** that outlive bulk pool free, or are shared via refcount across containers.
- Pool-per-container (subpool release) is the historical bulk tool that made request-scoped managed objects work with request GC; **value-level release of children** is the next step for true long-running managed graphs.
- Unmanaged continues to mean: no independent refcount; die with the pool you were allocated in.

Ties to stale tip: identity of `object->value` / `array->value` + managed create/get_reference/release on the **instance** (and eventually its contents), not double-wrapped value headers that ignore the container.

#### `->value` / create APIs (still open from archaeology)

- Memory/const impls embed and set `->value`.
- Generated `create_*_object` / `create_*_array` still often allocate **wrappers** separate from `instance->value`.
- Stale tip: return `->value`; managed path = object/array get_reference/release.

### Compiler (`src/afw/compile/`) and scopes / eval / closures

#### Compiler → single `afw_value`

Public entry: **`afw_compile_to_value`** / **`afw_compile_to_value_with_callback`** (`afw_compile.h`). Output is **one** `const afw_value_t *`:

| Compile type | What is returned |
|--------------|------------------|
| **script**, **template**, **test_script** | **`compiled_value`** (`&parser->compiled_value->pub`) — container for the whole unit |
| **json** / **relaxed_json** | **Direct** evaluated-ish value (object/list/scalar from `parse_Json`) — not wrapped in `compiled_value` |
| regexp / xpath | Not supported yet (throw) |

So for Adaptive Script (the usual case): compile builds a graph of **hand value kinds** (block, call, symbol_reference, …) under one **`compiled_value` root**. Eval is `afw_value_evaluate(compiled, p, xctx)` → `compiled_value`’s `optional_evaluate` evaluates **`root_value`** into pool `p`.

#### `compiled_value` layout (memory-relevant)

From `afw_value_internal.h`: owns **`p`**, optional **parent** compiled_value, **`root_value`**, source/location, symbol blocks, **literals** object, **shared strings**, contextual. Comment: top-level return when result is more than a literal; methods delegate evaluate/get_data_type to root.

**`compiled_value` inf today:** `optional_release` NULL, `clone_or_reference` NULL — lifetime is **the compile pool**, not managed value RC. If compile used caller/`shared`/`parent` pool, the unit dies when **that pool** is destroyed.

**Escape concern (note, open):** Scripting can call adaptive **`compile` / compile_and_evaluate / compile_from_file / …** (`afw_function_compiler*.c`). If the **compiled value is returned** from a script, stored, or closed over, while its compile pool was short-lived (e.g. request or a temporary pool), the result is a dangling graph. **May need reference counting related to the compiled unit’s pool** (or managed compiled_value) so a returned/escaped compile survives. Overkill for “compile once on env/request pool and eval in place”; real for “compile in script and return the compiled value.” Revisit before treating compile-in-script as long-running-safe.

#### Pool for compile

`afw_compile_lexical_parser_create` (precedence):

1. `shared->p` if shared
2. else `parent->p` if nested under parent compiled_value
3. else if cede_p → caller’s `p`
4. else **`afw_pool_create(p)`** — **child pool** for this compile

Parser allocates `compiled_value` and AST nodes into `parser->p`. Nested compile with `parent` shares symbols/pool with outer unit; `#{…}` uses isolated nested compile (`parent=NULL`, own story).

`afw_compile_and_evaluate`: compile then `afw_value_evaluate` into same `p`. Inline **@todo**: memory usage needs more thought — only **result** needs to survive, but side effects (e.g. started services) need lasting pools; “might solve itself with new memory management.” Classic #2 / request-vs-escape tension.

#### File map (brief)

| File | Role |
|------|------|
| `afw_compile.c` | Dispatch by compile_type; and_evaluate |
| `afw_compile_lexical.c` | Tokens, residual, parser create/finish, shared |
| `afw_compile_parse*.c` | Script / expression / value / template / assignment |

#### Eval link (for later)

- `afw_value_evaluate` on compiled_value → scope-stack NULL marker → evaluate root → clean scopes.
- Scopes / `closure_binding` / statement_flow: deeper under script-eval; compile only **builds** the value graph.

#### Hardening intent: evaluation stack (bounded recursion / eval depth)

**Philosophy (maintainer context):** AFW is written with a **production / mainframe-style** expectation of bounds and interceptable failure (pool bulk free, Adaptive errors, limited eval depth) — not “open systems don’t worry be happy” silent overflow. Running a **scripting language on a server thread** makes that hard: C stacks don’t bound, and process-per-request was rejected for env/static-data reasons. Hardening is layered: request pools, scope subpools, managed values (in progress), and an **explicit evaluation stack** so runaway recursion is an **Adaptive error**, not undefined C stack smash.

**Mechanism:**

- Each xctx gets `xctx->evaluation_stack` from **`afw_stack_internal_set_evaluation_stack(xctx)`** during xctx create (early: uses APR on `xctx->p`, before AFW_TRY-heavy paths).
- Sizes from env: **`evaluation_stack_initial_count`** (default **100**), **`evaluation_stack_maximum_count`** (default **500**) — `AFW_ENVIRONMENT_DEFAULT_EVALUATION_STACK_*` in `afw_common.h`.
- Eval of calls, blocks, symbol refs, etc. **push/pop** entries (`afw_xctx_evaluation_stack_push_value` / `pop_value`, parameter numbers for backtrace).
- On extend past max: **`AFW_THROW_ERROR_Z(general, "Stack max_count exceeded", xctx)`** (`afw_stack_extend_impl`) — not silent growth forever.
- Same max reused as a guard in places like decompile indent (`writer->indent > evaluation_stack_maximum_count`).
- Error reporting walks the evaluation stack for Adaptive backtraces (`afw_error.c`). Note: some error paths have FIXMEs (e.g. rethrow vs stack restore).

**Dual purpose — bounds *and* Adaptive backtrace:**

The same stack is excellent for **explaining where evaluation is** when something fails. As values evaluate they push; nested evaluation (e.g. a **call** value evaluating its **parameters**, which may themselves be **calls**, which evaluate *their* parameters, …) builds a natural **chain of adaptive values** on `xctx->evaluation_stack`. Error paths walk that stack for an Adaptive backtrace (`afw_error.c` — entries with values / parameter numbers), not only a C stack dump. So: **limit runaway depth** + **readable eval provenance** for authors and debuggers.

**What this is / isn’t:**

| Is | Isn’t |
|----|--------|
| Bound on **Adaptive evaluation depth** (logical call/value stack) | Full hardware C stack guard like mainframe bounds + abend intercept |
| **Backtrace of adaptive values** during eval (calls, params, nested calls, …) | Replacement for full source debugger |
| Request/xctx-scoped stack storage | Process isolation |
| Complements pool/OOM story | Complete substitute for memory limits |

Together with request-session GC and (future) pool accounting: try to keep a bad script from taking the whole multi-threaded server.

### Functions / data type bindings (generator)

#### Data type value infs

- **`_afwdev/generate/data_type_bindings.py`**: per-data-type value infs and create/allocate APIs (see Values section).
- Do not hand-edit `src/afw/generated/afw_data_type_*_binding.*`; regenerate via `./afwdev build --cdev`.
- Stale branch tip: special-case object (then array) create/release to use `instance->value` — re-apply against current generator.

#### Function bindings — permanent function values

**Metadata (hand-edit):**

| Path | Role |
|------|------|
| `src/afw/generate/objects/_AdaptiveFunctionGenerate_/*.json` | Concrete adaptive functions (core: ~186) |
| `src/afw/generate/objects/_AdaptivePolymorphicFunction_/*.json` | Polymorphic families (e.g. `add` over double/integer/string) — expanded into typed variants / poly handling |

**Generator:** `_afwdev/generate/function_bindings.py`  
**Output:** `src/afw/generated/afw_function_bindings.h` / `.c` (large; do not hand-edit).

**Among what is emitted — adaptive values (no pool):**

1. **`afw_function_definition_<label>`** — `const afw_value_function_definition_t` with **`afw_value_function_definition_inf`**  
   - Itself an **`afw_value`** (function-as-value): execute pointers, parameter metadata, pure flags, etc.  
   - Inf has **`optional_release` NULL** (same class as permanent for lifetime: binary/process life).  
   - Declared in the header; registered via `afw_function_bindings_get()` array of pointers.

2. **Runtime function object** per function — `afw_runtime_object_indirect_t` for `_AdaptiveFunction_/<id>`, with paired **`afw_value_object_t`** using **`afw_value_permanent_object_inf`**, `pub.value` set, `p` NULL. Internal points at the function definition value.

3. **Parameter / returns** as `afw_value_function_parameter_t` (+ permanent object value wrappers) — meta fields often point at **`afw_self_v_*` / `afw_boolean_self_v_*` / `afw_integer_self_v_*`** from generated `afw_strings.*`.

Hand implementation of execute bodies: `src/afw/function/afw_function_*.c` (`afw_function_execute_*`), not the generated closet.

**Environment registration (process bootstrap):**

```text
AFW_ENVIRONMENT_CREATE / afw_environment_create
  → … afw_environment_register_core …
    → afw_generated_register(xctx)          # generated/afw_generated.c
         → afw_environment_register_functions(
               afw_function_bindings_get(), xctx)
    → afw_function_internal_prepare_environment(xctx)  # after functions registered
```

- **`afw_function_bindings_get()`** returns a NULL-terminated array of pointers to the const **`afw_function_definition_*`** values.
- **`afw_environment_register_functions`** walks that list and **`afw_environment_register_function`** each by function id into the env registry (on **`env->p`** / process life).
- Extensions follow the same pattern (e.g. `afw_curl_function_bindings_get()` from their generated register).

So built-ins are not created per request: they are **static values**, registered **once** when the environment is created, then looked up for the life of the process.

**Memory role:** permanent function values + one-time env registry entries (env pool). Polymorphic JSON is metadata that expands into that generated const graph.

### Adapters, retrieve (beyond host shell)

_(handlers/adapters in libafw; progressive response #127, maxObjects #49 — related but separate)_

### Qualifiers / environment / runtime objects

- Process `environment::` object: **once** at server create; per-request only **push qualifier**.
- Request `request::` properties: per request-session xctx.

### Const / permanent / generated strings and objects

afwdev generate emits a large amount of **`static const` / `const` C** for core (and packages): no pool, process/binary lifetime, **permanent** value infs.

#### Generated static const strings/values — `strings.txt` → `afw_strings.*`

Source: `generate/strings/strings.txt` (and similar lists). Generator: `_afwdev/generate/strings.py`.  
Output: `src/afw/generated/afw_strings.h` / `.c` (huge; do not hand-edit).

**File format** (from header comments in `strings.txt`):

- Blank lines and `#` comments ignored.
- No `=` → name and value are the same string (`afw` → name `afw`, value `"afw"`).
- With `=` → name before `=`, value after (C escapes allowed). Convention: `a_` prefix when name ≠ value.
- **`dataType::name=value`** → permanent value of that data type (not only string).

**Supported `dataType::` (per comments + generator):**

- `boolean`, `double`, `integer` (scalar internal, not utf8)
- Any data type whose **`cType` is `afw_utf8_t`** (string, anyURI, objectId, …) — generator’s `supported_dataTypes` uses `AFW_UTF8_LITERAL` for those

Examples from `strings.txt`:

```text
boolean::true=true
boolean::false=false
integer::zero=0
integer::one=1
double::1=1.0e1
anyURI::example=www.example.com
```

**Default (string) line**, e.g. `200` / `true` as string:

```c
const afw_value_string_t afw_self_v_200 = {
    {&afw_value_permanent_string_inf},
    AFW_UTF8_LITERAL(AFW_Q_200)
};
```

Header aliases **the same storage** (no copy, no pool):

| Macro / symbol | Type | How |
|----------------|------|-----|
| `AFW_Q_200` | quoted C literal | `"200"` |
| `afw_self_v_200` | `const afw_value_string_t` | full permanent value |
| `afw_v_200` | `const afw_value_t *` | `&…pub` |
| `afw_s_200` / `afw_self_s_200` | utf8 | `.internal` |
| `afw_z_200` | `char *` | `.internal.s` |

**Typed (`dataType::`) lines** use a **prefixed** symbol namespace and permanent inf for that type:

```c
const afw_value_boolean_t afw_boolean_self_v_true = {
    {&afw_value_permanent_boolean_inf}, true };
// afw_boolean_v_true → &afw_boolean_self_v_true.pub

const afw_value_integer_t afw_integer_self_v_zero = {
    {&afw_value_permanent_integer_inf}, 0 };

const afw_value_double_t afw_double_self_v_1 = {
    {&afw_value_permanent_double_inf}, 1.0e1 };

const afw_value_anyURI_t afw_anyURI_self_v_example = {
    {&afw_value_permanent_anyURI_inf},
    AFW_UTF8_LITERAL(AFW_ANYURI_Q_example) };
// anyURI also gets Q / s / z aliases like string (utf8 cType)
```

Note: generate can still emit **string** `afw_v_true` (the characters `"true"`) **and** boolean `afw_boolean_v_true` (boolean permanent) — different values.

All permanent ⇒ no pool, no `optional_release`.

#### Cross-generator registration — `options['const']` / `get_string_label`

Permanent strings/values are **not** only whatever is listed in `generate/strings/*.txt`. Other generators **contribute** to one shared bag during the same `afwdev generate` pass; `strings.generate` runs **last** among const-related work and is the sole emitter of `generated/<prefix>strings.h` / `.c`.

**Shared bag** (initialized early in `generate.py`):

```text
options['const'][dataType][label] = value
```

- `dataType` defaults to `'string'`; also `boolean`, `integer`, `double`, and utf8-like types from `strings.py` `supported_dataTypes`.
- `label` is the C stem (`brief`, `objectId`, `zz__…`, `a_dash`, …).
- `value` is the permanent content (text, or C token like `true` / `0` for non-string types).

**API:** `get_string_label(options, string, type, labelPreference=None, dataType='string')` in `_afwdev/generate/strings.py`.

- **Registers** (or reuses by equal **value**) into `options['const'][dataType]`.
- **Returns** the C symbol for the requested form:

| `type` | Symbol form | Meaning |
|--------|-------------|---------|
| `Q` | `AFW_Q_label` | quoted `#define` |
| `s` / `self_s` | `afw_s_` / `afw_self_s_` | `afw_utf8_t` view |
| `v` / `self_v` | `afw_v_` / `afw_self_v_` | permanent `afw_value_*` |
| `U` | `AFW_U_…` | unquoted `#define` |
| `*z` | `afw_z_…` | `const char *` into the utf8 |

Non-`string` types get a prefix insert (`afw_boolean_v_true`, `afw_integer_v_zero`, …). Bad identifiers become `zz__…` (or a numbered label if very long). Same value within a dataType reuses one label.

**Who fills the bag before emit** (order matters; from `generate.py`):

| Contributor | When | What it adds |
|-------------|------|----------------|
| **`function_bindings`** | Before strings | Parameter/function names, briefs, descriptions, signatures; `dataType='boolean'|'integer'` for flags and minArgs, etc. Emits C that **points at** `get_string_label(..., 'self_v')` symbols. |
| **`const_objects`** | Before strings | String **property values** via `get_string_label(..., 'self_v')`; objectId / objectType / embedder propname via `self_s`. Property **names** in the property table often reference `afw_self_s_<name>` without calling the helper at that site. |
| **`generate.py` (direct)** | e.g. manifest | Occasional `options['const']['string'][label] = value` without the helper. |
| **`strings.generate` harvest** | Last | Fixed extras (`default`, `indirect`); walk object JSON for type ids, object ids, every property **name** (hyphen → `a_…`), nested objects, `dataType` string fields; function object-type names; all `generate/strings/*.txt` lines — then **emit** `.h` / `.c`. |

If anything already put entries in `options['const']['string']`, generate forces `options['strings'] = True` so the emit step runs.

**Flow:**

```text
  strings.txt  ──┐
  object JSON  ──┤  (during strings.generate harvest)
  generate.py  ──┤
  const_objects ─┼── get_string_label / options['const'][dt][label]=value
  function_bind ─┘              │
                                ▼  (last among these)
                     <prefix>strings.h / .c
                     permanent afw_self_v_* + macros
```

**Why this exists (especially const objects):** while emitting a const object, property **values** that are string literals (and meta ids) need **stable permanent symbols** in the generated C. Registering them into the same bag as `strings.txt` means one `afw_self_v_*` / `afw_s_*` pool for property names, literals, function metadata, and hand-listed strings — deduped by value.

**Caveat:** `additional_generate` runs **after** `strings.generate` in the current pipeline, so it cannot contribute to this bag for the **same** pass unless something else is arranged.

**−1 implications:**

1. New permanent literals from generators → `get_string_label` (or intentional bag write), not ad-hoc local static utf8 / one-off defines, if they should join the shared pool and macros.
2. Hand C call sites → prefer existing `afw_s_*` / `afw_v_*` when the string is (or will be) in that bag (−1a style). Prefer **named** typed permanents from `strings.txt` (`afw_integer_v_zero` / `…_one`, not auto `…_v_0` / `…_v_1`) when early seed makes those the canonical labels.
3. Ordering is load-bearing: `seed_from_strings_dir` runs early; `const_objects` / `function_bindings` register into the bag; `strings.generate` harvests more and emits last. `labelPreference` always creates/keeps that label (needed for `a_*` aliases); value-only calls dedupe onto the first label for that value.
4. Typed bag entries (`boolean` / `integer`) are the same pipeline as `boolean::true` / `integer::zero` in `strings.txt` — same world as −1b / −1c permanent helpers.
5. **Done (−1c):** const_objects scalar properties use the bag; local permanent bool/int/double property_value statics removed.

#### Const objects — `src/afw/generated/afw_const_objects.c` (+ thin `.h`)

Generator: `const_objects.py`. Header is mostly **`afw_const_objects_register(xctx)`**; bulk is in `.c`.

Pattern per object:

- Runtime const object instance (`afw_runtime_const_object_instance_t`) with property tables pointing at **`afw_self_s_*` / `afw_self_v_*.pub`** (permanent string/value ids — filled via the bag above).
- Paired **`afw_value_object_t`** with **`afw_value_permanent_object_inf`**, `internal` → that object; object’s `pub.value` → that value.
- Meta objects same permanent-object-value pattern; `p` is NULL (const).

Register at env bootstrap so paths like `/afw/_AdaptiveObjectType_/…` resolve without building objects in a pool.

#### Memory-story role

- **Permanent** = process/binary life; no request GC, no managed RC.
- Call sites should reuse `afw_v_*` / `afw_s_*` instead of allocating the same literal in a request pool.
- Generators already funnel many property names/literals and function metadata through one permanent bag (`get_string_label`); −1 is mostly **using** that bag from hand C and bindings, not inventing a second pool.
- **#110 class risk:** permanent/shared mutables (if any path allows mutation of const objects) must not be returned as defaults without clone — const objects are intended immutable; open mutables elsewhere still need clone rules.
- Object/array `->value` on const instances is already the permanent value — aligns with “use instance value” for permanent case.

---

## Chronological discussion log

### 2026-07-22 — kickoff

- Reviewed issue #2, stale branch tip, current object/array value wiring, value-memory rules.
- Agreed: use old branch as knowledge; re-implement intent later on current tree; discuss before large code.
- Started this file; will hop by AFW area rather than a fixed layer order.

### 2026-07-23 — historical GC + afwfcgi server/request

- Early model: REST-like requests; **OK to GC mostly at request end**; that ultimate cleanup still applies.
- Walked `afw_server_fcgi`: env create (base pool), server on env xctx, thread accept, **request_session xctx + pool per accept**, destroy in FINALLY.
- Environment has its **own long-lived pool** (`env->p`); request-session has a **child pool** destroyed every request.
- `afw_request_release` is deliberately thin; real free is `afw_xctx_release` → `afw_pool_destroy`.
- Pools deeper next; this pass is the outer server/request lifetime shell only.

### 2026-07-23 — afw_pool interface + original full-pool pair

- `afw_pool` is an interface; four infs in `afw_pool.c`: pool, multithreaded, subpool, multithreaded_subpool.
- Original pair: **single-thread `pool`** + **`multithreaded`** lock wrapper; both APR-backed full pools (own `apr_p`).
- Base env pool = multithreaded; thread-specific lineage starts at `afw_pool_create_thread` (`thread` field → non-locking infs for children).
- Multi uses env-wide `multithreaded_pool_lock`. Subpool pair deferred to next discussion.

### 2026-07-23 — objects/arrays as interfaces; managed containers

- `afw_object` / `afw_array`: many impls; some own a **subpool** so release frees the whole container’s pool memory (memory object managed path).
- Unmanaged: release no-op; lifetime = enclosing pool (classic request GC).
- **Direction:** managed containers should also **release references to contained values** when the container is released — pool bulk free is necessary but not sufficient for long-running graphs.
- Memory arrays’ release still mostly no-op today; object managed path is further along on pool ownership.

### 2026-07-23 — afw_value interface + two impl families

- `afw_value` is an interface; everything **evaluated** is a value instance; many non-value AFW structs exist alongside.
- Hand kinds in `src/afw/value/` (compile/eval graph: call, block, closure_binding, …).
- Generated multi-inf per data type via `data_type_bindings.py`: permanent / managed / managed_slice / unmanaged.
- Memory-critical methods: **optional_release**, **clone_or_reference**, optional_evaluate.
- Macro `afw_value_evaluate`: optional_evaluate if present, else identity.

### 2026-07-23 — compiler returns a single value

- `src/afw/compile/`: `afw_compile_to_value*` → one `const afw_value_t *`.
- Script/template/test_script → **`compiled_value`** (pool + root_value + literals/symbols); JSON → direct value.
- Compile pool: shared / parent / child-of-caller `p`; AST lives there; `compiled_value` not managed-released today.
- `afw_compile_and_evaluate` has explicit @todo on memory (result vs side-effect pools) — points at #2.

### 2026-07-23 — generated permanent strings and const objects

- afwdev emits large const C: `afw_strings.*`, `afw_const_objects.*` (and peers).
- All those values use **permanent_*** inf — no pool required.
- Strings: one `afw_self_v_*` permanent string value; macros for `afw_v_*` (value), `afw_s_*` (utf8), `afw_z_*` (char *), `AFW_Q_*` (quoted literal) share the same storage.
- Const objects: permanent object values + NULL `p`; registered into env.

### 2026-07-23 — strings.txt multi-dataType permanent values

- `strings.txt` comments: `dataType::name=value` for boolean, double, integer, and utf8 cTypes.
- Produces e.g. `afw_boolean_v_true`, `afw_integer_v_zero`, `afw_anyURI_v_example` with matching permanent_* inf — still no pool.
- Distinct from string `"true"` (`afw_v_true`) vs boolean true (`afw_boolean_v_true`).

### 2026-07-23 — function generate → permanent function values

- `_AdaptiveFunctionGenerate_` + `_AdaptivePolymorphicFunction_` → `function_bindings.py` → `afw_function_bindings.*`.
- Each function: **`afw_function_definition_<label>`** as `afw_value` (`function_definition_inf`, no optional_release), plus permanent function **object** value and parameter meta objects.
- Wired to permanent strings/booleans/integers; execute bodies stay hand-written in `function/`.

### 2026-07-23 — function bindings registered at env create

- `afw_generated_register` → `afw_environment_register_functions(afw_function_bindings_get(), xctx)`.
- All built-in function values registered once into the environment registry (env/process life), not per request.
- Same pattern for extensions (`*_function_bindings_get` from their generated register).

### 2026-07-23 — subpools, scopes, assign / clone_or_reference

- Subpools added for **long-running scripts**: per-scope free without destroying whole xctx pool; bytes in **parent** pool, subpool tracks and returns them.
- Short expressions/scripts/model runs can still use one outer pool for the whole unit.
- Cross-scope assign: scalars clone; objects/arrays need **RC** — managed containers + value `clone_or_reference`.
- `afw_xctx_scope_create` uses subpool of `xctx->p`; scope_clone has FIXME for value references.
- Script **`impl_assign`** currently `afw_value_clone` for object/array; intended path **`clone_or_reference`** after managed object/array + data-type value wiring.
- Part of #2 not fully implemented yet.

### 2026-07-23 — component full pools (adapters, etc.)

- Pre-script pattern: many long-lived components get **`afw_pool_create(env->p)`** — full multithreaded child pool, not a script subpool.
- Release component pool = free all that component’s memory at once while env stays.
- Adapter create: parent `env->p` → multithreaded inf → locking inside pool methods.
- Vocabulary: “subpool” = new tracking impl; “adapter pool” = full child of env.

### 2026-07-23 — free_memory_internal vs afw_pool_free_memory

- Individual free is **new**; classic model was release pool only.
- Method named **`free_memory_internal`**: may no-op if unsupported; use **`afw_pool_free_memory(addr)`** so correct pool is chosen from allocation prefix.

### 2026-07-23 — APR decoupling / OOM accounting (direction)

- Pools began close to apr_pool; may later allocate without APR except lazy `get_apr_pool`.
- Other APR uses may keep APR in the process even if pool backing changes.
- Goal: know memory use per xctx/thread/request → kill bad request instead of crashing server.
- APR historically hard-failed on OOM; graceful Adaptive memory errors need AFW control of alloc + accounting (#64 / #2).

### 2026-07-23 — no real OS per-thread heap number

- Kernel/process RSS is process-wide; no clean “thread N heap bytes.”
- AFW path: instrument/aggregate **pool trees per xctx/request** (`bytes_allocated` seed); optional process RSS as outer limit.
- Windows native largely abandoned (WSL); ELF vs DLL data-import model was a historical externals pain.

### 2026-07-23 — threads + shared env (not process-per-request)

- Huge static/const data + one registered environment → requests share a process; threads + request xctx pools.
- Process-per-request would multiply env/const copies; hard to share adapters/conf.
- Isolation = pool/xctx free, not OS process death — reinforces need for #2 / accounting inside one process.

### 2026-07-23 — hardening philosophy + evaluation_stack

- Mainframe-style bounds/intercept culture vs unbounded C stacks; script-on-server-thread is hard.
- `afw_stack_internal_set_evaluation_stack` on xctx create; env max default 500; push on eval; **Stack max_count exceeded** instead of runaway recursion forever.
- Complements pools/managed memory; not full OS abend model.

### 2026-07-23 — evaluation_stack as Adaptive backtrace

- Same stack = provenance while evaluating: call → params (may be calls) → nested values, etc.
- Error reporting walks it for Adaptive backtraces, not only C frames.

### 2026-07-23 — Q&A before code (partial answers)

1. **Done for #2:** know when we get there; step by step.  
2. **Order:** roughly containers/`->value` → assign `clone_or_reference` → scope correctness → accounting; step by step.  
3. **Object/array containers:** value layer owns policy; many impls; hide nastiness; stay consistent.  
4. **Subpool release:** when pool RC → 0, walk allocations and return blocks to parent free chain (fast); reparent surviving child pools.  
5. **`compiled_value` release NULL:** currently dies with compile pool; **open** — may need pool/value RC if scripts compile and **return/escape** the compiled unit (closures of returned compiled values).

### 2026-07-23 — pool-lifetime containers vs scope subpools vs escaping parts

- Some containers: lifetime = **ref on own pool/subpool** (not a separate counter); good when **all parts go away at once** (compiled unit, adapter, one-shot eval workspaces).
- **Scope** lifetime is always a **subpool** of xctx — different from those unit full pools.
- Object **property values** (etc.) may need to **outlive** the container → value RC / clone_or_reference, not only container pool free.
- Case-by-case validation; no single pattern for everything.

### 2026-07-23 — assign to variables: value owns needed lifetime

- In compiled-unit eval, assigning to a scope variable: always an **`afw_value`**; anything important to that value must be covered by **the value’s** lifetime (inf methods + hidden pool/object RC).
- **`afw_value` hides complications**; hope **pools/subpools + value methods** are the only abstractions assign/scope need.

### 2026-07-23 — “adaptive values are a big deal”

- Confirmed principle (also said in other conversations): **`afw_value` is central to AFW**, not peripheral. Memory/eval/escape design orbits values + pools.

### 2026-07-23 — pause; resume plan

- Break; keep suggested order with **step 0 = audit data_type_bindings / generated create+release APIs** before container/assign work.
- Old branch = ideas only for object/array value identity once bindings are trustworthy.
- Open questions listed under Cross-cutting for next session.

### 2026-07-23 — partner workflow for complex #2

- Per step: discuss → plan → tweak → execute when told → commit/push when both satisfied → reassess.
- Same phase order as manual work; think ahead so each step aligns with later ones; use full MM discussion.

### 2026-07-23 — phase 0 analysis (data_type_bindings audit)

See **Phase 0 findings** section in this file (generator + generated C vs model + old branch). Discussion, not execute.

### 2026-07-23 — proposed step −1: use permanent values not re-create

- strings.py emits **static const** Adaptive values (`afw_v_*`, etc. in `afw_strings.h`); older code often still **`create_unmanaged_*` / `set_property_as_string`** when a static const already exists.
- Prefer those static const values over allocate/create — less alloc, “permanent = no pool.” Only when an `afw_v_*` (or typed permanent) already exists.

**`strings.txt` naming subtleties (for −1 and general use):**

| Form | Example | Generated (among others) |
|------|---------|---------------------------|
| name = value when name must be valid C | `a_empty_string=` → value `""` | `afw_s_a_empty_string`, **`afw_v_a_empty_string`**, `afw_z_…` |
| | `a_period=.` | `afw_s_a_period` / **`afw_v_a_period`** (value `"."`) |
| `dataType::name=value` non-string | `integer::zero=0` | **`afw_integer_v_zero`** / `afw_integer_self_v_zero` (integer permanent, **not** string `"0"`) |
| | `boolean::true=true` | **`afw_boolean_v_true`** (boolean) — distinct from string **`afw_v_true`** (`"true"`) |
| | `integer::one=1`, `double::1=1.0e1` | `afw_integer_v_one`, `afw_double_v_1` |

`a_` prefix convention = C-safe label when value is empty, punctuation, or differs from the identifier. Step −1 must pick the **right type** of permanent value (boolean vs string `"true"`, integer zero vs string `"zero"`).

### 2026-07-23 — null / undefined / XACML / address identity

- Manual singletons in `afw_value.h`: `afw_value_null`, `afw_value_undefined`, `afw_value_unique_default_case_value` (address uniqueness matters).
- C NULL ≈ undefined for values (use macros); Adaptive null is a real typed value; address compares in C are intentional.
- Data type zoo + many functions: **XACML v3 mapping** history + later **ECMAScript-like** script — two worlds; don’t make null/undefined worse.
- Phase 0 must not casually invent more null instances.
- Manual `afw_value.h` entries predate `dataType::` strings; **keep** them for Doxygen/public highlight even where generate could also emit similar permanents.

### 2026-07-23 — special types for TS-like compiler checking

- `any` / `unknown` / `void` / `undefined` special: type-system support; syntax in Type/OptionalType/annotations; enforcement incomplete (future #28-ish).
- Default missing type → `any`; `void` return; `type X = …`; flags like `compile:noImplicitAny` registered.

### 2026-07-23 — stash notes for typechecking in ~1 month

- Full map under **Future: compile-time type checking** in this file (paths, special types, null/undefined, flags, XACML, what not to break during #2 MM).

### 2026-07-23 — step −1a executed

- Replaced 19 `afw_object_set_property_as_string(..., afw_s_NAME)` with `afw_object_set_property(..., afw_v_NAME)` in action/request/adapter/environment/command.
- Rewrapped those call sites to ~80 columns (maintainer preference).
- **C line width ~80:** also recorded in `.cursor/rules/afw-c-runtime.mdc` so other sessions pick it up.

### 2026-07-23 — step −1b executed

- Generator: `set_property_as_boolean` → `afw_value_for_boolean` (permanent true/false).
- Generator: `set_property_as_integer` → `afw_integer_v_zero` / `afw_integer_v_one` for 0/1, else unmanaged create.
- Regenerated boolean/integer bindings; convention note in `afw_value.h` + `afw-value-memory.mdc`.
- No further obvious `create_unmanaged_string(afw_s_*)` call sites; dynamic strings left alone.
- **Lexicon:** avoid “catalog” in **source**/generated comments for these; prefer static const / permanent. Chat is fine.

### 2026-07-23 — afwdev generate: other generators add to permanent string bag

- Documented under **Const / permanent → Cross-generator registration**: `options['const']` bag + `get_string_label`; `const_objects` / `function_bindings` register property names, literals, and typed scalars before `strings.generate` emits `afw_strings.*`.
- Useful for −1: permanent reuse is one shared pipeline (not only hand `strings.txt`); order is load-bearing; `additional_generate` is after strings emit today.

### 2026-07-24 — phase 1c executed

- **`data_type_bindings.py` object managed:**
  - `optional_release`: `afw_object_release`; free heap header only when `!embedded` and value RC hits 0; **never free dual face**.
  - `get_reference` / `clone_or_reference`: `afw_object_get_reference`; heap wrappers also bump value RC (multi-ref free).
  - `create_managed_object`: require non-NULL `internal` (`AFW_THROW_ERROR_Z`), then `afw_object_get_reference` before allocating wrapper.
- **array managed (parallel):** throw if `!internal`; `afw_array_release` on release; no container `get_reference` (API gap); heap RC for free only; never free embedded.
- **memory array:** dual face → **`unmanaged_array_inf`** (pool-owned instance; array release often no-op; avoid free-header on embed). Memory **object** already managed/unmanaged by options (1b′).
- **Why:** naive create returning `->value` SEGV’d because managed optional_release freed the **embedded** header. 1c makes dual-face managed/unmanaged safe under paired clone_or_reference / optional_release.
- **Not 1c:** create policy matrix (still **1d**); assign/scope rollout; array container get_reference API.
- Tests: **3079** passed (`afwdev test -j`).

##### 1c durable rules (for 1d / assign / later)

1. **Embedded vs heap:** `embedded = (container && container->value == instance)`. Dual face embeds plain `afw_value_object_t` / `afw_value_array_t` — **no** `reference_count` field. Never cast dual face to `*_managed_t` for RC; only heap wrappers from `create_managed_*` are managed headers.
2. **Object hold = object RC:** managed value `get_reference` / `optional_release` call `afw_object_get_reference` / `afw_object_release`. Value RC on heap wrappers only decides **when to free the wrapper**, not object lifetime.
3. **Protocol:** dual-face managed `optional_release` always releases the object once — must pair with a prior get_reference (or create hold). Unpaired release over-releases (no longer SEGV-on-free-embed).
4. **create_managed null:** do **not** soft-skip NULL `internal`. Core rarely checks; soft path builds a managed face that misbehaves later. **Throw** at create (`AFW_THROW_ERROR_Z`). Same for managed array.
5. **Array gap:** no `afw_array_get_reference`; managed array clone is identity + heap RC only. Memory arrays use **unmanaged** dual face.
6. **1d create policy:** when returning existing `->value`, match face lifetime; if returning managed dual face as create result, ensure hold/protocol matches caller’s optional_release expectations (likely get_reference on return, or document non-owned return). Prefer identity over second heap wrap when face already correct.

### 2026-07-23 — phase 1b′ executed

- **value_meta:** set `pub.value` to `meta_object_value` (was missing).
- **object meta (`impl_set_meta_object`):** `value.internal` was the **entity** — fixed to **meta_self**; face **unmanaged** (pool-owned).
- **memory object:** managed vs unmanaged create option selects **managed/unmanaged_object** value inf.
- **property_meta / fcgi properties:** unmanaged dual face (pool/xctx-owned).
- **arrays:** const_array, view_of_c_array, meta_values_* use **unmanaged_array** face (embedded/pool; avoid free-header). Memory array still managed in 1b′; **1c** moves it to unmanaged.
- **const meta / registry / generate const_objects:** already set value (static init); no code change.
- Tests: **2832** passed. Create policy still **1d**.

### 2026-07-23 — phase 1 reordered: 1b′ before create policy

- **1b′ plan:** finish non-NULL `->value` + correct lifetime inf on **all object and array** impls (and meta objects) before changing create_*.
- Avoid temporary create special cases (NULL / permanent-only) that must reverse later.
- Old 1b create experiment: educational (managed embedded free crash); **do not land** before 1b′/1c.

### 2026-07-23 — phase 1b draft (uncommitted; discuss) — superseded order

- `create_managed/unmanaged_object`: if `internal->value` and inf is **`permanent_object_inf`**, return it (no double-wrap).
- **Naive** “always return `->value`” **segfaulted** (~6 tests, -11): memory objects embed **managed_object** value; `optional_release` can **free the embedded header** inside the object.
- Broader “any value with `optional_release == NULL`” also failed (likely unmanaged embedded faces); **permanent-only** → **2832 pass**.
- Full identity for managed memory objects waits on **1c** (container-aware release/clone).
- 1a pad commit: `fd09f2ea`. Create-policy 1b not committed; **1b′ first**.

### 2026-07-23 — rollout: safe incremental vs scope/assign switch

- xctx FIXME / incomplete clone_or_reference call sites: keep-working choice during large scope/#2 work, not a dead end.
- Do object/array identity and create fixes **without** waiting for full scope assign; those should not break if additive.
- Break-glass risk mainly when enabling full scope lifetime + assign (`afw_function_compiler_script.c` etc.).
- Later: temporary ifdef for “new regime” in local dev if needed; prefer always-on safe changes when possible.

### 2026-07-23 — value impls + clone_or_reference strategy

- Inventory: ~17 hand value kinds + full generated DT permanent/managed/slice/unmanaged matrix; discovery via `afw_value_impl_declares.h`.
- **closure_binding** already implements real clone/release; most expression/compile kinds still NULL.
- Plan: containers call clone_or_reference/optional_release; special cases live in those methods per value kind.
- Related: xctx still has FIXME to switch to `afw_value_clone_or_reference` when ready.

### 2026-07-23 — note: mixed lifetimes inside objects/arrays

- Managed and unmanaged containers will hold property/element values of **all** lifetime kinds.
- Expect correctness mainly from value lifetime methods used at set/clone/release/escape; keep revisiting edge cases as phase 1–2 land.

### 2026-07-23 — phase 1a inventory executed (not committed)

- Full tables under **Phase 1 plan → 1a results**: object/array impls, value fill patterns, create call counts.
- Most impls already set `value`; gaps: value_meta pub, runtime_const_meta consumers; registry uses static init.
- Corrected: **managed memory objects** ≈ `afw_object_create` (~72) etc.; **`afw_value_create_managed_object` ≈ unused**. ~97 unmanaged **value** wraps; ~60 array value wraps.

### 2026-07-23 — phase 1 plan drafted (objects first, multi-step)

- Steps **1a–1f** under Phase 1 plan: inventory → object value-create → memory reference impl → fill value → hot call sites → array.
- **1a discovery:** `afw_*_impl_declares.h` + `AFW_IMPLEMENTATION_ID`; one file may implement multiple interfaces; **same header may be included multiple times** with redefine between for multi-impl of one interface — inventory by include site / id, not by file only.

### 2026-07-23 — phase 0d: phase 1 handoff written

- Full **0d** section: dual create surface, `->value`, catch-22 wrap, permanent `empty_array_value` vs mutable compile `[]`/`{}`, do/do-not, phase 1 order, success criteria.
- Phase **0 complete** (docs); next is phase **1** discuss/plan when ready.

### 2026-07-23 — do / do-not: empty managed object|array value creates

- Process: always record **what we do** and **what we explicitly will not do** in Decisions / Non-goals.
- Checked `data_type_bindings.py`: no residual of abandoned empty-managed object/array **value** create helpers (gone with old #2 branches).
- **Decision: do not reintroduce** those generates. Keep dual create surface; keep permanent empty **array of dataType** / `impl_value_empty_array_of_*`.

### 2026-07-23 — phase 0c+: boolean create → permanent true/false

- `create_managed/unmanaged_boolean` return **`afw_value_for_boolean`** (permanent dual; intentional, documented).
- `set_property_as_boolean` already used permanents (−1b).
- `allocate_unmanaged_boolean` left as pool header; Doxygen prefers permanents.
- **Note for later:** `allocate_*` APIs in general can be problematic — revisit as a group, not only boolean/null.

### 2026-07-23 — phase 0c: null create → permanent singleton

- `create_managed_null` / `create_unmanaged_null` return **`afw_value_null`** (no alloc); `set_property_as_null` sets the singleton.
- `allocate_unmanaged_null` left as pool header for rare writable cases (documented not the singleton).
- **unevaluated** / **function**: keep create APIs; unevaluated is real payload wrapper for compile graphs; function values mostly permanent from generate. No `special: true` without a broader type-system plan.
- Did **not** mark null special (would drop create APIs entirely; singleton routing is enough for identity).

### 2026-07-23 — phase 0b: binding comments match behavior

- `data_type_bindings.py`: fixed unmanaged/managed/permanent/slice inf and create Doxygen to match impl (clone_or_reference as-is vs bump RC; RC starts at 0; pointer managed stores pointer only).
- Regenerated `afw_data_type_*_binding.*` — comment-only contract clarity for phase 1 (no semantic create/release change).

### 2026-07-23 — phase 0a: type × lifetime matrix

- Inventoried all 35 `_AdaptiveDataTypeGenerate_` types vs `data_type_bindings.py` branches.
- Families A–F (special / utf8 / memory / struct embed / small direct / pointer direct); full matrix under **Phase 0 — 0a**.

### 2026-07-23 — step −1c executed (generate const reuse)

- **`const_objects.py`:** boolean / integer / double property values no longer emit per-property `static const afw_value_*_t`; they use `get_string_label` → shared bag (`afw_boolean_self_v_true/false`, `afw_integer_self_v_zero/one`, …). object/array wrappers stay per-instance.
- **`strings.seed_from_strings_dir`:** load `generate/strings/*.txt` into `options['const']` **before** function_bindings / const_objects so preferred labels (`boolean::true`, `integer::zero`, …) win; value-only registration reuses them (no more parallel `integer_v_0` + `integer_v_zero` from auto labels).
- **Do not** value-dedupe when `labelPreference` is set — `a_*` aliases intentionally share text with other labels (e.g. `a_decision_not_applicable=notApplicable` vs `notApplicable`).
- Hand fix: `afw_compile_parse_script.c` `afw_integer_v_1` → `afw_integer_v_one` (numeric label no longer auto-emitted when `one` is seeded first).
- Spot-check: ~930 local boolean property statics removed; properties point at `&afw_boolean_self_v_*.pub`; `afw_const_objects.c` smaller by ~5.6k lines.

### 2026-07-24 — Value Lifetime Model (target) recorded

- Parallel brainstorm captured under **Cross-cutting → Value Lifetime Model (target)**.
- Core: `clone_or_reference` → always permanent or managed; scope/managed-container storage only permanent/managed; unmanaged object/array escape via **managed wrapper** + pool pin; scope and managed container **release walks**; property get lazy-promotes.
- Does **not** change phased plan order (1d still next); marks highest-priority long-run pieces for phase 2-ish work.
- Decisions table updated; some draft invariants marked superseded/refined.

### 2026-07-24 — 1d plan revised under target model

- Earlier 1d draft overridden: create identity ≠ full escape story; unmanaged+managed create → **wrapper + pin `instance->p`**, not object_get_ref no-op.
- create_unmanaged still returns dual face (borrow) to kill double-wrap; create_managed P/M → face+hold.
- Full clone_or_reference / assign / scope walks stay phase 2+.

### 2026-07-24 — Pause / PR into mgg-develop

- Landing **−1 through 1c** (+ pad) to `mgg-develop` while 1d+ is thought through (~week).
- **1d not coded** yet; plan in pad only.
- `whats-new.md`: α/β + **recompile** out-of-tree commands/extensions against new libafw.
- Resume: 1d.0 sign-off → implement create policy under target model.

_(Append dated notes as we talk; fold durable points up into **By area** / **Cross-cutting**.)_
