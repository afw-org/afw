# VS Code extension for Adaptive Script — proposal (no code yet)

**Status:** design-only pad, prompted by an open "what do you think?" — not a committed plan. No issue filed yet; if a maintainer wants to move on any track below, open it as a normal GitHub issue with a "what do you think?" framing rather than treating this pad as authority on its own.

## Motivation

Native `.as` (Adaptive Script) support in VS Code: syntax highlighting as the immediate ask, with richer IntelliSense, `afwdev` integration, and eventually debugging as further-out stretch goals. This pad grounds each of those asks in what already exists in the repo so the real gaps — and the real precedents — are clear before anyone estimates effort.

## Current state

- **Monaco grammar** (`src/afw_components/react/monaco/src/language/AdaptiveScript.js`) is a thorough Monarch tokenizer + language-configuration, reverse-engineered from the real C lexer/parser: full keyword/reserved-word list, a separate `typeKeywords` set for data types, numeric-literal states (hex/octal/binary/float), and string/backtick-template interpolation states for both `${...}` (eval-time) and `#{...}` (compile-time) substitution. It lives in a React/DOM-free npm package (`@afw/react-monaco`) — the grammar *logic* is portable, but **Monarch tokenizers are a Monaco-in-browser concept**. VS Code extensions render syntax via TextMate grammars (`contributes.grammars`, run through oniguruma in a different host process), so reusing this means **porting** the keyword lists/states into a `.tmLanguage.json`, not dropping the file in as-is. The `language-configuration.json` half (brackets, comments, auto-closing pairs, folding markers) *does* translate almost 1:1.
- **IntelliSense today** (completion, hover, signature help, wired up in `MonacoProvider.js` + `AdaptiveScript.js`) is driven entirely by **live REST introspection** against a running `afw` instance — `useFunctions`/`useAdapters`/`useObjectTypes`/`useDataTypes` hooks in `@afw/react` pull from `useEnvironmentRegistry()`. There is no static/offline data source wired up for `.as` source anywhere today.
- **Diagnostics today** come from server round-trips only: the Fiddle tool (`src/afw_app/admin/src/Tools/Fiddle/Fiddle.js`) POSTs an `eval<script>`/compile action to `/afw` via `@afw/client`'s `AfwClient`, and renders the returned `{offset, message}` as an editor decoration (`CodeEditor.js`). No client-side linting exists.
- **No prior VS Code extension, LSP, or DAP work** exists anywhere in the repo (confirmed by a broad grep for "language server", "vscode", "debug adapter", "monarch").
- **`afwdev` CLI** dispatches subcommands through a registry (`src/afw_dev/_afwdev/cli/`). `afwdev test` already has `--output-format json`/`json-compact`, built explicitly for tooling/CI/agent use. `build`, `generate`, and `validate` are plain-text/log output only today — no structured output to parse.
- **`.vscode/tasks.json` and `launch.json` already exist** in-repo: basic `afwdev build`/`test`/`generate` tasks, and debug configs for `afw`, `afwfcgi`, Chrome (admin app), and generic Python. A real but partial baseline — no problem-matcher tied to afwdev's JSON output, and no extension scaffold of any kind.
- **`.vscode/settings.json` already does offline metadata-driven IntelliSense today** — just scoped to authoring metadata, not `.as` source. Its `json.schemas` block maps every `src/*/generate*/objects/_Adaptive<Type>_/*.json` glob (function generate, object type, data type, adapter, model, manifest, ~70 entries) to a JSON Schema under `generated/schemas/afw/*.json`, giving VS Code's built-in JSON language server real completion/validation while someone hand-writes a function/type/adapter definition. See `.cursor/rules/afw-json-schema.mdc` for the schema-generation pipeline. This is a stronger precedent than a from-scratch design would suggest: a future offline base for **`.as` source** IntelliSense should look at extending or reusing this same generated-schema output (or the registry data it's projected from), not building new metadata-loading tooling from zero.
- **Runtime debugging primitives are effectively nonexistent** for interactive use. Two things exist, both diagnostic/one-way: (a) the xctx evaluation stack (`afw_xctx_evaluation_stack_entry_s`, `src/afw/xctx/afw_xctx.h`) that powers backtraces on error, and (b) a **dormant** `AFW_XCTX_DEBUG_EVALUATION_PRINT` macro gated by a build flag with zero call sites in `src/afw/**/*.c` — not wired into the interpreter. What *does* exist and matters for the future: the compiler retains precise per-node **source offset → line/column** (`afw_compile_source_location_of_value`, `src/afw/compile/afw_compile.c`; `value_offset`/`value_size` in `afw_compile_internal_value_contextual_s`), used currently only for error messages.

## Proposed architecture — three largely independent tracks

These are separable phases a maintainer could greenlight independently, not one monolithic build.

### A. Syntax highlighting & editing ergonomics

Port the Monarch keyword/state logic to a TextMate grammar; port `language-configuration.json` near-verbatim; register the `.as` file association. No live-server dependency. Smallest, most immediately useful slice, and the one with essentially no open design questions.

### B. Language intelligence (hybrid: offline base + live enhancement)

A small out-of-process language server (LSP) rather than ad hoc logic in the extension host itself:

- **Offline base** — extend/reuse the same `generated/schemas/afw/*.json` output (or the registry data it's projected from) that `.vscode/settings.json` already leans on for metadata authoring, this time to drive completions/hover for `.as` *source* (function signatures, data types, object types) with zero live dependency.
- **Live enhancement** — when a target `afw`/`afwfcgi` URL is configured (mirroring Fiddle's `/afw` + `AfwClient` pattern), reuse the same `eval<script>`/compile actions for real compile diagnostics, and to pick up what's *actually* loaded in that instance (custom/extension-registered functions the static metadata can't know about).

### C. `afwdev` integration

Expand past the existing `tasks.json`: command-palette wrappers for `build`/`test`/`generate`/`validate`; a VS Code **Test Explorer** integration built on `afwdev test --output-format json` (already designed for tooling — best-fit win here); CodeLens ("Run this script/test") over `.as` files. Open, separate small ask: `validate` and `build` would need a `--output-format json` option (mirroring `test`) before their errors could become real inline Diagnostics instead of scraped stderr.

## Debugging — design sketch only, explicitly deferred from implementation

The clearest way to scope this is to look at **how TypeScript debugging actually works**, since it makes precise what AFW is missing.

`tsc` itself has **zero** runtime debugging capability — it is purely a batch compiler (`.ts` in, `.js` + source maps out, then it exits). All real breakpoint/pause/step/stack-inspection machinery lives in **V8** (Node's/Chrome's JS engine): it's built into the interpreter itself and exposed over the **Chrome DevTools Protocol** (CDP, a websocket JSON-RPC-ish protocol opened via `--inspect`). VS Code's Debug Adapter Protocol (DAP) is a thin translation layer on top: the built-in `js-debug` extension speaks DAP to VS Code on one side and CDP to the running V8 process on the other, converting `setBreakpoints`/`stepIn`/`continue`/`stackTrace` requests into V8 calls. Source maps are what let V8 — which only ever knows JS line/column — get breakpoints placed at the right JS location and get its paused-frame locations translated back to `.ts` source for display. **The compiler contributes only source maps; the runtime contributes everything else.**

Mapped onto AFW, this sharpens exactly what's missing:

- **The V8-equivalent is absent.** `libafw`'s evaluator has no native pause/step/breakpoint/inspect machinery, and nothing like a CDP-style protocol exposing it. This is the expensive, load-bearing piece, and it is **C runtime work**, not editor tooling:
  1. A breakpoint registry keyed by source location, checked at a natural per-statement granularity in the compiled_value graph.
  2. A pause/resume mechanism on the evaluating xctx (block the evaluating thread at a breakpoint; resume on an external command) — evaluation is synchronous today, so this is new.
  3. A native control protocol a debug adapter can talk to (AFW's analog of CDP) — possibly extending `--local`'s existing chunked stdin/fd request-response framing, though DAP/CDP's async, event-driven shape (the runtime must proactively notify "hit breakpoint," not just answer requests) likely needs something new.
  4. Live state inspection while paused (qualifier stack / in-scope variable bindings) — related in spirit to the *static* brace-depth local-variable scanner Monaco already does (`AdaptiveScript.js:495-689`), but would need to read real `xctx` bindings, not infer them from source text.
- **The source-map-equivalent is already solved.** `afw_compile_source_location_of_value` gives exact line/column for any compiled node today — AFW's ready-made "source map," and arguably simpler than TS/JS's case since there's no separate emitted language: the compiled_value graph *is* the execution representation, closer to how V8 bytecode relates to JS source.
- **The DAP adapter itself would be the thin, easy part** — comparable effort to `js-debug`'s translation layer — *once* the native protocol above exists. It is not the bottleneck.

**Conclusion:** this is fundamentally a **core-runtime feature** (build AFW's own "V8 debugging half" first), not editor tooling. Raise it as its own future initiative for maintainer consensus (per `AGENTS.md`'s "consensus first" for structural evaluation changes) — do not bundle it into an initial extension ship.

## Open questions (unresolved, for maintainer discussion)

- **Repo placement.** New self-contained `src/<srcdir>` in this repo (matches the extensions/commands pattern in `AGENTS.md`) vs. a separate sibling repo (per [`sibling-afw-package.md`](sibling-afw-package.md), since VS Code extensions are normally released independently to the Marketplace on their own cadence). No lean yet — present the tradeoff, don't decide it here.
- **Language server implementation language** — Node/TypeScript (matches `@afw/client`/`@afw/react-monaco` and the VS Code extension host itself) vs. Python (sits next to `afwdev`). Worth a lean when track B is scoped, not a decision now.
- **`afwdev validate`/`build` JSON output** — small, separate CLI enhancement, only needed if/when track C's inline-diagnostics goal is pursued.
