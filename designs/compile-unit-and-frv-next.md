# Compile unit (landed), leave path (landed), next: FRV

**Audience:** next session. Not user docs (`whats-new.md` has nothing for this slice).

**Landed on `develop`:**
- [PR #305](https://github.com/afw-org/afw/pull/305) (`0fc0f2b8`, 2026-09-09) — `compile()` is a unit; `app::` get of compiled templates.
- [PR #306](https://github.com/afw-org/afw/pull/306) (`5d5b0096`, 2026-09-10) — every `{ }` is a scope; `last_result` on the running frame; leave path.

**Verify for #306:** `./afwdev build --fulldev` then `afwdev test -j` and `afwdev test -j --env-mode valgrind`: **4433 passed**, 71 skipped.

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

**Extra-hold** (`afw_xctx_scope_hold_last_result`): `get_assignable` + `afw_pool_release_value_at_cleanup` on **current** `scope->p`, then store `last_result`. Two doors only: nested `{ }` adopt (child tracker already died) and `return()`. Not at clone. Not a slot replace on every statement.

**`for` / `while` / `try` are void** except `return` / `rethrow`. Nested assignment writes last on the **running** scope. Do not C-return the loop’s last assignment.

**`for (let)` clone** is for closures, not a result stack:

- First trip **is** the for-let `{ }` (not a template).
- Next trip: sibling `scope_clone` (copy `frame_slots[]`, not last; same `parent_lexical_scope`). `clone()` marks the original **cloned**. Increment / for-of assign run on the clone so a closure still sees the old `i`. Creator-`release` the previous; it dies unless a closure `get_reference`s it.
- Without closures, two frames: the `{ }` until `for` ends, plus the **current** clone.
- How we know which iteration is last: the one that was **never cloned**. Do not pick a winner. Nested assignment already wrote `script_result` in loop order.

**Finally:** a **normal** finally `{ }` must not adopt last onto the parent (that overwrote `return 'try'` with `count.finally += 1`). Finally **return** still wins. Nested assignment in finally still writes last when try/catch did not return.

**Rejected this wave:** dest `p` on deactivate; treating `last_result` like `frame_slots[]`; extra-hold “harder” on cloned-from last; `for` C-return of last; clone-first as a template; `iter_p`; isolating FRV at `return()` `get_assignable`/`slot_store` as the design (that is the next slice).

---

## Next slices (agreed order)

1. **FRV as stack leftover** — `#function_return_value` is compile-time (parse/decompile). Intended inf: evaluate / `get_assignable` are of the **inner**; wrapper has its own RC; last release frees wrapper + inner extra-hold. Enclosing call `pop_value` releases leftovers. **Remove** `consume()` / `is_function_return_value` peels. Unique consume today **transfers occupant, RC 0, no `free_memory`** → `function_return` soak. Do not paper over with a helper around assign. Do not treat extra-hold at `return()` as that design.
2. **Runtime call-result hold** — second, evaluate-only inf (like `closure_binding`: display decompile, not recompile) for **managed built-in returns** and transferred occupants (`pop`). Same leftover protocol. Identity **`push`** (return same array) stays unwrapped. Keep separate from compile-time FRV until they match.
3. **Merge 1 and 2** only if the infs are actually the same.

Parked on 1–2: array in-place mutate vs managed return (`array_push_pop` soak is a separate inf/`push`/`pop` bug — do not wrap-at-execute of unmanaged).

---

## Traps

- Object-push get is **not** “app:: only.” Only run **compiled units**.
- `evaluate(symbol)` does not run the occupant. Adaptive `evaluate()` is the hop for a variable holding a unit.
- Returning a function **from** `evaluate(compile(…))` skips clone-unmanaged (no evaluated-data-type clone). **Inside** a unit, `return function(){…}` is normal extra-hold → deactivate isolate.
- Do not `git add -A` while `--fulldev --clean` is rewriting `src/afw/generated/`.
- Handbook XML uses `<italic>`, not `<emphasis>` (Doxygen).
- Cloned-from deactivate that still `script_result_set`s is LIFO: first-trip last wins (0 instead of 3).
- `last_result` whose only extra-hold is the xctx `script_result` slot goes stale when the slot is replaced. Extra-hold on the **scope that points at it**.
- `evaluate_statement` of a nested `{ }` adopts onto **current**. A normal finally must not do that over a pending return.

---

## Probes

- `afwdev test --test-pattern 'substitution.as'`
- `afwdev test --test-pattern 'language/script/for.as'` (includes `for-let-break-keeps-previous-last`)
- `afwdev test --test-pattern 'language/script/script_result.as'`
- `afwdev test --test-pattern 'test262/statements/try.as'` (`completion-values-fn-finally-normal`)
- `afwdev test -T src/afw/tests-extra/issue-2 --show-all` — live soaks; `function_return` still climbs until FRV leftover.
- Full PR bar: `./afwdev build --fulldev && afwdev test -j && afwdev test -j --env-mode valgrind`
