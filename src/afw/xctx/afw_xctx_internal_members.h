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
     * Scope pools whose last release is delayed while a throw is
     * processed, oldest first. See afw_error_processing_handled().
     */
    const afw_pool_t *error_delaying_release_first;

    /** Newest pool on error_delaying_release_first, or NULL. */
    const afw_pool_t *error_delaying_release_last;

    /**
     * Cycle collection state for this owner (possible roots). See
     * afw_reference.h. NULL until the first possible root.
     */
    void *reference_collector;

    /**
     * Depth of nested container element releases on this xctx. See
     * afw_reference_release_held().
     */
    afw_size_t release_depth;

    /**
     * Element releases deferred because release_depth reached
     * AFW_REFERENCE_RELEASE_DEPTH_MAX. C malloc; drained and freed
     * when release_depth returns to 0.
     */
    const afw_reference_t **release_pending;
    afw_size_t release_pending_count;
    afw_size_t release_pending_cap;

    /**
     * Number of try catch bodies being evaluated on this xctx. rethrow()
     * outside of one is an error.
     */
    afw_size_t catch_depth;

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
     * Managed isolate of the running script result (issue #62).
     * Does not require the current scope. slot_store / get_assignable
     * into dest p->managed_p. Deactivate writes last_statement_non_void_value
     * here unless that scope was cloned. Nested evaluate parks and
     * restores this occupant.
     */
    const afw_value_t *script_result;
