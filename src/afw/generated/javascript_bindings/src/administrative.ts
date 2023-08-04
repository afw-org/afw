// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for administrative
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */


interface IAnyObject {
    [prop : string] : any;
}

/**
 * Load an extension by its extension id if it is not already loaded. Loading
 * an AFW package's manifest extension will register the manifest of all
 * extensions in the package.
 * 
 * @param {string} extension_id - This is the object id of a
 *     /afw/_AdaptiveManifest_/ object.
 * 
 * @returns {boolean} If false the extension was already loaded. If true, the
 *     extension was successfully loaded which might have caused side effects
 *     such as environment registry changes. An error is thrown if there is a
 *     problem.
 */
export function afwExtensionLoad(client : any, extension_id : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "extension_load";
    _action["extension_id"] = extension_id;

    return client.perform(_action);
}

/**
 * Load an extension by its module path. Loading an AFW package's manifest
 * extension will register the manifest of all extensions in the package.
 * 
 * @param {string} module_path - This is the path to the dso containing the
 *     extension. If the extension is installed in the normal place, the
 *     library name without a file extension (.so) will suffice.
 * 
 * @returns {string} The extension id of the extension loaded.
 */
export function afwExtensionLoadByModulePath(client : any, module_path : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "extension_load_by_module_path";
    _action["module_path"] = module_path;

    return client.perform(_action);
}

/**
 * Get a list of of the flagId of flags that are set in the current execution
 * context (xctx).
 * 
 * @returns {list} This is a list of the flagId of flags that are set in the
 *     current execution context (xctx).
 */
export function afwFlagGetActive(client : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "flag_get_active";

    return client.perform(_action);
}

/**
 * Get a list of the flagId of flags that are set by default when a new
 * execution context (xctx) is created.
 * 
 * @returns {list} This is a list of the flagId of flags that are set by
 *     default when a new execution context (xctx) is created.
 */
export function afwFlagGetActiveDefaults(client : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "flag_get_active_defaults";

    return client.perform(_action);
}

/**
 * Get the list of the flagId of flags that are used to determine the default
 * active flags when an execution context (xctx) is created. This list can
 * contain the flagId of flags that have not yet been registered. Each of
 * these flags and the flags they include are set as the active default
 * flags.
 * 
 * @returns {list} This is a list of the flagId of flags used to determine
 *     the default active flags.
 */
export function afwFlagGetDefaults(client : any) : any {

    let _action : IAnyObject = {};

    _action["function"] = "flag_get_defaults";

    return client.perform(_action);
}

/**
 * Add or remove flags from the list of the flagId of flags that are used to
 * determine the default active flags when an execution context (xctx) is
 * created. This list can contain the flagId of flags that have not yet been
 * registered. These flags and the flags they include are set as the active
 * default flags.
 * 
 * This change only lasts for the life of the current adaptive environment
 * (e.g. until the adaptive server or afw command ends). If you want the
 * change to persist, change the defaultFlags property in the application
 * config.
 * 
 * @param {list} flagId - The flagId of flags to be added or removed.
 * 
 * @param {boolean} add - Specify true to add and false to remove flags. If
 *     not specified, flags are added.
 * 
 * @returns {null}
 */
export function afwFlagModifyDefaults(client : any, flagId : any[], add? : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "flag_modify_defaults";
    _action["flagId"] = flagId;

    if (add !== undefined)
        _action["add"] = add;

    return client.perform(_action);
}

/**
 * Completely replace the list of the flagId of flags that are used to
 * determine the default active flags when an execution context (xctx) is
 * created. This list can contain the flagId of flags that have not yet been
 * registered. These flags and the flags they include are set as the active
 * default flags.
 * 
 * This change only lasts for the life of the current adaptive environment
 * (e.g. until the adaptive server or afw command ends). If you want the
 * change to persist, change the defaultFlags property in the application
 * config.
 * 
 * @param {list} flagId - The list of the flagId of flags used to determine
 *     the default active flags.
 * 
 * @returns {null}
 */
export function afwFlagReplaceDefaults(client : any, flagId : any[]) : any {

    let _action : IAnyObject = {};

    _action["function"] = "flag_replace_defaults";
    _action["flagId"] = flagId;

    return client.perform(_action);
}

/**
 * Set or unset one or more active xctx (request) flags.
 * 
 * @param {list} flagId - List of flagId of flags to set or unset.
 * 
 * @param {boolean} setTo - Specify true to set and false to unset. If not
 *     specified, flags are set.
 * 
 * @returns {null}
 */
export function afwFlagSet(client : any, flagId : any[], setTo? : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "flag_set";
    _action["flagId"] = flagId;

    if (setTo !== undefined)
        _action["setTo"] = setTo;

    return client.perform(_action);
}

/**
 * This will check to see if a registry key exists for a specified registry
 * type and optionally load it's associated extension if needed.
 * 
 * @param {string} registryType - This is the registry type, which is the
 *     object id of a /afw/_AdaptiveEnvironmentRegistryType_/ object.
 * 
 * @param {string} key - This is a key to check for existence in the
 *     specified registryType.
 * 
 * @param {boolean} loadExtension - Specifying true for this optional
 *     parameter will cause the associated extension to be loaded if needed.
 * 
 * @returns {boolean} If false the extension was already loaded. If true, the
 *     extension was successfully loaded which might have caused side effects
 *     such as environment registry changes. An error is thrown if there is a
 *     problem.
 */
export function afwRegistryKeyCheck(client : any, registryType : string, key : string, loadExtension? : boolean) : any {

    let _action : IAnyObject = {};

    _action["function"] = "registry_key_check";
    _action["registryType"] = registryType;
    _action["key"] = key;

    if (loadExtension !== undefined)
        _action["loadExtension"] = loadExtension;

    return client.perform(_action);
}

/**
 * Get a service object.
 * 
 * @param {string} serviceId - The serviceId of the service.
 * 
 * @returns {object} _AdaptiveService_ object for the service which will
 *     contain the current status of the service. If there is an error, the
 *     status property value will be "error" and "statusMessage" contain an
 *     error message.
 */
export function afwServiceGet(client : any, serviceId : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "service_get";
    _action["serviceId"] = serviceId;

    return client.perform(_action);
}

/**
 * Restart a service.
 * 
 * @param {string} serviceId - The serviceId of the service to restart.
 * 
 * @returns {object} _AdaptiveService_ object for the service which will
 *     contain the current status of the service. If there is an error, the
 *     status property value will be "error" and "statusMessage" contain an
 *     error message.
 */
export function afwServiceRestart(client : any, serviceId : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "service_restart";
    _action["serviceId"] = serviceId;

    return client.perform(_action);
}

/**
 * Start a service.
 * 
 * @param {string} serviceId - The serviceId of the service to start
 * 
 * @returns {object} _AdaptiveService_ object for the service which will
 *     contain the current status of the service. If there is an error, the
 *     status property value will be "error" and "statusMessage" contain an
 *     error message.
 */
export function afwServiceStart(client : any, serviceId : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "service_start";
    _action["serviceId"] = serviceId;

    return client.perform(_action);
}

/**
 * Stop a service.
 * 
 * @param {string} serviceId - The serviceId of the service to stop.
 * 
 * @returns {object} _AdaptiveService_ object for the service which will
 *     contain the current status of the service. If there is an error, the
 *     status property value will be "error" and "statusMessage" contain an
 *     error message.
 */
export function afwServiceStop(client : any, serviceId : string) : any {

    let _action : IAnyObject = {};

    _action["function"] = "service_stop";
    _action["serviceId"] = serviceId;

    return client.perform(_action);
}

