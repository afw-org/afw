Types, opaques, and multi-layout values {#afw_dev_types_opaques}
=======================================

@brief How AFW names public types vs private layouts (for Doxygen and C authors).

## Public names are `afw_*_t`

Almost every important C type is a **typedef** ending in `_t` (e.g.
`afw_adapter_t`, `afw_pool_t`, `afw_value_t`). That is the name used in
parameters, locals, and docs.

The corresponding tag is usually `struct afw_*_s`. Full definitions live in
other headers, or stay incomplete so the type stays opaque.

## Why opaques exist

Early headers such as `afw_common_opaques.h` and generated
`afw_interface_opaques.h` declare typedefs **without** pulling every struct
body. That:

- Avoids include-order / circular dependency problems  
- Lets some layouts stay private to a .c or internal header  
- Keeps the public surface small for extensions and commands  

Doxygen is configured with **`TYPEDEF_HIDES_STRUCT = YES`** so Data Structures
prefers the **`_t` name** when a simple typedef/struct pair is documented.

## One typedef, many structs (especially values)

Some public types are **not** 1:1 with a single struct body.

**`afw_value_t`** is the main example. Callers always use
`const afw_value_t *`. Internally, many different layouts exist
(`afw_value_string_s`, `*_managed_s`, `*_managed_slice_s`, block, call_*,
symbol_reference, compiled_value, …). Each kind starts with the shared
inf/`afw_value_t` face so it can be passed as a value pointer.

Behavior is selected by **`inf`** (especially evaluate), not by the caller
knowing the C layout. **Do not cast** a value pointer to a single “the”
value struct from extension code.

See @ref afw_value, `afw_value.h`, and (core only) `afw_value_internal.h`.

Interface **instances** (`afw_adapter_t`, …) are more often one published
struct in the generated interface header, but **implementations** may still
embed that as the first field of a larger self struct in .c files. Call
**macros** (`afw_<iface>_<method>`) are the method-like API.

## What to open in Doxygen

| Goal | Where |
|------|--------|
| Mental model for values | Modules → Value (`afw_value`) |
| Evaluate / create helpers | `afw_value.h` |
| Interface call macros | Modules → Interfaces / each interface group |
| Typedef list | Data Structures (names should be mostly `*_t`) |
| Kind layouts (core maintainers) | `afw_value_internal.h`, data-type bindings |

## Related

- @ref afw_dev_interfaces  
- @ref afw_dev_runtime  
- @ref afw_dev_doxygen_skin (typedef hide-struct and C-focused input)  
