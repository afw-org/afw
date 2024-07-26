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
 * generated Reference.  It pulls the Object Type objects from an adapter
 * and creates tables for Object Types and Property Types, which allows the 
 * user to explore the hierarchy, diagrams, data types, descriptions, and 
 * other constraints.
 */
export const ObjectTypes = () => {

    const theme = useTheme();
    const [objectTypesHash, setObjectTypesHash] = useState();
    const match = useRouteMatch();

    const {adapterId} = match.params;
    const {adapters} = useAppCore();

    const {objects: objectTypeObjects, isLoading} = useRetrieveObjects({ 
        adapterId, 
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
                            message="Adapters provide Object Type and Property Type definitions, used to create Adaptive Object instances from.  Select an adapter
                            to view the Object Type references."
                        />
                        <div style={{ height: theme.spacing(4) }} />
                        <Table 
                            columns={[
                                {
                                    key: "Adapter",
                                    name: "Adapter",
                                    minWidth: 100,
                                    maxWidth: 200,
                                    isResizable: true,
                                    style: { wordWrap: "break-word" },
                                    onRender: (adapter) => {
                                        const adapterId = adapter.adapterId;
                                        return (
                                            <Link 
                                                url={"/Documentation/Reference/Schema/" + encodeURIComponent(adapterId)}
                                                text={adapterId}
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
                                    onRender: (adapter) => {
                                        return (adapter.properties.description);
                                    }
                                }
                            ]}
                            rows={adapters}
                            selectionMode="none"                                    
                        />
                    </>
                ); 
            }} />
            <Route exact path="/Documentation/Reference/Schema/:adapterId" render={() => {

                if (!adapterId || !objectTypeObjects || !objectTypesHash)
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
                                                                url={"/Documentation/Reference/Schema/" + encodeURIComponent(adapterId) + "/" + encodeURIComponent(item.objectType)} 
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
                                        adapterId={adapterId}
                                        objectTypeObjects={objectTypesHash}                                            
                                    />
                                    
                            },
                            {
                                text: "API",
                                contains:
                                    <SchemaApi 
                                        adapterId={adapterId}
                                        objectTypeObjects={objectTypesHash}
                                    />
                            }
                        ]}
                    />
                );
            }} />
            <Route exact path="/Documentation/Reference/Schema/:adapterId/:objectType" render={(props) => {
                const objectType = decodeURIComponent(props.match.params["objectType"]);
                const adapterId = decodeURIComponent(props.match.params["adapterId"]);                 

                if (!adapterId || !objectTypeObjects)
                    return null;

                for (const objectTypeObject of objectTypeObjects) {
                    if (objectTypeObject.getObjectId() === objectType) {
                        return (
                            <ObjectType 
                                adapterId={adapterId}
                                objectTypeObjects={objectTypeObjects}
                                objectTypeObject={objectTypeObject}
                            />
                        );
                    }
                }
                
                return <Typography text={"Object Type " + objectType + " not found."} />;

            }} />
            <Route exact path="/Documentation/Reference/Schema/:adapterId/:objectType/:propertyName" render={(props) => {
                const objectType = decodeURIComponent(props.match.params.objectType);
                const adapterId = decodeURIComponent(props.match.params.adapterId);
                const propertyName = decodeURIComponent(props.match.params.propertyName);                   

                if (!adapterId || !objectTypeObjects || !propertyName)
                    return null;

                for (let objectTypeObject of objectTypeObjects) {
                    if (objectTypeObject.getObjectId() === objectType) {
                        let propertyTypes = objectTypeObject.getPropertyValue("propertyTypes");
                        if (propertyTypes && propertyTypes.getPropertyValue(propertyName)) {
                            return (
                                <PropertyType 
                                    adapterId={adapterId}
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
