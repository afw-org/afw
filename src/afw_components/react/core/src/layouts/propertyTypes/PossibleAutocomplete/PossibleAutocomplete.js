// See the 'COPYING' file in the project root for licensing information.
import {Autocomplete} from "@afw/react";

export const PossibleAutocomplete = (props) => {
    const {property} = props;

    const possibleOptions = property.getPossibleValues().map((value, index) => ({
        key: String(index),
        text: String(value),
        value,
    }));

    let value;
    if (value) {
        possibleOptions.forEach(o => {
            if (o.value === props.value)
                value = o;
        });
    }

    return (
        <Autocomplete 
            {...props} 
            options={possibleOptions} 
            value={value}
            onChanged={opt => props.onChanged(opt.value)}
        />
    );    
};

export default PossibleAutocomplete;
