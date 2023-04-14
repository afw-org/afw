// See the 'COPYING' file in the project root for licensing information.
import {
    Button,
    Typography
} from "@afw/react";

import {useTheme} from "../../hooks";

const HelpText = ({ text }) => {
    return (
        <Typography 
            color="textSecondary"
            size="3"
            style={{ whiteSpace: "pre-line" }}
            paragraph={true}
            text={text}
        />
    );
};

export const ServicesHelp = () => {

    const theme = useTheme();

    return (
        <>
            <HelpText text={` \
                Adaptive Services allow you to create a configuration for an Service Type (Adaptor, Log or Authorization Handler), along with \
                an identifier and startup condition, and make themselves known to Adaptive Framework Core.  This page allows you to \
                manage existing Adaptive Services or create new ones.
                
                On this page, you'll find a list of the currently known Adaptive Services, along with their service configuration type \
                and status.  By selecting one of these services from the table, you may be able to Start, Stop or Restart \
                the service, if its configuration permits. 

                The toolbar along the top allows you to perform a variety of operations on new or existing services.
            `} />

            <div style={{ display: "flex", justifyContent: "center", margin: theme.spacing(1) }}>
                <div style={{ marginRight: theme.spacing(0.5) }}>
                    <Button label="New" icon="add" variant="text" />
                </div>
                <div style={{ marginRight: theme.spacing(0.5) }}>
                    <Button label="Edit" icon="edit" variant="text" />
                </div>                
                <div style={{ marginRight: theme.spacing(0.5) }}>
                    <Button label="Delete" icon="delete" variant="text" />
                </div>
                <div style={{ marginRight: theme.spacing(0.5) }}>
                    <Button label="Refresh" icon="refresh" variant="text" />
                </div>                
            </div>            
            <HelpText text={` \
                To select a service, click on the row containing the service, or select the check mark next to the row containing the service. \
                Once selected, you may use the Edit, or Delete buttons to modify the service, or delete it. 

                Additionally, three more action buttons will be visible at the bottom, allowing you to Start, Stop or Restart the service, if \
                the framework allows.

            `} />
            <div style={{ display: "flex", justifyContent: "center" }}>
                <div style={{ marginRight: theme.spacing(0.5) }}>
                    <Button label="Start" />
                </div>
                <div style={{ marginRight: theme.spacing(0.5) }}>
                    <Button label="Stop" />
                </div>                
                <div style={{ marginRight: theme.spacing(0.5) }}>
                    <Button label="Restart" />
                </div>              
            </div>
        </>        
    );
};

const ServiceEditorHelp = () => {
    return (
        <>
            <HelpText text={` \
                The Service Editor allows you to view or edit a specific service.  General properties are properties that apply to all \
                services.  Configuration properties are specific to this service type.  Runtime properties are read-only properties that \
                are associated with the runtime instance of the service, if it's currently started.
            `} />
        </>
    );
};


export const ContextualHelpRoutes = [
    
    { path: "/Admin/Services/:serviceId",   title: "Service Editor Help",   Component: ServiceEditorHelp    },
    { path: "/Admin/Services",              title: "Services Help",         Component: ServicesHelp         },

];

export default ContextualHelpRoutes;
