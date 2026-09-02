# Issue #277 — unmanaged / managed two worlds

**Audience:** maintainers / assistants. **Not** handbook.  
**GitHub:** [#277](https://github.com/afw-org/afw/issues/277) (part of [#2](https://github.com/afw-org/afw/issues/2)). Last_return contract: [#62](https://github.com/afw-org/afw/issues/62) / `src/afw/tests/language/script/script_result.as`.

This is `develop` truth once [#277](https://github.com/afw-org/afw/issues/277) lands. Rails for holds remain [`issue-2-hold-in-inf.md`](issue-2-hold-in-inf.md).

## Two worlds

| | Unmanaged | Managed |
|---|---|---|
| Where | caller `p` / `scope->p` (usually a tracker) | this `xctx->p` (ST heap) |
| Death | pool bulk-free | last RC, then `free_memory` of the header in `xctx->p` |
| Role | temps, compile unit, snapshots | anything a slot / managed property / managed element **holds** |

- Managed scalars: `get_reference` / `get_assignable_value` **bump self**.
- Unmanaged scalar `get_assignable_value`: **promote** (`create_managed` in this `xctx->p`, RC 1).
- `afw_pool_release_value_at_cleanup`: extra pin that is not a slot.

When **evaluation is done**, an **evaluated** result is an **unmanaged clone in dest `p`**. Functions/closures as the compile/eval result are not cloned that way yet (follow-up).

Dest `p` is often this `xctx->p` or a tracker under it — then the result is unmanaged in a pool this xctx already owns.

## C names (no aliases)

| | Lives in a pool | Has its own pool |
|---|---|---|
| Create | `create_unmanaged` | `create_unmanaged_new_p` / `create_unmanaged_cede_p` |
| Death | that pool bulk-frees | object `get_reference` / `release` control **the pool** |

Options: `0` = live in `p`; `new_p` and `cede_p` are the two flags. All three named creates are pool-world unmanaged bags (unmanaged dual-face; **value** `get_reference` / `release` throw). Instance `get_reference` / `release` pin `object->p` / `array->p` regardless of those flags. Frames are `create_managed` (no pool).

Wrappers: `create_wrapper_unmanaged`, `_unmanaged_new_p`, `_unmanaged_cede_p`.

Clone: `afw_value_clone_unmanaged` (dest `p`) / `afw_value_clone_managed` (this `xctx->p`). Adaptive `clone()` is still `afw_value_clone()`.

**Value `get_reference` / `release`:** only permanent (as-is) and `memory_managed` (bump / last-release). Unmanaged object/array **value** infs throw. Isolate with `get_assignable_value`. Do not stamp `afw_value_managed_*_inf` on a pool-world header.

## Last_return

Assignment, `return`, and a non-void call write the running result. `let` / `const`, empty `{ }`, and `for` / `while` / `try` as statements do not. Nested assignment inside a loop **does** write it. Loops and `try` do not keep an unheld leftover last.

## Managed frames

Separate inf (`memory_managed`), alloc in `xctx->p`, RC 1. Slots: new property name is `get_assignable_value` once; value is `slot_store`. Last bag release: remaining names/elements, then `free_memory` of the header.

**Unmanaged `get_assignable_value` (object/array value)**

| Internal | Result |
|---|---|
| already `memory_managed` | bump; dual-face |
| generic `"memory"` bag, not a wrapper | `clone_managed` |
| view / wrapper / meta / other | `object_hold` / `array_hold` |

`wrap_literal_*` uses hold, not value `get_reference`. `afw_object_meta_clone_and_set` throws if the instance is `memory_managed`. Walk `first_property` only on a real memory bag.

## Follow-ups

- Eval `p` is still caller `p` (`FIXME_GET_IT_WORKING`). `scope->p` hangs `comments-bmp-slash-0.as`.
- Unevaluated clone-out (script_function, closure).
- Adaptive `clone()` still the old function.
- `qualifier("current")` snapshot list tail.
- `double_free_throws` still skipped (prefix overlay).

`AFW_DEBUG_POOL` fills freed USER with `0x0BADF00D0BADF00D` so a dangling `inf` faults on any vtable access.

## Verify

`./afwdev build --fulldev`, `afwdev test -j`, `afwdev test -j --env-mode valgrind` (4297 passed, 71 skipped).
