# Lessons learned: AI partner for AFW development and support

**Audience:** maintainers and collaborators who might work with an AI assistant on AFW.  
**Status:** optional lessons from practice — **follow or not**. Not a required process, not product docs, not always-on agent rules.

If you never use an AI partner, you can ignore this page entirely.

---

## Intent

Treat an AI assistant as an **ongoing support and development partner** for Adaptive Framework: careful implementation, real debugging, and knowledge that can survive a chat session — not only “close tickets until beta.”

Product direction and hard “should we?” choices still grow by **consensus** among people building AFW. The assistant surfaces options and tradeoffs; it does not own the product and does not replace code review or human judgment.

These notes come from pairing on real issues (runtime catalog, graceful stop, afwdev harness, and the maps that followed). They match how human development and support partners have been treated for a long time: discuss, disagree, show the system, share the win when understanding clicks.

---

## What worked

### 1. Start with “what do you think?”

Open an issue or design question by asking the partner for **their** read first — not only after you have already decided. Keep asking while you walk the design. Good partners notice angles you missed.

### 2. Let disagreement happen

Pushback is allowed, including firm “I think you’re missing X” or an offer to write something down. That is part of finding consensus, not a failure of manners.

When you see it differently, **guide with why** — reasons, code, live behavior — not a decree. Either side can be wrong. The good outcome is when the partner **discovers** that something is correct without being argued into submission. Shared understanding beats winning.

### 3. Close the loop on a live system

When chat gets abstract or stuck, show the real thing:

- `afw` / `afwfcgi`
- registries in `afw_environment.h`
- runtime objects on `adapterId=afw`
- const vs mapped runtime views, services, adapters

Once the partner’s model clicks, they often **intuit the next layer** (start/stop adapters, how pieces wire) without a full lecture.

### 4. Ah-ha → map (short story)

On environment / runtime catalog work, the partner pushed for **smaller, safer chunks** of environment information rather than one huge “give me everything” view. That tension was useful. The turn came from a live tour: talk to `afwfcgi`, see how const objects and runtime objects work, and see what the environment registries already expose. With that loop closed, the partner could reason about adapters and services from the same mental model.

That kind of session is why this “support partner + durable maps” path exists: if a careful partner can get there with you, **support and continuity are real work**, not only ticket churn. After an ah-ha, write a **thin map** into git so the next person does not re-earn it only in chat.

### 5. Prefer maps over ticket diaries

Durable notes that help later:

- concept or contract  
- wrong path not to take  
- live probe that proves it  
- optional user-facing sentence  

Issue numbers are **pointers**, not the title of the knowledge. Do not dump every PR status into long memory files.

In this repo, starting points for maps:

| Doc | Role |
|-----|------|
| [`knowledge-atlas.md`](knowledge-atlas.md) | Topic → rules, pads, probes, gaps |
| [`agent-support.md`](agent-support.md) | Symptom → layer → probe playbooks |
| [`mantras-and-working-style.md`](mantras-and-working-style.md) | Sticky design phrases + consensus method (deeper) |
| [`afw-philosophy-and-core-model.md`](afw-philosophy-and-core-model.md) | Why the core is shaped this way |
| [`../AGENTS.md`](../AGENTS.md) | System map and agent mission |

Code and tests remain ground truth. When a note and the tree disagree, fix the note.

### 6. Practical defaults that worked here

Use any subset; none of this is mandatory for the team.

- **Discuss → plan → execute when agreed** on multi-step hard work (especially memory / long-running lifetime).  
- **Hold commits and PRs until asked** in a pairing session, unless you agree otherwise for a stretch.  
- Keep **language tests** (`afwdev test -j`) as the correctness gate; keep **load / soak / experimental hosts** as a separate lab (blast, advanced-test) — do not redefine the gate as soak.  
- **Never hand-edit `generated/`** — metadata is the single source of truth.  
- Prefer **plain language** in chat and maintainer notes when a short phrase is enough.  
- After a deep session, **promote** one thin map; do not grow unbounded session memory.

---

## What we do not claim

- An AI partner does not replace maintainers, reviewers, or product consensus.  
- Speed is not the goal; **shared, checkable understanding** is.  
- These lessons are not a requirement to use any particular AI product.  
- Always-on agent rules (`.cursor/rules/`) stay about **how to edit the tree safely** — this page is optional human guidance.

---

## If you try it

1. Pick a real issue.  
2. Ask the partner what they think.  
3. Discuss; push back; show live AFW when stuck.  
4. Implement only when you share the next step.  
5. After a real click, add or update a short map in `designs/` (or the atlas / playbook).  

Refine this page when another lesson sticks. Skip anything that does not fit how you work.
