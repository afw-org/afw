# test262 `FIXME:` triage

**Branch:** `test262-skipreason-sweep` (stacked on `mgg-develop` since merge-base `f945f97c`)  
**Done log:** [`changes.md`](changes.md) — what already converted  
**Suite rules:** [`README.md`](README.md)

**Skip counts (approx, re-grep after edits):** ~65 skips total — **FIXME ~26**, **Incompatible ~18**, **Harness ~19**, **Deferred ~2**.

Use this to pick **next** converts. Prefer small rewrites or single product decisions over full ES ports.

## Priority shortlist (remaining)

| Pri | Cluster | Status | Rough size | Notes |
|-----|---------|--------|------------|-------|
| **1** | **Arithmetic IEEE (more)** | In progress | division/subtraction/`**` residual | **modulus** pure `A4_*` + **division** `A4_T4/T5/T7–T9` done; coerce → **Incompatible** |
| **2** | **try `cptn-*` completion** | Done (#62) | — | Assignment probes; see [`changes.md`](changes.md) |
| **3** | **`**` signed-zero / ∞** | Open | ~5 | `exponentiation.as` edges |
| **4** | **for-of TDZ / ASI leftovers** | Open | ~2–3 | `head-const-…-tdz`, `let-array-with-newline`; language-wide |
| **5** | **for-of const + closures** | Blocked | 1 | **#35 / #2** — keep skip+FIXME, honest expect |
| **6** | **Harness leftovers** | Open | ~19 | `valueOf` / `assert.throws` / ASI block-eval |
| **7** | **Half-converted try/catch Pattern** | Open | few | `#140` residuals |

**Already done on this line of work** (see [`changes.md`](changes.md)): void probes + `void` undefined; raw LT in strings; for-of non-iterable / member LHS / string CP; const reassignment; no TDZ self-init; leading/trailing-dot numerics; NonEscape/`\x`/`\0`/line-continuation; unary+ identity; `??=` whitespace; switch rewrite; LTR without assign-in-expr; modulus + division IEEE template; #55 array helpers; #39 elision; #140 param defaults; #62 try `cptn-*` / `S12.14_A6` assignment probes.

## Closures and lifetime (**#35**, **#2**)

Adaptive **has** closures; many ES-style fails are **escape / capture / lifetime**. Prefer **`skip: true`** + **`FIXME: … (#35 / #2)`** with correct desired `expect` — not false-green `expect: error` stand-ins.

## Theme inventory (remaining FIXME-ish)

| Theme | Notes |
|-------|--------|
| Arithmetic IEEE / coercion | More `division`/`subtraction`/`**`; ToNumber cases → Incompatible |
| try/catch `cptn-*` | Done (#62): assignment probes, not ES UpdateEmpty |
| for-of TDZ / ASI / closures | Language / #35 |
| Harness | Runner form first |
| Half-converted try/switch | Rewrite under #140 / differences |

## Explicitly **not** first convert targets

| Bucket | Why |
|--------|-----|
| **`Incompatible:`** | e.g. `String.fromCharCode`, unary `+` boolean, `%`/`/` ToNumber — permanent unless product flips |
| **`Deferred:`** | TCO deep recursion |
| **`Harness:`** | Fix runner form first |
| **Assignment as expression** | Permanent non-support (typescript-differences) |
| Full **cptn-*** / full IEEE in one go | Too large for one PR slice |

## How to convert one candidate later

1. Open original under https://github.com/tc39/test262/tree/main/test/language if intent is unclear.  
2. Rewrite Adaptive `source` (assert / expect / no ES globals).  
3. Unskip; run `afwdev test` on that file.  
4. Refresh `description` / optional `differences`; keep prefixed `skipReason` only if still skipped.  
5. Add/update a row in [`changes.md`](changes.md) in the **same** change.  
6. Prefer product notes in `differences` or #22 over silent behavior drift.

## Related

- Suite conventions: [`README.md`](README.md)  
- **Done log:** [`changes.md`](changes.md)  
- Skip prefixes also in `.cursor/rules/afw-tests.mdc`  
