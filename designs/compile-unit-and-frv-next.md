# Compile unit, leave, isolate-at-clone, builtin lifetime, pop temp-on-scope (landed); FRV leftover dropped (`issue-2-frv`)

**Audience:** maintainers. Not user docs (`whats-new.md` notes `slice` / `map` stay mutable).

Lifetime story: [`lifetime-principles.md`](lifetime-principles.md). This pad is landed history (compile unit, leave, isolate-at-clone, builtin lifetime, pop, FRV dropped). Extra-hold in this file means register last-release on dest `p` as it landed then. Current isolate dest: dest `p` passed to `script_result_set` (`scope->p` at deactivate, `original_scope->p` at clone). `xctx->script_result` is the running pointer. There is no dest-pool field on the xctx.

**Landed on `develop`:**
- [PR #305](https://github.com/afw-org/afw/pull/305) (`0fc0f2b8`, 2026-09-09) — `compile()` is a unit; `app::` get of compiled templates.
- [PR #306](https://github.com/afw-org/afw/pull/306) (`5d5b0096`, 2026-09-10) — every `{ }` is a scope; `last_statement_non_void_value` on the running frame; leave path.
- [PR #307](https://github.com/afw-org/afw/pull/307) (`0c0816de`, 2026-09-10) — isolate last at `for (let)` clone; wrap unbraced loop bodies after parse in the current block.
- [PR #308](https://github.com/afw-org/afw/pull/308) (`9a79eeb7`, 2026-09-10) — `get_assignable_for_lifetime` vs `set_last_statement_non_void_value_for_lifetime`; mutating builtins hold the instance first; new array results `create_managed` then fill (`get_assignable_for_scope_lifetime` on those results was extra-RC; extra-hold only). `array()` / `create_array()` stay script wrappers.
- [PR #309](https://github.com/afw-org/afw/pull/309) (`b484813f`, 2026-09-11) — managed `pop`/`shift` transfer, then `afw_pool_release_value_at_cleanup` on the current scope (temp). Contract: `afw_array_create_managed`. Soak **flat**. Do not `get_assignable_for_lifetime` on the pop result.
- **`issue-2-frv`** (off `reduce-apr-pool`, 2026-09-13) — no `function_return_value` wrapper. Script return is `get_assignable_for_p_lifetime` on the **caller**. Closures are managed (`is_managed` inf flag). `destroy` is storage-only; `run_cleanups` first (`xctx_release`). `register_cleanup_before` → `register_cleanup`. Verify: `afwdev test -j` and `afwdev test -j --env-mode valgrind` **4484 passed**, 71 skipped.

**Verify for #309:** `./afwdev build --fulldev`, `afwdev test -j`, and `afwdev test -j --env-mode valgrind`: **4449 passed**, 71 skipped.

Do **not** start with “implement a fix” unless you share the plan. Open with “what do you think?”

---

## What #305 decided

- `compile()` of script/template/test_script returns a **`compiled_value` unit**. JSON compile returns an already-evaluated value (no wrapper).
- **Evaluate of the unit runs it** (`optional_evaluate` on `compiled_value`). **`get_assignable` of the unit is the unit** (assignable face). Do not extra-`evaluate()` after you already have the compile result.
- Extra hops **removed** from assign, formals, callee, spread, destructure. **Kept** in Adaptive `evaluate()` / `evaluate_with_retry` / `safe_evaluate` (evaluating a *variable* only yields the occupant).
- **`app::` / handler `qualifiedVariables`:** `afw_compile_templates` at conf load. Qualifier **get** (`impl_get_object_variable_cb`) `evaluate()`s **only if `is_compiled_value`**. That get_cb is **shared** with `current::`, `adapter::`, `environment::`, `request::`, `application::` — always-`evaluate()` broke `qualifier("current")` (allocate memory / onGetProperty). Contribute matches get for compiled units.
- Three times (templates): `#{…}` compile, `${…}` on get, function from `#{…}` on **call**. Tests in `src/afw/tests/miscellaneous/substitution/`.
- Model **`on*`** / log **`filter`:** scripts, C `evaluate()` of the pointer at hook/write. Path conf: compile **and** evaluate at configure. Model **`custom::`:** templates, evaluate on `custom::` get.
- Log conf **`custom`** removed (never wired). Unused adapter `impl->custom_variables` removed.

**Rejected that wave:** `afw_value_evaluate_create` wrapper; `evaluate_program` host hunt; making `compiled_value` evaluate identity (that forced a second run API everywhere).

---

## What #306 decided (leave path)

Rails: [`issue-2-hold-in-inf.md`](issue-2-hold-in-inf.md) (*Frame, last_statement_non_void_value*). Tests: `language/script/for.as` (`for-let-break-keeps-previous-last`), `script_result.as`, `test262/statements/try.as` (`completion-values-fn-finally-normal`).

**Every remaining `{ }` is a scope.** Runtime does not skip 0-name blocks (the old #245 skip). Compile omits a `{ }` with **no names and no statements** (statement `{ }`, and empty function / `catch` / `finally` bodies). `{ stmt }` stays a frame so temps die with it. `iter_p` is gone; braced loop bodies are frames. Tests: `language/script/empty_block.as`.

**`last_statement_non_void_value` lives on the scope.** Pointer at the last non-void statement. Valid while `scope->p` is alive (not a managed slot like `frame_slots[]`). Starts void. `afw_pool_scope_set_last_statement_non_void_value` ignores void / NULL / no current scope. `xctx->script_result` is the managed isolate; it does not require the current scope.

**Deactivate** `script_result_set`s `last_statement_non_void_value` unless the scope was **cloned**. Isolate out of the frame is that slot_store, dest `scope->p`. Nested `compiled_value` / script call / block-as-value still save/restore `script_result`.

**Extra-hold** (`afw_pool_scope_set_last_statement_non_void_value_for_lifetime`): `get_assignable_for_lifetime` then store `last_statement_non_void_value`. Use when an unmanaged occupant must survive a dying scope. Nested `{ }` adopt (child tracker already died) and `return()` (parameter can be an FRV leftover the eval stack would drop). Not at clone. Not a slot replace on every statement. `try`/`finally` extra-hold is a filtered `{ }` adopt, not the FRV door.

**Built-in returns ([PR #308](https://github.com/afw-org/afw/pull/308)):** `afw_pool_scope_get_assignable_for_scope_lifetime` is `get_assignable` plus last-release on the nearest scope in dest `p`’s parent chain (throws at a job heap, `p == p->managed_p`). It does **not** write `last_statement_non_void_value`. Mutating builtins (`push` / `unshift` / `add_entries` / `add_properties` / `freeze`): hold the **instance** first (that helper), write that `internal`, return that value. New array results (`bag` / `filter` / `map` / `sort` / `slice` / `reverse` / …): `create_managed` (RC 1), `afw_pool_scope_release_value_at_cleanup` only, fill, return; stay **mutable**. Do not wrap `create_managed` in `get_assignable_for_scope_lifetime` (extra bump, leftover RC). Language constructors `array()` / `create_array()` stay `create_script_wrapper` in `x->p` (temps; `[i]` compiles to `array()`). `pop`/`shift` still return the occupant, not the array. Do not wrap unmanaged at the return of compiler `test_value` / `qualifier()` / `parse_uri` (those are temps in `x->p`). `test_script` / `test_template` copy-out is `create_managed_clone` + extra-hold only. `clone()` of object/array is always-copy `create_managed` + extra-hold of the container (not `afw_value_clone`, not snapshot / sharing `create_managed_clone`; copy meta so reconcilable/path survive; nested objects recurse; nested scalars `get_assignable` so the parent can last-release them). Do **not** extra-hold `compile()` of a unit: evaluate does not last-release it; `evaluate(compile())` and closures from that unit still need the heap. Listing path last-releases the unit after the dump is copied.

**`for` / `while` / `try` are void** except `return` / `rethrow`. Nested assignment writes last on the **running** scope. Do not C-return the loop’s last assignment.

**Loop trips** (branch `issue-loop-frames`, 2026-10-10): each trip runs in one scope that also holds its condition, increment, and for-of target, so what they make goes with the trip (they used to evaluate in the loop's dest `p`: 300+ MB at 400k trips, some loops quadratic).

- `while`, `do`, and a `for` / `for-of` with **no** `let`/`const` in the head: each trip runs in the **body** `{ }`'s scope (`impl_evaluate_trip` with a step before the statements and one after, the after also after `continue`). The scope steps are `afw_value_block_scope_enter` / `leave` / `finish`, shared with `evaluate_block`. A loop body is always a `{ }`, even empty, because the trip runs in it.
- `for` / `for-of` **with** `let`/`const` in the head: compile wraps the loop in a head `{ }` marked `is_loop_head` (decompiles as `#loop_head(...)`); the loop finds it by that mark, not by its shape. Each trip runs in a copy of it (below); the body is its own `{ }`, and an empty body is left out.
- An unbraced loop body is parsed **in** its `{ }`, where it runs; a `let` / `const` / `function` declaration there (or as an unbraced `if` / `else`) is a compile error, as in TypeScript.

**`for (let)` clone** is for closures, not a result stack:

- First trip **is** the head `{ }` (not a template).
- Next trip: sibling `scope_clone` (copy `frame_slots[]`; same `parent_lexical_scope`), made **every** trip (also with no increment, as ECMAScript). `clone()` `script_result_set`s original last, dest `original_scope->p`, then clone last stays void from create. Marks original **cloned** so deactivate does not write the slot. Increment, condition, and for-of next value and assign run on the clone so a closure still sees the old `i`. Creator-`release` the previous; it dies unless a closure `get_reference`s it.
- Loop `{ }` bodies `evaluate_block` and only point last at the occupant already in `script_result` (no extra-hold on the clone). The slot is not rewritten until this clone is cloned or it deactivates.
- Without closures, two frames: the head `{ }` until `for` ends, plus the **current** clone.
- How we know which iteration is last: the one that was **never cloned**. Do not pick a winner.

**Finally:** a **normal** finally `{ }` must not adopt last onto the parent (that overwrote `return 'try'` with `count.finally += 1`). Finally **return** still wins. Nested assignment in finally still writes last when try/catch did not return.

**Rejected this wave:** a dest `p` parameter on `deactivate` itself (hop dest — `deactivate` still takes `(scope, xctx)` and passes `scope->p` to `script_result_set`); treating `last_statement_non_void_value` like `frame_slots[]`; extra-hold “harder” on cloned-from last; extra-hold previous sibling last onto the clone (useless: isolate at clone, void last will not override the slot); wrap unbraced **before** parse (hid the `let` clash; now done, since a declaration can not be an unbraced body); `for` C-return of last; clone-first as a template; `iter_p`; isolating FRV at `return()` `get_assignable`/`slot_store` as the design (that is a later slice).

---

## After leftover wrapping (dropped)

Leftover wrapping is **dropped**. No `function_return_value`; pin on caller. Do **not** reopen unique consume, eval-stack leftover, or `#function_return_value`.

No evaluate-only call-result inf for managed built-in returns. Builtins already extra-hold on the current `{ }` ([PR #308](https://github.com/afw-org/afw/pull/308)); `pop`/`shift` are the scope-temp path ([PR #309](https://github.com/afw-org/afw/pull/309)); identity `push` stays unwrapped. `pop_value` pops the call, not leftover wrappers. A managed header may still sit in `xctx->p` until the request pool dies — that is the managed world, not a new inf.

Compile units use `afw_pool_heap_create(parent, 4k)` (own ST heap). Managed eval allocs use `p->managed_p`. **Landed:** [PR #327](https://github.com/afw-org/afw/pull/327) ([`remaining-apr.md`](remaining-apr.md)). Gate 2026-09-14: **4484 passed** (`fulldev` + `test -j` + valgrind).

---

## FRV sitting (2026-09-13) — landed on `issue-2-frv`

**Branch off `reduce-apr-pool`**, not `develop`. Did **not** merge `issue-2-frv-leftover`.

**Shipped:** no `function_return_value` type. Script function produce path is `get_assignable_for_p_lifetime` on `scope_of_caller` while the callee frame is alive. `return()` is `set_last_statement_non_void_value` (pointer); isolate-up is that pointer → managed `script_result`. Nested `{ }` that wrote `script_result` **clears** parent last (do not plant the occupant). Try `keep_return` extra-holds a real `return` so a normal finally does not drop it. Closures are managed (`inf->is_managed`); pin may register on any dest `p`. `get_assignable` of managed is `get_reference` of self; unmanaged often `clone_managed`. Script/template/test_script compile returns a **managed** `compiled_value`. Evaluate last-releases the result on dest `p` of that evaluate (no `clone_unmanaged`); isolate dest is dest `p` of the write. The unit is not last-released at evaluate.

**Pool (this branch; also [`remaining-apr.md`](remaining-apr.md)):** last-`release` still runs callbacks then teardown. **`destroy` is storage-only.** **`run_cleanups`** first (`xctx_release` TRY cleanups, FINALLY destroy). Subtree marked destroying before callbacks; leftover/free after; cleanup list detached so re-entry is a no-op. `register_cleanup_before` → `register_cleanup` (do not throw uncaught in a callback).

**Verify:** `./afwdev build --cdev`, `afwdev test -j`, `afwdev test -j --env-mode valgrind`: **4484 passed**, 71 skipped.

Squash-merged [PR #326](https://github.com/afw-org/afw/pull/326) into `reduce-apr-pool` as `0bed0e4f`. `issue-2-frv` and `issue-2-frv-leftover` **deleted**. No twin leftover inf for managed built-in returns. `reduce-apr-pool` landed on `develop` as [PR #327](https://github.com/afw-org/afw/pull/327) ([`remaining-apr.md`](remaining-apr.md)).

---

## FRV next sitting (2026-09-11) — history

**Dropped.** Leftover wrapping did not land. Do not treat this section as current (no `function_return_value.c`). See **Next slices** above.

Sept 8 talk + 2026-09-11 recall. **Do not start with implement.**

**Two lifetimes, mashed today.** The **occupant** is the returned value (`return` already `as_assignable`s into `script_result`; wrap extra-holds it so it outlives the next `script_result` replace and the dying callee frame). The **wrapper** is only the token “this just returned.” Wrapper RC is `get_reference` / `release`. At 0, `optional_release` already `release`s the inner and `free_memory`s the header.

What landed instead: unique `get_assignable_value` **transfers the occupant, sets wrapper RC 0, and does not `free_memory`**. Public `evaluate()` peels so callers never see FRV. `consume()` is the same peel at the host. Occupant lifetime and wrapper lifetime are one path. That is the mush.

**The simple hole:** last `optional_release` already does the right RC-0 work. Unique consume abandons the empty header in `self->p`. Before every `{ }` was a scope, that was `evaluation_heap` (request-lived) and `function_return` climbed ~150 MiB/s. After [PR #306](https://github.com/afw-org/afw/pull/306) leftover dies with the body tracker, so the soak is **under the bar**. Leftover is still real. This sitting is protocol, not flattening 42 MiB/s.

**Intended inf (not unique consume):**
- `optional_evaluate` = evaluate / return the **inner** (peek, not consume).
- `get_reference` / `optional_release` move **wrapper** RC only.
- At wrapper RC 0: `release` inner and `free_memory` the header (already what last `optional_release` does).
- `get_assignable_value` = assignable / extra-hold of the **inner**. Wrapper stays. Do **not** unique-consume (do not transfer-and-RC-0 the wrapper).
- Enclosing call `pop_value` `release`s leftover FRVs above that call (`i = f()` is `assign` as that call: slot gets the occupant, then `assign` pop drops the wrapper). `add2(a(), b())` is two FRVs above `add2`.
- **Remove** `afw_value_is_function_return_value()` peels and `afw_value_function_return_value_consume()`.

**`xctx->script_result` is not function-return.** It is the running latest script result (blocks can change it with no `return()`). Wrap-in-the-dying-callee glues nested-eval hygiene onto FRV because `call_script_function` currently does both in one FINALLY. Wrap belongs to the **use** of a call result (almost always a parameter of an enclosing call), not callee exit. Save/restore of `script_result` around a nested eval is only so `f();` does not adopt `f`’s last.

**Do not:**
- Paper over with a helper around assign.
- Treat extra-hold at `return()` as this design.
- Unique-consume **and** leave the same pointer on the eval stack (UAF if unique consume `free_memory`s). Unique consume and “still on the stack” cannot both be true of the same pointer.
- `create_managed` in `array()` / `create_array()`. Extra-hold popped occupant in `execute_pop` (that is #309, done).

**Open (not decided) — leftover of the dropped FRV plan:**
- Who pushes the FRV — callee just before return, or the caller once it has the wrapper.
- Hosts (CLI, `test_script`) have no enclosing Adaptive call: `get_assignable` of the occupant, then `release` the wrapper — not a named `consume()` in `execute_*`.

**Decided (isolate dest):** `xctx->script_result` stays a slot (`slot_store`). Dest `p` is passed to `script_result_set`. It is not a raw pointer and not a dest-pool field on the xctx.

**Probes (of the dropped plan):** `language/script/return_temps.as`; RSS `function_return`; `script_result.as`. There is no `function_return_value.c` on this branch.

---

## Traps

- Object-push get is **not** “app:: only.” Only run **compiled units**.
- `evaluate(symbol)` does not run the occupant. Adaptive `evaluate()` is the hop for a variable holding a unit.
- Returning a function **from** `evaluate(compile(…))` skips clone-unmanaged (no evaluated-data-type clone). **Inside** a unit, `return function(){…}` is normal extra-hold → deactivate isolate.
- Do not `git add -A` while `--fulldev --clean` is rewriting `src/afw/generated/`.
- Handbook XML uses `<italic>`, not `<emphasis>` (Doxygen).
- Cloned-from deactivate that still `script_result_set`s is LIFO: first-trip last wins (0 instead of 3). Isolate at clone instead; void last on the new clone will not override the slot.
- `last_statement_non_void_value` whose only extra-hold is the xctx `script_result` slot goes stale when the slot is replaced. Extra-hold on the **scope that points at it** (unmanaged that must survive that scope’s death). Not onto the clone.
- `evaluate_statement` of a nested `{ }` adopts onto **current**. Loop `{ }` bodies `evaluate_block` so they do not extra-hold onto the clone/script. A normal finally must not adopt over a pending return.
- Wrap unbraced loop bodies **after** parse in the current block. Opening the wrapper first hides `for (let x of []) let x`.
- Do not `create_managed` in `array()` / `create_array()`. `[i]` compiles to `array()`; that spiked `array_rebind`.
- Do not extra-hold the popped occupant in `execute_pop`. Do not wrap compiler `test_*` / `qualifier()` / `parse_uri` at return (temps in `x->p`).
- Do not seal `slice` / `map` with `determine_data_type_and_set_immutable`. `freeze()` is the explicit door.

---

## Probes

- `afwdev test --test-pattern 'substitution.as'`
- `afwdev test --test-pattern 'language/script/for.as'` (includes `for-let-break-keeps-previous-last`)
- `afwdev test --test-pattern 'language/script/script_result.as'`
- `afwdev test --test-pattern 'language/script/loop_unbraced_body.as'` (`for-of-unbraced-let-same-name`)
- `afwdev test --test-pattern 'test262/statements/try.as'` (`completion-values-fn-finally-normal`)
- `afwdev test -T src/afw/tests-extra/issue-2 --show-all` — live table in `01-rss-hard-loops/README.md`. Unbraced assign / rebind / `compile_once_eval` / `array_push_pop` / `function_return` / `try_catch` are **flat**. Leftover wrapping is gone (pin on caller).
- Full PR bar: `./afwdev build --fulldev && afwdev test -j && afwdev test -j --env-mode valgrind`
