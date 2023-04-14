// See the 'COPYING' file in the project root for licensing information.
import {Dropdown} from "@afw/react";

export const AllowableDropdown = (props) => {
    const {property} = props;

    const allowedOptions = property.getAllowedValues().map((value, index) => ({
        key: String(index),
        text: String(value),
        value,
    }));
    
    let selectedKey;

    if (props.value) {
        allowedOptions.forEach((o, key) => {
            if (o.value === props.value)
                selectedKey = String(key);
        });
    }

    return (
        <Dropdown 
            {...props} 
            options={allowedOptions} 
            selectedKey={selectedKey}
            onChanged={opt => props.onChanged(opt.value)}
        />
    );
    
};

export default AllowableDropdown;
