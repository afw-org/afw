# test262 case changes

Maintainer map of **intentional** changes to cases under `src/afw/tests/test262/`
(unskip, rewrite, rebucket, product `differences`, related language fixes).

**Audience:** Mike, Jeremy, and anyone reviewing Adaptive vs ES language
behavior in this suite.

**Window:** everything that touched this tree on **`mgg-develop`** (and
stacked work such as **`test262-skipreason-sweep`**) **since `mgg-develop`
branched from `develop`** — merge-base **`f945f97c`** (mid‑July 2026), same
horizon as root [`whats-new.md`](../../../whats-new.md). Includes issue
**#55**, **#140**, **#39** elision, suite metadata, and the skipReason
convert sweep. Not the original full test262 import years ago, and not
every harness-only line noise.

**Not this file:**

| Doc | Role |
|-----|------|
| [`README.md`](README.md) | Suite purpose, `//?` tags, skip prefixes |
| [`FIXME-triage.md`](FIXME-triage.md) | What is **left** / convert priority |
| [`typescript-differences.md`](../../../typescript-differences.md) | Product ES vs Adaptive (beta) |
| [`whats-new.md`](../../../whats-new.md) | User-facing language/runtime notes |

### Maintenance

Update this file **in the same change** that alters a case’s run/skip/expect
or product meaning. Prefer **Index-only** rows. Add a detail section only for
product decisions, C surface changes, or non-obvious rewrites. Do **not** add
a row per bulk `skipReason:` label — one **meta** row covers taxonomy passes.
Do not paste diffs; git remains the record of *when*.

---

## Ledger (How)

| Code | Meaning |
|------|---------|
| **→run** | Was skip (or false-green); now runs with hard expect |
| **→impl** | Language/runtime fix landed; test unskipped or rewritten to pass |
| **→diff** | Runs with Adaptive semantics + `differences` (not full ES) |
| **→inc** | Labeled / rebucketed **Incompatible:** (permanent non-support) |
| **→fixme** | Still skip; **FIXME:** cleaned or left for later (# / theme) |
| **→har** | **Harness:** / runner-blocked |
| **→fix** | Body/expect fixed only (probe bug, false green) — little product change |
| **meta** | Suite docs / skipReason taxonomy only |

Optional in Brief: **`C:`** = C/runtime change, **`T:`** = test-only.

---

## Index

Each linked **Case** jumps to a detail section when there is more than one
line of story. Detail sections end with **[↑ Index](#index)**. Unlinked rows
are Index-only.

Rough time order: **#55 → #140 → #39 / docs → skipReason sweep → converts.**

### Meta / suite docs

| Case | How | Brief |
|------|-----|--------|
| [Suite README + official `//?` tags](#suite-readme-and-official-tags) | meta | Purpose, harness-once, tags |
| [skipReason prefix convention](#skipreason-prefix-convention) | meta | Incompatible/FIXME/Deferred/Harness |
| [First-pass skipReason labels](#first-pass-skipreason-labels) | meta | Every skip got a prefix |
| [FIXME triage note](#fixme-triage-note) | meta | [`FIXME-triage.md`](FIXME-triage.md) shortlist |

### Object / array helpers (#55) and dense arrays (#39)

| Case | How | Brief |
|------|-----|--------|
| [for-of push/pop mutation](#for-of-array-helpers-issue-55) | →run / →diff | `push`/`pop` not `Array.prototype` |
| [for-of dense array lineage](#for-of-array-helpers-issue-55) | →diff | No holes; no `.entries`/Symbol |
| [object keys/values/entries for-of](#for-of-array-helpers-issue-55) | →run | Adaptive `keys`/`values`/`entries` |
| [array elision for-of (#39)](#array-elision-for-of-issue-39) | →impl / →diff | Elision → dense undefined, visited |

### Patterns / params / catch (#140)

| Case | How | Brief |
|------|-----|--------|
| [function param defaults](#function-param-defaults-issue-140) | →impl / →fix | Defaults are Expressions; prior params OK |
| switch `S12.11_A1_T3` NaN/Infinity | →run | Expect undefined (NaN/Infinity cases) |
| try catch Pattern expects | →fix / →fixme | Dup catch bind message; half-converted skip |

### void / unary

| Case | How | Brief |
|------|-----|--------|
| [void `S11.4.2_A4_T3`–`T5`](#void-probes-and-undefined) | →fix / →impl | Probe fix; then `void` accepts undefined |
| [unary+ `S9.3_A2_T2`, `S11.4.6_A3_T4`](#unary-plus-identity) | →diff | + is identity; +null === null |

### Strings / line terminators / literals

| Case | How | Brief |
|------|-----|--------|
| [raw LT in strings](#raw-line-terminators-in-strings) | →diff | Adaptive allows raw CR/LF in string literals |
| [line-continuation double/single](#string-escapes-and-line-continuation) | →impl | C: `\` + LT contributes nothing |
| [NonEscape / `\x` / `\0` family](#string-escapes-and-line-continuation) | →impl | C: identity `\A`, `\xHH`, `\0` |
| string fromCharCode cases | →inc | No String.prototype / fromCharCode |
| [leading/trailing-dot numerics](#leading-and-trailing-dot-numerics) | →impl | C: ES DecimalLiteral `.5`, `1.`, `.1e1` |

### for-of / const / let (sweep)

| Case | How | Brief |
|------|-----|--------|
| [for-of non-iterable heads](#for-of-non-iterable-and-member-lhs) | →run | Hard error: array or string only |
| [for-of `head-lhs-member`](#for-of-non-iterable-and-member-lhs) | →impl | C: assign to `x.y` / `reference_by_key` |
| [for-of `string-bmp` / `string-astral`](#for-of-string-code-points) | →impl / →diff | C: UTF-8 code points; not UTF-16 units |
| for-of `head-const-bound-names-fordecl-tdz` | →fixme | No general TDZ (language-wide) |
| for-of `let-array-with-newline` | →fixme | ASI / let vs `[` after empty for-of |
| [for-of const + closures](#for-of-const-and-closures) | →fixme | #35 / #2 capture; expect desired 0 |
| [const reassignment](#const-reassignment) | →impl | C: clear `read_only` / reject assign |
| [no temporal dead zone self-init](#no-temporal-dead-zone) | →diff | `let x = x` → undefined (Adaptive) |
| for-of destructure head / body decls | →run | Converted with #39 pass (Adaptive OK) |

### `??` / operators / switch / assignment-as-expression

| Case | How | Brief |
|------|-----|--------|
| [coalesce abrupt short-circuit](#coalesce-abrupt-short-circuit) | →run | try/throw rewrite of ES assert.throws |
| [`??=` whitespace](#nullish-assignment-whitespace) | →run | T: tab/space/LF/CR around `??=` |
| [switch `S12.11_A1_T4`](#switch-isnan-rewrite) | →fix | Adaptive rewrite (is_NaN, fall-through) |
| [`>` / `>=` LTR `A2.4_T1`](#assignment-is-statement-only) | →diff | No assign-in-expr; throw order probe |
| [`>=` `A2.4_T4` `(y=1)`](#assignment-is-statement-only) | →diff | Expect error; permanent non-support |
| [modulus `A4_T*` IEEE](#double-mod-ieee-template) | →impl | C: `mod<double>` via fmod |
| [modulus null/string `%`](#double-mod-ieee-template) | →inc | No ToNumber on `%` |
| [division `A4_T4/T5/T7–T8` IEEE](#double-mod-ieee-template) | →impl / →fix | Double `/` IEEE; 0.0/0.0 → NaN |
| division null/object `/` | →inc | No ToNumber on `/` |
| [unary+ string/object/undef false greens](#unary-plus-identity) | →diff | Identity, not is_NaN type-error expects |
| [id-res `S10.2.2_A1_T4`](#identifier-resolution-inner-let) | →diff | Outer x (0); not expect-error on assert |

### Related (outside pure test-only)

| Case | How | Brief |
|------|-----|--------|
| empty-separator `split` | →impl | C: split by code points (with for-of) |
| #153 utf8 sequences pad | meta | Design only; not test convert |

---

## Suite README and official tags

Expanded [`README.md`](README.md): suite purpose (regression fence, not full
ES), TC39 lineage, harness-once guidance, and **official** `//?` keys
(`test`, `description`, `differences`, `expect`, `skip`, `skipReason`,
`source`, …). Aligns with TestScript EBNF / `afw-tests` rule.

[↑ Index](#index)

---

## skipReason prefix convention

Shared vocabulary (not compiler-enforced): **`Incompatible:`**, **`FIXME:`**,
**`Deferred:`**, **`Harness:`**. Documented in README and project test rules.
Use **Incompatible** only for permanent non-support.

[↑ Index](#index)

---

## First-pass skipReason labels

Bulk pass: every previously skipped case got a prefixed `skipReason`. No
per-case behavior change in that pass alone. Counts drift as cases convert.

[↑ Index](#index)

---

## FIXME triage note

[`FIXME-triage.md`](FIXME-triage.md) is the **backlog** (what to convert next).
This file is the **done log**. Update triage when a cluster moves; add Index
rows here for the cases that actually changed.

[↑ Index](#index)

---

## for-of array helpers (issue #55)

**Commit lineage:** PR **#134** / `1e295420` (on `mgg-develop`).

**Cases (for-of.as and related):** mutation during traversal
(`array-contract*`, `array-expand*`), dense `array` / iterator-lineage names
(`Array.prototype.entries` / `keys` / `Symbol.iterator` rewritten), plus
**object** `keys` / `values` / `entries` for-of probes.

| Was (ES) | Adaptive |
|----------|----------|
| `array.push` / `.pop` methods | **`push(array, …)`** / **`pop(array)`** |
| `array.entries()` / `keys()` / `Symbol.iterator` | Direct **for-of** on array, or **`at`** + index walk; object **`entries(o)`** etc. |
| Sparse holes in fixture | **Dense** arrays only |

Product helpers also documented in [`whats-new.md`](../../../whats-new.md)
(#55). Out-of-tree commands/extensions that link libafw need rebuild when
those land.

[↑ Index](#index)

---

## array elision for-of (issue #39)

**Commit:** `62718b2e` (with dense array semantics from PR **#152** / #39).

Literal **elision** is dense **undefined** (visited by for-of), not an ES
hole. Updated array fixtures and added **`array-elision-only`**. Same pass
also unskipped some for-of destructure / body-decl cases Adaptive already
allowed, and improved skipReason text on remaining for-of skips (many later
fixed in the sweep).

[↑ Index](#index)

---

## function param defaults (issue #140)

**Commit lineage:** PR **#141** / `f2bad1af`.

**Cases:** `expressions/function.as` — `dflt-params-abrupt`,
`dflt-params-ref-prior` (runs: prior params visible in default Expression),
`dflt-params-ref-later` (still error: later name not in scope), etc.

**Product:** Parameter defaults are full **Expressions** (not literals only);
binding scope order fixed for recursion/defaults. Catch Pattern / try cases
got expect message tweaks; some half-converted catch Pattern probes remain
**→fixme**.

[↑ Index](#index)

---

## coalesce abrupt short-circuit

**Case:** `expressions/coalesce.as` — `abrupt-is-a-short-circuit` (and related
from the #39-era convert pass).

**→run:** ES `assert.throws` rewritten to Adaptive **try/throw** + assert on
message; `??` short-circuits so later poisons do not run.

[↑ Index](#index)

---

## void probes and undefined

**Cases:** `expressions/void.as` — `S11.4.2_A4_T3`–`T5` (and related).

1. **→fix:** Rewrote bad probes (`isNaN(void …)` style) to check
   `void x === undefined`.
2. **→impl:** `void_operator` may take **undefined** (`canBeUndefined`) and
   always returns the undefined singleton (C + generate).

[↑ Index](#index)

---

## unary plus identity

**Cases:** `expressions/unary-plus.as` — `S9.3_A2_T2`, `S11.4.6_A3_T4`.

**Product:** Unary `+` is a **no-op** (parse-time identity). It does **not**
ES `ToNumber`. So `+null === null`, `+undefined === undefined`, `+"x" === "x"`,
`+obj === obj`. Boolean coercion with `+` stays **Incompatible** elsewhere.

Cleared false-green `expect:error` on `is_NaN(+…)` type errors (string/object/
undefined) — rewrote to identity asserts.

[↑ Index](#index)

---

## raw line terminators in strings

**Cases:** `literals/string.as` `S7.8.4_A1.1_T2`, `A1.2_T2`;
`line-terminators.as` `invalid-string-cr` / `lf`.

**Product:** Adaptive **allows** raw CR/LF inside string literals (ES forbids
unescaped line terminators). Unskipped with `differences`; asserts length 1
(or equivalent), not parse error.

[↑ Index](#index)

---

## string escapes and line continuation

**Cases (→impl):** NonEscape identity (`A4.2_T2/T4/T6/T8`), `\u0000`/`\0` /
`\x00` probes (`A5.1_T2`/`T3`), hex alphabet (`A6.1_T2`/`T3`),
`line-continuation-double` / `single`.

**C:** `afw_compile_lexical.c` — identity NonEscapeSequence, `\xHH`, `\0`
(not followed by digit), LineContinuation (`\` + LF/CR/CRLF/LS/PS).

**Still →inc:** cases that require `String.fromCharCode` / full ES String
prototype APIs.

[↑ Index](#index)

---

## leading and trailing-dot numerics

**Cases:** `literals/numeric.as` — `S7.8.3_A2.1_*`, `A2.2_*`, `A3.1_*`,
`A3.3_*` (leading-dot, trailing-dot, empty fraction + exponent).

**C:** `impl_parse_number` + `.` token path: `.5`, `.1e1`, `1.`, `1.e10`.
Fixed false-green `expect:error` on `A2.1_*` after implement.

[↑ Index](#index)

---

## for-of non-iterable and member LHS

**Non-iterable heads (→run):** for-of over `{}` / boolean / number / null —
hard `expect: error:…array or string` (earlier FIXME skips from #39 pass).

**Member LHS (→impl):** `head-lhs-member` — C `impl_assign` accepts
`reference_by_key` so `for (x.y of …)` works.

[↑ Index](#index)

---

## for-of string code points

**Cases:** `string-bmp`, `string-astral`.

**C:** for-of on string walks **Unicode code points** in UTF-8 (not ES
UTF-16 code units). Empty-separator **split** aligned to code points.
`differences` on astral/BMP cases. Design pad / **#153** for later
index/array-face work — not required for these tests.

[↑ Index](#index)

---

## for-of const and closures

**Case:** `head-const-fresh-binding-per-iteration`.

Still **→fixme** with **#35 / #2**. Desired expect kept honest (e.g. `0`);
do not leave a green test that only asserts today’s broken capture, and do
not use `expect: error` as a permanent stand-in for “closures wrong.”

[↑ Index](#index)

---

## const reassignment

**C:** assignment to `const` rejected with a clear **read_only** /
“Cannot assign to const variable” style message. for-of **const head** still
**rebinds each iteration** (not user assign).

Unskipped for-of body / classic for cases that only needed a real error.

[↑ Index](#index)

---

## no temporal dead zone

**Cases:** self-init style `let x = x` / related const/let (see
`statements/let`, `const`, `expressions/assignment`).

**Product:** Adaptive has **no TDZ**; RHS of self-init sees the new binding as
**undefined**. Converted with `differences` (not ES ReferenceError).

[↑ Index](#index)

---

## nullish assignment whitespace

**Case:** `expressions/logical-assignment.as` — `lgcl-nullish-whitespace`.

**→run:** Statement form with tab / space / LF / CR around `??=` (no
`assert.sameValue` harness). Exotic ES whitespace (NBSP, LS/PS in the
original) not all required for the Adaptive probe.

[↑ Index](#index)

---

## switch isNaN rewrite

**Case:** `statements/switch.as` — `S12.11_A1_T4`.

**→fix:** Dropped `Number` / `parseInt` / ES-only probes; Adaptive switch
with `is_NaN`, `null` fall-through, `Infinity`, `NaN` → default, etc.

[↑ Index](#index)

---

## assignment is statement only

**Cases:** `greater-than.as` / `greater-than-or-equal.as` LTR `A2.4_T1`;
`A2.4_T4` parenthesized `(y = 1) >= y`.

**Product (permanent):** Assignment is a **statement**, not an expression
value. No `(x = 1) > x`. **Not planned.** Documented in
[`typescript-differences.md`](../../../typescript-differences.md) and case
`differences`.

LTR tests use **throw + `safe_evaluate`** side effects (same idea as ES order
probes without assign-in-expr).

[↑ Index](#index)

---

## double mod IEEE template

**Cases:** `expressions/modulus.as` — pure IEEE `S11.5.3_A4_T*` (NaN,
Infinity, zero divisor, signed zero, finite remainder).

**→impl:** polymorphic **`mod<double>`** via **`fmod`** (sign of dividend;
NaN on invalid IEEE). Use **double** literals so `%` does not pick integer
mod (integer `%` still **throws** on divide-by-zero).

**→inc:** null/string/object `%` that rely on ES **ToNumber** — Adaptive
requires integer or double operands, no coercion.

**Division follow-on:** `expressions/division.as` — unskipped/rewrote pure
IEEE `S11.5.2_A4_T4`, `T5`, `T8`; fixed **false green** `A4_T7`
(integer `0/0` throw expect → double `0.0/0.0` → NaN). Coercion
null/object `/` → **Incompatible**. `A4_T9`/`A4_T10` still FIXME (extreme
literal exponents). Same pattern for later subtraction/`**`.

[↑ Index](#index)

---

## identifier resolution inner let

**Case:** `identifier-resolution.as` — `S10.2.2_A1_T4`.

Was **false green**: `expect: error:#1: Scope chain disturbed` (assert throw).
Adaptive: nested `f2` returns **outer** `x` (**0**), not inner `let x = 1`.
Converted to hard expect `0` + `differences` (no var-hoist / ES TDZ lineage).

[↑ Index](#index)
