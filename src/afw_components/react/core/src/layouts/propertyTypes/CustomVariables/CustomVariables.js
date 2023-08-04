// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";

import {
    Button,
    Dialog,
    Message,
    Table,
    TextField,
    Typography,
} from "@afw/react";

import {Hybrid} from "../../dataTypes/Hybrid";
import {useOperational, useEventId} from "@afw/react";


/**
 * CustomVariableEditor
 * 
 * When a variable is edited, or a new one created, this displays
 * a Dialog to name the variable and assign a value.
 */
export const CustomVariableEditor = (props) => {
    
    const {custom, variable} = props;

    const [newName, setNewName] = useState();    
    const [savable, setSavable] = useState();

    const changed = useEventId({ object: custom, eventId: "onChanged" });

    useEffect(() => {
        if (variable)
            setNewName(variable.getName());
    }, [variable]);

    useEffect(() => setSavable(true), [changed]);

    /* When the name of the property is changed, determine if it's savable */
    const onPropertyNameChanged = (newName) => {
        let savable = true;
        
        if (!newName)
            savable = false;

        else if (custom && custom.hasProperty(newName))
            savable = false;

        setNewName(newName);
        setSavable(savable);
    };


    /* Make sure the property is valid (not already assigned) */
    const onPropertyValidate = (newName) => {

        if (variable.getName() === newName)
            return true;

        else if (custom && custom.hasProperty(newName))
            return false;        

        else
            return true;
    };

    const onSave = () => {
        if (newName !== variable.getName())
            variable.setName(newName);

        setNewName();
        setSavable();

        props.onSave(variable);
    };

    const onDismiss = () => {
        setNewName();
        setSavable();

        props.onDismiss();
    };

    return (
        <Dialog 
            open={Boolean(variable)}
            isBlocking={true}
            onDismiss={onDismiss}
            title="Custom Variable Editor"
            maxWidth="lg"
            contains={
                <>
                    <div>                                        
                        <TextField 
                            // eslint-disable-next-line jsx-a11y/no-autofocus
                            autoFocus={true}
                            label="Variable Name"         
                            required={true}                         
                            description="The variable name can be referenced by other context-appropriate expressions."
                            value={newName}
                            onValidate={onPropertyValidate}
                            onChanged={onPropertyNameChanged}
                        />                        
                    </div>
                    <div style={{ height: "24px "}} />
                    <div>
                        <Hybrid   
                            label="Variable Value"
                            property={variable}       
                            value={variable ? variable.getValue() : undefined}
                            description="Enter or create a value for this custom variable."
                        />
                    </div>
                    <div style={{ height: "24px "}} />
                </>
            }
            footer={
                <div style={{ display: "flex", textAlign: "right" }}>
                    <div style={{ marginRight: "5px" }}>
                        <Button 
                            label="Save"
                            color="primary"
                            variant="contained"
                            onClick={onSave}
                            disabled={!savable}
                        />
                    </div>
                    <div style={{ marginRight: "5px" }}>
                        <Button 
                            label="Cancel"
                            onClick={onDismiss}
                        />
                    </div>                            
                </div>
            }
        />
    );
};

/**
 * 
 * CustomVariables
 * 
 * Displays the custom "variables" properties in an editable table format, 
 * that uses a modal dialog to add or edit new variables.
 */
export const CustomVariables = (props) => {

    const [variableNames, setVariableNames] = useState([]);
    const [selectedVariables, setSelectedVariables] = useState([]);
    const [editVariable, setEditVariable] = useState();        

    const {editable} = useOperational(props);

    const custom = props.custom;    

    useEffect(() => {
        if (custom && custom.getValue()) {
            const properties = custom.getValue().getProperties().sort(
                (A, B) => A.getName().toLowerCase().localeCompare(B.getName().toLowerCase())
            );
            
            setVariableNames(properties.map(p => p.getName()));
        }
    }, [custom]);
    
    
    const onAddVariable = () => {
        if (!custom.getValue())
            custom.setValue({});

        const editVariable = custom.getValue().newProperty();        

        setEditVariable(editVariable);
    };

    const onEditVariable = () => {
        const editVariable = selectedVariables[0];

        setEditVariable(custom.getValue().getProperty(editVariable));
    };

    const onDeleteVariable = () => {
        selectedVariables.forEach(variable => {
            custom.getValue().removeProperty(variable);
        });

        const properties = custom.getValue().getProperties().sort(
            (A, B) => A.getName().toLowerCase().localeCompare(B.getName().toLowerCase())
        );
        
        setVariableNames(properties.map(p => p.getName()));
    };

    const onVariableInvoked = (variable) => {
        setEditVariable(custom.getValue().getProperty(variable));
    };

    const onDismiss = () => {
        editVariable.discardChanges();
        setEditVariable();        
    };

    const onSave = () => {
        
        custom.getValue().setPropertyValue(editVariable.getName(), editVariable.getValue());

        const properties = custom.getValue().getProperties().sort(
            (A, B) => A.getName().toLowerCase().localeCompare(B.getName().toLowerCase())
        );
        
        setVariableNames(properties.map(p => p.getName()));
        setEditVariable();        
    };
    

    return (
        <>
            <div style={{ padding: "4px" }}>
                <Message message="Custom variables are user-defined constants, expressions or functions, that may be used by expressions to influence policy decision making." />
            </div>
            <div style={{ height: "16px" }} />
            <div style={{ display: "flex", alignItems: "center" }}>                
                <Typography size="5" text="Custom Variables" />                
                {
                    editable && (
                        <div style={{ display: "flex" }}>
                            <Button 
                                type="icon"
                                label="Add Variable"
                                icon="add"
                                color="primary"
                                onClick={onAddVariable}
                                tooltip="Add Variable"
                            />    
                            <Button 
                                type="icon"
                                label="Remove Selected Variables"
                                icon="remove"
                                color="primary"
                                disabled={!selectedVariables || selectedVariables.length === 0}
                                onClick={onDeleteVariable}
                                tooltip="Remove Selected Variable(s)"
                            />                        
                            <Button 
                                type="icon"
                                label="Edit Selected Variable"
                                icon="edit"
                                color="primary"
                                disabled={!selectedVariables || selectedVariables.length !== 1 }
                                onClick={onEditVariable}
                                tooltip="Edit Selected Variable"
                            />                                                        
                        </div>
                    )
                }
            </div>
            <Table 
                selectionMode={editable ? "multiple" : "none"}
                compact={true}
                rows={variableNames}
                onSelectionChanged={(vars) => setSelectedVariables([...vars])}
                onRowInvoked={onVariableInvoked}
                columns={[
                    {
                        key: "Variable",
                        name: "Variable",
                        minWidth: 100,
                        maxWidth: 200,
                        isResizable: true,
                        onRender: (variable) => {
                            return <Typography text={variable} />;
                        }
                    },
                    {
                        key: "Description",
                        name: "Description",
                        minWidth: 100,
                        maxWidth: 400,
                        isResizable: true,
                        onRender: (variable) => {
                            const property = custom.getValue().getProperty(variable);

                            return (
                                <Typography size="2" text={property.getValue()} />
                            );
                        }
                    }
                ]}
            />
            <CustomVariableEditor 
                custom={custom ? custom.getValue() : undefined}
                variable={editVariable}
                onSave={onSave}
                onDismiss={onDismiss}
            />
        </>
    );
};


export default CustomVariables;
