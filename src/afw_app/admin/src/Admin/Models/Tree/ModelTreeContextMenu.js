// See the 'COPYING' file in the project root for licensing information.
import {useReducer, useEffect} from "react";
import {useHistory} from "react-router";

import {PropertyResponsive, Menu} from "@afw/react";
import {useOperational, useValues} from "@afw/react";

import {ModelPropertyTypeManualAdd} from "../Overview/ModelPropertyTypesTable";
import {ModelObjectTypeManualAdd} from "../Overview/ModelObjectTypesTable";

const treeHash = "#tree";

const initialState = {
    contextMenuItems:       [],
    showAddCustom:          false,
    showAddObjectType:      false,
    showAddPropertyType:    false,
    showDeleteItem:         false,
    showRenameItem:         false,
};

const reducer = (state, action) => {

    switch (action.type) {

    case "MENU_ITEMS":
        return {
            ...state,
            contextMenuItems: action.contextMenuItems,
            node: action.node,
        };

    case "NEW_CUSTOM":
        return {
            ...state,
            addItemObject: action.custom,
            showAddCustom: true,
        };

    case "NEW_PROPERTY_TYPE":
        return {
            ...state,
            addItemObject: action.propertyTypes,
            showAddPropertyType: true,
        };

    case "NEW_OBJECT_TYPE":
        return {
            ...state,
            addItemObject: action.objectTypes,
            showAddObjectType: true,
        };

    case "RENAME_PROPERTY":        
        return {
            ...state,
            showRenameItem: true,
            renameProperty: action.property,
        };

    case "DELETE_ITEM":
        return {
            ...state,
            deleteItemObject: action.object,
            deleteItemPropertyName: action.propertyName,
        };

    case "DELETE_DISMISS":
    case "DELETE":
    case "RENAME_DISMISS":
    case "RENAME":
    case "NEW_OBJECT_TYPE_DISMISS":
    case "NEW_OBJECT_TYPE_SAVE":
    case "NEW_PROPERTY_TYPE_DISMISS":
    case "NEW_PROPERTY_TYPE_SAVE":
    case "NEW_CUSTOM_DISMISS":
    case "NEW_CUSTOM_SAVE":

        return {
            ...state,
            addItemObject: undefined,
            deleteItemObject: undefined,
            deleteItemPropertyName: undefined,
            renameProperty: undefined,
            showAddCustom: false,
            showAddPropertyType: false,
            showAddObjectType: false,
            showDeleteItem: false,
            showRenameItem: false,            
        };
        
    default:
        return state;
    }
};

/**
 * ModelTreeContextMenu
 * 
 * This component handles all of the interaction of onContextMenu
 * (where the user right-clicks on a Tree Node).
 * 
 */
export const ModelTreeContextMenu = (props) => {

    const [state, dispatch] = useReducer(reducer, initialState);

    const {editable} = useOperational(props);
    const history = useHistory();

    const {contextMenuNode, contextMenuClientX, contextMenuClientY, onReloadTree, model, onClose} = props;
        
    const {custom, modelId, objectTypes, propertyTypes} = useValues(model);

    const adapterId = model.getAdapterId();
    const {
        addItemObject,
        deleteItemObject,        
        deleteItemPropertyName,
        renameItemProperty,
        contextMenuItems,        
        node,
        showAddCustom,
        showAddObjectType,
        showAddPropertyType,        
        showDeleteItem,
        showRenameItem,
    } = state;

    useEffect(() => {
        const node = contextMenuNode;        
        let contextMenuItems = [];
        
        if (model && contextMenuNode && editable) {

            if (node.type === "model")
            {
                contextMenuItems = [
                    {
                        key: "newObjectType",
                        label: "New Object Type",
                        icon: "add",
                        onClick: () => dispatch({ type: "NEW_OBJECT_TYPE", objectTypes }),
                        
                    },
                    {
                        key: "newPropertyType",
                        label: "New Property Type",
                        icon: "add",
                        onClick: () => dispatch({ type: "NEW_PROPERTY_TYPE", propertyTypes }),
                    },
                    {                        
                        key: "newCustom",
                        label: "New Custom Variable",
                        icon: "add",
                        onClick: () => dispatch({ type: "NEW_CUSTOM", custom }),
                    }    
                ];
            }

            if (node.type === "objectTypes") 
            {
                contextMenuItems = [
                    {
                        key: "new",
                        label: "New Object Type",
                        icon: "add",
                        onClick: () => dispatch({ type: "NEW_OBJECT_TYPE", objectTypes }),
                    }    
                ];
            }

            else if (node.type === "propertyTypes") 
            {
                contextMenuItems = [
                    {
                        key: "new",
                        label: "New Property Type",
                        icon: "add",
                        onClick: () => {                            
                            let propertyTypes = model.getPropertyValue(["objectTypes", node.objectType, "propertyTypes"]);                            
                            if (!propertyTypes) {
                                model.getPropertyValue(["objectTypes", node.objectType]).setPropertyValue("propertyTypes", {});                       
                                propertyTypes = model.getPropertyValue(["objectTypes", node.objectType, "propertyTypes"]);                                     
                            }
                            dispatch({ type: "NEW_PROPERTY_TYPE", propertyTypes });
                        }
                    }    
                ];
            }     

            else if (node.type === "objectType") 
            {
                contextMenuItems = [
                    {
                        key: "newPropertyType",
                        label: "New Property Type",   
                        icon: "add",                 
                        onClick: () => {
                            let propertyTypes = model.getPropertyValue(["objectTypes", node.objectType, "propertyTypes"]);     
                            if (!propertyTypes) {
                                model.getPropertyValue(["objectTypes", node.objectType]).setPropertyValue("propertyTypes", {});                       
                                propertyTypes = model.getPropertyValue(["objectTypes", node.objectType, "propertyTypes"]);                                     
                            }                       
                            dispatch({ type: "NEW_PROPERTY_TYPE", propertyTypes });
                        }
                    },  
                    {
                        key: "newCustom",
                        label: "New Custom Variable",
                        icon: "add",
                        onClick: () => {
                            const custom = model.getPropertyValue(["objectTypes", node.objectType, "custom"]);                            
                            dispatch({ type: "NEW_CUSTOM", custom });
                        }
                    },  
                    {
                        key: "newMethod",
                        label: "New Method",
                        icon: "add",
                        disabled: true,
                        onClick: () => {                            
                            //const propertyTypes = model.getPropertyValue(["objectTypes", node.objectType, "propertyTypes"]);                            
                            //dispatch({ type: "NEW_METHOD", propertyTypes });
                        }
                    },  
                    {
                        key: "divider",
                        type: "divider"
                    },
                    {
                        key: "rename",
                        label: "Rename",
                        icon: "edit",
                        onClick: () => {                            
                            const property = model.getProperty(["objectTypes", node.objectType]);
                            dispatch({ type: "RENAME_PROPERTY", property });
                        }
                    },
                    {
                        key: "delete",
                        label: "Delete",
                        icon: "delete",
                        onClick: () => dispatch({ type: "DELETE_ITEM", object: objectTypes, propertyName: node.objectType }),
                    }
                ];
            }

            else if (node.type === "propertyType") 
            {
                contextMenuItems = [                
                    {
                        key: "newCustom",
                        label: "New Custom Variable",
                        icon: "add",
                        onClick: () => {
                            let custom = model.getPropertyValue(["objectTypes", node.objectType, "propertyTypes", node.propertyType, "custom"]);
                            dispatch({ type: "NEW_CUSTOM", custom });
                        }
                    },                  
                    {
                        key: "divider",
                        type: "divider"
                    },
                    {
                        key: "rename",
                        label: "Rename",
                        icon: "edit",
                        onClick: () => {                            
                            const property = model.getProperty(["objectTypes", node.objectType, "propertyTypes", node.propertyType]);                            
                            dispatch({ type: "RENAME_PROPERTY", property });
                        }
                    },
                    {
                        key: "delete",
                        label: "Delete",
                        icon: "delete",
                        onClick: () => {
                            const object = model.getPropertyValue(["objectTypes", node.objectType, "propertyTypes"]);
                            dispatch({ type: "DELETE_ITEM", object, propertyName: node.propertyType });
                        }
                    }
                ];
            }

            else if (node.type === "customs") 
            {
                contextMenuItems = [
                    {
                        key: "new",
                        label: "New Variable",
                        icon: "add",
                        onClick: () => {
                            const custom = model.getPropertyValue(["custom"]);         
                            dispatch({ type: "NEW_CUSTOM", custom });
                        }
                    }    
                ];
            }

            else if (node.propertyName) 
            {
                contextMenuItems = [
                    {
                        key: "rename",
                        label: "Rename",
                        icon: "edit",
                        onClick: () => {
                            /*! \fixme */
                        }
                    },
                    {
                        key: "delete",
                        label: "Delete",
                        icon: "delete",
                        onClick: () => {
                            /*! \fixme */
                        }
                    },
                ];
            }
        } 

        dispatch({ type: "MENU_ITEMS", contextMenuItems, node });        

    }, [custom, objectTypes, propertyTypes, contextMenuNode, model, editable]);

    return (
        <>
            { (contextMenuNode) &&
                <Menu
                    open={Boolean(contextMenuNode)}
                    anchorPosition={{ left: contextMenuClientX, top: contextMenuClientY }}
                    onClose={onClose}
                    items={contextMenuItems}
                />
            }
            { showAddCustom &&
                <PropertyResponsive.Add 
                    open={showAddCustom}
                    title="New Custom Variable"
                    label="Variable Name"
                    description="Enter the name of the new Custom Variable"
                    object={addItemObject}
                    onDismiss={() => {
                        dispatch({ type: "NEW_CUSTOM_DISMISS" });
                        onClose();
                    }}
                    onSave={(property, propertyName) => {
                        if (!addItemObject) {
                            /* we didn't have a "custom" object defined yet, so make one */
                            let custom;
                            if (node.objectType && node.propertyType)
                                custom = model.getPropertyValue(["objectTypes", node.objectType, "propertyTypes", node.propertyType]).newProperty("custom");
                            else if (node.objectType)
                                custom = model.getPropertyValue(["objectTypes", node.objectType]).newProperty("custom");
                            else 
                                custom = model.newProperty("custom");
                                
                            custom.setValue({});
                            custom.getValue().newProperty(propertyName);
                        }

                        dispatch({ type: "NEW_CUSTOM_SAVE" });
                        onReloadTree();

                        let uri = "/Admin/Models/" + adapterId + "/" + modelId;
                        if (node.objectType)
                            uri += "/objectTypes/" + node.objectType;
                        if (node.propertyType)
                            uri += "/propertyTypes/" + node.propertyType;

                        history.push(uri + "/custom/" + propertyName + treeHash);
                    }}
                />
            }
            {
                showAddObjectType &&
                    <ModelObjectTypeManualAdd 
                        open={showAddObjectType}
                        onDismiss={() => {
                            dispatch({ type: "NEW_OBJECT_TYPE_DISMISS" });
                            onClose();
                        }}
                        onCreate={(objectTypeId, description) => {
                            const newObjectTypeObject = addItemObject.newProperty(objectTypeId);
                            newObjectTypeObject.setValue({ description });

                            dispatch({ type: "NEW_OBJECT_TYPE_SAVE" });
                            onReloadTree();

                            let uri = "/Admin/Models/" + adapterId + "/" + modelId;

                            history.push(uri + "/objectTypes/" + objectTypeId + treeHash);
                        }}
                    />
            }
            {
                showAddPropertyType &&
                    <ModelPropertyTypeManualAdd
                        open={showAddPropertyType}
                        onDismiss={() => {
                            dispatch({ type: "NEW_PROPERTY_TYPE_DISMISS" });
                            onClose();
                        }}
                        onCreate={(propertyName, description) => {
                            const newPropertyTypeObject = addItemObject.newProperty(propertyName);
                            newPropertyTypeObject.setValue({ description });

                            dispatch({ type: "NEW_PROPERTY_TYPE_SAVE" });
                            onReloadTree();

                            let uri = "/Admin/Models/" + adapterId + "/" + modelId;
                            if (node.objectType)
                                uri += "/objectTypes/" + node.objectType;

                            history.push(uri + "/propertyTypes/" + propertyName + treeHash);
                        }}
                    />
            }            
            {
                showDeleteItem &&
                    <PropertyResponsive.Delete 
                        object={deleteItemObject}
                        propertyName={deleteItemPropertyName}
                        onDismiss={() => {
                            dispatch({ type: "DELETE_DISMISS" });
                            onClose();
                        }}
                        onDelete={() => {
                            dispatch({ type: "DELETE" });
                            onReloadTree();
                        }}
                    />
            }
            {
                showRenameItem &&
                    <PropertyResponsive.Rename 
                        property={renameItemProperty}
                        onDismiss={() => {
                            dispatch({ type: "RENAME_DISMISS" });
                            onClose();
                        }}
                        onRename={() => {
                            dispatch({ type: "RENAME" });
                            onReloadTree();
                        }}
                    />
            }
        </>
    );
};

export default ModelTreeContextMenu;
