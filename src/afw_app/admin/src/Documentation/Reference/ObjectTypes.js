// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";
import {Route, Switch, useRouteMatch} from "react-router";

import {
    Link,
    Message,
    Table,
    Tabs,
    Typography,
    Spinner
} from "@afw/react";

import {useRetrieveObjects} from "@afw/react";

import ObjectType from "./ObjectType";
import PropertyType from "./PropertyType";
import SchemaDiagram from "./SchemaDiagram";
import SchemaApi from "./SchemaApi";

import {useAppCore, useTheme} from "../../hooks";

const objectTypeId = "_AdaptiveObjectType_";
const objectOptions = { 
    objectId: true, 
    path: true,  
    composite: true, 
    resolvedParentPaths : true 
};

/**
 * ObjectTypes
 * 
 * This component is responsible for the "Object Types" section of the 
 * generated Reference.  It pulls the Object Type objects from an adaptor
 * and creates tables for Object Types and Property Types, which allows the 
 * user to explore the hierarchy, diagrams, data types, descriptions, and 
 * other constraints.
 */
export const ObjectTypes = () => {

    const theme = useTheme();
    const [objectTypesHash, setObjectTypesHash] = useState();
    const match = useRouteMatch();

    const {adaptorId} = match.params;
    const {adaptors} = useAppCore();

    const {objects: objectTypeObjects, isLoading} = useRetrieveObjects({ 
        adaptorId, 
        objectTypeId, 
        objectOptions
    });

    useEffect(() => {
        if (objectTypeObjects) {
            const objectTypesHash = Object.assign(
                ...objectTypeObjects.map(i => ({[i.getPath()]: i}))
            );

            setObjectTypesHash(objectTypesHash);
        }
    }, [objectTypeObjects]);
    
    if (isLoading) {
        return (
            <Spinner 
                size="large" 
                label="Loading Object Types..."
                fullScreen={true}
            />
        );
    }

    return (
        <Switch>
            <Route exact path="/Documentation/Reference/Schema" render={() => {
                return (
                    <>            
                        <Message 
                            message="Adaptors provide Object Type and Property Type definitions, used to create Adaptive Object instances from.  Select an adaptor
                            to view the Object Type references."
                        />
                        <div style={{ height: theme.spacing(4) }} />
                        <Table 
                            columns={[
                                {
                                    key: "Adaptor",
                                    name: "Adaptor",
                                    minWidth: 100,
                                    maxWidth: 200,
                                    isResizable: true,
                                    style: { wordWrap: "break-word" },
                                    onRender: (adaptor) => {
                                        const adaptorId = adaptor.adaptorId;
                                        return (
                                            <Link 
                                                url={"/Documentation/Reference/Schema/" + encodeURIComponent(adaptorId)}
                                                text={adaptorId}
                                            />
                                        );
                                    }
                                },
                                {
                                    key: "Description",
                                    name: "Description",  
                                    minWidth: 200,
                                    maxWidth: 400,
                                    isResizable: true,      
                                    isMultiline: true,            
                                    style: { wordWrap: "break-word" },                        
                                    onRender: (adaptor) => {
                                        return (adaptor.properties.description);
                                    }
                                }
                            ]}
                            rows={adaptors}
                            selectionMode="none"                                    
                        />
                    </>
                ); 
            }} />
            <Route exact path="/Documentation/Reference/Schema/:adaptorId" render={() => {

                if (!adaptorId || !objectTypeObjects || !objectTypesHash)
                    return null;
                
                return (
                    <Tabs 
                        style={{ height: "100%" }}
                        gapSpace={20}
                        tabs={[
                            {
                                text: "Reference",
                                contains: 
                                    <div>
                                        <Table 
                                            columns={[
                                                {
                                                    key: "ObjectType",
                                                    name: "Object Type",
                                                    fieldName: "objectType",
                                                    minWidth: 100,
                                                    maxWidth: 200,
                                                    isResizable: true,
                                                    style: { wordWrap: "break-word" },
                                                    onRender: (item) => {
                                                        return (
                                                            <Link 
                                                                url={"/Documentation/Reference/Schema/" + encodeURIComponent(adaptorId) + "/" + encodeURIComponent(item.objectType)} 
                                                                text={item.objectType} 
                                                            />
                                                        );
                                                    }
                                                },
                                                {
                                                    key: "Description",
                                                    name: "Description",
                                                    fieldName: "description",
                                                    minWidth: 200,
                                                    maxWidth: 400,
                                                    isResizable: true,
                                                    isMultiline: true,
                                                    style: { wordWrap: "break-word" },
                                                }
                                            ]}
                                            rows={
                                                objectTypeObjects.map((objectTypeObject, index) => {
                                                    return ({
                                                        key: index,
                                                        objectType: objectTypeObject.getObjectId(),
                                                        description: objectTypeObject.getPropertyValue("description"),
                                                    });
                                                }).sort((A, B) => {
                                                    return A.objectType.toLowerCase().localeCompare(B.objectType.toLowerCase());
                                                })
                                            }
                                            selectionMode="none"
                                            compact={true}
                                        />   
                                    </div>
                            },
                            {
                                text: "Diagram",
                                style: {height: "100%"},
                                contains: 
                                    <SchemaDiagram 
                                        adaptorId={adaptorId}
                                        objectTypeObjects={objectTypesHash}                                            
                                    />
                                    
                            },
                            {
                                text: "API",
                                contains:
                                    <SchemaApi 
                                        adaptorId={adaptorId}
                                        objectTypeObjects={objectTypesHash}
                                    />
                            }
                        ]}
                    />
                );
            }} />
            <Route exact path="/Documentation/Reference/Schema/:adaptorId/:objectType" render={(props) => {
                const objectType = decodeURIComponent(props.match.params["objectType"]);
                const adaptorId = decodeURIComponent(props.match.params["adaptorId"]);                 

                if (!adaptorId || !objectTypeObjects)
                    return null;

                for (const objectTypeObject of objectTypeObjects) {
                    if (objectTypeObject.getObjectId() === objectType) {
                        return (
                            <ObjectType 
                                adaptorId={adaptorId}
                                objectTypeObjects={objectTypeObjects}
                                objectTypeObject={objectTypeObject}
                            />
                        );
                    }
                }
                
                return <Typography text={"Object Type " + objectType + " not found."} />;

            }} />
            <Route exact path="/Documentation/Reference/Schema/:adaptorId/:objectType/:propertyName" render={(props) => {
                const objectType = decodeURIComponent(props.match.params.objectType);
                const adaptorId = decodeURIComponent(props.match.params.adaptorId);
                const propertyName = decodeURIComponent(props.match.params.propertyName);                   

                if (!adaptorId || !objectTypeObjects || !propertyName)
                    return null;

                for (let objectTypeObject of objectTypeObjects) {
                    if (objectTypeObject.getObjectId() === objectType) {
                        let propertyTypes = objectTypeObject.getPropertyValue("propertyTypes");
                        if (propertyTypes && propertyTypes.getPropertyValue(propertyName)) {
                            return (
                                <PropertyType 
                                    adaptorId={adaptorId}
                                    objectTypeObject={objectTypeObject}
                                    propertyName={propertyName}
                                    propertyType={propertyTypes.getPropertyValue(propertyName)}
                                />
                            );
                        }
                    }
                }

                return <Typography text={"Property " + propertyName + " not found."} />;
            }} />                

        </Switch>            
    );
};

export default ObjectTypes;
