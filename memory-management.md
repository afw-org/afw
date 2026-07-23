# Memory management (issue #2) — working notes

**Audience:** maintainers and assistants.  
**Not user docs.** Related always-on runtime rules: [`.cursor/rules/afw-value-memory.mdc`](.cursor/rules/afw-value-memory.mdc). Beta checklist pointer: [`beta-backlog.md`](beta-backlog.md). GitHub: [#2 Memory management](https://github.com/afw-org/afw/issues/2).

## Purpose of this file

Living design / discussion notes for long-running AFW process memory: pools, value lifetimes, object/array ↔ value identity, escape (closures, retained results), and what we will implement later.

**End goal of the discussion:** a coherent **AFW memory-management story** — useful for implementing **#2** now, and as durable context for **humans and assistants** on later work (not only this issue). Prefer clear narrative and invariants over a pile of undigested archaeology.

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

### Invariants (draft — not agreed)

_Edit as we decide. Strike or mark rejected._

1. **(draft)** Every constructed object/array sets `->value` before it is published to any API that can observe it.
2. **(draft)** For a given instance, `create_unmanaged_object(obj, …)` and `obj->value` refer to the **same** value (identity), not a fresh wrapper — *if* we adopt the tip’s direction.
3. **(draft)** Managed escape of an object value increments object (or its pool) lifetime; release decrements; freeing the value header alone without the object is wrong when the value is embedded.
4. **(draft)** Evaluation temporaries that do not escape use `scope->p` (existing rule).
5. **(draft)** Shared permanent/const mutables returned as defaults still require clone-into-call-pool (existing #110 class rule).
6. **(draft / direction)** Managed object/array = **container**: on container release, release refs to **contained values** when those values may outlive the container’s pool (see lifetime patterns below). Unmanaged = no independent container refcount; die with enclosing pool.
7. **(draft / direction)** Scope create uses a **subpool of `xctx->p`**; scope end releases that subpool (with reparent of still-referenced children). Assigning into another scope uses **`clone_or_reference`** (not always deep clone) once managed values are wired.
8. **(draft / historical)** Short-lived expression/script/model work may use a single outer pool for the whole unit; long-running loops/recursion need scope subpools + managed escape.
9. **(draft / pattern)** Some containers manage lifetime by **holding a reference on their own pool/subpool** (pool RC) rather than a separate object counter — hide behind value/object release. **Case-by-case:** “all parts die together” → managed **full pool** (or unit pool) can be enough; “parts may escape” → value RC / clone_or_reference.
10. **(draft / core goal for script scopes)** When **assigning an `afw_value` to a variable** in a scope during compiled-unit evaluation, **everything important to that value’s continued correctness must be covered by the value’s lifetime policy** (via its inf: `clone_or_reference` / `optional_release`, and whatever pool/object RC the impl hides). Callers (assign, scope) should not special-case “also hold this pool / that object.” **`afw_value` hides the complications.** Preferred surface: **pools/subpools + `afw_value` methods** — hope that’s enough abstraction.

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

### Open questions (need maintainer perspective when back)

**Phase 0 / bindings**

- After audit: any data types that should **not** get full managed/unmanaged create APIs (special-only permanent)?
- For **object** and **array** data types specifically: is the end-state create path **always** return `internal->value` (with get_reference for managed), or only when `->value` is non-NULL?
- `create_managed_*` for pointer-ish types still has generator FIXME energy (“might need clone to correct pool”) — intended policy?

**Containers / assign (phases 1–2)**

- Managed object/array: value-layer `clone_or_reference` / `optional_release` as public story; hide pool vs object RC — keep impls **consistent**. Confirm when we hit code.
- Unmanaged memory object still tagged with **managed** object value inf — intentional or transitional?
- Arrays same milestone as objects or objects first?
- On managed object release: must first milestone walk properties and `optional_release` each, or is subpool bulk free enough at first?

**Later**

- **`compiled_value` escaped** (script `compile` + return): pool RC / managed unit — design when we get there.
- Hard error on missing `object->value` / `array->value` — when to flip?
- Request memory limit: charge only request-session pool tree, or escaped managed too?
- `#35` after2-* unskip bar vs step-by-step #2 — no fixed bar yet.

### Non-goals (near term)

- **Immediately** replacing hierarchical pools or ripping out APR (possible later; see APR section under Pools)
- Closing #49 / #127 inside #2 (related, tracked elsewhere)
- Hand-editing `generated/` bindings (change generator + regenerate)
- Full OOM/stack handling productization (**#64** adjacent) — noted as future; needs accounting first

### Phase 0 findings — `data_type_bindings` audit (2026-07-23)

**Scope:** Does current generator + generated `afw_data_type_*_binding.*` match the permanent / managed / managed_slice / unmanaged model? What’s incomplete vs old branch / #2 story? **No code changes yet.**

#### What already makes sense (architecture OK)

| Area | Assessment |
|------|------------|
| **Four lifetime policies** | Documented in generator header; infs wired: permanent (`optional_release` NULL, clone as-is), managed (RC + free header), managed_slice (utf8/memory only, RC on containing), unmanaged (pool lifetime, clone as-is). |
| **Special types** | `special: true` → permanent inf only, no create_managed/unmanaged. Today: **undefined, void, any, unknown** (and similar). |
| **Scalars (integer, boolean, double, …)** | Managed: store by value in xctx-allocated header, RC starts 0. Unmanaged: pool header. Permanent: static const (e.g. `afw_strings.h`). Matches “value owns payload copy” for non-pointer internals. |
| **utf8 / memory managed** | Embed bytes after header (owning copy). **managed_slice** create/release/get_reference look coherent (recent #115 work). |
| **No create_permanent_*** | Correct — permanent instances from strings/const_objects only. |
| **Old FIXME `False and` object path** | **Gone** from current generator (was never activated). No half-enabled object special case in Python. |
| **allocate_managed_* removed** | Historical #2 cleanup; only allocate_unmanaged remains. |

#### Critical gaps vs our MM model / old branch

**1. Object and array create_* still double-wrap (phase 1 territory, but bindings block it)**

```c
// generated today — does NOT use object->value
afw_value_create_managed_object(obj, xctx) {
    v = afw_xctx_malloc(sizeof(afw_value_object_managed_t), xctx);
    v->inf = managed_object_inf;
    v->internal = obj;   // pointer only
    v->reference_count = 0;
    return &v->pub;
}
// optional_release: only afw_pool_free_memory(v) — does NOT afw_object_release(obj)
```

Same shape for **array**. Old branch tip wanted: return `internal->value`, managed path `afw_object_get_reference`, release → object/array release (or pool ref). **Not present.** Memory objects already embed `->value` with managed_object_inf, but create APIs ignore it.

**2. Managed create almost unused in hand code**

Rough hand-C call counts (excluding generated):  
`create_unmanaged_object` ~**97**, `create_managed_object` **0**, `create_managed_string` **0**, `create_managed_string_slice` **0**.  
So the managed path is **scaffolding**; production traffic is still **unmanaged + pool bulk free**. Phase 0 must get the generated APIs *correct*; phase 1–2 must *use* them.

**3. Pointer `directReturn` managed types store bare pointers**

For `const afw_object_t *`, `const afw_array_t *`, `const afw_value_t *` (function), `void *` (null): managed create **does not clone** the referent; RC only covers the **value header**. Generator comment admits no automatic clone. That is **not** “value lifetime covers everything important” until object/array (and function?) special-case container RC.

**4. Managed RC starts at 0**

Create leaves `reference_count = 0`; first `optional_release` **frees immediately** if nothing called get_reference. Intentional “create and hand off” / “release without prior get_reference” semantics — document and keep consistent; easy to misuse if callers assume create ⇒ held ref.

**5. `null` is not `special`**

`null.json` has no `special: true` → full unmanaged/managed/permanent machinery and create APIs for null. Runtime still has permanent null for normal use. **Odd / likely incomplete** — candidates for permanent-only like undefined (discuss).

**6. `unevaluated` is not special**

`cType: const afw_value_t *`, not special → gets managed/unmanaged create like a normal type. May or may not be intentional.

**7. Unmanaged clone_or_reference comment vs behavior**

Generated comment: unmanaged “get_reference returns new reference”; implementation **returns instance as-is**. Misleading docs only.

**8. managed_slice free of containing**

Slice release decrements/frees **containing managed** header; does not free slice’s xctx allocation of header carefully? It frees both containing (if RC 0) and slice instance via `afw_pool_free_memory`. Slice header was `afw_xctx_calloc` — free_memory must resolve to xctx pool (prefix). OK if free_memory path is solid. Slice **get_reference** allocates **new** slice header (copy of view) — good.

#### Relative to old branch (ideas only)

| Old tip | Current tree |
|---------|----------------|
| create_*_object → `internal->value` | Still allocate wrapper |
| managed get_reference → object | Only bumps value-header RC |
| managed release → object_release | Only free value header |
| drop allocate_unmanaged_object | Still generated for object |
| throw if missing `->value` | N/A |
| registry/env permanent values | Partial / separate |

Old work is still a **valid target shape for phase 1**, not something already finished in bindings.

#### What’s “complete enough” for phase 0 vs later

| In phase 0 (bindings correctness) | Later phases |
|-----------------------------------|--------------|
| Confirm generator matrix per cType/special | Flip object/array to `->value` + container RC |
| Fix clear bugs/inconsistencies (null special?, comments, maybe unevaluated) | Migrate call sites off double-wrap |
| Ensure managed/slice RC semantics documented and consistent | assign `clone_or_reference` |
| Decide which types get which APIs | Scope release |

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
| **Discuss** | Memory story pad (`memory-management.md`); invariants; no big code yet | **paused** (good foundation) |
| **−1** | **Prefer permanent `afw_v_*` (and typed permanent values) over allocate/create when the string/scalar already exists from generate** — cleanup call sites left over from before strings.py emitted values | **−1a + −1b + −1c done** |
| **0** | **Audit `data_type_bindings.py` + generated bindings** — correct, complete, match permanent/managed/managed_slice/unmanaged model; finish gaps from recent work; use old branch tip as ideas (object/array create → `->value`, release via container) not as merge | **analysis done — discuss / plan next** |
| **1** | Managed **object/array** containers + `->value` identity (consistent impls; hide nastiness) | pending |
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

**Objects** (examples): `memory`, composite, view, meta / meta_accessor, properties_callback, const_key_value, aggregate_external, runtime indirect, FCGI request properties, env variables object, …

**Arrays** (examples): `memory`, const array of values, wrapper_for_array, …

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

### 2026-07-23 — step −1c executed (generate const reuse)

- **`const_objects.py`:** boolean / integer / double property values no longer emit per-property `static const afw_value_*_t`; they use `get_string_label` → shared bag (`afw_boolean_self_v_true/false`, `afw_integer_self_v_zero/one`, …). object/array wrappers stay per-instance.
- **`strings.seed_from_strings_dir`:** load `generate/strings/*.txt` into `options['const']` **before** function_bindings / const_objects so preferred labels (`boolean::true`, `integer::zero`, …) win; value-only registration reuses them (no more parallel `integer_v_0` + `integer_v_zero` from auto labels).
- **Do not** value-dedupe when `labelPreference` is set — `a_*` aliases intentionally share text with other labels (e.g. `a_decision_not_applicable=notApplicable` vs `notApplicable`).
- Hand fix: `afw_compile_parse_script.c` `afw_integer_v_1` → `afw_integer_v_one` (numeric label no longer auto-emitted when `one` is seeded first).
- Spot-check: ~930 local boolean property statics removed; properties point at `&afw_boolean_self_v_*.pub`; `afw_const_objects.c` smaller by ~5.6k lines.

_(Append dated notes as we talk; fold durable points up into **By area** / **Cross-cutting**.)_
