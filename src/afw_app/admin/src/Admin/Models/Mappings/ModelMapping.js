// See the 'COPYING' file in the project root for licensing information.
import {useEffect, useState} from "react";

import {
    Dropdown,
    Responsive,
    Spinner,
    TextField,
    Tree,
    Typography,    
    useOperational,
    useRetrieveObjects,
    useEventId,
} from "@afw/react";

import {useApplication, useTheme} from "../../../hooks";


const PropertyTypeModelMapName = ({ editable, propertyTypes, propertyType }) => {

    const [name, setName] = useState(propertyType);
    const [error, setError] = useState();

    if (editable)
        return (
            <TextField 
                value={name}
                onChanged={newPropertyName => {                                                                                                      
                    setName(newPropertyName);
                    if (newPropertyName !== propertyType && propertyTypes.hasProperty(newPropertyName))
                        setError("Property Type already exists.");
                    else setError();
                }}
                onBlur={() => {
                    if (name !== propertyType)
                        propertyTypes.getProperty(propertyType).setName(name);
                }}
                onEnter={() => {
                    if (name !== propertyType)
                        propertyTypes.getProperty(propertyType).setName(name);
                }}
                error={error ? true : false}
                description={error}
            />
                                
        );
    else return (
        <Typography text={name} />
    );
};

const PropertyTypeModelMapValue = ({ editable, propertyTypes, propertyType }) => {

    if (editable)
        return (        
            <TextField 
                value={propertyTypes.getPropertyValue([propertyType, "mappedPropertyName"])}
                placeholder={propertyType}
                onChanged={(mappedPropertyName) => {                                        
                    const mappedProperty = propertyTypes.getPropertyValue(propertyType);
                    mappedProperty.setPropertyValue("mappedPropertyName", mappedPropertyName);
                }}
            />
        );
    else
        return (
            <Typography 
                color={
                    propertyTypes.getPropertyValue([propertyType, "mappedPropertyName"]) ?
                        undefined : "textSecondary"
                }
                text={
                    propertyTypes.getPropertyValue([propertyType, "mappedPropertyName"]) ?
                        propertyTypes.getPropertyValue([propertyType, "mappedPropertyName"]) :
                        propertyType
                } 
            />
        );

};

/**
 * PropertyTypeModelMapping
 * 
 * Provides a UI for rendering the mapping between Model Properties
 * and the Mapped Properties, using a table to arrange Model and Mapped
 * in separate columns.  If Property Types are available from the Mapped
 * Adapter, then we can use Dropdowns instead of TextField's.
 */
export const PropertyTypeModelMapping = (props) => {
            
    const {editable} = useOperational(props);    
    const {propertyTypes} = props;

    useEventId({ object: propertyTypes, eventId: "onChildChanged" });

    const sortedPropertyNames = propertyTypes ? propertyTypes.getProperties().map(property => property.getName()).sort((A, B) => {
        return (A.toLowerCase().localeCompare(B.toLowerCase()));
    }) : [];

    return (
        sortedPropertyNames.map((propertyType) => 
            <Responsive 
                key={propertyType}
                spacing={1}
                items={[                    
                    {
                        breakpoints: {
                            small: 6,
                            medium: 4,
                            xl: 3
                        },
                        contains:
                            <PropertyTypeModelMapName 
                                editable={editable}
                                propertyTypes={propertyTypes}
                                propertyType={propertyType}
                            /> 
                    },
                    {
                        breakpoints: {
                            small: 6,
                            medium: 4,
                            xl: 3
                        },
                        contains: 
                            <PropertyTypeModelMapValue 
                                editable={editable}
                                propertyTypes={propertyTypes}
                                propertyType={propertyType}
                            />
                    }  
                ]}
            />
        )
    );
};

const ObjectTypeModelMapName = ({ editable, modelObjectTypeObjects, objectType }) => {

    const [name, setName] = useState(objectType);
    const [error, setError] = useState();

    const theme = useTheme();

    if (editable)
        return (        
            <TextField        
                id={"models-mapping-model-" + objectType}                                                                                                                                 
                data-testid={"models-mapping-model-" + objectType}                                                                                                                              
                style={{ paddingRight: theme.spacing(1) }}
                value={name} 
                onChanged={(newObjectType) => {
                    setName(newObjectType);
                    if (newObjectType !== objectType && modelObjectTypeObjects.hasProperty(newObjectType))
                        setError("Object Type already exists.");
                    else setError();
                }}
                onBlur={() => {
                    if (name !== objectType)
                        modelObjectTypeObjects.getProperty(objectType).setName(name);
                }}
                onEnter={() => {
                    if (name !== objectType)
                        modelObjectTypeObjects.getProperty(objectType).setName(name);
                }}
                error={error ? true : false}
                description={error}
            />            
        );
    else 
        return (
            <Typography text={name} size="6" />
        );
};

const ObjectTypeModelMapValue = ({ editable, modelObjectTypeObjects, objectType }) => {

    if (editable)
        return (
            <TextField   
                id={"models-mapping-to-" + objectType}
                data-testid={"models-mapping-to-" + objectType}
                value={modelObjectTypeObjects.getPropertyValue([objectType, "mappedObjectType"])}                                            
                placeholder={objectType}
                onChanged={mappedObjectType => {
                    const modelObjectTypeObject = modelObjectTypeObjects.getPropertyValue(objectType);
                    modelObjectTypeObject.setPropertyValue("mappedObjectType", mappedObjectType);                                                                                
                }}
                disabled={editable === false}
            />
        );
    else 
        return (
            <Typography
                size="6"
                color={
                    modelObjectTypeObjects.getPropertyValue([objectType, "mappedObjectType"]) ?
                        undefined : "textSecondary"
                }
                text={
                    modelObjectTypeObjects.getPropertyValue([objectType, "mappedObjectType"]) ?
                        modelObjectTypeObjects.getPropertyValue([objectType, "mappedObjectType"]) :
                        objectType
                }
            />
        );
};

const objectOptions = { 
    objectId: true, 
    objectType: true 
};

const modelOptions = { 
    loadObjectTypes: false,  
};

/**
 * ModelMapping
 * 
 * This component provides a UI for mapping both Model Object Types
 * and Model Property Types to Mapped Object and Property Types by
 * displaying tables nested in trees that the user can collapse and 
 * dig into each Object Type.
 */
export const ModelMapping = (props) => {

    const {mappedAdapterId, modelObjectTypeObjects} = props;
    const [expandedKeys, setExpandedKeys] = useState([]);
    
    const {editable} = useOperational(props);
    useEventId({ object: modelObjectTypeObjects, eventId: "onChildChanged" });

    const theme = useTheme();
    const {notification} = useApplication();        

    const {
        objects: mappedObjectTypeObjects = [],
        isLoading,
        error,
    } = useRetrieveObjects({
        adapterId: mappedAdapterId,
        objectTypeId: "_AdaptiveObjectType_",
        objectOptions, 
        modelOptions,
    });

    const mappedObjectTypesDropdownOptions = mappedObjectTypeObjects.map(
        mappedObjectTypeObject => ({
            key: mappedObjectTypeObject?.getObjectId(),
            text: mappedObjectTypeObject?.getObjectId(),
            mappedObjectTypeObject,
        })
    );

    useEffect(() => {
        if (notification && error)
            notification({ type: "error", message: error });
    }, [notification, error]);

    const buildPropertyMappings = (objectType, modelObjectTypeObject, propertyTypes) => {

        let mappedObjectTypeObject;        
        mappedObjectTypeObjects.forEach(m => {
            let mappedObjectType = modelObjectTypeObject.getPropertyValue("mappedObjectType");
            if (!mappedObjectType)
                mappedObjectType = objectType;

            if (m?.getObjectId() === mappedObjectType)
                mappedObjectTypeObject = m;                
        });

        return ({
            key: objectType + "-PropertyMappingTable",
            contains: (
                <div key={objectType} style={{ padding: theme.spacing(1) }}>
                    <PropertyTypeModelMapping 
                        {...props}               
                        modelObjectTypeObject={modelObjectTypeObject}
                        mappedObjectTypeObject={mappedObjectTypeObject}
                        propertyTypes={propertyTypes} 
                    />
                </div>
            )
        });
    };
        
    const modelObjectTypes = modelObjectTypeObjects ?
        modelObjectTypeObjects.getProperties()
            .map(property => property.getName())
            .sort((a, b) => {
                return a.toLowerCase().localeCompare(b.toLowerCase());
            }) : [];    

    return (
        <div data-testid="admin-admin-models-mappings" style={{ padding: theme.spacing(0.5) }}>
            { isLoading && 
                <Spinner 
                    fullScreen={true}
                    size="large"
                    label="Loading Object Types..."
                />
            }
            <div style={{ padding: theme.spacing(2) }}>
                <Responsive 
                    items={[
                        {
                            breakpoints: {
                                small: 6,
                                medium: 4,
                                xl: 3
                            },
                            contains: 
                                <Typography                                         
                                    text="Model Object Type"
                                    size="6"
                                />
                        },
                        {
                            breakpoints: {
                                small: 6,
                                medium: 4,                                    
                                xl: 3
                            },
                            contains: 
                                <Typography                                         
                                    text="Maps To/From"
                                    size="6"
                                />
                        }
                    ]}
                />
            </div>
            <div style={{ height: theme.spacing(2) }} />
            <div>
                {
                    modelObjectTypes.map((objectType) => {                    
                        const modelObjectTypeObject = modelObjectTypeObjects.getPropertyValue(objectType);
                        const propertyTypes = modelObjectTypeObject.getPropertyValue("propertyTypes");

                        return (
                            <div key={objectType}>
                                <Tree 
                                    selectionMode="none"
                                    dense={true}
                                    expandedKeys={expandedKeys}
                                    onNodeToggle={setExpandedKeys}
                                    children={[
                                        {
                                            key: objectType,
                                            contains: 
                                                <Responsive 
                                                    key={objectType}
                                                    items={[
                                                        {
                                                            breakpoints: {
                                                                small: 6,
                                                                medium: 4,                                                                
                                                                xl: 3
                                                            },
                                                            contains: 
                                                                <ObjectTypeModelMapName 
                                                                    editable={editable}
                                                                    modelObjectTypeObjects={modelObjectTypeObjects}
                                                                    objectType={objectType}
                                                                />
                                                        },
                                                        {
                                                            breakpoints: {
                                                                small: 6,
                                                                medium: 4,                                                                
                                                                xl: 3
                                                            },
                                                            contains: 
                                                                mappedAdapterId ? 
                                                                    <Dropdown 
                                                                        id={"models-mapping-to-" + objectType + "-dropdown"}
                                                                        options={mappedObjectTypesDropdownOptions}
                                                                        selectedKey={modelObjectTypeObjects.getPropertyValue([objectType, "mappedObjectType"])}
                                                                        onChanged={mappedObjectType => {
                                                                            modelObjectTypeObjects.getProperty([objectType, "mappedObjectType"]).setValue(mappedObjectType.key);                                                
                                                                        }}
                                                                        disabled={editable === false}
                                                                    /> :
                                                                    <ObjectTypeModelMapValue 
                                                                        editable={editable}
                                                                        modelObjectTypeObjects={modelObjectTypeObjects}
                                                                        objectType={objectType}
                                                                    />
                                                        }
                                                    ]}
                                                />,    
                                            modelObjectTypeObject: modelObjectTypeObject,
                                            children: [ buildPropertyMappings(objectType, modelObjectTypeObject, propertyTypes) ]                                         
                                        }
                                    ]}  
                                />
                            </div>
                        );
                    })
                }
            </div>
        </div>
    );
};

export default ModelMapping;
