// See the 'COPYING' file in the project root for licensing information.
import {AfwObject, AfwArray, AfwValue, AfwEventId } from ".";

import {isObject, isArray} from "../utils";
import {IMetaObject} from "./types";

interface Config {    
    parent?:        any;
    propertyName?:  string;
    value?:         any;
    getMeta?:       () => any;
}

export class AfwArrayEntry extends AfwValue {

    parent:                 any;
    getMeta:                () => any;

    constructor(config: Config) {
        super(config);

        this.getMeta    = config.getMeta ? config.getMeta : () => undefined;
        this.parent     = config.parent;
    }

    getMetaProperty(name: string) {          
        if (this._meta_ && this._meta_[name])
            return this._meta_[name];
    }

    getDataType() {
        if (this.getMetaProperty("dataType"))
            return this.getMetaProperty("dataType");

        const meta = this.getMeta();
        const dataTypeParameter = meta?.dataTypeParameter;

        if (dataTypeParameter && dataTypeParameter.indexOf(" ") > 0)
            return dataTypeParameter.split(" ")[0];
        else if (dataTypeParameter)
            return meta.dataTypeParameter;
    }

    getDataTypeParameter() {
        if (this.getMetaProperty("dataType"))
            return this.getMetaProperty("dataTypeParameter");

        const meta = this.getMeta();
        const dataTypeParameter = meta?.dataTypeParameter;

        if (dataTypeParameter && dataTypeParameter.indexOf(" ") > 0) {
            return dataTypeParameter.split(" ")[1];
        }
    }

    /*! \fixme this shouldn't be valid for AfwArrayEntry types */
    getName() {        
        return this.getPropertyName();
    }

    getValue() {
        const {value, getMeta} = this;        
        const propertyName = this.getPropertyName();
        const dataType = this.getDataType();
        const dataTypeParameter = this.getDataTypeParameter();
        const meta = getMeta();

        if (!this.adaptiveValue) {
            if (dataType === "object" || isObject(value)) {
                const embeddingObject = this.getEmbeddingObject();
                let objectTypeObject : IMetaObject | undefined;
                if (embeddingObject) {
                    const model = embeddingObject.getModel();
                    const adapterId = embeddingObject.getAdapterId();
                    objectTypeObject = model.getObjectTypeObject({ adapterId, objectTypeId: dataTypeParameter });
                }

                this.adaptiveValue = new AfwObject({
                    objectTypeId: dataTypeParameter,                
                    object: value,
                    parent: this,
                    propertyName,      
                    objectTypeObject,       
                });
            } else if (dataType === "array" || isArray(value)) {
                this.adaptiveValue = new AfwArray({
                    value,
                    parent: this,
                    getMeta: () => ({
                        ...meta,
                        dataType,
                        dataTypeParameter
                    }),
                    propertyName
                });
            } else 
                this.adaptiveValue = value;
        }            

        return this.adaptiveValue;
    }

    setValue(value: any) {
        if (value instanceof AfwArrayEntry)
            this.value = value.toJSON();
        else
            this.value = value;

        // force us to rebuild the value, in case it was an Object or List
        this.adaptiveValue = undefined;

        this.dispatchEvent({ eventId: AfwEventId.onChanged, source: this, value });
    }

    remove() {
        this.parent.removeValue(this);
    }

    isOtherPropertyType() {
        return false;
    }

    getPath() {
        const embeddingObject = this.getEmbeddingObject();
        if (embeddingObject.getPath())
            return embeddingObject.getPath() + "/" + this.propertyName;
        else
            return this.propertyName;
    }

    /**
     * validate()
     *
     * Validates a list entry conforms to its propertyType.
     */
    validate(recurse: boolean): any[] {
        /* \fixme */
        return [];
    }
}

export default AfwArrayEntry;