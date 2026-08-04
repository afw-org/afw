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

**Still open:** later improvements to destructure *parsing* itself; binding-site Pattern sugar where the grammar still takes a bare name (below).

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

**Pattern feature residuals** (apply everywhere Patterns exist, not new sites):

- Computed keys in object destructure `{ [k]: x }` still open (near #38).
- Param surface defaults are `= Literal` only; destructure element defaults already allow **Expression** (align when touching params).

**Not a Pattern site — related “args as a whole” asks:**

| Need | Status | Issue / mechanism |
|------|--------|-------------------|
| **Process / CLI args as one array** (script body, Node-like `process.argv`) | **Done** (close pending Jeremy) | Jeremy **[#74](https://github.com/afw-org/afw/issues/74)** → `process::args` (+ `programName`, `pid`, …). Secrets path via `afw_crypto` + file/stream; interactive `readpass` never built (asked Jeremy if close is OK without it). |
| **All call arguments inside a script function** (ES `arguments` object) | Rest param covers common case | Use `function f(...args)` (already supported). No separate ES `arguments` binding unless someone files a real need. |
| **Call-site spread** `f(...arr)` | **Done** (follow-up) | list_expression marker + expand at call; see TS-shaped table |

Do **not** conflate `process::args` (#74) with function-parameter rest or param destructure.

### Function parameter destructuring (issue #140 — implemented on branch)

**GitHub:** [#140](https://github.com/afw-org/afw/issues/140) (enhancement; assignee mike000000000).

**Status:** Core landed on `issue-#140` / PR **#141** (params + catch Patterns, Expression defaults, bind order). Follow-up on `Issue-#140-followup`: call-site `f(...arr)`, string/computed Pattern keys, catch Pattern decompile d1==d2, type syntax on formals/leaves, TS-shaped confidence tests. Tests: `language/script/param_destructure.as`, `language/list/spread.as`, decompile fidelity / pragma.

### Cleanup notes for a later major pass (do not block #140)

| Item | Notes |
|------|--------|
| **Destructure runtime** | Still in `afw_function_compiler_script.c` as static helpers; consider a small public bind API if more sites appear. |
| **Object Pattern property names** | **Done** for string + `[expr]` keys (follow-up). Residual: exotic edge cases only. |
| **Catch identifier vs Pattern** | Identifier: StatementList callback + `symbol_reference`. Pattern: early block + AssignmentTarget + `use_existing_current_block`. **Decompile d1==d2 for Pattern catch done** (embed bind in catch `#block`; execute uses first-statement bind when no argv[4]). |
| **`#script_function` Pattern vs body `{`/`[`** | Speculative parse then cursor restore; advanced pragma only. |
| **Call-site `f(...arr)`** | **Done** (follow-up). |
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
