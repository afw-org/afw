// See the 'COPYING' file in the project root for licensing information.
import {useMemo, useCallback} from "react";
import PropTypes from "prop-types";

import {Dropdown} from "@afw/react";
import {useAdaptors} from "@afw/react";

/* \fixme find an appropriate home for these */    
const AdaptorIcons = {
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
 * AdaptorDropdown presents a Dropdown component with the options
 * that represent adaptors that are currently active.
 * 
 * @param {string} label
 * @param {string} description 
 * @param {string} selectedAdaptorId
 * @param {boolean} disabled
 * @param {boolean} required
 * @param {boolean} isModelLocation
 * @param {boolean} isPolicyLocation
 * @param {boolean} isDevelopmentInput
 * @param {string} type
 * 
 */
export const AdaptorDropdown = (props) => {    
    const {adaptors} = useAdaptors(props.adaptorType);

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

    const adaptorOptions = useMemo(() => {
        if (!adaptors)
            return;        
        
        /* use filter, sort and map to transform the adaptors into dropdown items */
        const adaptorOptions = adaptors.filter(adaptor => {                        

            if (isModelLocation)
                return adaptor.properties.isModelLocation;                

            if (isPolicyLocation)
                return adaptor.properties.isPolicyLocation;                

            if (isDevelopmentInput)
                return adaptor.properties.isDevelopmentInput;                
            
            /* passed the filters */
            return true;
        }).sort((A, B) => {
            /* first sort by type */
            if (A?.properties?.adaptorType.localeCompare(B?.properties?.adaptorType) !== 0)
                return (A.properties.adaptorType.localeCompare(B.properties.adaptorType));

            /* sort by adaptorId */
            const a = A.adaptorId.toLowerCase();
            const b = B.adaptorId.toLowerCase();

            return a.localeCompare(b);
        }).map(adaptor => {
            /* map each adaptor to an Dropdown item, including icons */
            const adaptorId = adaptor.adaptorId;
            const adaptorType = adaptor?.properties?.adaptorType;
            const icon = AdaptorIcons[adaptorType];

            return ({
                adaptor,
                key: adaptorId,
                text: adaptorId,                
                icon,
            });
        });

        for (let i = 0; i < adaptorOptions.length; i++) {
            const currType = adaptorOptions[i].adaptor.properties.adaptorType;
            const prevType = (i > 0) ? adaptorOptions[i-1].adaptor.properties.adaptorType : currType;

            if (currType !== prevType) {
                adaptorOptions.splice(i, 0, { key: currType + "-divider", type: "divider" });
                adaptorOptions.splice(i+1, 0, { key: currType + "-header", type: "header", text: currType + " adaptors" });
                i+=2;
            }
        }

        if (allowNone) {
            adaptorOptions.unshift({
                key: "none",
                text: "(None)",
                value: "",
            });
        }
                
        return adaptorOptions;
    }, [adaptors, isModelLocation, isPolicyLocation, isDevelopmentInput, allowNone]);
    

    const handleOnChanged = useCallback((selectedAdaptorId) => {
        if (onChanged) {
            onChanged(selectedAdaptorId.key, selectedAdaptorId.adaptor);
        }
    }, [onChanged]);

    return (
        <Dropdown       
            id={id}    
            data-testid={dataTestid}
            data-component-type="AdaptorDropdown"  
            aria-label={ariaLabel} 
            label={label ? label : "Adaptor"}
            description={description ? description : "Select an AdaptorId"}
            options={adaptorOptions}
            selectedKey={adaptorOptions ? value : adaptorOptions}
            onChanged={handleOnChanged}
            disabled={disabled}
            required={required}
        />
    );
};

AdaptorDropdown.propTypes = {
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

export default AdaptorDropdown;
