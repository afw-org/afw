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
- Compile-unit scalar literals (integer / double / string): `compile_literal_*` inf — **as-is** in slots; `clone_*` **copies**. `true` / `null` / `undefined` / `0` / `1` / `""` stay process permanents. Eval temps stay unmanaged-promote. Compiler-only `afw_compile_literal_<dt>_create()`.
- `afw_pool_release_value_at_cleanup`: extra pin that is not a slot.

Same 14,336-iteration nest (`i1<7`, `i2<8`, `i3<16`, `i4<16`), one machine, three `afw` binaries. `concat` is `hex[i1]+hex[i2]+hex[i3]+hex[i4]`.

| Body | Pre-#277 (`1271992b`) | develop (`ef57b8f7`) | This branch |
|------|----------------------:|---------------------:|------------:|
| `n = n + 1` only | 0.024s | 0.024s | 0.024s |
| `let uu = "abcd"; n = n + 1` | 0.029s | 1.744s | 0.027s |
| `let uu = concat; n = n + 1` | 0.042s | 2.904s | 2.902s |
| original concat + `last` + `n` | 0.044s | 3.295s | 3.226s |

Literal slot fill is back to pre-#277. Concat temps still promote.

**Later (not now):** unique managed concat string **and** unique managed integer last_return in the same loop body (~3s). Each alone is cheap. Heap free list is address-ordered insert + first-fit; mixed sizes may walk a growing list (~14k²). Tune how pool deals with free memory for different sizes. Empty `for` / `hex[i]` / concat-only are fine.

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
