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
3. `afwdev generate` / `./afwdev build --cdev -j` harvests into
   `src/afw/generated/ebnf/` (e.g. `syntax.ebnf`).  
4. Handbook syntax diagrams are maintained from that harvest.

**Do not hand-edit** `generated/ebnf/`. The **C parser is authoritative** if
prose and code disagree.

## Related

- Doxygen group @ref afw_compile  
- Cursor rule `afw-compiler-ebnf`  
- @ref afw_dev_runtime (short pointer for everyone else)  
