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
- **#280 landed:** lexer mints token payloads as values; parse-word strings (`name == value`, identifier-like) register as environment registry type `string_literal` (key-only, `const afw_value_string_t *`). Say **environment registry**, not “catalog”. `get_string_literal` hits that first. Keywords pointer-compare interned `afw_v_*`. Symbol names, script function `param->name`, loop labels, type/interface declaration names are interned string values.
- `afw_pool_release_value_at_cleanup`: extra pin that is not a slot.

Same 14,336-iteration nest (`i1<7`, `i2<8`, `i3<16`, `i4<16`). `concat` is `hex[i1]+hex[i2]+hex[i3]+hex[i4]`. After #280:

| Body | Pre-#277 | develop before intern | After #280 |
|------|----------:|----------------------:|-----------:|
| `n = n + 1` only | ~0.03s | ~0.03s | ~0.03s |
| `let uu = "abcd"; n = n + 1` | ~0.03s | ~1.6s | ~0.03s |
| `let uu = concat; n = n + 1` | ~0.04s | ~2.9s | ~2.8s |
| original concat + `last` + `n` | ~0.04s | ~3.3s | ~3.1s |

Literal slot fill is back to pre-#277. Concat temps still promote. Default `afwdev test -j` ~33s. BMP comment sweeps are unskipped on [PR #287](https://github.com/afw-org/afw/pull/287).

**Later (not now):** unique managed concat string **and** unique managed integer last_return in the same loop body (~3s). Each alone is cheap. Heap free list is address-ordered insert + first-fit; mixed sizes may walk a growing list (~14k²). Tune how pool deals with free memory for different sizes. Empty `for` / `hex[i]` / concat-only are fine. Possible later registry MAP flag “include in big object”; do not special-case size now. `source_location` as interned string after compile splice settles. Type-graph names (`type_property`, `type_function_param`, `reference.name`) still utf8 views.

**Next session (suggested):** heap free-list mixed sizes is the remaining eval win from the timings. Alternate: `source_location` after splice. Eval `p` = `scope->p` is [PR #287](https://github.com/afw-org/afw/pull/287) / [`experiment-eval-p.md`](experiment-eval-p.md). Restart `afwfcgi` after install (stale mapped binary).

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

`return` is `as_assignable` only (does not `slot_store`). Statement list starts void. Block finish stores a non-void last into `script_result` before deactivate. Nested eval save/restore the pointer. `let` / `const` do not write it. Nested assignment inside a loop **does**. Loops keep a body last only if it is already `script_result`; `try` stays void except `return` / `rethrow`.

## Managed frames

Separate inf (`memory_managed`), alloc in `xctx->p`, RC 1. Slots: new property name is `get_assignable_value` once; value is `slot_store`. Last bag release: remaining names/elements, then `free_memory` of the header.

**Unmanaged `get_assignable_value` (object/array value)**

| Internal | Result |
|---|---|
| already `memory_managed` | bump; dual-face |
| generic `"memory"` bag, not a wrapper | `clone_managed` |
| view / wrapper / runtime / adapter | managed look-through wrapper (preserves meta) |

`get_reference` / `slot_store` / `as_assignable` take `xctx` only (no dest `p`). `wrap_literal_*` uses that isolate, not value `get_reference`. `afw_object_meta_clone_and_set` throws if the instance is `memory_managed`. Walk `first_property` only on a real memory bag. Full clone of unmanaged objects still drops some meta; fix later.

## Follow-ups

- Eval `p` = `scope->p` when `{ }` has a frame: [PR #287](https://github.com/afw-org/afw/pull/287) / [`experiment-eval-p.md`](experiment-eval-p.md).
- Adapter clones (held).
- Clone-of-unmanaged object meta.
- FRV keep as-is unless special cases spread.
- Unevaluated clone-out (script_function, closure).
- Adaptive `clone()` still the old function.
- `qualifier("current")` snapshot list tail.
- `double_free_throws` still skipped (prefix overlay).

`AFW_DEBUG_POOL` fills freed USER with `0x0BADF00D0BADF00D` so a dangling `inf` faults on any vtable access.

## Verify

`./afwdev build --fulldev`, `afwdev test -j`, `afwdev test -j --env-mode valgrind` (4297 passed, 71 skipped).
