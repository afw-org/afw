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
| Pragmas (author policy) | `afw_compile_parse_pragma.c` |
| Compiler-private `#…` | `afw_compile_parse_compiler_internal.c` |

## Harvest pipeline

1. Edit the EBNF comment blocks **and** the C that implements them together.  
2. File lists under `src/afw/generate/ebnf/*.txt` say which sources are scanned
   (not the grammar itself).  
3. `afwdev generate` / `./afwdev build --cdev` harvests into
   `src/afw/generated/ebnf/` (e.g. `syntax.ebnf`).  
4. Docs build (`./afwdev build --docs` / `--fulldev`) draws railroad
   diagrams in Python (`src/afw_dev/_afwdev/build/ebnf/`, no Java): one
   `diagram/<Name>.svg` per production plus the “Syntax EBNF” page
   (`index.html`) under `build/docs/afw/html/reference/language/ebnf/syntax/`.
   It applies the same rewrites as Railroad Diagram Generator (left and right
   factoring, `X ( sep X )*` as one loop, single-literal productions drawn
   inline), so diagrams keep their familiar shape.
5. Handbook pages (e.g. statements) embed `diagram/<Name>.svg` via
   `generated-src="ebnf/syntax/diagram/…"`. Each SVG carries the handbook's
   light and dark palettes and follows `prefers-color-scheme`, so these
   images get no `dark:invert`.

**Do not hand-edit** `generated/ebnf/`. The **C parser is authoritative** if
prose and code disagree.

### Comments become captions and labels

- `*#` lines **before** a production (in the same `ebnf` block) describe it:
  the caption on the Syntax EBNF page and the SVG's `<desc>`. `#62`-style
  issue numbers become links.
- A `*#` line **inside** a production labels the item after it in the
  diagram. Keep it to a word or two (`C-style`, `for-of`).
- The docs build warns about names that are used but have no production.

### Other tools

`syntax.ebnf` is plain W3C EBNF, so other tools can read it too, e.g.
https://bottlecaps.de/rr/ui for one-off diagrams.

## Related

- Doxygen group @ref afw_compile  
- Cursor rule `afw-compiler-ebnf`  
- @ref afw_dev_runtime (short pointer for everyone else)  
