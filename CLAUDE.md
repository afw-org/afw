# Claude Code — Adaptive Framework

Project guidance for coding agents lives in the tool-agnostic hub:

**Read [`AGENTS.md`](AGENTS.md) first** (mission, system map, build/test loop, knowledge map, optional pattern for working an issue with an AI partner).

**Before you edit C or invent a design:** open [`designs/knowledge-atlas.md`](designs/knowledge-atlas.md) and read **How the C surface fits**. Then the topic row for the area you are in. Those pads are not optional background — they hold pairing knowledge that **is not obvious from the code** (create vs evaluate, GET vs POST `/afw`, face delete tombstones, lock-safe load ≠ lifetime, stale `afwfcgi` after install, #28 decided-not, …). Symptom first? [`designs/agent-support.md`](designs/agent-support.md). Open with “what do you think?” unless you already share a plan.

This file does **not** preload `designs/` or `.cursor/rules/`. You have to open them.

## Quick pointers

| Need | Where |
|------|--------|
| Day-to-day build / test | `AGENTS.md` — `./afwdev build --cdev`, `afwdev test -j` |
| Always-on edit habits (Cursor-style rules) | `.cursor/rules/` especially `afw-project.mdc` |
| Topic → sources / probes | `designs/knowledge-atlas.md` |
| Support playbooks | `designs/agent-support.md` |
| Optional AI partnering lessons | `designs/ai-partner-lessons.md` |
| Design philosophy / mantras | `designs/afw-philosophy-and-core-model.md`, `designs/mantras-and-working-style.md` |

## Hard rules (short)

- Do **not** hand-edit `generated/` — edit `generate/` metadata (or hand C/Python outside generated trees), then regenerate via `./afwdev build --cdev`.
- Prefer **discuss before large changes**; open issues with “what do you think?” when pairing.
- Primary focus: **C runtime** (`src/afw`) and **Python afwdev** unless asked otherwise.
- Structured values: **object** + **properties**, **array** (not “bag” for objects).
- Ground truth: **code and tests**; maintainer pads under `designs/` orient only.

When this file and `AGENTS.md` disagree, prefer **`AGENTS.md`** and the tree.
