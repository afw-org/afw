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

#include "afw.h"



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
 *   function extension_load(
 *       extension_id: string
 *   ): boolean;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function extension_load_by_module_path(
 *       module_path: string
 *   ): string;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}



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
 *   function flag_get_active(
 *   
 *   ): (list string);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function flag_get_active_defaults(
 *   
 *   ): (list string);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function flag_get_defaults(
 *   
 *   ): (list string);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function flag_modify_defaults(
 *       flagId: (list string),
 *       add?: boolean
 *   ): null;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function flag_replace_defaults(
 *       flagId: (list string)
 *   ): null;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function flag_set(
 *       flagId: (list string),
 *       setTo?: boolean
 *   ): null;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function registry_key_check(
 *       registryType: string,
 *       key: string,
 *       loadExtension?: boolean
 *   ): boolean;
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function service_get(
 *       serviceId: string
 *   ): (object _AdaptiveService_);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function service_restart(
 *       serviceId: string
 *   ): (object _AdaptiveService_);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function service_start(
 *       serviceId: string
 *   ): (object _AdaptiveService_);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
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
 *   function service_stop(
 *       serviceId: string
 *   ): (object _AdaptiveService_);
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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", xctx);
}
