// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Environment Header
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

#ifndef __AFW_ENVIRONMENT_H__
#define __AFW_ENVIRONMENT_H__

#include "afw_interface.h"

/**
 * Compile-time defaults copied into `afw_environment_t` at environment
 * create. Do not use these directly; use `xctx->env-><field>`.
 *
 * Exception: the base pool is created before env exists
 * (`afw_pool_internal_create_base_pool()`). That one call may use
 * `AFW_ENVIRONMENT_CHUNK_MIN`. After env create, heap `chunk_min == 0`
 * means `env->chunk_min`.
 */
#define AFW_ENVIRONMENT_LIMIT_EVALUATION_STACK_COUNT ((afw_size_t)500)
#define AFW_ENVIRONMENT_LIMIT_REQUEST_POOL_BYTES \
    ((afw_size_t)(64 * 1024 * 1024))
#define AFW_ENVIRONMENT_LIMIT_C_STACK_HEADROOM_BYTES \
    ((afw_size_t)(256 * 1024))
#define AFW_ENVIRONMENT_CHUNK_MIN ((afw_size_t)65536)
#define AFW_ENVIRONMENT_COMPILE_CHUNK_MIN ((afw_size_t)4096)
#define AFW_ENVIRONMENT_XCTX_CHUNK_MIN ((afw_size_t)65536)

/**
 * @addtogroup afw_environment
 * @{
 */

/**
 * @file afw_environment.h
 * @brief Header for the Adaptive Framework environment (registries).
 *
 * See the @ref afw_environment group (defined in afw_doxygen.h) for the mental model.
 *
 * Registration order matters critically. See afw_environment_register_core.c
 * and the environment rule for the exact sequence (generated first, then
 * prepare_environment for operators, then types, adapters, etc.).
 */

AFW_BEGIN_DECLARES

/**
 * @brief Process-wide environment (`afw_environment_t`).
 *
 * Registry fabric for the AFW process (one per application). Accessed as
 * `xctx->env`. See group afw_environment.
 */
struct afw_environment_s {

    /** @brief Pool used to hold environment. */
    const afw_pool_t *p;

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

    /** @brief Director log.  This log will direct to other logs. */
    const afw_log_t *log;

    /** @brief Open file descriptor used for debug writes. Default stderr. */
    FILE *debug_fd;

    /** @brief Open file descriptor used for writing error output. Default stderr. */
    FILE *stderr_fd;

    /** @brief Open file descriptor used for writing standard output. Default stdout. */
    FILE *stdout_fd;

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

    /** @brief Used to give unique number for pool. */
    AFW_ATOMIC afw_integer_t pool_number;

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

    /** @brief Indicates that environment is terminating. */
    afw_boolean_t terminating;


    /*---------- LOCKS ----------*/

    /** @brief Lock for whole environment. */
    const afw_lock_t *environment_lock;
    
    /** @brief Lock used internal to afw_pool.c. */
    const afw_lock_t *multithreaded_pool_lock;
    
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

    /** @brief Flag index of compile:noImplicitAny. */
    afw_size_t flag_index_compile_noImplicitAny_active;

    /** @brief Flag index of compile:typeCheck (full compile+runtime). */
    afw_size_t flag_index_compile_typeCheck_active;

    /** @brief Flag index of compile:typeCheckCompileOnly. */
    afw_size_t flag_index_compile_typeCheckCompileOnly_active;

    /** @brief Flag index of compile:strictNullChecks. */
    afw_size_t flag_index_compile_strictNullChecks_active;

    /** @brief Flag index of compile:strict. */
    afw_size_t flag_index_compile_strict_active;

    /** @brief Flag index of compile:noOptimize. */
    afw_size_t flag_index_compile_noOptimize_active;

    /** @brief Flag index of debug:function_active. */
    afw_size_t flag_index_debug_function_active;

    /** @brief Flag index of debug:function_active:detail. */
    afw_size_t flag_index_debug_function_active_detail;

    /** @brief Flag index of debug:evaluation. */
    afw_size_t flag_index_debug_evaluation;

    /** @brief Flag index of debug:evaluation:detail. */
    afw_size_t flag_index_debug_evaluation_detail;

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

};


/**
 * @brief Environment Core Registry Type Map.
 * @param id of registry type
 * @param register_additional custom callback function or NULL
 * @param allow_reregister true/false
 * @param property_name in _AdaptiveEnvironmentRegistry_
 * @param object_type_id in afw adapter of registered keys of this registry type
 * @param description of this registry type
 *
 * There must be and afw_s_ with each name in map.
 *
 * The register_additional function must be defined as a static function in
 * afw_environment.c.  If register_additional is NULL, a
 * afw_environment_registry_entry_t is created and a runtime object of the
 * specified object_type_id is set.
 *
 * These are just the core registry type.  Other types can be created by
 * calling afw_environment_create_registry_type() in an extension.
 */
#define AFW_ENVIRONMENT_REGISTRY_TYPE_MAP(XX)                                   \
                                                                                \
    XX(registry_type,                                                           \
        NULL,                                                                   \
        false,                                                                  \
        "registryType",                                                         \
        "_AdaptiveEnvironmentRegistryType_",                                    \
        "The afw_environment_registry_type_t associated with this property "    \
            "name .")                                                           \
                                                                                \
    XX(singleton,                                                               \
        impl_internal_additional_register_key_only,                             \
        false,                                                                  \
        "singleton",                                                            \
        "_AdaptiveSingleton_",                                                  \
        "The void pointer for a singleton key.")                                \
                                                                                \
    XX(adapter_id,                                                              \
        impl_internal_additional_register_default,                              \
        false,                                                                  \
        "adapterId",                                                            \
        "_AdaptiveAdapter_",                                                    \
        "The afw_adapter_id_t struct for this adapter id.")                     \
                                                                                \
    XX(adapter_type,                                                            \
        impl_internal_additional_register_default,                              \
        false,                                                                  \
        "adapterType",                                                          \
        "_AdaptiveAdapterType_",                                                \
        "The afw_adapter_factory_t instance for an adapter type.")              \
                                                                                \
    XX(authorization_handler_id,                                                \
        impl_internal_additional_register_default,                              \
        false,                                                                  \
        "authorizationHandlerId",                                               \
        "_AdaptiveAuthorizationHandler_",                                       \
        "The afw_authorization_handler_id_t struct for an "                     \
        "authorization handler id.")                                            \
                                                                                \
    XX(authorization_handler_type,                                              \
        impl_internal_additional_register_default,                              \
        false,                                                                  \
        "authorizationHandlerType",                                             \
        "_AdaptiveAuthorizationHandlerType_",                                   \
        "The afw_authorization_handler_factory_t instance for an "              \
        "authorization handler type.")                                          \
                                                                                \
    XX(conf_type,                                                               \
        impl_internal_additional_register_default,                              \
        false,                                                                  \
        "confType",                                                             \
        "_AdaptiveConfType_",                                                   \
        "The afw_environment_conf_type_t for a configuration type.")            \
                                                                                \
    XX(content_type,                                                            \
        impl_internal_additional_register_key_only,                             \
        false,                                                                  \
        "contentType",                                                          \
        "_AdaptiveContentType_",                                                \
        "The afw_content_type_t instance for a content type.")                  \
                                                                                \
    XX(context_type,                                                            \
        impl_internal_additional_register_object,                               \
        true,                                                                   \
        "contextType",                                                          \
        "_AdaptiveContextType_",                                                \
        "The context type object.  "                                            \
        "`/afw/_AdaptiveContextType_/<context_type_id>`"                        \
        " runtime object will be set to this object as well.")                  \
                                                                                \
    XX(data_type,                                                               \
        NULL,                                                                   \
        false,                                                                  \
        "dataType",                                                             \
        "_AdaptiveDataType_",                                                   \
        "The afw_data_type_t struct for a data type id.")                       \
                                                                                \
    XX(error_rv_decoder,                                                        \
        impl_internal_additional_register_key_only,                             \
        false,                                                                  \
        "errorRVDecoder",                                                       \
        "_AdaptiveErrorRVDecoder_",                                             \
        "The afw_environment_error_rv_decoder_z_t function for a rv source id.")\
                                                                                \
    XX(extension,                                                               \
        impl_internal_additional_register_default,                              \
        false,                                                                  \
        "extension",                                                            \
        "_AdaptiveExtension_",                                                  \
        "The afw_extension_t for a loaded extension.")                          \
                                                                                \
    XX(flag,                                                                    \
        NULL,                                                                   \
        false,                                                                  \
        "flagType",                                                             \
        "_AdaptiveFlag_",                                                       \
        "The afw_flag_t for a flag_id.")                                        \
                                                                                \
    XX(function,                                                                \
        NULL,                                                                   \
        false,                                                                  \
        "function",                                                             \
        "_AdaptiveFunction_",                                                   \
        "The afw_value_function_definition_t struct for a function id.")        \
                                                                                \
    XX(lock,                                                                    \
        impl_internal_additional_register_default,                              \
        true,                                                                   \
        "lockId",                                                               \
        "_AdaptiveLock_",                                                       \
        "The afw_lock_t instance for a log id.")                                \
                                                                                \
    XX(log,                                                                     \
        impl_internal_additional_register_default,                              \
        true,                                                                   \
        "logId",                                                                \
        "_AdaptiveLog_",                                                        \
        "The afw_log_t instance for a log id.")                                 \
                                                                                \
    XX(log_type,                                                                \
        impl_internal_additional_register_log_type,                             \
        false,                                                                  \
        "logType",                                                              \
        "_AdaptiveLogType_",                                                    \
        "The afw_log_factory_t instance for a log type.")                       \
                                                                                \
    XX(policy_combining_algorithm,                                              \
        impl_internal_additional_register_default,                              \
        false,                                                                  \
        "policyCombiningAlgorithmId",                                           \
        "_AdaptivePolicyCombiningAlgorithm_",                                   \
        "The afw_value_function_definition_t struct for a policy combining algorithm.")    \
                                                                                \
    XX(request_handler,                                                         \
        impl_internal_additional_register_key_only,                             \
        false,                                                                  \
        "requestHandler",                                                       \
        "_AdaptiveRequestHandler_",                                             \
        "The afw_request_handler_t instance for a request handler entry.")      \
                                                                                \
    XX(request_handler_type,                                                    \
        impl_internal_additional_register_default,                              \
        false,                                                                  \
        "requestHandlerType",                                                   \
        "_AdaptiveRequestHandlerType_",                                         \
        "The afw_request_handler_factory_t instance for a handler type.")       \
                                                                                \
    XX(rule_combining_algorithm,                                                \
        impl_internal_additional_register_default,                              \
        false,                                                                  \
        "ruleCombiningAlgorithmId",                                             \
        "_AdaptiveRuleCombiningAlgorithm_",                                     \
        "The afw_value_function_definition_t struct for a rule combining algorithm.")      \
                                                                                \
    XX(runtime_custom,                                                          \
        impl_internal_additional_register_key_only,                             \
        false,                                                                  \
        "runtimeCustom",                                                        \
        "_AdaptiveRuntimeCustom_",                                              \
        "The afw_runtime_custom_t for custom afw object type handling.")        \
                                                                                \
    XX(runtime_object_map_inf,                                                  \
        NULL,                                                                   \
        false,                                                                  \
        "runtimeObjectMapInf",                                                  \
        "_AdaptiveRuntimeObjectMapInf_",                                        \
        "The runtime object map interface for an object type id.")              \
                                                                                \
    XX(runtime_value_accessor,                                                  \
        impl_internal_additional_register_default,                              \
        false,                                                                  \
        "runtimeValueAccessor",                                                 \
        "_AdaptiveRuntimeValueAccessor_",                                       \
        "The afw_runtime_value_accessor_info_t for an accessor name "           \
            "(function plus Adaptive-visible contract properties).")            \
                                                                                \
    XX(service,                                                                 \
        NULL,                                                                   \
        true,                                                                   \
        "serviceId",                                                            \
        "_AdaptiveService_",                                                    \
        "The afw_service_t for a service_id.")                                  \
                                                                                \
    XX(service_type,                                                            \
        impl_internal_additional_register_default,                              \
        false,                                                                  \
        "serviceType",                                                          \
        "_AdaptiveServiceType_",                                                \
        "The afw_service_type_t for a service_type_id.")                        \
                                                                                \
    XX(string_literal,                                                          \
        impl_internal_additional_register_key_only,                             \
        false,                                                                  \
        "stringLiteral",                                                        \
        "_AdaptiveStringLiteral_",                                              \
        "Reusable string values (const afw_value_string_t), keyed by content.") \
                                                                                \
    XX(value_inf,                                                               \
        impl_internal_additional_register_key_only,                             \
        false,                                                                  \
        "valueInf",                                                             \
        "_AdaptiveValueInf_",                                                   \
        "The afw_value_inf_t struct for a value interface implementation.")     \


/** @brief Enum for the core registry types. */
typedef enum {
#define XX(id, register_additional, allow_reregister, property_name,          \
    object_type_id, description)                                              \
    afw_environemnt_registry_type_ ## id,
    AFW_ENVIRONMENT_REGISTRY_TYPE_MAP(XX)
#undef XX
    afw_environemnt_registry_type_max_core_type
} afw_environment_registry_type_enum_t;



/** @brief Default register_additional object struct. */
typedef struct afw_environment_default_register_additional_s {
    const afw_utf8_t *key;
} afw_environment_default_register_additional_t;



/** @brief environment extension instance quoted string. */
#define AFW_ENVIRONMENT_Q_EXTENSION_INSTANCE \
    "afw_environment_extension_instance"


/**
 * @brief Macro to put in each afw_extension implementation.
 * 
 * Put this after #include "afw_extension_impl_declares.h" in an
 * implementation of the afw_extension interface.  It provides the
 * symbol used by afw_environment_load_extension() to determine
 * the extensions id and version information.
 */
#define AFW_ENVIRONMENT_DEFINE_EXTENSION_IMPL() \
static const afw_extension_t \
impl_extension = \
{ \
    &impl_afw_extension_inf, \
    AFW_UTF8_LITERAL(AFW_IMPLEMENTATION_ID), \
    AFW_UTF8_LITERAL(AFW_IMPLEMENTATION_VERSION), \
    AFW_UTF8_LITERAL(AFW_VERSION_STRING), \
    AFW_VERSION_HEX \
}; \
\
AFW_DEFINE_DSO(const afw_extension_t *) \
afw_environment_extension_instance = &impl_extension



/**
 * @brief Typedef for afw conf type create functions.
 * @param type of conf (same as "conf" property in conf).
 * @param conf object.
 * @param source_location indicating the source of conf.
 * @param p ceded to handler.
 * @param xctx of caller.
 *
 * The p should be a multithreaded pool whose control is ceded to the thing
 * being created.  For example, for type=adapter, the adapter should
 * destroy this pool when the adapter's use count goes to zero.
 */
typedef void(*afw_environment_conf_type_create_cede_p_t)(
    const afw_utf8_t *type, const afw_object_t *conf,
    const afw_utf8_t *source_location,
    const afw_pool_t *p, afw_xctx_t *xctx);



/**
 * @brief Auto register callback function.
 * @param registry_type_id
 * @param key
 * @param xctx of caller.
 *
 * A function of this type can be specified on
 * afw_environment_create_registry_type() as auto_register parameter.
 */
typedef void * (*afw_environment_auto_register_cb_t)(
    const afw_utf8_t *registry_type_id,
    const afw_utf8_t *key,
    afw_xctx_t *xctx);



/**
 * @brief Additional register callback function.
 * @param registry_type_id
 * @param key
 * @param xctx of caller.
 *
 * A function of this type can be specified on
 * afw_environment_create_registry_type() as register_additional parameter.
 */
typedef void (*afw_environment_register_additional_cb_t)(
    const afw_utf8_t *registry_type_id,
    int type_number,
    const afw_utf8_t *key,
    const void *value,
    const void *register_additional_param,
    const void **register_additional_use,
    afw_xctx_t *xctx);



/** @brief Entries in environment->registry_types[] */
struct afw_environment_registry_type_s {

    /* Registry type id */
    const afw_utf8_t *registry_type_id;

    /* Type number */
    int number;

    /* Object type id */
    const afw_utf8_t *object_type_id;

    /* Description */
    const afw_utf8_t *description;

    /* Key Name */
    const afw_utf8_t *property_name;

    /* Indicates reregister of a key is allowed. */
    afw_boolean_t allow_reregister;

    /* Set and used by _AdaptiveEnvironmentRegistryType_. */
    afw_boolean_t auto_register_specified;

    /* Hash table used to hold keyed values for this type. */
    afw_void_hash_table_t *ht;

    /* Function to call for auto register or NULL. */
    afw_environment_auto_register_cb_t auto_register;

    /* Function to call after normal registration or NULL. */
    afw_environment_register_additional_cb_t register_additional;

    /* Parameter for register_additional function. */
    void *register_additional_param;

    /* &register_additional_use passed to register_additional for it's use. */
    const void *register_additional_use;

};


/** @brief Struct for afw environment conf type. */
struct afw_environment_conf_type_s {

    /** @brief Id of type. */
    afw_utf8_t conf_type_id;

    /** @brief Title of this type. */
    const afw_utf8_t *title;

    /** @brief Description of this type. */
    const afw_utf8_t *description;

    /** @brief Id property name for instances of this conf type. */
    const afw_utf8_t *id_property_name;

    /** @brief Registry type for instances of this conf type. */
    const afw_utf8_t *id_registry_type_id;

    /** @brief Runtime object type for instances of this conf type. */
    const afw_utf8_t *id_runtime_object_type_id;

    /** @brief Subtype property name for instances of this conf type. */
    const afw_utf8_t *subtype_property_name;

    /** @brief Subtype registry type for instances of this conf type. */
    const afw_utf8_t *subtype_registry_type_id;

    /** @brief Subtype runtime object type for instances of this conf type. */
    const afw_utf8_t *subtype_runtime_object_type_id;

    /** @brief Create function for this type. */
    afw_environment_conf_type_create_cede_p_t create;

    /** @brief This conf type can only be specified once. */
    afw_boolean_t is_unique;
};



/**
 * @brief Typedef for error rv decoder functions.
 * @param rv to decode.
 * @param wa work area that can be used to build return value.
 * @param wa_size work area size.
 *
 * IMPORTANT Implementation of this function must not do anything that might
 * throw an error or change xctx->error or a recursive error producing a
 * stack overflow will occur.
 */
typedef const afw_utf8_z_t * (*afw_environment_error_rv_decoder_z_t) (
    int rv, afw_utf8_z_t *wa, afw_size_t wa_size);


/**
 * @brief Create the Adaptive Framework core environment and return base xctx.
 * @param version of Adaptive Framework core caller is compiled against.
 * @param argc
 * @param argv
 * @param environment_create_error place if error occurs while creating environment. 
 * @return base xctx or NULL if environment_create_error points to an error.
 *
 * Call this function once in an Adaptive Framework application to initialize the
 * Adaptive Framework environment and create the initial base xctx.  All core
 * capabilities will be registered.  See afw_environment_register_core.c for
 * more detail.
 *
 * The following are important snippets for an Adaptive Framework application
 * that establishes an Adaptive Framework core environment and insures that it
 * will be released.
 *
 * int main(int argc, char * const * argv)
 * {
 *    ... Only used on afw_environment_create() call ...
 *    const afw_error_t *create_error;
 *
 *    ... xctx returned from AFW_ENVIRONMENT_CREATE() call ...
 *    afw_xctx_t *xctx;
 *
 *    ... Create Adaptive Framework environment using helper macro ...
 *    AFW_ENVIRONMENT_CREATE(xctx, argc, argv, &create_error);
 *    if (!xctx) {
 *        // Create failed: xctx is NULL. Use C stderr here; stream redirects
 *        // via env->stderr_fd apply only after a successful create.
 *        afw_error_print(stderr, create_error);
 *        return -1; 
 *    }
 *   
 *    ... Body of app protected by AFW_TRY ...
 *    AFW_TRY {
 *       ... Adaptive Framework application code ...
 *       ... Use break to leave AFW_TRY, not goto or return.
 *    }
 *
 *    ... Catch other errors that need special handling ...
 *
 *    ... Print any other errors. ...
 *    AFW_CATCH_UNHANDLED{
 *        afw_error_print(xctx->env->stderr_fd, AFW_ERROR_THROWN);
 *        rv = -1;
 *    }
 *
 *    ... Makes sure environment is released ...
 *    AFW_FINALLY {
 *        afw_environment_release(xctx);
 *
 *        ... Special case: xctx is gone, so return before AFW_ENDTRY. ...
 *        return rv;
 *    }
 *
 *    AFW_ENDTRY;
 * }
 *
 */
AFW_DECLARE(afw_xctx_t *)
afw_environment_create(
    unsigned int version,
    int argc, const char * const *argv,
    const afw_error_t **environment_create_error);

/**
 * @brief Call afw_environment_create() supplying version compiled with.
 * @param xctx set to base xctx or NULL if environment_create_error points to an error.
 * @param argc
 * @param argv
 * @param environment_create_error place if error occurs while creating environment.
 *
 * This macro calls AFW_VERSION_ABORT_IF_NOT_COMPATIBLE() to check for
 * compatibility.  If you want to handle this situation differently,
 * use AFW_VERSION_CHECK before AFW_ENVIRONMENT_CREATE().  
 */
#define AFW_ENVIRONMENT_CREATE(xctx, argc, argv, environment_create_error) \
    AFW_VERSION_ABORT_IF_NOT_COMPATIBLE("AFW_ENVIRONMENT_CREATE()"); \
    xctx = afw_environment_create(AFW_VERSION_HEX, argc, argv, \
        environment_create_error)


/**
 * @brief Release the Adaptive Framework core environment and base xctx.
 * @param xctx returned from afw_environment_create().
 */
AFW_DECLARE(void)
afw_environment_release(afw_xctx_t *xctx);



/**
 * @brief Get current environment registry object.
 * @param xctx of caller.
 * @return /afw/_AdaptiveEnvironmentRegistry_/current object.
 */
AFW_DEFINE(const afw_object_t *)
afw_environment_registry_object_get_current(afw_xctx_t *xctx);



/**
 * @brief Configure environment with a configuration entry.
 * @param conf object.
 * @param source_location to associate with this entry.
 * @param xctx MUST be base xctx.
 *
 * Parameter source_location default to the path of the entry object.
 */
AFW_DECLARE(void)
afw_environment_configure_with_object(
    const afw_object_t *conf,
    const afw_utf8_t *source_location,
    afw_xctx_t *xctx);


/**
 * @brief Configure environment with list of configuration entries.
 * @param entry_list configuration entry list.
 * @param source_location to associate with this list of entries.
 * @param xctx MUST be base xctx.
 *
 * Parameter entry_list must contain mutable objects and persist for life of environment.
 *
 * Parameter source_location default to "Configuration".
 */
AFW_DECLARE(void)
afw_environment_configure_with_object_list(
    const afw_array_t *entry_list,
    const afw_utf8_t *source_location,
    afw_xctx_t *xctx);



/**
 * @brief Create a readonly object for accessing environment variables.
 * @param preload_variables should be true if using threads or false otherwise.
 * @param xctx of caller.
 * @return object instance.
 * 
 * If not using threads, preload_variables false is better since the property
 * associated with each environment variable is only create if accessed. If
 * threads are being using, make sure preload_variables is true because the
 * object is not threadsafe otherwise. 
 */
AFW_DECLARE(const afw_object_t *)
afw_environment_create_environment_variables_object(
    afw_boolean_t preload_variables,
    afw_xctx_t *xctx);



/**
 * @brief Load and initialize environment extension.
 * @param extension_id id of extension or NULL.
 * @param module_path Path to extension dso or NULL.
 * @param properties object.
 * @param xctx of caller.
 * @param extension interface
 *
 * The properties object can supply extension_id with the "extensionId"
 * property and module_path with the modulePath property. If either is
 * supplied in the object as well as parameters, their values must match.
 * 
 * Either extension_id or module_path must be available.
 * 
 * If module_path is NULL and there is not a "modulePath" property in
 * properties, the extension path must be registered for the extension_id.
 *
 * After the DSO is loaded, the environment extension initialize method
 * is called.
 *
 * Extensions are normally loaded by
 * afw_environment_configure_with_object_list().
 *
 * If properties is not specified, one is created.  The extensionId and
 * modulePath properties are set.  The path of the properties object is
 * set to `/afw/_AdaptiveExtension_/<extension_id>` and set as a run time
 * object.
 */
AFW_DECLARE(const afw_extension_t *)
afw_environment_load_extension(
    const afw_utf8_t *extension_id,
    const afw_utf8_t *module_path,
    const afw_object_t *properties,
    afw_xctx_t *xctx);



/**
 * @brief Callback function typedef for afw_environment_foreach().
 * @param type_number is registry type number.
 * @param data passed to call of afw_environment_foreach().
 * @param key_s is key->s.
 * @param key_len is key->len.
 * @param value to cast as appropriate for type_number.
 * @param p to use.
 * @param xctx of caller.
 * @return true if the callback function requests that it not be called again.
 *
 * The purpose of returning true from a callback function is to short circuit
 * the foreach when the callback has determined that it is "complete".
 * Unless the callback has a special purpose like this, it should always
 * return false so that it will continue to called until the end.
 */
typedef afw_boolean_t (*afw_environment_foreach_cb_t) (
    int type_number,
    void *data,
    const afw_utf8_octet_t *key_s,
    afw_size_t key_len,
    void *value,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Call a callback function for each entry of a registry type.
 * @param type_number is registry type number.
 * @param callback function.
 * @param data to pass to callback.
 * @param p to pass to callback.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_foreach(
    int type_number,
    afw_environment_foreach_cb_t callback,
    void *data,
    const afw_pool_t *p,
    afw_xctx_t *xctx);


/**
 * @brief Create a new registry type.
 * @param registry_type_id name.
 * @param property_name is a unique name for this type associated with key.
 * @param description of registry type.
 * @param object_type_id of objects in afw adapter for entries of this type.
 * @param allow_reregister of a key by replacing value.
 * @param auto_register is function to call if key is not already registered.
 * @param register_additional function pointer or NULL.  Required if
 * @param register_additional_param or NULL.
 * @param xctx of caller.
 * @return Number associated with registry type.
 *
 * Create a new registry type and return its number.
 */
AFW_DECLARE(int)
afw_environment_create_registry_type(
    const afw_utf8_t *registry_type_id,
    const afw_utf8_t *property_name,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *description,
    afw_boolean_t allow_reregister,
    afw_environment_auto_register_cb_t auto_register,
    afw_environment_register_additional_cb_t register_additional,
    void * register_additional_param,
    afw_xctx_t *xctx);


/**
 * @brief Get the registry type associated with a registry type id.
 * @param registry_type_id
 * @param load_extension if needed.
 * @param xctx of caller.
 * @return pointer to afw_environment_registry_type_t.
 *
 * Gets the registry type number assigned to a registry type name.
 */
AFW_DECLARE(const afw_environment_registry_type_t *)
afw_environment_get_registry_type_by_id(
    const afw_utf8_t *registry_type_id,
    afw_boolean_t load_extension,
    afw_xctx_t *xctx);


/**
 * @brief Get the registry type associated with a registry type number.
 * @param registry_type_number
 * @param xctx of caller.
 * @return pointer to afw_environment_registry_type_t.
 *
 * Gets the registry type number assigned to a registry type name.
 */
AFW_DECLARE(const afw_environment_registry_type_t *)
afw_environment_get_registry_type_by_number(
    int registry_type_number,
    afw_xctx_t *xctx);



/**
 * @brief Register a value by key for a registry type.
 * @param type_number registry type number.
 * @param key to associate with value.
 * @param value to associate with key.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_registry_register(
    int type_number,
    const afw_utf8_t *key,
    const void *value,
    afw_xctx_t *xctx);


/**
 * @brief Check to see if a key exists.
 * @param type_number registry type number.
 * @param key associated with value.
 * @param xctx of caller.
 * @return true or false
 */
AFW_DECLARE(afw_boolean_t)
afw_environment_registry_key_exists(
    int type_number,
    const afw_utf8_t *key,
    afw_xctx_t *xctx);


/**
 * @brief Get the value associated with a key for a registry type.
 * @param type_number registry type number.
 * @param key associated with value.
 * @param xctx of caller.
 * @return value associated with key or NULL if not found.
 */
AFW_DECLARE(void *)
afw_environment_registry_get(
    int type_number,
    const afw_utf8_t *key,
    afw_xctx_t *xctx);


/**
 * @brief Limited get the value associated with a key for a registry type.
 * @param env pointer to environment.
 * @param type_number registry type number.
 * @param key associated with value.
 * @return value associated with key or NULL if not found.
 *
 * The is a limited version of afw_environment_registry_get() and should only be
 * used when xctx is not available.  It will return NULL on any error condition
 * and will not check for extensions that might provide support for the
 * type_number or do auto register.
 */
AFW_DECLARE(void *)
afw_environment_registry_get_without_xctx(
    const afw_environment_t *env,
    int type_number,
    const afw_utf8_t *key);



/**
 * @brief Get the registry type associated with property name.
 * @param property_name associated with registry type.
 * @param xctx of caller.
 * @return pointer to afw_environment_registry_type_t.
 *
 * Gets the registry type number assigned to a registry type name.
 */
AFW_DECLARE(const afw_environment_registry_type_t *)
afw_environment_get_registry_type(
    const afw_utf8_t *property_name,
    afw_xctx_t *xctx);



/**
 * @brief Register an singleton.
 * @param singleton_key used to identify the singleton being registered.
 * @param void_ptr void pointer to singleton value.
 * @param xctx of caller.
 *
 * Singletons are useful for associating a name with a major struct or object
 * that needs to be accessed across an AFW application.
 */
AFW_DECLARE(void)
afw_environment_register_singleton(
    const afw_utf8_t *singleton_key,
    const void *void_ptr,
    afw_xctx_t *xctx);


/**
 * @brief Get the singleton value.
 * @param singleton_key used to identify the singleton.
 * @param xctx of caller.
 * @return Pointer to singleton value or NULL if not found.
 */
AFW_DECLARE(void *)
afw_environment_get_singleton(
    const afw_utf8_t *singleton_key, afw_xctx_t *xctx);


/**
 * @brief Register an adapter factory.
 * @param adapter_type used to identify adapter factory being registered.
 * @param adapter_factory instance to register.
 * @param xctx of caller.
 *
 * This also registers flag `trace:adapter_type:<adapter_type>` where
 * `<adapter_type>` is the supplied adapter_type.
 */
AFW_DEFINE(void)
afw_environment_register_adapter_type(
    const afw_utf8_t *adapter_type,
    const afw_adapter_factory_t *adapter_factory,
    afw_xctx_t *xctx);


/**
 * @brief Get the adapter factory instance associated with adapter type.
 * @param adapter_type of adapter factory instance.
 * @param xctx of caller.
 * @return Associated instance or NULL if not found.
 */
AFW_DECLARE(const afw_adapter_factory_t *)
afw_environment_get_adapter_type(
    const afw_utf8_t *adapter_type,
    afw_xctx_t *xctx);


/**
 * @brief Register an adapter id anchor.
 * @param adapter_id being registered.
 * @param anchor instance to register.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_adapter_id(
    const afw_utf8_t *adapter_id,
    const afw_adapter_id_anchor_t *anchor,
    afw_xctx_t *xctx);


/**
 * @brief Get the adapter id anchor associated with adapter id.
 * @param adapter_id
 * @param xctx of caller.
 * @return Associated adapter id anchor or NULL if not found.
 */
AFW_DECLARE(const afw_adapter_id_anchor_t *)
afw_environment_get_adapter_id(
    const afw_utf8_t *adapter_id,
    afw_xctx_t *xctx);


/**
 * @brief Register an authorization handler factory.
 * @param authorization_handler_type used to identify authorization handler
 *    factory being registered.
 * @param authorization_handler_factory instance to register.
 * @param xctx of caller.
 *
 * This also registers flag
 * `trace:authorization_handler_type:<authorization_handler_type>` where
 * `<authorization_handler_type>` is the supplied authorization_handler_type.
 */
AFW_DEFINE(void)
afw_environment_register_authorization_handler_type(
    const afw_utf8_t *authorization_handler_type,
    const afw_authorization_handler_factory_t *authorization_handler_factory,
    afw_xctx_t *xctx);



/**
 * @brief Get the authorization_handler factory instance associated with
 *    authorization handler type.
 * @param authorization_handler_type of authorization handler factory instance.
 * @param xctx of caller.
 * @return Associated instance or NULL if not found.
 */
AFW_DECLARE(const afw_authorization_handler_factory_t *)
afw_environment_get_authorization_handler_type(
    const afw_utf8_t *authorization_handler_type,
    afw_xctx_t *xctx);



/**
 * @brief Register an authorization handler id anchor.
 * @param authorization_handler_id being registered.
 * @param anchor instance to register.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_authorization_handler_id(
    const afw_utf8_t *authorization_handler_id,
    const afw_authorization_handler_id_anchor_t *anchor,
    afw_xctx_t *xctx);



/**
 * @brief Get the authorization handler id anchor associated with
 *    authorization handler id.
 * @param authorization_handler_id
 * @param xctx of caller.
 * @return Associated authorization handler id anchor or NULL if not found.
 */
AFW_DECLARE(const afw_authorization_handler_id_anchor_t *)
afw_environment_get_authorization_handler_id(
    const afw_utf8_t *authorization_handler_id,
    afw_xctx_t *xctx);



/**
 * @brief Create and register a configuration (conf) type.
 * @param conf_type_id of configuration tpe.
 * @param create function to call.
 * @parm id_property_name for instances of this conf type.
 * @parm id_registry_type_id for instances of this conf type.
 * @parm id_runtime_object_type_id for instances of this conf type.
 * @parm subtype_property_name for instances of this conf type.
 * @parm subtype_registry_type_id for instances of this conf type.
 * @parm subtype_runtime_object_type_id for instances of this conf type
 * @param related_object_type_id 
 * @param id_property_name of the id property for this type or NULL.
 * @param subtype_property_name of the subtype property for this type or NULL.
 * @param title of configuration type.
 * @param description of configuration type.
 * @param is_unique indicate this type can only occur once.
 * @param xctx of caller.
 *
 * For adapter, id_property_name is "adapterId' and subtype_property_name
 * is "adapter_type".
 * 
 * If related_object_type_id is not NULL, the uri of the related object
 * will be `/afw/<related_object_type_id>/<id_property_name` value>
 */
AFW_DECLARE(void)
afw_environment_create_and_register_conf_type(
    const afw_utf8_t *conf_type_id,
    afw_environment_conf_type_create_cede_p_t create,
    const afw_utf8_t *title,
    const afw_utf8_t *description,
    const afw_utf8_t *id_property_name,
    const afw_utf8_t *id_registry_type_id,
    const afw_utf8_t *id_runtime_object_type_id,
    const afw_utf8_t *subtype_property_name,
    const afw_utf8_t *subtype_registry_type_id,
    const afw_utf8_t *subtype_runtime_object_type_id,
    afw_boolean_t is_unique,
    afw_xctx_t *xctx);


/**
 * @brief Get the conf_type associated with type.
 * @param type is conf type.
 * @param xctx of caller.
 * @return Associated afw_environment_conf_type_t or NULL if not found.
 */
AFW_DECLARE(const afw_environment_conf_type_t *)
afw_environment_get_conf_type(
    const afw_utf8_t *type,
    afw_xctx_t *xctx);



/**
 * @brief Prepare properties for a conf type.
 * @param properties to prepare.
 * @param xctx of caller.
 * @return processed properties.
 *
 * This function should be called by the create function of the conf_type.  It
 * validates, normalizes, and compiles properties based on the appropriate
 * object type.
 *
 * The p of properties is used for the result.
 *
 * The "type" parameter in properties is used to call
 * afw_environment_get_conf_type() to get the conf_type.
 *
 * The object type used is:
 *
 * `/afw/_AdaptiveObjectType_/_AdaptiveConf_<type>[_<subtype>]`
 *
 * Where:
 *
 * `<type>` is the value of properties.type
 *
 * `[_<subtype>]` is present if conf_type has a subtype_property_name, in which
 *     case, `<subtype>` will be the value of the subtype_property_name in
 *     properties.
 *
 * The returned object will be an entity with a path of:
 *
 * `/afw/<object type>/<id>`
 *
 * Where:
 *
 *  <object type> is object type described above.
 *
 *  `<id>` is the value of the id_property_name for the conf_type from
 *      properties.
 *
 *
 * Several properties in the properties parameter object may be changed plus
 * return in the result:
 *
 * 1) If sourceLocation is not already set in properties, one will be set in
 *   the properties that includes the available type, subtype, and id.
 *
 * 2) If a type expects an id but it is not provided, it will be set to the
 *   subtype in properties.
 *
 * Other than these 2, the result will be a new object that may reuse values
 * from properties, but will have validated, normalized, and compiled values.
 */
AFW_DECLARE(const afw_object_t *)
afw_environment_prepare_conf_type_properties(
    const afw_object_t *properties,
    afw_xctx_t *xctx);



/**
 * @brief Register an content type.
 * @param type content type
 * @param Content type struct.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_content_type(
    const afw_utf8_t *type,
    const afw_content_type_t *content_type,
    afw_xctx_t *xctx);



/**
 * @brief Get the afw_content_type struct associated with a content type.
 * @param type configuration entry type.
 * @param xctx of caller.
 * @return Pointer to associated afw_content_type_t struct or NULL if not found.
 */
AFW_DECLARE(const afw_content_type_t *)
afw_environment_get_content_type(
    const afw_utf8_t *type,
    afw_xctx_t *xctx);



/**
 * @brief Register an context type.
 * @param context_type_id
 * @param context type object.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_context_type(
    const afw_utf8_t *context_type_id,
    const afw_object_t *context_type_object,
    afw_xctx_t *xctx);



/**
 * @brief Get the afw_environment_context_type struct associated with a
 *    context type.
 * @param context_type_id
 * @param xctx of caller.
 * @return context_type_object.
 */
AFW_DECLARE(const afw_object_t *)
afw_environment_get_context_type(
    const afw_utf8_t *context_type_id,
    afw_xctx_t *xctx);



/**
 * @brief Register a data type.
 * @param data_type_id Data type id.
 * @param data_type Data type instance.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_data_type(
    const afw_utf8_t *data_type_id,
    const afw_data_type_t *data_type,
    afw_xctx_t *xctx);



/**
 * @brief Register a NULL terminated list of data types.
 * @param data_types NULL terminated list of afw_data_type_t *.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_data_types(
    const afw_data_type_t * const *data_types,
    afw_xctx_t *xctx);



/**
 * @brief Get the data_type associated with configuration entry
 *    type.
 * @param type configuration entry type.
 * @param xctx of caller.
 * @return Associated instance or NULL if not found.
 */
AFW_DECLARE(const afw_data_type_t *)
afw_environment_get_data_type(
    const afw_utf8_t *type,
    afw_xctx_t *xctx);



/**
 * @brief Register an error rv decoder.
 * @param rv_source_id see rv_source_id_z in afw_error_t.
 * @param rv_decoder function.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_error_rv_decoder(
    const afw_utf8_t *rv_source_id,
    afw_environment_error_rv_decoder_z_t rv_decoder,
    afw_xctx_t *xctx);



/**
 * @brief Get the error rv decoder function associated with rv_source_id.
 * @param rv_source_id see rv_source_id_z in afw_error_t.
 * @param xctx of caller.
 * @return Decoder function or NULL if not found.
 */
AFW_DECLARE(afw_environment_error_rv_decoder_z_t)
afw_environment_get_error_rv_decoder(
    const afw_utf8_t *rv_source_id,
    afw_xctx_t *xctx);



/**
 * @brief Register a flag.
 * @param flag_id
 * @param brief
 * @param description
 * @param included_by_flag_id or NULL (afw_flag_add_included_by() adds more)
 * @param log_priority to be used if this flag triggers a log message.
 * @param xctx of caller.
 */
#define afw_environment_register_flag( \
    flag_id, brief, description, included_by_flag_id, xctx) \
afw_flag_environment_register_flag( \
    flag_id, brief, description, included_by_flag_id, xctx)



/**
 * @brief Get the flag instance associated with flag_id.
 * @param flag_id of flag.
 * @param xctx of caller.
 * @return associated instance or NULL if not found.
 */
AFW_DECLARE(const  afw_flag_t *)
afw_environment_get_flag(
    const afw_utf8_t *flag_id,
    afw_xctx_t *xctx);



/**
 * @brief Register a function.
 * @param function_id used to identify function being registered.
 * @param function instance to register.
 * @param xctx of caller.
 *
 * If function->inf is NULL, a copy of the function will be created with
 * with inf and data_type supplied.
 */
AFW_DECLARE(void)
afw_environment_register_function(
    const afw_utf8_t *function_id,
    const afw_value_function_definition_t *function,
    afw_xctx_t *xctx);



/**
 * @brief Register a NULL terminated list of functions.
 * @param functions NULL terminated list of afw_value_function_definition_t *.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_functions(
    const afw_value_function_definition_t **functions,
    afw_xctx_t *xctx);



/**
 * @brief Register a reusable interned string value.
 * @param string utf8 content (usually `&value->internal`).
 * @param value `const afw_value_string_t` (often a generated permanent).
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_string_literal(
    const afw_utf8_t *string,
    const afw_value_string_t *value,
    afw_xctx_t *xctx);



/**
 * @brief Register a NULL terminated list of reusable interned strings.
 * @param literals NULL terminated list of `const afw_value_string_t *`.
 * @param xctx of caller.
 *
 * Skips empty strings and keys already registered (core, extensions, and
 * commands can share spellings).
 */
AFW_DECLARE(void)
afw_environment_register_string_literals(
    const afw_value_string_t * const *literals,
    afw_xctx_t *xctx);



/**
 * @brief Get a reusable interned string value for utf8 content.
 * @param string content to look up.
 * @param xctx of caller.
 * @return `const afw_value_string_t` or NULL if not found.
 */
AFW_DECLARE(const afw_value_string_t *)
afw_environment_get_string_literal(
    const afw_utf8_t *string,
    afw_xctx_t *xctx);


/**
 * @brief Get the function instance associated with function id.
 * @param function_id of function instance.
 * @param xctx of caller.
 * @return Associated instance or NULL if not found.
 */
AFW_DECLARE(const afw_value_function_definition_t *)
afw_environment_get_function(
    const afw_utf8_t *function_id,
    afw_xctx_t *xctx);


/**
 * @brief Get the qualified function instance.
 * @param qualifier or NULL.
 * @param name of function/method.
 * @param xctx of caller.
 * @return Associated instance or NULL if not found.
 */
AFW_DECLARE(const afw_value_function_definition_t *)
afw_environment_get_qualified_function(
    const afw_utf8_t *qualifier,
    const afw_utf8_t *name,
    afw_xctx_t *xctx);


/**
 * @brief Get the function associated with a data type method.
 * @param data_type to check.
 * @param dataTypeMethodNumber to retrieve.
 * @param xctx of caller.
 * @return Associated function instance or NULL if not found.
 */
AFW_DECLARE(const afw_value_function_definition_t *)
afw_environment_registry_get_data_type_method(
    const afw_data_type_t * data_type,
    afw_integer_t dataTypeMethodNumber,
    afw_xctx_t *xctx);


/**
 * @brief Register an lock.
 * @param lock_id of the lock.
 * @param lock instance to register.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_lock(
    const afw_utf8_t *lock_id,
    const afw_lock_t *lock,
    afw_xctx_t *xctx);


/**
 * @brief Get the lock instance associated a lock id.
 * @param lock_id of the lock.
 * @param xctx of caller.
 * @return Associated lock or NULL if not found.
 */
AFW_DECLARE(const afw_lock_t *)
afw_environment_get_lock(
    const afw_utf8_t *log_type,
    afw_xctx_t *xctx);


/**
 * @brief Register an log factory.
 * @param log_type used to identify log factory being registered.
 * @param log_factory instance to register.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_log_type(
    const afw_utf8_t *log_type,
    const afw_log_factory_t *log_factory,
    afw_xctx_t *xctx);



/**
 * @brief Get the log factory instance associated with log type.
 * @param log_type of log factory instance.
 * @param xctx of caller.
 * @return Associated instance or NULL if not found.
 */
AFW_DECLARE(const afw_log_factory_t *)
afw_environment_get_log_type(
    const afw_utf8_t *log_type,
    afw_xctx_t *xctx);


/**
 * @brief Register a log.
 * @param log_id used to identify log being registered.
 * @param log instance to register.
 * @param xctx of caller.
 *
 * If you want this log to be called for all environment log requests,
 * call afw_log_add_to_environment() instead.
 */
AFW_DECLARE(void)
afw_environment_register_log(
    const afw_utf8_t *log_id,
    const afw_log_t *log,
    afw_xctx_t *xctx);



/**
 * @brief Get the log instance associated with log id.
 * @param log_id of log instance.
 * @param xctx of caller.
 * @return Associated instance or NULL if not found.
 */
AFW_DECLARE(const afw_log_t *)
afw_environment_get_log(
    const afw_utf8_t *log_id,
    afw_xctx_t *xctx);



/**
 * @brief Register a policy combining algorithm function.
 * @param policy_combining_algorithm_id used to identify algorithm.
 * @param function struct to register.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_policy_combining_algorithm(
    const afw_utf8_t *policy_combining_algorithm_id,
    const afw_value_function_definition_t *function,
    afw_xctx_t *xctx);



/**
 * @brief Get the function instance associated with a policy combining
 *    algorithm id.
 * @param policy_combining_algorithm_id of function instance.
 * @param xctx of caller.
 * @return Associated struct or NULL if not found.
 */
AFW_DECLARE(const afw_value_function_definition_t *)
afw_environment_get_policy_combining_algorithm(
    const afw_utf8_t *policy_combining_algorithm_id,
    afw_xctx_t *xctx);



/**
 * @brief Register a request_handler factory.
 * @param handler_type used to identify request_handler factory being
 *    registered.
 * @param request_handler_factory instance to register.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_request_handler_type(
    const afw_utf8_t *handler_type,
    const afw_request_handler_factory_t *request_handler_factory,
    afw_xctx_t *xctx);



/**
 * @brief Get the request_handler factory instance associated with
 *    handler type.
 * @param request_handler_type of request_handler factory instance.
 * @param xctx of caller.
 * @return Associated instance or NULL if not found.
 */
AFW_DECLARE(const afw_request_handler_factory_t *)
afw_environment_get_request_handler_type(
    const afw_utf8_t *handler_type,
    afw_xctx_t *xctx);


/**
 * @brief Register a rule combining algorithm function.
 * @param rule_combining_algorithm_id used to identify algorithm.
 * @param function struct to register.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_rule_combining_algorithm(
    const afw_utf8_t *rule_combining_algorithm_id,
    const afw_value_function_definition_t *function,
    afw_xctx_t *xctx);



/**
 * @brief Get the function instance associated with a rule combining
 *    algorithm id.
 * @param rule_combining_algorithm_id of function instance.
 * @param xctx of caller.
 * @return Associated struct or NULL if not found.
 */
AFW_DECLARE(const afw_value_function_definition_t *)
afw_environment_get_rule_combining_algorithm(
    const afw_utf8_t *rule_combining_algorithm_id,
    afw_xctx_t *xctx);



/**
 * @brief Register afw_runtime_custom_t for a custom handled runtime object
 *    type.
 * @param object_type_id of the afw object type being registered.
 * @param custom handling information.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_runtime_custom(
    const afw_utf8_t *object_type_id,
    const afw_runtime_custom_t *custom,
    afw_xctx_t *xctx);


/**
 * @brief Get afw_runtime_custom_t for a custom handled runtime object type.
 * @param object_type_id of the afw object type.
 * @param xctx of caller.
 * @return Associated afw_runtime_custom_t pointer or NULL if not found.
 */
AFW_DECLARE(const afw_runtime_custom_t *)
afw_environment_get_runtime_custom(
    const afw_utf8_t *object_type_id,
    afw_xctx_t *xctx);


/**
 * @brief Register a runtime object map interface.
 * @param object_type_id used to identify interface being registered.
 * @param inf pointer associated with object map.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_runtime_object_map_inf(
    const afw_utf8_t *object_type_id,
    const afw_object_inf_t *inf,
    afw_xctx_t *xctx);


/**
 * @brief Get the interface associated with a runtime object map.
 * @param object_type_id used to identify interface.
 * @param xctx of caller.
 * @return Associated interface pointer or NULL if not found.
 */
AFW_DECLARE(const afw_object_inf_t *)
afw_environment_get_runtime_object_map_inf(
    const afw_utf8_t *object_type_id,
    afw_xctx_t *xctx);


/**
 * @brief Register a runtime value accessor.
 * @param info permanent (or env-pool) accessor info including key and function.
 * @param xctx of caller.
 *
 * The registry value is `info`. Adaptive objects of type
 * `_AdaptiveRuntimeValueAccessor_` map that struct. C callers still use
 * afw_environment_get_runtime_value_accessor() to obtain the function.
 *
 * Define `info` next to the accessor implementation so brief, description,
 * and lifetime flags stay with the code that must honor them.
 */
AFW_DECLARE(void)
afw_environment_register_runtime_value_accessor(
    const afw_runtime_value_accessor_info_t *info,
    afw_xctx_t *xctx);


/**
 * @brief Get the value accessor function for a registered name.
 * @param accessor_name used to identify accessor.
 * @param xctx of caller.
 * @return Associated function or NULL if not registered.
 */
AFW_DECLARE(afw_runtime_value_accessor_t)
afw_environment_get_runtime_value_accessor(
    const afw_utf8_t *accessor_name,
    afw_xctx_t *xctx);


/**
 * @brief Get the full registered value accessor info.
 * @param accessor_name used to identify accessor.
 * @param xctx of caller.
 * @return Associated info or NULL if not registered.
 */
AFW_DECLARE(const afw_runtime_value_accessor_info_t *)
afw_environment_get_runtime_value_accessor_info(
    const afw_utf8_t *accessor_name,
    afw_xctx_t *xctx);



/**
 * @brief Register a service.
 * @param service_id used to identify service being registered.
 * @param service instance.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_service(
    const afw_utf8_t *service_id,
    afw_service_t *service,
    afw_xctx_t *xctx);


/**
 * @brief Get the service instance associated with service_id.
 * @param service_id of service instance.
 * @param xctx of caller.
 * @return associated instance or NULL if not found.
 */
AFW_DECLARE(afw_service_t *)
afw_environment_get_service(
    const afw_utf8_t *service_id,
    afw_xctx_t *xctx);



/**
 * @brief Register a service type.
 * @param service_type_id used to identify service type being registered.
 * @param service_type instance.
 * @param xctx of caller.
 */
AFW_DECLARE(void)
afw_environment_register_service_type(
    const afw_utf8_t *service_type_id,
    const afw_service_type_t *service_type,
    afw_xctx_t *xctx);




/**
 * @brief Get the service type instance associated with service_type_id.
 * @param service_type_id of service type instance.
 * @param xctx of caller.
 * @return associated instance or NULL if not found.
 */
AFW_DECLARE(const afw_service_type_t *)
afw_environment_get_service_type(
    const afw_utf8_t *service_type_id,
    afw_xctx_t *xctx);



/**
 * @brief Register a value inf.
 * @param value_inf_id used to identify the value inf being registered.
 * @param value_inf pointer to value inf.
 * @param xctx of caller.
 *
 * value_inf_id should be the same as inf->rti->implementation_id
 */
AFW_DECLARE(void)
afw_environment_register_value_inf(
    const afw_utf8_t *value_inf_id,
    const afw_value_inf_t *value_inf,
    afw_xctx_t *xctx);


/**
 * @brief Get the value inf associated with a value inf id.
 * @param value_inf_id of interface implementation.
 * @param xctx of caller.
 * @return Associated interface pointer or NULL if not found.
 */
AFW_DECLARE(const afw_value_inf_t *)
afw_environment_get_value_inf(
    const afw_utf8_t *value_inf_id,
    afw_xctx_t *xctx);


/**
 * @brief Override fd used for debug.
 * @param fd to use.
 * @param xctx of caller.
 * 
 * If not set, stderr is used.
 */
AFW_DECLARE(void)
afw_environment_set_debug_fd(FILE *fd, afw_xctx_t *xctx);


/**
 * @brief Override fd used for stderr.
 * @param fd to use.
 * @param xctx of caller.
 * 
 * If not set, stderr is used.
 */
AFW_DECLARE(void)
afw_environment_set_stderr_fd(FILE *fd, afw_xctx_t *xctx);


/**
 * @brief Override fd used for stdout.
 * @param fd to use.
 * @param xctx of caller.
 * 
 * If not set, stdout is used.
 */
AFW_DECLARE(void)
afw_environment_set_stdout_fd(FILE *fd, afw_xctx_t *xctx);

AFW_END_DECLARES

/** @} */  // end of @addtogroup @addtogroup

#endif /* __AFW_ENVIRONMENT_H__ */
