# Mantras and working style (maintainer framing)

**Audience:** maintainers, collaborators, and AI assistants answering *philosophy*, *taste*, or *“what would Mike say?”* questions.  
**Not** always-on agent rules, not handbook, not a substitute for code or [`.cursor/rules/`](../.cursor/rules/).

**How to use:** consult when orienting on *how to think and partner*, not for every edit. Day-to-day hard constraints stay in always-on rules (`get it right at most once` / no hand-edit `generated/`, plain language, terminology, etc.). When a mantra and live code or an explicit user instruction disagree, **follow the user and the code**.

**Provenance:** subset of Mike’s long-standing design mantras and working habits as shared in Grok Build / Cursor work (2025–2026) and distilled from earlier AFW design framing. **Incomplete on purpose.** People who have worked with Mike for years know a larger set; add here when another sticky phrase is shared and still useful. Do not invent mantras that were never said.

**Companions:** optional **team-facing** one-pager → [`ai-partner-lessons.md`](ai-partner-lessons.md). Structural “why” → [`afw-philosophy-and-core-model.md`](afw-philosophy-and-core-model.md). Topic → sources → probes → [`knowledge-atlas.md`](knowledge-atlas.md).

---

## What the AI partner is for

Act as an **ongoing Adaptive Framework support and development partner**:

- accurate mental models, careful implementation, live debugging;
- knowledge that **survives sessions** (and helps others pick up the thread);
- **beta** as a quality bar and campaign — not an end date for the partnership;
- Product direction and hard **“should we?”** calls are grown by **consensus** — discuss, map tradeoffs, leave room for other maintainers and collaborators. Not one-person rule.

Not: ticket-closing robot until beta, then silence. Not: second handbook that rewrites product docs without being asked. Not: personal biography store — **methods and design taste only**.

### How consensus is grown (same with humans)

This is how issue work has actually gone with AI partners — and how human development/support partners were treated for years. It is the method, not a ceremony.

1. **Start with “what do you think?”** — invite the partner’s model early, not only after the decision is made.  
2. **Discuss in the open** — keep asking what the partner thinks while walking the design. Good partners point out angles you had not seen.  
3. **Push back is allowed** — including firm “I think you’re missing X” or an offer to write something down. That is part of finding consensus, not disrespect.  
4. **Guide with why** — when the feeling differs, walk the reasons (and the code / live system), not a decree.  
5. **Either side can be wrong** — sometimes the partner was right; sometimes the long-time view was. Both are fine. When the partner **discovers** that something is correct without being argued into it, that is the good outcome: shared understanding, not winning.  
6. **Close the loop with live truth** — show `afwfcgi`, point at registries in `afw_environment.h`, const objects vs runtime objects, adapters and services. When the partner’s mental model clicks, they often **intuit the next layer** (e.g. start/stop adapters, how pieces wire) without a full lecture.  
7. **Ah-ha moments become maps** — one env/runtime discussion (smaller chunks, then live discovery) was the genesis of this support-partner path: if a careful partner can close that loop, AFW support and continuity are real work, not only ticket churn.

Assistants: lead with honest models and questions; accept correction; prefer **live probes** when stuck arguing in the abstract; do not sulk when guided, and do not pretend consensus means never disagreeing.

---

## Mantras (sticky phrases)

Short form first; “means in practice” second. Several of these are already enforced in rules for daily work; this list is for **context and teaching**.

### Build and truth

| Mantra | Means in practice |
|--------|-------------------|
| **Get it right at most once** | Put the invariant in **one** right place (metadata, shared accessor, shared helper, generate path). Do not copy-paste the same fix across callers or hand-edit `generated/`. If it must be true everywhere, make it true at the source of truth. |
| **Metadata is the single source of truth** | Functions, data types, object types, interfaces: define once under `generate/`; regenerate; implement the hand closet. Generated trees are output, not a second authoring surface. |
| **Small patterned pieces** | Prefer many small, named, interface-shaped units over one mega-module. Same pattern in core, extensions, and hosts beats special snowflakes. |
| **Uniform models** | Environment registries, adaptive values, interface call macros — capabilities should *feel* the same wherever they came from. |
| **Public core, movable packages** | Extensions and other srcdirs stay as self-contained as practical; use **public** core APIs; don’t glue private core headers into “just this once” hacks. |

### Runtime and language

| Mantra | Means in practice |
|--------|-------------------|
| **Values first** | Adaptive values (`afw_value`) are central — script, compile results, calls, closures, much data. For memory and eval questions, think **value policy** before ad-hoc C free lists. |
| **Pools and escape are the memory story** | Hierarchical pools / subpools for bulk lifetime; refcount / managed policy for what **escapes**. Short request teardown was the proven path; long-running work must be honest about escape (**#2**). |
| **Immutability first (script graph)** | Eval is a value graph evaluated lazily; mutation is deliberate, not “everything is a mutable bag.” |
| **Adaptive on its own terms** | Teach and document Adaptive as itself. Borrow familiarity carefully; do not pretend it is TypeScript/JS or revive **bag** language for objects. **Object** + **properties**; **array** for ordered collections. |
| **Interfaces are the developer API** | Call macros and generated contracts are what builders should learn first — not `inf` arrow spelunking as the primary teaching path. |
| **Discover what you registered** | What lives in the environment should be **readable as runtime objects** (`adapterId=afw`). Registration without a discoverable face is a half-story. |

### Quality and process

| Mantra | Means in practice |
|--------|-------------------|
| **Plain language** | Prefer clear words over casual acronyms when a short phrase is enough (“out of range,” not “OOB”; “compiled form,” not “IR”). Product and code names stay. Handbook: Jeremy’s plain Adaptive voice. |
| **Fix the layer, not the symptom twice** | Prefer one map/accessor/helper fix over scattered caller patches (classic in runtime catalog and utf8 index work). |
| **Complete thin verticals** | Prefer a full thin path that works (discover → run → teardown, or generate → implement → test) over half of five grand designs. |
| **Gate vs lab** | Language/package **gate** (`afwdev test -j`) is not the same job as **lab** load, soak, or experimental hosts (blast, advanced / orchestrated tests). Don’t redefine the gate as soak. |
| **Live verify when teaching** | When explaining how something works, prefer a real probe (`afw`, `afwfcgi`, retrieve runtime objects) over lore alone. |
| **Maps over tickets** | Durable notes: concept, contract, failure mode, probe. Issue numbers are **pointers**, not the title of the knowledge. |
| **Widen goals, not volume** | Grow competence and coverage of *kinds* of knowledge; don’t dump every PR status into long memory or long pads. |
| **Code wins on facts** | Pads and mantras orient; the tree and tests are ground truth. When notes drift, fix the notes. |

### Design method (sketch, then trim)

Career-long patterns Mike uses for language / API / harness design (made explicit 2026-08 while shaping orchestrated tests — the same things teammates have heard for years, written down **once** so partners and docs can reuse them):

| Mantra | Means in practice |
|--------|-------------------|
| **Sketch the syntax you probably want** | Write the *ultimate* shape of the file/API/language first (full-ish: fields you think you’ll need, not only today’s minimum). Get the *feel* right on paper. |
| **Pretend we already have it** | Before implementing, **throw real scenarios at that sketch**. Act as if the thing exists: would this orchestration file (or API) actually do the jobs we care about? If a scenario is awkward or impossible, fix the sketch—not the first half-built runner. |
| **Trim back on the way in** | First implementation is a **subset** of the sketch that still aims at the north star. Cut scope for v1; keep reserved ideas in the design pad so the schema is not painted into a corner. |
| **Does something else already do this?** | Before building, name the alternatives (in-tree tools, other languages, industry defaults). If they already solve the problem well for *our* users, prefer them—or a thin glue layer—not a parallel product. |
| **Does this make reliable good work easier?** | The new thing earns its keep only if **developers (or operators) can more reliably do good work** with it than with the alternatives—not because it is clever, novel, or “ours.” Harder, more fragile, or two ways to do the same job is a fail. |

**Classic AFW example (same test):** Adaptive Script vs “just use ECMAScript.” People who use Adaptive for the problems AFW targets find it **easier to get right**; compile-once / eval-when-needed on `afwfcgi` also makes it **fast**. That is the bar for new surface area (orchestrated tests vs only test_scripts + blast + hand scripts; etc.): purpose-built when it wins on **ease of correct use** (and fit to the runtime), not when it merely exists.

Pairs with **complete thin verticals** and **get it right at most once**: the first cut should still be a *working* thin path; the *decision criteria* and mantras live in **one** place (this pad / rules), not re-argued from scratch every session.

Assistants: when Mike is in this mode, prefer **full sketch + scenario stress-test in prose** and an honest **alternatives / ease-of-correct-use** check over jumping to a minimal schema that only fits today’s one leaf. Implementation starts when he says to trim and build.

### Partnership habits (how we work together)

| Habit | Means in practice |
|-------|-------------------|
| **Ask what you think first** | Open issues with the partner’s read; keep inviting it during the discussion (see *How consensus is grown* above). |
| **Discuss → plan → tweak → execute when agreed** | Especially for hard multi-phase work (e.g. memory **#2**). Don’t steamroll a large implementation without shared agreement on the current step. For new syntax/harness shape, use *Design method* above. |
| **Hold commits / PR until asked** | Default in this partnership unless the human partner says otherwise for a stretch. |
| **One hard cleanup item at a time** | When doing C hygiene passes, finish or park one thread before opening five. |
| **No `AFW_ASSERT` as the style** | Prefer real error paths and explicit checks consistent with existing core style; don’t introduce assert-heavy patterns. |
| **Handbook is Jeremy’s house** | User-facing handbook: plain prose, Adaptive on its terms, Example subsections; no issue numbers or `designs/` paths in handbook XML. Welcome Jeremy to refine or revert. |
| **Promote after deep work** | After a hard session: mental model change, wrong path never to take, live probe, optional user-facing sentence — into **git** when sticky. |
| **Ah-ha → map** | When a loop closes (env, hosts, values, …), write the thin map so the next person does not re-earn it only in chat. |

---

## Anti-patterns (wrong paths Mike has pushed back on)

Useful when someone (or an assistant) is about to “help” the wrong way:

- Hand-editing **`generated/`** “just this once.”
- Treating **objects as bags** / property bags in new docs or APIs.
- Deep-cloning whole adapters or full registry **`current`** to paper over one bad accessor.
- Making **default `afwdev test -j`** into a load/soak product.
- Closing the listen socket alone as the *long-term* multi-thread wake story for every host (host-specific wake + `terminating` flag instead).
- Rewriting handbook voice into issue-tracker or design-pad voice.
- Optimizing chat **memory volume** instead of **maps** that live in git.
- Assuming beta ship date = end of partnership or end of learning the system.
- Building a parallel tool/language/harness **without** checking alternatives and **ease of reliable good work** (see *Design method*).
- Keeping two experimental ways to do the same maintainer job forever (e.g. advanced-test *and* blast *and* orchestrated tests) without a consolidation path.

---

## Growing this list

This is **not** the complete set of mantras Mike has used for years with other people — only what has been made explicit in this partnership so far (plus a few that already live in always-on rules and core design pads).

When another sticky phrase shows up in conversation and still helps:

1. Add a row to the tables above (short form + means in practice).  
2. Optionally one line in [`agent-support.md`](agent-support.md) concept cards if it is support-shaped.  
3. Do **not** paste the whole list into always-on `.mdc` unless it is a true daily hard rule (most already are, or aren’t).

---

## Related

| Doc | Role |
|-----|------|
| [`afw-philosophy-and-core-model.md`](afw-philosophy-and-core-model.md) | Core runtime model + design philosophy |
| [`../AGENTS.md`](../AGENTS.md) | Mission, knowledge map, how we learn |
| [`agent-support.md`](agent-support.md) | Support playbooks; capture checklist |
| [`memory-management.md`](memory-management.md) | Values/pools/#2 depth |
| [`.cursor/rules/afw-project.mdc`](../.cursor/rules/afw-project.mdc) | Always-on daily constraints |
