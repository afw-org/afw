Interfaces and call macros {#afw_dev_interfaces}
==========================

@brief XML IDL, generated macros, and where documentation lives.

## Why macros

AFW is **C** for multi-request servers (controlled memory, no C++ runtime tax).
Interfaces still need a clean call surface. Core defines contracts in
`generate/interfaces/afw_interface.xml`. afwdev generates:

- Public structs and `inf` vtables  
- **Call macros** `afw_<interface>_<method>(…)`  
- `*_impl_declares.h` for implementers  
- Closet **skeletons** for afwdev scaffolding  

**Developers should call the macros**, not write
`(instance)->inf->method(...)` by hand. The arrow form remains for the
compiler and GDB.

Example:

```c
session = afw_adapter_create_adapter_session(adapter, xctx);
```

## Source of truth for API docs

| Content | Edit here |
|---------|-----------|
| Method / parameter / return text | Interface **XML** descriptions |
| Macro Doxygen emission | `src/afw_dev/_afwdev/generate/interfaces.py` |
| Group mental models | `src/afw/include/afw_doxygen.h` |

**Do not hand-edit** `src/afw/generated/afw_interface.h` for documentation.
Regenerate with `./afwdev build --cdev -j` (or generate as part of build).

## Generated layout (core)

- `generated/afw_interface.h` — structs + **call macros** + per-interface groups  
- `generated/*_impl_declares.h` — include from implementation `.c` only  
- `generated/interface_closet/` — skeletons for afwdev (templates, not product code)

## Related

- @ref afw_interface group  
- @ref afw_dev_implementing  
- Rule for assistants: `.cursor/rules/afw-interfaces-doxygen.mdc`
