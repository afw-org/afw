// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

import {
    Button,
    CodeEditor,
    Dialog,
    Divider,    
    PropertyTypeDropdown,
    Responsive,
    Spinner,    
    Typography,
    OperationalContext,
    OperationalMode,
    ValueLayout,
} from "@afw/react";

import {useTheme} from "../hooks";

/*
 * ObjectsBatchEdit
 *
 * This component displays a component that allows the user to specify 
 * object "batch" edit commands, producing "modify" entries that may be 
 * applied to one or more object at once.
 * 
 */


/*
 * RemoveValue
 *
 * A simple row to Remove the value of a property.
 */
const RemoveValue = ({ mod, objectTypeObject, onChanged, index }) => {

    const propertyName = mod[1];
    const value = mod[2];   

    const onValueChanged = (value) => {
        onChanged([ mod[0], mod[1], value ]);
    };

    const onPropertyChanged = (propertyName) => {
        onChanged([ mod[0], propertyName, null ]);
    };


    let propertyType;
    if (propertyName) {
        if (objectTypeObject.getPropertyValue([ "propertyTypes", propertyName ]))
            propertyType = objectTypeObject.getPropertyValue([ "propertyTypes", propertyName ]).toJSON();
    }
        
    return (
        <Responsive.Row 
            spacing={4}
            alignItems="center"
            columns={[
                {
                    key: "operation",
                    breakpoints: {
                        small: 6,
                        medium: 2,
                        large: 1
                    },
                    contains: 
                        <Typography text="From Property" color="textSecondary" />                        
                },
                {
                    key: "property",
                    breakpoints: {
                        small: 6,
                        medium: 3,
                        large: 2
                    },
                    contains: 
                        <PropertyTypeDropdown 
                            id={"batch-edit-from-property-" + index}
                            required={true}
                            objectTypeObject={objectTypeObject}  
                            value={propertyName}          
                            onChanged={onPropertyChanged}   
                            onFilter={propertyType => {
                                if (propertyType) {
                                    const dataType = propertyType.getPropertyValue("dataType");
                                    if (dataType === "array" || dataType === "object")
                                        return false;
                                }
                                return true;
                            }}             
                        />
                },
                {
                    key: "operation",
                    breakpoints: {
                        small: 6,
                        medium: 2,
                        large: 2
                    },
                    contains: 
                        <Typography 
                            style={{ textAlign: "center" }} 
                            text="Remove Value" 
                            color="textSecondary" 
                        />                        
                },
                {
                    key: "value",
                    breakpoints: {
                        small: 6,
                        medium: 3,
                        large: 2
                    },
                    contains: 
                        propertyType ? 
                            <ValueLayout                         
                                valueMeta={{
                                    ...propertyType,
                                    label: undefined,
                                    brief: undefined,
                                    description: undefined
                                }}
                                value={value}
                                onChanged={onValueChanged}                                
                            /> : null
                }
            ]}
        />
    );
};

/*
 * AddValue
 *
 * A simple row to Add the value to a property.
 */
const AddValue = ({ mod, objectTypeObject, onChanged, index }) => {

    const propertyName = mod[1];
    const value = mod[2];   

    const onValueChanged = (value) => {
        onChanged([ mod[0], mod[1], value ]);
    };

    const onPropertyChanged = (propertyName) => {
        onChanged([ mod[0], propertyName, null ]);
    };


    let propertyType;
    if (propertyName) {
        if (objectTypeObject.getPropertyValue([ "propertyTypes", propertyName ]))
            propertyType = objectTypeObject.getPropertyValue([ "propertyTypes", propertyName ]).toJSON();
    }
        
    return (
        <Responsive.Row 
            spacing={4}
            alignItems="flex-end"
            columns={[
                {
                    key: "operation",
                    breakpoints: {
                        small: 6,
                        medium: 2,
                        large: 1
                    },
                    contains: 
                        <Typography text="Add Property" color="textSecondary" />                        
                },
                {
                    key: "property",
                    breakpoints: {
                        small: 6,
                        medium: 3,
                        large: 2
                    },
                    contains: 
                        <PropertyTypeDropdown 
                            id={"batch-edit-add-property-" + index}
                            required={true}
                            objectTypeObject={objectTypeObject}  
                            value={propertyName}          
                            onChanged={onPropertyChanged}        
                            onFilter={propertyType => {
                                if (propertyType) {
                                    const dataType = propertyType.getPropertyValue("dataType");
                                    if (dataType === "array" || dataType === "object")
                                        return false;
                                }
                                return true;
                            }}        
                        />
                },
                {
                    key: "operation",
                    breakpoints: {
                        small: 6,
                        medium: 2,
                        large: 2
                    },
                    contains: 
                        <Typography 
                            style={{ textAlign: "center" }} 
                            text="Value" 
                            color="textSecondary" 
                        />                        
                },
                {
                    key: "value",
                    breakpoints: {
                        small: 6,
                        medium: 3,
                        large: 2
                    },
                    contains: 
                        propertyType ? 
                            <ValueLayout                         
                                valueMeta={{
                                    ...propertyType,
                                    label: undefined,
                                    brief: undefined,
                                    description: undefined
                                }}
                                value={value}
                                onChanged={onValueChanged}    
                            /> : null
                }
            ]}
        />
    );
};

/*
 * SetProperty
 *
 * A simple row to set a Property to a specific value.
 */
const SetProperty = ({ objectTypeObject, onChanged, mod, index }) => {

    const propertyName = mod[1];
    const value = mod[2];    

    const onValueChanged = (value) => {
        onChanged([ mod[0], mod[1], value ]);
    };

    const onPropertyChanged = (propertyName) => {
        onChanged([ mod[0], propertyName, null ]);
    };

    let propertyType;
    if (propertyName) {
        if (objectTypeObject.getPropertyValue([ "propertyTypes", propertyName ]))
            propertyType = objectTypeObject.getPropertyValue([ "propertyTypes", propertyName ]).toJSON();
    }
            
    return (
        <Responsive.Row 
            spacing={4}
            alignItems="flex-end"
            columns={[
                {
                    key: "operation",
                    breakpoints: {
                        small: 6,
                        medium: 2,
                        large: 1
                    },
                    contains: 
                        <Typography text="Set Property" color="textSecondary" />                        
                },
                {
                    key: "property",
                    breakpoints: {
                        small: 6,
                        medium: 3,
                        large: 2
                    },
                    contains: 
                        <PropertyTypeDropdown 
                            id={"batch-edit-set-property-" + index}
                            required={true}
                            objectTypeObject={objectTypeObject}  
                            value={propertyName}          
                            onChanged={onPropertyChanged}               
                            onFilter={propertyType => {
                                if (propertyType) {
                                    const dataType = propertyType.getPropertyValue("dataType");
                                    if (dataType === "array" || dataType === "object")
                                        return false;
                                }
                                return true;
                            }} 
                        />
                },
                {
                    key: "operation",
                    breakpoints: {
                        small: 6,
                        medium: 2,
                        large: 1
                    },
                    contains: 
                        <Typography                             
                            text="To Value" 
                            color="textSecondary" 
                        />                        
                },
                {
                    key: "value",
                    breakpoints: {
                        small: 6,
                        medium: 3,
                        large: 2
                    },
                    contains: 
                        propertyType ? 
                            <ValueLayout                         
                                valueMeta={{
                                    ...propertyType,
                                    label: undefined,
                                    brief: undefined,
                                    description: undefined
                                }}
                                value={value}
                                onChanged={onValueChanged}                                                                    
                            />
                            : null
                }
            ]}
        />
    );
};

/*
 * RemoveProperty 
 *
 * A simple row to remove a Property.
 */
const RemoveProperty = ({ objectTypeObject, onChanged, mod, index }) => {

    const propertyName = mod[1];
    
    const onPropertyChanged = (propertyName) => {
        onChanged([ mod[0], propertyName, null ]);
    };
           
    return (
        <Responsive.Row 
            spacing={4}
            alignItems="flex-end"
            columns={[
                {
                    key: "operation",
                    breakpoints: {
                        small: 6,
                        medium: 2,
                        large: 2
                    },
                    contains: 
                        <Typography text="Remove Property" color="textSecondary" />                        
                },
                {
                    key: "property",
                    breakpoints: {
                        small: 6,
                        medium: 3,
                        large: 2
                    },
                    contains: 
                        <PropertyTypeDropdown 
                            id={"batch-edit-remove-property-" + index}
                            required={true}
                            objectTypeObject={objectTypeObject}  
                            value={propertyName}          
                            onChanged={onPropertyChanged}          
                            onFilter={propertyType => {
                                if (propertyType) {
                                    const dataType = propertyType.getPropertyValue("dataType");
                                    if (dataType === "array" || dataType === "object")
                                        return false;
                                }
                                return true;
                            }}      
                        />
                }                
            ]}
        />
    );
};

const ModEntry = ({ mods, index, children }) => {

    const theme = useTheme();
    return (
        <div key={index} style={{ marginTop: theme.spacing(2) }}>
            { children }                        
            { index < (mods.length - 1) &&                                     
                <Divider style={{ marginTop: theme.spacing(4) }} />                
            }            
        </div>
    );
};

/*
 * ObjectsBatchEdit
 *
 * Displays the main layout for adding modification commands. 
 */
const ObjectsBatchEdit = (props) => {
    
    const theme = useTheme();
    const [mods, setMods] = useState(props.mods);    

    const addMod = (mod) => {
        let newMods = [...mods, mod];

        setMods(newMods);
    };

    const onModChanged = (mod, index) => {
        let newMods = [...mods];
        newMods[index] = mod;

        setMods(newMods);

        props.onModEntriesChanged(newMods);
    };

    return (
        <div style={{ padding: theme.spacing(2) }}>  
            <div style={{ display: "flex", alignItems: "center" }}>
                <div>
                    <Typography 
                        size="4"
                        text="Create a set of modification actions to apply to the selected object(s)."
                    />
                </div>
                <div>
                    <Button 
                        type="icon"
                        label="Add Action"
                        tooltip="Add Action"
                        icon="add" 
                        color="primary"      
                        items={[
                            {
                                key: "set_property",
                                label: "Set Property",
                                onClick: () => addMod(["set_property"])
                            },
                            {
                                key: "remove_property",
                                label: "Remove Property",
                                onClick: () => addMod(["remove_property"])
                            },
                            {
                                key: "add_value",
                                label: "Add Value",
                                onClick: () => addMod(["add_value"])
                            },
                            {
                                key: "remove_value",
                                label: "Remove Value",
                                onClick: () => addMod(["remove_value"])
                            }
                        ]}                          
                    />
                </div>
            </div>
            <div style={{ marginLeft: theme.spacing(2) }}>
                <OperationalContext.Provider value={OperationalMode.Editable}>
                    {
                        mods.map((mod, index) => 
                            <ModEntry key={index} index={index} mods={mods}>
                                {
                                    (mod[0] === "set_property") ?
                                        <SetProperty                                 
                                            {...props}
                                            key={index}
                                            mod={mod}
                                            onChanged={(mod) => onModChanged(mod, index)}
                                        />     

                                        : (mod[0] === "remove_property") ?
                                            <RemoveProperty                                 
                                                {...props}
                                                key={index}
                                                mod={mod}
                                                onChanged={(mod) => onModChanged(mod, index)}
                                            />
                                    
                                            : (mod[0] === "add_value") ?
                                                <AddValue                                 
                                                    {...props}
                                                    key={index}
                                                    mod={mod}
                                                    onChanged={(mod) => onModChanged(mod, index)}
                                                />
                                    
                                                : (mod[0] === "remove_value") ?
                                                    <RemoveValue
                                                        {...props}
                                                        key={index}
                                                        mod={mod}
                                                        onChanged={(mod) => onModChanged(mod, index)}
                                                    />                                    

                                                    : null
                                }
                            </ModEntry>
                        )
                    }
                </OperationalContext.Provider>
            </div>
        </div>  
    );
};

/*
 * ObjectsBatchEditModal
 *
 * Wraps the ObjectsBatchEdit layout with a Dialog, along with
 * confirmation dialogs, spinners, error handling, and the actual
 * operation to modify selected objects.
 */
const ObjectsBatchEditModal = (props) => {

    const theme = useTheme();
    const [savable, setSavable] = useState(false);
    const [mods, setMods] = useState([]);    
    const [confirm, setConfirm] = useState(false);
    const [results, setResults] = useState();
    const [spinner, setSpinner] = useState(false);
    const [showSource, setShowSource] = useState(false);
    
    const onModEntriesChanged = (newMods) => {
        setMods(newMods);

        setSavable(true);
    };
   
    /*! \fixme this should batch the modifies in one document, instead of one by one? */
    const onConfirm = async () => {
        try {
            const objects = props.objects;

            setSpinner(true);
            for (let object of objects) {
                const response = object.modify(mods);
                await response.result();
            }

            setResults("Objects modified successfully.");
        } catch (error) {
            setResults(error);
        }

        setSpinner(false);
        setSavable(false);
        setConfirm(false);
    };

    const onDismiss = () => {
        setSavable(false);
        setMods([]);
        setResults();
        
        if (props.onDismiss)
            props.onDismiss();
    };

    return (
        <>
            <Dialog
                open={props.open}
                onDismiss={props.onDismiss}
                isBlocking={true}
                maxWidth="xl"
                title="Batch Edit"
                showClose={true}
                contains={
                    <div style={{ height: "60vh", overflow: "auto" }}>
                        {
                            showSource ?
                                <CodeEditor 
                                    source={ JSON.stringify(mods, null, 4) }
                                    onChanged={newValue => {
                                        try {
                                            const mods = JSON.parse(newValue);                                            
                                            setMods(mods);
                                        // eslint-disable-next-line no-empty
                                        } catch (e) {}
                                    }}
                                /> :
                                <ObjectsBatchEdit 
                                    {...props}
                                    mods={mods}
                                    onModEntriesChanged={onModEntriesChanged}
                                />
                        }                        
                        {
                            spinner &&
                                <Spinner />
                        }                        
                    </div>
                }
                footer={
                    <div style={{ display: "flex", justifyContent: "space-between", width: "100%", padding: theme.spacing(2) }}>
                        <div>
                            { showSource ?
                                <Button
                                    label="UI Mode"
                                    onClick={() => setShowSource(false)}                                
                                /> :
                                <Button
                                    label="Source Mode"
                                    onClick={() => setShowSource(true)}                                
                                />
                            }
                            
                        </div>
                        <div style={{ display: "flex", justifyContent: "flex-end" }}>
                            <div style={{ marginRight: theme.spacing(1) }}>
                                <Button
                                    color="primary"
                                    variant="contained"
                                    label="Apply Changes"
                                    onClick={() => setConfirm(true)}
                                    disabled={!savable}
                                />
                            </div>
                            <div>
                                <Button
                                    label="Cancel"
                                    onClick={onDismiss}
                                />
                            </div>  
                        </div>                                              
                    </div>
                }
            />
            <Dialog 
                open={confirm}
                title="Confirm Modifications"
                maxWidth="sm"
                contains={
                    <Typography 
                        text={"Apply these changes to " + props.objects.length + " selected objects?"}
                    />
                }
                footer={
                    <div style={{ display: "flex", justifyContent: "space-between", width: "100%", padding: theme.spacing(2) }}>                        
                        <Button
                            color="primary"
                            variant="contained"
                            label="Ok"
                            onClick={onConfirm}
                        />
                        <Button
                            label="Cancel"
                            onClick={() => setConfirm(false)}
                        />
                    </div>
                }
            />
            <Dialog 
                open={Boolean(results)}
                title="Results"
                maxWidth="sm"
                contains={
                    <Typography 
                        text={results}
                    />
                }
                footer={
                    <div style={{ display: "flex", justifyContent: "flex-end", width: "100%", padding: theme.spacing(2) }}>                       
                        <Button
                            color="primary"
                            variant="contained"
                            label="Ok"
                            onClick={onDismiss}
                        />
                    </div>
                }
            />
        </>
    );
};

export default ObjectsBatchEditModal;
