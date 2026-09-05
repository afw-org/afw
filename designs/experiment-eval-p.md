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

**Compile is not eval scratch.** A compile unit is an immutable bag with its **own heap**. `afw_pool_create(tracker)` (the extra rule) is the wrong shape for compile: a child tracker extra-holds the frame, so the frame never last-releases. Parent the compile heap at `evaluation_heap` / `xctx->p`, not `scope->p`. `compiled_value` already has `p`; `get_reference` / `release` should pin that pool (unmanaged_new_p style). `eval<script>` / friends: compile, evaluate (`clone_unmanaged` result into dest `p`), **release the unit**. That is consume.

Do **not** `afw_pool_destroy` from `execute_eval_script` as the API. An earlier probe did that to prove lifetime-must-end; `shared->temp_p` extra-hold is why a single `release` is not enough today.

## What was parked

`FIXME_GET_IT_WORKING` in `afw_value_block.c` was undefined, so eval `p` was **caller `p`** (usually the request / `afw` heap). That is why BMP finished on `develop`: ~14k nested `for (let …)` + two `eval<script>` per inner trip bump-alloc on a heap. RSS grew linearly. The test ended.

Earlier probe of `scope->p` hung `comments-bmp-slash-0.as` (tracker extra-hold of compile units). A later probe SIGSEGV’d on auth deny and emptied curl JSON (escaped values still pointing into a released tracker).

## Candidate order (re-decide after each)

1. **Eval `p` = `scope ? scope->p : p`.** Rip the ifdef. One site. Measure with 03-eval-p, then `language/script`, then a time/RSS-capped BMP extra.
2. **Compile unit owns a heap** (not a child tracker of the frame). One site: parser create. Not a splat across `execute_*`.
3. **`compiled_value` hold/release of its pool**, then `eval<script>` consume. Slot overwrite of `compile<script>(…)` can then end the previous unit. Parser work area (`afw_xctx_calloc_type` today) belongs in that pool or is freed in `parser_finish_and_release`.
4. **Then** `./afwdev build --fulldev`, `afwdev test -j`, `afwdev test -j --env-mode valgrind`. BMP extra is a canary, not a reason to keep parking. Auth deny / curl JSON are the escaped-value canaries — fix the inf that should have isolated, not a caller special case.
5. **Still not this branch unless the list of violators is one protocol hole:** `double_free_throws` skip; unevaluated clone-out of script_function / closure; Adaptive `clone()`; `qualifier("current")` snapshot tail.

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
