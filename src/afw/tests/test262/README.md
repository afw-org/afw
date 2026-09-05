# Language Tests (test262-derived)

These folders contain tests that were derived from the Official ECMAScript
Conformance Test Suite (**test262**), language chapter only:

https://github.com/tc39/test262/tree/main/test/language

Each test has an id whose prefix aligns with the chapter/section in the
ECMAScript Language Specification (historically 5.1 edition numbering is a
useful map):

https://262.ecma-international.org/5.1/

If a folder in test262 contained several tests, we usually collapsed them
into one Adaptive `.as` file with multiple `//? test:` cases to keep the
file count manageable.

Comment BMP sweeps (`S7.4_A5` / `S7.4_A6`) stay as ES `\\uXXXX` over Jeremy's
code-point grid (internally UTF-8). They are split into `comments-bmp-*.as`
so valgrind can finish; `comments.as` keeps the other cases plus a
supplementary-plane UTF-8 check. They run in default `afwdev test -j`.

## Why this suite exists

Adaptive Script is **not** ECMAScript, but we intentionally share a lot of
syntax and everyday semantics so authors who know JavaScript/TypeScript are
not surprised. This tree is a **regression fence** for that shared surface:
when Adaptive means to behave like ES for a construct, keep it working that
way. It is **not** a goal to pass full test262 or to grow prototypes,
`class`, DOM, or a JS runtime.

Broader product notes: root `typescript-differences.md` (maintainer/beta).
Polished author-facing ES differences may live under GitHub **#22**.

## Conversion history

The full Conformance Tests rely on Objects-as-ES-exotics, generators, async,
prototypes, and a small set of primitive types. Grammar names and EBNF also
differ. We took a **subset** of the language tests and adapted them (and
their descriptions) for Adaptive Script.

`_convert.py` in this directory was a **helper** for a first-pass `.js` →
`.as` conversion. Most cases still needed **hand** adjustment after it ran.

## Adaptive test_script metadata (official tags)

Cases use `//?` keys (see also `.cursor/rules/afw-tests.mdc` and the
TestScript EBNF in `afw_compile_parse_script.c`). The compiler accepts **any**
identifier as a custom property; the following are the **documented
official** ones for this suite and for language tests in general:

| Key | Role |
|-----|------|
| **`test`** | Case id (stable name) |
| **`description`** | Stay **close to the TC39 description**. Skip notes go in **`skipReason`**. For **`expect: error`**, say why in a source **`//`** comment — do not rewrite `description` to explain the Adaptive error. |
| **`differences`** | Optional. **Language** differences between ECMAScript and Adaptive for the construct under test only. Not harness wrapping. Good future harvest input for differences docs. |
| **`expect`** | **`success`** (compiled and ran, ignore result — usual test262 “did not throw”), Adaptive value (`0`, `undefined`, …), or **`error`** / **`error:…`** |
| **`skip`** | `true` / `false` — do not run |
| **`skipReason`** | Why skipped (see prefixes below). Long text: use `//? skipReason: ...` then body lines until the next `//?` (same `...` form as `description` / `source`; any key can do this for ~80-column editing) |
| **`source`** | Adaptive Script body of the case |
| **`sourceType`** | Usually `script` (file-level default OK) |

#### Suggested `skipReason` prefixes

Not enforced by the compiler — a shared vocabulary so bulk reviews and
harvests stay consistent. Prefer a **prefix + short clause**:

| Prefix | Use when |
|--------|----------|
| **`FIXME:`** | Adaptive should fix or decide; stays on the burn-down list |
| **`Never:`** | We do not plan to support this (ES-only, decided-not) |

Examples:

```text
//? skipReason: Never: no String.fromCharCode / ES String prototype APIs
//? skipReason: FIXME: produce-type false positive (runtime would pass)

// Long reason (any //? key can use this form):
//? skipReason: ...
FIXME: skipped because this body would pass at runtime and be a
false positive. The real check is compile-time produce-type.
```

Do **not** use **`Never:`** for “not yet” features — use **`FIXME:`** and
say when/why in the sentence.

**First-pass status (2026-08, branch `issue-#106-fixme-reclassify`):** two
prefixes only. Counts drift as cases unskip or reclassify — re-grep after
edits.

**Leftover `FIXME:`** (not a convert shortlist): [`FIXME-triage.md`](FIXME-triage.md).

**Case change log (what we already did):** see [`changes.md`](changes.md) —
Index of unskips / rewrites / product `differences` **since the mid‑July
2026 fork** (merge-base `f945f97c`; same horizon as root `whats-new.md`).
Two-way links like `whats-new.md`. Update it when you change a case’s
run/skip/expect meaning.

Suggested order when several are present:

```text
//? test: name
//? description: TC39-ish purpose
//? differences: ES vs Adaptive for this construct (optional)
//? expect: 0
//? skip: true
//? skipReason: FIXME … or permanent non-support reason
//? source: ...
```

### Harness pattern (document once, not per test)

Shared adaptation for Adaptive’s runner (try/`assert`, `return 0`, no bare
expression statements, no `assert.throws` / `typeof`, etc.) is **the same
for almost every case**. That pattern lives in developer/test rules
(`.cursor/rules/afw-tests.mdc`); do **not** restate it in every
`description` or `differences`.

If a case has something **unusual**, put a short **`//` comment in the
source** next to the oddity. No special metadata tag is required for
harness notes.

### What belongs in `differences` vs comments

| Put in **`differences`** | Put in **source `//` comments** (or nowhere) |
|--------------------------|-----------------------------------------------|
| Dense elision vs ES sparse holes | try/catch instead of `assert.throws` |
| Structural `===` on objects | `return 0` / shebang |
| No `for (x.y of …)` member LHS | “no typeof; assert the value only” |

## Running

From the package root (narrow as needed):

```bash
afwdev test -j --srcdir-pattern afw --test-pattern 'test262'
```
