# Sibling AFW package (prime kit)

**Audience:** maintainers and AI assistants. **Not** handbook.

How to work in **another Git repository** that is an AFW package (extensions and/or commands that use **libafw**) **next to** this base, without mixing the two trees. When someone says **prime** that package, write the files in *When asked to prime* into **that** repository, not into this one.

This pad is generic. It does not name other remotes.

## Layout

```text
parent/
  afw/                 ← this repository (the base)
  <other-package>/     ← the other Git repo
```

The other package’s `../afw` is this tree. Do **not** nest one repo inside the other. Do **not** open `parent/` as the Grok or Cursor workspace.

| Tool | What selects the tree |
|------|------------------------|
| Grok CLI | Directory you start `grok` in (or `--cwd`) |
| Cursor | Folder / `.code-workspace` you opened |

Two Cursor windows (one folder each) is fine. One window with both folders is one workspace that can edit both trees.

## Write wall

In a session whose root is the other package:

- **Read** `../afw` for the base floor (rules listed below).
- **Do not** create or edit anything under `../afw` unless the human names that path and asks.
- “Remember”, “save”, “promote”, “leave a map” means **that** repository and **that** Grok workspace memory.
- Do **not** load this repository’s `AGENTS.md` as session law. It tells the base partner to promote maps into **this** tree.

Grok workspace memory is keyed by git `origin`. Different remotes get different memory. Global `~/.grok/memory/MEMORY.md` is shared — do not put package internals there.

## Sandbox (so the wall is not only a request)

Grok’s built-in profile **named** `workspace` means: read anywhere, write only the current directory + `~/.grok/` + temp. It is not Cursor’s open folder and not the path `/workspaces`.

```bash
cd /path/to/<other-package>
grok --sandbox workspace
```

Same thing without the flag if `GROK_SANDBOX=workspace` is already in the environment **before** `grok` starts (direnv in that directory is the usual way).

Project `.grok/config.toml` cannot set `[sandbox] profile`. That key is only in `~/.grok/config.toml` and would apply to **every** project, including this base. Do not set it globally unless that is intended.

If the sandbox cannot apply, Grok may warn and continue unsandboxed. Check the startup line.

## Base floor (read, do not copy)

If `../afw` exists, open these when the work is extension or command shaped:

| File | When |
|------|------|
| `../afw/.cursor/rules/afw-extensions.mdc` | Loadable DSO, manifest, public APIs |
| `../afw/.cursor/rules/afw-command.mdc` | Only if that package has a command |
| `../afw/.cursor/rules/afw-interfaces-doxygen.mdc` | Call macros, scaffolds |
| `../afw/.cursor/rules/afw-environment.mdc` | Registries; peers after register |
| `../afw/.cursor/rules/afw-generate-metadata.mdc` | Edit `generate/`, not `generated/` |
| `../afw/.cursor/rules/afw-c-runtime.mdc` | C habits |
| `../afw/.cursor/rules/afw-tests.mdc` | Tests |
| `../afw/.cursor/rules/afw-headers.mdc` | `#include "afw.h"`, never `afw_internal.h` |
| `../afw/designs/lineage-and-library-floor.md` | Base vs other packages |

Do **not** copy this repository’s `designs/` novels, compiler/evaluate pads, or this `AGENTS.md` in full. Those are base development.

Hard rules that still apply in the other package: never hand-edit `generated/`; public core APIs only; keep each `src/<srcdir>/` as self-contained as practical; **object** + **properties**, **array** (do not call objects “bags”).

How the other tree **builds** against sibling libafw is that package’s problem. Do not invent a cmake story here.

## When asked to prime

Only when the human asks, and only in the **other** repository (they name the path or you are already in that session). Write three files if they are missing. Do not commit unless asked.

Fill **This package** from that tree (`afw-package.json` or whatever manifest it still uses, `src/` names). Keep it facts. Leave build-against-sibling empty until they ask to work that.

### `.envrc`

```bash
# Apply with direnv (`direnv allow`). Sets the Grok sandbox before `grok` starts.
# Built-in profile named workspace: read anywhere, write only this directory + ~/.grok/ + temp.
export GROK_SANDBOX=workspace
```

### `.cursor/rules/afw-package.mdc`

```markdown
---
description: This repo is an AFW package beside public afw — read sibling, do not write it
alwaysApply: true
---

# Out-of-tree AFW package

- Sibling base is `../afw` when present. **Read** it. **Do not edit** it unless the human names a path under `../afw` and asks.
- Save / remember / promote stays **in this repository**.
- Do not load `../afw/AGENTS.md` as session law.
- Never hand-edit `generated/`. Public `afw.h` / interface macros only.
- Sandbox should be the built-in profile `workspace` (`GROK_SANDBOX` or `--sandbox workspace`) so writes cannot leave this directory.
```

### `AGENTS.md`

Use the following as the body. Replace `<this-package>` in the layout comment if you want; the **This package** section must be filled from the tree you are priming.

```markdown
# AGENTS.md — AFW package (not the base)

This repository is an **AFW package**: extensions and/or commands that use **libafw**. It is not public `afw` (the base).

## Layout

Check this repo out **next to** a clone of public `afw`:

    parent/
      afw/                 ← base (read for APIs, rules, generate floor)
      <this-package>/      ← you are here

Start Grok **in this directory** (`grok`, or `grok --cwd` here). Do not start
it from `parent/` or from `afw/`.

Cursor: open **only this folder**. Do not add `afw` to the same window.

## Write wall

- You may **read** `../afw`.
- Do **not** create or edit anything under `../afw` unless the human names
  that path and says to edit it.
- “Remember”, “save”, “promote”, “leave a map” means **this repository**
  and **this** Grok workspace memory. Never `../afw/designs/`,
  `../afw/.cursor/`, or `../afw/AGENTS.md`.
- Do **not** treat `../afw/AGENTS.md` as law for this session. That file
  tells the base partner to promote maps into the public tree.

## Base floor (read these if `../afw` exists)

Open them when the work is extension or command shaped. Do not copy them here.

- `../afw/.cursor/rules/afw-extensions.mdc`
- `../afw/.cursor/rules/afw-command.mdc` — only if this package has a command
- `../afw/.cursor/rules/afw-interfaces-doxygen.mdc`
- `../afw/.cursor/rules/afw-environment.mdc`
- `../afw/.cursor/rules/afw-generate-metadata.mdc`
- `../afw/.cursor/rules/afw-c-runtime.mdc`
- `../afw/.cursor/rules/afw-tests.mdc`
- `../afw/.cursor/rules/afw-headers.mdc`
- `../afw/designs/lineage-and-library-floor.md`

If `../afw` is missing, say so and work from this file plus the in-tree
`generate/` and existing srcdirs. Do not invent a core API.

## Hard rules (same as any AFW srcdir)

- Never hand-edit `generated/`.
- Public core APIs only (`afw.h`, interface call macros). No `afw_internal.h`.
- Keep each `src/<srcdir>/` as self-contained as practical.
- Generate → this package’s documented build → tests. Do not assume
  today’s base `./afwdev build --cdev` until that package’s build against
  sibling libafw is known.
- Structured values: **object** + **properties**, **array**. Do not call
  objects “bags”.

## This package

_(Fill in from this tree: srcdirs, manifest, how it finds sibling libafw
if already known. Leave the include/build story empty until that work
is asked for.)_
```

After priming, the human starts a **new** Grok in the other directory (`grok --sandbox workspace` unless direnv already exported `GROK_SANDBOX`). That session does not inherit the base chat.

## Related

- Layers and repo roles: [`lineage-and-library-floor.md`](lineage-and-library-floor.md)
- In-tree extension DSOs: [`.cursor/rules/afw-extensions.mdc`](../.cursor/rules/afw-extensions.mdc)
- Handbook packages page: `src/afw/doc/guide/developer/packages.xml` (`make-afw-package`, `make-extension`, `make-command`)
