// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

import Button from "@mui/material/Button";
import Tooltip from "@mui/material/Tooltip";
import Badge from "@mui/material/Badge";
import CircularProgress from "@mui/material/CircularProgress";
import Notifications from "@mui/icons-material/Notifications";

import AppNotifications from "./AppNotifications";
import AppActiveRequests from "./AppActiveRequests";

import {useNotifications} from "../hooks";


/*
 * This component listens for informational events, such as
 *   loading data, or receiving errors.  It may then display
 *   appropriate icons or animations to indicate the event, so
 *   it can be displayed somewhere appropriate, such as the Menu.
 */
const AppStatus = () => {

    const [notificationsOpen, setNotificationsOpen] = useState(false);
    const [activeRequestsOpen, setActiveRequestsOpen] = useState(false);
    const [activeTasks, ] = useState(false);
    //const tasksCounter = useRef(0);
           
    const {notifications} = useNotifications();    
   
            
    return (
        <div> 
            {
                (!activeTasks) ?
                    <div style={{ display: "inline-block", verticalAlign: "top" }}>                    
                        <div>
                            <Tooltip title="Show Notifications">
                                { (notifications.length > 0) ?                                    
                                    <Button 
                                        style={{ color: "white" }}   
                                        onClick={() => setNotificationsOpen(true)}
                                        aria-label="Notifications"                                 
                                    >
                                        <Badge badgeContent={notifications.length}>
                                            <Notifications />
                                        </Badge>
                                    </Button>                                             
                                    :
                                    <Button 
                                        style={{ color: "white" }}   
                                        aria-label="Notifications"                                 
                                        onClick={() => setNotificationsOpen(true)}
                                    >
                                        <Notifications />
                                    </Button>
                                }
                            </Tooltip>
                        </div>                                 
                    </div>  
                    :
                    <div style={{ display: "inline-block", textAlign: "center" }}>      
                        <Button 
                            style={{ color: "white" }}   
                            onClick={() => setActiveRequestsOpen(true)}
                            aria-label="Notifications"                                 
                        >
                            <CircularProgress 
                                style={{ color: "white" }} 
                                variant="indeterminate"
                                size={24} 
                                value={100} 
                            />                                            
                        </Button>
                    </div>                
            }
            <AppNotifications 
                open={notificationsOpen}
                onDismiss={() => setNotificationsOpen(false)}
            />
            <AppActiveRequests 
                open={activeRequestsOpen}
                onDismiss={() => setActiveRequestsOpen(false)}
            />
        </div>
    );
};

export default AppStatus;
