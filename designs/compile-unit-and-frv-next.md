# Compile unit (landed) and next: leave path / FRV

**Audience:** next session. Not user docs (`whats-new.md` already covers testers).

**Landed:** [PR #305](https://github.com/afw-org/afw/pull/305) on `develop` (`0fc0f2b8`, 2026-09-09).

## What #305 decided

- `compile()` of script/template/test_script returns a **`compiled_value` unit**. JSON compile returns an already-evaluated value (no wrapper).
- **Evaluate of the unit runs it** (`optional_evaluate` on `compiled_value`). **`get_assignable` of the unit is the unit** (assignable face). Do not extra-`evaluate()` after you already have the compile result.
- Extra hops **removed** from assign, formals, callee, spread, destructure. **Kept** in Adaptive `evaluate()` / `evaluate_with_retry` / `safe_evaluate` (evaluating a *variable* only yields the occupant).
- **`app::` / handler `qualifiedVariables`:** `afw_compile_templates` at conf load. Qualifier **get** (`impl_get_object_variable_cb`) `evaluate()`s **only if `is_compiled_value`**. That get_cb is **shared** with `current::`, `adapter::`, `environment::`, `request::`, `application::` — always-`evaluate()` broke `qualifier("current")` (allocate memory / onGetProperty). Contribute matches get for compiled units.
- Three times (templates): `#{…}` compile, `${…}` on get, function from `#{…}` on **call**. Tests in `src/afw/tests/miscellaneous/substitution/`.
- Model **`on*`** / log **`filter`:** scripts, C `evaluate()` of the pointer at hook/write. Path conf: compile **and** evaluate at configure. Model **`custom::`:** templates, evaluate on `custom::` get.
- Log conf **`custom`** removed (never wired). Unused adapter `impl->custom_variables` removed.

**Rejected this wave:** `afw_value_evaluate_create` wrapper; `evaluate_program` host hunt; making `compiled_value` evaluate identity (that forced a second run API everywhere).

## Next slices (agreed order)

Do **not** start with “implement a fix” unless you share this plan. Open with “what do you think?”

1. **Leave path** — `return()` does not `get_assignable` / `slot_store`. Statement-flow loops/blocks finish **return and break** (including holding a scope until the target consumes it). Same “stop isolating in the wrong place.”
2. **FRV as stack leftover** — `#function_return_value` is compile-time (parse/decompile). Intended inf: evaluate / `get_assignable` are of the **inner**; wrapper has its own RC; last release frees wrapper + inner extra-hold. Enclosing call `pop_value` releases leftovers. **Remove** `consume()` / `is_function_return_value` peels. Unique consume today **transfers occupant, RC 0, no `free_memory`** → `function_return` soak. Do not paper over with a helper around assign.
3. **Runtime call-result hold** — second, evaluate-only inf (like `closure_binding`: display decompile, not recompile) for **managed built-in returns** and transferred occupants (`pop`). Same leftover protocol. Identity **`push`** (return same array) stays unwrapped. Keep separate from compile-time FRV until they match.
4. **Merge 2 and 3** only if the infs are actually the same.

Parked on 2–3: `script_result` as **running last** (as-is pointer, void start, non-void replaces) vs FRV; array in-place mutate vs managed return (`array_push_pop` soak is a separate inf/`push`/`pop` bug — do not wrap-at-execute of unmanaged).

## Traps

- Object-push get is **not** “app:: only.” Only run **compiled units**.
- `evaluate(symbol)` does not run the occupant. Adaptive `evaluate()` is the hop for a variable holding a unit.
- Returning a function **from** `evaluate(compile(…))` skips clone-unmanaged (no evaluated-data-type clone). **Inside** a unit, `return function(){…}` is normal `as_assignable` → closure.
- Do not `git add -A` while `--fulldev --clean` is rewriting `src/afw/generated/`.
- Handbook XML uses `<italic>`, not `<emphasis>` (Doxygen).

## Probes

- `afwdev test --test-pattern 'substitution.as'`
- `afwdev test -T src/afw/tests-extra/issue-2 --show-all` — live soaks; `function_return` still climbs until slice 2.
- Full PR bar: `./afwdev build --fulldev && afwdev test -j && afwdev test -j --env-mode valgrind`
