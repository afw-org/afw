// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework xctx internal members
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_xctx_internal_members.h
 * @brief Non-public members of struct afw_xctx_s.
 *
 * Included from struct afw_xctx_s in afw_xctx.h only when
 * AFW_XCTX_INTERNAL_MEMBERS is defined (afw_internal.h). Do not
 * include this file directly. Public fields stay a prefix of the
 * struct so a short view is a valid prefix of the full one.
 */

    /**
     * Error function for libxml2.
     */
    void *libxml2_error_func;

    /**
     * OS backtrace session for this xctx. NULL until the first
     * afw_os_backtrace() here. The OS code owns the bytes. Released
     * by afw_os_backtrace_cleanup() before this xctx's pool is
     * destroyed. Not shared with other xctxs or threads.
     */
    void *os_backtrace_data;

    /**
     * Private data used by xctx implementation.
     */
    void *priv;

    /**
     * Pools whose last release/destroy is delayed. First is most
     * recent; tail is innermost. See afw_error_processing_handled().
     */
    const afw_pool_t *error_delaying_release_first;

    /**
     * Runtime objects for xctx.
     */
    const afw_runtime_objects_t *runtime_objects;

    /**
     * The number of the scopes created.
     */
    afw_size_t scope_count;

    /**
     * The execution context (xctx) runtime scope stack. Entries are
     * const afw_pool_scope_t * (NULL sentinel around compiled_value
     * evaluate).
     */
    afw_pool_scope_p_vector_t *scope_stack;

    /**
     * The execution context (xctx) qualifier stack. Entries are
     * const afw_xctx_qualifier_stack_entry_t *.
     */
    const afw_xctx_qualifier_stack_t *qualifier_stack;

    /**
     * Internal struct used by adapters for this xctx. May be NULL.
     */
    afw_adapter_xctx_internal_t *adapter_xctx_internal;

    /**
     * The execution context (xctx) cache.
     */
    afw_adapter_internal_cache_t *cache;

    /**
     * The local dateTime when execution context was created.
     */
    afw_dateTime_t local_dateTime_when_created;

    /**
     * The UTC/Zulu dateTime when execution context was created.
     */
    afw_dateTime_t utc_dateTime_when_created;

    /**
     * A counter writers can increment and use to help identify the sequence
     * of writes.
     */
    afw_integer_t write_sequence;

    /**
     * Block statement flow type used while evaluate adaptive script.
     */
    afw_xctx_statement_flow_t statement_flow;

    /**
     * Target loop label for break/continue (issue #62). Interned string
     * value, or NULL if unlabeled. Cleared when the matching loop consumes
     * the flow.
     */
    const afw_value_t *statement_flow_label;

    /**
     * Last non-void result promoted out of a dying scope (issue #62).
     * Each frame keeps last_result; deactivate copies it here unless
     * that scope was cloned (no longer the running iteration). Nested
     * evaluate (compiled_value, script call, block as_value) saves
     * this pointer and restores it.
     */
    const afw_value_t *script_result;
