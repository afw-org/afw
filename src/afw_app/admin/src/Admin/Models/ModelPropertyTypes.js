// See the 'COPYING' file in the project root for licensing information.
import {useMemo} from "react";

import {
    CodeEditor,    
    useOperational
} from "@afw/react";

import ModelPropertyTypeOverview from "./Overview/ModelPropertyTypeOverview";
import {PropertyTypeModelMapping} from "./Mappings/ModelMapping";
import ModelTree from "./Tree/ModelTree";
import ModelSpreadsheet from "./Spreadsheet/ModelSpreadsheet";

export const ModelPropertyTypes = (props) => {

    const {model, objectType, propertyType, perspective, update} = props;
    const {editable} = useOperational(props);
    
    const source = useMemo(() => {
        if (model && objectType && propertyType && (perspective === "source")) {
            let propertyTypeObject;
            if (model.hasProperty(["objectTypes", objectType, "propertyTypes", propertyType]))
                propertyTypeObject = model.getPropertyValue(["objectTypes", objectType, "propertyTypes", propertyType]);
    
            if (propertyTypeObject && (update || !update)) {                    
                return JSON.stringify(propertyTypeObject.toJSON(false), null, 4);                
            }
        }
    }, [model, perspective, objectType, propertyType, update]);

    
    const schema = useMemo(() => {
        let propertyTypeObject;
        if (model.hasProperty(["objectTypes", objectType, "propertyTypes", propertyType]))
            propertyTypeObject = model.getPropertyValue(["objectTypes", objectType, "propertyTypes", propertyType]);

        if (propertyTypeObject) {
            return propertyTypeObject.getJSONSchema();            
        }

    }, [model, objectType, propertyType]);

        
    if (!model || !objectType || !propertyType)
        return null;

    let propertyTypeObject;
    if (model.hasProperty(["objectTypes", objectType, "propertyTypes", propertyType]))
        propertyTypeObject = model.getPropertyValue(["objectTypes", objectType, "propertyTypes", propertyType]);    
        
    if (!propertyTypeObject)
        return <div>Property Type {propertyType} not found.</div>;

    const onSourceChanged = (src) => {
        try {
            // try to parse the JSON to ensure it's valid
            JSON.parse(src);

            // use it for the AfwObject
            propertyTypeObject.fromJSON(src);
        } catch (error) {
            //eslint-disable-next-line
        }
    };

    switch (perspective) {

    case "mappings":
        return (
            <PropertyTypeModelMapping
                {...props}
                propertyTypeObject={propertyTypeObject} 
            />
        );

    case "source":
        return (
            <CodeEditor 
                style={{ height: "100%" }}
                readOnly={!editable}
                source={source}
                schema={schema}
                showLineNumbers={true}
                onChanged={onSourceChanged}
            />
        );

    case "tree":
        return <ModelTree {...props} />;  

    case "spreadsheet":    
        return <ModelSpreadsheet model={model} />;    

    case "overview":
    default:
        return (
            <ModelPropertyTypeOverview 
                {...props}
                propertyTypeObject={propertyTypeObject}
            />
        );
    }
    
};

export default ModelPropertyTypes;
