# Discovery — eval `p` = `scope->p` / `FIXME_GET_IT_WORKING`

**Audience:** maintainers / assistants. **Not** handbook.  
**Branch:** `experiment/eval-p-scope` (tip `c1ebe71f`).  
**Base:** `develop` after [#282](https://github.com/afw-org/afw/issues/282) (scope frames, compile facts, RC 1, dest `p` ripped).  
**Rails:** [`issue-2-hold-in-inf.md`](issue-2-hold-in-inf.md). Two worlds: [`experiment-brainstorm.md`](experiment-brainstorm.md) ([#277](https://github.com/afw-org/afw/issues/277)).  
**Probes:** [`src/afw/tests-extra/issue-2/03-eval-p/`](../src/afw/tests-extra/issue-2/03-eval-p/) (not default `test -j`).

## Protocol

When a `{ }` **has a frame**, statement eval `p` is that frame’s **`scope->p`** (tracker). Nested `evaluate` in a frame gets the same `p`. Temps land there and die with the frame’s last-release.

Nested `{ }` with **no symbols** is not a frame. It keeps caller `p` (already the enclosing tracker once the parent flipped).

`while` / `do_while` / classic `for` / `for_of` (not `for (let)`) eval the **body** on a per-trip tracker under `evaluation_heap` and last-release it at the end of the trip. Condition stays on enclosing `p`. `for (let)` still clones the wrapper.

**Compile is not eval scratch.** `afw_compile*()` is a C API: `shared->p`, else `parent->p` (and `parent->shared` if set), else dest `p` (`cede_p` is dest `p` as the unit; otherwise `afw_pool_create(dest p)`). Adaptive `compile()` / `eval<script>` pass **`xctx->p`**. Do not veto dest `p` inside compile.

Caller `p` owns `shared` (model `on*`, `compile_templates`). If this parser **created** `shared`, `parser_finish` releases `temp_p` (literal lookup table) so the unit can last-release. Passed-in `shared` is not managed here.

`afw_compile*()` returns unmanaged. `afw_value_release` of a `compiled_value` releases the unit pool. `get_assignable_value` stamps the assignable face when the unit is a heap (throws if dest `p` was a tracker). Untyped / `:unevaluated` assign **stores** the unit; a concrete data type extra-evaluates then `release`s a throwaway. `eval<script>` compile, evaluate, `afw_value_release`. Parser work area is calloc’d in the compile unit (dies with the unit).

`#{ }` uses the **outer `shared`** (compiles into that unit). The result may be a function that still needs the unit — no clone-out, so do not `release` a nested unit after evaluate.

**Throw:** `xctx->error_processing_count`. Throw increments; catching `ENDTRY` decrements and, at 0, runs delayed last `release`/`destroy` (`error_delaying_release_*`; deepest parent-chain first). Only trackers whose parent is a heap wait. CATCH runs before that flush; `error->data` stays a raw pointer.

## What was parked

`FIXME_GET_IT_WORKING` in `afw_value_block.c` was undefined, so eval `p` was **caller `p`**. BMP (~14k nested `for (let)` + two `eval<script>` per inner trip) bump-alloc’d on the request heap; RSS grew linearly.

Earlier `scope->p` hung BMP (compile units as child trackers extra-holding the frame). Later SIGSEGV on auth deny / curl JSON (escaped values still pointing into a released tracker).

## Landed on this branch

Ripped `FIXME_GET_IT_WORKING`. Default `afwdev test -j`: **4304 passed**, 71 skipped. `comments-bmp-*.as` **unskipped** (~2.4s for the four files; no longer ~6 min). Valgrind suite has been green on this line.

### 03-eval-p (after consume, parser-in-unit, loop scratch)

| probe | d_in_use | note |
|-------|----------|------|
| for_let_int / concat / script | ~600 | temps die with the frame |
| compile_once_for_let | 472 | one kept unit |
| while_eval | 568 | per-trip tracker |
| for_let_eval | 472 | compile unit consumed |
| for_let_compile | 2960 | one `last` occupant |
| for_let_eval_steps (after loop) | 664 | climbs during the loop, then drops |
| mini_bmp | 568 | 256×2 evals |
| classic `for` without `let` (one-off) | 424 | same scratch as while |

## Still not this experiment

- `#267` `create(tracker)` extra rule (may revisit)
- FRV dest `p` on `evaluation_heap`
- `test_script` isolate of `result` / `error` (unmanaged object set still stores the pointer)
- Destructure extra-eval of a compile unit without `release`
- [#277](https://github.com/afw-org/afw/issues/277) follow-ups: Adaptive `clone()`, `qualifier("current")` snapshot, `double_free_throws` skip, unevaluated clone-out of script_function / closure

## MUST NOT

- Helpers around assign / silent dest hop / `assignable_p` on create
- Wrap at execute / spreading `get_reference` in `execute_*`
- Managed values on a tracker
- Veto dest `p` inside `afw_compile*()` (Adaptive `compile()` passes `xctx->p` at the **call**)
- `afw_pool_destroy` from `eval<script>` as the consume API (`afw_value_release` of the unmanaged unit)
- One-off retention patches at failing tests if the list is long — restore or change the protocol
- Calling objects or compile units “bags”

## How to re-run

```bash
src/afw/tests-extra/issue-2/03-eval-p/run.sh
afwdev test -j
afwdev test --srcdir-pattern afw --test-pattern 'test262/comments-bmp'
# maintainer: ./afwdev build --fulldev && afwdev test -j --env-mode valgrind
```
