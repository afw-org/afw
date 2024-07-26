// See the 'COPYING' file in the project root for licensing information.
import {useState, useCallback, useEffect} from "react";
import PropTypes from "prop-types";

import {Autocomplete} from "@afw/react";

import {useRetrieveObjects} from "@afw/react";

const objectTypeId = "_AdaptiveObjectType_";
const objectOptions = {
    path: true, 
    objectId: true, 
    objectType: true
};

/**
 * The ObjectTypeDropdown component read Object Types from an adapter and
 * presents them in Dropdown component, so one can be selected.
 * 
 * @param {string} adapterId
 * @param {string} value
 * @param {boolean} requireEntity
 * @param {function} onChanged
 * @param {string} label
 * @param {string} description
 * @param {boolean} disabled
 * 
 */
export const ObjectTypeDropdown = (props) => {

    const [objectTypeOptions, setObjectTypeOptions] = useState([]);
    const [selectedObjectTypeOption, setSelectedObjectTypeOption] = useState();

    const {
        id,
        "data-testid": dataTestid,
        adapterId, 
        value, 
        requireEntity, 
        onChanged, 
        label, 
        description, 
        disabled, 
        ignoreAdaptive, 
        defaultFirstNonAdaptive, 
        autoSelect,
        onError
    } = props;

    const {objects, isLoading, error} = useRetrieveObjects({ 
        adapterId,
        objectTypeId, 
        objectOptions
    });

    useEffect(() => {
        let objectTypeOptions = [];

        if (objects && objects.length > 0) {
            let objectTypeObjects = objects;                        
            
            /* only allow objectTypeObjects with allowEntity to be in the dropdown */
            if (requireEntity === true)
                objectTypeObjects = objectTypeObjects.filter(objectTypeObject => 
                    objectTypeObject.getPropertyValue("allowEntity") !== false);            

            /* create dropdown options for each object type object */
            objectTypeObjects.forEach(objectTypeObject => {
                let objectId = objectTypeObject.getObjectId();
                if (ignoreAdaptive && objectId && objectId.startsWith("_Adaptive"))
                    return;

                if (objectId) {
                    const option = {
                        key: objectId,
                        text: objectId,
                        description: objectTypeObject.getPropertyValue("description"),
                        objectTypeObject,
                    };

                    objectTypeOptions.push(option);                                 
                }
            });

            /* sort the object types alphabetically */
            objectTypeOptions.sort((A, B) => {
                const a = A.key.toLowerCase();
                const b = B.key.toLowerCase();

                return a.localeCompare(b);
            });
        }

        setSelectedObjectTypeOption();
        setObjectTypeOptions(objectTypeOptions);
    }, [ignoreAdaptive, objects, requireEntity]);

    useEffect(() => {         
        let selectedObjectTypeOption;
        if (autoSelect === false)
            return;

        if (value) {
            for (let i = 0; i < objectTypeOptions.length; i++) {
                if (value === objectTypeOptions[i].key) {
                    selectedObjectTypeOption = objectTypeOptions[i];
                    break;
                }
            }                
        }

        if (!selectedObjectTypeOption) {                
            if (defaultFirstNonAdaptive) {
                for (let i = 0; i < objectTypeOptions.length; i++) {
                    if (!objectTypeOptions[i].key.startsWith("_Adaptive")) {
                        selectedObjectTypeOption = objectTypeOptions[i];
                        break;
                    }
                }

                /* can't find one, so choose first one */
                if (!selectedObjectTypeOption)
                    selectedObjectTypeOption = objectTypeOptions[0];
            } else
                selectedObjectTypeOption = objectTypeOptions[0];
        }

        if (selectedObjectTypeOption) {
            if (onChanged)
                onChanged(selectedObjectTypeOption.key, selectedObjectTypeOption.objectTypeObject);
            setSelectedObjectTypeOption(selectedObjectTypeOption);
        }                    
    }, [autoSelect, defaultFirstNonAdaptive, objectTypeOptions, onChanged, value]);

    useEffect(() => {
        for (let option of objectTypeOptions) {
            if (option.key === value)
                setSelectedObjectTypeOption(option);
        }
    }, [value, objectTypeOptions]);
    
    useEffect(() => {
        if (error && onError) {            
            onError(error);
        }
    
        // we don't want to call onError every time it onError changes, because
        // the caller may have provided an anonymous function that changes on
        // every render.    
        // eslint-disable-next-line react-hooks/exhaustive-deps
    }, [error]);

    const onCreateObjectTypeOption = useCallback((value) => {
        if (!value)
            return;

        let options = [...objectTypeOptions];
        const selectedObjectTypeOption = {
            key: value,
            text: value,
            description: value + " Object Type"
        };

        options.push(selectedObjectTypeOption);

        setObjectTypeOptions(options);
        setSelectedObjectTypeOption(selectedObjectTypeOption);            

        if (onChanged)
            onChanged(selectedObjectTypeOption.key);
    }, [objectTypeOptions, setObjectTypeOptions, setSelectedObjectTypeOption, onChanged]);

    const onSelectObjectType = useCallback((selectedObjectTypeOption) => {
        if (selectedObjectTypeOption && onChanged)
            onChanged(selectedObjectTypeOption.key, selectedObjectTypeOption.objectTypeObject);
    }, [onChanged]);

    return (
        <Autocomplete 
            id={id}    
            data-testid={dataTestid}
            data-component-type="ObjectTypeDropdown"
            label={label}
            placeholder="Object Type"
            description={description}
            options={objectTypeOptions}
            isLoading={isLoading}
            value={selectedObjectTypeOption}
            onChanged={onSelectObjectType}
            onCreateOption={onCreateObjectTypeOption}
            disabled={disabled || isLoading}
            error={(error && error.message) ? error.message : undefined}
            noOptionsText="No ObjectType"
        />
    );    
};

ObjectTypeDropdown.propTypes = {    
    description:            PropTypes.string,
    disabled:               PropTypes.bool,
    label:                  PropTypes.string,
    onChanged:              PropTypes.func,
    value:                  PropTypes.string,
    adapterId:              PropTypes.string,
    requireEntity:          PropTypes.bool,
};

export default ObjectTypeDropdown;
