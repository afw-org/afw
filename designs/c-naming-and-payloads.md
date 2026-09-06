# C naming: values vs utf8/memory payloads

**Audience:** maintainers and assistants.  
**Not user docs.** User-facing rename map: [`whats-new.md`](../whats-new.md) (UTF-8 create / set / forced_safe).  
**Code:** `src/afw/utf8/afw_utf8.h`, `src/afw/memory/afw_memory.h`, `src/afw/value/afw_value.h`, generated data-type bindings.  
**#2** still owns value `managed` / `unmanaged` / `permanent`. The value methods are `get_reference` (bump) and `get_assignable_value` (slot occupant). `clone_or_reference` is a compatibility name for `get_reference`.

## Three layers (do not share adjectives)

| Layer | What it is | Pool we can hold? |
|-------|------------|-------------------|
| **1. C payload** | `afw_utf8_t`, `afw_memory_t`, `afw_integer_t`, `afw_date_t` | **No.** Struct does not record a pool. |
| **2. Adaptive value** | `const afw_value_t *` (`inf` + private payload) | Header lives in a pool. `get_reference` / `get_assignable_value` / `release` are on the **inf**. |
| **3. Object / array instance** | `afw_object_t`, `afw_array_t` | **Yes** when it is pool-world (`create_unmanaged` / `_new_p` / `_cede_p`). Frames (`create_managed`) have **no pool**. `get_reference` / `release` hold `instance->p` where a pool exists. Const permanents have no pool; script gets a **wrapper**. |

`managed` on an **object/array frame** means RC in this `xctx->p` (no pool).  
`managed` on an **integer** means “value header has an RC in `xctx->p`.” Those are not the same idea as “owns a pool.” Do not put `managed` / `unmanaged` on utf8/memory **payloads**.

**`get_assignable_value`** is the assign/escape door (slot occupant). **`get_reference`** is a bump. `clone_or_reference` is a compatibility name for that bump.

## Payloads: who owns the struct vs the bytes

Most scalars are one chunk (`afw_integer_t`). Objects/arrays are a `const` pointer (talk via inf). **`afw_utf8_t` / `afw_memory_t`** are a little struct **plus** bytes the struct points at. Those bytes have no lifetime of their own.

| Verb | Struct | Bytes |
|------|--------|--------|
| **`create`** | New `const` in `p` | **Copy** (utf8: NFC or throw) |
| **`create_no_copy`** | New `const` in `p` | **Point** at `s`. NFC or throw. `p` is the struct only. |
| **`z_to_utf8`** | New `const` in `p` | Ingest `utf8_z` (copy + NFC) |
| **`z_as_utf8`** | New `const` in `p` | Ingest `utf8_z` (point) |
| **`set` / `z_set`** | Your non-const struct | Copy into `p` (octets vs `utf8_z`) |
| **`set_no_copy` / `z_set_no_copy`** | Your non-const struct | Point. No `p`. |
| **`clone`** | New `const` in `p` | Copy struct **and** `.s` from an existing `afw_utf8_t` |

**Rule:** shorter name does more / is safer. Extra words take a guard off (`no_copy`, like `create_unmanaged` on objects). **`p` only if something new lives there.**

- Prefix **`afw_utf8_z_`** when you **have** a C string. Prefix **`afw_utf8_`** when you have length-prefixed `utf8`.
- Mixed predicates spell both types in argument order (`starts_with_utf8_z`).
- **`from_memory` / `as_memory`**: utf8 ↔ `afw_memory_t`. No `afw_raw_t`.

**Internal** is NFC `afw_utf8_t` (`.s` + `.len`). **External** is not that world. Spell **external C string** or **external octets** when the shape matters. Cross with a named door: `to_utf8_z` / `z_create` (C string, throw if interior `0`), `forced_safe` (encode for logs/names), `as_memory` / write with `len` (octets + size). `afw_utf8_utf8_z_t` is the utf8 + z pair when the buffer is already a C string (literal / `z_create`); generated strings use `afw_s_*` / `afw_z_*`.

`AFW_UTF8_LITERAL` is a trusted C `"…"` initializer. ASCII (including `\n`) is always UTF-8 NFC. `\x` or a non-UTF-8 source file is a programmer error. AFW does not support EBCDIC.

## Invalid input: three policies

| Policy | Invalid / Cc become | NFC? | Adaptive value / property name? |
|--------|---------------------|------|----------------------------------|
| **`create` / `set`** | Throw | Yes | Yes (if you wrap it) |
| **`forced_safe`** | `^` + uppercase hex + `^` (runs); `^^` = caret | **No** | **No** |
| **`create_property_name`** | Same encode | Then NFC | **Yes** — a name |

Valid UTF-8 text passes through encode. Unicode **Cc** (`afw_code_point_is_control`) and invalid UTF-8 bytes are hex. **Whitespace/EOL** (`afw_code_point_is_whitespace_or_eol`) stays text. `forced_safe` always **copies**. `printf` / `z_printf` always run the assembled result through it — **viewable text**, not a data-file writer.

Env / FCGI names: only three `create_property_name` callers. Documented in object types + `whats-new`.

## Values (enough to name payloads)

- Face is `inf`. Evaluated data-type values have `.internal` (`afw_integer_t`, `afw_utf8_t`, `const afw_object_t *`, …). Graph kinds (`call`, `block`, `compiled_value`, …) have `contextual` and other fields — same `const afw_value_t *`.
- Evaluate always returns a value; new memory from the passed `p` or a child of `p`.
- **`create`** is the verb for “here is a new value.” Anyone in C can add an inf.
- Built-in execute: `AFW_FUNCTION_EVALUATE_*` writes a **pointer** on the stack (`const afw_value_string_t *`); work uses `arg->internal`.
- Header is immutable. Object/array `internal` points at an instance that can mutate.
- **`compiled_value`** keeps one `full_source`. Children store a **contextual** window. Backtrace is the **evaluation stack**, not `parent`. Adaptive `compile()` at eval time uses `x->p` and `parent = NULL` so a compile-once script evaluated many times does not grow the script pool ([#212](https://github.com/afw-org/afw/issues/212)).

Managed scalar headers live in `xctx->p` and last-release `free_memory`s them ([#277](https://github.com/afw-org/afw/issues/277)). Eval completion clones **evaluated** results unmanaged into dest `p`.

Do **not** rename `afw_value_create_managed_<dt>` to `afw_value_create_<dt>`. `afw_value_create_*` already means “new value” (graph nodes, `from_external`, `now_utc`). Most C uses `create_unmanaged_*`. Object/array: `create_unmanaged` / `_new_p` / `_cede_p` are pool-world; `create_managed` is a **frame** (no pool). `managed` stays extra words where a hold is real.

**Gotcha:** a walker that treats `afw_utf8_t->s` as a C string needs a trailing `0`. Old `create` could point at a `z` buffer. New `create` copies **without** a `0`. The RQL origin string uses `create_no_copy` onto `afw_utf8_z_create` for that.

**External C string:** `afw_utf8_to_utf8_z`, `afw_utf8_z_create`, and `afw_utf8_array_to_utf8_z_with_separator` throw if the length-prefixed bytes contain a `0` (pieces and separator). `afw_utf8_z_array_with_separator` checks the separator the same way. A C string cannot hold that value. Length-prefixed concat (`array_to_utf8_with_separator`) stays internal and does not throw. Do not ban `\0`/`\x00` in the lexer. `forced_safe` / `z_printf` still encode U+0000 as `^00^`. File logical paths already rejected an embedded NUL (`afw_file_path.c`).

## Code points vs UTF-8

Unicode **code-point** tests (identifier, whitespace, Cc) live in **`src/afw/code_point/`** (`afw_code_point.h`). They take an `afw_code_point_t`, not octets. UTF-8 encode/decode stays in `afw_utf8`.

ICU: `afw_utf8.c` (NFC, to_lower, `afw_utf8_icu_error_name_z`) and `afw_code_point.c` (properties). Env decoder uses that wrap.

`afw_utf8_printf` / `z_printf`: own formatter; `AFW_UTF8_FMT` (`%.*s`) copies n bytes (interior `0` is data), then `forced_safe` the assembled buffer. libc `printf` with the same specifier still stops at `0`. Do not use these to write data files or round-trip octets — `.s` + `.len` / `as_memory`.

LDAP filters, file-adapter paths (dir open, journal, object files), and VFS host-path joins: **concat `.len`**, then **`to_utf8_z`**. Do not glue those with `AFW_UTF8_FMT` / `apr_psprintf`.

## Related

- [`memory-management.md`](memory-management.md) — #2 pools / escape (do not fold this pad into it).
- [`lineage-and-library-floor.md`](lineage-and-library-floor.md) — ICU home.
- [`utf8-code-point-sequences.md`](utf8-code-point-sequences.md) — #153 Adaptive values as CP sequences.
- Doxygen: groups `afw_utf8`, `afw_memory`, `afw_code_point`, `afw_value`.
