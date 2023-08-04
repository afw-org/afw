// See the 'COPYING' file in the project root for licensing information.
import {useMemo, useCallback} from "react";
import PropTypes from "prop-types";

import {Dropdown} from "@afw/react";

import {useDataTypes} from "@afw/react";

/**
 * 
 * @param {string} ariaLabel 
 * @param {string} description
 * @param {boolean} disabled 
 * @param {string} label
 * @param {function} onChanged
 * @param {string} value
 */
export const DataTypeDropdown = (props) => {
       
    const {dataTypes} = useDataTypes();

    const {
        id,
        "aria-label": ariaLabel,
        description,
        disabled,
        required,
        label,
        onChanged,
        value
    } = props;

    const options = useMemo(() => {
        /* map each dataType to a Dropdown item */
        if (dataTypes) {
            return dataTypes.sort((a, b) => {
                return (a.dataType.toLowerCase().localeCompare(
                    b.dataType.toLowerCase()));
            }).map(dataType => ({
                key: dataType.dataType,
                text: dataType.dataType,
            }));            
        }

    }, [dataTypes]);

    const handleChange = useCallback((option) => {
        if (onChanged)
            onChanged(option.key);
    }, [onChanged]);        
        
    return (
        <Dropdown 
            id={id}
            data-component-type="DataTypeDropdown"
            label={label}
            aria-label={ariaLabel}
            disabled={disabled}
            required={required}            
            options={options}
            selectedKey={value}                    
            onChanged={handleChange}
            description={description}            
        />                
    );
};

DataTypeDropdown.propTypes = {
    "aria-label":          PropTypes.string,
    className:          PropTypes.string,
    description:        PropTypes.string,
    disabled:           PropTypes.bool,
    label:              PropTypes.string,
    onChanged:          PropTypes.func,
    value:              PropTypes.string,
};

export default DataTypeDropdown;
