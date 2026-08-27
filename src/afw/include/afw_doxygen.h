// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Main Doxygen groups
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_DOXYGEN_H__
#define __AFW_DOXYGEN_H__

/**
 * @file afw_doxygen.h
 * @brief Top-level Doxygen @defgroup definitions for the AFW C API.
 */

/* These are major doxygen defgroup for Adaptive Framework base package. */

/** @defgroup package_afw Adaptive Framework Base Package
 *
 * The Adaptive Framework base package: libafw (the C runtime) plus the
 * included commands, extensions, and supporting infrastructure.
 *
 * Most of the interesting public and internal APIs live under the
 * afw_c_api_public and afw_c_api_internal subgroups.
 *
 * @{
 */

/**
 * @defgroup afw_c_api_public C API (public)
 *
 * **Supported** C API for extensions, commands, and applications on libafw.
 *
 * ## What to include
 *
 * - **`.c` files:** `#include "afw.h"` (convenient umbrella: call API +
 *   interface impl helpers).
 * - **Module headers:** `#include "afw_interface.h"` or `afw_minimal.h`
 *   (header bootstrap — not a second, thinner product API).
 * - **Do not** use `afw_internal.h` or `*_internal.h` outside `src/afw/`.
 *
 * ## Public vs impl vs internal (this tree)
 *
 * | Layer | Doxygen | Audience |
 * |-------|---------|----------|
 * | **Public call** | Groups under this section | Everyday call macros, pools, values, env, … |
 * | **Impl support** | @ref afw_c_api_impl | Authors *implementing* interfaces (`*_impl.h`) |
 * | **Internal** | @ref afw_c_api_internal | **libafw only** — may change any build |
 *
 * Members tagged `@internal` are omitted when Doxygen
 * `INTERNAL_DOCS = NO` (default). Internal *groups* remain listed with
 * **libafw only** titles so core maintainers can navigate; treat them as
 * unsupported for out-of-tree code. See
 * `designs/libafw-headers-and-api-surface.md`.
 *
 * @{
 */

/**
 * @defgroup afw_pool Pool
 *
 * Adaptive Framework hierarchical memory pools.
 *
 * General pools (afw_pool_create*) are destroy-is-lifetime; parent
 * decides multithreaded vs thread-specific. Heap / heap tracker are
 * single-thread only: create, use, and release on the same thread
 * (normally one compiled_value evaluate). Trackers are scope->p and
 * return memory to the heap. afw_pool_get_apr_pool() is a door for
 * leftover APR function calls, not the heap's store.
 *
 * Key functions: afw_pool_create(), afw_pool_create_as_managed_p(),
 * afw_pool_create_xctx_p(), afw_pool_heap_create(),
 * afw_pool_heap_tracker_create(), afw_pool_calloc(), afw_pool_malloc(),
 * afw_pool_free_memory(), afw_xctx_malloc() / afw_xctx_free(),
 * afw_pool_get_apr_pool(). `afw_memory_malloc` / `calloc` / `free`
 * (`p, xctx` last) live in `afw_memory.h`.
 *
 * @{
 */

/**
 * @defgroup afw_pool_internal Pool internal
 * @ingroup afw_c_api_internal
 *
 * Pool implementation details for libafw only.
 */

/** @} */

/**
 * @defgroup afw_value Value
 *
 * Adaptive Framework value graph (the core data model).
 *
 * ## Public name vs many layouts
 *
 * In API and script, everything is a **`const afw_value_t *`**. That typedef
 * is the public handle. **Do not assume one C struct body** behind it.
 *
 * Concrete values are different `struct afw_value_*_s` layouts that all start
 * with (or embed) the public `inf` / `afw_value_t` face so code can pass them
 * as `afw_value_t *`. Examples:
 * - Data-type values: `afw_value_string_s`, `*_managed_s`, `*_managed_slice_s`, …
 * - Compiler/runtime kinds: block, call_*, symbol_reference, closure_binding,
 *   compiled_value, list/object expression, …
 * - `afw_value_common_s` — layout used to access the shared prefix + internal
 *
 * This is intentional **implementation hiding**: extension authors use
 * evaluate/create APIs and macros, not casts to a single private struct.
 * Doxygen may still list several `afw_value_*` compounds under Data
 * Structures; treat **`afw_value_t`** as the only type name for the public API.
 *
 * ## How values work
 *
 * - Generally immutable once created.
 * - Behavior is selected by **`inf`** (vtable), especially
 *   `optional_evaluate`.
 * - Evaluation is usually `afw_value_evaluate()` (macro) into a pool/scope.
 *
 * Key lifetime policies (see also afw-value-memory rule):
 * - **Permanent** — live for the environment (often const in the .so).
 * - **Managed** — refcount/clone when escaping.
 * - **Managed slice** — utf8/memory view into a managed container.
 * - Prefer evaluation results in current `scope->p` for long-running scripts.
 *
 * Start with `afw_value.h`. Kind layouts for core maintainers:
 * `afw_value_internal.h` and generated data-type bindings.
 * Payloads vs values: `designs/c-naming-and-payloads.md`.
 *
 * @{
 */

/**
 * @defgroup afw_value_internal Value internal
 * @ingroup afw_c_api_internal
 *
 * Concrete value-kind structs and helpers used inside libafw.
 * Not a single layout for `afw_value_t` — see @ref afw_value.
 */

/** @} */

/**
 * @defgroup afw_environment Environment
 *
 * The central registry fabric of Adaptive Framework.
 *
 * One `afw_environment_t` per process (accessed via `xctx->env`).
 * Nearly every capability (functions, data types, adapter types, content types,
 * request handlers, etc.) is registered into keyed registries.
 *
 * Core populates most of them at environment creation time.
 * Extensions and hosts add more, often lazily via manifests.
 * After registration, everything is a peer.
 *
 * Bootstrap order is critical (see afw_environment_register_core.c):
 * 1. afw_generated_register (data types, functions, etc.)
 * 2. afw_function_internal_prepare_environment (for operators)
 * 3. Then conf types, adapter types, etc.
 *
 * @{
 */

/**
 * @defgroup afw_environment_internal Environment internal
 * @ingroup afw_c_api_internal
 *
 * Environment registry and bootstrap internals for libafw only.
 */

/** @} */

/**
 * @defgroup afw_function Adaptive Functions
 *
 * Built-in Adaptive functions (callable from script and from C via the
 * function value / execute path).
 *
 * Metadata lives under `generate/objects/_AdaptiveFunctionGenerate_/`.
 * Hand `execute_*` implementations are in `src/afw/function/` (not
 * `generated/function_closet/`). The compiler lowers operators and calls
 * onto these bindings after functions are registered.
 *
 * Polymorphic functions use a NULL execute and resolve at runtime from the
 * argument data type. Operator functions need
 * `afw_function_internal_prepare_environment` during bootstrap (see
 * environment registration order).
 *
 * @{
 */

/** @} */

/**
 * @defgroup afw_object Objects
 *
 * Adaptive objects and the afw_object interface. Core support for property
 * metadata, paths, types, views, etc.
 *
 * Prefer afw_object_create* over raw structs. Meta drives validation and UI.
 *
 * Objects can be const (permanent) or mutable via sessions.
 *
 * Related hand helpers (same conceptual area; some use nested groups):
 * object path, meta, options, type, view.
 *
 * @{
 */

/**
 * @defgroup afw_object_meta Object meta
 * @ingroup afw_object
 *
 * Object meta accessors and helpers (objectId, objectType, parentPaths, …).
 */
/**
 * @defgroup afw_object_path Object path
 * @ingroup afw_object
 *
 * Adaptive object path parse/format helpers.
 */
/**
 * @defgroup afw_object_options Object options
 * @ingroup afw_object
 *
 * View/retrieve option bits and helpers.
 */
/**
 * @defgroup afw_object_type Object type
 * @ingroup afw_object
 *
 * Object type definitions and property type helpers.
 */
/**
 * @defgroup afw_object_view Object view
 * @ingroup afw_object
 *
 * Object views over other objects (options, hybrids).
 */
/**
 * @defgroup afw_object_impl Object impl helpers
 * @ingroup afw_object
 *
 * Shared helpers for implementing `afw_object` (and related) interfaces.
 */
/**
 * @defgroup afw_object_internal Object internal
 * @ingroup afw_c_api_internal
 *
 * Object internals for libafw only.
 */
/**
 * @defgroup afw_object_view_internal Object view internal
 * @ingroup afw_c_api_internal
 *
 * Object view internals for libafw only.
 */

/** @} */

/**
 * @defgroup afw_adapter Adapters
 *
 * Adapter interface and common support. Pluggable object stores.
 * Core + extensions register adapter_type factories.
 *
 * Sessions are per-request; transactions optional. Journal support in some.
 *
 * Call adapter methods via macros in generated `afw_interface.h`
 * (e.g. `afw_adapter_session_retrieve_objects`). Hand helpers for get/add/
 * retrieve/journal live in `afw_adapter_*.h` (included from `afw_adapter.h`).
 *
 * @{
 */

/**
 * @defgroup afw_adapter_impl Adapter impl helpers
 * @ingroup afw_adapter
 *
 * Shared helpers for adapter implementations (create_cede_p, requests, …).
 */
/**
 * @defgroup afw_adapter_index_impl Adapter index impl
 * @ingroup afw_adapter
 *
 * Index definition helpers used by adapters that support indexes (e.g. LMDB).
 */
/**
 * @defgroup afw_adapter_internal Adapter internal
 * @ingroup afw_c_api_internal
 *
 * Adapter internals for libafw only.
 */

/** @} */

/**
 * @defgroup afw_file File Adapter
 *
 * Core adapter type that stores adaptive objects as files/directories on
 * the local filesystem (including optional journal support).
 *
 * Use as a simple object store or as a backend under a model adapter.
 * Configuration is via conf object types for the file adapter; sessions
 * follow the usual adapter session CRUD macros.
 *
 * @{
 */

/**
 * @defgroup afw_file_internal File adapter internal
 * @ingroup afw_c_api_internal
 *
 * File adapter implementation details for libafw only.
 */

/** @} */

/**
 * @defgroup afw_compile Compile
 *
 * Compilation of Adaptive Script, templates, JSON, etc. into value graphs.
 *
 * Main entry: afw_compile_to_value_with_callback and variants.
 * Result is usually a compiled_value owning its own pool.
 * Requires functions to be registered first (bootstrap order).
 *
 * Residual check (none/to_newline/to_full/to_close_brace) in lexical.
 *
 * **Compiler maintainers (not most extension authors):** grammar fragments
 * live in special comments next to the real parser/lexer in
 * `src/afw/compile/` (open/close markers ebnf then triple greater-than /
 * triple less-than ebnf). afwdev harvests those into a single generated
 * EBNF under `generated/ebnf/` (file lists in `generate/ebnf/`). Edit the
 * C comments and parser together; never hand-edit the harvested EBNF.
 * Handbook syntax diagrams are maintained from that harvest. The C parser
 * remains authoritative if docs and code ever disagree.
 *
 * @{
 */

/**
 * @defgroup afw_compile_internal Compile internal
 * @ingroup afw_c_api_internal
 *
 * Compiler/parser internals for libafw only (not extension API).
 */
/** @} */

/**
 * @defgroup afw_code_point Code points
 *
 * Unicode **code-point** property tests (identifier, whitespace, EOL,
 * general category Cc). Encoding-neutral: they take an `afw_code_point_t`,
 * not UTF-8 octets. UTF-8 encode/decode lives in @ref afw_utf8.
 *
 * Home: `src/afw/code_point/` (`afw_code_point.h`).
 */

/**
 * @defgroup afw_request Request & Handlers
 *
 * HTTP-like request handling used by hosts (`afwfcgi`, `afw --local`, …).
 *
 * A director matches configured `uriPrefix` values and dispatches to request
 * handlers (commonly adapter REST). Hosts implement `afw_server` /
 * `afw_request`; handlers implement `afw_request_handler`.
 *
 * Call methods via interface macros. See also `afw_server_fcgi` and the
 * `afw` command local path.
 *
 * @{
 */

/**
 * @defgroup afw_request_handler_impl Request handler impl
 * @ingroup afw_request
 *
 * Shared helpers for implementing request handlers.
 */
/**
 * @defgroup afw_request_handler_internal Request handler internal
 * @ingroup afw_c_api_internal
 *
 * Request handler internals for libafw only.
 */

/** @} */

/**
 * @defgroup afw_request_handler Request handler
 * @ingroup afw_request
 *
 * Request handler interface and director support.
 *
 * The director routes by `uriPrefix`. Handler implementations live in
 * adapters, authorization, and hosts. Prefer call macros on the handler
 * and director interfaces over raw `inf` access.
 */

/**
 * @defgroup afw_content_type Content Types
 *
 * Serialization between adaptive values and raw bytes (media types).
 *
 * Core provides JSON; extensions add YAML, UBJSON, etc. Types register
 * with the environment (`afw_content_type_register`) and are selected by
 * content-type / conf. Implementers use content_type (+ optional object
 * list writer) interfaces from afwdev scaffolds.
 *
 * @{
 */

/**
 * @defgroup afw_content_type_impl Content type impl
 * @ingroup afw_content_type
 *
 * Shared helpers for implementing content types (afwdev add-content-type).
 */

/** @} */

/**
 * @defgroup afw_authorization Authorization
 *
 * Authorization checks and pluggable authorization handlers.
 *
 * Core asks handlers whether an action is allowed; the script handler runs
 * policy as Adaptive Script. Intermediate mode and handler install order
 * matter for adapters and internal resource access.
 *
 * @{
 */

/**
 * @defgroup afw_authorization_handler_impl Authz handler impl
 * @ingroup afw_authorization
 *
 * Shared helpers for implementing authorization handlers.
 */
/**
 * @defgroup afw_authorization_internal Authorization internal
 * @ingroup afw_c_api_internal
 *
 * Authorization internals for libafw only.
 */

/** @} */

/**
 * @defgroup afw_service Services
 *
 * Service lifecycle (start/stop/status) for long-running components.
 *
 * Adapters, logs, and similar conf-driven pieces are services started when
 * the environment/application starts. Use service APIs when adding
 * startable components rather than one-off global init.
 *
 * @{
 */

/**
 * @defgroup afw_service_internal Service internal
 * @ingroup afw_c_api_internal
 *
 * Service lifecycle internals for libafw only.
 */

/** @} */

/**
 * @defgroup afw_action Action perform
 *
 * Batch execution of `_AdaptiveAction_` lists (function calls and related
 * steps) used by admin/apps and hosts.
 *
 * `afw_action_perform` runs the list with shared request context; see
 * action helpers and request integration when embedding AFW in a host.
 */

/**
 * @defgroup afw_log Log
 *
 * Adaptive Framework Log Support.
 *
 * Log priorities are defined in enum afw_log_priority_t in afw_interface.h.
 * The AFW_LOG macros accept the last part of the enum value name as the
 * priority parameter.
 *
 * Log priority trace1-8 are not logged, but instead goes to stderr or the
 * configured trace destination.
 *
 * priority   | Description
 * ---------- | -----------
 * none       | Priorities will be set individually
 * emerg      | System unusable
 * alert      | Immediate action required
 * crit       | Critical conditions
 * err        | Error conditions
 * warning    | Warning, errors may occur
 * notice     | Normal but significant
 * info       | Informational
 * debug      | Debug
 * trace1     | Trace level 1 message
 * trace2     | Trace level 2 message
 * trace3     | Trace level 3 message
 * trace4     | Trace level 4 message
 * trace5     | Trace level 5 message
 * trace6     | Trace level 6 message
 * trace7     | Trace level 7 message
 * trace8     | Trace level 8 message
 */

/**
 * @defgroup afw_error Error
 *
 * Adaptive Framework error handling.
 *
 * Typedef afw_error_code_t is defined in afw_common.h.
 *
 * The AFW_TRY / AFW_CATCH / AFW_FINALLY / AFW_ENDTRY macros provide
 * structured exception-like handling using setjmp/longjmp.
 *
 * See the large comment block in afw_error.h (or the original group
 * description) for full usage of AFW_TRY, AFW_CATCH, AFW_CATCH_UNHANDLED,
 * AFW_FINALLY, AFW_ENDTRY, AFW_THROW_ERROR_*, etc.
 */

/**
 * @defgroup afw_memory Memory
 *
 * Untyped pointer+length (`afw_memory_t`) plus copy/encode helpers.
 *
 * Same dest/copy verbs as @ref afw_utf8 (`create` copies, `create_no_copy`
 * points, `set` / `set_no_copy`) but **no NFC** and no `forced_safe`.
 * Cast utf8 → memory with `afw_utf8_as_memory`; the other way is
 * `afw_utf8_from_memory` (NFC). There is no `afw_raw_t`.
 */

/**
 * @defgroup afw_flag Flags
 *
 * Feature and debug/trace flags registered in the environment.
 *
 * Flags gate optional behavior and tracing. Register and test flags rather
 * than hard-coding global booleans in extensions when possible.
 */

/**
 * @defgroup afw_trace Software Trace
 *
 * Macros and functions for writing trace messages to the environment log.
 * Each message carries runtime information (rti) used for prefixes and
 * filtering. The first parameter of AFW_TRACE(), AFW_TRACE_Z(),
 * AFW_TRACE_FZ(), and AFW_TRACE_VZ() is an instance used to locate the
 * implementation rti. AFW_TRACE_WRITE*() macros and afw_trace_write*()
 * take rti as the first parameter.
 *
 * Trace output goes to the environment log (see afw_log.h).
 */

/**
 * @defgroup afw_lock Locks
 *
 * Process locks and related synchronization used inside libafw.
 *
 * Prefer documented lock helpers over inventing ad-hoc mutex usage across
 * pool/env boundaries; many AFW structures assume single-xctx or
 * documented multi-thread rules.
 */

/**
 * @defgroup afw_thread Threads
 *
 * Thread create/join helpers and thread attributes.
 *
 * AFW often uses APR thread primitives under these wrappers. Follow pool
 * and xctx rules when sharing data across threads.
 */

/**
 * @defgroup afw_time Time
 *
 * Date/time parse, format, and arithmetic for adaptive date/time types.
 *
 * Used by data types and functions; prefer these helpers over platform
 * time APIs when values must round-trip as adaptive types.
 */

/**
 * @defgroup afw_uri URI
 *
 * URI parse and related helpers for paths and adaptive anyURI values.
 */

/**
 * @defgroup afw_utf8 UTF-8
 *
 * UTF-8 strings (`afw_utf8_t`) and NFC normalization helpers.
 *
 * Most AFW text is UTF-8 with length + pointer (not always NUL-terminated).
 * The little struct has **no pool and no refcount**. Lifetime is whoever
 * owns the bytes (a pool, a value header, a stack, a literal). Adaptive
 * **values** (`afw_value_*`) are what can `get_reference` / release.
 *
 * **Internal** is NFC `afw_utf8_t`. **External** (libc, APR, LDAP, logs)
 * uses a named door: `to_utf8_z` / `z_create`, `forced_safe`, or
 * `as_memory`.
 *
 * **Naming (short name does more):**
 *
 * Prefix `afw_utf8_z_` when you **have** a C string. Prefix `afw_utf8_`
 * when you have length-prefixed `utf8`. Mixed predicates spell both
 * types in argument order (`starts_with_utf8_z`).
 *
 * | Door | Dest | Bytes |
 * |------|------|--------|
 * | `create` | New `const` in `p` | Copy + NFC (or throw) |
 * | `create_no_copy` | New `const` in `p` | Point at `s`; NFC or throw |
 * | `z_to_utf8` | New `const` in `p` | Ingest `utf8_z` (copy + NFC) |
 * | `z_as_utf8` | New `const` in `p` | Ingest `utf8_z` (point) |
 * | `set` / `z_set` | Caller `afw_utf8_t *` | Copy into `p` + NFC |
 * | `set_no_copy` / `z_set_no_copy` | Caller `afw_utf8_t *` | Point; no `p` |
 * | `clone` | New `const` in `p` | Copy struct + `.s` |
 * | `to_utf8_z` / `z_create` | `utf8_z` | External C string; throw if embedded 0 |
 * | `forced_safe` | create/set (always copy) | External encode; `^hex^`; not NFC; not a value |
 * | `create_property_name` | New `const` in `p` | Same encode, then NFC (is a name) |
 *
 * `p` only if something new lives there.
 *
 * See `afw_utf8.h` and `designs/c-naming-and-payloads.md`.
 */

/**
 * @defgroup afw_uuid UUID
 *
 * UUID create, parse, and format helpers used by adapters and objects.
 */

/**
 * @defgroup afw_number Number
 *
 * Numeric parse/format helpers for adaptive integer/double and related types.
 */

/**
 * @defgroup afw_safe_cast Safe Cast
 *
 * Integer casts that check range and report errors instead of silent wrap.
 *
 * Use when converting sizes or protocol fields into AFW integer types.
 */

/**
 * @defgroup afw_common Common
 *
 * Common types, macros, and includes used throughout AFW
 * (`afw_common.h` layer under `afw_interface.h` / `afw_minimal.h`).
 *
 * Base layer included early: fundamental typedefs, try/catch-related
 * pieces, and shared macros. Prefer including `afw.h` or
 * `afw_interface.h` rather than pulling common alone from extensions.
 */

/**
 * @defgroup afw_atomic Atomic
 *
 * Atomic load/store and compare-exchange helpers for shared counters/flags.
 */

/**
 * @defgroup afw_endian Endian
 *
 * Portable endian conversion for binary protocols and on-disk formats.
 */

/**
 * @defgroup afw_ascii ASCII
 *
 * ASCII classification and case helpers (also valid for ASCII subset of
 * Unicode code points). Prefer UTF-8 APIs for full Unicode text.
 */

/**
 * @defgroup afw_stack Stack
 *
 * Growable stacks used by the compiler and runtime (values, frames, etc.).
 *
 * Not a general application container; use adaptive arrays/objects for
 * script-visible data.
 */

/**
 * @defgroup afw_stream Stream
 *
 * Readable/writable streams (files, memory, response bodies, etc.).
 *
 * Open helpers and progressive write paths for large responses live here.
 * Distinct from adapter retrieve limits/paging (see retrieve max-objects
 * notes in core-services docs). Prefer `afw_stream_*` call macros and
 * open helpers over inventing raw file I/O for response bodies.
 *
 * @{
 */

/**
 * @defgroup afw_writer Writer
 * @ingroup afw_stream
 *
 * Incremental writers used when serializing values (JSON, etc.).
 *
 * Helpers wrap the `afw_writer` interface for C strings and `afw_utf8_t`.
 * Prefer writer APIs over building giant intermediate strings when
 * streaming output.
 */

/** @} */

/**
 * @defgroup afw_version Version
 *
 * Runtime version strings and git-info macros for packages and extensions.
 *
 * Extensions usually include generated `*_version_info.h` and report
 * versions via the extension interface / manifest.
 */

/**
 * @defgroup afw_components Components
 *
 * UI component identities and related objects used with the admin app.
 *
 * Not required for most C extensions; relevant when shipping component
 * metadata for the React admin UI.
 */

/**
 * @defgroup afw_debug Software Debug
 *
 * Macros and functions for writing debug messages to the environment log.
 * Each message carries runtime information (rti) used for prefixes and
 * filtering. The first parameter of AFW_DEBUG(), AFW_DEBUG_Z(),
 * AFW_DEBUG_FZ(), and AFW_DEBUG_VZ() is an instance used to locate the
 * implementation rti. AFW_DEBUG_WRITE*() macros and afw_debug_write*()
 * take rti as the first parameter.
 *
 * Debug output goes to the environment log (see afw_log.h).
 */

/**
 * @defgroup afw_context Context
 *
 * Context types for pushing qualified variables and related runtime
 * context (used by authorization, adapters, and script evaluation).
 *
 * Distinct from `afw_xctx` (execution context). Context registration
 * supplies variable bags that script sees as qualifiers.
 */

/**
 * @defgroup afw_json JSON
 *
 * JSON content-type support and value ↔ JSON text conversion (core).
 *
 * Primary built-in content type. For YAML/UBJSON see extensions. Internals
 * of the JSON parser/writer live under the internal subgroup when present.
 *
 * @{
 */

/**
 * @defgroup afw_json_internal JSON internal
 * @ingroup afw_c_api_internal
 *
 * JSON parser/writer internals for libafw only.
 */

/** @} */

/**
 * @defgroup afw_query_criteria Query Criteria
 *
 * Query criteria structures and RQL-like parsing for adapter retrieves.
 *
 * Built or parsed criteria drive `retrieve_objects` filters; indexes may
 * accelerate sargable criteria on adapters that support them (e.g. LMDB).
 */

/**
 * @defgroup afw_model Models
 *
 * Model adapter: reshape objects between a backend adapter and a
 * model-facing object type graph (mappings and `on*` Adaptive Script).
 *
 * Hybrid models, location resolution, and compile of model expressions
 * live here. Prefer model APIs over hand-transforming objects in hosts.
 *
 * @{
 */

/**
 * @defgroup afw_model_internal Model internal
 * @ingroup afw_c_api_internal
 *
 * Model adapter internals for libafw only.
 */

/** @} */

/**
 * @defgroup afw_runtime Runtime Objects
 *
 * Read-only adaptive objects that mirror environment/runtime state
 * (registry entries, etc.) for introspection and admin tools.
 *
 * Object maps are generated; value accessors convert C fields to values.
 *
 * @{
 */

/**
 * @defgroup afw_runtime_value_accessor Runtime value accessors
 * @ingroup afw_runtime
 *
 * Generated-style accessors that convert C fields on runtime objects to values.
 */

/** @} */

/**
 * @defgroup afw_array Arrays / lists
 *
 * Adaptive array (list) values and helpers.
 *
 * Prefer array create APIs and interface methods (via macros) for
 * script-visible lists. Templates and associative arrays are related
 * helpers for C-side structures.
 *
 * @{
 */

/**
 * @defgroup afw_array_impl Array impl helpers
 * @ingroup afw_array
 *
 * Shared helpers for implementing adaptive arrays.
 */
/**
 * @defgroup afw_array_internal Array internal
 * @ingroup afw_c_api_internal
 *
 * Array implementation details for libafw only.
 */
/**
 * @defgroup afw_array_template_internal Array templates internal
 * @ingroup afw_c_api_internal
 *
 * C template helpers for array-like structures (libafw only).
 */

/** @} */

/**
 * @defgroup afw_data_type Data types (hand helpers)
 *
 * Hand helpers around adaptive data types (beyond generated bindings).
 *
 * Generated per-type APIs live under @ref afw_c_api_data_types. Use those
 * create/evaluate paths from extensions unless you are inside libafw.
 */

/**
 * @defgroup afw_application Application
 *
 * Application conf, qualified variables, and startup for an AFW application
 * instance (distinct from a single extension).
 *
 * @{
 */

/**
 * @defgroup afw_application_internal Application internal
 * @ingroup afw_c_api_internal
 *
 * Application conf/startup internals for libafw only.
 */

/** @} */

/**
 * @defgroup afw_os OS helpers
 *
 * Thin OS/portability helpers used by libafw (paths, process bits, etc.).
 */

/**
 * @defgroup afw_c_api_data_types Adaptive data types
 * 
 * Adaptive data types that are part of the core C API.
 *
 * See individual @defgroup afw_data_type_* for each type's create APIs and infs.
 * Data types drive value representation and polymorphic behavior.
 */

/**
 * @defgroup afw_c_api_data_types_internal Adaptive data types (internal)
 * 
 * Internal data type APIs.
 * 
 * This API might change between releases and even between patches, so do not
 * use outside of afw library source (libafw).
 */

/**
 * @defgroup afw_interface Interfaces
 *
 * The core C interfaces (contracts) that both libafw and extensions implement.
 *
 * Defined in `generate/interfaces/afw_interface.xml` and generated into
 * `afw_interface.h` plus the various `*_impl_declares.h` headers.
 *
 * **Call macros are the developer API.** Prefer
 * `afw_<interface>_<method>(instance, …)` over
 * `(instance)->inf-><method>(…)`. The arrow form is generated wiring and is
 * useful in GDB; the macros are what extension and command authors should use
 * and what Doxygen documents as the call surface.
 *
 * Method descriptions and parameter text come from the interface XML and are
 * emitted on those call macros by afwdev generate. Do not hand-edit
 * `generated/afw_interface.h` for documentation.
 *
 * When implementing (not only calling), use afwdev `add-core-interface`,
 * `add-adapter-type`, etc., which copy closet skeletons and leave `@todo`
 * markers. See also @ref afw_c_api_impl.
 */

/**
 * @defgroup afw_c_api_impl Interface implementation support
 *
 * **Supported** helpers for *implementing* core interfaces (not everyday
 * call sites). Still part of the public install surface via `afw.h` /
 * `*_impl.h` — almost every extension implements at least one interface.
 *
 * Typical path:
 * 1. `afwdev add-adapter-type` / `add-content-type` / `add-core-interface` / …
 * 2. Set `AFW_IMPLEMENTATION_ID` and include the matching `*_impl_declares.h`.
 * 3. Fill `impl_<interface>_<method>` from the closet skeleton (`@todo`).
 * 4. Register via manifest / generated register for the srcdir.
 *
 * Prefer `AFW_DECLARE` helpers documented here over anything in
 * @ref afw_c_api_internal. Closet files under `generated/interface_closet/`
 * are templates for afwdev, not production source. See
 * `src/afw/doc/developer/` (`implementing-interfaces`, `interfaces`).
 */

/** @} */  // end of afw_c_api_public

/**
 * @defgroup afw_c_api_internal C Internal (libafw only)
 *
 * **Unsupported** outside libafw. For maintainers working in `src/afw/` only.
 *
 * - Include path: `afw_internal.h` and module `*_internal.h` (not installed
 *   with the default public header set; always in the git tree).
 * - Stability: may change between patches and builds.
 * - Extensions/commands: use `afw.h`, call macros, and @ref afw_c_api_impl.
 *
 * Nested `*_internal` groups hang here for Doxygen navigation when reading
 * full source. Do not treat visibility in HTML as an API promise.
 */

/**
 * @defgroup afw_log_impl Log impl helpers
 * @ingroup afw_log
 *
 * Shared helpers for implementing log types (afwdev add-log-type).
 */

/**
 * @defgroup afw_log_internal Log internal
 * @ingroup afw_c_api_internal
 *
 * Log internals for libafw only.
 */

/**
 * @defgroup afw_log_deprecated Deprecated log API
 * @ingroup afw_log
 *
 * Compatibility log surface; prefer current log interfaces and AFW_LOG macros.
 */

/**
 * @defgroup afw_included_commands Commands
 *
 * Command / host srcdirs shipped in this **AFW base** package (primarily
 * `src/afw_command/` for the `afw` CLI). Not part of libafw core; they
 * depend on public core APIs only.
 *
 * Commands load conf and extensions, then compile/eval or serve
 * `--local` requests. Scaffold new commands with `afwdev make-command`.
 */

/**
 * @defgroup afw_included_extensions Extensions
 *
 * Loadable extension **srcdirs** (and similar packages outside this tree)
 * that register into the same environment as core.
 *
 * **Layout:** `src/afw/` is **libafw core**. Other `src/<srcdir>/` trees
 * (curl, ldap, lmdb, … in this base package) should stay **as
 * self-contained as practical** — think “could copy this srcdir into
 * another AFW package repo.” They may use all **public** core headers and
 * call macros; they must not depend on `afw_internal.h` or core-private
 * groups. Core may mention base extensions lightly for navigation; detailed
 * Doxygen for an extension lives in **that extension’s headers**.
 *
 * After load, extensions are peers of core (adapter types, content types,
 * functions, …). New extensions: @ref afw_dev_extending,
 * @ref afw_dev_implementing. In this package, `src/afw_curl/` is a good
 * functions-focused example; adapter-type examples include ldap/vfs/lmdb.
 */

/**
 * @defgroup afw_xctx Execution context (xctx)
 *
 * Execution context (scope stack, statement flow, error handling, etc.).
 *
 * An `afw_xctx_t` represents a unit of work. It holds the scope stack,
 * evaluation stack, statement_flow flags, qualifiers, and its own pool.
 * Child contexts (e.g. per-request) have shorter lifetimes.
 *
 * Scopes use subpools for automatic cleanup. statement_flow controls
 * break/continue/return/rethrow without C exceptions.
 *
 * @{
 */

/**
 * @defgroup afw_xctx_internal xctx internal
 * @ingroup afw_c_api_internal
 *
 * Execution-context internals for libafw only.
 */

/** @} */

/** @} */  // end of package_afw

/** @addtogroup afw_c_api_public
 * @{
 */

/**
 * @file afw_doxygen.h
 * @brief Top-level Doxygen @defgroup definitions for the AFW C API.
 *
 * Adaptive Framework Base Package Doxygen main groups.
 */

/** @} */  // end of group

#endif /* __AFW_DOXYGEN_H__ */
