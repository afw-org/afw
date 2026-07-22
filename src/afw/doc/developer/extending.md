Extending AFW (sketch) {#afw_dev_extending}
======================

@brief End-to-end path for a new extension and adapter type.

## Sketch

From a package root (or after `afwdev make-afw-package`):

```bash
# New extension srcdir (example name)
afwdev make-extension myext

# Add adapter-type skeletons into that srcdir
afwdev add-adapter-type myadapter --srcdir myext

# Implement @todo methods, fix conf object types as needed
./afwdev build --cdev -j
afwdev test -j   # plus any extension-local tests
```

Then configure the adapter in conf, load the extension, and exercise via
`afw` / `afwfcgi` or script tests.

## What you are wiring

1. **Interface impls** — factory / adapter / session (macros for *callers*;
   `impl_*` for *you*).  
2. **Registration** — adapter_type (and anything else) into the environment
   via generated register / manifest.  
3. **Conf** — object types under `generate/objects/` so conf and UI know the
   properties.  

## Content types, logs, other interfaces

Same idea: `add-content-type`, `add-log-type`, or `add-core-interface` for a
single interface. Always start from afwdev scaffolds unless you are copying
an existing extension deliberately.

## Study an existing extension

**`src/afw_curl/`** is a good first read for a functions-focused extension
(extension initialize, function execute files, manifest error RV decoder).
Adapter-type extensions (e.g. `afw_vfs`, `afw_ldap`) add factory/session
impls from `add-adapter-type` skeletons.

## Core vs base package layout

- **`src/afw/`** — **libafw core** (pools, values, interfaces, compiler, …).  
- **Other `src/<srcdir>/`** in this repo — commands and extensions that form
  **AFW base**. Keep each non-core srcdir **self-contained** enough that it
  could live in another repository: its own `generate/`, tests, public
  header, and Doxygen group.  
- Extensions **depend on public core only** (`afw.h`, call macros) — never
  `afw_internal.h`. Core may mention base examples lightly; detailed docs
  for an extension stay **in that srcdir**.

## Related

- @ref afw_dev_implementing  
- @ref afw_included_extensions  
- Extension layout notes in `.cursor/rules/afw-extensions.mdc`  
- Public map header: `src/afw_curl/afw_curl.h`  

