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
 * The public C API intended for use by extensions, commands, and
 * applications built on top of Adaptive Framework.
 *
 * Includes the main interfaces, data types, functions, pools, values, etc.
 * @{
 */

/**
 * @defgroup afw_pool Pool
 *
 * Adaptive Framework hierarchical memory pools.
 *
 * Pools provide fast, hierarchical allocation with bulk free semantics.
 * Most memory in AFW (values, objects, scopes, etc.) is allocated from pools.
 * Subpools allow scoped lifetime management while still returning memory
 * to the parent when released.
 *
 * Key concepts:
 * - Parent/child relationship for lifetime control.
 * - Subpools for scoped allocation (e.g. per-xctx scope).
 * - Thread-specific vs multithreaded pools.
 * - No manual free for most allocations; bulk release on pool destroy.
 *
 * Key functions: afw_pool_create(), afw_pool_create_subpool(),
 * afw_pool_calloc(), afw_pool_malloc().
 *
 * @{
 */

/** @defgroup afw_pool_internal Pool internal @ingroup afw_c_api_internal */

/** @} */

/**
 * @defgroup afw_value Value
 *
 * Adaptive Framework value graph (the core data model).
 *
 * Every significant runtime entity is represented as an `afw_value_t`
 * (inf pointer + payload). Values are generally immutable.
 * The graph includes `compiled_value`, blocks, calls, symbol references,
 * closures, and all data-type values.
 *
 * Evaluation is driven by `inf->optional_evaluate`.
 *
 * Key invariants (see also afw-value-memory rule):
 * - Permanent values last for the life of the environment.
 * - Managed values use refcounting or cloning for escape.
 * - Managed slices for utf8/memory views into managed values.
 * - Prefer allocating evaluation results into current scope->p for long-running use.
 *
 * @{
 */

/** @defgroup afw_value_internal Value internal @ingroup afw_c_api_internal */

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

/** @defgroup afw_environment_internal Environment internal @ingroup afw_c_api_internal */

/** @} */

/**
 * @defgroup afw_function Adaptive Functions
 *
 * Built-in Adaptive functions. Defined in generate/objects/_AdaptiveFunctionGenerate_.
 * Hand implementations are in src/afw/function/. The compiler lowers operators
 * and calls to these.
 *
 * Polymorphic functions use NULL execute and resolve at runtime based on arg type.
 * Operator functions are prepared specially in prepare_environment.
 */

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

/** @defgroup afw_object_meta Object meta @ingroup afw_object */
/** @defgroup afw_object_path Object path @ingroup afw_object */
/** @defgroup afw_object_options Object options @ingroup afw_object */
/** @defgroup afw_object_type Object type @ingroup afw_object */
/** @defgroup afw_object_view Object view @ingroup afw_object */
/** @defgroup afw_object_impl Object impl helpers @ingroup afw_object */
/** @defgroup afw_object_internal Object internal @ingroup afw_c_api_internal */

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

/** @defgroup afw_adapter_impl Adapter impl helpers @ingroup afw_adapter */
/** @defgroup afw_adapter_index_impl Adapter index impl @ingroup afw_adapter */
/** @defgroup afw_adapter_internal Adapter internal @ingroup afw_c_api_internal */

/** @} */

/**
 * @defgroup afw_file File Adapter
 *
 * Adapter for storing objects in the local filesystem.
 */

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

/** @defgroup afw_compile_internal Compile internal @ingroup afw_c_api_internal */
/** @defgroup afw_code_point Code points (compiler) @ingroup afw_compile */

/** @} */

/**
 * @defgroup afw_request Request & Handlers
 *
 * Request handling, director, and related interfaces.
 *
 * The director matches uriPrefix and dispatches to handlers (typically
 * adapter REST). Implements afw_server/afw_request in hosts.
 *
 * See also afw_server_fcgi and afw command --local.
 */

/**
 * @defgroup afw_request_handler Request handler
 *
 * Request handler interface and director support.
 *
 * The director routes requests based on configured uriPrefix values.
 * Actual handler implementations live in adapters, authorization, etc.
 *
 * See afw_request_handler_director.
 */

/**
 * @defgroup afw_content_type Content Types
 *
 * Serialization between values and bytes (JSON, YAML, UBJSON, etc.).
 *
 * Core has JSON; extensions add others. Registered via afw_content_type_register.
 */

/**
 * @defgroup afw_authorization Authorization
 *
 * Authorization handlers and policy support.
 *
 * Handlers decide access. Script handler allows policy as Adaptive Script.
 */

/**
 * @defgroup afw_service Services
 *
 * Service lifecycle (start/stop) for long-running components.
 *
 * Adapters, logs etc. are services started from conf.
 */

/**
 * @defgroup afw_action Action perform
 *
 * Support for _AdaptiveAction_ batch execution.
 *
 * afw_action_perform executes lists of actions (function calls etc.).
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
 * Memory utilities.
 */

/**
 * @defgroup afw_flag Flags
 *
 * Feature and trace flags.
 */

/**
 * @defgroup afw_trace Software Trace
 *
 * These macros and functions assist writing trace message to the environment
 * log.  All trace message have associated runtime information (rti) that can
 * be used for prefixing messages as well as filtering.  The first parameter
 * of macros AFW_TRACE(), AFW_TRACE_Z(), AFW_TRACE_FZ(), and AFW_TRACE_VZ()
 * is a instance, which is used to locate the implementation rti.  The
 * AFW_TRACE_WRITE*() macros and afw_trace_write*() functions have rti as the
 * first parameter.
 *
 * All trace message are written to the environment log.  See afw_log.h for
 * more information.
 */

/**
 * @defgroup afw_lock Locks
 *
 * Synchronization primitives.
 */

/**
 * @defgroup afw_thread Threads
 *
 * Threading support.
 */

/**
 * @defgroup afw_time Time
 *
 * Date and time handling.
 */

/**
 * @defgroup afw_uri URI
 *
 * URI parsing.
 */

/**
 * @defgroup afw_utf8 UTF-8
 *
 * UTF-8 and NFC strings.
 */

/**
 * @defgroup afw_uuid UUID
 *
 * UUIDs.
 */

/**
 * @defgroup afw_number Number
 *
 * Numeric support.
 */

/**
 * @defgroup afw_safe_cast Safe Cast
 *
 * Safe integer casting.
 */

/**
 * @defgroup afw_common Common
 *
 * Common types, macros, and includes used throughout AFW.
 * This is the base layer included early.
 */

/**
 * @defgroup afw_atomic Atomic
 *
 * Atomic operations.
 */

/**
 * @defgroup afw_endian Endian
 *
 * Byte order.
 */

/**
 * @defgroup afw_ascii ASCII
 *
 * ASCII utilities.
 */

/**
 * @defgroup afw_stack Stack
 *
 * Stacks.
 */

/**
 * @defgroup afw_stream Stream
 *
 * Streams.
 */

/**
 * @defgroup afw_writer Writer
 *
 * Writers.
 */

/**
 * @defgroup afw_version Version
 *
 * Version info.
 */

/**
 * @defgroup afw_components Components
 *
 * UI components.
 */

/**
 * @defgroup afw_debug Software Debug
 *
 * These macros and functions assist writing debug message to the environment
 * log.  All debug message have associated runtime information (rti) that can
 * be used for prefixing messages as well as filtering.  The first parameter
 * of macros AFW_DEBUG(), AFW_DEBUG_Z(), AFW_DEBUG_FZ(), and AFW_DEBUG_VZ()
 * is a instance, which is used to locate the implementation rti.  The
 * AFW_DEBUG_WRITE*() macros and afw_debug_write*() functions have rti as the
 * first parameter.
 *
 * All debug message are written to the environment log.  See afw_log.h for
 * more information.
 */

/**
 * @defgroup afw_context Context
 *
 * Contexts.
 */

/**
 * @defgroup afw_json JSON
 *
 * JSON.
 */

/**
 * @defgroup afw_query_criteria Query Criteria
 *
 * RQL-like query criteria.
 */

/**
 * @defgroup afw_model Models
 *
 * Model adapter for transforming objects between adapters.
 *
 * Models map shapes using scripts or simple mappings.
 */

/**
 * @defgroup afw_runtime Runtime Objects
 *
 * Read-only view of environment state as adaptive objects.
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
 * Helpers and macros for writing implementations of the core interfaces.
 *
 * Typical path:
 * 1. `afwdev add-adapter-type` / `add-content-type` / `add-core-interface` / …
 * 2. Set `AFW_IMPLEMENTATION_ID` and include the matching `*_impl_declares.h`.
 * 3. Fill `impl_<interface>_<method>` from the closet skeleton (`@todo`).
 * 4. Register via manifest / generated register for the srcdir.
 *
 * Closet files under `generated/interface_closet/` are templates for afwdev
 * scaffolding, not production source. See developer markdown under
 * `src/afw/doc/developer/` when present.
 */

/** @} */  // end of afw_c_api_public

/**
 * @defgroup afw_c_api_internal C Internal
 * 
 * Internal C APIs that are only for use inside libafw itself.
 *
 * These headers and functions may change at any time. Do not use from
 * extensions or application code.
 */

/** @defgroup afw_included_commands Commands
 *
 * The afw command-line tool and related command infrastructure.
 */

/**
 * @defgroup afw_included_extensions Extensions
 *
 * Loadable extensions (afw_curl, afw_ldap, afw_lmdb, afw_ubjson, afw_vfs,
 * afw_yaml, etc.).
 *
 * Extensions register the same kinds of things core does (adapter types,
 * content types, functions, etc.) and are first-class after loading.
 *
 * **Golden path to read first:** `src/afw_curl/` — extension initialize,
 * Adaptive function execute modules, manifest error RV decoder. Public
 * header `afw_curl.h` maps the layout. For adapter-type extensions, also
 * read `afw_vfs` or `afw_ldap` (factory/session impls from afwdev
 * `add-adapter-type` scaffolds).
 *
 * New extensions: @ref afw_dev_extending and @ref afw_dev_implementing.
 */

/** @defgroup afw_curl CURL extension @ingroup afw_included_extensions */

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

/** @defgroup afw_xctx_internal xctx internal @ingroup afw_c_api_internal */

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
