// See the 'COPYING' file in the project root for licensing information.
import {useState, useMemo} from "react";
import {useLocation, matchPath} from "react-router";

import {
    PropertyResponsive,
    Menu,
    useOperational
} from "@afw/react";

import {ModelPropertyTypeManualAdd} from "./Overview/ModelPropertyTypesTable";
import {ModelObjectTypeManualAdd} from "./Overview/ModelObjectTypesTable";

export const ModelEditorContextMenu = (props) => {
    
    const [showAddObjectType, setShowAddObjectType] = useState();
    const [showAddPropertyType, setShowAddPropertyType] = useState();
    const [showAddCustom, setShowAddCustom] = useState();

    const {editable} = useOperational(props);
    const {pathname} = useLocation();

    const {
        model,
        target, 
        onClose, 
        adaptors, 
        mappedAdaptorId,
        onSelectMappedAdaptorId,
        onDeploy,
        onRestartModelAdaptor,
        onEditable,
        onReloadView,
        onRefresh,
        onRoute,
        onSave,
    } = props;

    const {modelId, objectTypes} = model.getPropertyValues();
    const adaptorId = model.getAdaptorId();

    /* use the current path to determine which actions are appropriate */
    const {
        newOptions, 
        objectType, 
        propertyTypes, 
        propertyType, 
        custom, 
        currentLabel
    } = useMemo(() => {
        let newOptions = [];
        let objectType, propertyTypes, propertyType, custom, currentLabel;

        const newObjectTypeItem = {
            key: "ObjectType",
            label: "Object Type",
            onClick: () => setShowAddObjectType(true),
        };

        const newPropertyTypeItem = {
            key: "PropertyType",
            label: "Property Type",
            onClick: () => setShowAddPropertyType(true),
        };

        const newCustomItem = {
            key: "Custom",
            label: "Custom Variable",
            onClick: () => setShowAddCustom(true),
        };

        if (matchPath(pathname, { path: "/Admin/Models/:adaptorId/:modelId", exact: true })) 
        {
            propertyTypes = model.getPropertyValue("propertyTypes");
            custom = model.getPropertyValue([ "custom" ]);
            newOptions = [ newObjectTypeItem, newPropertyTypeItem, newCustomItem ];
        }

        else if (matchPath(pathname, { path: "/Admin/Models/:adaptorId/:modelId/objectTypes", exact: true })) 
        {
            newOptions = [ newObjectTypeItem ];
        }

        else if (matchPath(pathname, { path: "/Admin/Models/:adaptorId/:modelId/propertyTypes", exact: true })) 
        {
            propertyTypes = model.getPropertyValue("propertyTypes");
            newOptions = [ newPropertyTypeItem ];
        }

        else if (matchPath(pathname, { path: "/Admin/Models/:adaptorId/:modelId/propertyTypes/:propertyType", exact: true })) 
        {
            const match = matchPath(pathname, { path: "/Admin/Models/:adaptorId/:modelId/propertyTypes/:propertyType" });
            custom = model.getPropertyValue([ "propertyTypes", match.params.propertyType, "custom" ]);
            currentLabel = propertyType = match.params.propertyType;
            newOptions = [ newCustomItem ];
        }

        else if (matchPath(pathname, { path: "/Admin/Models/:adaptorId/:modelId/custom", exact: true })) 
        {
            custom = model.getPropertyValue([ "custom" ]);
            newOptions = [ newCustomItem ];
        }

        else if (
            matchPath(pathname, { path: "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType", exact: true }) 
        ) 
        {
            const match = matchPath(pathname, { path: "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType" });
            propertyTypes = model.getPropertyValue([ "objectTypes", match.params.objectType, "propertyTypes" ]);
            custom = model.getPropertyValue([ "objectTypes", match.params.objectType, "custom" ]);
            currentLabel = objectType = match.params.objectType;
            newOptions = [ newCustomItem ];
        }

        else if (
            matchPath(pathname, { path: "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/propertyTypes", exact: true }) 
        ) 
        {
            const match = matchPath(pathname, { path: "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType" });
            propertyTypes = model.getPropertyValue([ "objectTypes", match.params.objectType, "propertyTypes" ]);
            objectType = match.params.objectType;
            propertyType = match.params.propertyType;
            newOptions = [ newPropertyTypeItem ];
        }

        else if (matchPath(pathname, { path: "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/propertyTypes/:propertyType", exact: true })) 
        {
            const match = matchPath(pathname, { path: "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/propertyTypes/:propertyType" });
            propertyTypes = model.getPropertyValue([ "objectTypes", match.params.objectType, "propertyTypes" ]);
            custom = model.getPropertyValue([ "objectTypes", match.params.objectType, "propertyTypes", match.params.propertyType, "custom" ]);
            objectType = match.params.objectType;
            currentLabel = propertyType = match.params.propertyType;
            newOptions = [ newCustomItem ];
        }

        else if (matchPath(pathname, { path: "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/custom", exact: true })) 
        {
            const match = matchPath(pathname, { path: "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/custom" });
            propertyTypes = model.getPropertyValue([ "objectTypes", match.params.objectType, "propertyTypes" ]);
            custom = model.getPropertyValue([ "objectTypes", match.params.objectType, "propertyTypes", match.params.propertyType, "custom" ]);
            objectType = match.params.objectType;
            newOptions = [ newCustomItem ];
        }

        else if (matchPath(pathname, { path: "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/propertyTypes/:propertyType/custom", exact: true })) 
        {
            const match = matchPath(pathname, { path: "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/propertyTypes/:propertyType/custom" });
            propertyTypes = model.getPropertyValue([ "objectTypes", match.params.objectType, "propertyTypes" ]);
            custom = model.getPropertyValue([ "objectTypes", match.params.objectType, "propertyTypes", match.params.propertyType, "custom" ]);
            objectType = match.params.objectType;
            propertyType = match.params.propertyType;
            newOptions = [ newCustomItem ];
        }

        return {
            objectType,
            propertyTypes,
            propertyType,
            custom,
            currentLabel,
            newOptions
        };

    }, [pathname, model]);

    const mappedAdaptorOptions = useMemo(() => {
        let mappedAdaptorOptions = [
            {
                key: "None",
                label: "(None)",
                onClick: () => {
                    onSelectMappedAdaptorId();               
                },
                checked: (!mappedAdaptorId),
                canCheck: true
            },
            {
                key: "divider",
                type: "divider"
            }
        ];

        if (adaptors) {
            adaptors.sort((A, B) => 
                A.adaptorId.toLowerCase().localeCompare(B.adaptorId.toLowerCase())
            );

            adaptors.forEach(adaptor => {
                const adaptorId = adaptor.adaptorId;
                mappedAdaptorOptions.push({
                    key: adaptorId,
                    label: adaptorId,
                    onClick: () => onSelectMappedAdaptorId(adaptor),
                    checked: adaptorId === mappedAdaptorId,
                    canCheck: true,
                });
            });
        }

        return mappedAdaptorOptions; 
    }, [adaptors, mappedAdaptorId, onSelectMappedAdaptorId]);

    return (
        <>
            <Menu 
                aria-label="More Options Menu"
                open={Boolean(target)}
                onClose={onClose}
                target={target}
                items={[
                    {
                        key: "Edit",
                        label: "Edit Model",
                        icon: "edit",
                        disabled: editable,
                        onClick: () => onEditable(true)
                    },
                    {
                        key: "Save",
                        label: "Save Model",
                        icon: "save",
                        disabled: !editable,
                        onClick: () => onSave()
                    },
                    {
                        key: "Refresh",
                        label: "Refresh Model",
                        icon: "refresh",
                        onClick: () => onRefresh()
                    },
                    {
                        key: "divider",
                        type: "divider"
                    },
                    {
                        key: "New",
                        label: "New",
                        disabled: !editable,
                        subMenu: {
                            items: newOptions
                        }
                    },
                    {
                        key: "Rename",
                        label: "Rename" + (currentLabel ? (" \"" + currentLabel + "\"") : ""),
                        disabled: true,
                    },
                    {
                        key: "Delete",
                        label: "Delete" + (currentLabel ? (" \"" + currentLabel + "\"") : ""),
                        disabled: true,
                    },
                    {
                        key: "divider2",
                        type: "divider"
                    },
                    {
                        key: "RestartModelAdaptor",
                        label: "Restart Model Adaptor",
                        onClick: () => onRestartModelAdaptor()
                    },
                    {
                        key: "MappedAdaptor",
                        label: "Set Mapped Adaptor",
                        subMenu: {
                            items: mappedAdaptorOptions
                        }                                
                    },
                    {
                        key: "Simulate",
                        label: "Simulate",
                        subMenu: {
                            items: [
                                {
                                    key: "add",
                                    label: "Add Object",
                                    disabled: true,
                                },
                                {
                                    key: "modify",
                                    label: "Modify Object",
                                    disabled: true,
                                },
                                {
                                    key: "replace",
                                    label: "Replace Object",
                                    disabled: true,
                                },
                                {
                                    key: "delete",
                                    label: "Delete Object",
                                    disabled: true,
                                },
                                {
                                    key: "get",
                                    label: "Get Object",
                                    disabled: true,
                                },
                                {
                                    key: "retrieve",
                                    label: "Retrieve Object",
                                    disabled: true,
                                },
                            ]
                        }
                    },
                    {
                        key: "Service",
                        label: "Deploy to Service",     
                        onClick: () => onDeploy()
                    }
                ]}
            />   
            <ModelObjectTypeManualAdd 
                open={showAddObjectType}
                onDismiss={() => {
                    setShowAddObjectType(false);
                    onClose();
                }}
                onCreate={(objectTypeId, description) => {
                    const newObjectTypeObject = objectTypes.newProperty(objectTypeId);
                    newObjectTypeObject.setValue({ description });

                    setShowAddObjectType(false);
                    onReloadView();

                    onRoute("/Admin/Models/" + adaptorId + "/" + modelId + "/objectTypes/" + objectTypeId);
                }}
            />
            { (showAddPropertyType && propertyTypes) &&
                <ModelPropertyTypeManualAdd
                    open={showAddPropertyType}
                    onDismiss={() => {
                        setShowAddPropertyType(false);
                        onClose();
                    }}
                    onCreate={(propertyName, description) => {
                        const newPropertyTypeObject = propertyTypes.newProperty(propertyName);
                        newPropertyTypeObject.setValue({ description });

                        setShowAddPropertyType(false);
                        onReloadView();
                        
                        if (objectType)
                            onRoute("/Admin/Models/" + adaptorId + "/" + modelId + "/objectTypes/" + objectType + "/propertyTypes/" + propertyName);
                        else
                            onRoute("/Admin/Models/" + adaptorId + "/" + modelId + "/propertyTypes/" + propertyName);
                    }}
                />
            }
            {
                showAddCustom &&
                    <PropertyResponsive.Add 
                        open={showAddCustom}
                        title="New Custom Variable"
                        label="Variable Name"
                        description="Enter the name of the new Custom Variable"
                        object={custom}
                        onDismiss={() => {
                            setShowAddCustom(false);
                            onClose();
                        }}
                        onSave={(property, propertyName) => {
                            if (!custom) {
                                /* we didn't have a "custom" object defined yet, so make one */
                                let newCustom;
                                
                                if (objectType && propertyType)
                                    newCustom = model.getPropertyValue(["objectTypes", objectType, "propertyTypes", propertyType]).newProperty("custom");
                                else if (objectType)
                                    newCustom = model.getPropertyValue(["objectTypes", objectType]).newProperty("custom");
                                else 
                                    newCustom = model.newProperty("custom");
                                    
                                newCustom.setValue({});
                                newCustom.getValue().newProperty(propertyName);
                            }

                            setShowAddCustom(false);
                            onReloadView(false);

                            let uri = "/Admin/Models/" + adaptorId + "/" + modelId;
                            if (objectType)
                                uri += "/objectTypes/" + objectType;
                            if (propertyType)
                                uri += "/propertyTypes/" + propertyType;

                            onRoute(uri + "/custom/" + propertyName);
                        }}
                    />
            }
            
        </>
    );
};

export default ModelEditorContextMenu;
