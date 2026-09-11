# Compile unit, leave, isolate-at-clone, builtin lifetime, pop temp-on-scope (landed); next: FRV

**Audience:** next session. Not user docs (`whats-new.md` notes `slice` / `map` stay mutable).

**Landed on `develop`:**
- [PR #305](https://github.com/afw-org/afw/pull/305) (`0fc0f2b8`, 2026-09-09) — `compile()` is a unit; `app::` get of compiled templates.
- [PR #306](https://github.com/afw-org/afw/pull/306) (`5d5b0096`, 2026-09-10) — every `{ }` is a scope; `last_result` on the running frame; leave path.
- [PR #307](https://github.com/afw-org/afw/pull/307) (`0c0816de`, 2026-09-10) — isolate last at `for (let)` clone; wrap unbraced loop bodies after parse in the current block.
- [PR #308](https://github.com/afw-org/afw/pull/308) (`9a79eeb7`, 2026-09-10) — `get_assignable_for_lifetime` vs `set_last_result_for_lifetime`; mutating builtins hold the instance first; new array results `create_managed` then fill; `array()` / `create_array()` stay script wrappers.
- [PR #309](https://github.com/afw-org/afw/pull/309) (`b484813f`, 2026-09-11) — managed `pop`/`shift` transfer, then `afw_pool_release_value_at_cleanup` on the current scope (temp). Contract: `afw_array_create_managed`. Soak **flat**. Do not `get_assignable_for_lifetime` on the pop result.

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

Rails: [`issue-2-hold-in-inf.md`](issue-2-hold-in-inf.md) (*Frame, last_result*). Tests: `language/script/for.as` (`for-let-break-keeps-previous-last`), `script_result.as`, `test262/statements/try.as` (`completion-values-fn-finally-normal`).

**Every `{ }` is a scope.** Runtime does not skip 0-name blocks (the old #245 skip). Flattening a useless block is compile-side only. `iter_p` is gone; braced loop bodies are frames.

**`last_result` lives on the scope**, not only on `xctx->script_result`. Pointer only (not a managed slot like `frame_slots[]`). Starts void. `afw_xctx_scope_set_last_result` ignores void / NULL / no current scope.

**Deactivate** `script_result_set`s `last_result` unless the scope was **cloned**. Isolate out of the frame is that slot_store. Nested `compiled_value` / script call / block-as-value still save/restore `script_result`.

**Extra-hold** (`afw_xctx_scope_set_last_result_for_lifetime`): `get_assignable_for_lifetime` then store `last_result`. Use when an unmanaged occupant must survive a dying scope. Nested `{ }` adopt (child tracker already died) and `return()` (parameter can be an FRV leftover the eval stack would drop). Not at clone. Not a slot replace on every statement. `try`/`finally` extra-hold is a filtered `{ }` adopt, not the FRV door.

**Built-in returns ([PR #308](https://github.com/afw-org/afw/pull/308)):** `afw_xctx_scope_get_assignable_for_lifetime` is `get_assignable` plus release on current `scope->p`. It does **not** write `last_result`. Mutating builtins (`push` / `unshift` / `add_entries` / `add_properties` / `freeze`): hold the **instance** first, write that `internal`, return that value. New array results (`bag` / `filter` / `map` / `sort` / `slice` / `reverse` / …): `create_managed`, hold, fill, return; stay **mutable**. Language constructors `array()` / `create_array()` stay `create_script_wrapper` in `x->p` (temps; `[i]` compiles to `array()`). `pop`/`shift` still return the occupant, not the array. Do not wrap unmanaged at the return of compiler `test_*` / `qualifier()` / `parse_uri` (those are temps in `x->p`).

**`for` / `while` / `try` are void** except `return` / `rethrow`. Nested assignment writes last on the **running** scope. Do not C-return the loop’s last assignment.

**`for (let)` clone** is for closures, not a result stack:

- First trip **is** the for-let `{ }` (not a template).
- Next trip: sibling `scope_clone` (copy `frame_slots[]`; same `parent_lexical_scope`). `clone()` `script_result_set`s original last, then clone last stays void from create. Marks original **cloned** so deactivate does not write the slot. Increment / for-of assign run on the clone so a closure still sees the old `i`. Creator-`release` the previous; it dies unless a closure `get_reference`s it.
- Loop `{ }` bodies `evaluate_block` and only point last at the occupant already in `script_result` (no extra-hold on the clone). The slot is not rewritten until this clone is cloned or it deactivates.
- Unbraced while / do / for / for-of bodies wrap as a 0-symbol `{ }` **after** parsing the Statement in the current block (`for (let x of []) let x` is still already defined). `if` is not wrapped.
- Without closures, two frames: the `{ }` until `for` ends, plus the **current** clone.
- How we know which iteration is last: the one that was **never cloned**. Do not pick a winner.

**Finally:** a **normal** finally `{ }` must not adopt last onto the parent (that overwrote `return 'try'` with `count.finally += 1`). Finally **return** still wins. Nested assignment in finally still writes last when try/catch did not return.

**Rejected this wave:** dest `p` on deactivate; treating `last_result` like `frame_slots[]`; extra-hold “harder” on cloned-from last; extra-hold previous sibling last onto the clone (useless: isolate at clone, void last will not override the slot); wrap unbraced **before** parse (hides `let` clash); `for` C-return of last; clone-first as a template; `iter_p`; isolating FRV at `return()` `get_assignable`/`slot_store` as the design (that is a later slice).

---

## Next slices (agreed order)

1. **FRV as stack leftover** — last of the leave/FRV wave. Two lifetimes (occupant vs wrapper). Unique consume mashed them. Sitting brief below. Open with “what do you think?”
2. **Runtime call-result hold** — evaluate-only inf (like `closure_binding`: display decompile, not recompile) for **managed built-in returns** if they still leftover. Identity **`push`** stays unwrapped. `pop`/`shift` are the scope-temp path ([PR #309](https://github.com/afw-org/afw/pull/309)), not this inf. Keep separate from compile-time FRV until they match.
3. **Merge 1 and 2** only if the infs are actually the same.

---

## FRV next sitting (2026-09-11)

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

**Open (not decided):**
- Who pushes the FRV — callee just before return, or the caller once it has the wrapper.
- Should `xctx->script_result` go back to a raw pointer with no `slot_store`.
- Hosts (CLI, `test_script`) have no enclosing Adaptive call: `get_assignable` of the occupant, then `release` the wrapper — not a named `consume()` in `execute_*`.

**Probes:** `language/script/return_temps.as`; RSS `function_return`; `script_result.as`. Code: `afw_value_function_return_value.c` (unique consume vs last `optional_release`); wrap site in script-function execute.

---

## Traps

- Object-push get is **not** “app:: only.” Only run **compiled units**.
- `evaluate(symbol)` does not run the occupant. Adaptive `evaluate()` is the hop for a variable holding a unit.
- Returning a function **from** `evaluate(compile(…))` skips clone-unmanaged (no evaluated-data-type clone). **Inside** a unit, `return function(){…}` is normal extra-hold → deactivate isolate.
- Do not `git add -A` while `--fulldev --clean` is rewriting `src/afw/generated/`.
- Handbook XML uses `<italic>`, not `<emphasis>` (Doxygen).
- Cloned-from deactivate that still `script_result_set`s is LIFO: first-trip last wins (0 instead of 3). Isolate at clone instead; void last on the new clone will not override the slot.
- `last_result` whose only extra-hold is the xctx `script_result` slot goes stale when the slot is replaced. Extra-hold on the **scope that points at it** (unmanaged that must survive that scope’s death). Not onto the clone.
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
- `afwdev test -T src/afw/tests-extra/issue-2 --show-all` — live table in `01-rss-hard-loops/README.md`. Unbraced assign / rebind / `compile_once_eval` / `array_push_pop` are **flat**. `function_return` stays under the bar. FRV leftover is still real in `self->p` until that `{ }` dies.
- Full PR bar: `./afwdev build --fulldev && afwdev test -j && afwdev test -j --env-mode valgrind`
