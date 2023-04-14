// See the 'COPYING' file in the project root for licensing information.
import {useMemo} from "react";

import {    
    CodeEditor,    
    useOperational
} from "@afw/react";

import ModelObjectTypeOverview from "./Overview/ModelObjectTypeOverview";
import ModelSpreadsheet from "./Spreadsheet/ModelSpreadsheet";
import {PropertyTypeModelMapping} from "./Mappings/ModelMapping";
import ModelTree from "./Tree/ModelTree";

export const ModelObjectTypes = (props) => {

    const {editable} = useOperational(props);
    const {model, objectType, perspective} = props;    

    const source = useMemo(() => {
        if (model && objectType && perspective === "source") {
            let objectTypeObject;
            if (model.hasProperty(["objectTypes", objectType]))
                objectTypeObject = model.getPropertyValue(["objectTypes", objectType]);
    
            if (objectTypeObject) {                                    
                const source = JSON.stringify(objectTypeObject.toJSON(false), null, 4);                
    
                return source;                                 
            }
        }
    }, [model, objectType, perspective]);

    const schema = useMemo(() => {
        let objectTypeObject;
        if (model.hasProperty(["objectTypes", objectType]))
            objectTypeObject = model.getPropertyValue(["objectTypes", objectType]);

        if (objectTypeObject) {
            return objectTypeObject.getJSONSchema();            
        }
    }, [model, objectType]);


    if (!model || !objectType)
        return null;            

    let objectTypeObject;
    if (model.hasProperty(["objectTypes", objectType]))
        objectTypeObject = model.getPropertyValue(["objectTypes", objectType]);
    if (!objectTypeObject)
        return <div>Object Type {objectType} not found.</div>;

    const onSourceChanged = (src) => {
        try {
            // try to parse the JSON to ensure it's valid
            JSON.parse(src);

            // use it for the AfwObject
            objectTypeObject.fromJSON(src);
        } catch (error) {
            //eslint-disable-next-line
        }
    };
    
    switch (perspective) {

    case "tree":
        return <ModelTree {...props} />;

    case "spreadsheet":
        return <ModelSpreadsheet model={model} />;    

    case "mappings":
        return (
            <PropertyTypeModelMapping
                {...props}
                objectTypeObject={objectTypeObject}
                propertyTypes={objectTypeObject.getPropertyValues()}
            />
        );

    case "source":
        return (
            <CodeEditor 
                data-testid="admin-admin-models-objecttype-source"
                style={{ height: "100%" }}
                readOnly={!editable}
                source={source}
                schema={schema}
                showLineNumbers={true}
                onChanged={onSourceChanged}
            />
        );

    case "overview":
    default:        
        return (
            <ModelObjectTypeOverview
                {...props}
                objectTypeObject={objectTypeObject} 
            />
        );           
    }   

};


export default ModelObjectTypes;
