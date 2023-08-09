// See the 'COPYING' file in the project root for licensing information.
import {AfwObject} from ".";
import {AfwList} from ".";
import {AfwListEntry} from ".";
import {AfwEvent, AfwEventId} from ".";

import {
    isObject, 
    isArray, 
    isString, 
    isInteger, 
    isBoolean,    
} from "../utils";

import {
    IMetaObject,
    IPropertyType,    
} from "./types";

interface Config {    
    parent?:        any;
    propertyName?:  string;
    value?:         any;
    propertyType?:  IPropertyType;    
    debounce?:      boolean;
}

export class AfwValue extends AfwEvent {

    propertyName?:  string;
    adaptiveValue:  any;
    propertyType?:  IPropertyType;
    _meta_?:        IMetaObject;
    value?:         any;
    initialValue?:  string;

    constructor(config: Config) {
        super(config);

        this.propertyName   = config?.propertyName;
        this.propertyType   = config?.propertyType;  
        this.value          = config?.value;      
        this.debounce       = config?.debounce;
        
        if (this.debounce === undefined)
            this.debounce = this.getEmbeddingObject()?.getModel()?.getDebounce(); 

    }

    /**
     * getMeta()
     *      
     */
    getMeta() {
        return {
            ...this.propertyType,
            ...this._meta_
        };
    }

    /**
     * getMetaProperty()
     * 
     */
    getMetaProperty(name: string) {
        return (
            (this._meta_ && this._meta_[name]) ?
                this._meta_[name] :
                (this.propertyType ?
                    this.propertyType[name] : undefined)
        );
    }

    /**
     * setMetaProperty()
     * 
     * \fixme we need a way to indicate that the _meta_ property that was 
     * set, was changed here and not from the original document.  This way, 
     * we can know we have the capability to change it later.
     */
    setMetaProperty(name: string, value: any) {
        if (!this._meta_)
            this._meta_ = {};

        this._meta_[name] = value;

        /* !\fixme change to onMetaChanged? */
        this.dispatchEvent({ eventId: AfwEventId.onChanged, source: this, value });
    }

    /**
     * getAllowedValues()
     */
    getAllowedValues() {
        return this.getMetaProperty("allowedValues");
    }


    /**
     * getAllowWrite()
     */
    getAllowWrite() {
        return this.getMetaProperty("allowWrite");
    }

    /**
     * getAllowQuery()
     */
    getAllowQuery() {
        return this.getMetaProperty("allowQuery");
    }

    /**
     * getDataType()
     * 
     */
    getDataType() {
        const dataType = this.getMetaProperty("dataType");
        if (dataType)
            return dataType;
        
        const value = this.value;

        /* try to interpret the dataType from the value */
        if (value instanceof AfwList)
            return "array";
        else if (value instanceof AfwObject)
            return "object";
        else if (isArray(value))
            return "array";
        else if (isObject(value))
            return "object";
        else if (isInteger(value))
            return "integer";
        else if (isBoolean(value))
            return "boolean";
        else if (isString(value))
            return "string";
        
    }

    /**
     * Gets the contextType meta property.
     * 
     */
    getContextType() {
        return this.getMetaProperty("contextType");
    }

    /**
     * Gets the dataTypeParameter meta property.
     */
    getDataTypeParameter() {
        return this.getMetaProperty("dataTypeParameter");
    }

    /**
     * Gets the defaultValue meta property.
     */
    getDefaultValue() {
        return this.getMetaProperty("defaultValue");
    }

    /**
     * Gets the brief meta property.
     */
    getBrief() {
        return this.getMetaProperty("brief");
    }

    /**
     * Gets the description meta property.
     */
    getDescription() {
        return this.getMetaProperty("description");
    }

    /**
     * Gets the inherited meta property.
     */
    getInheritedFrom() {
        return this.getMetaProperty("inheritedFrom");
    }

    /**
     * 
     */
    getLabel() {
        return this.getMetaProperty("label");
    }

    /**
     * Gets the lowerBound meta property.
     */
    getLowerBound() {
        return this.getMetaProperty("lowerBound");
    }

    /**
     * Gets the possibleValues meta property.
     */
    getPossibleValues() {
        return this.getMetaProperty("possibleValues");
    }

    /**
     * Gets the required meta property.
     */
    getRequired() {
        return this.getMetaProperty("required");
    }

    /**
     * Gets the skeleton meta property.
     */
    getSkeleton() {
        return this.getMetaProperty("skeleton");
    }

    /**
     * Gets the upperBound meta property.
     */
    getUpperBound() {
        return this.getMetaProperty("upperBound");
    }

    
    /**
     * getValue()
     */
    getValue() {
        const {propertyName, value} = this;
        const dataType = this.getDataType();
        const dataTypeParameter = this.getDataTypeParameter();
        
        if (!this.adaptiveValue) {

            if ((dataType === "object") && (value !== undefined)) 
            {
                let objectTypeObject;
                const embeddingObject = this.getEmbeddingObject();
                if (embeddingObject) {
                    const model = embeddingObject.getModel();
                    const adaptorId = embeddingObject.getAdaptorId();
                    objectTypeObject = dataTypeParameter ? 
                        model.getObjectTypeObject({ adaptorId, objectTypeId: dataTypeParameter }) :
                        undefined;
                }

                this.adaptiveValue = new AfwObject({ 
                    object: value, 
                    objectTypeId: dataTypeParameter,
                    parent: this,
                    propertyName,
                    objectTypeObject,
                });
            } 
            
            else if ((dataType === "array") && (value !== undefined)) 
            {           
                this.adaptiveValue = new AfwList({ 
                    value, 
                    parent: this,
                    getMeta: () => this.getMeta(),
                    propertyName,
                });
            } 

            else if ((dataType === "double") && (value === "Infinity"))            
                this.adaptiveValue = Infinity;
            
            else if ((dataType === "double") && (value === "-Infinity"))            
                this.adaptiveValue = -Infinity;

            else if ((dataType === "double") && (value === "NaN"))            
                this.adaptiveValue = NaN;
            
            else if (typeof(value) === "symbol")
                this.adaptiveValue = "[symbol]";

            else if (typeof(value) === "function")
                this.adaptiveValue = "[function]";
                
            else
                this.adaptiveValue = value;
        }

        return this.adaptiveValue;
    }

    /**
     * setDataType()
     */
    setDataType(dataType: string) {
        this.setMetaProperty("dataType", dataType);
    }

    /**
     * setObjectType()
     */
    setObjectType(objectType: string) {
        this.setDataTypeParameter(objectType);
    }

    /**
     * setDataTypeParameter()
     */
    setDataTypeParameter(dataTypeParameter: string) {
        this.setMetaProperty("dataTypeParameter", dataTypeParameter);
    }

    /**
     * setValue()
     * 
     */
    setValue(value? : any | undefined) {

        /* go ahead and store away the initial value so we have the ability to undo changes */
        if (!this.initialValue && this.value)
            this.initialValue = JSON.stringify(this.value);

        if (value instanceof AfwObject || value instanceof AfwList || value instanceof AfwListEntry)
            this.value = value.toJSON();               
        else 
            this.value = value;

        // force us to rebuild the value, if it was an Object or List
        this.adaptiveValue = undefined;
        
        this.dispatchEvent({ eventId: AfwEventId.onChanged, source: this, value });

        return this.getValue();
    }

    

    /*

        General-purpose functions.

     */

    /**
     * Discards any changes that have been made to this value, since 
     * its construction,
     */
    discardChanges() {
        if ((this.value !== undefined) && (this.initialValue !== undefined))
            this.value = JSON.parse(this.initialValue);
        
        this.adaptiveValue = undefined;

        this.dispatchEvent({ eventId: AfwEventId.onChanged, source: this, value: this.value });
        this.dispatchEvent({ eventId: AfwEventId.onDiscardChanges, source: this, value: this.value });
    }

    /**
     * 
     */
    getParent() {
        return this.parent;
    }

    /**
     * 
     */
    getPropertyName() {
        return this.propertyName;
    }

    /**
     * 
     */
    getEmbeddingObject() {
        if (this.parent instanceof AfwObject)
            return this.parent;
        else if (this.parent)
            return this.parent.getEmbeddingObject();
    }

    setPropertyName(name: string) {
        this.propertyName = name;
    }

    /**
     * 
     */
    toJSON(meta = false) {
        const value = this.getValue();
        if (!value)
            return value;

        if (this.getDataType() === "object") {  
            return value.toJSON(meta);
        } else if (this.getDataType() === "array") {
            return value.toJSON(meta);
        } else return value;
    }

    /**
     * validate() 
     *
     * Validates a value conforms to its propertyType.
     */
    // eslint-disable-next-line no-unused-vars
    validate(recurse: boolean = false) : any[] {

        let errors: any[] = [];

        return errors;

        /* \fixme

        if (this.getRequired() && (this.value === undefined))
            return false;

        
        return true;
        */
    }
}

export default AfwValue;