// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework environment internal members
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_environment_internal_members.h
 * @brief Non-public members of struct afw_environment_s.
 *
 * Included from struct afw_environment_s in afw_environment.h only when
 * AFW_ENVIRONMENT_INTERNAL_MEMBERS is defined (afw_internal.h). Do not
 * include this file directly. Public fields stay a prefix of the
 * struct so a short view is a valid prefix of the full one.
 *
 * Vector types are completed in afw_environment_internal.h.
 */

    /** @brief Program name. */
    union {
        afw_utf8_t program_name;
        const afw_utf8_z_t * program_name_z;
    };

    /** @brief Environment variables at environment create. */
    const afw_object_t *initial_environment_variables;

    /**
     * @brief Live process environment variables object
     * (`/afw/_AdaptiveEnvironmentVariables_/current`) for `environment::`.
     *
     * Created once at environment create. Qualifier is pushed in
     * afw_application_internal_push_qualifiers() for every xctx.
     */
    const afw_object_t *environment_variables_object;

    /**
     * @brief Process invocation object (`/afw/_AdaptiveProcess_/current`)
     * for `process::` (argv, programName, …).
     *
     * Created once at environment create. Qualifier is pushed in
     * afw_application_internal_push_qualifiers() for every xctx.
     */
    const afw_object_t *process_object;

    /** @brief Command-line args for process:: (env pool). */
    const afw_array_t *process_args;

    /** @brief cwd snapshot at environment create. */
    const afw_utf8_t *process_cwd;

    /** @brief pid at environment create. */
    afw_integer_t process_pid;

    /** @brief Start time at environment create. */
    const afw_dateTime_t *process_start_time;

    /** @brief Active libafw version string. */
    const afw_utf8_t *afw_version;

    /** @brief Adaptive framework core adapter. */
    const afw_adapter_t *afw_adapter;

    /** @brief Adapter for application.confAdapterId or NULL. */
    const afw_adapter_t *conf_adapter;

    /** @brief The id of the application. */
    afw_utf8_t application_id;

    /** @brief Application object - /afw/_AdaptiveApplication_/current. */
    const afw_object_t *application_object;

    /** brief Application qualified variables objects of compiled variables. */
    const afw_object_t *application_qualified_variables;

    /** @brief  rootFilePaths - /afw/_AdaptiveApplication_/current/rootFilePaths. */
    const afw_object_t *root_file_paths;

    /** @brief Custom layout adapter or NULL. */
    const afw_utf8_t *layout_adapter_id;

    /** @brief Double infinity */
    afw_double_t infinity;

    /** @brief Double infinity value */
    const afw_value_t *infinity_value;

    /** @brief Double minus infinity */
    afw_double_t minus_infinity;

    /** @brief Double minus infinity value */
    const afw_value_t *minus_infinity_value;

    /** @brief Double NaN */
    afw_double_t NaN;

    /** @brief Double NaN value */
    const afw_value_t *NaN_value;

    /** @brief Used by authorization. */
    const afw_authorization_control_t *authorization_control;

    /** @brief Used by function execution. */
    const afw_function_environment_t *function_environment;

    /**
     * @brief Adaptive evaluation-stack cap for each xctx.
     *
     * Vector is allocated at this count (`create_fixed_unhandled`).
     * Default `AFW_ENVIRONMENT_LIMIT_EVALUATION_STACK_COUNT`. Do not
     * use the `#define` after env create; use this field.
     */
    afw_size_t limit_evaluation_stack_count;

    /**
     * @brief Request/thread ST asked-for cap (bytes). 0 = unlimited.
     *
     * Default `AFW_ENVIRONMENT_LIMIT_REQUEST_POOL_BYTES`.
     */
    afw_size_t limit_request_pool_bytes;

    /**
     * @brief True when application conf set limitRequestPoolBytes.
     *
     * Then the cap also applies to the CLI base thread. Absent conf
     * leaves CLI uncapped.
     */
    afw_boolean_t limit_request_pool_apply_to_base;

    /**
     * @brief C-stack remaining required before throw. 0 = unlimited.
     *
     * Default `AFW_ENVIRONMENT_LIMIT_C_STACK_HEADROOM_BYTES`.
     */
    afw_size_t limit_c_stack_headroom_bytes;

    /**
     * @brief Default heap posix_memalign size when create passes 0.
     *
     * Default `AFW_ENVIRONMENT_CHUNK_MIN`. Align and packing stay
     * pool implementation, not env knobs.
     */
    afw_size_t chunk_min;

    /**
     * @brief Compile-unit heap chunk_min.
     *
     * Default `AFW_ENVIRONMENT_COMPILE_CHUNK_MIN`.
     */
    afw_size_t compile_chunk_min;

    /**
     * @brief xctx/thread heap chunk_min (process default).
     *
     * Default `AFW_ENVIRONMENT_XCTX_CHUNK_MIN`.
     */
    afw_size_t xctx_chunk_min;

    /** @brief Copy of director log's mask for short circuit tests. */
    afw_log_priority_mask_t log_mask;

    /**
     * @brief Sum of every pool's bytes_allocated.
     *
     * Outstanding size AFW malloc/calloc asked for and has not yet
     * given back: heap/tracker free decrements; general-pool free is a
     * no-op; destroy subtracts what is left. Rounding and prefixes
     * included. Not APR's private usage — process RSS is that hint.
     * Atomic; multithreaded pools update it.
     */
    AFW_ATOMIC afw_size_t pool_bytes_in_use;

    /** @brief High-water of pool_bytes_in_use. */
    AFW_ATOMIC afw_size_t peak_pool_bytes_in_use;

    /**
     * @brief Sum of every heap's chunk_bytes (posix_memalign held).
     *
     * Asked-for is pool_bytes_in_use. This is store still mapped.
     * Atomic; multithreaded heaps update it.
     */
    AFW_ATOMIC afw_size_t pool_chunk_bytes;

    /** @brief High-water of pool_chunk_bytes. */
    AFW_ATOMIC afw_size_t peak_pool_chunk_bytes;

    /*---------- LOCKS ----------*/

    /** @brief Lock for whole environment. */
    const afw_lock_t *environment_lock;

    /** @brief Lock for protecting changes to adapter id anchors. */
    const afw_lock_t *adapter_id_anchor_lock;

    /**
     * @brief Lock for protecting changes to authorization handler id
     *     anchors.
     *
     * Read/write lock is needed because anchor chain can be reordered
     * while processing handlers.
     */
    const afw_lock_rw_t *authorization_handler_id_anchor_rw_lock;

    /**
     * @brief Lock for protecting changes to active log list.
     */
    const afw_lock_t *active_log_list_lock;

    /**
     * @brief Lock for protecting changes to flags (internal to afw_flag.c).
     */
    const afw_lock_t *flags_lock;

    /*---------- FLAGS ----------*/

    /** @brief The number of flags allocated in global_flags. */
    AFW_ATOMIC afw_size_t flags_count_allocated;

    /** @brief The number of flags registered. */
    AFW_ATOMIC afw_size_t flags_count_registered;

    /**
     * @brief Default flags array indexed by flag_index.
     *
     * Always get a copy of flag_count_registered and/or flag_count_allocated
     * first.  This may change to a larger set of flags if allocated is not
     * large enough to hold a new flag registration.
     *
     * Normally this will not be accessed directly since this pointer is copied
     * to xctx->flags.
     */
    AFW_ATOMIC const afw_boolean_t *default_flags;

    /**
     * @brief Flag struct indexed by flag_index.
     *
     * Always get a copy of flag_count_registered and/or flag_count_allocated
     * first.  This may change to a larger set of flags if allocated is not
     * large enough to hold a new flag registration.
     */
    const afw_flag_t * AFW_ATOMIC const *flag_by_index;

    /** @brief Flag index of compile:strict. */
    afw_size_t flag_index_compile_strict_active;

    /** @brief Flag index of debug:function_active. */
    afw_size_t flag_index_debug_function_active;

    /** @brief Flag index of debug:function_active:detail. */
    afw_size_t flag_index_debug_function_active_detail;

    /** @brief Flag index of debug:lock. */
    afw_size_t flag_index_debug_lock;

    /** @brief Flag index of debug:lock:detail. */
    afw_size_t flag_index_debug_lock_detail;

    /** @brief Flag index of debug:pool. */
    afw_size_t flag_index_debug_pool;

    /** @brief Flag index of debug:pool:detail. */
    afw_size_t flag_index_debug_pool_detail;

    /** @brief Flag index of response:error:backtrace. */
    afw_size_t flag_index_response_error_backtrace;

    /** @brief Flag index of response:error:backtraceEvaluation. */
    afw_size_t flag_index_response_error_backtraceEvaluation;

    /** @brief Flag index of response:error:contextual. */
    afw_size_t flag_index_response_error_contextual;

    /** @brief Flag index of response:error:hasAdditionalDetail. */
    afw_size_t flag_index_response_error_hasAdditionalDetail;

    /** @brief Flag index of trace:authorization:check. */
    afw_size_t flag_index_trace_authorization_check;

    /** @brief Flag index of trace:authorization:check:bypass. */
    afw_size_t flag_index_trace_authorization_check_bypass;

    /** @brief Flag index of trace:authorization:check:detail. */
    afw_size_t flag_index_trace_authorization_check_detail;

    /** @brief Flag index of trace:authorization:decision. */
    afw_size_t flag_index_trace_authorization_decision;

    /** @brief Flag index of trace:authorization:decision:detail. */
    afw_size_t flag_index_trace_authorization_decision_detail;

    /** @brief Flag index of trace:request. */
    afw_size_t flag_index_trace_request;

    /** @brief Flag index of trace:request:detail. */
    afw_size_t flag_index_trace_request_detail;

    /* Base execution context. */
    afw_xctx_t *base_xctx;

    /* Configuration object. */
    const afw_array_t *configuration;

    /* Hash table of registry type name to registry type number. */
    afw_void_hash_table_t *registry_names_ht;

    /*
     * Vector indexed by afw_environment_registry_type_enum_t of
     * afw_environment_registry_type_t * for each type.
     */
    afw_environment_registry_type_p_vector_t *registry_types;

    /* Hash table of data type method numbers. */
    afw_void_hash_table_t *data_type_method_number_ht;

    /*
     * Vector indexed by data_type_number of pointers to method
     * vectors.
     */
    afw_environment_data_type_methods_vector_t *data_type_methods;

    /* Core data types have been registered. */
    afw_boolean_t core_data_types_registered;

    /* Core functions have been registered. */
    afw_boolean_t core_functions_registered;

    /* Core object type maps have been registered. */
    afw_boolean_t core_object_type_maps_registered;

    /*
     * NULL terminated list of default flagIds. Use
     * afw_flag_set_default_flag_ids() to set.
     */
    const afw_utf8_t * const *default_flag_ids;

    /*
     * Pool used to hold everything in default_flags_list.  This pool will be
     * destroyed when a new default_flags_list is set by a call to
     * afw_flag_set_default_flag_ids()
     */
    const afw_pool_t *default_flags_ids_p;
