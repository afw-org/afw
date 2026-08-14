# Compile-time optimize (notes for a future branch)

**Not part of issue #18 / pragma and decompile work.** Separate feature branch and design conversation later.

**Issue #18 status snapshot:** [`designs/issue-18-decompile-status.md`](issue-18-decompile-status.md) (goals, landed compiled forms, footguns, tests, tip commit).

## Idea

At compile time, fold calls where:

- the function is **`pure`** (function metadata already has `pure: true`), and  
- every argument is a **compile-time constant** (literal / permanent / already folded data-type value).

Replace the call node with the evaluated result in the value graph (classic constant folding).

Example: `add(1, multiply(2, 3))` → `7`.

## How current #18 work helps testing later

- **`decompile(compile(...))`** shows compiled shape before/after fold (call disappears, literal appears).
- **`#block(...)`** and other PragmaValues let tests inject compiled values without going through full Script surface.
- Round-trip tests establish eval + decompile stability; optimize tests can assert **structural** decompile change under an optimize flag/pragma.

## Possible controls

- Compile option (whole unit).
- Future **`#optimize`** (or similar) as **PragmaStatement** for regions — not implemented yet.

## Engine notes (sketch)

- Trust `pure` on builtins first (arithmetic, etc.).
- Constant lattice: permanent evaluated data types; extend carefully to const bindings already folded in-block.
- Existing `optimized_value` fields on some value kinds may relate; decompile of “what runs” should prefer folded form.
- Policy for pure calls that throw at fold time: compile error vs leave call.

## Explicitly out of scope for #18

Implementing the optimizer, pragma, or permanent purity audits. This file is only so the idea is not lost in chat.

---

## Produce type percolation (`inf->data_type`) — deferred

**Not #28 type-checking proper** and **not #153 iterator surface**. Captured during
#153 brainstorm (2026-08); implement on a compile-optimize / produce-type pass.

### Intent

At compile time, when the result type of an expression is knowable, **percolate
it into the value produce-type channel** so soft probes work without evaluating:

- Inf variable **`data_type`** (`afw_value_quick_data_type`) — preferred for kinds
  that can put a fixed type on the shared inf.
- For **call** IR, produce type is often per-instance today
  (`evaluated_data_type` + `get_data_type()`), because one shared call inf cannot
  hold every return type. Long-term: keep the *intent* (known produce type without
  eval); design how that surfaces (instance + method, helpers, or other).

**`is_evaluated_of_data_type`** remains cast-safe / finished layout only.

When produce type **cannot** be known (true dynamic), leave it NULL and rely on
**compile-time type checking** (#28) rather than inventing a type on the IR.

### Situation snapshot (explore, branch tip ~#153)

| Kind | Produce type today |
|------|--------------------|
| Evaluated data-type values | Both inf fields set (good). |
| Built-in **call** (compile create, `allow_optimize`) | Instance `evaluated_data_type` from fixed `returns->data_type`; poly return-from-param1 when arg1 type known; specialized poly hub when first arg type known. **`inf->data_type` still NULL** (shared call inf). |
| Built-in call (runtime create, no optimize) | Often unset (by design — HOF argv slots). |
| **Script function definition** | Produce type **function** (correct for pass-as-value). Signature `returns` defaults to **any** if omitted; author `: Type` stored on definition for checks. |
| **Script function call** / generic **call** | `get_data_type` → **NULL**; create has `@fixme Get right data type`. |
| `reference_by_key`, block, many other IR | Often NULL / incomplete. |

#153 macros: `afw_value_iterator_return_data_type` uses **quick** `inf->data_type`
only — correct field, sparse coverage until percolation lands. Eval-time
has/initialize use `is_evaluated_of_data_type` (separate).

### Tests to add when this work is done

Prefer compiler-listing / decompile / small C or Adaptive probes over only
end-to-end script luck. Ideas:

1. **Fixed-return builtin call** (e.g. `substring`, `bag_size`): after compile,
   produce type is string/integer (via `get_data_type` and, if design puts it
   there, any quick path). Soft step-type for string-returning call →
   `iterator_return_data_type` string when produce type is string.
2. **Polymorphic return ≈ param1** (e.g. `clone(x)`): when `x` has known type,
   call produce type matches; when `x` is unknown, produce type NULL (no lie).
3. **Polymorphic hub specialized** at create when arg1 type known: produce type
   matches specialized method returns (not the open hub).
4. **Script call with `: string` (etc.) return**: call produce type is that leaf
   when known; **without** annotation still **any** / unknown policy (document).
5. **Script / function *value*** (not call): produce type remains **function**.
6. **Dynamic only at eval**: expression that cannot be typed at compile stays
   NULL produce type; with `#typecheck` / flags, assignability still enforced
   where #28 already covers (regression that we did not fake a type).
7. **Quick vs method**: tests that document which channel is authoritative for
   calls after the design choice (avoid flaky tests that only read
   `inf->data_type` if instance remains source of truth for calls).
8. **Constant fold** (this pad’s original idea): pure call with constant args
   becomes literal; produce type of folded node is the literal’s type; decompile
   shows fold under optimize flag/pragma.
9. **#153 consumers later**: for-of / index soft probes on expressions that are
   calls returning string/array only after produce type is honest — optional
   cross-link tests, not a substitute for (1)–(4).

### Explicitly not in this deferred bag

- Rewiring for-of / `s[i]` onto iterators (#153 consumer step).
- Expanding #28 annotation surface (generics, etc.).

---

## Related #18 follow-ups (not optimize)

### Destructuring `#assignment_target` (implemented direction)

**Landed on issue-#18:** decompile + pragma use Pattern as second arg.

```text
#assignment_target("const", [a,b])
#assignment_target("const", {a, b: x, c=1, ...r})
```

- First arg: assignment kind string (`"const"`, `"let"`, …).
- Second arg: **Pattern**, not Expression — identifier/string **or** list/object pattern (`[…]` / `{…}`: holes, defaults, rest, rename, nesting).
- Parse reuses `AssignmentTarget` / existing destructure parsers (no thrash of that file’s structure).
- Nested patterns decompile as nested `[`/`{` only (no nested `#assignment_target` wrapper).
- Prefer **not** a forest of nested `#list_destructure` / `#assignment_element` pragmas as the primary form.

**Landed:** binding-site Pattern sugar for params and catch (#140). Remaining: later improvements to destructure *parsing* itself if needed; no new binding sites planned.

### Binding sites that use Pattern / `AssignmentTarget`

Inventory of where Adaptive Script binds names, and whether a list/object **Pattern** is allowed. Prefer one shared Pattern story (assignment-target parsers + desugar), not parallel binders.

| Site | Grammar today | Pattern? | Notes |
|------|---------------|----------|--------|
| `let` / `const` | `AssignmentTarget` | **Yes** | Nested, rest, defaults (`= Expression`), rename |
| Plain assignment `… = …` | `AssignmentTarget` | **Yes** | Object form often needs parens: `({a,b} = o)` |
| **`for (… of …)`** head | `OptionalDefineTarget` → `AssignmentTarget` | **Yes already** | e.g. `for (const {dataType, brief} of …)` |
| C-style **`for` init** | `OptionalDefineAssignment` → `AssignmentTarget` | **Yes already** | e.g. `for (let [x] = [23]; ;)` |
| **Function / lambda params** | `ParameterName` or Pattern | **Yes (#140)** | Options-object style; whole-arg default then Pattern |
| **`catch (…)`** | Identifier or Pattern | **Yes (#140)** | Same assign path as let Patterns |
| Import / class / `for await` | — | n/a | Not in the language |

Removed: stub `declare …` statement (was never implemented; no longer reserved).

**Pattern features (shared across all Pattern sites):**

- **Computed / string keys** in object destructure: `{ [k]: x }`, `{ "name": x }` — **done** (#140 follow-up / PR #142). Near #38 for expression property names on *literals*, not destructure.
- **Param and destructure defaults** are both `= Expression` (aligned in #141).

**Not a Pattern site — related “args as a whole” asks:**

| Need | Status | Issue / mechanism |
|------|--------|-------------------|
| **Process / CLI args as one array** (script body, Node-like `process.argv`) | **Done** (close pending Jeremy) | Jeremy **[#74](https://github.com/afw-org/afw/issues/74)** → `process::args` (+ `programName`, `pid`, …). Secrets path via `afw_crypto` + file/stream; interactive `readpass` never built (asked Jeremy if close is OK without it). |
| **All call arguments inside a script function** (ES `arguments` object) | Rest param covers common case | Use `function f(...args)` (already supported). No separate ES `arguments` binding unless someone files a real need. |
| **Call-site spread** `f(...arr)` | **Done** (#140 follow-up / #142) | list_expression marker + expand at call; see TS-shaped table |

Do **not** conflate `process::args` (#74) with function-parameter rest or param destructure.

### Function parameter destructuring (issue #140 — closed)

**GitHub:** [#140](https://github.com/afw-org/afw/issues/140) (enhancement; assignee mike000000000).

**Status:** **Done on `mgg-develop`.** PR **#141** (params + catch Patterns, Expression defaults, bind order) + PR **#142** (call-site `f(...arr)`, string/computed Pattern keys, catch Pattern decompile d1==d2, type syntax on formals/leaves, TS-shaped confidence tests). Tests: `language/script/param_destructure.as`, `language/list/spread.as`, decompile fidelity / pragma. Docs: Language Reference Function + Features; `whats-new.md`.

### Post-#140 residual (non-blocking; not open work for this issue)

| Item | Notes |
|------|--------|
| **Destructure runtime** | Still in `afw_function_compiler_script.c` as static helpers; consider a small public bind API if more sites appear. |
| **Object Pattern property names** | String + `[expr]` keys landed (#142). Residual: exotic edge cases only. |
| **Catch identifier vs Pattern** | Identifier: StatementList callback + `symbol_reference`. Pattern: early block + AssignmentTarget + `use_existing_current_block`. Decompile d1==d2 for Pattern catch done (embed bind in catch `#block`; execute uses first-statement bind when no argv[4]). |
| **`#script_function` Pattern vs body `{`/`[`** | Speculative parse then cursor restore; advanced pragma only. |
| **argv / parameter_number** | Documented in `afw_function.h` + `afw_value_call_args_s` + call_script_function bind comments (1-based params, `argv[0]` = function). |

### TS-shaped confidence (ranked high → low)

Goal: Adaptive stays Adaptive, but Patterns/params/catch should not feel awkward to someone who writes TypeScript. Not full TS/ES parity.

| Rank | Item | Why (TS lens) | Status / action |
|------|------|---------------|-----------------|
| **1** | Options object + property/whole defaults | Everyday TS API shape | Done in #141 + tests |
| **2** | Prior-param defaults `y = x` / Expression defaults | Core TS default semantics | Tests `ts-prior-param-default` |
| **3** | Object rest on param `{a, ...r}` | Everyday destructure | Tests `ts-object-rest-on-param` |
| **4** | Required Pattern missing arg → error | Don’t fail open | Tests `ts-required-pattern-missing-arg` |
| **5** | Catch destructure (+ rename / data) | Structured error handling | Tests `ts-catch-rename-and-data` |
| **6** | Recursion with Pattern formals | Bind order must stay correct | Tests `ts-recursive-pattern-formal` |
| **7** | Pattern then `...rest` formal | Normal rest placement | Tests `ts-pattern-then-rest-param` |
| **8** | Wrong-type destructure error | Fail clearly, not garbage | Tests `ts-wrong-type-array-pattern` |
| **9** | Optional Pattern without `= {}` | Document Adaptive choice (prefer `= {}`) | Tests `ts-optional-pattern-no-default` |
| **10** | Catch Pattern decompile d1==d2 | Tooling/Fiddle fidelity | **Done** (follow-up): try embeds Pattern in catch `#block` |
| **11** | Call-site `f(...arr)` | Common TS | **Done** (follow-up): expand list_expression markers |
| **12** | Computed / string keys in Patterns | Occasional TS | **Done** (follow-up): `[expr]: bind`, `"name": bind` |
| **13** | Arrow functions | Explicit non-goal (Jeremy) | Out (parser FIXME left) |
| **14** | ES `arguments` / `#script_function` pragma | Not app-author surface | Out / advanced only |
| **15** | Full TS type-check on Patterns | Compile-time types | Syntax: leaf + whole Pattern `: Type` stored; enforce in #28 |

ECMAScript/TypeScript-style “destruct in the parameter list”, e.g. conceptually:

```text
function f([a, b], {x, y}) { … }
// or AFW-flavored sugar along the same idea
```

Primary use landed: ES/TS “options object” idiom without a manual intermediate bind:

```text
function connect({ host, port = 443 } = {}) { … }
// desugars to AFW’s existing Pattern / assignment_target model
// (same compiled form family as const { host, port = 443 } = …)
```

Intent remains **syntax sugar** into one shared binding story, not a parallel system.

Invariants after landing:

- Pattern machinery stays **shared** across `let`/`const`, assignment, for heads, params, and catch.
- `#script_function` decompile/pragma supports Pattern formals (speculative parse vs body `{`/`[`); prefer one binding story if more pragma surface is added.
- Stay **AFW Script** (explicit, metadata-friendly), not a full ES port.
- Arrow functions remain an explicit non-goal unless product asks later.

### `catch` binding Pattern (**done** with #140)

EBNF: catch binding may be Identifier **or** list/object Pattern (same as params / `let`).

```text
catch ({ message, data }) { … }
catch ({ message: msg }) { … }
```

Identifier path: StatementList callback + `symbol_reference`. Pattern path: early block + AssignmentTarget + `StatementList(..., use_existing_current_block)`. Decompile embeds the bind as the first statement of the catch `#block` so d1==d2 holds; execute uses that first-statement bind when there is no separate argv[4].

### `#closure_binding`

Decompile of a **runtime** closure is `#closure_binding(#script_function(...))`. The binding holds a live `enclosing_lexical_scope` (xctx scope), which is not reconstructible from decompile text alone (free variables like outer `x` need that scope).

Do **not** implement a fake `#closure_binding` pragma that drops the scope: calls with free vars would mis-evaluate. Treat runtime-closure decompile as **display / debug** unless a future design serializes closed-over bindings deliberately.

**Compile:** `#closure_binding` is a **known** pragma that always fails with a clear message (not “unknown pragma”):
`#closure_binding is runtime-only (closed-over scope) and cannot be recompiled from decompile text`.

### `#function_thunk`

Decompile is `#function_thunk("detail or name")` — C-side hook label only.

**Compile:** known pragma, always fails with:
`#function_thunk is C-side only and cannot be recompiled from decompile text`.
