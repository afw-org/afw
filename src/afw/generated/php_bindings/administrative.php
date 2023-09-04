<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for administrative
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category administrative
 */
class administrative
{
    private $session;

    /**
     * Constructs the administrative class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * extension_load()
     *
     * Load an extension by its extension id if it is not already loaded.
     * Loading an AFW package's manifest extension will register the manifest
     * of all extensions in the package.
     *
     * @param string $extension_id This is the object id of a
     *                             /afw/_AdaptiveManifest_/ object.
     *
     * @return boolean If false the extension was already loaded. If true, the
     *                 extension was successfully loaded which might have
     *                 caused side effects such as environment registry
     *                 changes. An error is thrown if there is a problem.
     */
    public function extension_load(, $extension_id)
    {
        $request = $this->$session->request();

        $request->set("function", "extension_load");

        /* pass along required parameters to the request payload */
        $request->set("extension_id", $extension_id);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * extension_load_by_module_path()
     *
     * Load an extension by its module path. Loading an AFW package's manifest
     * extension will register the manifest of all extensions in the package.
     *
     * @param string $module_path This is the path to the dso containing the
     *                            extension. If the extension is installed in
     *                            the normal place, the library name without a
     *                            file extension (.so) will suffice.
     *
     * @return string The extension id of the extension loaded.
     */
    public function extension_load_by_module_path(, $module_path)
    {
        $request = $this->$session->request();

        $request->set("function", "extension_load_by_module_path");

        /* pass along required parameters to the request payload */
        $request->set("module_path", $module_path);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * flag_get_active()
     *
     * Get an array of of the flagId of flags that are set in the current
     * execution context (xctx).
     *
     *
     * @return array This is an array of the flagId of flags that are set in
     *               the current execution context (xctx).
     */
    public function flag_get_active()
    {
        $request = $this->$session->request();

        $request->set("function", "flag_get_active");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * flag_get_active_defaults()
     *
     * Get an array of the flagId of flags that are set by default when a new
     * execution context (xctx) is created.
     *
     *
     * @return array This is an array of the flagId of flags that are set by
     *               default when a new execution context (xctx) is created.
     */
    public function flag_get_active_defaults()
    {
        $request = $this->$session->request();

        $request->set("function", "flag_get_active_defaults");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * flag_get_defaults()
     *
     * Get the array of the flagId of flags that are used to determine the
     * default active flags when an execution context (xctx) is created. This
     * array can contain the flagId of flags that have not yet been
     * registered. Each of these flags and the flags they include are set as
     * the active default flags.
     *
     *
     * @return array This is an array of the flagId of flags used to determine
     *               the default active flags.
     */
    public function flag_get_defaults()
    {
        $request = $this->$session->request();

        $request->set("function", "flag_get_defaults");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * flag_modify_defaults()
     *
     * Add or remove flags from the array of the flagId of flags that are used
     * to determine the default active flags when an execution context (xctx)
     * is created. This array can contain the flagId of flags that have not
     * yet been registered. These flags and the flags they include are set as
     * the active default flags.
     * 
     * This change only lasts for the life of the current adaptive environment
     * (e.g. until the adaptive server or afw command ends). If you want the
     * change to persist, change the defaultFlags property in the application
     * config.
     *
     * @param array $flagId The flagId of flags to be added or removed.
     * @param boolean $add Specify true to add and false to remove flags. If
     *                     not specified, flags are added.
     *
     * @return void
     */
    public function flag_modify_defaults(, $flagId, $add = null)
    {
        $request = $this->$session->request();

        $request->set("function", "flag_modify_defaults");

        /* pass along required parameters to the request payload */
        $request->set("flagId", $flagId);

        /* pass along any optional parameters to the request payload */
        if ($add != null)
            $request->set('add', $add);

        return $request->get_result();
    }

    /**
     * flag_replace_defaults()
     *
     * Completely replace the array of the flagId of flags that are used to
     * determine the default active flags when an execution context (xctx) is
     * created. This array can contain the flagId of flags that have not yet
     * been registered. These flags and the flags they include are set as the
     * active default flags.
     * 
     * This change only lasts for the life of the current adaptive environment
     * (e.g. until the adaptive server or afw command ends). If you want the
     * change to persist, change the defaultFlags property in the application
     * config.
     *
     * @param array $flagId The array of the flagId of flags used to determine
     *                      the default active flags.
     *
     * @return void
     */
    public function flag_replace_defaults(, $flagId)
    {
        $request = $this->$session->request();

        $request->set("function", "flag_replace_defaults");

        /* pass along required parameters to the request payload */
        $request->set("flagId", $flagId);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * flag_set()
     *
     * Set or unset one or more active xctx (request) flags.
     *
     * @param array $flagId List of flagId of flags to set or unset.
     * @param boolean $setTo Specify true to set and false to unset. If not
     *                       specified, flags are set.
     *
     * @return void
     */
    public function flag_set(, $flagId, $setTo = null)
    {
        $request = $this->$session->request();

        $request->set("function", "flag_set");

        /* pass along required parameters to the request payload */
        $request->set("flagId", $flagId);

        /* pass along any optional parameters to the request payload */
        if ($setTo != null)
            $request->set('setTo', $setTo);

        return $request->get_result();
    }

    /**
     * registry_key_check()
     *
     * This will check to see if a registry key exists for a specified
     * registry type and optionally load it's associated extension if needed.
     *
     * @param string $registryType This is the registry type, which is the
     *                             object id of a
     *                             /afw/_AdaptiveEnvironmentRegistryType_/
     *                             object.
     * @param string $key This is a key to check for existence in the
     *                    specified registryType.
     * @param boolean $loadExtension Specifying true for this optional
     *                               parameter will cause the associated
     *                               extension to be loaded if needed.
     *
     * @return boolean If false the extension was already loaded. If true, the
     *                 extension was successfully loaded which might have
     *                 caused side effects such as environment registry
     *                 changes. An error is thrown if there is a problem.
     */
    public function registry_key_check(, $registryType, $key, $loadExtension = null)
    {
        $request = $this->$session->request();

        $request->set("function", "registry_key_check");

        /* pass along required parameters to the request payload */
        $request->set("registryType", $registryType);
        $request->set("key", $key);

        /* pass along any optional parameters to the request payload */
        if ($loadExtension != null)
            $request->set('loadExtension', $loadExtension);

        return $request->get_result();
    }

    /**
     * service_get()
     *
     * Get a service object.
     *
     * @param string $serviceId The serviceId of the service.
     *
     * @return object _AdaptiveService_ object for the service which will
     *                contain the current status of the service. If there is
     *                an error, the status property value will be 'error' and
     *                'statusMessage' contain an error message.
     */
    public function service_get(, $serviceId)
    {
        $request = $this->$session->request();

        $request->set("function", "service_get");

        /* pass along required parameters to the request payload */
        $request->set("serviceId", $serviceId);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * service_restart()
     *
     * Restart a service.
     *
     * @param string $serviceId The serviceId of the service to restart.
     *
     * @return object _AdaptiveService_ object for the service which will
     *                contain the current status of the service. If there is
     *                an error, the status property value will be 'error' and
     *                'statusMessage' contain an error message.
     */
    public function service_restart(, $serviceId)
    {
        $request = $this->$session->request();

        $request->set("function", "service_restart");

        /* pass along required parameters to the request payload */
        $request->set("serviceId", $serviceId);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * service_start()
     *
     * Start a service.
     *
     * @param string $serviceId The serviceId of the service to start
     *
     * @return object _AdaptiveService_ object for the service which will
     *                contain the current status of the service. If there is
     *                an error, the status property value will be 'error' and
     *                'statusMessage' contain an error message.
     */
    public function service_start(, $serviceId)
    {
        $request = $this->$session->request();

        $request->set("function", "service_start");

        /* pass along required parameters to the request payload */
        $request->set("serviceId", $serviceId);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * service_stop()
     *
     * Stop a service.
     *
     * @param string $serviceId The serviceId of the service to stop.
     *
     * @return object _AdaptiveService_ object for the service which will
     *                contain the current status of the service. If there is
     *                an error, the status property value will be 'error' and
     *                'statusMessage' contain an error message.
     */
    public function service_stop(, $serviceId)
    {
        $request = $this->$session->request();

        $request->set("function", "service_stop");

        /* pass along required parameters to the request payload */
        $request->set("serviceId", $serviceId);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
