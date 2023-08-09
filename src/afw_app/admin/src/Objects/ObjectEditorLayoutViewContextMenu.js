// See the 'COPYING' file in the project root for licensing information.
import {useCallback, useState, useEffect, useMemo} from "react";

import {
    Button,
    Checkbox,
    Dialog,
    Menu,
    Responsive,
    useObjectOptions,
    useOperational,
    useEventId,
} from "@afw/react";

import {useTheme} from "../hooks";

const internalObjectOptions = [ 
    "objectId", 
    "objectType", 
    "path", 
    "reconcilable" 
];

/**
 * ObjectOptionsDialog
 * 
 * This dialog box opens up when the user clicks "Object Options" under the main menu.
 * 
 * It provides column(s) of checkboxes, one for each option that can be changed on the
 * object view options.
 */
const ObjectOptionsDialog = ({ open, onDismiss, onApply, options }) => {

    const theme = useTheme();
    const {objectOptions} = useObjectOptions();
    const [newOptions, setNewOptions] = useState(options);    

    const sortedObjectOptions = useMemo(() => {
        if (objectOptions) {
            return Object.entries(objectOptions.propertyTypes).map(([name, { label, brief, description }]) => ({
                name, label, brief, description
            })).sort((option1, option2) => {
                if (internalObjectOptions.includes(option1.name))
                    return -1;
                else if (internalObjectOptions.includes(option2.name))
                    return 1;
                else return option1.label.toLowerCase().localeCompare(option2.label.toLowerCase());
            });
        } else return [];
    }, [objectOptions]);

    return (
        <Dialog
            open={open}
            isBlocking={false}
            onDismiss={onDismiss}
            maxWidth="md"
            title="Select Object Options"
            showClose={true}
            contains={
                <Responsive.Row
                    columns={
                        sortedObjectOptions.map(option => ({
                            key: option.name,
                            breakpoints: {
                                small: 12,
                                large: 6,
                            },
                            contains: 
                                <Checkbox      
                                    label={option.label}
                                    description={option.brief}
                                    value={newOptions[option.name]}
                                    onChanged={value => {
                                        setNewOptions(opts => ({ ...opts, [option.name]: value }));
                                    }}
                                    disabled={internalObjectOptions.includes(option.name)}
                                />
                        }))                           
                    }
                />
            }
            footer={
                <div style={{ display: "flex", justifyContent: "flex-end", padding: theme.spacing(2), width: "100%" }}>
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button
                            label="Apply"
                            color="primary"
                            variant="contained"
                            onClick={() => onApply(newOptions)}
                        />
                    </div>
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button
                            label="Cancel"
                            onClick={() => {
                                setNewOptions(options);
                                onDismiss();
                            }}
                        />
                    </div>
                </div>
            }
        />
    );
};



/*
 * LayoutViewContextualMenu
 *
 * This represents the menu at the top of an ObjectEditorLayout.  It provides submenu
 * options to switch between layouts and edit current layout options.
 */
export const LayoutViewContextMenu = (props) => {

    const [layoutsMenuTarget, setLayoutsMenuTarget] = useState();
    const [layoutOptionsMenu, setLayoutOptionsMenu] = useState([]);
    const [objectOptionsDialog, setObjectOptionsDialog] = useState(false);

    const {
        layoutParameters, onChanged, onLayoutParamsAdvanced,
        onSave, onReconcile, onReplace, onDiscard, onReview,
        objectOptions, setObjectOptions, onRefresh, object
    } = props;

    const {editable} = useOperational();
    const savable = useEventId({ object, eventId: "onSavable" });    
       
    const buildMenu = useCallback((layoutParameters) => {

        const onClick = (event) => {
            event.stopPropagation();
            setLayoutsMenuTarget();
            onChanged(layoutParameters);
            setLayoutOptionsMenu(buildMenu(layoutParameters));
        };

        let items = [];
        if (layoutParameters) {
            layoutParameters.getProperties().forEach((property, index) => {       
                
                const label = property.getLabel() || property.getName();
                
                /* the object is being handled by the editor */
                if (property.getName() === "object")
                    return;

                if (property.getDataType() === "boolean") 
                {
                    const checked = (
                        (property.getValue() !== undefined) ? property.getValue() :
                            (property.getDefaultValue() ? property.getDefaultValue() : false)
                    );
                    items.unshift({
                        key: index,
                        canCheck: true,
                        label,
                        checked,
                        onClick: (event) => {
                            property.setValue(!checked);
                            onClick(event);
                        }
                    });
                }                 

                else if (property.getAllowedValues()) 
                {
                    const allowedValues = property.getAllowedValues();
                    let subItems = [];
                    allowedValues.forEach((allowedValue, pvIndex) => {
                        subItems.push({
                            key: pvIndex,
                            label: allowedValue,
                            canCheck: true,
                            checked: (property.getValue() === allowedValue),
                            onClick: (event) => {
                                if (property.getValue() === allowedValue || property.getDefaultValue() === allowedValue)
                                    property.setValue();
                                else
                                    property.setValue(allowedValue);
                                onClick(event);
                            }
                        });
                    });
                    items.push({
                        key: index,
                        label,
                        subMenu: {
                            items: subItems
                        }
                    });
                } 

                else if (property.getPossibleValues()) 
                {
                    const possibleValues = property.getPossibleValues();
                    let subItems = [];
                    possibleValues.forEach((possibleValue, pvIndex) => {
                        subItems.push({
                            key: pvIndex,
                            label: possibleValue,
                            canCheck: true,
                            checked: (property.getValue() === possibleValue),
                            onClick: (event) => {
                                if (property.getValue() === possibleValue)
                                    property.setValue();
                                else
                                    property.setValue(possibleValue);
                                onClick(event);
                            }
                        });
                    });

                    items.push({
                        key: index,
                        label,
                        subMenu: {
                            items: subItems
                        }
                    });
                } 

                else if (property.getDataType() === "object") 
                {
                    if (property.getValue() === undefined)
                        property.setValue({});

                    /* recursively search for boolean options for easy menu items */
                    const subItems = buildMenu(property.getValue());
                    if (subItems.length > 0) {
                        items.push({
                            key: index,
                            label,
                            subMenu: {
                                items: subItems
                            }
                        });
                    }
                }

                else if (property.getDataType() === "array")
                {
                    let subItems = [];

                    if (property.getValue() !== undefined || property.getDefaultValue() !== undefined) {
                        
                        subItems.push({
                            key: "divider",
                            type: "divider"
                        });
                    }

                    subItems.push({
                        key: "1",
                        label: "Add Custom Value",
                        icon: "add"
                    });

                    items.push({
                        key: index,
                        label,
                        subMenu: {
                            items: subItems
                        }
                    });
                }

                else 
                {
                    /* create a menu item that, when clicked, brings up a dialog to input */
                    let subItems = [];
                    if (property.getValue() !== undefined || property.getDefaultValue() !== undefined) {
                        subItems.push({
                            key: "0",
                            canCheck: true,
                            checked: true,
                            label: property.getValue() ? property.getValue() : property.getDefaultValue()
                        });

                        subItems.push({
                            key: "divider",
                            type: "divider"
                        });
                    }

                    subItems.push({
                        key: "1",
                        label: "Set Custom Value",
                        icon: "edit"
                    });

                    items.push({
                        key: index,
                        label,
                        subMenu: {
                            items: subItems
                        }
                    });
                }
            });
        }

        return items.sort((item1, item2) => {

            if (item1.canCheck && !item2.canCheck)
                return -1;

            else if (item2.canCheck && !item1.canCheck)
                return 1;
                
            return item1.label.toLowerCase().localeCompare(item2.label.toLowerCase());
        });
    }, [onChanged]);

    useEffect(() => {
        setLayoutOptionsMenu(buildMenu(layoutParameters));        
    }, [layoutParameters, buildMenu]);

    return (
        <div>
            <Button
                key="LayoutButton"
                type="icon"
                onClick={(event) => {
                    setLayoutsMenuTarget(event.currentTarget);
                }}
                icon="more_vert"
                color="primary"
                tooltip="Layout Options"
                label="More Options"
            />
            <Menu
                key="LayoutsMenu"
                target={layoutsMenuTarget}
                open={Boolean(layoutsMenuTarget)}
                onClose={() => setLayoutsMenuTarget()}
                color="primary"
                items={[
                    {
                        key: "edit",
                        label: "Edit",
                        disabled: (editable === true || object.getAllowChange() === false),
                        onClick: () => {
                            props.onEditObject(true);
                            setLayoutsMenuTarget();
                        }
                    },
                    {
                        key: "copy",
                        label: "Copy",
                        disabled: true,
                    },
                    {
                        key: "paste",
                        label: "Paste",
                        disabled: true,
                    },
                    {
                        key: "divider2",
                        type: "divider"
                    },
                    {
                        key: "save",
                        label: "Save",
                        disabled: !savable,
                        onClick: () => {
                            onSave();
                            setLayoutsMenuTarget();
                        }
                    },
                    {
                        key: "reconcile",
                        label: "Reconcile",
                        disabled: !savable,
                        onClick: () => {
                            onReconcile();
                            setLayoutsMenuTarget();
                        }
                    },
                    {
                        key: "replace",
                        label: "Replace",
                        disabled: !savable,
                        onClick: () => {
                            onReplace();
                            setLayoutsMenuTarget();
                        }
                    },
                    {
                        key: "review",
                        label: "Review Changes",
                        disabled: !savable,
                        onClick: () => {
                            onReview();
                            setLayoutsMenuTarget();
                        }
                    },
                    {
                        key: "discard",
                        label: "Discard Changes",
                        disabled: !savable,
                        onClick: () => {
                            onDiscard();
                            setLayoutsMenuTarget();
                        }
                    },
                    {
                        key: "refresh",
                        label: "Refresh",
                        onClick: () => {
                            onRefresh();
                            setLayoutsMenuTarget();
                        }
                    },
                    {
                        key: "divider",
                        type: "divider"
                    },
                    {
                        key: "options",
                        label: "Object Options",
                        onClick: () => {
                            setObjectOptionsDialog(true);
                            setLayoutsMenuTarget();
                        }
                    },
                    {
                        key: "layout",
                        label: "Layout Options",
                        color: "primary",
                        subMenu: {
                            items: [
                                ...layoutOptionsMenu,
                                {
                                    key: "divider",
                                    type: "divider"
                                },
                                {
                                    key: "advanced",
                                    label: "Advanced",
                                    color: "primary",
                                    onClick: (event) => {
                                        event.stopPropagation();
                                        onLayoutParamsAdvanced();
                                        setLayoutsMenuTarget();
                                    }
                                },
                            ]
                        }
                    },
                ]}
            />
            <ObjectOptionsDialog
                open={objectOptionsDialog}
                onDismiss={() => {
                    setObjectOptionsDialog(false);
                }}
                options={objectOptions}
                onApply={(options) => {
                    setObjectOptions(options);
                    setObjectOptionsDialog(false);
                }}
            />
        </div>
    );
};


export default LayoutViewContextMenu;
