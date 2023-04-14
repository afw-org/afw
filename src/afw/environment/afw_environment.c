// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Environment
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_environment.c
 * @brief Adaptive framework environment.
 */

#include "afw_internal.h"
#include "afw_config.h"
#include <apr_hash.h>
#include <apr_dso.h>
#include <libxml/xmlversion.h>


typedef struct {
    const afw_utf8_t *type;
    const afw_utf8_t *key;
} impl_check_manifest_cb_context_t;


static const afw_utf8_t
impl_default_name =
AFW_UTF8_LITERAL("afw_application");

static const afw_error_t
impl_early_error =
{
    NULL,
    afw_error_code_general,
    AFW__FILE_LINE__,
    "Early error creating environment",
    0
};


static afw_boolean_t
impl_check_manifest_cb(
    const afw_object_t *object,
    void *context,
    afw_xctx_t *xctx);



/* Create default runtime object that just has key. */
static void
impl_internal_additional_register_default(
    const afw_utf8_t *type_id,
    int type_number,
    const afw_utf8_t *key,
    const void *value,
    const void *register_additional_param,
    const void **register_additional_use,
    afw_xctx_t *xctx)
{
    afw_environment_internal_t *env;
    afw_environment_registry_type_t *type;

    env = (afw_environment_internal_t *)xctx->env;

    AFW_LOCK_BEGIN(xctx->env->environment_lock) {
        type = apr_hash_get(env->registry_names_ht, type_id->s, type_id->len);
        if (!type) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Invalid environment registry type %" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(type_id));
        }
    }
    AFW_LOCK_END;

    /* Create runtime object for the value. */
    if (value) {
        afw_runtime_env_create_and_set_indirect_object(
            type->object_type_id, key,
            (void *)value, true, xctx);
    }
    else {
        afw_runtime_remove_object(type->object_type_id,
            key, xctx);
    }
}



/* Create runtime object and context type for log type. */
static void
impl_internal_additional_register_log_type(
    const afw_utf8_t *type_id,
    int type_number,
    const afw_utf8_t *key,
    const void *value,
    const void *register_additional_param,
    const void **register_additional_use,
    afw_xctx_t *xctx)
{
    impl_internal_additional_register_default(type_id, type_number,
        key, value,
        register_additional_param, register_additional_use, xctx);

    afw_log_internal_register_logType_context_type(key, xctx);
}



/* Create default runtime object that just has key. */
static void
impl_internal_additional_register_key_only(
    const afw_utf8_t *type_id,
    int type_number,
    const afw_utf8_t *key,
    const void *value,
    const void *register_additional_param,
    const void **register_additional_use,
    afw_xctx_t *xctx)
{
    const afw_utf8_t * *ps;
    afw_environment_internal_t *env;
    afw_environment_registry_type_t *type;

    env = (afw_environment_internal_t *)xctx->env;

    AFW_LOCK_BEGIN(xctx->env->environment_lock) {
        type = apr_hash_get(env->registry_names_ht, type_id->s, type_id->len);
        if (!type) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Invalid environment registry type %" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(type_id));
        }

        if (value) {
            ps = afw_pool_calloc(xctx->env->p, sizeof(afw_utf8_t *), xctx);
            *ps = key;
            afw_runtime_env_create_and_set_indirect_object(
                type->object_type_id, key, (void *)ps, true, xctx);
        }
        else {
            afw_runtime_remove_object(type->object_type_id, key, xctx);
        }
    }
    AFW_LOCK_END;
}



/* Register /afw/ object as well. */
static void
impl_internal_additional_register_object(
    const afw_utf8_t *type_id,
    int type_number,
    const afw_utf8_t *key,
    const void *value,
    const void *register_additional_param,
    const void **register_additional_use,
    afw_xctx_t *xctx)
{
    afw_environment_internal_t *env;
    afw_environment_registry_type_t *type;

    env = (afw_environment_internal_t *)xctx->env;

    AFW_LOCK_BEGIN(xctx->env->environment_lock) {
        type = apr_hash_get(env->registry_names_ht, type_id->s, type_id->len);
        if (!type) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Invalid environment registry type %" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(type_id));
        }

        if (value) {
            afw_runtime_env_set_object((const afw_object_t *)value, true, xctx);
        }
        else {
            afw_runtime_remove_object(type->object_type_id, key, xctx);
        }
    }
    AFW_LOCK_END;
}



typedef struct {
    const afw_utf8_t registry_type_id;
    const afw_utf8_t property_name;
    const afw_utf8_t object_type_id;
    const afw_utf8_t description;
    afw_environment_register_additional_cb_t register_additional;
    afw_boolean_t allow_reregister;
} impl_registry_type_entry_t;


/*
 * Initial registry types. See afw_environemnt_registry_type_t. 
 *
 * IMPORTANT: This must correspond 1 for 1 with
 *    afw_environment_registry_type_enum_t.
 */
static impl_registry_type_entry_t
impl_initial_types[] = {
#define XX(id, register_additional, allow_reregister, property_name, \
    object_type_id, description) \
    {   \
        AFW_UTF8_LITERAL(AFW_STRINGIFY(id)), \
        AFW_UTF8_LITERAL(AFW_STRINGIFY(id)), \
        AFW_UTF8_LITERAL(object_type_id), \
        AFW_UTF8_LITERAL(description), \
        register_additional, \
        allow_reregister \
    },
    AFW_ENVIRONMENT_REGISTRY_TYPE_MAP(XX)
#undef XX
    { { NULL } }
};


/* Create the Adaptive Framework core environment and return base xctx. */
AFW_DEFINE(afw_xctx_t *)
afw_environment_create(
    unsigned int version,
    int argc, const char * const *argv,
    const afw_error_t **environment_create_error)
{
    apr_status_t rv;
    afw_xctx_t *xctx;
    const afw_pool_t *p;
    afw_utf8_t *name;
    afw_environment_internal_t *env;
    int i;
    afw_environment_registry_type_t *type;
    const afw_utf8_z_t *s;
    afw_error_t *error;
    afw_try_t unhandled_error;

    /* Check and initialize libxml2 */
    LIBXML_TEST_VERSION

    /* Initialize apr. */
    rv = apr_initialize();
    if (rv != APR_SUCCESS) {
        *environment_create_error = &impl_early_error;
        return NULL;
    }

    /* Create base pool. */
    p = afw_pool_internal_create_base_pool();
    if (!p) goto early_error;

    /* Allocate cleared afw_error_t. */
    error = apr_pcalloc(afw_pool_get_apr_pool(p), sizeof(afw_error_t));
    if (!error) {
        *environment_create_error = &impl_early_error;
        goto early_error;
    }

    /* Temporary way to handle errors. */
    AFW_ERROR_INTERNAL_ON_UNHANDLED(unhandled_error) {
        *environment_create_error = error;
        //! @fixme Commented out apr_terminate() since it free's error memory.
        //apr_terminate();
        return NULL;
    }

    /* Check version caller is complied against. */
    s = afw_version_check(version);
    if (s) {
        AFW_THROW_UNHANDLED_ERROR(&unhandled_error, error, general, na, 0, s);
    }

    /* Allocate memory for env. */
    env = apr_pcalloc(afw_pool_get_apr_pool(p),
        sizeof(afw_environment_internal_t));
    if (!env) {
        AFW_THROW_UNHANDLED_ERROR(&unhandled_error, error, general, na, 0,
            "apr_pcalloc() failed");
    };
    env->pub.p = p;
    env->pub.pool_number = 1; /* see afw_pool_internal_create_base_pool() */
    env->pub.evaluation_stack_initial_count =
        AFW_ENVIRONMENT_DEFAULT_EVALUATION_STACK_INITIAL_COUNT;
    env->pub.evaluation_stack_maximum_count =
        AFW_ENVIRONMENT_DEFAULT_EVALUATION_STACK_MAXIMUM_COUNT;
    env->pub.debug_fd = stderr;
    env->pub.stderr_fd = stderr;
    env->pub.stdout_fd = stdout;

    /* Create and initialize base xctx. */
    xctx = afw_xctx_internal_create_initialize(&unhandled_error,
        error, env, p);
    xctx->mode = afw_authorization_mode_id_core_value;
    env->base_xctx = xctx;

    /* >>>>>>>>> Errors can be thrown at this point. <<<<<<<<< */

    /* Create data type method number hash table. */
    env->data_type_method_number_ht = apr_hash_make(afw_pool_get_apr_pool(p));

    /* Create data type array for method arrays. */
    env->data_type_methods = apr_array_make(afw_pool_get_apr_pool(p),
        40, sizeof(apr_array_header_t *));

    /* Set xctx name to program name or default. */
    if (argc > 0) {
        name = afw_xctx_calloc_type(afw_utf8_t, xctx);
        /* Name is after last forward or back slash. */
        for (s = name->s = argv[0]; *s; s++) {
            if (*s == '/' || *s == '\\') {
                name->s = s + 1;
            }
        }
        name->len = strlen(name->s);
        xctx->name = name;
    }
    else {
        xctx->name = &impl_default_name;
    }
    env->pub.program_name.s = xctx->name->s;
    env->pub.program_name.len = xctx->name->len;

    /* Application id */
    env->pub.application_id.s = xctx->name->s;
    env->pub.application_id.len = xctx->name->len;

    /* Finish up xctx creation. */
    afw_xctx_internal_create_finishup(xctx);

    env->pub.log = afw_log_internal_create_environment_log(xctx);

    env->registry_names_ht = apr_hash_make(afw_pool_get_apr_pool(p));
    if (!env->registry_names_ht) AFW_THROW_MEMORY_ERROR(xctx);

    env->registry_types =
        apr_array_make(afw_pool_get_apr_pool(p), 20,
            sizeof(afw_environment_registry_type_t *));
    if (!env->registry_types) AFW_THROW_MEMORY_ERROR(xctx);

    /* Create core registry types. */
    for (i = 0; i < afw_environemnt_registry_type_max_core_type; i++) {
        type = afw_pool_calloc_type(p,
            afw_environment_registry_type_t, xctx);
        APR_ARRAY_PUSH(env->registry_types,
            afw_environment_registry_type_t *) = type;
        type->registry_type_id = &impl_initial_types[i].registry_type_id;
        type->property_name = &impl_initial_types[i].property_name;
        type->object_type_id = &impl_initial_types[i].object_type_id;
        type->description = &impl_initial_types[i].description;
        type->number = i;
        type->register_additional = impl_initial_types[i].register_additional;
        type->allow_reregister = impl_initial_types[i].allow_reregister;
        type->auto_register = NULL;
        type->ht = apr_hash_make(afw_pool_get_apr_pool(p));
        if (!type->ht) AFW_THROW_MEMORY_ERROR(xctx);
        apr_hash_set(env->registry_names_ht,
            type->registry_type_id->s, type->registry_type_id->len, type);
    }

    /* Register core with new xctx. */
    afw_environment_internal_register_core(xctx);

    /* Register registry type objects and property names. */
    for (i = 0; i < env->registry_types->nelts; i++) {
        type = APR_ARRAY_IDX(env->registry_types, i,
            afw_environment_registry_type_t *);
        afw_runtime_env_create_and_set_indirect_object_using_inf(
            &afw_runtime_inf__AdaptiveEnvironmentRegistryType_,
            type->registry_type_id, type, true, xctx);
        afw_environment_registry_register(
            afw_environemnt_registry_type_registry_type,
            type->property_name, (void *)type, xctx);
    }

    /* Create multithreaded pool lock. */
    env->pub.multithreaded_pool_lock =
        afw_lock_create_environment_lock(
            &afw_s_a_lock_multithreaded_pool, p, xctx);

    /* Create environment lock. */
    env->pub.environment_lock =
        afw_lock_create_and_register(
            &afw_s_a_lock_environment,
            &afw_s_a_lock_environment_brief,
            &afw_s_a_lock_environment_description,
            true, xctx);

    /* Create adaptor id anchors lock. */
    env->pub.adaptor_id_anchor_lock =
        afw_lock_create_and_register(
            &afw_s_a_lock_adaptor_id_anchor,
            &afw_s_a_lock_adaptor_id_anchor_brief,
            &afw_s_a_lock_adaptor_id_anchor_description,
            true, xctx);

    /* Create authorization handler id anchors rw lock. */
    env->pub.authorization_handler_id_anchor_rw_lock =
        afw_lock_create_rw_and_register(
            &afw_s_a_lock_authorization_handler_id_anchor,
            &afw_s_a_lock_authorization_handler_id_anchor_brief,
            &afw_s_a_lock_authorization_handler_id_anchor_description,
            xctx);

    /* Create active log list lock. */
    env->pub.active_log_list_lock =
        afw_lock_create_and_register(
            &afw_s_a_lock_log_list,
            &afw_s_a_lock_log_list_brief,
            &afw_s_a_lock_log_list_description,
            true, xctx);

    /* Create flags lock. */
    env->pub.flags_lock  =
        afw_lock_create_and_register(
            &afw_s_a_lock_flags,
            &afw_s_a_lock_flags_brief,
            &afw_s_a_lock_flags_description,
            true, xctx);

    /*
     * Adaptive Framework app should have a AFW_TRY right after call to create
     * environment.  If it doesn't and error occurs, terminate the environment
     * will be release and the application will abort.
     */
    AFW_ERROR_INTERNAL_ON_UNHANDLED(unhandled_error) {
        apr_terminate();
        abort();
    }

    /* Set error to NULL and return new xctx. */
    *environment_create_error = NULL;
    return xctx;

early_error:
    apr_terminate();
    return NULL;
}


/* Release resources associated with Adaptive Framework environment. */
AFW_DEFINE(void)
afw_environment_release(afw_xctx_t *xctx)
{
    //! @fixme Causing exception because some things not cleaned up properly
    //! \fixmeafw_pool_destroy(xctx->env->p, xctx);
    //! \fixmeapr_terminate();
}

/* Create a new registry type. */
AFW_DEFINE(int)
afw_environment_create_registry_type(
    const afw_utf8_t *registry_type_id,
    const afw_utf8_t *property_name,
    const afw_utf8_t *object_type_id,
    const afw_utf8_t *description,
    afw_boolean_t allow_reregister,
    afw_environment_auto_register_cb_t auto_register,
    afw_environment_register_additional_cb_t register_additional,
    void * register_additional_param,
    afw_xctx_t *xctx)
{
    afw_environment_registry_type_t *type;
    afw_environment_internal_t *env;

    env = (afw_environment_internal_t *)xctx->env;

    AFW_LOCK_BEGIN(xctx->env->environment_lock) {
        type = apr_hash_get(env->registry_names_ht, registry_type_id->s,
            registry_type_id->len);
        if (type) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "registry_type \"%" AFW_UTF8_FMT "\" is already assigned",
                AFW_UTF8_FMT_ARG(registry_type_id));
        }

        /** @fixme Need to lock/protect this update. */
        /* Create new registry type. */
        type = afw_pool_calloc_type(xctx->env->p,
            afw_environment_registry_type_t, xctx);
        APR_ARRAY_PUSH(env->registry_types,
            afw_environment_registry_type_t *) = type;
        type->registry_type_id = afw_utf8_clone(registry_type_id,
            xctx->env->p, xctx);
        type->property_name = afw_utf8_clone(property_name,
            xctx->env->p, xctx);
        type->object_type_id = afw_utf8_clone(object_type_id,
            xctx->env->p, xctx);
        type->description = afw_utf8_clone(description, xctx->env->p, xctx);
        type->number = env->registry_types->nelts - 1;
        type->ht = apr_hash_make(afw_pool_get_apr_pool(xctx->env->p));
        type->allow_reregister = allow_reregister;
        type->auto_register = auto_register;
        type->auto_register_specified = auto_register != NULL;
        type->register_additional = register_additional;
        type->register_additional_param = register_additional_param;
        apr_hash_set(env->registry_names_ht,
            type->registry_type_id->s, type->registry_type_id->len, type);
        afw_runtime_env_create_and_set_indirect_object_using_inf(
            &afw_runtime_inf__AdaptiveEnvironmentRegistryType_,
            type->registry_type_id, type, true, xctx);
        afw_environment_registry_register(
            afw_environemnt_registry_type_registry_type,
            type->property_name, (void *)type, xctx);
    }
    AFW_LOCK_END;

    return type->number;
}


/* Get the registry type associated with a registry type id. */
AFW_DECLARE(const afw_environment_registry_type_t *)
afw_environment_get_registry_type_by_id(
    const afw_utf8_t *registry_type_id,
    afw_boolean_t load_extension,
    afw_xctx_t *xctx)
{
    afw_environment_registry_type_t *type;
    afw_environment_internal_t *env;
    impl_check_manifest_cb_context_t ctx;

    env = (afw_environment_internal_t *)xctx->env;

    AFW_LOCK_BEGIN(xctx->env->environment_lock) {
        type = apr_hash_get(
            env->registry_names_ht,
            registry_type_id->s,
            registry_type_id->len);

        if (!type && load_extension) {
            ctx.type = &afw_s_registry_type;
            ctx.key = registry_type_id;
            afw_runtime_foreach(&afw_s__AdaptiveManifest_,
                &ctx, impl_check_manifest_cb, xctx);
            type = apr_hash_get(
                env->registry_names_ht,
                registry_type_id->s,
                registry_type_id->len);
        }
    }
    AFW_LOCK_END;

    return type;
}


/* Get the registry type name associated with a registry type number. */
AFW_DEFINE(const afw_environment_registry_type_t *)
afw_environment_get_registry_type_by_number(
    int registry_type_number,
    afw_xctx_t *xctx)
{
    afw_environment_internal_t *env;
    afw_environment_registry_type_t *type;

    env = (afw_environment_internal_t *)xctx->env;

    if (registry_type_number >= env->registry_types->nelts) {
        return NULL;
    }

    AFW_LOCK_BEGIN(xctx->env->environment_lock) {
        type = ((afw_environment_registry_type_t **)env->registry_types->elts)
            [registry_type_number];
    }
    AFW_LOCK_END;

    return type;
}


/* Register a value by key for a registry type. */
AFW_DEFINE(void)
afw_environment_registry_register(
    int type_number,
    const afw_utf8_t *key,
    const void *value,
    afw_xctx_t *xctx)
{
    afw_environment_registry_type_t *type;
    const void *old_value;
    afw_utf8_octet_t *new_key;
    const afw_utf8_octet_t *use_key;
    afw_environment_internal_t *env;

    env = (afw_environment_internal_t *)xctx->env;

    AFW_LOCK_BEGIN(xctx->env->environment_lock) {

        if (type_number >= env->registry_types->nelts) {
            AFW_THROW_ERROR_FZ(general, xctx, "Invalid type_number %d.",
                type_number);
        }
        type = ((afw_environment_registry_type_t **)env->registry_types->elts)
            [type_number];

        use_key = key->s;
        old_value = apr_hash_get(type->ht, key->s, key->len);
        if (old_value && !type->allow_reregister) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "%" AFW_UTF8_FMT " %" AFW_UTF8_FMT " is already registered",
                AFW_UTF8_FMT_ARG(
                    &impl_initial_types[type_number].registry_type_id),
                AFW_UTF8_FMT_ARG(key));
        }
        /** @fixme Small leak if deleting key and then adding again. */
        if (value && !old_value) {
            new_key = afw_pool_calloc(xctx->env->p, key->len, xctx);
            memcpy(new_key, key->s, key->len);
            use_key = new_key;
        }

        apr_hash_set(type->ht, use_key, key->len, value);

        if (type->register_additional) {
            type->register_additional(
                type->registry_type_id, type->number, key, value,
                type->register_additional_param, &type->register_additional_use,
                xctx);
        }
    }
    AFW_LOCK_END;
};



static afw_boolean_t
impl_check_manifest_cb(
    const afw_object_t *object,
    void *context,
    afw_xctx_t *xctx)
{
    const afw_value_t *registers_value;
    const afw_utf8_t *extension_id;
    const afw_utf8_t *module_path;
    afw_utf8_t registry_type_id;
    afw_utf8_t registry_key;
    const afw_utf8_t *entry;
    const afw_iterator_t *iterator;
    const afw_list_t *list;
    impl_check_manifest_cb_context_t *ctx = context;

    if (!object) {
        return false;
    }

    registers_value = afw_object_get_property(object, &afw_s_registers, xctx);
    if (!registers_value) {
        return false;
    }

    if (!afw_value_is_list_of_string(registers_value) &&
        !afw_value_is_list_of_anyURI(registers_value))
    {
        return false;
    }

    /* Loop through entries. */
    list = ((const afw_value_list_t *)registers_value)->internal;
    for (iterator = NULL;;)
    {
        afw_list_get_next_internal(list, &iterator, NULL,
            (const void **)&entry, xctx);
        if (!entry) {
            break;
        }

        /*
         * Separate <registry type id>/<registry key> from entry and load
         * extension if match.
         */
        for (
            registry_type_id.s = entry->s,
            registry_key.s = entry->s,
            registry_key.len = entry->len;
            registry_key.len > 0 && *registry_key.s != '/';
            registry_key.len--, registry_key.s++);

        if (registry_key.len > 0)
        {
            registry_type_id.len = registry_key.s - registry_type_id.s;
            registry_key.s++;
            registry_key.len--;
            if (afw_utf8_equal(ctx->type, &registry_type_id) &&
                afw_utf8_equal(ctx->key, &registry_key))
            {
                extension_id = afw_object_old_get_property_as_string(object,
                    &afw_s_extensionId, xctx);
                module_path = afw_object_old_get_property_as_string(object,
                    &afw_s_modulePath, xctx);
                if (extension_id && module_path) {
                    afw_environment_load_extension(extension_id, module_path,
                        NULL, xctx);
                    /*Return indicating complete. */
                    return true;
                }
            }
        }
    }

    /* Return indicating not to short circuit */
    return false;
}



/* Check to see if a key exists. */
AFW_DEFINE(afw_boolean_t)
afw_environment_registry_key_exists(
    int type_number,
    const afw_utf8_t *key,
    afw_xctx_t *xctx)
{
    afw_environment_registry_type_t *type;
    afw_boolean_t result;
    afw_environment_internal_t *env;

    env = (afw_environment_internal_t *)xctx->env;

    AFW_LOCK_BEGIN(xctx->env->environment_lock) {
        if (type_number < 0 ||
            type_number >= env->registry_types->nelts)
        {
            AFW_THROW_ERROR_FZ(general, xctx, "Invalid type_number.",
                type_number);
        }

        type = ((afw_environment_registry_type_t **)env->registry_types->elts)
            [type_number];

        result = (apr_hash_get(type->ht, key->s, key->len) != NULL);
    }
    AFW_LOCK_END;

    return result;
}



/* Get the value associated with a key for a registry type. */
AFW_DEFINE(void *)
afw_environment_registry_get(
    int type_number,
    const afw_utf8_t *key,
    afw_xctx_t *xctx)
{
    afw_environment_registry_type_t *type;
    void *result;
    afw_environment_internal_t *env;
    impl_check_manifest_cb_context_t ctx;

    env = (afw_environment_internal_t *)xctx->env;

    AFW_LOCK_BEGIN(xctx->env->environment_lock) {

        if (type_number < 0 ||
            type_number >= env->registry_types->nelts)
        {
            AFW_THROW_ERROR_FZ(general, xctx, "Invalid type_number.",
                type_number);
        }

        type = ((afw_environment_registry_type_t **)env->registry_types->elts)
            [type_number];

        result = apr_hash_get(type->ht, key->s, key->len);

        /*
         * If no result yet, see if there is an extension manifest that provides
         * it, load the extension.
         */
        if (!result) {
            ctx.type = type->registry_type_id;
            ctx.key = key;
            afw_runtime_foreach(&afw_s__AdaptiveManifest_,
                &ctx, impl_check_manifest_cb, xctx);
            result = apr_hash_get(type->ht, key->s, key->len);
        }

        /* If still no result and auto_register, do auto register. */
        if (!result && type->auto_register) {
            result = type->auto_register(type->registry_type_id, key, xctx);
            afw_environment_registry_register(type_number, key, result, xctx);
        }

    }
    AFW_LOCK_END;

    return result;
};


/* Limited get the value associated with a key for a registry type. */
AFW_DEFINE(void *)
afw_environment_registry_get_xctxless(
    const afw_environment_t *env,
    int type_number,
    const afw_utf8_t *key)
{
    afw_environment_registry_type_t *type;
    void *result;
    afw_environment_internal_t *self;

    self = (afw_environment_internal_t *)env;

    if (type_number < 0 ||
        type_number >= self->registry_types->nelts)
    {
        return NULL;
    }

    type = ((afw_environment_registry_type_t **)self->registry_types->elts)
        [type_number];

    result = apr_hash_get(type->ht, key->s, key->len);

    return result;
}



/* Call a callback function for each entry of a registry type. */
AFW_DEFINE(void)
afw_environment_foreach(
    int type_number,
    afw_environment_foreach_cb_t callback,
    void *data,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_environment_registry_type_t *type;
    afw_environment_internal_t *env;
    apr_hash_index_t *hi;
    void *value;
    afw_utf8_octet_t *key_s;
    apr_ssize_t key_len;

    env = (afw_environment_internal_t *)xctx->env;

    if (type_number >= env->registry_types->nelts) {
        AFW_THROW_ERROR_FZ(general, xctx, "Invalid type_number.",
            type_number);
    }

    AFW_LOCK_BEGIN(xctx->env->environment_lock) {
        type = ((afw_environment_registry_type_t **)env->registry_types->elts)
            [type_number];

        for (hi = apr_hash_first(afw_pool_get_apr_pool(p), type->ht);
            hi;
            hi = apr_hash_next(hi))
        {
            apr_hash_this(hi, (const void **)&key_s, &key_len, &value);
            if (callback(type_number, data, key_s, key_len, value, p, xctx)) {
                break;
            }
        }
    }
    AFW_LOCK_END;
}




/* Create and register a configuration type. */
AFW_DEFINE(void)
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
    afw_xctx_t *xctx)
{
    afw_environment_conf_type_t *conf_type;
    const afw_pool_t *p;
    const afw_utf8_t *s;

    p = xctx->env->p;
    conf_type = afw_pool_calloc_type(p,
        afw_environment_conf_type_t, xctx);

    s = afw_utf8_clone(conf_type_id, p, xctx);
    conf_type->conf_type_id.len = s->len;
    conf_type->conf_type_id.s = s->s;
    conf_type->create = create;
    conf_type->title =
        afw_utf8_clone(title, p, xctx);
    conf_type->description =
        afw_utf8_clone(description, p, xctx);
    conf_type->id_property_name =
        afw_utf8_clone(id_property_name, p, xctx);
    conf_type->id_registry_type_id =
        afw_utf8_clone(id_registry_type_id, p, xctx);
    conf_type->id_runtime_object_type_id =
        afw_utf8_clone(id_runtime_object_type_id, p, xctx);
    conf_type->subtype_property_name =
        afw_utf8_clone(subtype_property_name, p, xctx);
    conf_type->subtype_registry_type_id =
        afw_utf8_clone(subtype_registry_type_id, p, xctx);
    conf_type->subtype_runtime_object_type_id =
        afw_utf8_clone(subtype_runtime_object_type_id, p, xctx);

    conf_type->is_unique = is_unique;

    afw_environment_registry_register(
        afw_environemnt_registry_type_conf_type,
        &conf_type->conf_type_id,
        conf_type,
        xctx);
};


/* Load and initialize environment extension. */
AFW_DEFINE(const afw_extension_t *)
afw_environment_load_extension(
    const afw_utf8_t *extension_id,
    const afw_utf8_t *module_path,
    const afw_object_t *properties,
    afw_xctx_t *xctx)
{
    apr_status_t rv;
    apr_dso_handle_t *dso_handle;
    const afw_extension_t *extension;
    const afw_utf8_z_t *path_z;
    char dsoError[256];
    const afw_extension_t **extension_instance;
    const afw_pool_t *p;
    afw_environment_internal_t *env;
    const afw_utf8_t *dso_suffix;
    const afw_object_t *manifest;
    const afw_utf8_t *extensionId;
    const afw_utf8_t *modulePath;
    const afw_utf8_t *extension_id_for_message;

    env = (afw_environment_internal_t *)xctx->env;
    p = env->pub.p;

    extensionId = NULL;
    modulePath = NULL;
    if (properties) {
        extensionId = afw_object_old_get_property_as_string(properties,
            &afw_s_extensionId, xctx);
        if (extensionId) {
            if (extension_id && !afw_utf8_equal(extension_id, extensionId)) {
                AFW_THROW_ERROR_FZ(general, xctx,
                    "extension_id parameter (%" AFW_UTF8_FMT
                    ") does not match properties.extension_id (%" AFW_UTF8_FMT,
                    AFW_UTF8_FMT_ARG(extension_id),
                    AFW_UTF8_FMT_ARG(extensionId));
            }
        }

        modulePath = afw_object_old_get_property_as_string(properties,
            &afw_s_modulePath, xctx);
        if (modulePath) {
            if (module_path && !afw_utf8_equal(module_path, modulePath)) {
                AFW_THROW_ERROR_FZ(general, xctx,
                    "module_path parameter (%" AFW_UTF8_FMT
                    ") does not match properties.extension_id (%" AFW_UTF8_FMT,
                    AFW_UTF8_FMT_ARG(module_path),
                    AFW_UTF8_FMT_ARG(modulePath));
            }
        }
    }

    if (!extension_id && !module_path) {
        AFW_THROW_ERROR_Z(general,
            "Either extension_id or module_path must be supplied",
            xctx);
    }
 
    AFW_LOCK_BEGIN(xctx->env->environment_lock) {

        /* If extension_id specified, ... */
        if (extension_id) {

            extension_id_for_message = extension_id;

            /* If extension is already loaded, just return. */
            extension = afw_environment_registry_get(
                afw_environemnt_registry_type_extension,
                extension_id, xctx);
            if (extension) {
                break;
            }

            /* If there is not a properties object, make one. */
            if (!properties)
            {
                extension_id = afw_utf8_clone(extension_id, p, xctx);
                properties = afw_object_create_managed(p, xctx);
                afw_object_set_property_as_string(properties,
                    &afw_s_extensionId, extension_id, xctx);
            }

            if (!module_path) {
                manifest = afw_runtime_get_object(&afw_s__AdaptiveManifest_,
                    extension_id, xctx);
                if (manifest) {
                    module_path = afw_object_old_get_property_as_string(manifest,
                        &afw_s_modulePath, xctx);
                }
            }

            if (!module_path) {
                AFW_THROW_ERROR_FZ(general, xctx,
                    "\"modulePath\" needed for \"extension\" %" AFW_UTF8_FMT ".",
                    AFW_UTF8_FMT_ARG(extension_id));
            }

            /* Insure modulePath property matches what was decided.
            afw_object_set_property_as_string(properties,
                &afw_s_modulePath, module_path, xctx); */

            /* Prepare properties. Supply type=extension if needed. */
            if (!afw_object_has_property(properties, &afw_s_type, xctx)) {
                afw_object_set_property_as_string(properties,
                    &afw_s_type, &afw_s_extension, xctx);
            }
            properties = afw_environment_prepare_conf_type_properties(
                properties, xctx);
        }

        /* If just path_z specified. */
        else {
   
            extension_id_for_message = &afw_s_unknown;

            /* If there is not a properties object, make one. */
            if (!properties)
            {
                properties = afw_object_create_managed(p, xctx);
                afw_object_set_property_as_string(properties,
                    &afw_s_modulePath, module_path, xctx);                
            }

        }

        /* Load the extension module. */
        path_z = afw_utf8_to_utf8_z(module_path, p, xctx);
        rv = apr_dso_load(&(dso_handle), (const char *)path_z,
            afw_pool_get_apr_pool(p));
        if (rv != APR_SUCCESS) {
            dso_suffix = afw_os_get_dso_suffix();
            if (!afw_utf8_ends_with(module_path, dso_suffix)) {
                path_z = afw_utf8_z_printf(p, xctx,
                    "%" AFW_UTF8_FMT "%" AFW_UTF8_FMT,
                    AFW_UTF8_FMT_ARG(module_path),
                    AFW_UTF8_FMT_ARG(dso_suffix));
                rv = apr_dso_load(&(dso_handle), (const char *)path_z,
                    afw_pool_get_apr_pool(p));
            }
        }
        if (rv != APR_SUCCESS) {
            path_z = afw_utf8_z_printf(p, xctx,
                AFW_CONFIG_INSTALL_FULL_LIBDIR "/%" AFW_UTF8_FMT,
                AFW_UTF8_FMT_ARG(module_path));
            rv = apr_dso_load(&(dso_handle), (const char *)path_z,
                afw_pool_get_apr_pool(p));
            if (rv != APR_SUCCESS) {
                dso_suffix = afw_os_get_dso_suffix();
                if (!afw_utf8_ends_with(module_path, dso_suffix)) {
                    path_z = afw_utf8_z_printf(p, xctx,
                        AFW_CONFIG_INSTALL_FULL_LIBDIR "/%" AFW_UTF8_FMT "%" AFW_UTF8_FMT,
                        AFW_UTF8_FMT_ARG(module_path),
                        AFW_UTF8_FMT_ARG(dso_suffix));
                    rv = apr_dso_load(&(dso_handle), (const char *)path_z,
                        afw_pool_get_apr_pool(p));
                }
            }
        }

        if (rv != APR_SUCCESS) {
            memset(&dsoError, 0, sizeof(dsoError));
            apr_dso_error(dso_handle, dsoError, sizeof(dsoError));
            afw_pool_release(p, xctx);
            AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
                "Error loading extension extension_id=%" AFW_UTF8_FMT
                " modulePath=%s: %s",
                AFW_UTF8_FMT_ARG(extension_id_for_message), path_z, dsoError);
        }
            
        /* Get the function pointer to the initialization function. */
        rv = apr_dso_sym((apr_dso_handle_sym_t *)&extension_instance,
            dso_handle, AFW_ENVIRONMENT_Q_EXTENSION_INSTANCE);
        if (rv != APR_SUCCESS) {
            apr_dso_unload(dso_handle);
            afw_pool_release(p, xctx);
            AFW_THROW_ERROR_RV_FZ(general, apr, rv, xctx,
                "Error finding symbol " AFW_ENVIRONMENT_Q_EXTENSION_INSTANCE
                " in extension_id=%" AFW_UTF8_FMT " modulePath=%s",
                AFW_UTF8_FMT_ARG(extension_id_for_message), path_z);
        }

        /*
         * If extension_id was not supplied, get it from extension instance. If
         * it's already registered, break.
         */
        if (extension_id_for_message == &afw_s_unknown) {
            extension_id = &(*extension_instance)->extension_id;
            extension = afw_environment_registry_get(
                afw_environemnt_registry_type_extension,
                extension_id, xctx);
            if (extension) {
                break;
            }
        }

        /* Insure compiled against appropriate AFW library and dependents. */
        AFW_VERSION_THROW_ERROR_SUPPLIED_HEX_VERSION_IF_NOT_COMPATIBLE(
            &(*extension_instance)->extension_id,
            (*extension_instance)->afw_compiled_version_hex,
            xctx);

        /* Call the initialization function. */
        extension = afw_extension_initialize(
            (*extension_instance), properties, p, xctx);
        extension_id = &extension->extension_id;

        /* Set extensionId in properties */
        afw_object_set_property_as_string(properties,
            &afw_s_extensionId, extension_id, xctx);

        /* Register extension. */
        afw_environment_registry_register(
            afw_environemnt_registry_type_extension,
            extension_id, extension, xctx);
        AFW_LOG_FZ(info, xctx,
            "Extension %" AFW_UTF8_FMT " loaded.",
            AFW_UTF8_FMT_ARG(extension_id));

    }

    AFW_LOCK_END;

    return extension;
}


/* Register a data type. */
AFW_DEFINE(void)
afw_environment_register_data_type(
    const afw_utf8_t *data_type_id,
    const afw_data_type_t *data_type,
    afw_xctx_t *xctx)
{
    afw_data_type_t *dt;
    afw_environment_internal_t *env;
    apr_array_header_t **h;

    env = (afw_environment_internal_t *)xctx->env;

    AFW_LOCK_BEGIN(xctx->env->environment_lock) {

    /* Make sure data type is not already registered. */
        if (afw_environment_get_data_type(data_type_id, xctx)) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Data type %" AFW_UTF8_FMT " is already registered",
                AFW_UTF8_FMT_ARG(data_type_id));
        }

        /* If data_type number unassigned, make copy and assign it. */
        if (data_type->data_type_number == 0) {
            dt = afw_xctx_calloc(sizeof(afw_data_type_t), xctx);
            memcpy(dt, data_type, sizeof(afw_data_type_t));
            dt->data_type_number = env->data_type_methods->nelts;
            data_type = dt;
        }

        /* Only core data types can have preassigned numbers. */
        else if (env->core_data_types_registered) {
            AFW_THROW_ERROR_Z(general,
                "Only core data types can have preassigned data type number",
                xctx);
        }

        /* Assign empty array to data type's position. */
        while (env->data_type_methods->nelts < data_type->data_type_number) {
            APR_ARRAY_PUSH(env->data_type_methods, apr_array_header_t *) = NULL;
        }
        h = &((apr_array_header_t **)env->data_type_methods->elts)
            [data_type->data_type_number - 1];
        if (*h) {
            AFW_THROW_ERROR_FZ(general, xctx,
                "Data type number %d is already registered.",
                data_type->data_type_number);
        }
        *h = apr_array_make(afw_pool_get_apr_pool(xctx->p),
            30, sizeof(apr_array_header_t *));

        /* Register data type. */
        afw_environment_registry_register(
            afw_environemnt_registry_type_data_type,
            data_type_id,
            data_type,
            xctx);

        /* Add data type object to runtime. */
        afw_runtime_env_set_object(data_type->object, false, xctx);
    }

    AFW_LOCK_END;
}

static const afw_value_function_parameter_t *
impl_resolve_function_parameter(
    const afw_value_function_parameter_t *parameter,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p = xctx->env->p;
    afw_value_function_parameter_t *result;

    if (!parameter) {
        return NULL;
    }

    result = afw_memory_dup(parameter,
        sizeof(afw_value_function_parameter_t),
        p, xctx);

    result->meta.inf = &afw_runtime_inf__AdaptiveFunctionParameter_;

    if (parameter->dataType.s) {
        result->data_type = afw_environment_get_data_type(&parameter->dataType,
            xctx);

    }

    return result;
}



static const afw_value_function_definition_t *
impl_resolve_function(
    const afw_value_function_definition_t *function,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p = xctx->env->p;
    afw_value_function_definition_t *result;
    afw_size_t count;
    const afw_value_function_parameter_t **parameters;
    afw_runtime_object_indirect_t *object;

    result = afw_memory_dup(function, sizeof(afw_value_function_definition_t),
        p, xctx);

    result->inf = &afw_value_function_definition_inf;

    object = afw_memory_dup(function->object,
        sizeof(afw_runtime_object_indirect_t), p, xctx);
    object->pub.inf = &afw_runtime_inf__AdaptiveFunction_;
    object->internal = result;
    result->object = (const afw_object_t *)object;
    if (result->dataType.len > 0) {
        result->data_type = afw_environment_get_data_type(&result->dataType,
            xctx);
    }

    if (result->data_type) {
        AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
    }

    result->returns = impl_resolve_function_parameter(function->returns, xctx);

    if (result->parameters) {
        for (count = 0; function->parameters[count]; count++);
        result->parameters = parameters = afw_pool_malloc(p,
            sizeof(afw_value_function_parameter_t *) * (count + 1),
            xctx);
            parameters[count] = NULL;

            for (count = 0; function->parameters[count]; count++) {
                parameters[count] =
                    impl_resolve_function_parameter(
                        function->parameters[count],
                        xctx);
            }
    }

    return result;
}



/* Register a function. */
AFW_DEFINE(void)
afw_environment_register_function(
    const afw_utf8_t *function_id,
    const afw_value_function_definition_t *function,
    afw_xctx_t *xctx)
{
    afw_value_function_definition_t *f;
    int *method_number;
    apr_array_header_t *methods;
    afw_environment_internal_t *env;

    if (!function->inf) {
        function = impl_resolve_function(function, xctx);
    }

    env = (afw_environment_internal_t *)xctx->env;

    AFW_LOCK_BEGIN(xctx->env->environment_lock) {

        /*
         * If data type is not resolved in function definition, which is the
         * case if this is not core (not src/afw libafw), clone the function
         * defintion and polymorphic parameters then resolve their data type
         * and data type method number in the clone.
         */
        if (function->dataType.len > 0 && !function->data_type) {
            f = afw_xctx_calloc(sizeof(afw_value_function_definition_t),
                xctx);
            memcpy(f, function, sizeof(afw_value_function_definition_t));
            function = f;
            f->data_type = afw_environment_get_data_type(
                &function->dataType, xctx);
            if (!function->data_type) {
                AFW_THROW_ERROR_FZ(general, xctx,
                "dataType %" AFW_UTF8_FMT " doesn't exist",
                AFW_UTF8_FMT_ARG(&function->dataType));
            }
            if (function->dataTypeMethodNumber != 0)
            {
                AFW_THROW_ERROR_Z(general,
                    "Only core function can have preassigned "
                    "dataTypeMethodNumber",
                    xctx);
            }
            method_number = apr_hash_get(
                env->data_type_method_number_ht,
                function->untypedFunctionId.s,
                function->untypedFunctionId.len);
            if (method_number) {
                f->dataTypeMethodNumber = *method_number;
            }
            else {
                f->dataTypeMethodNumber = apr_hash_count(
                    env->data_type_method_number_ht) + 1;
                apr_hash_set(env->data_type_method_number_ht,
                    function->untypedFunctionId.s,
                    function->untypedFunctionId.len,
                    &function->dataTypeMethodNumber);
            }

            /** @todo resolve parameters. */
        }

        /** @fixme Add support for extensions to add data type methods. */
        /* Only core functions can have preassigned numbers. */
        if (env->core_functions_registered &&
            function->dataTypeMethodNumber != 0)
        {
            AFW_THROW_ERROR_Z(general,
                "Only core function can have preassigned function number",
                xctx);
        }

        /* Make sure data type method is in hash table. */
        if (function->data_type) {

            method_number = apr_hash_get(env->data_type_method_number_ht,
                function->untypedFunctionId.s,
                function->untypedFunctionId.len);
            if (method_number) {
                if (function->dataTypeMethodNumber == 0) {
                    f->dataTypeMethodNumber = *method_number;
                }
            }
            else {
                if (function->dataTypeMethodNumber == 0) {
                    f->dataTypeMethodNumber = apr_hash_count(
                        env->data_type_method_number_ht) + 1;
                }
                apr_hash_set(env->data_type_method_number_ht,
                    function->untypedFunctionId.s,
                    function->untypedFunctionId.len,
                    &function->dataTypeMethodNumber);
            }
        }

        /* Register function. */
        afw_environment_registry_register(
            afw_environemnt_registry_type_function,
            function_id,
            function,
            xctx);

        /* Register camelCaseFunctionLabel. */
        if (function->camelCaseFunctionLabel.len > 0 &&
            !afw_utf8_equal(&function->camelCaseFunctionLabel, function_id))
        {
            afw_environment_registry_register(
                afw_environemnt_registry_type_function,
                &function->camelCaseFunctionLabel,
                function,
                xctx);
        }

        /* Register afwCamelCaseFunctionLabel. */
        if (function->afwCamelCaseFunctionLabel.len > 0 &&
            !afw_utf8_equal(&function->afwCamelCaseFunctionLabel, function_id)
            &&
            !afw_utf8_equal(&function->afwCamelCaseFunctionLabel,
                &function->camelCaseFunctionLabel)
            )
        {
            afw_environment_registry_register(
                afw_environemnt_registry_type_function,
                &function->afwCamelCaseFunctionLabel,
                function,
                xctx);
        }

        /* Register function_label. */
        if (function->afwCamelCaseFunctionLabel.len > 0 &&
            !afw_utf8_equal(&function->functionLabel, function_id)
            &&
            !afw_utf8_equal(&function->afwCamelCaseFunctionLabel,
                &function->functionLabel)
            &&
            !afw_utf8_equal(&function->afwCamelCaseFunctionLabel,
                &function->camelCaseFunctionLabel)
            )
        {
            afw_environment_registry_register(
                afw_environemnt_registry_type_function,
                &function->functionLabel,
                function,
                xctx);
        }

        /* If there was a data type id, set method. */
        if (function->dataType.len > 0) {
            methods = ((apr_array_header_t **)env->data_type_methods->elts)
                [function->data_type->data_type_number - 1];
            while (methods->nelts < function->dataTypeMethodNumber) {
                APR_ARRAY_PUSH(methods, const afw_value_function_definition_t *) =
                    NULL;
            }
            ((const afw_value_function_definition_t **)methods->elts)
                [function->dataTypeMethodNumber - 1] = function;
        }

        /* Add function object to runtime. */
        afw_runtime_env_set_object(function->object, false, xctx);
    }
    AFW_LOCK_END;
}


/* Get the qualified function instance. */
AFW_DEFINE(const afw_value_function_definition_t *)
afw_environment_get_qualified_function(
    const afw_utf8_t *qualifier,
    const afw_utf8_t *name,
    afw_xctx_t *xctx)
{
    const afw_value_function_definition_t *result;
    int *method_number;
    afw_environment_internal_t *env;
    const afw_data_type_t *data_type;
    apr_array_header_t *methods;

    AFW_LOCK_BEGIN(xctx->env->environment_lock) {

        if (qualifier && qualifier->s) {
            env = (afw_environment_internal_t *)xctx->env;
            result = NULL;
            data_type = afw_environment_get_data_type(qualifier, xctx);
            if (data_type) {
                method_number = apr_hash_get(env->data_type_method_number_ht,
                    name->s, name->len);
                if (method_number) {
                    methods = ((apr_array_header_t **)
                        env->data_type_methods->elts)
                        [data_type->data_type_number - 1];
                    if (*method_number <= methods->nelts) {
                        result = ((const afw_value_function_definition_t **)
                            methods->elts)[(*method_number) - 1];
                    }
                }
            }
        }

        else {
            result = (const afw_value_function_definition_t *)
                afw_environment_registry_get(
                afw_environemnt_registry_type_function, name, xctx);
        }
    }
    AFW_LOCK_END;

    return result;
}


/* Get the function associated with a data type method. */
AFW_DEFINE(const afw_value_function_definition_t *)
afw_environment_registry_get_data_type_method(
    const afw_data_type_t *data_type,
    afw_integer_t dataTypeMethodNumber,
    afw_xctx_t *xctx)
{
    const afw_value_function_definition_t *result;
    apr_array_header_t *d;
    afw_environment_internal_t *env;

    env = (afw_environment_internal_t *)xctx->env;
    result = NULL;

    if (!data_type) {
        AFW_THROW_ERROR_Z(general, "Data type is NULL", xctx);
    }

    AFW_LOCK_BEGIN(xctx->env->environment_lock) {

        if (data_type->data_type_number > env->data_type_methods->nelts) {
            AFW_THROW_ERROR_Z(general, "Data type number out of range", xctx);
        }

        d = ((apr_array_header_t **)env->data_type_methods->elts)
            [data_type->data_type_number - 1];
        if (dataTypeMethodNumber <= d->nelts) {
            result = ((const afw_value_function_definition_t **)d->elts)
                [dataTypeMethodNumber - 1];
        }

    }
    AFW_LOCK_END;

    return result;
}


/* Register a NULL terminated list of data types. */
AFW_DEFINE(void)
afw_environment_register_data_types(
    const afw_data_type_t * const *data_types,
    afw_xctx_t *xctx)
{
    const afw_data_type_t * const *e;

    for (e = data_types; *e; e++) {
        afw_environment_register_data_type(
            &(*e)->data_type_id, *e, xctx);
    }

}


/* Register a NULL terminated list of functions. */
AFW_DEFINE(void)
afw_environment_register_functions(
    const afw_value_function_definition_t **functions,
    afw_xctx_t *xctx)
{
    const afw_value_function_definition_t **function;

    for (function = functions; *function; function++) {
        afw_environment_register_function(&(*function)->functionId, *function,
            xctx);
    }
}


/* Register an adaptor factory. */
AFW_DEFINE(void)
afw_environment_register_adaptor_type(
    const afw_utf8_t *adaptor_type,
    const afw_adaptor_factory_t *adaptor_factory,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p = xctx->env->p;
    const afw_utf8_t *flag_id;
    const afw_utf8_t *detail_flag_id;
    const afw_utf8_t *brief;
    const afw_utf8_t *description;
  
    AFW_LOCK_BEGIN(xctx->env->environment_lock) {

        afw_environment_registry_register(
            afw_environemnt_registry_type_adaptor_type,
            adaptor_type,
            adaptor_factory,
            xctx);

        detail_flag_id = afw_utf8_printf(p, xctx,
            "trace:adaptorType:%" AFW_UTF8_FMT ":detail",
            AFW_UTF8_FMT_ARG(adaptor_type));

        brief = afw_utf8_printf(p, xctx,
            "Debug trace adaptor type %" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(adaptor_type));

        description = afw_utf8_printf(p, xctx,
            "This produces a basic plus additional detail trace for "
            "all adaptors of type %" AFW_UTF8_FMT ".",
            AFW_UTF8_FMT_ARG(adaptor_type));

        afw_environment_register_flag(detail_flag_id, brief, description,
            &afw_s_a_flag_trace_adaptor_detail, xctx);

        flag_id = afw_utf8_printf(p, xctx,
            "trace:adaptorType:%" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(adaptor_type));

        brief = afw_utf8_printf(p, xctx,
            "Trace adaptor type %" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(adaptor_type));

        description = afw_utf8_printf(p, xctx,
            "This produces a basic trace of all adaptors of type %" AFW_UTF8_FMT ".",
            AFW_UTF8_FMT_ARG(adaptor_type));

        afw_environment_register_flag(flag_id, brief, description,
            &afw_s_a_flag_trace_adaptor, xctx);

        afw_flag_add_included_by(flag_id, detail_flag_id, xctx);
    }
    AFW_LOCK_END;
}



/* Register an authorization handler factory.  */
AFW_DEFINE(void)
afw_environment_register_authorization_handler_type(
    const afw_utf8_t *authorization_handler_type,
    const afw_authorization_handler_factory_t *authorization_handler_factory,
    afw_xctx_t *xctx)
{
    const afw_pool_t *p = xctx->env->p;
    const afw_utf8_t *flag_id;
    const afw_utf8_t *detail_flag_id;
    const afw_utf8_t *brief;
    const afw_utf8_t *description;

    AFW_LOCK_BEGIN(xctx->env->environment_lock) {

        afw_environment_registry_register(
            afw_environemnt_registry_type_authorization_handler_type,
            authorization_handler_type,
            authorization_handler_factory,
            xctx);

        detail_flag_id = afw_utf8_printf(p, xctx,
            "trace:authorizationHandlerType:%" AFW_UTF8_FMT ":detail",
            AFW_UTF8_FMT_ARG(authorization_handler_type));

        brief = afw_utf8_printf(p, xctx,
            "Debug trace authorizationHandler type %" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(authorization_handler_type));

        description = afw_utf8_printf(p, xctx,
            "This produces a basic plus additional detail trace for "
            "all authorizationHandlers of type %" AFW_UTF8_FMT ".",
            AFW_UTF8_FMT_ARG(authorization_handler_type));

        afw_environment_register_flag(detail_flag_id, brief, description,
            &afw_s_a_flag_trace_authorizationHandler_detail, xctx);

        flag_id = afw_utf8_printf(p, xctx,
            "trace:authorizationHandlerType:%" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(authorization_handler_type));

        brief = afw_utf8_printf(p, xctx,
            "Trace authorizationHandler type %" AFW_UTF8_FMT,
            AFW_UTF8_FMT_ARG(authorization_handler_type));

        description = afw_utf8_printf(p, xctx,
            "This produces a basic trace of all authorizationHandlers "
            "of type %" AFW_UTF8_FMT ".",
            AFW_UTF8_FMT_ARG(authorization_handler_type));

        afw_environment_register_flag(flag_id, brief, description,
            &afw_s_a_flag_trace_authorizationHandler, xctx);

        afw_flag_add_included_by(flag_id, detail_flag_id, xctx);
    }
    AFW_LOCK_END;
}


AFW_DEFINE(void)
afw_environment_set_debug_fd(FILE *fd, afw_xctx_t *xctx)
{
    afw_environment_internal_t *env;

    env = (afw_environment_internal_t *)xctx->env;
    env->pub.debug_fd = fd;
}


AFW_DEFINE(void)
afw_environment_set_stderr_fd(FILE *fd, afw_xctx_t *xctx)
{
    afw_environment_internal_t *env;

    env = (afw_environment_internal_t *)xctx->env;
    env->pub.stderr_fd = fd;
}


AFW_DEFINE(void)
afw_environment_set_stdout_fd(FILE *fd, afw_xctx_t *xctx)
{
    afw_environment_internal_t *env;

    env = (afw_environment_internal_t *)xctx->env;
    env->pub.stdout_fd = fd;
}
