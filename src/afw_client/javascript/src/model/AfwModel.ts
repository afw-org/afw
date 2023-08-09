// See the 'COPYING' file in the project root for licensing information.

import {AfwClient} from "../AfwClient";

import {AfwObject} from ".";

import {
    IJSONObject,
    IAnyObject,
    IObjectTypeObject,
    IModelOptions,
    IObjectOptions,
    AfwOrObject  
} from "./types";

interface IGetObjectParams {
    uri?:           string;
    adaptorId?:     string;
    objectTypeId?:  string;
    objectId?:      string;
    path?:          string;
    object?:        AfwObject|object;    
    objectOptions?: IObjectOptions;
    modelOptions?:  IModelOptions;
}

interface IRetrieveObjectsParams {
    uri?:           string;
    adaptorId?:     string;
    objectTypeId?:  string;
    objectId?:      string;
    queryCriteria?: string;
    objectOptions?: IObjectOptions;
    modelOptions?:  IModelOptions;
}

interface IConfig {
    client:     AfwClient;
    adaptorId:  string;
    debounce?:  boolean;
}

interface IfindDependencies {
    objectTypeObject?:  IObjectTypeObject;
    object?:            IAnyObject;
    adaptorId?:         string;
    objectTypeId?:      string;
    dependencies?:      any[];
}

interface IGetObjectResponse {
    object:             Promise<IAnyObject|AfwObject>;
    result:             IAnyObject;
    controller:         AbortController;
}

interface IRetrieveObjectsResponse {
    objects:            Promise<AfwObject[]|IAnyObject[]>;
    result:             IAnyObject;
    controller:         AbortController;
}

type CachedValue = AfwOrObject | AfwOrObject[];
type CacheCallback = (key?: string, newValue?: CachedValue) => void;

import {isObject} from "../utils";


/**
 * AfwModel is a class that abstracts the fetching and updating of 
 * Adaptive Objects, while providing some automatic features, such as
 * auto-fetching Object Type dependencies and using caching strategies.
 * 
 * When asynchronous requests are made to resolve object dependencies,
 * the promises are immediately cached, prior to caching the resolved
 * results.  This is done so that subsequent requests for the same out-
 * standing async request don't attempt to fetch the same thing.  Instead
 * they await on the promise instead of making a new one.
 */
export class AfwModel {

    private client:         AfwClient;
    private adaptorId:      string;    
    private debounce?:      boolean;
    private cachedPromises: {
        [prop: string]:     IGetObjectResponse;
    };
    private cachedObjects: {
        [prop: string]:     AfwOrObject;
    };
    private cache: {
        [key: string]:      CachedValue;
    };
    private cacheCallbacks: {
        [key: string]:      CacheCallback[]
    };

    /**
     * 
     * @param config configuration object
     */
    constructor(config: IConfig) {
        this.client         = config.client;
        this.adaptorId      = config.adaptorId;        
        this.debounce       = config.debounce;
        this.cachedPromises = {};
        this.cachedObjects  = {};        
        this.cache          = {};
        this.cacheCallbacks = {};        
    }
  
    /**
     * Returns the underlying client interface.
     */
    getClient() {
        return this.client;
    }

    /**
     * Returns the debounce setting.
     */
    getDebounce() {
        return this.debounce;
    }

    /**
     * Cache a promise, so future promises may not be required when in-flight.
     * 
     * @param object
     * @param path
     */
    cachePromise({ promise, path }: { promise: IGetObjectResponse; path: string }) {
        this.cachedPromises[ path ] = promise;
    }

    /**
     * Caches an object, for quick retrieval later.
     */
    cacheObject({ object, path }: { object: AfwOrObject; path?: string }) {

        let key = path;
        if (!key) {
            if (object instanceof AfwObject)
                key = object.getPath();        
            else
                key = object?._meta_?.path;
        }

        if (key) {            
            this.cachedObjects[ key ] = object;
        }        
    }

    /**
     * 
     * @param key 
     * @param value
     */
    cacheSet(key : string, value : CachedValue) {
        this.cache[key] = value;

        /* notify any function that needs to know it changed */
        if (this.cacheCallbacks[key]) {
            this.cacheCallbacks[key].forEach(cb => cb(key, value));
        }
    }

    /**
     * 
     * @param key 
     */
    cacheGet(key : string) : CachedValue | undefined {
        return this.cache[key];
    }

    /**
     * 
     * @param key 
     */
    cacheInvalidate(key? : string, value? : CachedValue) {
        if (key) {
            delete this.cache[key];
            if (value)
                this.cache[key] = value;

            if (this.cacheCallbacks[key])
                this.cacheCallbacks[key].forEach(cb => cb(key, value));
        } else {
            this.cache = {};
            /* notify callbacks that the cache has been cleared */
            Object.entries(this.cacheCallbacks).forEach(([,cbs]) => {
                cbs.forEach(cb => cb());
            });
        }
    }   

    /**
     * 
     * @param key 
     * @param cb 
     */
    cacheOnInvalidate(key : string, cb : CacheCallback) {
        if (this.cacheCallbacks[key] === undefined)
            this.cacheCallbacks[key] = [];

        this.cacheCallbacks[key].push(cb);        

        return cb;
    }

    /**
     * 
     * @param key 
     * @param cb 
     */
    cacheOffInvalidate(key: string, cb: CacheCallback) {
        if (this.cacheCallbacks[key]) {
            this.cacheCallbacks[key] = this.cacheCallbacks[key].filter(_cb => _cb !== cb);
        }
    }

    /**
     * Removes a promise from cache.
     * 
     * @param path <string> object path representing the promise.
     */
    uncachePromise(path: string) {
        delete this.cachedPromises[ path ];
    }

    /**
     * Removes an object from cache.
     * 
     * @param path <string> object path representing the promise.
     */
    uncacheObject(path: string) {
        delete this.cachedObjects[ path ];
    }

    /**
     * Get a cached promise with the provided path.
     * 
     * @param {string} path 
     * @returns promise for an object
     */
    getCachedPromise(path: string) {
        return this.cachedPromises[ path ];
    }

    /**
     * Gets an object that has been cached by path.
     * 
     * @param {string} path     
     */
    getCachedObject(path: string) {
        return this.cachedObjects[ path ];
    }

    /**
     * Clears any cache that may be stored internally.
     */
    clearCache() {
        this.cachedPromises = {};
        this.cachedObjects  = {};
    }

    /**          
     * This method constructs the Object Type URI from the adaptive object, fetches
     * its reference and caches the result for later.
     * 
     * @param {string} adaptorId 
     * @param {string} objectTypeId 
     * @param {string} path
     */
    async loadObjectTypeDependency({ adaptorId, objectTypeId, path }: IGetObjectParams) : Promise<IAnyObject> {
        const uri: string = path ? path : ("/" + adaptorId + "/_AdaptiveObjectType_/" + objectTypeId);
       
        /* check cache first */
        let objectTypeObject: AfwOrObject = this.getCachedObject(uri);
        if (objectTypeObject)
            return objectTypeObject;

        if (this.getCachedPromise(uri)) {
            ({ object: objectTypeObject } = this.getCachedPromise(uri));
            this.cacheObject({ object: objectTypeObject, path: uri });
            return objectTypeObject;
        }

        /* use the underlying data model to load our objectType */
        const response = this.getObjectWithUri({ 
            uri, 
            objectOptions: {                 
                composite: true,
                normalize: true
            },
            modelOptions: { adaptiveObject: false } 
        });

        /* cache the promise first, then move it to permanent cache */
        this.cachePromise({ promise: response, path: uri });

        objectTypeObject = await response.object;
        this.cacheObject({ object: objectTypeObject, path: uri });

        /* remove it from promise cache to avoid memory build up */
        this.uncachePromise(uri);
        
        return objectTypeObject;
    }

    /**
     * Takes an objectType object and examines its propertyTypes to discover objectType dependencies
     * that it may need to load for embedded objects.
     * 
     * @param {object}   objectTypeObject 
     * @param {string}   adaptorId 
     * @param {string[]} dependencies
     */
    async findObjectTypeDependencies({ 
        objectTypeObject, 
        adaptorId, 
        dependencies = [] 
    } : IfindDependencies) {

        /* check the objectTypeObject for any other deps on embedded objects */
        if (objectTypeObject?.propertyTypes) {
            for (const [, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
                const {dataType, dataTypeParameter} = propertyType;

                if (dataType === "object" && dataTypeParameter) {
                    const objectTypePath = "/" + adaptorId + "/_AdaptiveObjectType_/" + dataTypeParameter;    
                    if (!dependencies.includes(objectTypePath)) {
                        const embeddedObjectType: IObjectTypeObject = await this.loadObjectTypeDependency({ path: objectTypePath });                                                
                        dependencies.push(objectTypePath);       
                        
                        /* The embedded objectType may also have dependencies */
                        dependencies = await this.findObjectTypeDependencies({ objectTypeObject: embeddedObjectType, adaptorId, dependencies });
                    }             
                }

                else if (dataType === "array" && dataTypeParameter && (dataTypeParameter.indexOf("object ") >= 0)) {
                    const parts = dataTypeParameter.split(" ");
                    const objectTypePath = "/" + adaptorId + "/_AdaptiveObjectType_/" + parts[parts.length - 1];                    
                    if (!dependencies.includes(objectTypePath))                
                        dependencies.push(objectTypePath);
                }

                else if (dataType === "hybrid" && dataTypeParameter && (dataTypeParameter.indexOf("object ") >= 0)) {
                    const parts = dataTypeParameter.split(" ");
                    const objectTypePath = "/" + adaptorId + "/_AdaptiveObjectType_/" + parts[parts.length - 1];                                                                 
                    if (!dependencies.includes(objectTypePath))
                        dependencies.push(objectTypePath);                    
                }
            }
        }

        if (objectTypeObject?.otherProperties) {
            const {dataType, dataTypeParameter} = objectTypeObject["otherProperties"];

            if (dataType === "object" && dataTypeParameter) {
                const objectTypePath = "/" + adaptorId + "/_AdaptiveObjectType_/" + dataTypeParameter;
                if (!dependencies.includes(objectTypePath))                
                    dependencies.push(objectTypePath);
            }

            else if (dataType === "array" && dataTypeParameter && (dataTypeParameter.indexOf("object ") >= 0)) {
                const parts = dataTypeParameter.split(" ");
                const objectTypePath = "/" + adaptorId + "/_AdaptiveObjectType_/" + parts[parts.length - 1];                
                if (!dependencies.includes(objectTypePath))                
                    dependencies.push(objectTypePath);
            }

            else if (dataType === "hybrid" && dataTypeParameter && (dataTypeParameter.indexOf("object ") >= 0)) {
                const parts = dataTypeParameter.split(" ");
                const objectTypePath = "/" + adaptorId + "/_AdaptiveObjectType_/" + parts[parts.length - 1];                                                
                if (!dependencies.includes(objectTypePath))
                    dependencies.push(objectTypePath);
                
            }
        }

        return dependencies;
    }

    /**
     * Takes an object and searches the object, recursively, for any instance meta objectType 
     * dependencies that may not have been available in the compiled objectType dependencies.
     * 
     * @param {string}   adaptorId
     * @param {object}   object
     * @param {string[]} dependencies
     */
    findObjectDependencies({ 
        adaptorId, 
        object, 
        dependencies = [] 
    } : IfindDependencies) {
        
        if (object && isObject(object)) {
            Object.entries(object).forEach(([, value]: [string, IAnyObject]) => {

                if (value && value._meta_ && value._meta_.objectType) {
                    const path = "/" + adaptorId + "/_AdaptiveObjectType_/" + value._meta_.objectType;
                    if (!dependencies.includes(path))
                        dependencies.push(path);
                }                    

                /* recursively look for dependencies */
                this.findObjectDependencies({ adaptorId, object: value, dependencies });                
            });
        }

        return dependencies;
    }

    /**
     * 
     * @param adaptorId
     * @param objectTypeId
     * @param object
     */
    async loadDependencies({ adaptorId, objectTypeId, object }: IGetObjectParams ) {
        let dependencies = [
            "/" + adaptorId + "/_AdaptiveObjectType_/_AdaptiveObject_",
            "/" + adaptorId + "/_AdaptiveObjectType_/_AdaptiveMeta_"
        ];

        /* We need to synchronously load the ObjectType definition first, in order to know what else to load */
        const objectTypeObject = await this.loadObjectTypeDependency({ adaptorId, objectTypeId });

        /* look for objectType related dependencies */
        dependencies = await this.findObjectTypeDependencies({ adaptorId, objectTypeId, objectTypeObject, dependencies });

        /* find all object dependencies */
        dependencies = this.findObjectDependencies({ adaptorId, object, dependencies });     

        /* resolve all promises at once */ 
        /*! \fixme these could be pooled into a single transaction with multiple actions */       
        await Promise.all( dependencies.map(path => this.loadObjectTypeDependency({ path })) );  
    }

    /**
     * 
     * @param {string} adaptorId
     * @param {string} objectTypeId
     */
    getObjectTypeObject({ 
        adaptorId = this.adaptorId, 
        objectTypeId 
    } : IGetObjectParams) {
        const uri = "/" + adaptorId + "/_AdaptiveObjectType_/" + objectTypeId;

        /* look in cache first */
        const objectTypeObject = this.getCachedObject(uri);
        if (objectTypeObject)
            return objectTypeObject;
    }

    /**
     * Fetches and caches objectTypes from the given adaptorId.
     * 
     * @param {string} adaptorId
     */
    async loadObjectTypes({ 
        adaptorId = this.adaptorId, 
        objectOptions = {} 
    } : IGetObjectParams) {
        if (adaptorId) {
            const objectTypeId = "_AdaptiveObjectType_";

            let options = { ...objectOptions };
            options.objectId = true;
            options.path = true;
            options.composite = true;
            options.objectType = true;
            options.normalize = true;

            /*! 
                \fixme temporary until bindings fixed up 
                \fixme return everything, including abort controller
            */
            const action: IJSONObject = { 
                "function": "retrieve_objects", 
                adaptorId, objectType: objectTypeId,
                options: <IJSONObject>options,             
            };      
            
            const objectTypeObjects = await this.client.perform(action).result();            
            
            objectTypeObjects.forEach( (object: IAnyObject) => this.cacheObject({ object }) );
            
            return objectTypeObjects.map( (object: IAnyObject) => new AfwObject({ 
                model: this, object, objectTypeId, adaptorId
            }));
        }
    }

    /**
     * Gets an adaptive object using a path uri.
     * 
     * @param {string} uri
     * @param {object} objectOptions
     * @param {object} modelOptions
     */
    getObjectWithUri({ 
        uri, 
        objectOptions = {}, 
        modelOptions = { adaptiveObject: true } 
    } : IGetObjectParams) : IGetObjectResponse
    {
        /* default objectOptions, if using the model wrapper */
        let options: IObjectOptions = { ...objectOptions };
        if (modelOptions.adaptiveObject) {            
            options.objectId = true;
            options.path = true;
            options.objectType = true;
            options.normalize = true;
            options.composite = true;
        }

        /*! \fixme temporary until bindings fixed up */
        const action: IJSONObject = { 
            "function": "get_object_with_uri", 
            options: <IJSONObject>options,
            uri 
        };      
        
        const {result, ...rest} = this.client.perform(action);
        let object : Promise<IAnyObject|AfwObject>;
        object = new Promise((resolve, reject) => {
            result().then(res => {
                if (modelOptions.adaptiveObject) {                    
                    new AfwObject({ 
                        model: this, object: res, path: uri,
                    }).initialize().then(o => resolve(o));
                } else resolve(res);
            }).catch(e => reject(e));
        });        

        return { result, ...rest, object };
    }

    /**
     * Gets an adaptive object using an adaptorId, objectTypeId and objectId.
     * 
     * @param {string} objectTypeId
     * @param {string} objectId
     * @param {string} adaptorId
     * @param {object} objectOptions
     * @param {object} modelOptions 
     */
    getObject({ 
        objectTypeId, 
        objectId, 
        adaptorId, 
        objectOptions = {}, 
        modelOptions = { adaptiveObject: true } 
    } : IGetObjectParams) : IGetObjectResponse
    {

        if (!adaptorId)
            throw new Error("This operation requires an adaptorId.");

        if (!objectTypeId)
            throw new Error("This operation requires an objectTypeId.");

        if (!objectId)
            throw new Error("This operation requires an objectId.");

        const uri = "/" + encodeURIComponent(adaptorId) + "/" + encodeURIComponent(objectTypeId) + "/" + encodeURIComponent(objectId);

        return this.getObjectWithUri({ uri, objectOptions, modelOptions });
    }

    /**
     * Creates a new AfwObject instance.     
     */
    newObject({ 
        adaptorId, 
        objectTypeId, 
        objectId,
        object 
    } : IGetObjectParams) 
    {
        return new AfwObject({ 
            model: this, adaptorId, objectTypeId, objectId, object
        });
    }

    /**
     * Retrieves adaptive objects using an adaptorId and objectTypeId.
     * 
     * @param {string} objectTypeId
     * @param {string} adaptorId
     * @param {string} queryCriteria
     * @param {object} objectOptions  
     * @param {object} modelOptions
     */
    retrieveObjects({ 
        objectTypeId, 
        adaptorId, 
        queryCriteria, 
        objectOptions = {}, 
        modelOptions = { adaptiveObject: true, initialize: true }
    } : IRetrieveObjectsParams) : IRetrieveObjectsResponse
    {

        if (!adaptorId)
            throw new Error("This operation requires an adaptorId.");

        if (!objectTypeId)
            throw new Error("This operation requires an objectTypeId.");        
            
        let options: IObjectOptions = { ...objectOptions };
        options.objectId = true;
        options.path = true;
        options.objectType = true;
        options.normalize = true;

        /*! 
            \fixme temporary until bindings fixed up 
            \fixme return everything, including abort controller
        */
        const action: IJSONObject = { 
            "function": "retrieve_objects", 
            adaptorId, objectType: objectTypeId, queryCriteria,
            options: <IJSONObject>options,             
        };      
        
        const {result, ...rest} = this.client.perform(action);
        let objects : Promise<AfwObject[]|IAnyObject[]>;
        objects = new Promise((resolve, reject) => {
            result().then((res : IAnyObject[]) => {
                if (modelOptions.adaptiveObject) {                                        
                    let resolvedObjects = res.map(object => new AfwObject({ 
                        model: this, object, objectTypeId, adaptorId
                    }));
                    if (modelOptions.initialize) {
                        Promise.all(resolvedObjects.map(obj => obj.initialize())).then(
                            (initializedObjects : AfwObject[]) => resolve(initializedObjects)
                        );
                    } else resolve(resolvedObjects);
                } else resolve(res);
            }).catch(e => reject(e));
        });

        return { result, ...rest, objects };
    }
}

export default AfwModel;