// See the 'COPYING' file in the project root for licensing information.
import {AfwObject, AfwList, AfwValue, AfwEventId} from ".";

interface PropertyType {
    dataType?:          string;
    dataTypeParameter?: string;
    label?:             string;
    brief?:             string;
    description?:       string;
    [prop: string]:     any;
}

interface Config {    
    parent?:        any;
    propertyName:   string;
    propertyType?:  PropertyType;
    value?:         any;
}

export class AfwProperty extends AfwValue {

    constructor(config: Config) {
        super(config);
    }

    /**
     * Removes a property from the parent object.
     */
    remove() {
        const obj = this.getEmbeddingObject();
        if (obj)
            obj.removeProperty(this.getPropertyName());
        else
            this.setValue();
    }

    /**          
     * Returns whether or not this property is created from an 
     * "otherProperties" propertyType
     * @returns whether or not the propertyType is otherProperties type.
     */
    isOtherPropertyType() {
        const obj = this.getEmbeddingObject();
        if (obj) {
            const objectTypeObject = obj.getObjectTypeObject();
            if (objectTypeObject?.otherProperties && this.getPropertyName()) {
                if (! (objectTypeObject?.propertyTypes && objectTypeObject?.propertyTypes[this.getPropertyName()!]))                
                    return true;
            }
        }

        return false;
    }

    getName() {
        return this.getPropertyName();
    }

    /**
     * Returns the property's path.
     */
    getPath() {
        if (this.parent && this.parent.getPath())
            return this.parent.getPath() + "/" + this.getPropertyName();
        else
            return this.getPropertyName();
    }

    /**
     * Returns the propertyType object associated with this property.
     */
    getPropertyType() {
        if (this.parent && this.getPropertyName()) {
            return this.parent.getPropertyType(this.getPropertyName());
        }
    }

    /**
     * If this property dataType=object, this returns the objectType object associated
     * with its value.
     */
    getObjectTypeObject() {
        const dataType = this.getDataType();
        const objectTypeId = this.getDataTypeParameter();

        if (this.parent && (dataType === "object") && objectTypeId) {
            const model = this.parent.getModel();
            const adaptorId = this.parent.getAdaptorId();

            return model.getObjectTypeObject({ adaptorId, objectTypeId });
        }
    }

    /**
     * Sets the property's name.
     * 
     * @param {string} name new property name.
     */
    setName(name: string) {
        if (!name)
            throw new Error("Property name must not be empty.");

        const parent = this.getParent();
        if (parent) {
            parent.getProperties().forEach((property: AfwProperty) => {
                if (property.getName() === name)
                    throw new Error("Invalid property name (already present)!");
            });

            parent.renameProperty(this.getPropertyName(), name);
        }

        this.setPropertyName(name);

        this.dispatchEvent({ eventId: AfwEventId.onChanged, source: this, propertyName: name });
    }

    /**          
     * Returns any errors that exist for this property.
     * 
     * @param {boolean} recurse recursively validate the property.
     * @returns An array of errors.
     */
    validate(recurse: boolean = false) : any[] {
        let errors : any[] = [];

        /* make sure it has a value, if required */
        if (this.getRequired() && this.getValue() === undefined) {
            errors.push({
                property: this, 
                message: "property is required"
            });
        }

        if (this.getDataType() === "object" && recurse) {
            let object = this.getValue();
            if (object) {
                let embeddedErrors = object.validate(recurse);
                if (embeddedErrors && embeddedErrors.length > 0)
                    errors = errors.concat(embeddedErrors);
            }
        }

        return errors;
    }
}

export default AfwProperty;