# test262 `FIXME:` triage (first pass after skipReason sweep)

**Branch:** `test262-skipreason-sweep`  
**Scope:** inventory only — no converts required by this note.  
**Counts:** ~98 `FIXME:` (of 132 skips). `Incompatible:` / `Deferred:` / `Harness:` excluded here.

Use this to pick **a few** converts later. Prefer cases that need a small Adaptive rewrite or a single product decision over full ES ports.

## Priority shortlist (suggested convert / decide order)

| Pri | Cluster | Why next | Rough size | Example cases |
|-----|---------|----------|------------|---------------|
| **1** | **void rewrite** | Wrong probe (`isNaN` vs `undefined`); likely **unskip after body fix**, no language change | 2–3 | `expressions/void.as` `S11.4.2_A4_T4`, `_T5` (and `_T3` if needed) |
| **2** | **Raw line terminators in strings** | Product expects parse **error**; adapt `expect` / source, may already error | 4 | `literals/string.as` `S7.8.4_A1.1_T2`, `A1.2_T2`; `line-terminators.as` `invalid-string-cr` / `lf` |
| **3** | **for-of error quality** | Clear non-iterable / primitive / null errors — **product + messages**, high author value | 4 | `for-of.as` `head-expr-obj-iterator-method`, `…-primitive…`, `head-expr-to-obj` |
| **4** | **for-of temporal-dead-zone head / member LHS** | Real gaps; decide implement vs document permanent | 2 | `head-const-bound-names-fordecl-tdz`, `head-lhs-member` (member done) |
| **5** | **Temporal dead zone / self-init** | Adaptive: no temporal dead zone (undefined); opaque `closure_binding` errors → **#35/#2** or Adaptive `differences` | 3 | const/let self-init cases |
| **6** | **const reassignment errors** | Opaque errors on `const` assign in for / for-of | 2 | `const/syntax.as` for head + for-of body |
| **7** | **Leading-dot numerics** | Single product decision: support `.1e1` **or** rebucket **Incompatible** | 8 | `literals/numeric.as` `S7.8.3_A2.2_T*` |
| **8** | **String NonEscapeSequence** | Decide `\A === "A"` (and friends) or permanent non-support | 8 | `literals/string.as` `S7.8.4_A4.2_T2` etc. |
| **9** | **for-of string iteration** | ES code-unit / code-point walk vs one whole-string step — **#22** / product | 2 | `string-bmp`, `string-astral` |
| **10** | **try `cptn-*` completion** | ES completion values; large rewrite, lower beta urgency | 7 | `statements/try.as` `cptn-*` |
| **11** | **Arithmetic IEEE / coercion batch** | Many skips; need Adaptive `is_NaN` / double cases **without** `Number`/`Math`/`valueOf` | ~26 + related | `modulus.as`, `division.as`, `subtraction.as` (many labeled IEEE rewrite) |
| **12** | **`**` signed-zero / ∞ edges** | Needs double/`-0` policy + rewrite | 5 | `exponentiation.as` `applying-the-exp-operator_A*` |
| **13** | **Harness leftovers** | Still ES `valueOf` / `assert.throws` (also under **Harness:** prefix elsewhere) | various | division ASI; switch `is_NaN`; catch Pattern `#140` |

**Practical “convert a few” slice:** do **1 → 2 → 3** first (high chance of green unskips or clear product wins without a multi-week language project). Then **4–6** if focusing language quality.

## Closures and lifetime (**#35**, **#2**)

Adaptive **has** closures, but many ES-style tests still fail or only “pass” by expecting the wrong failure because of **escape / capture / lifetime** (pools, `closure_binding` JSON noise, for-of const “fresh binding per iteration” with closures, etc.).

| Tracker | Role |
|---------|------|
| **[#35](https://github.com/afw-org/afw/issues/35)** | Closures support — left open; many `closures.as` skips are escape/lifetime, not “no closures” |
| **[#2](https://github.com/afw-org/afw/issues/2)** | Value lifetime / memory — closures that escape scopes need managed paths |

**Policy for this suite:** when a case is really about **closure capture or escaped bindings**, prefer **`skip: true`** + **`FIXME: … (#35 / #2)`** (and correct `expect` for the *desired* Adaptive/ES outcome) over:

- leaving a green test that only asserts today’s broken capture, or  
- `expect: error:Assertion failed: …` as a permanent stand-in for “closures wrong.”

Do **not** try to “finish” those in a test262 labeling pass without #35/#2 design work.

### Converted on this branch (first slice)

| Cluster | Cases | Notes |
|---------|-------|--------|
| **1 void** | `void.as` `S11.4.2_A4_T3`–`T5` | Fixed bad `isNaN(void …)` → `void x === undefined`; T4 null only (`void undefined` still errors — note in `differences`); unskipped |
| **2 raw LT in strings** | `string.as` `S7.8.4_A1.1_T2`, `A1.2_T2`; `line-terminators.as` `invalid-string-cr` / `lf` | Unskipped as **Adaptive allows** raw CR/LF in strings (`differences`); assert length 1 |
| **3 for-of non-iterable** | `for-of.as` heads `{}` / boolean / number / null | Unskipped; **`expect: error:for-of head must be an array or string`** (C message) |
| **for-of member LHS** | `head-lhs-member` | **Fixed in C** (`impl_assign` accepts `reference_by_key`); unskipped |
| **for-of string iteration** | `string-bmp`, `string-astral` | **Fixed in C** (UTF-8 code points); unskipped + `differences` |
| **for-of temporal dead zone / semicolon insertion** | `head-const-bound-names-fordecl-tdz`, `let-array-with-newline` | Still **skip+FIXME** — language-wide, not for-of-only |
| **const assign** | `const` reassignment | **Fixed in C** (`read_only` / clear message); for-of body + classic for update tests unskipped |
| **no temporal dead zone (Adaptive)** | self-init `let x = x` etc. | Converted to Adaptive semantics + `differences` |
| **for-of const + closures** | `head-const-fresh-binding-per-iteration` | **skip+FIXME (#35 / #2)**; `expect: 0` desired — not `expect: error` stand-in |

## Theme inventory (all FIXME)

| Theme | ~N | Notes |
|-------|---:|-------|
| Arithmetic / IEEE / coercion rewrite | 26 | Mostly modulus/division/subtraction; half still need Adaptive operators, not ES objects |
| Half-converted / other rewrite | 16 | Numeric forms, try, switch, void, ASI |
| String escapes / line terminators | 11 | NonEscapeSequence + raw LT in strings |
| for-of | 10 | Errors, TDZ, member LHS, string iteration, newline ASI-ish |
| Numeric leading-dot | 8 | `.0e1` family |
| try/catch completion (`cptn-*`) | 7 | ES completion model |
| Exponentiation edges | 5 | −∞ / −0 / non-integer |
| Operators / eval order / `??=` / fn name | 4 | Side-effect order, binding id |
| TDZ / binding init | 3 | Clean errors |
| unary / void (decision) | 3 | Overlaps void rewrite + unary+ null |
| const assign / completion | 2 | |
| let shadowing rewrite | 1 | |
| line-terminators.as CR/LF | 2 | Overlaps theme “raw LT” |

## Explicitly **not** first convert targets

| Bucket | Why |
|--------|-----|
| **`Incompatible:`** (13) | e.g. `String.fromCharCode`, unary `+` on boolean — permanent unless product flips |
| **`Deferred:`** (2) | TCO deep recursion |
| **`Harness:`** (19) | Fix runner form first; then may move to FIXME/unskip |
| Full **cptn-*** / full **IEEE tables** in one go | Too large for “a few” |

## How to convert one candidate later

1. Open original under https://github.com/tc39/test262/tree/main/test/language if intent is unclear.  
2. Rewrite Adaptive `source` (assert / expect / no ES globals).  
3. Unskip; run `afwdev test` on that file.  
4. Refresh `description` / optional `differences`; keep prefixed `skipReason` only if still skipped.  
5. Prefer product decision notes in `differences` or #22 over silent behavior drift.

## Related

- Suite conventions: [`README.md`](README.md)  
- Skip prefixes also in `.cursor/rules/afw-tests.mdc`  
