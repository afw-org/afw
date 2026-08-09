# Claude Code — Adaptive Framework

Project guidance for coding agents lives in the tool-agnostic hub:

**Read [`AGENTS.md`](AGENTS.md) first** (mission, system map, build/test loop, knowledge map, optional pattern for working an issue with an AI partner).

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
