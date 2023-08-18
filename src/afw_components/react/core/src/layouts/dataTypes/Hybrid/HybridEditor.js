// See the 'COPYING' file in the project root for licensing information.
/* eslint-disable no-empty */
import {useState, useEffect} from "react";

import {
    Button,
    Dialog,
    TextField,
    Typography,
    useOperational,
    useTheme,
} from "@afw/react";

import {HybridBuilder} from "./HybridBuilder";


/**
 * HybridEditor displays a TextField with an "edit" icon to allow the user to either
 * enter in the hybrid expression or open up a Dialog to build the hybrid expression, 
 * using Expression Builder or Source Editor. 
 */
export const HybridEditor = (props) => {

    const [showModal, setShowModal] = useState(false);
    const [savable, setSavable] = useState(false);
    const [hybrid, setHybrid] = useState();
    const [warn, setWarn] = useState();    
    const [warnClear, setWarnClear] = useState();

    const {editable} = useOperational(props);
    const theme = useTheme();

    const {id, autoFocus, className, style, value, label, brief, description, dataTypeParameter, objectTypeObject, required, onChanged} = props;
    const [hybridStr, setHybridStr] = useState("");

    /*! \fixme use useReducer to combine all of these effect/states together */
    useEffect(() => {
        setHybrid(value);
        if (value !== undefined)
            setHybridStr(String(value));
        else 
            setHybridStr("");
    }, [value]);

    const onConfirmDismiss = () => {
        if (savable) {
            props.onDismiss();
            setHybrid(value);
        }
        setShowModal(false);
    };

    const onConfirmClear = () => {
        if (onChanged)
            onChanged();
        setHybridStr();
        setSavable(true);
    };

    const onDismiss = () => {
        if (!savable)
            onConfirmDismiss();
        else
            setWarn(true);
    };

    const onSave = () => {
        if (hybrid !== undefined) {
            props.onChanged(hybrid);
        }
        setShowModal(false);
    };

    const onTextChanged = (value) => {        
        if (onChanged)
            onChanged(value);
        setHybridStr(value);
        setSavable(true);
    };

    const onHybridChanged = (hybrid) => {
        setHybrid(hybrid);
        setSavable(true);
    };    

    return (
        <div className={className} style={style}>
            <Dialog 
                open={showModal}
                showClose={true}
                isBlocking={false}
                onDismiss={onDismiss}
                title="Expression Builder"
                maxWidth="xl"
                contains={
                    <div style={{ height: "calc(100vh - 300px)" /* \fixme */ }}>         
                        <HybridBuilder
                            hybrid={hybrid}
                            dataTypeParameter={dataTypeParameter}
                            objectTypeObject={objectTypeObject}
                            required={required}
                            onChanged={onHybridChanged}
                        > 
                        </HybridBuilder>     
                    </div>
                }
                footer={
                    <div style={{ display: "flex", justifyContent: "flex-end", width: "100%", padding: theme.spacing(1) }}>     
                        <Button                             
                            style={{ marginRight: theme.spacing(1) }}
                            color="primary"
                            variant="contained"
                            label="Ok"
                            onClick={onSave}
                            disabled={!savable}
                            size="small"
                        />
                        <Button                             
                            label="Cancel"
                            onClick={onDismiss}
                            size="small"
                        />
                    </div>
                }
            />
            <div style={{ display: "flex", alignItems: "center" }}>
                <div style={{ flex: 1 }}>                                      
                    <TextField 
                        id={id}
                        // eslint-disable-next-line jsx-a11y/no-autofocus                        
                        autoFocus={autoFocus}
                        type="text"
                        value={hybridStr}
                        required={required}
                        disabled={editable === false}
                        onChanged={onTextChanged}
                        label={label}
                        description={brief ? brief : description}
                    />
                </div>
                <div>
                    <Button
                        tooltip="Clear Value"
                        type="icon" 
                        label="Clear"                        
                        icon="clear"
                        disabled={(editable === false) || (value === undefined)}
                        onClick={() => setWarnClear(true)}
                    /> 
                    <Button
                        tooltip="Open in Editor"
                        type="icon" 
                        label="Edit"                        
                        icon="edit"
                        color="primary"                        
                        onClick={() => setShowModal(true)}
                    /> 
                </div>
            </div>
            <Dialog
                open={Boolean(warn)}
                onDismiss={() => setWarn(false)}
                isBlocking={false}
                maxWidth="sm"
                title="Discard Changes"
                showClose={true}
                contains={
                    <Typography text="This Expression has changed.  Discard the changes?" />
                }
                footer={
                    <div style={{ display: "flex", justifyContent: "flex-end", padding: theme.spacing(2), width: "100%" }}>
                        <div style={{ marginRight: theme.spacing(1) }}>
                            <Button
                                label="No"
                                onClick={() => setWarn(false)}
                            />
                        </div>
                        <div>
                            <Button
                                label="Yes"
                                color="primary"
                                variant="contained"
                                onClick={() => {
                                    setWarn(false);
                                    onConfirmDismiss();
                                }}
                            />
                        </div>
                    </div>
                }
            />
            <Dialog
                open={Boolean(warnClear)}
                onDismiss={() => setWarnClear(false)}
                isBlocking={false}
                maxWidth="sm"
                title="Clear Value"
                showClose={true}
                contains={
                    <Typography text="Clear this value?" />
                }
                footer={
                    <div style={{ display: "flex", justifyContent: "flex-end", padding: theme.spacing(2), width: "100%" }}>
                        <div style={{ marginRight: theme.spacing(1) }}>
                            <Button
                                label="No"
                                onClick={() => setWarnClear(false)}
                            />
                        </div>
                        <div>
                            <Button
                                label="Yes"
                                color="primary"
                                variant="contained"
                                onClick={() => {
                                    setWarnClear(false);
                                    onConfirmClear();
                                }}
                            />
                        </div>
                    </div>
                }
            />
        </div>
    );
};

HybridEditor.displayName = "HybridEditor";

export default HybridEditor;
