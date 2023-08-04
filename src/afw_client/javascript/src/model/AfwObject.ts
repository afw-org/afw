// See the 'COPYING' file in the project root for licensing information.
import {
    AfwList,
    AfwProperty,
    AfwEvent,
    AfwModel,
    AfwEventId,
} from ".";

import {isObject, isArray} from "../utils";

import {        
    afwReconcileObject,
    afwReplaceObjectWithUri,    
    afwDeleteObjectWithUri,
    afwAddObjectWithUri,
    afwAddObject,
    afwModifyObjectWithUri
} from "@afw/core";

import {
    IAnyObject,
    IJSONObject,
    IJSONSchema,
    IPropertyType,
    IPropertyTypesObject,
    IObjectOptions
} from "./types";

interface Config {
    model?:             AfwModel | undefined;
    debounce?:          boolean;
    adaptorId?:         string;    
    objectTypeId?:      string;
    objectId?:          string;
    path?:              string;
    object?:            IAnyObject;
    parent?:            any;
    propertyName?:      string;
    objectTypeObject?:  IAnyObject;
}


/* afw dataType to JSON Schema type simple mapping */
const afwDataTypeToJSONSchemaType : {
    [prop: string]:         string;
} = {
    "anyURI":               "string",
    "base64Binary":         "string",    
    "boolean":              "boolean",
    "date":                 "string",          
    "dateTime":             "string",
    "dayTimeDuration":      "string",
    "dnsName":              "string",
    "double":               "number",
    "expression":           "string",
    "function":             "string",
    "hexBinary":            "string",
    "hybrid":               "string",
    "ia5String":            "string",
    "integer":              "number",
    "ipAddress":            "string",
    "list":                 "array",
    "null":                 "null",
    "object":               "object",
    "objectId":             "objectId",
    "objectPath":           "objectPath",
    "password":             "string",
    "rfc822Name":           "string",
    "string":               "string",
    "template":             "string",
    "time":                 "string",
    "x500Name":             "string",
    "xpathExpression":      "string",
    "yearMonthDuration":    "string",    
};


/**
 * AfwObject
 * 
 * This class is used to represent an Adaptive Object via Javascript
 * methods for accessing and manipulating the object and its metadata.
 * 
 * This class is useful for a MVC type of applications, but also has the 
 * eventing support to provide Flux-based architecture, provided you use
 * the accompanying hooks or configure event callbacks to update your
 * components.
 * 
 * @example
 * const obj = new AfwObject(config);
 * 
 */
export class AfwObject extends AfwEvent {

    private model?:             AfwModel;    
    private adaptorId?:         string;
    private objectTypeId?:      string;
    private objectId?:          string;
    private path?:              string;
    private object?:            IAnyObject;
    private propertyName?:      string;
    private objectTypeObject?:  IAnyObject;
    private properties:         {
        [prop: string]:         AfwProperty
    } = {};

    private initialized:        boolean = false;
    private initialValue:       string = "";
    private dirty:              boolean = false;
    debounce?:                  boolean;

    constructor(config: Config) {        
        super(config);

        this.model              = config.model;
        this.adaptorId          = config.adaptorId;
        this.objectTypeId       = config.objectTypeId;
        this.objectId           = config.objectId;
        this.path               = config.path;
        this.object             = config.object;
        this.propertyName       = config.propertyName;
        this.objectTypeObject   = config.objectTypeObject;


        /*
         * Derive some path components if it's presented.
         */
        if (config.path) {
            this.adaptorId = config.path.split("/")[1];
            this.objectTypeId = config.path.split("/")[2];
            this.objectId = config.path.split("/")[3];
        }

        else if (config.object?._meta_?.path) {
            const path = config.object._meta_.path;

            this.path = path;
            this.adaptorId = path.split("/")[1];
            this.objectTypeId = path.split("/")[2];
            this.objectId = path.split("/")[3];
        }

        if (config.object?._meta_?.objectType)
            this.objectTypeId = config.object._meta_.objectType;

        if (config.object?._meta_?.objectId)
            this.objectId = config.object._meta_.objectId;

        if (config.debounce === undefined && this.model) 
            this.debounce = this.model.getDebounce();
        else 
            this.debounce = config.debounce;
        
    }

    /**
     * Initializes an AfwObject by loading any dependencies that it may need via the underlying
     * data model.
     */
    async initialize(model? : (AfwModel | undefined)) {

        const {adaptorId, objectTypeId, object} = this;

        if (!model)
            model = this.model;
        else 
            this.model = model;

        if (!model)
            throw new Error("Unable to initialize AfwObject without an underlying data model.");

        /* Without an adaptorId and objectTypeId, we can only assume it's an _AdaptiveObject_ */
        if (adaptorId && objectTypeId) {
            await model.loadDependencies({ adaptorId, objectTypeId, object });
        } else {
            /* we don't know the adaptorId/objectTypeId, so assume it's an _AdaptiveObject_ */
            await model.loadObjectTypeDependency({ path: "/afw/_AdaptiveObjectType_/_AdaptiveObject_" });
        }

        this.initialized = true;

        return this;
    }

    /**
     * Creates an internal "snapshot" for discard, diff's, etc.
     */
    snapshot() {        
        if (!this.initialValue && this.object) {            
            this.initialValue = JSON.stringify(this.object);
            
            /* if this is an embedded object, we need to snapshot our parent to prepare for a change */
            this.getEmbeddingObject()?.snapshot();
        } 
    }

    /**
     * Augments the default dispatchEvent to create the initialValue, in case a child property
     * has changed before we had a chance to initialize it.
     * 
     * @param event The event to be dispatched.
     */
    dispatchEvent(event : any) {
        if (event.eventId === "onChildChanged") {
            this.snapshot();
        }

        super.dispatchEvent(event);
    }

    /**
     * Clones and returns this AfwObject.
     */
    clone() {
        const { 
            objectTypeId, objectId, path, 
            parent, propertyName, objectTypeObject,
            initialized
        } = this;
        const model = this.getModel();
        const adaptorId = this.getAdaptorId();
       
        let obj = new AfwObject({
            model, adaptorId, objectTypeId, objectId,
            object: this.toJSON(),
            path, parent, propertyName, objectTypeObject,
        });

        obj.initialized = initialized;

        /* make sure to copy over meta */
        if (this.object?._meta_ && obj.object) {
            obj.object._meta_ = { ...this.object?._meta_ };
        }

        return obj;
    }

    /**
     * Discards any changes made to this object since the original value.
     */
    discardChanges() {        
        /* re-parse the original object */
        if (this.initialValue) {
            this.fromJSON(this.initialValue);            

            this.dispatchEvent({ eventId: AfwEventId.onDiscardChanges, source: this, value: this.object });
            this.dispatchEvent({ eventId: AfwEventId.onSavable, source: this, value: false });
        }
    }
    
    /**
     * Returns the object that embeds this object, if it exists.
     */
    getEmbeddingObject() : AfwObject | undefined {        
        if (this.parent instanceof AfwObject)
            return this.parent;

        else if (this.parent)
            return this.parent.getEmbeddingObject();
    }

    /**
     * Returns the original value of this object, before changes applied.
     */
    getOriginal() {
        if (this.initialValue)
            return JSON.parse(this.initialValue);
        else 
            return this.object;
    }

    /**
     * Returns the underlying AfwModel instance.
     */
    getModel() : AfwModel | undefined {
        if (this.model)
            return this.model;

        return this.getEmbeddingObject()?.getModel();
    }

    /**
     * Returns the propertyType object for this property name. 
     * 
     * @param {string} name The name of the property.
     * @param {boolean} useOtherProperties
     */
    getPropertyType(name: string, useOtherProperties: boolean = true) : IPropertyType | undefined {
        const {object} = this;
        const objectTypeObject = this.getObjectTypeObject();
        let propertyType : IPropertyType | undefined;

        /* start with the model, using its objectType Object definition */
        if (!name) {
            /* Use otherProperties */
            return objectTypeObject?.otherProperties;
        }

        /* first look inside propertyTypes/{name} for meta */
        if (objectTypeObject?.propertyTypes && objectTypeObject.propertyTypes[name])
            propertyType = objectTypeObject?.propertyTypes[name];

        /* if we don't see a propertyType/{name}, use otherProperties */
        else if (useOtherProperties && objectTypeObject?.otherProperties)
            propertyType = objectTypeObject.otherProperties;

        /* if we have "runtime" _meta_ in the object itself, merge its value into propertyType */
        if (object?._meta_?.propertyTypes && object?._meta_?.propertyTypes[name]) {               
            propertyType = { ...propertyType, ...object._meta_.propertyTypes[name] };
        }

        return propertyType;
    }

    /**
     * Returns an AfwProperty for with this "name".
     * 
     * @param name The name of the property.  It can be a single
     *             property name, or an array of property names that
     *             reference embedded property names.
     */
    getProperty(name: string | string[]) : AfwProperty | undefined {
        let {properties, object} = this;

        if (!name)
            return undefined;

        if (Array.isArray(name)) {
            let property = this.getProperty(name[0]);
            for (let i = 1; i < name.length; i++) {                                
                if (property && property.getValue() !== undefined) {
                    const value = property.getValue();
                    if (value instanceof AfwObject)
                        property = value.getProperty(name[i]);
                }
            }

            return property;
        }

        /* check to see if it's already built and cached yet, otherwise build it */
        if (properties[name] === undefined) {
            const value = object ? object[name] : undefined;

            /* determine the propertyType */
            let propertyType : (IPropertyType | undefined);            
            
            /* Get the propertyType.  We use otherProperties here, because a new property may have *just*
               been added and no value yet assigned.  If this is a problem later, then we have to adjust for
               both cases.  See CustomVariables for use case. 

               Note:  There may be a use-case for passing false to useOtherProperties.
             */
            propertyType = this.getPropertyType(name);
            if (!propertyType) {               
                return undefined;
            }

            properties[name] = new AfwProperty({ 
                propertyName: name, parent: this, value, propertyType 
            });
        }

        return properties[name];
    }

    /**
     * Returns an array of AfwProperty instances.
     */
    getProperties() {
        const {object, properties} = this;
        const objectTypeObject = this.getObjectTypeObject();

        let props: AfwProperty[] = [], keys: string[] = [];

        /* use keys from the ObjectType object definition */
        if (objectTypeObject && objectTypeObject.propertyTypes)
            keys = Object.keys(objectTypeObject.propertyTypes);

        /* check the object instance for additional properties */
        if (object) 
            keys = keys.concat(Object.keys(object).filter(name => !keys.includes(name)));

        /* if there are cached properties, not reflected by the internal object  */
        keys = keys.concat(Object.keys(properties).filter(name => !keys.includes(name)));

        keys.filter(key => key !== "_meta_").forEach(name => {
            const value = object ? object[name] : undefined;
            if (!Object.prototype.hasOwnProperty.call(properties,name)) {
                const propertyType = this.getPropertyType(name);                
                properties[name] = new AfwProperty({ 
                    propertyName: name, 
                    parent: this, value,
                    propertyType
                });
            }

            props.push(properties[name]);
        });

        return props;
    }

    /**
     * Returns the value of the AfwProperty by this name.
     * 
     * @param {string|string[]}  name The property name to retrieve.
     */
    getPropertyValue(name : string | string[]) : any {

        if (Array.isArray(name)) {            
            let value = this.getPropertyValue(name[0]);                  
            for (let i = 1; i < name.length; i++) {
                if (!(value instanceof AfwObject))
                    return undefined;

                value = value.getPropertyValue(name[i]);  
            }

            return value;
        }

        const property = this.getProperty(name);        
        if (property)
            return property.getValue();
    }

    /**
     * Returns an object containing property names as keys and AfwValue as 
     * values, which are useful for object destructuring.
     * 
     * This routine needs to look at the underlying object, as well as 
     * as any internal properties, in case some have changed or are not 
     * in the underlying objectType.
     */
    getPropertyValues() : { [prop: string] : any; }
    {  
        const {object} = this;

        let obj : { [prop: string] : any } = {};        

        object && Object.keys(object).forEach(key => {
            if (key === "_meta_")
                return;
            obj[key] = this.getPropertyValue(key);
        });

        Object.entries(this.properties).forEach(([propertyName, prop]) =>
            obj[ propertyName ] = prop.getValue()
        );                
        
        return obj;
    }

    /**
     * Returns the objectType object for this object.
     */
    getObjectTypeObject() : IAnyObject | undefined {
        const {objectTypeId} = this;
        const model = this.getModel();
        const adaptorId = this.getAdaptorId();
        const embeddingObject = this.getEmbeddingObject();

        if (this.objectTypeObject)
            return this.objectTypeObject;

        if (!embeddingObject && !this.initialized) {            
            throw new Error("Object /" + adaptorId + "/" + objectTypeId + "/" + this.objectId +  " is not initialized yet.");
        }

        if (adaptorId && objectTypeId) {            
            this.objectTypeObject = model?.getObjectTypeObject({ adaptorId, objectTypeId });  
        } else {
            /* without an adaptorId, or objectTypeId, then we don't have an objectType to go by */
            this.objectTypeObject = model?.getObjectTypeObject({ adaptorId: "afw", objectTypeId: "_AdaptiveObject_" });
        }

        return this.objectTypeObject;        
    }

    /**
     * Returns the adaptorId for this object.  If it's embedded, it requests from the parent.
     */
    getAdaptorId() : string | undefined {
        if (this.adaptorId)
            return this.adaptorId;

        /* if this is an embedded object, the parent will know the adaptorId */        
        return this.getEmbeddingObject()?.getAdaptorId();
    }

    /**
     * Gets the metadata propertyType object by name.
     * 
     * @param {string} name The meta property name whose value we are interested in.
     * 
     */
    getMetaProperty(name: string) {

        /* first check the instance object for meta */
        if (this.object?._meta_ && (this.object?._meta_[name] !== undefined))
            return this.object?._meta_[name];

        /* next examine the objectType object */
        const objectTypeObject = this.getObjectTypeObject();
        if (objectTypeObject) {
            return objectTypeObject[name];
        }
    }

    /**
     * Sets the metadata property.
     * 
     * @param {string} name
     * @param {string} value
     */
    setMetaProperty(name : string, value : any) {
        this.snapshot();
        
        if (!this.object)
            this.object = {};
        
        if (!this.object._meta_)
            this.object._meta_ = {};

        this.object._meta_[name] = value;
    }

    /**
     * Returns the "allowAdd" metadata property
     */
    getAllowAdd() {
        return this.getMetaProperty("allowAdd");
    }

    /**
     * Returns the "allowDelete" metadata property
     */
    getAllowDelete() {
        return this.getMetaProperty("allowDelete");
    }

    /**
     * Returns the "allowEntity" metadata property
     */
    getAllowEntity() {
        return this.getMetaProperty("allowEntity");
    }

    /**
     * Returns the "allowChange" metadata property
     */
    getAllowChange() {
        return this.getMetaProperty("allowChange");
    }

    /**
     * Returns the "description" metadata property
     */
    getDescription() {
        return this.getMetaProperty("description");
    }

    /**
     * Returns the "descriptionPropertyName" metadata property
     */
    getDescriptionPropertyName() {
        return this.getMetaProperty("descriptionPropertyName");
    }

    /**
     * Returns the "inheritedFrom" metadata property
     */
    getInheritedFrom() {
        return this.getMetaProperty("inheritedFrom");
    }    

    /**
     * Returns the "label" metadata property
     */
    getLabel() {
        return this.getMetaProperty("label");
    }

    /**
     * Returns the instance meta for this object as a JS object.
     */
    getMeta() {
        const {object, objectTypeObject} = this;

        const meta = Object.assign({}, objectTypeObject, object?._meta_);

        return meta;
    }

    /**
     * Returns the meta in the form of another AfwObject instance.
     */
    getMetaObject() {
        const model = this.getModel();
        const adaptorId = this.getAdaptorId();

        const obj = new AfwObject({
            model, 
            adaptorId, 
            objectTypeId: "_AdaptiveMeta_",
            object: this.getMeta(),
        });
        obj.initialized = true;

        return obj;
    }

    /**
     * Returns the objectId for this AfwObject.
     */
    getObjectId() {              
        const objectIdPropertyName = this.getObjectIdPropertyName();  
        if (objectIdPropertyName && this.getPropertyValue(objectIdPropertyName)) {
            return this.getPropertyValue( objectIdPropertyName );
        } else {            
            if (this.objectId)
                return this.objectId;
            else
                return this.getMetaProperty("objectId");
        }
    }

    /**
     * getObjectIdPropertyName()
     */
    getObjectIdPropertyName() {
        return this.getMetaProperty("objectIdPropertyName");
    }

    /**
     * Returns the objectTypeId for this object.
     */
    getObjectTypeId() {
        if (this.objectTypeId)
            return this.objectTypeId;

        else if (this.getPath())
            return this.getPath()?.split("/")[2];
        
        return this.getMetaProperty("objectTypeId");
    }

    /**
     * Returns the path for this object.
     */
    getPath() : string | undefined {
        const {adaptorId, objectTypeId, objectId} = this;

        if (this.getEmbeddingObject()) {
            if (this.getEmbeddingObject()?.getPath())
                return this.getEmbeddingObject()?.getPath() + "/" + this.propertyName;
            else
                return this.propertyName;
        }

        else if (this.path)
            return this.path;

        else if (adaptorId && objectTypeId && objectId)
            return "/" + adaptorId + "/" + objectTypeId + "/" + objectId;

        else
            return this.getMetaProperty("path");
    }

    /**
     * Returns the "resolvedParentPaths" metadata property
     */
    getResolvedParentPaths() {
        return this.getMetaProperty("resolvedParentPaths");
    }

    /**
     * Returns the "title" metadata property
     */
    getTitle() {
        return this.getMetaProperty("title");
    }

    /**
     * Sets the adaptorId metadata property
     */
    setAdaptorId(adaptorId : string) {
        this.adaptorId = adaptorId;
    } 

    /**
     * Sets the objectTypeId metadata property
     */
    setObjectTypeId(objectTypeId : string) {
        this.snapshot();

        this.objectTypeId = objectTypeId;

        this.setMetaProperty("objectType", objectTypeId);
    }

    /**
     * Sets the objectId metadata property
     */
    setObjectId(objectId : string) {
        this.snapshot();

        this.objectId = objectId;

        this.setMetaProperty("objectId", objectId);
    }

    /**
     * Sets the path metadata property
     */
    setPath(path : string) {
        this.snapshot();

        this.path = path;
    
        this.setMetaProperty("path", path);

        this.adaptorId = path.split("/")[1];
        this.objectTypeId = path.split("/")[2];
        this.objectId = path.split("/")[3];
    }

    /**
     * renameProperty(name, newName)
     * 
     * @param {string} name     Previous name of property.
     * @param {string} newName  New name of property.
     */
    renameProperty(name: string, newName : string) {
        this.snapshot();

        if (this.object)
            this.object[newName] = this.object[name];

        this.properties[newName] = this.properties[name];

        if (this.object)
            delete this.object[name];

        delete this.properties[name];

        this.dirty = true;
        this.dispatchEvent({ eventId: AfwEventId.onPropertiesChanged, source: this, value: this.object  });
        this.dispatchEvent({ eventId: AfwEventId.onChanged, source: this, value: this.object });
    }

    /**
     * removeProperty(name)
     *
     * Removes the property from this object.
     *
     * @param {string} name     Name of the property to remove.
     */
    removeProperty(name: string) {
        this.snapshot();
            
        if (this.object && Object.prototype.hasOwnProperty.call(this.object, name))
            delete this.object[name];

        if (this.properties && Object.prototype.hasOwnProperty.call(this.properties, name))
            delete this.properties[name];

        else if (this.properties && this.properties[name])
            this.properties[name].setValue();

        this.dirty = true;
        this.dispatchEvent({ eventId: AfwEventId.onPropertiesChanged, source: this, value: this.object  });
        this.dispatchEvent({ eventId: AfwEventId.onChanged, source: this, value: this.object });
    }

    /**
     * newProperty(name)
     * 
     * Creates a new property, if allowed by otherProperties.
     * 
     * @param {string} name     Name of the property to add.
     */
    newProperty(name : string) {
        this.snapshot();

        let property : AfwProperty;

        const propertyType = this.getPropertyType(name);
        if (propertyType) {
            property = new AfwProperty({
                propertyName: name, parent: this, propertyType
            });

            if (name) {
                this.dirty = true;
                this.properties[name] = property;
                this.dispatchEvent({ eventId: AfwEventId.onPropertiesChanged, source: this, value: { object: this.object }  });
                this.dispatchEvent({ eventId: AfwEventId.onChanged, source: this, value: property });
            }
        } else {
            throw new Error("No propertyType defined for " + name + ".");
        }

        return property;
    }

    /**
     * Sets the value for a property.
     * 
     * @param {string} name
     * @param {string} value 
     */
    setPropertyValue(name : string, value? : any | undefined) {
        this.snapshot();

        const property = this.getProperty(name);
        if (property) {
            property.setValue(value);            
        } else {
            throw new Error("Property " + name + " not found.");
        }
    }

    /**
     * This iterator will allow users to call (for... of) on it for each AfwProperty.
     */
    [Symbol.iterator]() {
        const properties = this.getProperties();
        let index = -1;

        /* return the "next" and "done" for the iterator to use */
        return {
            next: () => ({ 
                value: properties[++index], 
                done: !(index in properties) 
            })
        };
    }

    /**
     * This allows a callback to iterate over each AfwProperty.
     */
    forEach(callback : (value : AfwProperty, index : number, array : AfwProperty[]) => void, thisArg? : any) : void {
        const properties = this.getProperties();

        properties.forEach(callback, thisArg);
    }

    /**
     * This allows a callback to map AfwProperty values.
     */
    map(callback : (value : AfwProperty, index : number, array : AfwProperty[]) => void, thisArg? : any) : void[] {
        const properties = this.getProperties();

        return properties.map(callback, thisArg);        
    }

    /**
     * Returns the propertyName, if this is an embedded object.
     */
    getEmbeddingProperty() {
        return this.propertyName;
    }

    /**
     * Returns the parent, containing this object, if any.
     */
    getParent() {
        return this.parent;
    }

    /**
     * Checks whether a property with this name exists.
     */ 
    hasProperty(name : string) {

        if (isArray(name)) {
            if (this.hasProperty(name[0])) {
                let property = this.getProperty(name[0]);
                for (let i = 1; i < name.length; i++) {
                    if (property && property.getValue()) {
                        if (property.getValue().hasProperty(name[i])) {
                            property = property.getValue().getProperty(name[i]);
                        } else return false;
                    } else 
                        return false;
                }

                return true;
            }
        } 

        if (this.object && this.object[name] !== undefined)
            return true;        
        else if (this.properties && Object.prototype.hasOwnProperty.call(this.properties, name))
            return true;
        else
            return false;
    }     

    /**
     * fromJSON()
     * 
     * @param {string} json The raw JSON string to parse from.
     */
    fromJSON(json : string, preserve_meta : boolean = true) {
        this.snapshot();

        const meta = this.object?._meta_;
        const object = this.object = JSON.parse(json);

        if (preserve_meta && this.object)
            this.object._meta_ = meta;

        // child properties are no longer valid
        this.properties = {};        

        this.dirty = true;
        this.dispatchEvent({ eventId: AfwEventId.onPropertiesChanged, source: this, value: object });        
        this.dispatchEvent({ eventId: AfwEventId.onChanged, source: this, value: object });
    }

    /**
     * Returns any object errors.
     */
    getErrors(object : IAnyObject | undefined = this.object) : any[] {

        let errors : any[] = [];
        if (object?._meta_?.hasErrors) {

            if (object?._meta_?.propertyTypes) {   
                const propertyTypes: IPropertyTypesObject = object?._meta_?.propertyTypes;
                let propertyName: string;    
                let propertyType: IPropertyType;         

                for ([propertyName, propertyType] of Object.entries(propertyTypes)) {                    
                    if (propertyType?.errors) {
                        errors.push({
                            propertyName,
                            propertyType,
                            errors: propertyType?.errors
                        });
                    }
                }
            }

            /* look through object properties to determine if more errors are embedded */
            for (const [, value] of Object.entries(object)) {
                if (isObject(value) && value?._meta_?.hasErrors) {
                    const embeddedErrors : any[] = this.getErrors(value);
                    errors = errors.concat(embeddedErrors);
                }
            }
        }

        return errors;
    }

    /**
     * Checks if the object has any errors.
     */
    hasErrors() {
        const object = this.object;

        return object?._meta_?.hasErrors;
    }

    /**
     * Turns this AfwObject into a string.
     * 
     * @param {function}    replacer
     * @param {number}      space
     */
    stringify(replacer? : ((key : any, value : any) => any), space : number = 4) {
        const object = this.toJSON(true);
        if (object)
            return JSON.stringify(object, replacer, space);
    }

    /**
     * Turns this AfwObject into a JS object.
     * 
     * @param {boolean} meta Should the JSON object include _meta_ data.
     */
    toJSON(meta : boolean = false) {
        let object: IAnyObject = {};
        
        const properties = this.getProperties();        
        for (const property of properties) {     
            const name = property.getName();
            const value = property.toJSON();

            if (name && (value !== undefined)) {
                object[name] = value;
            }
        }

        if (meta && this.object)
            object["_meta_"] = this.object._meta_; 

        return object;
    }    

    /**
     * Validates an AfwObject by making sure all of its properties are valid.
     * 
     * @param {boolean} recurse Should the object validation be recursive.
     */
    validate(recurse : boolean = false) : any[] {

        let errors : any[] = [];

        /* go through each property and make sure they are valid */        
        for (const property of this.getProperties()) {            
            let error = property.validate(recurse);            
            if (error && error.length > 0)
                errors = errors.concat(error);
        }

        return errors;
    }

    /**
     * Returns a JSON Schema representation of this object.
     */
    getJSONSchema(objectTypeObject = this.getObjectTypeObject(), recurseMeta : boolean = true) {
        const model = this.getModel();
        let jsonSchema : IJSONSchema = { type: "object", properties: {} };
        let requiredProperties: string[] = [];

        if (!model)
            throw new Error("Cannot get JSON Schema without an underlying data model.");

        /* all object types have a _meta_ property, which is described by _AdaptiveMeta_ */    
        if (recurseMeta) {
            const metaObjectTypeObject = model.getObjectTypeObject({ adaptorId: "afw", objectTypeId: "_AdaptiveMeta_" });
            jsonSchema.properties["_meta_"] = this.getJSONSchema(metaObjectTypeObject, false);
        }

        if (objectTypeObject && objectTypeObject.propertyTypes) {
            const propertyTypes : IPropertyTypesObject = objectTypeObject.propertyTypes;
            
            Object.entries(propertyTypes).forEach( ([name, propertyType] : [name: string, propertyType: IPropertyType]) => {       
                if (name === "_meta_")
                    return;

                const {
                    dataType, dataTypeParameter, brief, description,
                    required, allowedValues, defaultValue, allowWrite
                } = propertyType;

                if (required)
                    requiredProperties.push(name);

                jsonSchema.properties[name] = {};
                if (dataType)
                    jsonSchema.properties[name].type = afwDataTypeToJSONSchemaType[dataType];                  

                if (dataType === "list" && dataTypeParameter) {
                    const dataTypeParameters = dataTypeParameter.split(" ");
                    if (dataTypeParameters.length > 1) {
                        /* may have a list of objects, or a list of list of... */
                        if (dataTypeParameters[0] === "object") {
                            const embeddedObjectTypeObject = model.getObjectTypeObject({ adaptorId: this.getAdaptorId(), objectTypeId: dataTypeParameters[1] });
                            if (embeddedObjectTypeObject) {
                                jsonSchema.properties[name].items = this.getJSONSchema(embeddedObjectTypeObject, recurseMeta);                                
                            } else 
                                jsonSchema.properties[name].items = { type: "object" };
                        } else {
                            jsonSchema.properties[name].items = {
                                type: afwDataTypeToJSONSchemaType[dataTypeParameters[0]]
                            };
                        }
                    } else {
                        jsonSchema.properties[name].items = {
                            type: afwDataTypeToJSONSchemaType[dataTypeParameter]
                        };
                    }
                }

                if (dataType === "object" && dataTypeParameter) {                    
                    const embeddedObjectTypeObject = model.getObjectTypeObject({ adaptorId: this.getAdaptorId(), objectTypeId: dataTypeParameter });
                    if (embeddedObjectTypeObject) {
                        jsonSchema.properties[name] = this.getJSONSchema(embeddedObjectTypeObject, recurseMeta);
                    }
                }

                if (brief)
                    jsonSchema.properties[name].title = brief;
                if (description)
                    jsonSchema.properties[name].description = description;        
                if (defaultValue)
                    jsonSchema.properties[name].default = defaultValue;

                if (allowedValues)
                    jsonSchema.properties[name].enum = allowedValues;

                if (allowWrite === false)
                    jsonSchema.properties[name].readonly = true;
            });
        }

        if (objectTypeObject && objectTypeObject.otherProperties) {
            const {dataType, dataTypeParameter} : {dataType?: string; dataTypeParameter?: string} = objectTypeObject.otherProperties;
            if (dataType) {                
                jsonSchema.additionalProperties = {
                    type: afwDataTypeToJSONSchemaType[dataType]
                };      
                
                if (dataType === "object" && dataTypeParameter) {
                    const embeddedObjectTypeObject = model.getObjectTypeObject({ adaptorId: this.getAdaptorId(), objectTypeId: dataTypeParameter });
                    if (embeddedObjectTypeObject) {
                        jsonSchema.additionalProperties = this.getJSONSchema(embeddedObjectTypeObject, recurseMeta);
                    }
                }
            } else 
                jsonSchema.additionalProperties = true;
        }


        if (requiredProperties.length > 0)
            jsonSchema.required = requiredProperties;
        
        return jsonSchema;
    }


    /*     

      The following methods use AfwModel routines.  They are asynchronous and
      interact with the backend store, using the model that was provided.

     */


    /**
     * Refreshes the object from the underlying client.
     * 
     * @param {object} options The object options to pass along to the get_object() method.
     */
    async refresh(options: IObjectOptions) {
        const model = this.getModel();
        if (!model)
            throw new Error("This method requires an adaptive data model.");
        
        const path = this.getPath();
        if (!path)
            throw new Error("Unable to determine Object Path.");       

        this.snapshot();

        const response = model.getObjectWithUri({ 
            uri: path, 
            objectOptions: options,
            modelOptions: { adaptiveObject: false } 
        });
        
        this.object = await response.object;        

        return this;
    }

    /**
     * Reconciles the object with the underlying client.
     * 
     * @param {boolean} checkOnly   A boolean flag to indicate whether the reconcile should only 
     *                              check without actually performing the reconcile operation.
     */
    reconcile(checkOnly: boolean) {
        const model = this.getModel();
        if (!model)
            throw new Error("This method requires an adaptive data model.");

        const client = model.getClient();
        const obj = this.toJSON(true);

        const response = afwReconcileObject(client, obj, checkOnly);        
        if (!checkOnly) {
            const {result} = response;
            const wrappedResult : () => Promise<IJSONObject> = () => new Promise((resolve, reject) => {
                result().then((r: IJSONObject) => {
                    this.dispatchEvent({ eventId: AfwEventId.onSavable, source: this, value: false });
                    resolve(r);
                }).catch((e : any) => reject(e));
            });
            response.result = wrappedResult;
        }

        return response;
    }

    /**
     * Replaces the object with the underlying client.
     */
    replace() {
        const model = this.getModel();
        if (!model)
            throw new Error("This method requires an adaptive data model.");

        const path = this.getPath();
        if (!path)
            throw new Error("Unable to determine Object Path.");

        const client = model.getClient();
        const obj = this.toJSON(true);

        const response = afwReplaceObjectWithUri(client, path, obj);
        const {result} = response;
        const wrappedResult : () => Promise<IJSONObject> = () => new Promise((resolve, reject) => {
            result().then((r: IJSONObject) => {
                this.dispatchEvent({ eventId: AfwEventId.onSavable, source: this, value: false });
                resolve(r);
            }).catch((e : any) => reject(e));
        });
        response.result = wrappedResult;

        return response;
    }

    /**
     * Modifies the object with the underlying client.
     * 
     * @param {string[]} mod_entries The modify entries to apply to the object.
     */
    modify(mod_entries: string[]) {
        const model = this.getModel();
        if (!model)
            throw new Error("This method requires an adaptive data model.");

        const path = this.getPath();
        if (!path)
            throw new Error("Unable to determine Object Path.");

        const client = model.getClient();
        const response = afwModifyObjectWithUri(client, path, mod_entries);    
        const {result} = response;
        const wrappedResult : () => Promise<IJSONObject> = () => new Promise((resolve, reject) => {
            result().then((r: IJSONObject) => {
                this.dispatchEvent({ eventId: AfwEventId.onSavable, source: this, value: false });
                resolve(r);
            }).catch((e : any) => reject(e));
        });
        response.result = wrappedResult;    

        return response;
    }

    /**
     * Deletes the object using the underlying client.
     */
    delete() {
        const model = this.getModel();
        if (!model)
            throw new Error("This method requires an adaptive data model.");

        const path = this.getPath();        
        if (!path)
            throw new Error("Unable to determine Object Path.");

        const client = model.getClient();
        const response = afwDeleteObjectWithUri(client, path);        
        const {result} = response;
        const wrappedResult : () => Promise<IJSONObject> = () => new Promise((resolve, reject) => {
            result().then((r: IJSONObject) => {
                this.dispatchEvent({ eventId: AfwEventId.onSavable, source: this, value: false });
                resolve(r);
            }).catch((e : any) => reject(e));
        });
        response.result = wrappedResult;

        return response;
    }

    /**
     * Adds the object to the underlying client.
     */
    add() {
        const model = this.getModel();
        if (!model)
            throw new Error("This method requires an adaptive data model.");

        const client = model.getClient();
        const obj = this.toJSON();
        
        let response;
        const path = this.getPath();
        if (path) {
            response = afwAddObjectWithUri(client, path, obj);               
            const {result} = response;
            const wrappedResult : () => Promise<IJSONObject> = () => new Promise((resolve, reject) => {
                result().then((r: IJSONObject) => {
                    this.dispatchEvent({ eventId: AfwEventId.onSavable, source: this, value: false });
                    resolve(r);
                }).catch((e : any) => reject(e));
            });
            response.result = wrappedResult;                       
        } else {
            const adaptorId = this.getAdaptorId();
            const objectTypeId = this.getObjectTypeId();
            const objectId = this.getObjectId();
            if (!adaptorId || !objectTypeId)
                throw new Error("Cannot add object without a path or adaptorId and objectTypeId");

            response = afwAddObject(client, objectTypeId, obj, adaptorId, objectId);   
            const {result} = response;            
            const wrappedResult : () => Promise<IJSONObject> = () => new Promise((resolve, reject) => {                
                result().then((r: IJSONObject) => {
                    this.dispatchEvent({ eventId: AfwEventId.onSavable, source: this, value: false });
                    resolve(r);
                }).catch((e : any) => reject(e));
            });
            response.result = wrappedResult;         
        }  

        return response;
    }
}

export default AfwObject;