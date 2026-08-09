# AFW philosophy and core model (maintainer framing)

**Audience:** maintainers, collaborators, and AI assistants who need *why AFW is shaped this way* and a stable mental model of the core runtime.  
**Not** published handbook, end-user docs, or always-on agent rules.  
**Not** a substitute for code, tests, or [`.cursor/rules/`](../.cursor/rules/).

**How to use this pad:** open it when questions touch product taste, origins, continuity, or “why not do X the usual way?” For **mantras and partnership habits**, prefer [`mantras-and-working-style.md`](mantras-and-working-style.md). For day-to-day implementation detail, prefer rules, `AGENTS.md`, issue pads, and the tree. When this note and live code disagree, **code wins** — then thin this pad.

**Provenance:** distilled (2026-08) from an earlier Grok conversation about Adaptive Framework’s design intent and core runtime, plus later maintainer work. Some inventory details below were snapshots at the time; the **philosophy and structural model** are what we keep. Do not treat test counts, exact inf lists, or unfinished memory polish notes as current status.

---

## Why this exists

AFW is large enough that grepping alone does not recover **design method** or **product taste**. Those live in the heads of people who built it. This pad is one place to keep that framing in git so:

- a new collaborator (human or assistant) can orient without replaying months of chat;
- support and development work stay aligned with the same invariants;
- knowledge is less dependent on any single working session — or any single person.

Mike’s product taste and hard “should we?” calls still own direction. This document is context, not a constitution.

---

## Design philosophy

These are the sticky choices that explain a lot of surface oddity:

| Principle | Meaning in AFW |
|-----------|----------------|
| **Metadata as single source of truth** | Object types, functions, data types, and C **interfaces** are defined once (generate metadata / interface XML). Headers, bindings, registration, and much docs/tests follow from that. Prefer fixing the source of truth over hand-editing generated output. |
| **Uniform models** | Capabilities show up the same way whether they came from core, an extension, or a host: environment registries, adaptive values, interface call macros. Prefer one pattern repeated over special snowflake stacks. |
| **Immutability first** | The script/eval world is an **immutable value graph** evaluated lazily. Mutation is deliberate and constrained (objects/faces, assignment into scopes) — not the default mental model of “everything is a bag of mutable state.” |
| **Pool-centric lifetime** | Memory is hierarchical pools and subpools (request/xctx/scope), not a general GC for all work. Escaping values use reference counting / managed policies so bulk free still works when a pool dies. |
| **Small patterned pieces** | Prefer many small, named, interface-shaped pieces over one mega-framework. Extensions and packages stay as self-contained as practical against **public** core APIs. |
| **Curated Adaptive Script** | Syntax borrows *some* familiarity from modern languages (lambdas, destructuring, optional types, etc.) without becoming TypeScript/JavaScript: no prototype chains, no loose “anything goes” object model, Adaptive types and values on Adaptive terms. |
| **Production short path first** | Short-lived request/script work was the proven path (pool teardown cleans up). Long-running subscribers, shared compiled units, and process lifetime are the harder campaign — same model, stricter escape discipline. |

**What this is not:** a general-purpose OS, a browser JS engine, or “YAML config that grew a programming language.” It is a **metadata-driven C runtime** with a real compiler/evaluator and a Python tool (`afwdev`) that keeps the metadata story honest.

---

## Intended uses (product framing)

Historically and still, AFW aims at places where **policy, mapping, and automation** need to be precise, embeddable, and inspectable:

- authorization and decision policies evaluated as Adaptive Script / expressions;
- object mapping and transformation across adapters and content types;
- long-running work that **compiles once** (e.g. into a longer-lived pool) and evaluates many times — with memory discipline still under active care (umbrella **#2**);
- tooling for people who are not full-time C programmers (higher-level script, admin surfaces, eventual natural-language → script paths);
- environments that care about clear lifetime and reentrancy (servers, embedded-ish hosts), not only one-shot CLI.

Hosts today include the **`afw` CLI** (including `--local`) and **`afwfcgi`**; both sit on the same core environment and value model.

---

## Core runtime model (enduring)

### Values are the center

Every significant script/eval entity is (or is reached through) an **`afw_value_t`**: an interface pointer (`inf`) plus payload. Evaluation is mostly **lazy** via `inf` methods (`optional_evaluate`, and for long-running correctness, release/clone policies — see value-memory rules and [`memory-management.md`](memory-management.md)).

Rough families of value kinds (names evolve; see `afw_value.h` / generated declares for truth):

- **Containers of work:** `compiled_value` (owns a pool; full source, literals, root graph), `block`
- **Calls:** generic `call`, `call_built_in_function`, `call_script_function`
- **Definitions / closures:** `script_function_definition`, `closure_binding`, function definition/thunk kinds
- **Structure builders:** list/array expressions, object expression / construct (historical name **list** still appears in C identifiers; product language is **array**)
- **References:** symbol, qualified variable, reference-by-key, assignment target
- **Other:** template definition, and data-type values for ordinary typed data

**Invariant:** think in **values first**; pools and C objects sit underneath. Hiding lifetime nastiness behind value policy is intentional.

### Compiled unit

`afw_value_compiled_value_t` is a **self-contained unit of compilation**: own pool, source mapping, literals/strings, top block / root value. Releasing the compiled value is meant to release that pool’s bulk allocation unless individual values have escaped with their own lifetime policy.

Compile entry points (e.g. `afw_compile_to_value` and related) place the result in a chosen pool (parent / shared / caller `p`) depending on API and host intent — important for “compile once, run many.”

### Pools and escape

- **Hierarchical pools:** process/base, thread or request-related, **subpools** for xctx and scopes.
- **Subpool destroy:** returns tracked memory to the parent story and deals with survivors that must outlive the subpool (reparent / refcount paths — details in code and memory pads).
- **Reference counting** is not “GC for everything”; it is for **escaping** values (classic example: `closure_binding` holding a scope alive while the closure remains reachable).
- **Data-type value lifetimes** (permanent / managed / managed_slice / unmanaged) are part of the same story; long-running work cares about managed release/clone and evaluating into the right pool (`scope->p`).

Short scripts and request-scoped work were production-proven early because **destroying the request pool** papered over incomplete escape polish. Long-running processes need the full story — that is why **#2** remains a first-class campaign, not a footnote.

### Scopes and symbols

Execution carries an **`afw_xctx_t`**: pool, scope stack, evaluation stack, qualifier stack, **statement_flow** (sequential / break / continue / return / rethrow-style control — structured leave paths rather than C++ exceptions for normal script control).

Scopes (`afw_xctx_scope_t`) bind a pool, a block, lexical parent, and a **symbol value array** sized from the block. Resolution walks lexical depth and indexes symbols directly. Assignment (`let` / `const` / assign) stores into the scope and participates in refcount/lifetime rules. Deactivate/release walks symbols and releases values.

### Compilation

Parsers under `src/afw/compile/` build the value graph. Grammar fragments live in **`/*ebnf>>>` … `<<<ebnf*/`** comments and are harvested for docs/railroads — the compiler is the authority; EBNF is documentation-shaped, not a second implementation. Assignment can auto-wrap escaping script functions in **closure bindings** when depth requires. Destructuring and rest are part of the language surface.

### Functions

Built-ins live in hand-written `afw_function_*.c` (execute helpers, evaluate-into-`x->p` patterns). Script functions compile to definition + body block. Built-in and script calls share the **call node** world; **polymorphic** built-ins resolve using runtime argument types (typically first argument) where metadata says so. Security hooks (e.g. execute-access requirements) wrap sensitive entry points.

### Safety and mapping

- **Source contextual:** values can carry compile contextual (`compiled_value` + offset/size into full source) so errors point at real script text.
- **No ambient global script heap** as the primary model; work is rooted in env + xctx + pools.
- **Reentrancy / threads:** hosts and core aim for registry and request patterns that can be used from multi-worker servers (`afwfcgi`), not only single-threaded toys.

### Environment (companion to values)

Almost every capability is registered on the process **environment** and is discoverable as runtime objects on `adapterId=afw`. Values are how script *runs*; the environment is how the process *knows what exists*. See [`runtime-objects-and-environment.md`](runtime-objects-and-environment.md) and `afw-environment` rules. That split is intentional: metadata → register → call through interfaces → evaluate values.

---

## Design method (how work is supposed to happen)

1. **Prefer the generate path** for anything that is metadata (functions, types, interfaces, strings).  
2. **Implement** in hand C/Python only where the generator leaves a closet or where behavior is inherently hand-written (parsers, pool impls, host mains).  
3. **Prove** with Adaptive tests (`afwdev test -j`) and, for process-shaped bugs, live hosts / advanced-test / valgrind as appropriate.  
4. **Promote knowledge** after deep work: short maps and invariants into git; do not rely on chat alone.  
5. **Keep packages movable:** base repo holds core + shipped srcdirs; extensions should not glue themselves into core private headers.

This method is why “small patterned pieces” and “metadata single source of truth” show up everywhere — including in how assistants are asked to work (`AGENTS.md`, always-on rules).

---

## Historical snapshot notes (do not treat as current status)

From the earlier dump and early AI onboarding — useful archaeology, easy to misuse:

| Snapshot claim | How to read it now |
|----------------|--------------------|
| Exact “~12 core inf” list | Illustrative of the **graph node** idea; more kinds exist (e.g. function thunk, object construct). Always check `afw_value.h`. |
| `list_expression` | C name heritage; product term is **array**. Do not revive “bag” for objects. |
| “No exceptions” | Script control is **statement_flow** / structured leave; Adaptive still has error/throw paths and C error macros. |
| “2730/2902 tests” | Point-in-time CI flavor only; ignore for progress math. |
| “optional_release/clone only remaining polish” | Directionally right for long-running; the real work is multi-phase (**#2**, managed policies, containers, faces, catalog lifetimes). See memory pad. |
| “Use as permanent context for all future questions” | Superseded by this **thin pad** + live code/rules. Do not paste the raw dump into always-on rules. |

---

## Key code entry points (orientation, not exhaustive)

| Area | Start here |
|------|------------|
| Value model | `src/afw/value/afw_value.h`, `afw_value_internal.h`, `afw_value_compiled_value.c`, block/call/script_function/closure sources |
| Pools | `src/afw/pool/` (`afw_pool*.c`) |
| Execution context | `src/afw/xctx/` |
| Compile / EBNF | `src/afw/compile/`, especially script parse + harvest comments |
| Built-in functions | `src/afw/function/afw_function_*.c` + generate function metadata |
| Environment | `afw_environment.h`, `afw_environment_register_core.c` |
| Tooling | `src/afw_dev/` (`afwdev`) |
| Hosts | `src/afw_command/` (`afw`), `src/afw_server_fcgi/` (`afwfcgi`) |

**Rules for depth:** `afw-runtime-model`, `afw-value-memory`, `afw-script-eval`, `afw-compile`, `afw-environment`, `afw-c-runtime`.

---

## Related pads and hubs

| Doc | Role |
|-----|------|
| [`../AGENTS.md`](../AGENTS.md) | System map, agent mission, knowledge map |
| [`mantras-and-working-style.md`](mantras-and-working-style.md) | Mantras, anti-patterns, partnership habits (reference) |
| [`memory-management.md`](memory-management.md) | Long-running pools / escape / #2 |
| [`runtime-objects-and-environment.md`](runtime-objects-and-environment.md) | Env registries as runtime objects |
| [`agent-support.md`](agent-support.md) | Support playbook stubs; capture checklist |
| [`../whats-new.md`](../whats-new.md) | User/operator-facing notes on develop |

---

## Continuity

The goal of writing this down is practical: so someone working with Mike — or continuing work later — can still recover **how AFW thinks**, not only what the last PR touched. Assistants should use this pad as **orientation and taste**, then verify and implement against the living tree. Prefer improving this document with a short, dated correction over letting chat become the only memory of a design decision.
