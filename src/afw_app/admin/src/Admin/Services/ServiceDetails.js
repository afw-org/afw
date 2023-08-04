// See the 'COPYING' file in the project root for licensing information.
import {
    Button,
    Link,
    ObjectResponsive,
    Typography,
    useServices
} from "@afw/react";

import {
    afwServiceStart, 
    afwServiceRestart, 
    afwServiceStop
} from "@afw/core";

import {useApplication, useTheme} from "../../hooks";

/**
 * ServiceDetails Displays more details when a user double-clicks a service
 * from the /Admin/Services table.
 */
const ServiceDetails = (props) => {

    const {client, notification} = useApplication();
    const {onRefresh: reloadServices} = useServices();
    const theme = useTheme();

    const service = props.service;
    if (!service)
        return null;

    const {serviceId, confId, canStart, canStop, canRestart} = service.getPropertyValues();


    const startService = async () => {        
        try {
            let result = await afwServiceStart(client, serviceId).result();            
            const dialogMessage = result["statusDescription"];

            notification({ message: dialogMessage, type: "success", duration: 3000 });
        } catch (error) {
            let errorMessage = String(error);

            if (error["message"])
                errorMessage = error["message"];

            notification({ message: errorMessage, duration: "error" });     
        }   

        reloadServices();   
    };

    const stopService = async () => {
        try {
            let result = await afwServiceStop(client, serviceId).result();            
            let dialogMessage = result["statusDescription"];

            notification({ message: dialogMessage, type: "success", duration: 3000 });
        } catch (error) {
            let errorMessage = String(error);

            if (error["message"])
                errorMessage = error["message"];

            notification({ message: errorMessage, type: "error" });
        }

        reloadServices();
    };

    const restartService = async () => {    
        try {
            const result = await afwServiceRestart(client, serviceId).result();            
            let dialogMessage = result["statusDescription"];

            notification({ message: dialogMessage, type: "success", duration: 3000 });
        } catch (error) {
            let errorMessage = String(error);

            if (error["message"])
                errorMessage = error["message"];

            notification({ message: errorMessage, type: "error" });
        }

        reloadServices();
    };

    return (
        <div className="SelectedService">
            <div className="ServicesHeader">                    
                <Typography size="8" text={"Service Information for " + confId} />
            </div>      
            <div style={{ paddingTop: theme.spacing(2) }}>                          
                <span>To edit this service configuration, </span>
                <Link 
                    style={{ display: "inline-block" }} 
                    url={"/Admin/Services/" + encodeURIComponent(serviceId)} text="click here" 
                    onClick={() => {
                        props.onDismiss();
                    }}
                />
                <span>.</span>
            </div>
            <div style={{ "padding": theme.spacing(2), maxHeight: "70vh", overflow: "auto" }}>   
                <ObjectResponsive
                    object={service}
                    spacing={theme.spacing(1)}
                    filterOptions={{
                        filterProperties: [
                            "status", "statusDescription", "startup", "startTime", 
                            "serviceType", "statusMessage", "statusDebug"
                        ],
                        filterValuedProperties: true
                    }} 
                />                                                                
            </div>                                  
            <div style={{ display: "flex", width: "100%" }} >
                <div style={{ marginRight: theme.spacing(1) }}>
                    <Button
                        color="primary" 
                        variant="contained"
                        label="Start"
                        disabled={!canStart}
                        onClick={startService}
                    />
                </div>
                <div style={{ marginRight: theme.spacing(1) }}>
                    <Button                            
                        label="Stop"
                        disabled={!canStop}
                        onClick={stopService}
                    />
                </div>
                <div style={{ marginRight: theme.spacing(1) }}>
                    <Button                               
                        label="Restart"
                        disabled={!canRestart}
                        onClick={restartService}
                    />
                </div>                    
                <div style={{ flex: 1 }} />
                <div>  
                    <Button 
                        label="Close"
                        onClick={props.onDismiss}
                    />                  
                </div>    
            </div>                
        </div>
    );  
};

export default ServiceDetails;
