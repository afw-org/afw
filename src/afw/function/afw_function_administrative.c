// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_function_execute_* functions for administrative
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_function_administrative.c
 * @brief afw_function_execute_* functions for administrative.
 */

#include "afw_internal.h"



/*
 * Adaptive function: flag_get_active
 *
 * afw_function_execute_flag_get_active
 *
 * See afw_function_bindings.h for more information.
 *
 * Get a list of of the flagId of flags that are set in the current execution
 * context (xctx).
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function flag_get_active(
 *   
 *   ): (list string);
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (list string) This is a list of the flagId of flags that are set in the
 *       current execution context (xctx).
 */
const afw_value_t *
afw_function_execute_flag_get_active(
    afw_function_execute_t *x)
{
    afw_size_t i, count;
    const afw_list_t *list;

    list = afw_list_create_with_options(0, afw_data_type_string,
        x->p, x->xctx);

    for (i = 0, count = x->xctx->flags_count;
        i < count && x->xctx->env->flag_by_index[i];
        i++)
    {
        if (x->xctx->flags[i]) {
            afw_list_add_internal(list, afw_data_type_string,
                x->xctx->env->flag_by_index[i]->flag_id, x->xctx);
        }
    }

    return afw_value_create_list(list, x->p, x->xctx);
}



/*
 * Adaptive function: flag_get_active_defaults
 *
 * afw_function_execute_flag_get_active_defaults
 *
 * See afw_function_bindings.h for more information.
 *
 * Get a list of the flagId of flags that are set by default when a new
 * execution context (xctx) is created.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function flag_get_active_defaults(
 *   
 *   ): (list string);
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (list string) This is a list of the flagId of flags that are set by
 *       default when a new execution context (xctx) is created.
 */
const afw_value_t *
afw_function_execute_flag_get_active_defaults(
    afw_function_execute_t *x)
{
    afw_size_t i, count;
    const afw_list_t *list;

    list = afw_list_create_with_options(0, afw_data_type_string,
        x->p, x->xctx);

    for (i = 0, count = x->xctx->env->flags_count_registered;
        i < count;
        i++)
    {
        if (x->xctx->env->default_flags[i]) {
            afw_list_add_internal(list, afw_data_type_string,
                x->xctx->env->flag_by_index[i]->flag_id, x->xctx);
        }
    }

    return afw_value_create_list(list, x->p, x->xctx);
}



/*
 * Adaptive function: flag_get_defaults
 *
 * afw_function_execute_flag_get_defaults
 *
 * See afw_function_bindings.h for more information.
 *
 * Get the list of the flagId of flags that are used to determine the default
 * active flags when an execution context (xctx) is created. This list can
 * contain the flagId of flags that have not yet been registered. Each of these
 * flags and the flags they include are set as the active default flags.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function flag_get_defaults(
 *   
 *   ): (list string);
 * ```
 *
 * Parameters:
 *
 * Returns:
 *
 *   (list string) This is a list of the flagId of flags used to determine the
 *       default active flags.
 */
const afw_value_t *
afw_function_execute_flag_get_defaults(
    afw_function_execute_t *x)
{
    const afw_environment_internal_t *env_internal =
        (const afw_environment_internal_t *)x->xctx->env;
    const afw_list_t *list;
    const afw_utf8_t * const *flag_id;
    const afw_utf8_t *s;
    afw_xctx_t *xctx = x->xctx;

    list = afw_list_create_with_options(0, afw_data_type_string,
        x->p, x->xctx);

    AFW_LOCK_BEGIN(xctx->env->flags_lock) {
        if (env_internal->default_flag_ids) {
            for (flag_id = env_internal->default_flag_ids; *flag_id; flag_id++)
            {
                s = afw_utf8_clone(*flag_id, x->p, x->xctx);
                afw_list_add_internal(list, afw_data_type_string, s, x->xctx);
            }
        }

    }
    AFW_LOCK_END;

    return afw_value_create_list(list, x->p, x->xctx);
}



/*
 * Adaptive function: flag_modify_defaults
 *
 * afw_function_execute_flag_modify_defaults
 *
 * See afw_function_bindings.h for more information.
 *
 * Add or remove flags from the list of the flagId of flags that are used to
 * determine the default active flags when an execution context (xctx) is
 * created. This list can contain the flagId of flags that have not yet been
 * registered. These flags and the flags they include are set as the active
 * default flags.
 * 
 * This change only lasts for the life of the current adaptive environment
 * (e.g. until the adaptive server or afw command ends). If you want the change
 * to persist, change the defaultFlags property in the application config.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function flag_modify_defaults(
 *       flagId: (list string),
 *       add?: boolean
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   flagId - (list string) The flagId of flags to be added or removed.
 *
 *   add - (optional boolean) Specify true to add and false to remove flags. If
 *       not specified, flags are added.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_flag_modify_defaults(
    afw_function_execute_t *x)
{
    const afw_value_list_t *list_value;
    const afw_value_boolean_t *set_to_value;
    const afw_data_type_t *data_type;
    const afw_iterator_t *iterator;
    const afw_utf8_t *flag_id;
    const void *internal;
    afw_boolean_t add;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(list_value, 1, list);

    add = true;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(set_to_value,
            2, boolean);
        add = set_to_value->internal;
    }

    for (iterator = NULL;;)
    {
        afw_list_get_next_internal(
            list_value->internal,
            &iterator,
            &data_type,
            &internal,
            x->xctx);

        if (!internal) break;

        if (afw_data_type_is_string(data_type)) {
            flag_id = (const afw_utf8_t *)internal;
        }
        else {
            flag_id = afw_data_type_internal_to_utf8(data_type,
                internal, x->p, x->xctx);
        }
        
        afw_flag_set_default(flag_id, add, x->xctx);
    }

    return afw_value_null;
}



/*
 * Adaptive function: flag_replace_defaults
 *
 * afw_function_execute_flag_replace_defaults
 *
 * See afw_function_bindings.h for more information.
 *
 * Completely replace the list of the flagId of flags that are used to
 * determine the default active flags when an execution context (xctx) is
 * created. This list can contain the flagId of flags that have not yet been
 * registered. These flags and the flags they include are set as the active
 * default flags.
 * 
 * This change only lasts for the life of the current adaptive environment
 * (e.g. until the adaptive server or afw command ends). If you want the change
 * to persist, change the defaultFlags property in the application config.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function flag_replace_defaults(
 *       flagId: (list string)
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   flagId - (list string) The list of the flagId of flags used to determine
 *       the default active flags.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_flag_replace_defaults(
    afw_function_execute_t *x)
{
    const afw_value_list_t *list_value;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(list_value, 1, list);

    afw_flag_set_default_flag_ids(list_value->internal, x->xctx);

    return afw_value_null;
}



/*
 * Adaptive function: flag_set
 *
 * afw_function_execute_flag_set
 *
 * See afw_function_bindings.h for more information.
 *
 * Set or unset one or more active xctx (request) flags.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function flag_set(
 *       flagId: (list string),
 *       setTo?: boolean
 *   ): null;
 * ```
 *
 * Parameters:
 *
 *   flagId - (list string) List of flagId of flags to set or unset.
 *
 *   setTo - (optional boolean) Specify true to set and false to unset. If not
 *       specified, flags are set.
 *
 * Returns:
 *
 *   (null)
 */
const afw_value_t *
afw_function_execute_flag_set(
    afw_function_execute_t *x)
{
    const afw_value_list_t *list_value;
    const afw_value_boolean_t *set_to_value;
    const afw_data_type_t *data_type;
    const afw_iterator_t *iterator;
    const afw_utf8_t *flag_id;
    const void *internal;
    afw_boolean_t set_to;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(list_value, 1, list);

    set_to = true;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(2)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(set_to_value,
            2, boolean);
        set_to = set_to_value->internal;
    }

    for (iterator = NULL;;)
    {
        afw_list_get_next_internal(
            list_value->internal,
            &iterator,
            &data_type,
            &internal,
            x->xctx);

        if (!internal) break;

        if (afw_data_type_is_string(data_type)) {
            flag_id = (const afw_utf8_t *)internal;
        }
        else {
            flag_id = afw_data_type_internal_to_utf8(data_type,
                internal, x->p, x->xctx);
        }
        
        afw_flag_set(flag_id, set_to, x->xctx);
    }

    return afw_value_null;
}



/*
 * Adaptive function: extension_load
 *
 * afw_function_execute_extension_load
 *
 * See afw_function_bindings.h for more information.
 *
 * Load an extension by its extension id if it is not already loaded. Loading
 * an AFW package's manifest extension will register the manifest of all
 * extensions in the package.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function extension_load(
 *       extension_id: string
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   extension_id - (string) This is the object id of a
 *       /afw/_AdaptiveManifest_/ object.
 *
 * Returns:
 *
 *   (boolean) If false the extension was already loaded. If true, the
 *       extension was successfully loaded which might have caused side effects
 *       such as environment registry changes. An error is thrown if there is a
 *       problem.
 */
const afw_value_t *
afw_function_execute_extension_load(
    afw_function_execute_t *x)
{
    const afw_value_string_t *extension_id;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(extension_id, 1, string);

    /* If extension already loaded, return false. */
    if (afw_environment_registry_get(
        afw_environemnt_registry_type_extension,
        &extension_id->internal, x->xctx))
    {
        return afw_value_false;
    }

    /* Load extension and return true. */
    afw_environment_load_extension(
        &extension_id->internal, NULL, NULL, x->xctx);

    return afw_value_true;
}



/*
 * Adaptive function: extension_load_by_module_path
 *
 * afw_function_execute_extension_load_by_module_path
 *
 * See afw_function_bindings.h for more information.
 *
 * Load an extension by its module path. Loading an AFW package's manifest
 * extension will register the manifest of all extensions in the package.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function extension_load_by_module_path(
 *       module_path: string
 *   ): string;
 * ```
 *
 * Parameters:
 *
 *   module_path - (string) This is the path to the dso containing the
 *       extension. If the extension is installed in the normal place, the
 *       library name without a file extension (.so) will suffice.
 *
 * Returns:
 *
 *   (string) The extension id of the extension loaded.
 */
const afw_value_t *
afw_function_execute_extension_load_by_module_path(
    afw_function_execute_t *x)
{
    const afw_value_string_t *module_path;
    const afw_utf8_t *extension_id;
    const afw_extension_t *extension;

    AFW_FUNCTION_EVALUATE_DATA_TYPE_PARAMETER(module_path, 1, string);

    /* Load extension and return true. */
    extension = afw_environment_load_extension(
        NULL, &module_path->internal, NULL, x->xctx);
    extension_id = (extension)
        ? &extension->extension_id
        : &afw_s_undefined;

    return afw_value_create_string(extension_id, x->p, x->xctx);
}



/*
 * Adaptive function: registry_key_check
 *
 * afw_function_execute_registry_key_check
 *
 * See afw_function_bindings.h for more information.
 *
 * This will check to see if a registry key exists for a specified registry
 * type and optionally load it's associated extension if needed.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function registry_key_check(
 *       registryType: string,
 *       key: string,
 *       loadExtension?: boolean
 *   ): boolean;
 * ```
 *
 * Parameters:
 *
 *   registryType - (string) This is the registry type, which is the object id
 *       of a /afw/_AdaptiveEnvironmentRegistryType_/ object.
 *
 *   key - (string) This is a key to check for existence in the specified
 *       registryType.
 *
 *   loadExtension - (optional boolean) Specifying true for this optional
 *       parameter will cause the associated extension to be loaded if needed.
 *
 * Returns:
 *
 *   (boolean) If false the extension was already loaded. If true, the
 *       extension was successfully loaded which might have caused side effects
 *       such as environment registry changes. An error is thrown if there is a
 *       problem.
 */
const afw_value_t *
afw_function_execute_registry_key_check(
    afw_function_execute_t *x)
{
    const afw_value_string_t *registry_type_value;
    const afw_value_string_t *key_value;
    const afw_value_boolean_t *load_extension_value;
    const afw_environment_registry_type_t *registry_type;
    afw_boolean_t load_extension;
    const afw_value_t *result;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(registry_type_value,
        1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(key_value, 2, string);

    load_extension = false;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(3)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(load_extension_value,
            3, boolean);
        load_extension = load_extension_value->internal;
    }

    registry_type = afw_environment_get_registry_type_by_id(
        &registry_type_value->internal, load_extension, x->xctx);

    if (!registry_type) {
        result = afw_value_false;
    }

    else if (load_extension) {
        result =
            (afw_environment_registry_get(
                registry_type->number, &key_value->internal, x->xctx))
            ? afw_value_true
            : afw_value_false;
    }
    else {
        result = (afw_environment_registry_key_exists(
            registry_type->number, &key_value->internal, x->xctx))
            ? afw_value_true
            : afw_value_false;
    }

    return result;
}



/*
 * Adaptive function: service_get
 *
 * afw_function_execute_service_get
 *
 * See afw_function_bindings.h for more information.
 *
 * Get a service object.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function service_get(
 *       serviceId: string
 *   ): (object _AdaptiveService_);
 * ```
 *
 * Parameters:
 *
 *   serviceId - (string) The serviceId of the service.
 *
 * Returns:
 *
 *   (object _AdaptiveService_) _AdaptiveService_ object for the service which
 *       will contain the current status of the service. If there is an error,
 *       the status property value will be "error" and "statusMessage" contain
 *       an error message.
 */
const afw_value_t *
afw_function_execute_service_get(
    afw_function_execute_t *x)
{
    const afw_value_string_t *service_id;
    const afw_object_t *obj;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(service_id, 1, string);

    obj = afw_service_get_object(&service_id->internal, x->p, x->xctx);

    if (!obj) {
        AFW_THROW_ERROR_Z(not_found, "Not found", x->xctx);
    }

    return afw_value_create_object(obj, x->p, x->xctx);
}



/*
 * Adaptive function: service_restart
 *
 * afw_function_execute_service_restart
 *
 * See afw_function_bindings.h for more information.
 *
 * Restart a service.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function service_restart(
 *       serviceId: string
 *   ): (object _AdaptiveService_);
 * ```
 *
 * Parameters:
 *
 *   serviceId - (string) The serviceId of the service to restart.
 *
 * Returns:
 *
 *   (object _AdaptiveService_) _AdaptiveService_ object for the service which
 *       will contain the current status of the service. If there is an error,
 *       the status property value will be "error" and "statusMessage" contain
 *       an error message.
 */
const afw_value_t *
afw_function_execute_service_restart(
    afw_function_execute_t *x)
{
    const afw_value_string_t *service_id;
    const afw_object_t *obj;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(service_id, 1, string);

    afw_service_restart(&service_id->internal, x->xctx);

    obj = afw_service_get_object(&service_id->internal, x->p, x->xctx);

    if (!obj) {
        AFW_THROW_ERROR_Z(not_found, "Not found", x->xctx);
    }

    return afw_value_create_object(obj, x->p, x->xctx);
}



/*
 * Adaptive function: service_start
 *
 * afw_function_execute_service_start
 *
 * See afw_function_bindings.h for more information.
 *
 * Start a service.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function service_start(
 *       serviceId: string
 *   ): (object _AdaptiveService_);
 * ```
 *
 * Parameters:
 *
 *   serviceId - (string) The serviceId of the service to start.
 *
 * Returns:
 *
 *   (object _AdaptiveService_) _AdaptiveService_ object for the service which
 *       will contain the current status of the service. If there is an error,
 *       the status property value will be "error" and "statusMessage" contain
 *       an error message.
 */
const afw_value_t *
afw_function_execute_service_start(
    afw_function_execute_t *x)
{
    const afw_value_string_t *service_id;
    const afw_object_t *obj;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(service_id, 1, string);

    afw_service_start(&service_id->internal, true, x->xctx);

    obj = afw_service_get_object(&service_id->internal, x->p, x->xctx);

    if (!obj) {
        AFW_THROW_ERROR_Z(not_found, "Not found", x->xctx);
    }

    return afw_value_create_object(obj, x->p, x->xctx);
}



/*
 * Adaptive function: service_stop
 *
 * afw_function_execute_service_stop
 *
 * See afw_function_bindings.h for more information.
 *
 * Stop a service.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters and has side effects.
 *
 * Declaration:
 *
 * ```
 *   function service_stop(
 *       serviceId: string
 *   ): (object _AdaptiveService_);
 * ```
 *
 * Parameters:
 *
 *   serviceId - (string) The serviceId of the service to stop.
 *
 * Returns:
 *
 *   (object _AdaptiveService_) _AdaptiveService_ object for the service which
 *       will contain the current status of the service. If there is an error,
 *       the status property value will be "error" and "statusMessage" contain
 *       an error message.
 */
const afw_value_t *
afw_function_execute_service_stop(
    afw_function_execute_t *x)
{
    const afw_value_string_t *service_id;
    const afw_object_t *obj;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(service_id, 1, string);

    afw_service_stop(&service_id->internal, x->xctx);

    obj = afw_service_get_object(&service_id->internal, x->p, x->xctx);

    if (!obj) {
        AFW_THROW_ERROR_Z(not_found, "Not found", x->xctx);
    }

    return afw_value_create_object(obj, x->p, x->xctx);
}
