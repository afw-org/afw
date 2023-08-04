// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

import {useNotifications} from "../hooks";

import {
    Button,
    Dialog,
    Table,
} from "@afw/react";

import {useTheme} from "../hooks";

export const AppActiveRequests = (props) => {

    const theme = useTheme();
    const {activeRequests, onCancelActiveRequests} = useNotifications();
    const [selectedRequests, setSelectedRequests] = useState([]);
    const {open, onDismiss} = props;

    const onRequestsSelected = (selectedRequests) => {
        setSelectedRequests([...selectedRequests]);
    };

    if (!open)
        return null;

    return (
        <Dialog 
            open={open}
            showClose={true}
            onDismiss={() => {
                setSelectedRequests([]);
                onDismiss();
            }}
            title="Active Requests"
            maxWidth="md"
            contains={
                <div style={{ padding: theme.spacing(2) }}>                                                                        
                    <Table 
                        columns={[
                            {
                                key: "timestamp",
                                name: "Time",                            
                                minWidth: 100,
                                onRender: (activeRequest) => {
                                    return activeRequest.timestamp.toLocaleTimeString();
                                }
                            },
                            {
                                key: "request",
                                name: "Request",
                                onRender: (activeRequest) => {                             
                                    return activeRequest.data;
                                }
                            }
                        ]}
                        rows={Object.values(activeRequests)}
                        onSelectionChanged={onRequestsSelected}
                    />   
                </div>
            }
            footer={
                <div style={{ display: "flex", padding: theme.spacing(2), width: "100%" }}>
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button 
                            label="Cancel Request(s)"  
                            size="small"
                            onClick={() => onCancelActiveRequests(selectedRequests)}                          
                            disabled={selectedRequests.length === 0}
                        />
                    </div>
                    <div style={{ flex: 1 }} />
                    <div>
                        <Button 
                            label="Ok"                             
                            variant="contained"
                            size="small" 
                            onClick={onDismiss}                        
                        />
                    </div>
                </div>
            }
        />
    );
};

export default AppActiveRequests;
