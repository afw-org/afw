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

## What the generator emits (Doxygen)

Per interface, `interfaces.py` documents:

| Symbol | Notes |
|--------|--------|
| Opaque `*_t` / `*_inf_t` | Briefs in `*_opaques.h`; public API names |
| Public instance struct | Layout + “call with macros” guidance |
| Inf / vtable struct | Method table pointed to by `inf` |
| Call macros | Full `@param` / `@return` from XML; `@relates` / `@see @ref *_t` toward the instance type |

Prefer **Modules → each interface group** (and the macro detail there) over hunting vtable function pointers. Doxygen’s “Related” list on a type page may not list `#define` macros even with `@relates`; the interface group is the reliable index.

## Generated layout (core)

- `generated/afw_interface.h` — structs + **call macros** + per-interface groups  
- `generated/afw_interface_opaques.h` — public `*_t` / `*_inf_t` typedefs  
- `generated/*_impl_declares.h` — include from implementation `.c` only  
- `generated/interface_closet/` — skeletons for afwdev (templates, not product code)

## Instance storage (created vs defined)

Most interfaces are **created** instances: obtain a `const T *` from a
create/factory API; lifetime is pool or explicit release.

Rare interfaces may set on the `<interface>` element:

```xml
defined_instance_storage="true"
```

(and often `self_const="false"` when methods mutate the instance).

**Defined instance storage** means:

- The public `afw_*_t` is a **fixed complete** layout the caller provides
  (stack or embed).
- The caller does **not** invent the `inf` pointer. A **host** method on the
  source fills the defined instance (sets `inf` + cursor), similar in spirit
  to `get_setter` knowing which inf to return—but initializing storage in
  place.
- Caller uses call macros and does **not** release the instance.
- Implementations use only the published struct fields.

Example: **`afw_iterator`** (values only) and **`afw_iterator_with_key`**
(key+value), both `defined_instance_storage` (#153):

```c
afw_iterator_t it;
afw_array_initialize_iterator(array, &it, xctx);
while ((value = afw_iterator_get_next(&it, p, xctx)) != NULL) { ... }
```

`afw_iterator_with_key` is for hosts that expose keys (e.g. object
properties); array only initializes the keyless `afw_iterator` for now.

Legacy cursor type remains `afw_iterator_old_t` on array/object `get_next_*`
until migrated.

When `defined_instance_storage` is omitted, the interface uses the normal
created-instance model (no host `initialize_*` for the face itself).

## Related

- @ref afw_interface group  
- @ref afw_dev_implementing  
- Rule for assistants: `.cursor/rules/afw-interfaces-doxygen.mdc`
