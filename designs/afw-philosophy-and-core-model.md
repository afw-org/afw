# AFW philosophy and core model (maintainer framing)

**Audience:** maintainers, collaborators, and AI assistants who need *why AFW is shaped this way* and a stable mental model of the core runtime.  
**Not** published handbook, end-user docs, or always-on agent rules.  
**Not** a substitute for code, tests, or [`.cursor/rules/`](../.cursor/rules/).

**How to use this pad:** this is the **whole-story** framing (why AFW is shaped this way, and how the internal parts relate). For **where is X / probe**, use [`knowledge-atlas.md`](knowledge-atlas.md). For mantras, [`mantras-and-working-style.md`](mantras-and-working-style.md). For day-to-day implementation, rules, `AGENTS.md`, issue pads, and the tree. When this note and live code disagree, **code wins** — then **correct this pad**, do not delete a map because it feels long.

**Provenance:** distilled (2026-08) from pairing sessions (including a pass over mgg-develop work) about Adaptive Framework’s design intent and core runtime, plus later maintainer work. **This pad lives in git** (`designs/afw-philosophy-and-core-model.md`) — not only in a container home directory. Some inventory details below were snapshots at the time; the **philosophy and structural model** are what we keep. Do not treat test counts, exact inf lists, or unfinished memory polish notes as current status. Lifetime campaign: [`issue-2-lifetime.md`](issue-2-lifetime.md) (the remaining unresolved part of this story as of 2026-08, now recorded).

---

## Why this exists

AFW is large enough that grepping alone does not recover **design method** or **product taste**. Those live in the heads of people who built it. This pad is one place to keep that framing in git so:

- a new collaborator (human or assistant) can orient without replaying months of chat;
- support and development work stay aligned with the same invariants;
- knowledge is less dependent on any single working session — or any single person.

Direction is grown by **consensus** among the people building AFW — not one person’s decree and not this pad. Framing here is context for discussion, not a constitution.

---

## Design philosophy

These are the sticky choices that explain a lot of surface oddity:

| Principle | Meaning in AFW |
|-----------|----------------|
| **Metadata as single source of truth** | Object types, functions, data types, and C **interfaces** are defined once (generate metadata / interface XML). Headers, bindings, registration, and much docs/tests follow from that. Prefer fixing the source of truth over hand-editing generated output. |
| **Uniform models** | Capabilities show up the same way whether they came from core, an extension, or a host: environment registries, adaptive values, interface call macros. Prefer one pattern repeated over special snowflake stacks. |
| **Immutability first** | The script/eval world is an **immutable value graph** evaluated lazily. Mutation is deliberate and constrained (objects/faces, assignment into scopes) — not the default mental model of “everything is a bag of mutable state.” |
| **Pool-centric lifetime** | Memory is hierarchical pools (request/`xctx`/scope). **Destroy of a pool is lifetime; optional `free` is reuse.** Escaping uses `add_reference` / `release` so bulk free still works when a pool dies. Working story: [`issue-2-lifetime.md`](issue-2-lifetime.md). |
| **Small patterned pieces** | Prefer many small, named, interface-shaped pieces over one mega-framework. Extensions and packages stay as self-contained as practical against **public** core APIs. |
| **Curated Adaptive Script** | Syntax borrows *some* familiarity from modern languages (lambdas, destructuring, optional types, etc.) without becoming TypeScript/JavaScript: no prototype chains, no loose “anything goes” object model, Adaptive types and values on Adaptive terms. |
| **Production short path first** | Short-lived request/script work was the proven path (pool teardown cleans up). Long-running subscribers, shared compiled units, and process lifetime are the harder campaign — same model, stricter escape discipline. |

**What this is not:** a general-purpose OS, a browser JS engine, or “YAML config that grew a programming language.” It is a **metadata-driven C runtime** with a real compiler/evaluator and a Python tool (`afwdev`) that keeps the metadata story honest.

---

## Adaptive concepts

**Adaptive Objects** started it. The type square (handbook architecture) is the core:

| Noun | Role |
|------|------|
| **Adaptive Object** | The instance. Described by an Adaptive Object Type. |
| **Adaptive Object Type** | Describes one or more Adaptive Objects. **Is itself an Adaptive Object** (its type is `_AdaptiveObjectType_`). |
| **Adaptive Property** | A name plus an **Adaptive Value** on an object. |
| **Adaptive Property Type** | Describes a property (data type, constraints, …). Lives on the Object Type (usually under `propertyTypes`). |

**Adaptive Value** is a 2012 pillar, not a primitive sitting in the type square. A property’s payload is a value, but a value is much more than that: a typed instance (including object and array), or something that **evaluates** to one (call, compiled unit, symbol, …) via an Adaptive Value Interface. That is also the center of the libafw implementation (`afw_value_t`). Same idea at both layers, not two different “values.” **Adaptive Data Type** is how a kind of value is represented, compared, and converted.

The rest of **Adaptive Framework** is more concepts to *support* that square. The vision has not changed since 2012; the implementation has. **libafw** (`src/afw`) is the C implementation of most of it. The public **`afw` repository** is the **base**. See [`lineage-and-library-floor.md`](lineage-and-library-floor.md) and [`AGENTS.md`](../AGENTS.md) *Main components*.

Some implementation surfaces also use the Adaptive prefix (**Adaptive Script**, **Adaptive Template**; **Adaptive Expression** is the original language surface). Those are how you write against the vision, not extra pillars. Do not treat Script as the whole of AFW.

Handbook architecture and the glossary use the same names. When a row and the tree disagree, the tree wins.

| Noun | Role |
|------|------|
| **Adaptive Interface** | Formal contract (generate XML → C call macros / vtables). Core and extensions supply **implementations** (adapter, content type, request handler, …). |
| **Adapter** | Adaptive Interface for access to Adaptive Objects (and their types). Each `adapterType` is an implementation (file, ldap, model, …). |
| **Adaptive Mapping** | Transform objects between shapes / adapters. The shipped adapter implementation is the **model adapter** (`adapterType` `model`). Other adapter types could do mapping another way later. `_AdaptiveModel_` is what that adapter loads — not a 2012 pillar. |
| **Adaptive Layout** | Map objects and properties onto UI (layout objects and components). Peer of Adaptive Mapping. |
| **Adaptive Function** | Named function registered in the environment; called from expressions, scripts, or templates. |
| **Adaptive Environment** | Process registries. Extensions register the same way as core. |
| **Content type** | External encoding of Adaptive Objects and their values. |
| **Adaptive Schema** | The Adaptive Object Types an adapter presents. |
| **Adaptive Service** | How an adapter, log, authorization handler, and similar are configured and started. |
| **Request handler** | How a host turns a client request into work. |

Related: **authorization** (handlers on the environment).

---

## Intended uses (product framing)

Historically and still, AFW aims at places where **policy, mapping, and automation** need to be precise, embeddable, and inspectable:

- authorization and decision policies evaluated as Adaptive Script / expressions;
- object mapping and transformation across adapters and content types;
- long-running work that **compiles once** (e.g. into a longer-lived pool) and evaluates many times — memory discipline is umbrella **#2**; working story [`issue-2-lifetime.md`](issue-2-lifetime.md);
- tooling for people who are not full-time C programmers (higher-level script, admin surfaces, eventual natural-language → script paths);
- environments that care about clear lifetime and reentrancy (servers, embedded-ish hosts), not only one-shot CLI.

Hosts today include the **`afw` CLI** (including `--local`) and **`afwfcgi`**; both sit on the same core environment and value model.

---

## Core runtime model (enduring)

### Values are the center

This is the libafw side of the **Adaptive Value** concept above. Every significant script/eval entity is (or is reached through) an **`afw_value_t`**: an interface pointer (`inf`) plus payload. Evaluation is mostly **lazy** via `inf` methods (`optional_evaluate`, and for long-running correctness, `add_reference` / `release` — see value-memory rules, [`issue-2-lifetime.md`](issue-2-lifetime.md), and archaeology in [`memory-management.md`](memory-management.md)).

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

Full working story: [`issue-2-lifetime.md`](issue-2-lifetime.md) (2026-08-21). Archaeology: [`memory-management.md`](memory-management.md).

- **Destroy is lifetime. Optional `free` is reuse.** Hierarchical pools: process/base, request/`xctx->p`, **scope** children. Creating a child holds the parent. The xctx try/finally **destroys** `xctx->p` at request / `afw` command end — leftover holds do not keep the request alive.
- **Managed** = in the hold protocol (`add_reference` / `release` match). Managed objects/arrays have **their own pool** under `xctx->p`. Scalar temps that escape become a **managed wrapper in `xctx->p`**.
- **Unmanaged** = holds optional; count to zero does **not** destroy the instance; `add_reference` keeps **that instance’s pool**.
- **Permanent** / **compiled unit** = immutable; holds are no-ops. **Everything in a compiled unit is immutable.** Script mutates a **face** over literals, not the compiled instance.
- **`add_reference` / `release`** are not “GC for everything.” Classic example: `closure_binding` holding a **scope** so symbols survive `}`.
- Current pool impl (prefixes, first-fit) can **host** this protocol. A simpler wrap-APR pool is a later swap, not a gate.

Short scripts and request-scoped work were production-proven early because **destroying the request pool** papered over incomplete escape polish. Long-running processes need the full hold protocol — that is why **#2** remains a first-class campaign.

### Scopes and symbols

Execution carries an **`afw_xctx_t`**: pool, scope stack, evaluation stack, qualifier stack, **statement_flow** (sequential / break / continue / return / rethrow-style control — structured leave paths rather than C++ exceptions for normal script control).

Scopes (`afw_xctx_scope_t`) bind a pool, a block, lexical parent, and a **symbol value array** sized from the block. Resolution walks lexical depth and indexes symbols directly. Assignment (`let` / `const` / assign) is the script **`add_reference` site**; **read** is a pointer copy. Deactivate is one `release`; **last `release`** (not `}`) walks slots then lets the scope pool go. Closures keep the scope. **`return`** writes a hidden result slot and ends the block; assign into the **caller** happens when the block ends. Tree today: last `release` is still mostly pool-only — the walk is the #2 target.

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
3. **Prove** with Adaptive tests (`afwdev test -j`) and, for process-shaped bugs, live hosts / orchestrated leaves / valgrind as appropriate.  
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
| “optional_release/clone only remaining polish” | Directionally right for long-running; the real work is **#2**. Working story: [`issue-2-lifetime.md`](issue-2-lifetime.md). |
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

## How the parts relate

One process, several layers. Do not flatten them into one adjective (`managed`, `borrow`, `raw`).

```text
generate/ metadata + interface XML
        ↓  afwdev
  generated/ headers, bindings, registration
        ↓
  environment registries  (what exists)
        ↓
  compile  →  compiled_value  (own pool, one full_source)
        ↓
  evaluate values  (inf + payload; always returns a value)
        ↓
  hosts: afw CLI · afwfcgi · admin POST /afw
```

| Part | Job | Talks to |
|------|-----|----------|
| **Metadata / generate** | Functions, data types, object types, C interfaces defined once | `afwdev` → headers, bindings, env register, docs |
| **Environment** | Process-wide registries; discoverable on `adapterId=afw` | Extensions and hosts register the same way |
| **Compile** | Syntax → value graph | `compiled_value` owns pool + `full_source`; nodes hold **contextual** windows (offset/size), not copies of source |
| **Evaluate** | Walk the graph; `optional_evaluate` or already-a-result | New memory from the evaluate `p` or a **child** of `p` |
| **Values** | Public type is only `const afw_value_t *` (`inf` + private body) | Built-ins see **already evaluated** args (`AFW_FUNCTION_EVALUATE_*` → typed `arg->internal`) |
| **Payloads** | `afw_utf8_t` / `afw_memory_t` / `afw_integer_t` | **No pool, no reference count.** Lifetime is whoever owns the bytes. Doors: `create`/`set`/`no_copy` — [`c-naming-and-payloads.md`](c-naming-and-payloads.md) |
| **Objects / arrays** | Instances (maybe own pool); embedded `->value` is the Adaptive **name** | Script mutates a **face** (look-through + overlay). Dual `->value` ≠ face ≠ scalar box. Compiled literals stay immutable. |
| **Code points** | Unicode properties (identifier, whitespace, Cc) | `src/afw/code_point/` — encoding-neutral. UTF-8 encode/NFC stays in `afw_utf8` |
| **Hosts** | `afw`, `afwfcgi`, admin app | Same env. GET adapter CRUD ≠ POST `/afw` actions |

**Lifetime (working story 2026-08-21):** almost everything dies with a **pool**. Request/`afw` command: **destroy `xctx->p`** is the safety net. Managed objects/arrays have their own child of `xctx->p`. Child pools hold their parent. Long-running scripts need **`add_reference` / `release`** on slots and on object/array instances — not “the request was long enough.” Assign uses **`add_reference`** (today’s `clone_or_reference` was a memory-jog). Scalars that escape become a **managed box in `xctx->p`** (copy utf8/memory bytes). Objects/arrays **`add_reference` the instance**. Script faces isolate compiled/adapter bases. Detail: [`issue-2-lifetime.md`](issue-2-lifetime.md).

**Compile vs evaluate of `compile()`:** a script is compiled **once** and may be evaluated many times. Adaptive `compile()` during an evaluation must live on **`x->p`**, not the containing script’s compile pool, or every eval leaks ([#212](https://github.com/afw-org/afw/issues/212)). `parent` on `compiled_value` is not the backtrace — the **evaluation stack** + per-unit `full_source` is. Lex intern is `shared`, not a walk of `parent`.

**Errors:** `get_info()` → contextual → exact span in that unit’s `full_source`. Nested compile+eval prints each source as the stack changes. `decompile()` is compiled-form Adaptive (recompilable when supported), not original pretty source. `stringify()` is JSON of an evaluated value. Listing is the human tree.

**Print / untrusted bytes:** `forced_safe` makes viewable UTF-8 (`^hex^` for invalid/Cc, `^^` for caret) — not NFC, not a value. `afw_utf8_printf` / `z_printf` always encode that way (logs and traces, not data files). Property names at env/FCGI boundaries: same encode, then NFC (`create_property_name`).

---

## Related pads and hubs

| Doc | Role |
|-----|------|
| [`../AGENTS.md`](../AGENTS.md) | System map, agent mission, knowledge map |
| [`knowledge-atlas.md`](knowledge-atlas.md) | Topic → rules / pad / probe (the index, not a second story) |
| [`c-naming-and-payloads.md`](c-naming-and-payloads.md) | Value vs utf8/memory doors; `forced_safe`; code_point |
| [`mantras-and-working-style.md`](mantras-and-working-style.md) | Mantras, anti-patterns, partnership habits (reference) |
| [`issue-2-lifetime.md`](issue-2-lifetime.md) | **#2 working story** (2026-08-21) — holds, pools, assign, faces |
| [`memory-management.md`](memory-management.md) | #2 archaeology / old phases; superseded as the campaign map |
| [`runtime-objects-and-environment.md`](runtime-objects-and-environment.md) | Env registries as runtime objects |
| [`lineage-and-library-floor.md`](lineage-and-library-floor.md) | Base vs private packages; ICU home |
| [`agent-support.md`](agent-support.md) | Support playbook stubs; capture checklist |
| [`../whats-new.md`](../whats-new.md) | User/operator-facing notes on develop |

---

## Continuity

The goal of writing this down is practical: so someone working with Mike — or continuing work later — can still recover **how AFW thinks**, not only what the last PR touched. Assistants should use this pad as **orientation and taste**, then verify and implement against the living tree. Prefer improving this document with a short, dated correction over letting chat become the only memory of a design decision. **Do not trim maps** (how parts relate, lifetime sentences, named doors) to make the file short — correct them when the tree changes.
