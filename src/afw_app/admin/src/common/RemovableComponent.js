// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

import {
    Button,
    Dialog,
    Typography,
    useOperational
} from "@afw/react";

import {useTheme} from "../hooks";

/**
 * RemovableComponent
 * 
 * This is a utility component that allows other components to be 
 * "removed", when a close icon is clicked.  This component tucks away
 * the state and Dialog, used to perform the task.
 * 
 */
export const RemovableComponent = ({ component, onRemove, removeTitle, removeText }) => {

    const theme = useTheme();
    const [showConfirm, setShowConfirm] = useState(false);
    const {editable} = useOperational();

    /* if we're not in editable mode, just return the component */
    if (!editable)
        return component;

    return (
        <>
            <div style={{ display: "flex", height: "100%", width: "100%" }}>
                <div>
                    <Button 
                        style={{ color: "red" }}
                        type="icon"
                        icon="close"
                        tooltip={removeTitle}
                        label={removeTitle}
                        onClick={() => setShowConfirm(true)}
                    />
                </div>
                <div style={{ flex: 1 }}>
                    { component }
                </div>
            </div>
            <Dialog 
                open={showConfirm}
                title={removeTitle}
                showClose={true}
                onDismiss={() => setShowConfirm(false)}
                contains={
                    <div>
                        <Typography text={removeText} />
                    </div>
                }
                footer={
                    <div style={{ paddingTop: theme.spacing(2), textAlign: "right" }}>
                        <Button 
                            style={{ display: "inline-block", marginRight: theme.spacing(0.5) }}
                            label="Yes"
                            color="primary"
                            variant="contained"
                            onClick={() => onRemove()}
                        />
                        <Button 
                            style={{ display: "inline-block" }}
                            label="No"
                            onClick={() => setShowConfirm(false)}
                        />
                    </div>
                }
            />
        </>
    );

};

export default RemovableComponent;
