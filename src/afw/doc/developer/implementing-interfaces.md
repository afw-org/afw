Implementing interfaces {#afw_dev_implementing}
=======================

@brief afwdev scaffolds, impl_declares, skeletons, and registration.

## Prefer afwdev over empty files

afwdev `make-*` and `add-*` subcommands create directories and copy
skeletons with substitutions and `@todo` checklists:

| Subcommand | Typical use |
|------------|-------------|
| `make-afw-package` | New package tree + `afw-package.json` |
| `make-extension` | New loadable extension srcdir |
| `make-command` | New command srcdir |
| `add-adapter-type` | Adapter factory + adapter (+ related) skeletons |
| `add-content-type` | Content type skeletons |
| `add-log-type` | Log factory + log skeletons |
| `add-core-interface` | One core interface skeleton into an existing srcdir |

See `afwdev <subcommand> --help` and `src/afw_dev/_afwdev/cli/info.py` for
wording and examples.

## After scaffolding

1. Review **`@todo`** markers in the new files (intentional checklist).  
2. Set `AFW_IMPLEMENTATION_ID` and include the matching
   `generated/<interface>_impl_declares.h`.  
3. Implement `impl_<interface>_<method>` functions.  
4. Wire registration (manifest register snippets / generated register).  
5. `./afwdev build --cdev -j` and test.

## Placeholders you must not “clean up”

In closet and scaffolded files:

- **`<afwdev {…}>`** — filled by afwdev when copying skeletons  
- **`@todo`** — human/fill-in markers (also used as substitution targets in some templates)

These are part of the developer product, not incomplete Doxygen to rewrite.

## Closet vs hand code

| Location | Role |
|----------|------|
| `generated/interface_closet/` | Templates; regenerate freely |
| Your srcdir `.c` / `.h` | Real implementation |

Do not ship production logic only in the closet.

## Related

- @ref afw_c_api_impl  
- @ref afw_dev_extending  
- @ref afw_dev_interfaces  
