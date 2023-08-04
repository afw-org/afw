// See the 'COPYING' file in the project root for licensing information.
import {useMemo} from "react";

import {Autocomplete} from "@afw/react";

export const PropertyTypeDropdown = (props) => {

    const {id, "aria-label" : ariaLabel, "data-testid": dataTestId, allowQuery, objectTypeObject, value, onFilter} = props;

    const propertyTypeOptions = useMemo(() => {
        let options = [];
        if (objectTypeObject) {      
            const propertyTypeObjects = objectTypeObject.getPropertyValue("propertyTypes");            
            if (propertyTypeObjects && propertyTypeObjects.getProperties()) {
                            
                propertyTypeObjects.getProperties().forEach(propertyType => {                                        
                    const propertyName = propertyType.getName();
                    const value = propertyType.getValue();
                    const label = value ? value.getPropertyValue("label") : undefined;

                    if (onFilter && !onFilter(value))
                        return;

                    /* if we only care about allowQuery=true, then skip this one */
                    if (allowQuery && value && value.getPropertyValue("allowQuery") !== true)
                        return;

                    options.push({
                        key: propertyName,
                        text: label ? label : propertyName,
                        propertyTypeObject: value,
                    });
                });
            }
        }
        return options;
    }, [allowQuery, objectTypeObject, onFilter]);

    let selectedPropertyTypeOption;    
    for (let option of propertyTypeOptions) {
        if (option.key === value)
            selectedPropertyTypeOption = option;
    }    

    const onSelectPropertyType = (selectedPropertyTypeOption) => {
        if (props.onChanged)
            props.onChanged(
                selectedPropertyTypeOption ?
                    selectedPropertyTypeOption.key : null, 
                selectedPropertyTypeOption ?
                    selectedPropertyTypeOption.propertyTypeObject : null
            );
    };

    const {label, description, disabled} = props;

    return (
        <Autocomplete 
            id={id}
            data-testid={dataTestId}
            data-component-type="PropertyTypeDropdown"
            aria-label={ariaLabel}
            label={label}
            placeholder="Property Type"
            noOptionsText="No Property"
            options={propertyTypeOptions}
            value={selectedPropertyTypeOption}
            onChanged={onSelectPropertyType}
            description={description}
            disabled={disabled}            
        />
    );
};

export default PropertyTypeDropdown;
