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
    adapterId?:     string;
    objectTypeId?:  string;
    objectId?:      string;
    path?:          string;
    object?:        AfwObject|object;    
    objectOptions?: IObjectOptions;
    modelOptions?:  IModelOptions;
}

interface IRetrieveObjectsParams {
    uri?:           string;
    adapterId?:     string;
    objectTypeId?:  string;
    objectId?:      string;
    queryCriteria?: string;
    objectOptions?: IObjectOptions;
    modelOptions?:  IModelOptions;
}

interface IConfig {
    client:     AfwClient;
    adapterId:  string;
    debounce?:  boolean;
}

interface IfindDependencies {
    objectTypeObject?:  IObjectTypeObject;
    object?:            IAnyObject;
    adapterId?:         string;
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
    private adapterId:      string;    
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
        this.adapterId      = config.adapterId;        
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
     * @param {string} adapterId 
     * @param {string} objectTypeId 
     * @param {string} path
     */
    async loadObjectTypeDependency({ adapterId, objectTypeId, path }: IGetObjectParams) : Promise<IAnyObject> {
        const uri: string = path ? path : ("/" + adapterId + "/_AdaptiveObjectType_/" + objectTypeId);
       
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
     * @param {string}   adapterId 
     * @param {string[]} dependencies
     */
    async findObjectTypeDependencies({ 
        objectTypeObject, 
        adapterId, 
        dependencies = [] 
    } : IfindDependencies) {

        /* check the objectTypeObject for any other deps on embedded objects */
        if (objectTypeObject?.propertyTypes) {
            for (const [, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
                const {dataType, dataTypeParameter} = propertyType;

                if (dataType === "object" && dataTypeParameter) {
                    const objectTypePath = "/" + adapterId + "/_AdaptiveObjectType_/" + dataTypeParameter;    
                    if (!dependencies.includes(objectTypePath)) {
                        const embeddedObjectType: IObjectTypeObject = await this.loadObjectTypeDependency({ path: objectTypePath });                                                
                        dependencies.push(objectTypePath);       
                        
                        /* The embedded objectType may also have dependencies */
                        dependencies = await this.findObjectTypeDependencies({ objectTypeObject: embeddedObjectType, adapterId, dependencies });
                    }             
                }

                else if (dataType === "array" && dataTypeParameter && (dataTypeParameter.indexOf("object ") >= 0)) {
                    const parts = dataTypeParameter.split(" ");
                    const objectTypePath = "/" + adapterId + "/_AdaptiveObjectType_/" + parts[parts.length - 1];                    
                    if (!dependencies.includes(objectTypePath))                
                        dependencies.push(objectTypePath);
                }

                else if (dataType === "script" && dataTypeParameter && (dataTypeParameter.indexOf("object ") >= 0)) {
                    const parts = dataTypeParameter.split(" ");
                    const objectTypePath = "/" + adapterId + "/_AdaptiveObjectType_/" + parts[parts.length - 1];                                                                 
                    if (!dependencies.includes(objectTypePath))
                        dependencies.push(objectTypePath);                    
                }
            }
        }

        if (objectTypeObject?.otherProperties) {
            const {dataType, dataTypeParameter} = objectTypeObject["otherProperties"];

            if (dataType === "object" && dataTypeParameter) {
                const objectTypePath = "/" + adapterId + "/_AdaptiveObjectType_/" + dataTypeParameter;
                if (!dependencies.includes(objectTypePath))                
                    dependencies.push(objectTypePath);
            }

            else if (dataType === "array" && dataTypeParameter && (dataTypeParameter.indexOf("object ") >= 0)) {
                const parts = dataTypeParameter.split(" ");
                const objectTypePath = "/" + adapterId + "/_AdaptiveObjectType_/" + parts[parts.length - 1];                
                if (!dependencies.includes(objectTypePath))                
                    dependencies.push(objectTypePath);
            }

            else if (dataType === "script" && dataTypeParameter && (dataTypeParameter.indexOf("object ") >= 0)) {
                const parts = dataTypeParameter.split(" ");
                const objectTypePath = "/" + adapterId + "/_AdaptiveObjectType_/" + parts[parts.length - 1];                                                
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
     * @param {string}   adapterId
     * @param {object}   object
     * @param {string[]} dependencies
     */
    findObjectDependencies({ 
        adapterId, 
        object, 
        dependencies = [] 
    } : IfindDependencies) {
        
        if (object && isObject(object)) {
            Object.entries(object).forEach(([, value]: [string, IAnyObject]) => {

                if (value && value._meta_ && value._meta_.objectType) {
                    const path = "/" + adapterId + "/_AdaptiveObjectType_/" + value._meta_.objectType;
                    if (!dependencies.includes(path))
                        dependencies.push(path);
                }                    

                /* recursively look for dependencies */
                this.findObjectDependencies({ adapterId, object: value, dependencies });                
            });
        }

        return dependencies;
    }

    /**
     * 
     * @param adapterId
     * @param objectTypeId
     * @param object
     */
    async loadDependencies({ adapterId, objectTypeId, object }: IGetObjectParams ) {
        let dependencies = [
            "/" + adapterId + "/_AdaptiveObjectType_/_AdaptiveObject_",
            "/" + adapterId + "/_AdaptiveObjectType_/_AdaptiveMeta_"
        ];

        /* We need to synchronously load the ObjectType definition first, in order to know what else to load */
        const objectTypeObject = await this.loadObjectTypeDependency({ adapterId, objectTypeId });

        /* look for objectType related dependencies */
        dependencies = await this.findObjectTypeDependencies({ adapterId, objectTypeId, objectTypeObject, dependencies });

        /* find all object dependencies */
        dependencies = this.findObjectDependencies({ adapterId, object, dependencies });     

        /* resolve all promises at once */ 
        /*! \fixme these could be pooled into a single transaction with multiple actions */       
        await Promise.all( dependencies.map(path => this.loadObjectTypeDependency({ path })) );  
    }

    /**
     * 
     * @param {string} adapterId
     * @param {string} objectTypeId
     */
    getObjectTypeObject({ 
        adapterId = this.adapterId, 
        objectTypeId 
    } : IGetObjectParams) {
        const uri = "/" + adapterId + "/_AdaptiveObjectType_/" + objectTypeId;

        /* look in cache first */
        const objectTypeObject = this.getCachedObject(uri);
        if (objectTypeObject)
            return objectTypeObject;
    }

    /**
     * Fetches and caches objectTypes from the given adapterId.
     * 
     * @param {string} adapterId
     */
    async loadObjectTypes({ 
        adapterId = this.adapterId, 
        objectOptions = {} 
    } : IGetObjectParams) {
        if (adapterId) {
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
                adapterId, objectType: objectTypeId,
                options: <IJSONObject>options,             
            };      
            
            const objectTypeObjects = await this.client.perform(action).result();            
            
            objectTypeObjects.forEach( (object: IAnyObject) => this.cacheObject({ object }) );
            
            return objectTypeObjects.map( (object: IAnyObject) => new AfwObject({ 
                model: this, object, objectTypeId, adapterId
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
     * Gets an adaptive object using an adapterId, objectTypeId and objectId.
     * 
     * @param {string} objectTypeId
     * @param {string} objectId
     * @param {string} adapterId
     * @param {object} objectOptions
     * @param {object} modelOptions 
     */
    getObject({ 
        objectTypeId, 
        objectId, 
        adapterId, 
        objectOptions = {}, 
        modelOptions = { adaptiveObject: true } 
    } : IGetObjectParams) : IGetObjectResponse
    {

        if (!adapterId)
            throw new Error("This operation requires an adapterId.");

        if (!objectTypeId)
            throw new Error("This operation requires an objectTypeId.");

        if (!objectId)
            throw new Error("This operation requires an objectId.");

        const uri = "/" + encodeURIComponent(adapterId) + "/" + encodeURIComponent(objectTypeId) + "/" + encodeURIComponent(objectId);

        return this.getObjectWithUri({ uri, objectOptions, modelOptions });
    }

    /**
     * Creates a new AfwObject instance.     
     */
    newObject({ 
        adapterId, 
        objectTypeId, 
        objectId,
        object 
    } : IGetObjectParams) 
    {
        return new AfwObject({ 
            model: this, adapterId, objectTypeId, objectId, object
        });
    }

    /**
     * Retrieves adaptive objects using an adapterId and objectTypeId.
     * 
     * @param {string} objectTypeId
     * @param {string} adapterId
     * @param {string} queryCriteria
     * @param {object} objectOptions  
     * @param {object} modelOptions
     */
    retrieveObjects({ 
        objectTypeId, 
        adapterId, 
        queryCriteria, 
        objectOptions = {}, 
        modelOptions = { adaptiveObject: true, initialize: true }
    } : IRetrieveObjectsParams) : IRetrieveObjectsResponse
    {

        if (!adapterId)
            throw new Error("This operation requires an adapterId.");

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
            adapterId, objectType: objectTypeId, queryCriteria,
            options: <IJSONObject>options,             
        };      
        
        const {result, ...rest} = this.client.perform(action);
        let objects : Promise<AfwObject[]|IAnyObject[]>;
        objects = new Promise((resolve, reject) => {
            result().then((res : IAnyObject[]) => {
                if (modelOptions.adaptiveObject) {                                        
                    let resolvedObjects = res.map(object => new AfwObject({ 
                        model: this, object, objectTypeId, adapterId
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