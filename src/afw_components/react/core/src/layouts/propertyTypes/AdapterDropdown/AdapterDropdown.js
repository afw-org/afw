// See the 'COPYING' file in the project root for licensing information.
import {useMemo, useCallback} from "react";
import PropTypes from "prop-types";

import {Dropdown} from "@afw/react";
import {useAdapters} from "@afw/react";

/* \fixme find an appropriate home for these */    
const AdapterIcons = {
    "afw_runtime":      "Atom",
    "model":            "Sitemap",
    "file":             "FileMultiple",
    "lmdb":             "Flash",    
    "ldap":             "FileTree",
    "oracle":           "Database",
    "sql":              "Database",
    "dbd_freetds":      "Database",
    "dbd_mysql":        "Database",
    "dbd_odbc":         "Database",
    "dbd_postgresql":   "Database",
    "dbd_sqlite3":      "Database",
    "bdb":              "Flash",
    "dynamo_db":        "Cloud",
    "vfs":              "Server",
};

/**
 * AdapterDropdown presents a Dropdown component with the options
 * that represent adapters that are currently active.
 * 
 * @param {string} label
 * @param {string} description 
 * @param {string} selectedAdapterId
 * @param {boolean} disabled
 * @param {boolean} required
 * @param {boolean} isModelLocation
 * @param {boolean} isPolicyLocation
 * @param {boolean} isDevelopmentInput
 * @param {string} type
 * 
 */
export const AdapterDropdown = (props) => {    
    const {adapters} = useAdapters(props.adapterType);

    const {
        id,
        "data-testid": dataTestid,
        "aria-label": ariaLabel,
        label, 
        description, 
        value, 
        disabled, 
        required,
        isModelLocation,
        isPolicyLocation,
        isDevelopmentInput,
        onChanged,
        allowNone,        
    } = props;

    const adapterOptions = useMemo(() => {
        if (!adapters)
            return;        
        
        /* use filter, sort and map to transform the adapters into dropdown items */
        const adapterOptions = adapters.filter(adapter => {                        

            if (isModelLocation)
                return adapter.properties.isModelLocation;                

            if (isPolicyLocation)
                return adapter.properties.isPolicyLocation;                

            if (isDevelopmentInput)
                return adapter.properties.isDevelopmentInput;                
            
            /* passed the filters */
            return true;
        }).sort((A, B) => {
            /* first sort by type */
            if (A?.properties?.adapterType.localeCompare(B?.properties?.adapterType) !== 0)
                return (A.properties.adapterType.localeCompare(B.properties.adapterType));

            /* sort by adapterId */
            const a = A.adapterId.toLowerCase();
            const b = B.adapterId.toLowerCase();

            return a.localeCompare(b);
        }).map(adapter => {
            /* map each adapter to an Dropdown item, including icons */
            const adapterId = adapter.adapterId;
            const adapterType = adapter?.properties?.adapterType;
            const icon = AdapterIcons[adapterType];

            return ({
                adapter,
                key: adapterId,
                text: adapterId,                
                icon,
            });
        });

        for (let i = 0; i < adapterOptions.length; i++) {
            const currType = adapterOptions[i].adapter.properties.adapterType;
            const prevType = (i > 0) ? adapterOptions[i-1].adapter.properties.adapterType : currType;

            if (currType !== prevType) {
                adapterOptions.splice(i, 0, { key: currType + "-divider", type: "divider" });
                adapterOptions.splice(i+1, 0, { key: currType + "-header", type: "header", text: currType + " adapters" });
                i+=2;
            }
        }

        if (allowNone) {
            adapterOptions.unshift({
                key: "none",
                text: "(None)",
                value: "",
            });
        }
                
        return adapterOptions;
    }, [adapters, isModelLocation, isPolicyLocation, isDevelopmentInput, allowNone]);
    

    const handleOnChanged = useCallback((selectedAdapterId) => {
        if (onChanged) {
            onChanged(selectedAdapterId.key, selectedAdapterId.adapter);
        }
    }, [onChanged]);

    return (
        <Dropdown       
            id={id}    
            data-testid={dataTestid}
            data-component-type="AdapterDropdown"  
            aria-label={ariaLabel} 
            label={label ? label : "Adapter"}
            description={description ? description : "Select an AdapterId"}
            options={adapterOptions}
            selectedKey={adapterOptions ? value : adapterOptions}
            onChanged={handleOnChanged}
            disabled={disabled}
            required={required}
        />
    );
};

AdapterDropdown.propTypes = {
    label:                          PropTypes.string,
    description:                    PropTypes.string,
    value:                          PropTypes.string,
    onChanged:                      PropTypes.func,
    disabled:                       PropTypes.bool,
    type:                           PropTypes.string,
    isModelLocation:                PropTypes.bool,
    isPolicyLocation:               PropTypes.bool,
    isDevelopmentInput:             PropTypes.bool,    
};

export default AdapterDropdown;
