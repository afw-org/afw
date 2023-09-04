<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for model
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category model
 */
class model
{
    private $session;

    /**
     * Constructs the model class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * model_default_add_object_action()
     *
     * This function returns a _AdaptiveAction_ object for the default action
     * performed if a onAdd parameter is not specified on the associated
     * _AdaptiveModelObjectType_. The action is not performed, but be aware
     * that functions called while producing the _AdaptiveAction_ object may
     * cause side effects. This function can be called as part of a onAdd
     * expression or for testing.
     *
     * @param string $adaptorId This is the adaptorId of a model adaptor.
     *                          Variable custom::adaptorId can be used to
     *                          access this value in model expressions.
     * @param string $objectType This is the adaptive object type of object
     *                           being added. Variable custom::objectType can
     *                           be used to access this value in model
     *                           expressions.
     * @param object $object This is the object to add. Variable
     *                       custom::object can be used to access this value
     *                       in model expressions.
     * @param string $objectId This is the optional preferred objectId of
     *                         object to add. The adaptor may ignore this.
     *                         Variable custom::objectId can be used to access
     *                         this value in model expressions.
     * @param string $modelId This specifics a modelId of model to use for
     *                        producing results. If not specified, the
     *                        adaptor's current model will be used.
     * @param object $context This specifies additional context information
     *                        available to model expressions.
     *
     * @return object The default action object.
     */
    public function model_default_add_object_action(, $objectType, $object, $adaptorId = null, $objectId = null, $modelId = null, $context = null)
    {
        $request = $this->$session->request();

        $request->set("function", "model_default_add_object_action");

        /* pass along required parameters to the request payload */
        $request->set("objectType", $objectType);
        $request->set("object", $object);

        /* pass along any optional parameters to the request payload */
        if ($adaptorId != null)
            $request->set('adaptorId', $adaptorId);

        if ($objectId != null)
            $request->set('objectId', $objectId);

        if ($modelId != null)
            $request->set('modelId', $modelId);

        if ($context != null)
            $request->set('context', $context);

        return $request->get_result();
    }

    /**
     * model_default_delete_object_action()
     *
     * This function returns a _AdaptiveAction_ object for the default action
     * performed if a onDeleteObject parameter is not specified on the
     * associated _AdaptiveModelObjectType_. The action is not performed, but
     * be aware that functions called while producing the _AdaptiveAction_
     * object may cause side effects. This function can be called as part of a
     * onDelete expression or for testing.
     *
     * @param string $adaptorId This is the adaptorId of a model adaptor.
     *                          Variable custom::adaptorId can be used to
     *                          access this value in model expressions.
     * @param string $objectType This is the adaptive object type of object
     *                           being deleted. Variable custom::objectType
     *                           can be used to access this value in model
     *                           expressions.
     * @param string $objectId This is the objectId of object to delete.
     *                         Variable custom::object can be used to access
     *                         this value in model expressions.
     * @param string $modelId This specifics a modelId of model to use for
     *                        producing results. If not specified, the
     *                        adaptor's current model will be used.
     * @param object $context This specifies additional context information
     *                        available to model expressions.
     *
     * @return object The default action object.
     */
    public function model_default_delete_object_action(, $objectType, $objectId, $adaptorId = null, $modelId = null, $context = null)
    {
        $request = $this->$session->request();

        $request->set("function", "model_default_delete_object_action");

        /* pass along required parameters to the request payload */
        $request->set("objectType", $objectType);
        $request->set("objectId", $objectId);

        /* pass along any optional parameters to the request payload */
        if ($adaptorId != null)
            $request->set('adaptorId', $adaptorId);

        if ($modelId != null)
            $request->set('modelId', $modelId);

        if ($context != null)
            $request->set('context', $context);

        return $request->get_result();
    }

    /**
     * model_default_modify_object_action()
     *
     * This function returns a _AdaptiveAction_ object for the default action
     * performed if a onModifyObject parameter is not specified on the
     * associated _AdaptiveModelObjectType_. The action is not performed, but
     * be aware that functions called while producing the _AdaptiveAction_
     * object may cause side effects. This function can be called as part of a
     * onModify expression or for testing.
     *
     * @param string $adaptorId This is the adaptorId of a model adaptor.
     *                          Variable custom::adaptorId can be used to
     *                          access this value in model expressions.
     * @param string $objectType This is the adaptive object type of object
     *                           being modified. Variable custom::objectType
     *                           can be used to access this value in model
     *                           expressions.
     * @param string $objectId This is the objectId of object to modify.
     *                         Variable custom::objectId can be used to access
     *                         this value in model expressions.
     * @param array $entries This is an array of modifications. Variable
     *                       custom::actions can be used to access this value
     *                       in model expressions. Entries are of the form:
     * 
     *                           [         'add_value',         'property
     *                       name' or ['property name', ... ],         value  
     *                         ]
     * 
     *                           [         'remove_property',        
     *                       'property name' or ['property name', ... ]     ]
     * 
     *                           [         'remove_value',         'property
     *                       name' or ['property name', ... ],         value  
     *                         ]
     * 
     *                           [         'set_property',         'property
     *                       name' or ['property name', ... ],         value  
     *                         ]
     * @param string $modelId This specifics a modelId of model to use for
     *                        producing results. If not specified, the
     *                        adaptor's current model will be used.
     * @param object $context This specifies additional context information
     *                        available to model expressions.
     *
     * @return object The default action object.
     */
    public function model_default_modify_object_action(, $objectType, $objectId, $entries, $adaptorId = null, $modelId = null, $context = null)
    {
        $request = $this->$session->request();

        $request->set("function", "model_default_modify_object_action");

        /* pass along required parameters to the request payload */
        $request->set("objectType", $objectType);
        $request->set("objectId", $objectId);
        $request->set("entries", $entries);

        /* pass along any optional parameters to the request payload */
        if ($adaptorId != null)
            $request->set('adaptorId', $adaptorId);

        if ($modelId != null)
            $request->set('modelId', $modelId);

        if ($context != null)
            $request->set('context', $context);

        return $request->get_result();
    }

    /**
     * model_default_replace_object_action()
     *
     * This function returns a _AdaptiveAction_ object for the default action
     * performed if a onReplaceObject parameter is not specified on the
     * associated _AdaptiveModelObjectType_. The action is not performed, but
     * be aware that functions called while producing the _AdaptiveAction_
     * object may cause side effects. This function can be called as part of a
     * onReplace expression or for testing.
     *
     * @param string $adaptorId This is the adaptorId of a model adaptor.
     *                          Variable custom::adaptorId can be used to
     *                          access this value in model expressions.
     * @param string $objectType This is the adaptive object type of object
     *                           being replaced. Variable custom::objectType
     *                           can be used to access this value in model
     *                           expressions.
     * @param string $objectId The objectId of object to replace. Variable
     *                         custom::objectId can be used to access this
     *                         value in model expressions.
     * @param object $object This is the object to replace. Variable
     *                       custom::object can be used to access this value
     *                       in model expressions.
     * @param string $modelId This specifics a modelId of model to use for
     *                        producing results. If not specified, the
     *                        adaptor's current model will be used.
     * @param object $context This specifies additional context information
     *                        available to model expressions.
     *
     * @return object The default action object.
     */
    public function model_default_replace_object_action(, $objectType, $objectId, $object, $adaptorId = null, $modelId = null, $context = null)
    {
        $request = $this->$session->request();

        $request->set("function", "model_default_replace_object_action");

        /* pass along required parameters to the request payload */
        $request->set("objectType", $objectType);
        $request->set("objectId", $objectId);
        $request->set("object", $object);

        /* pass along any optional parameters to the request payload */
        if ($adaptorId != null)
            $request->set('adaptorId', $adaptorId);

        if ($modelId != null)
            $request->set('modelId', $modelId);

        if ($context != null)
            $request->set('context', $context);

        return $request->get_result();
    }

    /**
     * model_mapBackObject_signature()
     *
     * This is the function signature for current:mapBackObject used by
     * onGetObjects and onRetrieveObjects. Calling this directly will throw a
     * 'Do not call directly' error.
     *
     * @param object $mappedObject This is the object to map back.
     *
     * @return object This is the mappedObject mapped back from mapped adaptor
     *                object.
     */
    public function model_mapBackObject_signature(, $mappedObject)
    {
        $request = $this->$session->request();

        $request->set("function", "model_mapBackObject_signature");

        /* pass along required parameters to the request payload */
        $request->set("mappedObject", $mappedObject);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * model_mapObject_signature()
     *
     * This is the function signature for current:mapObject used by
     * onAddObject. onDeleteObject, onModifyObject, and onReplaceObject.
     * Calling this directly will throw a 'Do not call directly' error.
     *
     * @param object $object This is the object to map.
     *
     * @return object This is the object mapped to mappedAdaptor object.
     */
    public function model_mapObject_signature(, $object)
    {
        $request = $this->$session->request();

        $request->set("function", "model_mapObject_signature");

        /* pass along required parameters to the request payload */
        $request->set("object", $object);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * model_returnObject_signature()
     *
     * This is the function signature for current:returnObject used by
     * onGetObjects and onRetrieveObjects. Calling this directly will throw a
     * 'Do not call directly' error.
     *
     * @param object $object This is the object to return.
     * @param boolean $userData If this is present and true, the object will
     *                          be mapped its mapped adaptor's object type to
     *                          the model adaptor's object type.
     *
     * @return boolean This will return true if no more objects can be
     *                 returned for any reason including a limit exceeded,
     *                 connection closed, or server stopping.
     */
    public function model_returnObject_signature(, $object, $userData = null)
    {
        $request = $this->$session->request();

        $request->set("function", "model_returnObject_signature");

        /* pass along required parameters to the request payload */
        $request->set("object", $object);

        /* pass along any optional parameters to the request payload */
        if ($userData != null)
            $request->set('userData', $userData);

        return $request->get_result();
    }

}

?>
