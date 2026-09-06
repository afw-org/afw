# Implementation plan: two methods + assignable inf

**Dated plan — do not execute again.** V1–V7 already landed on `develop`. Current two worlds / create names / last_return: [`experiment-brainstorm.md`](experiment-brainstorm.md) ([#277](https://github.com/afw-org/afw/issues/277) closed). Inf-method rails: [`issue-2-hold-in-inf.md`](issue-2-hold-in-inf.md). Eval `p`: [`experiment-eval-p.md`](experiment-eval-p.md) ([PR #287](https://github.com/afw-org/afw/pull/287)).

Present-tense “today `slot_store` → `add_reference`” below is the **before** state. V6’s Adaptive `clone()` line (structural copy then `get_assignable_value`) is **not** the tree: Adaptive `clone()` is still the old `afw_value_clone()`.

Rails: [`designs/issue-2-hold-in-inf.md`](designs/issue-2-hold-in-inf.md) **Locked design**. This file is history of **how**, not a queue.

Script and model authors keep the same behavior except `#110` identity (`property_get({}, "m", def)` shares `def`) and retrieve/`clone()` wrap at slot fill (reconcile looks through). C may change.

Re-decide after each vertical. If a step needs a type `if` at a call site, stop.

---

## Constraint: do not split hold vs wrap before slots call assignable

Today `slot_store` → `add_reference` → `clone_or_reference`, and unmanaged object **wraps**. If `get_reference` of a bag becomes **self** while slots still call `get_reference`, isolation of `{a:1}` dies.

**Order is load-bearing:** add `get_assignable_value` (default = current hold/wrap) → switch slot fill to it → **then** split bag `get_reference` = self vs assignable = wrap.

---

## Vertical 1 — Interface: two methods, behavior unchanged

**XML** (`src/afw/generate/interfaces/afw_interface.xml` `afw_value`):

- Rename `clone_or_reference` → `get_reference` (XML description: keep alive; matching `release`; no wrap of bags).
- Add `get_assignable_value` (same params as `get_reference`; description: slot occupant; matching `release`).

**Generate** (`./afwdev build --cdev`):

- `afw_value_impl_declares.h`, call macros, generator `#define`s.
- Graph infs: both NULL (already `#define impl_afw_value_clone_or_reference NULL` → `get_reference`; add assignable NULL).
- Data-type bindings: default `#define impl_afw_value_get_assignable_value impl_afw_value_get_reference` (today’s clone_or_reference body).
- `script_function`: keep current body on **both** for this vertical (still `closure_binding`). Split in V5.
- Compatibility: `afw_value_clone_or_reference(...)` macro → `afw_value_get_reference(...)` so leftover C compiles.
- `afw_value_add_reference` stays NULL-safe `get_reference` for this vertical.
- Add `afw_value_get_assignable_value()` NULL-safe helper (missing method = as-is, like add_reference).

**Tests:** `language/script` still 452. No behavior change.

---

## Vertical 2 — Slot fill uses `get_assignable_value`

Still no split of bag wrap vs hold.

- `afw_value_slot_store`: store `get_assignable_value(incoming)` instead of `add_reference`. Same-pointer skip stays. Donate stays until V7.
- Face overlay SET already goes through `slot_store`.
- Frame assign already `slot_store`.
- `reference_by_key` LHS: `get_assignable_value` of aggregate (today `add_reference`).
- Do **not** change operator execute (`+` still returns a pointer).

**Tests:** `language/script`, `wrapper_property_holds`, `array_literal_wrapper`, `object_literal_wrapper`.

---

## Vertical 3 — Assignable object/array inf + split bag methods

**Mint:** memory wrapper/script-wrapper create stamps dual-face `value.inf` = `afw_value_assignable_object_inf` / `assignable_array_inf` (fourth inf in the object/array binding files, same pattern as managed/permanent). `afw_object_as_value` already returns `object->value` when set.

Assignable inf:

- `get_reference` / `get_assignable_value` = bump instance (`afw_object_get_reference`)
- `optional_release` = instance `release`

**Bag infs (unmanaged + permanent object/array):**

- `get_reference` = **self** (stop calling `object_hold` / `array_hold` here)
- `get_assignable_value` = mint wrapper (existing `create_wrapper_*`), stamp assignable inf, hold face, return `as_value`

Callers never `if (is_memory_wrapper)`. `object_hold` / `array_hold` become the bag **assignable** bodies (or inline in generated assignable method). Custom immutable array views (`metas()`, const): assignable = self (keep the `!is_memory && !setter` path **inside** array assignable only).

`object_expression` / `object_construct` evaluate already `create_script_wrapper` — that create now stamps assignable inf. No extra wrap call.

**Tests:** isolation (`array_literal_wrapper`, `object_literal_wrapper`), `wrapper_property_holds`, `language/script`. `let y = x` still shares.

---

## Vertical 4 — Drop `#110` peel — **landed**

- `property_get` / `variable_get`: return evaluated default as-is (identity). Remove `isolate_mutable_default` at those sites.
- Face GET/array materialize: `get_assignable_value` of the child, not `isolate_mutable_default`.
- Update `property_get-default-shared-constant-object` (and kin): `a` / `b` / `def` **share**. Inline `{ }` still isolated via bag assignable.
- Delete `isolate_mutable_default` if nothing remains.

**Tests:** `property_get.as`, `variable_get.as`, nested hard-edge in `array_literal_wrapper`.

---

## Vertical 5 — `script_function` split — **landed**

- `get_reference` = self (definition).
- `get_assignable_value` = `closure_binding` (today’s `create_if_needed` + hold).
- `closure_binding`: `#define` assignable = `get_reference`.

Safe only after V2 (slots use assignable). `let f = function() ...` still binds.

**Tests:** `wrapper_property_holds`, closures, `language/script`.

---

## Vertical 6 — Retrieve / `clone()` / `freeze()` — **landed**

- Retrieve/journal: stop skip-hold for reconcilable/path. Execute returns the entity; slot fill wraps.
- Adaptive `clone()`: structural copy, then `get_assignable_value`. Drop “no path” exception.
- `freeze()`: `get_assignable_value` then `set_immutable` on **that** handle (already close; confirm it uses assignable not `add_reference` if those differ).
- Reconcile: look through `wrapper_base`. No `get_base` method.

**Tests:** `clone.as`, `file_adapter/reconcile_object.as` (may start passing if wrap was the path bug), freeze list tests, retrieve-facing script tests.

---

## Vertical 7 — Call / `return` honest; donate optional — **landed**

- `return` / `call_script_function` / `compiled_value` FINALLY: occupant that **leaves** the activation is `get_assignable_value` **before** scope/heap release.
- Then drop take-donate in `slot_store` and `donate_return` if nothing needs them.
- `script_result` stays a side channel (`#62`); do not mix into the methods.

**Tests:** `language/script` (all 452), `pool_eval` / heap lifetime if present. If donate drop fails, stop — do not add a new helper.

---

## Verify (after each vertical, and before PR)

- `./afwdev build --cdev` then `afwdev test -j --srcdir-pattern afw --test-pattern 'language/script/.*'`
- Isolation + holds: `array_literal_wrapper`, `object_literal_wrapper`, `wrapper_property_holds`
- After V4: `property_get`, `variable_get`
- After V6: freeze, clone, reconcile
- Before PR: `./afwdev build --fulldev` and broader `afwdev test -j --srcdir-pattern afw`

Pad scatter is **current code**, not rails. Update it when a helper actually dies.

---

## Out of this plan

- `get_base` method
- Literal inf / constants as `object_expression`
- Scalar/object create names (already landed)
- Teaching generic objects to own properties
- `#62` `script_result` redesign
