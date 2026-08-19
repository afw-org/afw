# C naming: values vs utf8/memory payloads

**Audience:** maintainers and assistants.  
**Not user docs.** User-facing rename map: [`whats-new.md`](../whats-new.md) (UTF-8 create / set / forced_safe).  
**Code:** `src/afw/utf8/afw_utf8.h`, `src/afw/memory/afw_memory.h`, `src/afw/value/afw_value.h`, generated data-type bindings.  
**#2** still owns value `managed` / `unmanaged` / `permanent` and `clone_or_reference` → someday `get_reference`.

## Three layers (do not share adjectives)

| Layer | What it is | Pool we can hold? |
|-------|------------|-------------------|
| **1. C payload** | `afw_utf8_t`, `afw_memory_t`, `afw_integer_t`, `afw_date_t` | **No.** Struct does not record a pool. |
| **2. Adaptive value** | `const afw_value_t *` (`inf` + private payload) | Header lives in a pool. `optional_release` / `clone_or_reference` are on the **inf**. |
| **3. Object / array instance** | `afw_object_t`, `afw_array_t` | **Yes** (or it will). `get_reference` / `release` hold `instance->p`. Const permanents have no pool; script gets a **wrapper**. |

`managed` on an **object** means the instance has a pool you can hold.  
`managed` on an **integer** today means “value header has an RC in `xctx->p`.” Those are not the same idea. Do not put `managed` / `unmanaged` on utf8/memory **payloads**.

**`clone_or_reference`** is the assign/escape door. The caller wants a value they can **keep**. The inf decides clone vs hold vs wrap. The name we actually want on the value interface is **`get_reference`**.

## Payloads: who owns the struct vs the bytes

Most scalars are one chunk (`afw_integer_t`). Objects/arrays are a `const` pointer (talk via inf). **`afw_utf8_t` / `afw_memory_t`** are a little struct **plus** bytes the struct points at. Those bytes have no lifetime of their own.

| Verb | Struct | Bytes |
|------|--------|--------|
| **`create` / `create_z`** | New `const` in `p` | **Copy** (utf8: NFC or throw) |
| **`create_no_copy` / `_z`** | New `const` in `p` | **Point** at `s`. NFC or throw. `p` is the struct only. |
| **`set` / `set_z`** | Your non-const struct | Copy into `p` |
| **`set_no_copy` / `_z`** | Your non-const struct | Point. No `p`. |
| **`clone`** | New `const` in `p` | Copy struct **and** `.s` from an existing `afw_utf8_t` |

**Rule:** shorter name does more / is safer. Extra words take a guard off (`no_copy`, like `create_unmanaged` on objects). **`p` only if something new lives there.**

- Suffix **`_z`** = that **argument** is `0`-terminated.
- Prefix **`z_`** = the **result** is `utf8_z`.
- **`from_memory` / `as_memory`**: utf8 ↔ `afw_memory_t`. No `afw_raw_t`.

`AFW_UTF8_LITERAL` is a trusted C `"…"` initializer. ASCII (including `\n`) is always UTF-8 NFC. `\x` or a non-UTF-8 source file is a programmer error. AFW does not support EBCDIC.

## Invalid input: three policies

| Policy | Invalid / Cc become | NFC? | Adaptive value / property name? |
|--------|---------------------|------|----------------------------------|
| **`create` / `set`** | Throw | Yes | Yes (if you wrap it) |
| **`forced_safe`** | `^` + uppercase hex + `^` (runs); `^^` = caret | **No** | **No** |
| **`create_property_name`** | Same encode | Then NFC | **Yes** — a name |

Valid UTF-8 text passes through encode. Unicode **Cc** (`afw_code_point_is_control`) and invalid UTF-8 bytes are hex. **Whitespace/EOL** (`afw_code_point_is_whitespace_or_eol`) stays text. `forced_safe` always **copies**. `printf` / `z_printf` use it.

Env / FCGI names: only three `create_property_name` callers. Documented in object types + `whats-new`.

## Values (enough to name payloads)

- Face is `inf`. Evaluated data-type values have `.internal` (`afw_integer_t`, `afw_utf8_t`, `const afw_object_t *`, …). Graph kinds (`call`, `block`, `compiled_value`, …) have `contextual` and other fields — same `const afw_value_t *`.
- Evaluate always returns a value; new memory from the passed `p` or a child of `p`.
- **`create`** is the verb for “here is a new value.” Anyone in C can add an inf.
- Built-in execute: `AFW_FUNCTION_EVALUATE_*` writes a **pointer** on the stack (`const afw_value_string_t *`); work uses `arg->internal`.
- Header is immutable. Object/array `internal` points at an instance that can mutate.
- **`compiled_value`** keeps one `full_source`. Children store a **contextual** window. Backtrace is the **evaluation stack**, not `parent`. Adaptive `compile()` at eval time uses `x->p` and `parent = NULL` so a compile-once script evaluated many times does not grow the script pool ([#212](https://github.com/afw-org/afw/issues/212)).

Managed scalar headers in `xctx->p` only work if we actually `free` (including `AFW_TRY`). That will not last #2 as the *home* for those headers; assign should clone into the destination pool.

## Code points vs UTF-8

Unicode **code-point** tests (identifier, whitespace, Cc) live in **`src/afw/code_point/`** (`afw_code_point.h`). They take an `afw_code_point_t`, not octets. UTF-8 encode/decode stays in `afw_utf8`.

ICU: `afw_utf8.c` (NFC, to_lower) and `afw_code_point.c` (properties). `u_errorName` still in env register.

## Related

- [`memory-management.md`](memory-management.md) — #2 pools / escape (do not fold this pad into it).
- [`lineage-and-library-floor.md`](lineage-and-library-floor.md) — ICU home.
- [`utf8-code-point-sequences.md`](utf8-code-point-sequences.md) — #153 Adaptive values as CP sequences.
- Doxygen: groups `afw_utf8`, `afw_memory`, `afw_code_point`, `afw_value`.
