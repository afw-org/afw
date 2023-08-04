// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect, useMemo} from "react";

import {
    Checkbox,
    Skeleton,
    Tree,
    useRetrieveObjects,
} from "@afw/react";

import {useApplication, useTheme} from "../../hooks";


const objectOptions = { path: true, objectId: true, objectType: true };

/*
 * ModelObjectTypeSelector
 *
 * Given an adaptorId and, this provides a list of selectable
 * Object Type / Property Types to choose from,
 * presented as a Tree.
 * 
 */
export const ModelObjectTypeSelector = ({ adaptorId, onSelect }) => {

    const [selectedKeys, setSelectedKeys] = useState([]);

    const {        
        objects: objectTypeObjects = [],
        isLoading,
        error,
    } = useRetrieveObjects({
        adaptorId,
        objectTypeId: "_AdaptiveObjectType_",
        objectOptions,
    });

    const theme = useTheme();
    const {notification} = useApplication();
    
    useEffect(() => {
        if (notification && error) {
            notification({ type: "error", message: error });
        }
    }, [notification, error]);

    const onSelectObjectType = (objectTypeObject) => {            

        let newSelectedKeys = [];
        let key = objectTypeObject.getPath();

        const propertyTypes = objectTypeObject.getPropertyValue("propertyTypes");
        if (!selectedKeys.includes(key)) {            
            newSelectedKeys = [...selectedKeys, objectTypeObject.getPath()];

            /* also, select all property types of this object type */
            if (propertyTypes && propertyTypes.getProperties())
                propertyTypes.getProperties().forEach(property => {
                    if (!newSelectedKeys.includes(property.getPath()))
                        newSelectedKeys.push(property.getPath());
                });
            
        } else {
            newSelectedKeys = selectedKeys.filter(selectedKey => selectedKey !== key);

            /* also, unselect all property types of this object type */
            if (propertyTypes && propertyTypes.getProperties())
                propertyTypes.getProperties().forEach(property => {
                    if (newSelectedKeys.includes(property.getPath()))
                        newSelectedKeys = newSelectedKeys.filter(selectedKey => selectedKey !== property.getPath());
                });
        }

        setSelectedKeys(newSelectedKeys);
        
        onSelect(objectTypeObjects, newSelectedKeys);
    };

    const onSelectPropertyType = (objectTypeObject, propertyType) => {

        let newSelectedKeys = [];
        let key = propertyType.getPath();

        if (!selectedKeys.includes(key)) {
            if (!selectedKeys.includes(key))
                newSelectedKeys = [...selectedKeys, key];
        } else 
            newSelectedKeys = selectedKeys.filter(selectedKey => selectedKey !== key);

        setSelectedKeys(newSelectedKeys);

        onSelect(objectTypeObjects, newSelectedKeys);
    };

    const children = useMemo(() => {        

        return objectTypeObjects.map((objectTypeObject) => {
            const propertyTypes = objectTypeObject.getPropertyValue("propertyTypes");           
            return ({
                key: objectTypeObject.getPath(),   
                objectTypeObject,                                       
                contains:                                
                    <Checkbox  
                        style={{ marginLeft: theme.spacing(2) }}     
                        label={objectTypeObject.getObjectId()} 
                        value={selectedKeys.includes(objectTypeObject.getPath()) ? true : false}                              
                    />,
                children: 
                    (propertyTypes && propertyTypes.getProperties()) ?
                        propertyTypes.getProperties().map((propertyType) => {
                            return ({
                                key: propertyType.getPath(), 
                                objectTypeObject, 
                                propertyType,                                                  
                                contains:                                                
                                    <Checkbox    
                                        style={{ marginLeft: theme.spacing(2) }}        
                                        label={propertyType.getName()}                                                    
                                        value={selectedKeys.includes(propertyType.getPath())}                              
                                    />
                            });
                        }) : []
            });
        });
    }, [objectTypeObjects, selectedKeys, theme]);

    
    return (
        isLoading ?
            <Skeleton width="100%" height="100%" /> :            
            <Tree 
                onSelectTreeNode={(node) => {
                    const {objectTypeObject, propertyType} = node;
                    if (propertyType)
                        onSelectPropertyType(objectTypeObject, propertyType);
                    else 
                        onSelectObjectType(objectTypeObject);
                }}
                selectedKeys={selectedKeys}
                dense={true}
                children={children}
            />   
            
    );
    
};
