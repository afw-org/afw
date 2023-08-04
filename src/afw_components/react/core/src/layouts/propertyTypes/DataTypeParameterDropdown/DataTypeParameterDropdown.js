// See the 'COPYING' file in the project root for licensing information.
import {
    Autocomplete,
    TextField, 
} from "@afw/react";
import { useEventId } from "../../../hooks";

const stringOptions = [
    { key: "text/plain",    text: "text/plain"      },
    { key: "text/html",     text: "text/html"       },
    { key: "text/xml",      text: "text/xml"        }, 
    { key: "text/css",      text: "text/css"        },
    { key: "text/csv",      text: "text/csv"        },
];

const binaryOptions = [
    { key: "audio/mpeg",    text: "audio/mpeg"      },
    { key: "audio/ogg",     text: "audio/ogg"       },    
    { key: "image/gif",     text: "image/gif"       },
    { key: "image/jpeg",    text: "image/jpeg"      },
    { key: "image/png",     text: "image/png"       },
    { key: "video/mpeg",    text: "video/mpeg"      },
];

// fixme requires a hook/query and appropriate adaptorId
const objectOptions = [];

export const DataTypeParameterDropdown = (props) => {

    const {property, value} = props;

    /* detect when the dataType changes and present new dataTypeParameter options */
    useEventId({ 
        property: property?.getParent()?.getProperty("dataType"), 
        eventId: "onChanged" 
    });
    
    let options;
    let selectedOption;

    const dataType = property?.getParent()?.getPropertyValue("dataType");    

    let selected = [];
    if (dataType === "string") {
        options = stringOptions;
        if (value)
            selected = options.filter(o => o.key === value);
    } else if (dataType === "base64Binary" || dataType === "hexBinary") {
        options = binaryOptions;
        if (value)
            selected = options.filter(o => o.key === value);
    } else if (dataType === "object") {
        options = objectOptions;                
    }

    if (selected.length > 0)
        selectedOption = selected[0];
    else if (value) {
        selectedOption = { key: value, text: value };            
        if (options)
            options = [ ...options, selectedOption ];
    }                        


    if (options) {        
        return (
            <Autocomplete 
                {...props}
                value={selectedOption}
                data-component-type="DataTypeParameterDropdown"
                options={options}                
                onChanged={option => props.onChanged(option.key)}          
                onCreateOption={props.onChanged}   
                noOptionsText="No Datatype"   
            />
        );
    } 

    else {
        return (
            <TextField 
                {...props} 
                data-component-type="DataTypeParameterDropdown"
            />
        );
    }
};

export default DataTypeParameterDropdown;
