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
| **`description`** | What the case checks. Prefer staying **close to the TC39 description**, lightly tweaked for Adaptive wording. |
| **`differences`** | Optional. **Language** differences between ECMAScript and Adaptive for the construct under test only. Not harness wrapping. Good future harvest input for differences docs. |
| **`expect`** | Expected result (`0`, `error`, …) |
| **`skip`** | `true` / `false` — do not run |
| **`skipReason`** | Why skipped (see prefixes below). Long text: use `//? skipReason: ...` then body lines until the next `//?` (same `...` form as `description` / `source`; any key can do this for ~80-column editing) |
| **`source`** | Adaptive Script body of the case |
| **`sourceType`** | Usually `script` (file-level default OK) |

#### Suggested `skipReason` prefixes

Not enforced by the compiler — a shared vocabulary so bulk reviews and
harvests stay consistent. Prefer a **prefix + short clause**:

| Prefix | Use when |
|--------|----------|
| **`Incompatible:`** | Never plan to convert (generators, `class`, prototypes, typed arrays via `new`, full ES iterator exotics, …) |
| **`FIXME:`** | Adaptive should fix or decide; stays on the burn-down list |
| **`Deferred:`** | Plausible later, not current priority |
| **`Harness:`** | Rare — blocked by runner/adaptation limits, not language (prefer fixing harness instead) |

Examples:

```text
//? skipReason: Incompatible: ES generators / function*
//? skipReason: Incompatible: String.fromCharCode and prototype string APIs
//? skipReason: FIXME: for-of member LHS (x.y)
//? skipReason: FIXME: TDZ for for-of head const binding
//? skipReason: Deferred: deep TCO; not planned for beta

// Long reason (any //? key can use this form):
//? skipReason: ...
FIXME: Adaptive for-of does not enforce TDZ for for-of head const
binding (outer x may be readable in [x]); ES requires error.
```

Plain **`Incompatible`** with no detail is fine for a bulk first pass; enrich
when you touch the file. Do **not** use `Incompatible` for “not yet” features
that Adaptive might still implement — use **`FIXME:`** or **`Deferred:`**.

**First-pass status (2026-08, branch `test262-skipreason-sweep`):** every
skipped case has a prefixed `skipReason`. Counts will drift as cases unskip or
reclassify — re-grep after edits. Use **`Incompatible:`** only for ES-only
behavior Adaptive does not plan to support (confirm against the original
test262 case when unsure); default uncertain skips to **`FIXME:`** or
**`Deferred:`**.

**FIXME triage (what to convert next):** see [`FIXME-triage.md`](FIXME-triage.md)
— shortlist and theme inventory after the labeling pass.

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
