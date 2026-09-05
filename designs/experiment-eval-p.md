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

**Compile is not eval scratch.** A compile unit is an immutable bag with its **own heap**. `afw_pool_create(tracker)` (the extra rule) is the wrong shape for compile: a child tracker extra-holds the frame, so the frame never last-releases. Parent the compile heap at **`xctx->p`**, not dest `p` / `scope->p`. `compiled_value` already has `p`; `get_reference` / `release` should pin that pool (unmanaged_new_p style). `eval<script>` / friends: compile, evaluate (`clone_unmanaged` result into dest `p`), **release the unit**. That is consume.

Do **not** `afw_pool_destroy` from `execute_eval_script` as the API. An earlier probe did that to prove lifetime-must-end; `shared->temp_p` extra-hold is why a single `release` is not enough today.

**Throw / last_return must not point at a dead tracker.** Block FINALLY last-releases the frame, then the error or `script_result` clone still walks values that lived there. `AFW_DEBUG_POOL` poison `0x0BADF00D0BADF00D` on a string `.s` is that hole. One class: isolate at the boundary (error snapshot / `get_assignable` of the result) before the frame dies — not a splat at each failing test.

## What was parked

`FIXME_GET_IT_WORKING` in `afw_value_block.c` was undefined, so eval `p` was **caller `p`** (usually the request / `afw` heap). That is why BMP finished on `develop`: ~14k nested `for (let …)` + two `eval<script>` per inner trip bump-alloc on a heap. RSS grew linearly. The test ended.

Earlier probe of `scope->p` hung `comments-bmp-slash-0.as` (tracker extra-hold of compile units). A later probe SIGSEGV’d on auth deny and emptied curl JSON (escaped values still pointing into a released tracker).

## This sitting (after #282)

Ripped `FIXME_GET_IT_WORKING`. Compile units parent at `xctx->p` (`afw_compile_create_unit_pool`). Generic unmanaged object set still stores the pointer; `call_test_script` isolates `result` / `error` before store.

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

`AFW_THROW_ERROR_WITH_DATA*` now `afw_error_set_data` → `as_assignable` before longjmp (block FINALLY would otherwise last-release the tracker under `error->data`). Auth `deny*.as` green.

Gate now **4157 passed**, 75 skipped, **10 failed** (empty JSON, not SIGSEGV):

- `type_check*.as` (3)
- `afw_curl` HTTP `http_*.as` (7)

Same class until proven otherwise: a value used after the frame tracker died. Not 10 splat fixes.

## Candidate order (re-decide after each)

1. ~~**Eval `p` = `scope ? scope->p : p`.**~~ Done.
2. ~~**Compile unit owns a heap** parented at `xctx->p`.~~ Done. Consume / hold-release still open.
3. **Throw and last_return isolate before the frame dies.** One protocol, json-elision expect-error as the canary. Auth deny / curl should move with it.
4. **`compiled_value` hold/release of its pool**, then `eval<script>` consume. Parser work area in that pool or freed in `parser_finish_and_release`.
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
