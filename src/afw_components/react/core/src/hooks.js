// See the 'COPYING' file in the project root for licensing information.
/**
 * AFW React Core Hooks
 *
 * Custom React Hooks for accessing AFW React Core data
 * and routines.
 */

import {useContext, useState, useEffect, useCallback, useReducer, useDebugValue, useMemo, useRef} from "react";
import {css} from "@emotion/css";
import {useTheme} from "@emotion/react";
//import {unstable_trace as trace} from "scheduler/tracing";

import {
    ComponentsContext,
    AdaptiveLayoutContext,
    ModelContext,
    OperationalContext,
    QualifiersContext,
    RouteBasePathContext,
    ClipboardContext,
    ObjectEditorContext,
} from "./context";


function isSameObjectType(obj1, obj2) {
    return (obj1.constructor.name === obj2.constructor.name);
}

/**
 * A very simple hook for forcing a functional component to update via hooks.
 */
export const useForceUpdate = () => {
    const [, forceUpdate] = useState();

    return useCallback(() => forceUpdate({}), []);
};

/**
 * useIsMounted()
 * 
 * A hook to return the mounted state of a component.
 */
export const useIsMounted = () => {
    const mountedRef = useRef(false);
    const isMounted = useCallback(() => mountedRef.current, []);

    useEffect(() => {
        mountedRef.current = true;

        return () => {
            mountedRef.current = false;
        };
    }, []);

    return isMounted;
};

/**
 * useObjectEditor()
 * 
 * A hook to get the ObjectEditor properties, used by layouts to 
 * render an object inside an ObjectEditor context.
 */
export const useObjectEditor = (props) => {
    const ctx = useContext(ObjectEditorContext);

    const value = useMemo(() => {
        let value = ctx;

        if (props && Object.prototype.hasOwnProperty.call(props, "onRenderHeader")) {
            value = { ...value, onRenderHeader : props.onRenderHeader };
        }

        return value;
    }, [ctx, props]);

    return value;
};

/**
 * useComponents()
 *
 * A simple hook that wraps the useContext hook to provide
 * consumers with Components context data.
 */
export const useComponents = () => {
    return useContext(ComponentsContext).components;
};

/**
 * useModel()
 *
 * A simple hook that wraps the useContext hook to provide
 * consumers with the adaptiveModel context data.
 */
export const useModel = () => {
    const model = useContext(ModelContext);

    useDebugValue(model ? "[model]" : "[no model]");

    return model;
};

/**
 * useClipboard()
 *
 * A simple hook that wraps useContext to provide consumers
 * with access to the adaptiveModel's clipboard data.
 */
export const useClipboard = () => {
    return useContext(ClipboardContext);
};

/**
 * useOperational(props)
 *
 * A hook to know if the current content is editable, using
 * a combination of Context and props.
 */
export const useOperational = (props = {}) => {
    const ctx = useContext(OperationalContext);
    const { editable, add } = props;

    let operational = ctx ? ctx : {};
    if (ctx || (editable !== undefined) || (add !== undefined)) {
        let op = {};

        if (editable !== undefined)
            op.editable = editable;

        if (add !== undefined)
            op.add = add;

        operational = { ...ctx, ...op };
    }  

    return operational;
};

/**
 * useRouteBasePath()
 *
 * Hook to get the "route base path", used to create a 
 * Link href, based in reference to the provider's 
 * location.
 */
export const useRouteBasePath = () => {
    return useContext(RouteBasePathContext);
};

/**
 *
 * @param {*} object The object to subscribe to.
 * @return [savable, changed]
 *
 * This hook adds event listeners to the object in order to detect state
 * changes for when the object has changed and/or is savable.
 */
export const useEventId = ({ object, property, eventId }) => {

    const [event, setEvent] = useState();
    const isMounted = useIsMounted();

    useDebugValue(eventId + "(" + (object ? object.getObjectId() : property ? property.getName() : "[object]") + ")");

    useEffect(() => {
        if (object && eventId) {
            const onEvent = object.addEventListener(eventId, 
                (event) => { 
                    if (isMounted()) {
                        if (eventId === "onSavable") {                            
                            setEvent(evt => {
                                if (!evt || evt.value !== event.value)
                                    return event;
                                else 
                                    return evt;
                            });
                        } else
                            setEvent(event); 
                    }
                }
            );

            return () => {
                object.removeEventListener(eventId, onEvent);                
            };
        } else if (property && eventId) {
            const onEvent = property.addEventListener(eventId, 
                (event) => { 
                    if (isMounted()) 
                        setEvent(event); 
                }
            );

            return () => {
                property.removeEventListener(eventId, onEvent);                
            };
        }

    }, [object, property, eventId, isMounted]);

    return event;
};

/**
 * useValues()
 *
 * This hook gets all of the adaptive property/values of an object
 * and returns a new copy anytime the object changes.
 */
export const useValues = (object) => {

    const [values, setValues] = useState({});
    const isMounted = useIsMounted();

    useDebugValue(object ? object.getPath() : "[object]");

    useEffect(() => {
        if (object) {            
            setValues( object.getPropertyValues() );

            const onChanged = object.addEventListener("onChanged", () => {
                if (isMounted())
                    setValues({ ...object.getPropertyValues()} );
            });

            const onObjectChanged = object.addEventListener("onObjectChanged", () => {
                if (isMounted())
                    setValues({ ...object.getPropertyValues()} );
            });

            const onChildChanged = object.addEventListener("onChildChanged", () => {
                if (isMounted())
                    setValues({ ...object.getPropertyValues()} );
            });

            return () => {                
                object.removeEventListener("onChanged", onChanged);
                object.removeEventListener("onObjectChanged", onObjectChanged);
                object.removeEventListener("onChildChanged", onChildChanged);                
            };
        }
    }, [object, isMounted]);

    return values;
};

const getObject_initialState = { isLoading: false, savable: false };

const getObjectReducer = (state, action) => {
    switch (action.type) {

    case "getting":
        return ({ 
            ...state, 
            isLoading: true, 
            object: undefined, 
            savable: false, 
        });

    case "got":
        return ({ 
            ...state, 
            object: action.object,             
            isLoading: false, 
            error: undefined,        
            refresh: undefined,                
        });

    case "invalidate":
        return ({ 
            ...state, 
            object: isSameObjectType(action.object, state.object) ? action.object : state.object,
            isLoading: false, 
            error: undefined,        
            refresh: undefined,                
        });

    case "error":
        return ({
            ...state,
            object: undefined,
            isLoading: false,
            error: action.error,
        });    

    case "savable":
        // avoid excessive rendering if savable value hasn't changed
        if (state.savable && (state.savable.value === action.savable.value))
            return state;

        return ({
            ...state,
            savable: action.savable,
        });

    case "discard":
        return ({
            ...state,
            savable: false,            
        });

    case "refresh":
        return ({
            ...state,
            isLoading: true,
            object: undefined,
            savable: false,
            refresh: true
        });

    case "reset":
        return getObject_initialState;

    default:
        return state;

    }    
};

/**
 * useGetObject()
 *
 * This hook gets an adaptive object, returning the object, whether it
 * is loading, or if an error occurred.  Additionally, it will
 * automatically trigger re-renders when the object has changed.
 */
export const useGetObject = ({ objectUri, adapterId, objectTypeId, objectId, objectOptions, modelOptions, cache = false }) => {

    const [state, dispatch] = useReducer(getObjectReducer, getObject_initialState);    
    const model = useModel();      

    const key = (objectUri ? objectUri : ("/" + adapterId + "/" + objectTypeId + "/" + objectId));
    useDebugValue(key);

    const onRefresh = () => dispatch({ type: "refresh" });

    /* When the object has changed, register a new event listener */
    useEffect(() => {      
        let onInvalidate, onSavable, onDiscardChanges;  

        if (state.object) {            
            if (!(modelOptions && modelOptions.adaptiveObject === false)) {
                onSavable = state.object.addEventListener("onSavable", (savable) => {                
                    dispatch({ type: "savable", savable });
                });
                onDiscardChanges = state.object.addEventListener("onDiscardChanges", () => {
                    dispatch({ type: "discard" });
                });
            }

            onInvalidate = model.cacheOnInvalidate(key, (_key, _object) => {            
                if (_key && _object)
                    dispatch({ type: "invalidate", object: _object });
                else
                    dispatch({ type: "refresh" });
            });

            return () => {         
                if (onSavable)       
                    state.object.removeEventListener("onSavable", onSavable);
                if (onDiscardChanges)
                    state.object.removeEventListener("onDiscardChanges", onDiscardChanges);    
                if (onInvalidate)
                    model.cacheOffInvalidate(key, onInvalidate);                
            };
        }
        
    }, [model, state.object, modelOptions, key]);

    /* Use the getObject() method to get the adaptive object, via model. */
    useEffect(() => {
        let response;        
        
        const getObject = async () => {
            dispatch({ type: "getting" });
                
            try {
                let object;

                if (objectUri) {
                    response = model.getObjectWithUri({ uri: objectUri, objectOptions, modelOptions });
                } else {
                    response = model.getObject({ objectTypeId, objectId, adapterId, objectOptions, modelOptions });
                }

                object = await response.object;                

                if (cache)
                    model.cacheSet(key, object);                                    
           
                if (!response.controller.signal.aborted)
                    dispatch({ type: "got", object });
            } catch (error) {
                if (!response || !response.controller.signal.aborted)
                    dispatch({ type: "error", error });
            }
        };

        if (model && key && cache && model.cacheGet(key)) {            
            dispatch({ type: "got", object: model.cacheGet(key) });            
        } else if (model && (objectUri || (adapterId && objectTypeId && objectId))) {                
            getObject();
        } else {
            /* If we no longer have an adapterId/objectTypeId/objectId, reset the state */
            dispatch({ type: "reset" });
        }

        return () => {       
            if (response) {                       
                response.controller.abort();
            }
        };
        

    }, [model, key, cache, objectUri, adapterId, objectTypeId, objectId, objectOptions, modelOptions, state.refresh]);

    return { ...state, onRefresh };
};


const retrieveObjects_initialState = { isLoading: false };

const retrieveObjectsReducer = (state, action) => {
    switch (action.type) {

    case "retrieving":
        return { ...state, isLoading: true };

    case "retrieved":        
        return ({ 
            ...state, 
            objects: action.objects, 
            isLoading: false, error: undefined,    
            refresh: false,        
        });

    case "retrieved_cache":        
        return ({ 
            ...state, 
            objects: action.objects, 
            isLoading: false, error: undefined,    
            refresh: false,        
        });

    case "error":
        return ({
            ...state,
            isLoading: false,
            error: action.error,
            refresh: false,
        });

    case "refresh":
        return ({
            ...state,
            isLoading: true,
            refresh: true,
            objects: undefined,
        });

    case "reset":
        return retrieveObjects_initialState;

    default:
        return state;        

    }
};

const calcKey = ({ adapterId, objectTypeId, objectOptions, modelOptions, queryCriteria }) => {
    if (!adapterId || !objectTypeId)
        return null;

    let key = "/" + adapterId + "/" + objectTypeId + "/";
    if (objectOptions)
        key = key + ";" + JSON.stringify(objectOptions);
    if (modelOptions)
        key = key + ";" + JSON.stringify(modelOptions);
    if (queryCriteria)
        key = key + "?" + queryCriteria;

    return key;
};

/**
 * useRetrieveObjects()
 *
 * This hook retrieves adaptive objects, returning the objects, whether
 * they are loading, or if an error has occurred.
 */
export const useRetrieveObjects = ({ adapterId, objectTypeId, queryCriteria, objectOptions, modelOptions, cache = false }) => {
    
    const [state, dispatch] = useReducer(retrieveObjectsReducer, retrieveObjects_initialState);    
    
    useDebugValue(state.isLoading ? "Loading" : state.error ? "Found Error" : state.objects ? (state.objects.length + " Objects") : "[Unknown]");

    const model = useModel();    
    
    const onRefresh = () => dispatch({ type: "refresh" });

    const key = calcKey({ adapterId, objectTypeId, queryCriteria, objectOptions, modelOptions });

    useEffect(() => {
        let response;
        let onInvalidate;

        const retrieveObjects = async () => {            
            dispatch({ type: "retrieving" });
            
            try {
                response = model.retrieveObjects({
                    objectTypeId, adapterId, queryCriteria, objectOptions, modelOptions
                });

                const objects = await response.objects;
                
                if (cache && key)
                    model.cacheSet(key, objects);    

                if (!response.controller.signal.aborted) {
                    dispatch({ type: "retrieved", objects });                    
                }

            } catch (error) {
                if (!response.controller.signal.aborted)
                    dispatch({ type: "error", error });
            }
        };

        if (cache && key && model.cacheGet(key) && !state.refresh) {
            dispatch({ type: "retrieved_cache", objects: model.cacheGet(key) });
        } else if (model && adapterId && objectTypeId) {                 
            retrieveObjects();
        } else {
            /* If we no longer have an adapterId/objectTypeId, reset the state */
            dispatch({ type: "reset" });
        }

        return () => { 
            if (onInvalidate) {
                model.cacheOffInvalidate(key, onInvalidate);
            }
            if (response) {
                response.controller.abort();
            }
        };

    }, [model, cache, key, adapterId, objectTypeId, queryCriteria, objectOptions, modelOptions, state.refresh]);

    useEffect(() => {
        let onInvalidate;

        if (key && state.objects && cache) {
            onInvalidate = model.cacheOnInvalidate(key, (_key, _objects) => {
                if (_key && _objects)
                    dispatch({ type: "retrieved", objects: _objects });
                else
                    dispatch({ type: "refresh" });
            });
        }

        return () => {
            if (onInvalidate)
                model.cacheOffInvalidate(onInvalidate);
        };
    }, [model, key, state.objects, cache]);

    return { ...state, onRefresh };
};

/* some static object options that never need to be re-rendered */
export const objectOptions_objectId                 = { objectId: true };
export const objectOptions_objectId_path            = { objectId: true, path: true };
export const objectOptions_composite                = { composite: true };
export const objectOptions_objectId_composite       = { objectId: true, composite: true };
export const objectOptions_objectId_path_composite  = { objectId: true, path: true, composite: true };
export const objectOptions_reconcilable             = { reconcilable: true };
export const objectOptions_objectId_objectType_path = { objectId: true, objectType: true, path: true };
export const modelOptions_noAdaptiveObject          = { adaptiveObject: false };


/**
 * useEnvironmentRegistry()
 * 
 * A hook that fetches the _AdaptiveEnvironmentRegistry_/current object
 */
export const useEnvironmentRegistry = () => {
    const {object: registry, ...rest} = useGetObject({
        objectUri: "/afw/_AdaptiveEnvironmentRegistry_/current", cache: true,
        modelOptions: modelOptions_noAdaptiveObject,
    });

    return { registry, ...rest };
};

/**
 * useDataTypes()
 *
 * A hook that fetches adaptive dataType objects.
 */
export const useDataTypes = () => {    
    
    const {registry, ...rest} = useEnvironmentRegistry();
    const dataTypes = useMemo(() => 
        registry ? Object.values(registry.data_type) : undefined, [registry]);

    return { dataTypes, ...rest };
};

/**
 * useFunctionCategories()
 *
 * A hook that fetches adaptive function categories.
 */
export const useFunctionCategories = () => {
    const {objects, ...rest} = useRetrieveObjects({
        adapterId: "afw", objectTypeId: "_AdaptiveFunctionCategory_",
        objectOptions: objectOptions_objectId, cache: true,
        modelOptions: modelOptions_noAdaptiveObject,
    });

    return { categories: objects, ...rest };
};

/* useFunctions()
 *
 * A hook that fetches adaptive function objects.
 */
export const useFunctions = () => {

    const {registry, ...rest} = useEnvironmentRegistry();

    const functions = useMemo(() => {
        if (registry && registry.function) {
            return Object.values(registry.function).sort((a, b) => {
                const a_cat = a.category.toLowerCase();
                const b_cat = b.category.toLowerCase();

                if (a_cat < b_cat)
                    return -1;
                else if (a_cat > b_cat)
                    return 1;
                else {
                    const a_label = a.functionId.toLowerCase();
                    const b_label = b.functionId.toLowerCase();

                    return (a_label.localeCompare(b_label));                    
                }
            });
        }
    }, [registry]);

    return { functions, ...rest };
};

/**
 * useObjectTypes()
 *
 * A hook that fetches core adaptive object type objects.
 */
export const useObjectTypes = () => {

    const {objects: objectTypes, ...rest} = useRetrieveObjects({
        adapterId: "afw", objectTypeId: "_AdaptiveObjectType_",
        objectOptions: objectOptions_objectId_path_composite,
        cache: true
    });

    return { objectTypes, ...rest };
};

/**
 * useObjectType()
 * 
 * A hook that gets the core adaptive object type specified by
 * the objectTypeId.
 */
export const useObjectType = (objectTypeId) => {

    const {objectTypes, ...rest} = useObjectTypes();
    const objectType = useMemo(() => {
        if (objectTypes) {
            for (const o of objectTypes) {
                if (o.getObjectId() === objectTypeId)
                    return o;
            }
        }
    }, [objectTypeId, objectTypes]);

    return { objectType, ...rest };
};

/**
 * useAdapters()
 *
 * A hook that fetches adapter objects.
 */
export const useAdapters = (adapterType) => {    

    const {registry, ...rest} = useEnvironmentRegistry();

    const adapters = useMemo(() => {
        let adapters;
        if (registry) {
            adapters = Object.values(registry.adapter_id);
            if (adapterType)
                adapters = adapters.filter(a => a.properties?.adapterType === adapterType);
        }

        return adapters;
    }, [registry, adapterType]);
    
    return { adapters, ...rest };
};

/**
 * useServices()
 * 
 * A hook that fetches adaptive service objects.
 */
export const useServices = () => {
  
    const {objects: services, ...rest} = useRetrieveObjects({
        adapterId: "afw", objectTypeId: "_AdaptiveService_",
        objectOptions: objectOptions_objectId,
        cache: true
    });

    return { services, ...rest };
};

/**
 * useExtensions()
 *
 * A hook that fetches extension objects.
 */
export const useExtensions = () => {    

    const {registry, ...rest} = useEnvironmentRegistry();
    const extensions = useMemo(() => 
        registry ? Object.values(registry.extension) : undefined, [registry]);
    
    return { extensions, ...rest };
};

/**
 * useExtensionManifests()
 * 
 * A hook that fetches the extensions available to the manifest.
 */
export const useExtensionManifests = () => {

    const {objects: manifests, ...rest} = useRetrieveObjects({
        adapterId: "afw", objectTypeId: "_AdaptiveManifest_",
        cache: true
    });

    return { manifests, ...rest };
};

/**
 * useRequestHandlers()
 *
 * A hook that fetches adapter objects.
 */
export const useRequestHandlers = () => {    

    const {registry, ...rest} = useEnvironmentRegistry();
    const requestHandlers = useMemo(() =>
        registry ? Object.values(registry.request_handler) : undefined, [registry]);
    
    return { requestHandlers, ...rest };
};

/**
 * useQualifiers()
 *
 * A hook that gets contextual qualifiers.
 */
export const useQualifiers = (/*context*/) => {
    return useContext(QualifiersContext);
};


/**
 * useContextTypes()
 *
 * A hook that fetches _AdaptiveContextType_ objects.
 */
export const useContextTypes = () => {
    const {registry, ...rest} = useEnvironmentRegistry();
    const contextTypes = useMemo(() => 
        registry ? Object.values(registry.context_type) : undefined, [registry]);

    return { contextTypes, ...rest };
};

/**
 * useFlags()
 *
 * A hook that fetches the _AdaptiveFlag_ objects.
 */
export const useFlags = () => {    

    const {registry, ...rest} = useEnvironmentRegistry();

    const flags = useMemo(() => {
        if (registry && registry.flag) {
            return Object.values(registry.flag).sort((a, b) => {
                return (a.flagId.toLowerCase().localeCompare(
                    b.flagId.toLowerCase()));
            });
        }
    }, [registry]);

    return { flags, ...rest };
};

/**
 * useLayoutComponentTypes()
 *
 * A hook that fetches the _AdaptiveLayoutComponentType_ objects.
 */
export const useLayoutComponentTypes = () => {
    const {objects: layoutComponentTypes, ...rest} = useRetrieveObjects({
        adapterId: "afw", objectTypeId: "_AdaptiveLayoutComponentType_",
        modelOptions: modelOptions_noAdaptiveObject,
        objectOptions: objectOptions_objectId_composite, cache: true
    });

    return { layoutComponentTypes, ...rest };
};

/**
 * useLayoutComponentTypeCategories()
 *
 * A hook that fetches the _AdaptiveLayoutComponentType_ objects.
 */
export const useLayoutComponentTypeCategories = () => {
    const {objects: layoutComponentTypeCategories, ...rest} = useRetrieveObjects({
        adapterId: "afw", objectTypeId: "_AdaptiveLayoutComponentTypeCategory_",
        modelOptions: modelOptions_noAdaptiveObject,
        objectOptions: objectOptions_objectId_composite, cache: true
    });

    return { layoutComponentTypeCategories, ...rest };
};

/**
 * A hook that fetches the _AdaptiveObjectOption_ objects.
 */
export const useObjectOptions = () => {
    const {object: objectOptions, ...rest} = useGetObject({
        objectUri: "/afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_", 
        modelOptions: modelOptions_noAdaptiveObject,
        cache: true
    });

    return { objectOptions, ...rest };
};

/**
 * A hook that fetches the _AdaptiveVersionInfo_ objects.
 */
export const useVersionInfo = () => {
    const {objects: versions, ...rest} = useRetrieveObjects({
        adapterId: "afw", objectTypeId: "_AdaptiveVersionInfo_",
        objectOptions: objectOptions_objectId_composite, cache: true
    });

    return { versions, ...rest};
};

/**
 * A hook that fetches the _AdaptiveServiceConf_ objects.
 */
export const useServiceConfigurations = ({ adapterId }) => {
    const {objects: serviceConfigs, ...rest} = useRetrieveObjects({
        adapterId, objectTypeId: "_AdaptiveServiceConf_",
        objectOptions: objectOptions_objectId_path_composite, cache: true
    });

    return { serviceConfigs, ...rest };
};

/**
 * A hook that fetches the _AdaptiveSystemInfo_ objects
 */
export const useSystemInfo = () => {
    const {objects: systemInfo, ...rest} = useRetrieveObjects({
        adapterId: "afw", objectTypeId: "_AdaptiveSystemInfo_",
        modelOptions: modelOptions_noAdaptiveObject,
        objectOptions: objectOptions_objectId_composite, cache: true
    });

    return { systemInfo, ...rest };
};

export const useServer = () => {
    const {object: server, ...rest} = useGetObject({
        objectUri: "/afw/_AdaptiveServer_/current", 
        objectOptions: objectOptions_composite,
        modelOptions: modelOptions_noAdaptiveObject,
        cache: true
    });

    return { server, ...rest };
};

/**
 * useAdaptiveLayout()
 * 
 * A hook that returns layout describing how components should be displayed. 
 * 
 */
export const useAdaptiveLayout = (props, propTypes, defaultProps) => {

    const ctx = useContext(AdaptiveLayoutContext);    
    
    const layout = useMemo(() => {
        let layout = ctx ? (ctx.layoutParameters && ctx.data) ? ctx : { data: {...ctx.data}, layoutParameters: {...ctx.layoutParameters}}  : { data: {}, layoutParameters: {} };

        /* allow explicit props to override */
        if (props) {
            let newLayout = { data: {...ctx.data}, layoutParameters: {...ctx.layoutParameters} };

            if (propTypes) {
                /* merge each property with our layoutParameters from context */
                for (const p of Object.keys(propTypes)) {
                    if (props[p] !== undefined && newLayout.layoutParameters[p] !== undefined) {
                        /* we have values derived from context and from props, so let props override */
                        newLayout.layoutParameters[p] = props[p];
                    } else if (props[p] !== undefined) {
                        /* parent props have something context does not have, so use it */
                        newLayout.layoutParameters[p] = props[p];
                    } else if (newLayout.layoutParameters[p] === undefined && props[p] === undefined && defaultProps && (defaultProps[p] !== undefined)) {
                        /* it wasn't defined by props, or by context, so use default values */
                        newLayout.layoutParameters[p] = defaultProps[p];
                    }
                }
            }

            if (props.object)
                newLayout.data.object = props.object;

            if (props.property)                
                newLayout.data.property = props.property;            

            if (props.value !== undefined)
                newLayout.data.value = props.value;

            layout = { 
                data: { ...layout.data, ...newLayout.data },
                layoutParameters: { ...layout.layoutParameters, ...newLayout.layoutParameters }
            };
        }

        return layout;

    }, [props, propTypes, defaultProps, ctx]);    

    return layout;
};

/**
 * useLayoutParameters()
 * 
 * A hook that returns layout parameters describing how components should 
 * be displayed.  These parameters, in essence, override local props, but are
 * declared at the top of a context, globally.
 */
export const useLayoutParameters = (props, propTypes, defaultProps) => {

    const ctx = useContext(AdaptiveLayoutContext);
    let layoutParameters = ctx ? {...ctx.layoutParameters} : {};

    /* allow explicit props to override */
    if (props) {
        if (propTypes) {
            /* merge each property with our layoutParameters from context */
            for (const p of Object.keys(propTypes)) {
                if (props[p] !== undefined && layoutParameters[p] !== undefined) {
                    /* we have values derived from context and from props, so let parent override */
                    layoutParameters[p] = props[p];
                } else if (props[p] !== undefined) {
                    /* parent props have something context does not have, so use it */
                    layoutParameters[p] = props[p];
                } else if (layoutParameters[p] === undefined && props[p] === undefined && defaultProps && (defaultProps[p] !== undefined)) {
                    /* it wasn't defined by parent, or by context, so use default values */
                    layoutParameters[p] = defaultProps[p];
                }
            }
        } else {
            /* just check to see if any props should override */
            for (const p of Object.keys(props)) {
                if (props[p] !== undefined && layoutParameters[p] !== undefined) {
                    layoutParameters[p] = props[p];
                }
            }
        }
    }

    return layoutParameters;
};

/** 
 * useAdaptiveLayoutData()
 * 
 * A hook, similar to useAdaptiveLayout, but only returns the data inside.
 */
export const useAdaptiveLayoutData = (props) => {
    let ctx = useContext(AdaptiveLayoutContext);

    if (ctx && props && (props.object || props.property || props.value !== undefined)) {
        let data = { ...ctx.data };

        if (props.object)
            data.object = props.object;
        if (props.property)
            data.property = props.property;
        if (props.value !== undefined)
            data.value = props.value;
        return { ...data };
    }
    
    else if (ctx)
        return ctx.data;
};



/**
 * useAfwObject()
 *
 * This hook gets an AfwObject instance out of context, and 
 * can be overridden by props.
 */
export const useAfwObject = (props) => {

    const data = useAdaptiveLayoutData(props);

    if (props && props.object)
        return props.object;

    else if (data)
        return data.object;
};

/**
 * useAfwProperty()
 *
 * This hook gets an AfwProperty instance out of context, and 
 * can be overridden by props.
 */
export const useAfwProperty = (props) => {

    const data = useAdaptiveLayoutData(props);

    if (props && props.property)
        return props.property;

    else if (data) {
        return data.property;    
    }
};

/**
 * useAfwValue()
 *
 * This hook gets an AfwValue instance out of context, and 
 * can be overridden by props.
 */
export const useAfwValue = (props) => {

    const data = useAdaptiveLayoutData(props);

    if (props?.value !== undefined)
        return props.value;

    else return data?.value;
};


/**
 * \fixme Temporary hook to migrate operations out of
 *        @afw/client.  This is used to preload data
 *        for the admin application, and orders the hooks
 *        to run in a specific order, as some depend on others. 
 */
export const useLoadApplicationData = () => {

    const model = useModel();    
    const [error, setError] = useState();
    const [environment, setEnvironment] = useState();
    const [afwComponents, setAfwComponents] = useState();
    const [objectTypeObjects, setObjectTypeObjects] = useState();
    const [appData, setAppData] = useState();
    const [application, setApplication] = useState();
    const [services, setServices] = useState();    
    const isMounted = useIsMounted();
    
    useEffect(() => {
        let response;

        const loadApplicationComponents = async (model) => {
            try {
                response = model.getObjectWithUri({
                    uri: "/afw/_AdaptiveApplicationComponents_/current",
                    modelOptions: { adaptiveObject: false }
                });

                const object = await response.object;
                
                if (!response.controller.signal.aborted)
                    setAfwComponents(object);
            } catch (error) {
                if (!response || !response.controller.signal.aborted)
                    setError(error);
            }
        };

        if (model) 
            loadApplicationComponents(model);

        return () => {
            if (response)
                response.controller.abort();
        };
    }, [model]);

    useEffect(() => {
        let response;

        const preloadObjectTypes = async (model) => {
            try {
                const objectTypes = await model.loadObjectTypes({ adapterId: "afw" });
                
                for (const objectType of objectTypes) {
                    if (isMounted())
                        await objectType.initialize();
                }

                const objectTypeObjects = Object.assign( 
                    ...objectTypes.map(i => ({ [i.getPath()] : i }))
                );

                if (isMounted())
                    setObjectTypeObjects(objectTypeObjects);
            } catch (error) {
                if (isMounted() && !response || !response.controller.signal.aborted)
                    setError(error);   
            }
        };

        if (afwComponents && model)
            preloadObjectTypes(model);

        return () => {
            if (response)
                response.controller.abort();            
        };

    }, [afwComponents, model, isMounted]);

    const dataFromEnvironment = (environment) => {
        
        let functions = Object.values(environment.function);
        let dataTypes = Object.values(environment.data_type);
        const extensions = Object.values(environment.extension);
        const adapters = Object.values(environment.adapter_id);
        const logs = Object.values(environment.log);
        const requestHandlers = Object.values(environment.request_handler);
        const authHandlers = Object.values(environment.authorization_handler_id);
        const contentTypes = Object.values(environment.content_type);
        const contextTypes = Object.values(environment.context_type);
        const flags = Object.values(environment.flag);
        const qualifiers = contextTypes.qualifierDefinitions;   
        
        functions.sort((a, b) => {
            const a_cat = a.category.toLowerCase();
            const b_cat = b.category.toLowerCase();

            if (a_cat < b_cat)
                return -1;
            else if (a_cat > b_cat)
                return 1;
            else {
                const a_label = a.functionId.toLowerCase();
                const b_label = b.functionId.toLowerCase();

                return (a_label.localeCompare(b_label));
            }
        });

        dataTypes = dataTypes.sort((a, b) => {
            const a_name = a.dataType.toLowerCase();
            const b_name = b.dataType.toLowerCase();

            return a_name.localeCompare(b_name);
        });

        return { 
            functions, 
            dataTypes, 
            extensions, 
            adapters, 
            logs, 
            requestHandlers, 
            authHandlers, 
            contentTypes, 
            contextTypes, 
            flags, 
            qualifiers 
        };        
    };

    useEffect(() => {
        let response;

        const loadEnvironmentRegistry = async (model) => {
            const uri = "/afw/_AdaptiveEnvironmentRegistry_/current";

            try {                
                response = model.getObjectWithUri({
                    uri,
                    modelOptions: { adaptiveObject: false }
                });

                const environment = await response.object;
                if (environment) {
                    const data = dataFromEnvironment(environment);

                    if (!response.controller.signal.aborted) {
                        model.cacheSet(uri, environment);
                        setEnvironment(environment);
                        setAppData({ environment, ...data });                        
                    }
                }
                    
            } catch (error) {
                if (!response || !response.controller.signal.aborted)
                    setError(error);
            }
        };

        if (model && objectTypeObjects && model) {
            loadEnvironmentRegistry(model);
        }

        return () => {
            if (response)
                response.controller.abort();
        };

    }, [model, objectTypeObjects]);

    useEffect(() => {
        let response;

        const loadServices = async (model) => {
            try {
                response = model.retrieveObjects({
                    adapterId: "afw", objectTypeId: "_AdaptiveService_",                
                });

                const services = await response.objects;

                if (!response.controller.signal.aborted)
                    setServices(services);
            } catch (error) {
                if (!response || !response.controller.signal.aborted)
                    setError(error);
            }
        };

        if (model && appData) 
            loadServices(model);

        return () => {
            if (response)
                response.controller.abort();
        };
    }, [model, appData]);

    useEffect(() => {
        let response;

        const loadApplication = async (model) => {
            try {
                response = model.getObjectWithUri({
                    uri: "/afw/_AdaptiveApplication_/current",
                });

                const object = await response.object;
                
                if (!response.controller.signal.aborted)
                    setApplication(object);
            } catch (error) {
                if (!response || !response.controller.signal.aborted)
                    setError(error);
            }
        };

        if (model && appData) 
            loadApplication(model);

        return () => {
            if (response)
                response.controller.abort();
        };
    }, [model, appData]);


    const refresh = useCallback(async () => {
        let response;

        const uri = "/afw/_AdaptiveEnvironmentRegistry_/current";
        try {                
            response = model.getObjectWithUri({
                uri,
                modelOptions: { adaptiveObject: false }
            });

            const environment = await response.object;
            if (environment) {
                const data = dataFromEnvironment(environment);

                if (isMounted() && !response.controller.signal.aborted) {
                    /* invalidate any model cache */
                    //model.cacheInvalidate();

                    model.cacheSet(uri, environment);
                    setEnvironment(environment);
                    setAppData({ environment, ...data });                                            
                }
            }

            response = model.retrieveObjects({
                adapterId: "afw", objectTypeId: "_AdaptiveService_",                
            });

            const services = await response.objects;

            if (isMounted() && !response.controller.signal.aborted)
                setServices(services);
                
        } catch (error) {
            if (isMounted() && !response || !response.controller.signal.aborted)
                setError(error);
        }
    }, [model, isMounted]);


    return {
        environment,
        appData,
        error,
        afwComponents,
        objectTypeObjects,
        services,
        application,
        refresh,
    };
};

export const useClasses = stylesElement => {
    const theme = useTheme();
    return useMemo(() => {
        const rawClasses = typeof stylesElement === "function" ? stylesElement(theme) : stylesElement;
        const prepared = {};

        Object.entries(rawClasses).forEach(([key, value = {}]) => {
            prepared[key] = css(value);
        });

        return prepared;
    }, [stylesElement, theme]);
};

export {useTheme};
