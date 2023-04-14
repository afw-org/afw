// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

import {
    Button,
    Dialog,
    Message,
    Table,
} from "@afw/react";

import {ErrorDialog} from "../common/ErrorForm";

import {copyToClipboard} from "../utils";
import {useNotifications, useTheme} from "../hooks";

const AppNotificationsError = (props) => {
    
    if (!props.open || !props.error)
        return null;
    
    let error;
    if (props.error.error && props.error.error.response && props.error.error.response.data) {
        error = props.error.error.response.data;            
    } else if (props.error.error instanceof Error) {
        error = props.error.error;
    } else if (props.error.error) {
        error = props.error.error;
    } else {
        error = props.error;
    }

    return (
        <ErrorDialog 
            error={error}
            onDismiss={props.onDismiss}
            errorDetails={true}
        />
    );
    
};


const AppNotifications = (props) => {

    const theme = useTheme();
    const [showError, setShowError] = useState(false);
    const [selectedNotifications, setSelectedNotifications] = useState([]);
    const [notification, setNotification] = useState();
    
    const onNotificationInvoked = (error) => {
        setShowError(true);
        setNotification(error);        
    };

    const onNotificationsSelected = (selectedNotifications) => {
        setSelectedNotifications([...selectedNotifications]);
    };

    const onCopyNotification = () => {
        copyToClipboard(selectedNotifications);
    };
        
    const {notifications, onClearNotifications} = useNotifications();
    const {open, onDismiss} = props;

    return (
        <Dialog 
            open={open}
            showClose={true}
            onDismiss={() => {
                setNotification();
                setSelectedNotifications([]);
                onDismiss();
            }}
            title="Notifications"
            maxWidth="lg"
            contains={
                <div style={{ padding: theme.spacing(2) }}>    
                    <Message 
                        status="info" 
                        message="As you use the Administrative App, notifications may appear in the table below to alert you to any potential problems.  These problems may provide messages and contextual information, that can be helpful in troubleshooting what went wrong." 
                    />
                    <Table 
                        columns={[
                            {
                                key: "timestamp",
                                name: "Time",                            
                                minWidth: 100,
                                maxWidth: 150,
                                isResizable: true,
                                onRender: (error) => {
                                    return error.timestamp.toLocaleTimeString();
                                }
                            },
                            {
                                key: "message",
                                name: "Message",
                                minWidth: 100,
                                maxWidth: 400,
                                isResizable: true,
                                isMultiline: true,
                                onRender: (error) => {                                
                                    if (error.message)
                                        return error.message;

                                    if (error.error && error.error instanceof Error) {
                                        if (error.error.response && error.error.response.data && error.error.response.data.message)
                                            return error.error.response.data.message;
                                        else if (error.error.message)
                                            return error.error.message;
                                    } else if (error.error)
                                        return error.error;
                                    else if (error.response && error.response.statusText)
                                        return error.response.statusText;
                                    else 
                                        return error.message;
                                }
                            }
                        ]}
                        rows={notifications}
                        onSelectionChanged={onNotificationsSelected}
                        onRowInvoked={onNotificationInvoked}
                    />                  
                    <AppNotificationsError 
                        open={showError}
                        error={notification}
                        onDismiss={() => setShowError(false)}
                    />
                </div>
            }
            footer={
                // \fixme condense buttons/options for mobile
                <div style={{ display: "flex", justifyContent: "space-between", padding: theme.spacing(2), width: "100%" }}>
                    <div>
                        <div style={{ display: "inline-block", marginRight: theme.spacing(1) }}>
                            <Button 
                                label="View Details"
                                size="small"
                                onClick={() => {
                                    setNotification(selectedNotifications[0]);
                                    setShowError(true);
                                }}                            
                                disabled={selectedNotifications.length !== 1}
                            />
                        </div>
                        <div style={{ display: "inline-block", marginRight: theme.spacing(1) }}>
                            <Button 
                                label="Copy Error(s)"  
                                size="small"
                                onClick={onCopyNotification}                          
                                disabled={selectedNotifications.length === 0}
                            />
                        </div>
                        <div style={{ display: "inline-block" }}>
                            <Button 
                                label="Cancel"  
                                size="small"
                                onClick={onDismiss}                        
                            />
                        </div>
                    </div>
                    <div>
                        <Button 
                            label="Clear Error(s)" 
                            color="primary"
                            variant="contained"
                            size="small" 
                            disabled={selectedNotifications.length === 0}
                            onClick={() => onClearNotifications(selectedNotifications)}                        
                        />
                    </div>
                </div>
            }
        />
    );
};

export default AppNotifications;
