// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

import {
    Button,
    Dialog,
    Icon,
    Table,
    Typography,
} from "@afw/react";

import {useTheme} from "../hooks";

const ErrorObject = ({ obj }) => {
    const [expand, setExpand] = useState(false);

    const stringify = JSON.stringify(obj, Object.getOwnPropertyNames(obj), 4);

    if (expand) {
        return (
            <span style={{ whiteSpace: "pre-wrap" }}>
                <Typography text={stringify} />
            </span>
        );
    } else {
        return (
            <Button 
                type="icon" 
                label="More Details"
                tooltip="More Details"
                size="small"
                icon="more_horizontal" 
                onClick={() => setExpand(true)} 
            />
        );
    }
};

export const ErrorForm = (props) => {

    const error = props.error;

    if (!error)
        return null;

    return (
        <Table 
            columns={[
                {
                    key: "Property",
                    name: "Property",
                    style: {verticalAlign: "top"},
                    minWidth: 100,
                    maxWidth: 200,
                    isResizable: true,
                    onRender: (key) => {
                        return (
                            <Typography color="textSecondary" text={key} />
                        );
                    }
                },
                {
                    key: "Value",
                    name: "Value",
                    minWidth: 100,
                    maxWidth: 400,
                    isResizable: true,
                    isMultiline: true,
                    onRender: (key) => {
                        const err = error[key];
                        if ((typeof(err) === "string") && err.indexOf("\n") >= 0) {
                            return (
                                <span style={{ whiteSpace: "pre-wrap" }}>
                                    <Typography text={String(err)} />
                                </span>
                            );
                        } else if (typeof(err) === "string" || key === "timestamp") {
                            return <Typography text={String(err)} />;
                        } else {
                            if (typeof(err) === "object")
                                return (
                                    <ErrorObject obj={err} />
                                );

                            return <Typography text={String(err)} />;
                        }
                    }
                }
            ]}   
            rows={
                Object.keys(error)
            }   
            selectionMode="none"                          
        />
    );
};

export const ErrorDialog = (props) => {

    const theme = useTheme();
    const {error, onDismiss} = props;
    const [errorDetails, setErrorDetails] = useState(props.errorDetails);

    return (
        <Dialog 
            open={Boolean(error)}
            onDismiss={onDismiss}
            isBlocking={false}
            showClose={true}
            maxWidth={errorDetails ? "lg" : "sm"}
            title={
                <div style={{ display: "flex", alignItems: "center" }}>
                    <Icon iconName="error" style={{ marginRight: theme.spacing(1) }} />
                    <Typography text="An Error Occurred" size="6" component="span" />
                </div>
            }
            contains={
                <div style={{ maxHeight: "80vh", overflow: "auto" }}>                                    
                    { errorDetails ?
                        <ErrorForm error={error} /> :
                        <Typography text={error ? error["message"] : "See Error notification for details."} />
                    }
                </div>
            }
            footer={
                <div style={{ display: "flex", justifyContent: "flex-end", padding: theme.spacing(2), width: "100%" }}>
                    <div>          
                        <div style={{ display: "inline-block", marginRight: theme.spacing(1) }}>    
                            <Button 
                                label="Details"        
                                icon="unfold_more"                                
                                onClick={() => {                                                        
                                    errorDetails ? setErrorDetails(false) : setErrorDetails(true);
                                }}
                            />                                                                                                      
                        </div>                                    
                        <div style={{ display: "inline-block" }}>    
                            <Button 
                                color="primary"
                                variant="contained"
                                label="Ok"                                        
                                onClick={onDismiss}
                            />                                                                                                      
                        </div>
                    </div>
                </div>
            }
        />
    );
};

export default ErrorForm;
