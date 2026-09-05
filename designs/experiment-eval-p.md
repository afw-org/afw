# Discovery — eval `p` = `scope->p` / `FIXME_GET_IT_WORKING`

**Audience:** maintainers / assistants. **Not** handbook.  
**Branch:** `experiment/eval-p-scope` (try things; may or may not land).  
**Base:** `develop` after [#282](https://github.com/afw-org/afw/issues/282) (scope frames, compile facts, RC 1, dest `p` ripped).  
**Rails:** [`issue-2-hold-in-inf.md`](issue-2-hold-in-inf.md). Two worlds: [`experiment-brainstorm.md`](experiment-brainstorm.md) ([#277](https://github.com/afw-org/afw/issues/277)).  
**Probes:** [`src/afw/tests-extra/issue-2/03-eval-p/`](../src/afw/tests-extra/issue-2/03-eval-p/) (not `test -j`).

## Protocol (one rule)

When a `{ }` **has a frame**, statement eval `p` is that frame’s **`scope->p`** (tracker). Nested `evaluate` in a frame gets the same `p`. Temps (`1+1`, concat, `{ }`, `[]`) land there and die with the frame’s last-release.

Nested `{ }` with **no symbols** is not a frame. It keeps caller `p`, which is already the enclosing tracker once the parent flipped.

Catch already binds the error on `scope->p`. That is the same rule, not a special case.

**Compile is not eval scratch.** `afw_compile*()` is a C API: `shared->p`, else `parent->p`, else dest `p` (`cede_p` is dest `p` as the unit; otherwise `afw_pool_create(dest p)`). Callers pick the parent. Adaptive `compile()` / `eval<script>` pass **`xctx->p`** (a heap that outlives the frame). Do not veto dest `p` inside compile.

`afw_compile*()` returns unmanaged. `afw_value_release` of a `compiled_value` releases the unit pool. `get_assignable_value` extra-holds that pool and stamps the assignable face when the unit is a heap; it throws if dest `p` was a tracker. `eval<script>` compile, evaluate, `afw_value_release`.

**Throw / last_return must not point at a dead tracker.** Block FINALLY last-releases the frame, then the error or `script_result` clone still walks values that lived there. `AFW_DEBUG_POOL` poison `0x0BADF00D0BADF00D` on a string `.s` is that hole. One class: isolate at the boundary (error snapshot / `get_assignable` of the result) before the frame dies — not a splat at each failing test.

## What was parked

`FIXME_GET_IT_WORKING` in `afw_value_block.c` was undefined, so eval `p` was **caller `p`** (usually the request / `afw` heap). That is why BMP finished on `develop`: ~14k nested `for (let …)` + two `eval<script>` per inner trip bump-alloc on a heap. RSS grew linearly. The test ended.

Earlier probe of `scope->p` hung `comments-bmp-slash-0.as` (tracker extra-hold of compile units). A later probe SIGSEGV’d on auth deny and emptied curl JSON (escaped values still pointing into a released tracker).

## This sitting (after #282)

Ripped `FIXME_GET_IT_WORKING`. Adaptive compile/eval pass `xctx->p` at the call; compile itself uses dest `p`. Generic unmanaged object set still stores the pointer; `call_test_script` isolates `result` / `error` before store.

### 03-eval-p after both flips (N as in the scripts)

| probe | d_in_use | note |
|-------|----------|------|
| for_let_int / concat / script | ~600 | temps die with the frame |
| compile_once_for_let | 472 | one unit |
| while_eval / for_let_eval | ~510–590k | throwaway compile still lives until xctx |
| for_let_eval_steps | linear ~1.8k/eval | not superlinear tracker tax |
| mini_bmp | 863k / 0.023s | was 1.6MB as child tracker; parked-heap was ~1.2MB |

Temps flatten. Compile leak is cheap linear heaps (consume still open).

### Gate (`afwdev test -j`)

After pointing parser `contextual.source_location` at the compile-unit clone (not dest `p`): compile-error canary green. BMP extra 4/4 in ~2.4s.

**Try in:** `xctx->error_processing_count`. Throw increments; catching `ENDTRY` decrements and, at 0, runs waiting last `release`/`destroy` (deepest parent-chain first). Only **scope trackers** (tracker whose parent is a heap) wait; compile heaps and `create()` of a tracker do not. CATCH runs before that flush, so `error->data` stays a raw pointer (`WITH_DATA` / Adaptive `throw` assign it; no throw-time `as_assignable` — that would be an extra hold with no matching release). Auth `deny*.as` green. Gate **4300 passed**, 75 skipped, 0 failed. BMP extra 4/4.

## Candidate order (re-decide after each)

1. ~~**Eval `p` = `scope ? scope->p : p`.**~~ Done.
2. ~~**Compile uses dest `p`.** Adaptive compile/eval pass `xctx->p` at the call.~~ Done.
3. **Throw and last_return isolate before the frame dies.** One protocol, json-elision expect-error as the canary. Auth deny / curl should move with it.
4. ~~**`compiled_value` `get_assignable_value` is a counted occupant**; `eval<script>` consumes the unit.~~ Done. `shared->temp_p` extra-hold may still keep a released unit alive.
5. **Then** `./afwdev build --fulldev`, `afwdev test -j`, `afwdev test -j --env-mode valgrind`, BMP extra.
6. **Still not this branch unless the list of violators is one protocol hole:** `double_free_throws` skip; unevaluated clone-out of script_function / closure; Adaptive `clone()`; `qualifier("current")` snapshot tail.

## MUST NOT

- Helpers around assign / silent dest hop / `assignable_p` on create
- Wrap at execute / spreading `get_reference` in `execute_*`
- Managed values on a tracker
- Shipping eval `p` because `language/script` is green if BMP still climbs
- Shipping compile-pool `destroy` from `eval<script>` because BMP is fast
- One-off retention patches at failing tests if the list is long — restore or change the protocol

## How to re-run

```bash
src/afw/tests-extra/issue-2/03-eval-p/run.sh
afwdev test --srcdir-pattern afw --test-pattern 'language/script' -j
# BMP extra is the hang canary (skipped in default test -j)
afwdev test --srcdir-pattern afw --test-pattern 'test262/comments-bmp' -T src/afw/tests-extra/test262
```
