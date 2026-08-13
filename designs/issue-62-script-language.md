# Adaptive Script language changes (issue #62)

**Audience:** maintainers / assistants. **Not** user docs.  
**GitHub:** [#62](https://github.com/afw-org/afw/issues/62) — *Adaptive Script language changes*  
**Branch:** `issue-#62-script-language` off `mgg-develop`. One feature branch; commit as we go.  
**Status:** in development. Productions were not written for these forms; parser and EBNF need rework.

Jeremy still wants the index items. Many of the ideas come from TypeScript / ECMAScript. They were not originally planned, so several productions (especially assignment and `for` init) have to change rather than grow a flag.

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

**Assignment chain is a statement, not a general expression.** `x = y = 1;` (and the same idea with `+=` / `??=` on the rightmost assign) is the goal. `(x = 1) > x` stays illegal. That keeps the motivated difference in `typescript-differences.md` (assignment is not an expression you can nest). Item 4 is a production change to Assignment’s RHS, not “assignment everywhere Expression is.”

**Script result (item 1, later):** a running result, not last-statement-wins.

- Start at `undefined` (empty script today).
- `return` sets it and leaves.
- Assignment rebind (`=`, `+=`, …) updates it.
- `let` / `const` do not.
- `if` / `for` / `while` / `try` / `function` / call statements do not reset it.
- Nested assignment inside those forms does update it.
- `break;` **preserves** the running result (today it wipes to `undefined`). No `break expr` in the first cut; the built-in already has an optional value, syntax does not.
- Single-expression script (`1 + 2`, no semicolon-as-statement) still returns that expression. `abs(-3);` is a call statement and will not write the result once item 1 lands.

This is Adaptive completion, not ES `cptn-*` / test262.

## Vertical order (this branch)

| Step | Item | Notes |
|------|------|--------|
| **2** | Multi `let` / `const` | Landed. Enables a sane `for` header. |
| **3** | `for` init is one statement | Landed. `for (let i = 0, j = 1; …)` works; `for (let i = 0, let j = 1; …)` is a reserved-word error. |
| **4** | Chained assignment **statement** | Production rework; not general expression. |
| **1** | Running result | After the syntax that writes results is in place. |
| **5** | Loop labels | Independent; last or a later slice. |

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

On this tree, last evaluated statement is the script result: `let x = 1;` → `1`; `let x = 1; if (true) { let y = 2; }` → `2`; empty `if` or bare `break;` → `undefined`. `x = y = 1` does not parse. `for (let i = 0, j = 1; …)` works; `for (let i = 0, let j = 1; …)` is “Variable name can not be a reserved word”. `outer: for` does not parse. Implicit `x = 1` without `let` is “Unknown built-in function `x`.”

## Item 3 (landed on this branch)

C-style `for` initializer is one `LetDeclaration`, one `ConstDeclaration`, or one or more `Assignment`s (comma-separated, same stand-in as the increment list). `for-of` still uses `OptionalDefineTarget 'of' Expression`. `let`/`const` still open a block around the loop so names do not leak.
