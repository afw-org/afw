# Adaptive Script language changes (issue #62)

**Audience:** maintainers / assistants. **Not** user docs.  
**GitHub:** [#62](https://github.com/afw-org/afw/issues/62) — *Adaptive Script language changes*  
**Branch:** `issue-#62-script-language` off `mgg-develop`. One feature branch; commit as we go.  
**Status:** landed on this branch (index items 1–5 plus void / running-result follow-through). Ready to review / PR when asked.

Jeremy still wants the index items. Many of the ideas come from TypeScript / ECMAScript. They were not originally planned, so several productions (especially assignment and `for` init) have to change rather than grow a flag.

## How we got here (decision context)

Adaptive started as **Adaptive expression**. Scripting came later (Jeremy): Mike made a **functional** language that also allowed a **list of statements**, and every statement was a **function call**. Jeremy then wanted more **ECMAScript** surface while staying inside Adaptive Framework (values, compile, qualifiers, adapters). Incremental ES-shaped changes produced Adaptive Script as it is now.

#62 is another step on that path — Jeremy asked for these items — not a rewrite into JS. The IR is still compiled calls/blocks. Use ES to decide **shared syntax and what a script/function yields** when we support the construct; keep Adaptive where the model already differs (assignment is a statement, no general `2;`, no completion records).

Original yield rule: a statement list or last expression was the result, and a script could also be only an expression. The lone-expression script is still true. Jeremy pushed back on **every last statement** being the result (not what he expected from ES). Current direction: **follow ES for what the return is**, knowing we cannot be exact. Function bodies match scripts; non-void call statements write; for init/increment do not.

**Taste (do not fork this here):** [`typescript-differences.md`](../typescript-differences.md) — Adaptive Script is **not** TypeScript and **not** JavaScript. When we **support** a TS/JS-looking construct, it should behave as a TypeScript-literate author would reasonably expect **for that construct**. Prefer motivated Adaptive differences over accidental ones; do not half-implement the JS platform. Use TS/ES to answer subtle questions (comma lists, what `const` requires, what a `for` head looks like), not to import the rest of those languages.

How that applies to this index:

| Item | Shared-surface reading | Motivated Adaptive difference |
|------|------------------------|-------------------------------|
| Multi `let` / `const` | `let a = 1, b = 2` / `const a = 1, b = 2` like TS/JS; no trailing comma; `const` needs `=` on every name | No `var`; semicolons required |
| `for` init | `for (let i = 0, j = 1; …)` is one `let` (TS/JS). `for (let i = 0, let j = 1; …)` is **not** TS/JS — drop it | Init is one statement, not a special mixed comma list |
| `x = y = 1` | Authors expect the chain **as a statement** | Assignment is **not** a nestable expression (`(x = 1) > x` stays illegal) |
| Result value | `return` is how a script/function yields a value | Not ES completion / last-statement-wins / `cptn-*` |
| Labels | `outer: for` / `break outer` / `continue outer` as in TS/JS | **Not** on blocks (Jeremy: too confusing) |

## Index (issue body)

1. Most statements should not affect the result value. Exceptions named on the issue: `return`, `break`, and assignment (not `let` / `const`).
2. `let` / `const` allow multiple variables.
3. `for` initializer is a single `let`, `const`, or assignment (not today’s comma list of mixed defines).
4. `=` assignments as expressions so `x = y = 1` works.
5. Labels on loops, `break`, and `continue`. Not on blocks.

## Locked product rules

**Assignment chain is a statement, not a general expression.** `x = y = 1;` (and the same idea with `+=` / `??=` on the rightmost assign) is the goal. `(x = 1) > x` stays illegal — assignment inside a larger expression is almost always a typo (`=` vs `===` / `==`). That is a **decided not to**, not a leftover: see `typescript-differences.md`. Item 4 is a production change to Assignment’s RHS, not “assignment everywhere Expression is.”

**Script result (item 1, landed):** a running result, not last-statement-wins.

- Start at `undefined` (empty script today).
- `return` sets it and leaves.
- Assignment rebind (`=`, `+=`, …) updates it.
- `let` / `const` do not.
- `if` / `for` / `while` / `try` / `function` declarations do not reset it.
- A non-void **call statement** writes it (ES `eval` ExpressionStatement). `print()` is void and does not.
- Nested assignment inside those forms does update it.
- `break;` **preserves** the running result. No `break expr` (ES does not have one; the built-in is optional `label` only).
- A script that is **only** a call or expression (`1 + 2`, `abs(-3);`, `#block(add(1,2))`) yields that value. `x = 1; abs(-3);` is `3` (ES `eval`). `print();` is void and does not override.

This is Adaptive completion, not ES `cptn-*`. test262 cases that only care that the case does not throw use **`expect: success`** (ignore the result). `expect: undefined` still means the result really is `undefined`.

## Vertical order (this branch)

| Step | Item | Notes |
|------|------|--------|
| **2** | Multi `let` / `const` | Landed. Enables a sane `for` header. |
| **3** | `for` init is one statement | Landed. `for (let i = 0, j = 1; …)` works; `for (let i = 0, let j = 1; …)` is a reserved-word error. |
| **4** | Chained assignment **statement** | Landed. `x = y = 1;` works; `(x = 1) > x` and `let x = y = 1` do not. |
| **1** | Running result | Landed. Assignment and return write it; most statements do not. |
| **5** | Loop labels | Landed. `outer: for` / `break outer` / `continue outer`. Not on blocks/`if`. |

Do not mix **#170** / **#101** / **#35** onto this branch.

## Item 2 (landed on this branch)

**Today:** one `AssignmentTarget` per `let` / `const`. `let a = 1, b = 2;` is a syntax error. Built-in `let` / `const` metadata already says “one or more names,” but each call still has one target and one value.

**Accept:**

```adaptive
let a = 1, b = 2;
let a, b = 2;
const a = 1, b = 2;
let a: integer = 1, b: string = "x";
let {x} = obj, [y] = arr;
```

Each binding is its own `let` / `const` call in the **current** block (no extra scope). A comma list is the same keyword; `let a = 1, const b = 2` is not one statement. No trailing comma. `const` still requires `=` on every binding.

Decompile of the comma form may emit separate `let` / `const` lines (same bindings). Behavior tests matter more than comma-for-comma source fidelity.

## Live probes (before item 1)

On this tree (after item 5): running result as in item 1. `x = y = 1;` works as a statement. `for (let i = 0, j = 1; …)` works; `for (let i = 0, let j = 1; …)` is “Variable name can not be a reserved word”. `outer: for` / `break outer` / `continue outer` work. Label on `if` or a block is “Labels are only allowed on for, while, and do statements”. Implicit `x = 1` without `let` is “Unknown built-in function `x`.”

## Item 1 (landed on this branch)

`xctx->script_result` is the running result for the current **script** compile (not test_script / template). `assign` and `return` write it. `evaluate_block` of a script body uses that slot; `break` / `continue` keep the prior value. Nested `evaluate(compile<script>)` and script-function calls save and restore the slot so `f();` does not adopt `f`’s result. A **#block as a value** (decompile / `evaluate(b)`) still uses last-statement. A script that is only one call or `#block(add(1,2))` yields that value so decompile of `1+2` stays `#block(add(1,2))`. test262 `expect: undefined` cases that only `throw` on failure got a trailing `return;`. `try.as` `cptn-*` / `S12.14_A6` / `scope-catch-*` rewritten to Adaptive throw/data/Pattern (not ES `var` / assignment-in-default).

## Void singleton (in progress, leftover 1 mechanism)

`afw_value_void` in `afw_value.h` is the permanent instance of data type `void`. Built-ins declared `returns: void` (including `let` / `const` / loops / `break` / `continue` / `try` / `switch`) return that pointer. `if` is a value (it is also `?:`). The statement-list loop skips void; `assign` / `return` still write `script_result`. Function brace bodies use the same running-result path as a script. A script function declared `: void` discards that running result and returns the void singleton so `f();` does not write. C-style `for` init/increment evaluate assigns but restore the running result.

## Item 5 (landed on this branch)

A label may precede `for` / `while` / `do` only. `break` / `continue` take an optional Identifier that must name an enclosing loop label. Unknown, duplicate, and `a: b:` (two labels on one loop) are compile errors. Nested functions do not see outer labels. IR: optional string last arg on `for` / `for_of` / `while` / `do_while` / `break` / `continue`. Runtime: `xctx->statement_flow_label`; an inner loop or `switch` consumes unlabeled break (switch) or unlabeled break/continue (loop) only; labeled flow propagates until the matching loop. Unlabeled `continue` from a `switch` now continues the enclosing loop (ES-shaped; previously the switch reset it).

## Item 4 (landed on this branch)

`AssignmentOperation` RHS is `Assignment`, so `x = y = 1` / `x = y += 1` is right-associative and still a statement. `let` / `const` / `if (…)` / parenthesized expressions stay Expression-only. `x = (y = 1)` is not accepted (the Adaptive cut vs TS/JS).

## Item 3 (landed on this branch)

C-style `for` initializer is one `LetDeclaration`, one `ConstDeclaration`, or one or more `Assignment`s (comma-separated, same stand-in as the increment list). `for-of` still uses `OptionalDefineTarget 'of' Expression`. `let`/`const` still open a block around the loop so names do not leak.
