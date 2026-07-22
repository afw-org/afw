Compiler EBNF harvest {#afw_dev_compiler_ebnf}
=====================

@brief For people changing Adaptive Script grammar — not for most extension authors.

## Who this is for

**Compiler / language maintainers** working under `src/afw/compile/`.

Most extension and command authors only **call** compile/evaluate APIs.
They do not need this pipeline.

## Where the grammar lives

Grammar fragments sit in special comments **next to the real parser/lexer**,
using open/close markers of the form: `ebnf` followed by triple greater-than
to open, and triple less-than then `ebnf` to close (written that way so this
markdown and C comments stay valid).

Typical files:

| Area | Primary sources |
|------|-----------------|
| Tokens / residual | `afw_compile_lexical.c` |
| Script / statements | `afw_compile_parse_script.c` |
| Expressions | `afw_compile_parse_expression.c` |
| Values / JSON-ish | `afw_compile_parse_value.c` |
| Templates | `afw_compile_parse_template.c` |

## Harvest pipeline

1. Edit the EBNF comment blocks **and** the C that implements them together.  
2. File lists under `src/afw/generate/ebnf/*.txt` say which sources are scanned
   (not the grammar itself).  
3. `afwdev generate` / `./afwdev build --cdev` harvests into
   `src/afw/generated/ebnf/` (e.g. `syntax.ebnf`).  
4. Docs build (`./afwdev build --docs` / `--fulldev`) runs **rr.war**
   (Railroad Diagram Generator; same engine as https://bottlecaps.de/rr)
   with `-color:#DCDCDC`, extracts under
   `build/docs/afw/html/reference/language/ebnf/syntax/`, and **themes**
   `index.html` so “Syntax EBNF” matches handbook slate light/dark chrome.
5. Handbook pages (e.g. statements) embed `diagram/*.png` via
   `generated-src="ebnf/syntax/diagram/…"`. Those `<img>` tags use Tailwind
   **`dark:invert`** (see `_resources/html_templates/image.html`) so gray
   PNGs invert on dark docs pages. The full Syntax EBNF `index.html` applies
   the same invert via CSS (`filter: invert(1)` under `prefers-color-scheme:
   dark`).

**Do not hand-edit** `generated/ebnf/`. The **C parser is authoritative** if
prose and code disagree.

### Optional: bottlecaps web UI

You can still paste `syntax.ebnf` into https://bottlecaps.de/rr/ui for one-off
colors/SVG. That is for exploration; the published site uses the automated
`rr.war` path above. Steps are summarized in the harvest header inside
`syntax.ebnf` (from `afw_compile_parse.c`).

## Related

- Doxygen group @ref afw_compile  
- Cursor rule `afw-compiler-ebnf`  
- @ref afw_dev_runtime (short pointer for everyone else)  
