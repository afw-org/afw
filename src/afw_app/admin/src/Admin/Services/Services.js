// See the 'COPYING' file in the project root for licensing information.
import {useMemo, useReducer} from "react";
import {Switch, Route, useHistory} from "react-router";

import {
    Breadcrumb,
    Button,
    Dialog,
    Link,
    Message,
    Spinner,
    Table,
    TextField,
    Toggle,
    Toolbar,
    Typography,
    useValues,
    useServiceConfigurations
} from "@afw/react";
import {afwServiceStart, afwServiceRestart, afwServiceStop} from "@afw/core";

import {ContextualHelpButton, ContextualHelp} from "../../common/ContextualHelp";
import {useApplication, useAppCore, useBreadcrumbs, useTheme} from "../../hooks";

import ServiceNew from "./ServiceNew";
import ServiceDetails from "./ServiceDetails";
import ServiceEditor from "./ServiceEditor";

import {ContextualHelpRoutes} from "./ContextualHelp";


const breadcrumbsRoot = { text: "Admin", link: "/Admin" };

const initialState = {
    showServiceName:    false,
    showAdapters:       true,
    showLogs:           true,
    showAuthHandlers:   true,
    filter:             "",
    serviceDetailsOpen: false,
    showHelp:           false,
};

const reducer = (state, action) => {

    switch (action.type) {

    case "SERVICE_NEW_CLICK":
        return {
            ...state,
            showServiceNew: true,
        };

    case "SERVICE_NEW_DISMISS":
        return {
            ...state,
            showServiceNew: false,
        };

    case "SERVICE_NEW":
        return {
            ...state,
            showServiceNew: false,
            newServiceName: action.newService,
            showStartService: action.newService.getPropertyValue("startup") === "immediate" ? true : false,
        };

    case "TOGGLE_ADAPTERS":
        return {
            ...state,
            showAdapters: action.showAdapters,
        };

    case "TOGGLE_LOGS":
        return {
            ...state,
            showLogs: action.showLogs,
        };

    case "TOGGLE_AUTH_HANDLERS":
        return {
            ...state,
            showAuthHandlers: action.showAuthHandlers,
        };

    case "FILTER_CHANGE":
        return {
            ...state,
            filter: action.filter,
        };

    case "SERVICE_DETAILS_OPEN":
        return {
            ...state,
            serviceDetailsOpen: true,
        };

    case "SERVICE_DETAILS_DISMISS":
        return {
            ...state,
            serviceDetailsOpen: false,
        };

    case "SERVICE_ROW_INVOKED":
        return {
            ...state,
            serviceDetailsOpen: true,
            selectedService: action.selectedService,
        };

    case "SERVICES_RELOADED":
        return {
            ...state,
            selectedService: null,
        };        

    case "SELECTION_CHANGED":
        return {
            ...state,
            selectedService: action.selectedService,
        };

    case "STARTING_SERVICE":
        return {
            ...state,
            spinnerMessage: "Starting service...",
            showStartService: false,
        };

    case "STARTING_SERVICE_SUCCESS":
        return {
            ...state,
            spinnerMessage: null,
        };

    case "DELETE_SERVICE_STOPPING":
        return {
            ...state,
            spinnerMessage: "Stopping service " + action.serviceId + "...",
        };

    case "DELETE_SERVICE_STOPPED":
        return {
            ...state,
            spinnerMessage: null,
        };

    case "STOPPING_SERVICE":
        return {
            ...state,
            spinnerMessage: "Stopping service...",
        };

    case "STOPPING_SERVICE_SUCCESS":
        return {
            ...state,
            spinnerMessage: null,
        };

    case "RESTARTING_SERVICE":
        return {
            ...state,
            spinnerMessage: "Restarting service...",            
        };

    case "RESTARTING_SERVICE_SUCCESS":
        return {
            ...state,
            spinnerMessage: null,
        };        

    case "ERROR":
        return {
            ...state,
            spinnerMessage: null,
            error: action.error,
        };

    case "CONFIRM_DELETE":
        return {
            ...state,
            showConfirmDeleteDialog: true,
        };

    case "CONFIRM_DELETE_DISMISS":
        return {
            ...state,
            showConfirmDeleteDialog: false,
        };        

    case "DELETING":
        return {
            ...state,
            showConfirmDeleteDialog: false,
        };

    case "SERVICE_START_NO":
        return {
            ...state,
            showStartService: false,
        };
    
    case "HELP_OPEN":
        return {
            ...state,
            showHelp: true,
        };

    case "HELP_DISMISS":
        return {
            ...state,
            showHelp: false,
        };


    default:
        return state;

    }
};

/**
 * The Services component renders a table of available _AdaptiveService_ objects,
 * where the user can select, edit, start, stop and delete services.
 * 
 * This is rendered on the route /Admin/Services.
 */
export const Services = () => {

    const [state, dispatch] = useReducer(reducer, initialState);

    const {
        showServiceNew,
        newService,
        showAdapters,
        showLogs,
        showAuthHandlers,
        filter,
        serviceDetailsOpen,
        selectedService,
        spinnerMessage,
        showConfirmDeleteDialog,
        showStartService,
        showHelp,
    } = state;
        
    const theme = useTheme();
    const {client, notification} = useApplication();
    const {application, reloadServices, services, environment} = useAppCore();    
    const {confAdapterId} = useValues(application);    
    const {
        serviceConfigs, isLoading, 
        onRefresh : reloadServiceConfigs, 
        error : serviceConfigError
    } = useServiceConfigurations({ adapterId: confAdapterId });
    const history = useHistory();
    const {serviceId, canStart, canStop, canRestart, uriServiceConf} = useValues(selectedService);    
    const breadcrumbItems = useBreadcrumbs(breadcrumbsRoot);
    
    const filteredServices = useMemo(() => {
        if (services) {
            return services.filter(service => {
                const {serviceType, confId} = service.getPropertyValues();

                if (filter && confId.toLowerCase().indexOf(filter.toLowerCase()) < 0)
                    return false;

                if (showAdapters && serviceType === "adapter")
                    return true;
                else if (showLogs && serviceType === "log")
                    return true;
                else if (showAuthHandlers && serviceType === "authorizationHandler")
                    return true;
                
                return false;
            }).sort((service1, service2) => {
                let type1 = service1.getPropertyValue("serviceType");
                let type2 = service2.getPropertyValue("serviceType");

                if (type1 === type2)
                    return service1.getPropertyValue("confId").toLowerCase().localeCompare(
                        service2.getPropertyValue("confId").toLowerCase());
                else
                    return type1.toLowerCase().localeCompare(type2.toLowerCase());
            });
        } else return [];
    }, [services, filter, showAdapters, showLogs, showAuthHandlers]);
    
    const onReloadServices = async () => {       
        await reloadServiceConfigs();
        await reloadServices();        

        dispatch({ type: "SERVICES_RELOADED" });      
    };
    
    const startService = async (selectedService) => {
        try {
            dispatch({ type: "STARTING_SERVICE" });
            let result = await afwServiceStart(client, selectedService ? selectedService.getPropertyValue("serviceId") : serviceId).result();            
            dispatch({ type: "STARTING_SERVICE_SUCCESS" });

            const dialogMessage = result["statusDescription"];
            notification({ message: dialogMessage, type: "success", duration: 3000 });
        } catch (error) {            
            dispatch({ type: "ERROR", error });    
            notification({ message: error, type: "error" });         
        }   

        await onReloadServices();   
    };

    const stopService = async (selectedService) => {                
        try {
            dispatch({ type: "STOPPING_SERVICE" });
            let result = await afwServiceStop(client, selectedService ? selectedService.getPropertyValue("serviceId") : serviceId).result();            
            dispatch({ type: "STOPPING_SERVICE_SUCCESS" });

            const dialogMessage = result["statusDescription"];

            notification({ message: dialogMessage, type: "success", duration: 3000 });
        } catch (error) {
            dispatch({ type: "ERROR", error });
            notification({ message: error, type: "error" });
        }

        await onReloadServices();   
    };

    const restartService = async (selectedService) => {    
        try {
            dispatch({ type: "RESTARTING_SERVICE" });
            const result = await afwServiceRestart(client, selectedService ? selectedService.getPropertyValue("serviceId") : serviceId).result();
            dispatch({ type: "RESTARTING_SERVICE_SUCCESS" });

            const dialogMessage = result["statusDescription"];

            notification({ message: dialogMessage, type: "success", duration: 3000 });
        } catch (error) {
            dispatch({ type: "ERROR", error });
            notification({ message: error, type: "error" });
        }

        await onReloadServices();   
    };

    const onConfirmDelete = async () => {

        dispatch({ type: "DELETING" });
        
        try {
            /* if it's running, stop it first */
            if (canStop) {
                dispatch({ type: "DELETE_SERVICE_STOPPING", serviceId });
                await afwServiceStop(client, serviceId).result();                
                dispatch({ type: "DELETE_SERVICE_STOPPED", serviceId });
            }

            /* find the service config to delete */                                                            
            const matchingServiceConf = serviceConfigs.filter(serviceConf => serviceConf.getPath() === uriServiceConf);                   
            if (matchingServiceConf.length > 0) {
                const response = matchingServiceConf[0].delete();           
                await response.result();
                
                notification({ message: "Service deleted successfully.", type: "success", duration: 3000 });
            } else {
                /* can't find the service configuration to remove */
                notification({ message: "Unable to delete service:  cannot determine service configuration location.", type: "error" });
            }
        } catch (error) {            
            notification({ message: "Unable to delete service: " + error, type: "error" });
        }
        
        await onReloadServices();   
    };

    const onSelectionChanged = (selection) => {   
        let selectedService;

        if (selection.length > 0) {
            selectedService = selection[0];            
        }

        dispatch({ type: "SELECTION_CHANGED", selectedService });          
    };

    const onNewService = async (newService) => {
        
        dispatch({ type: "SERVICE_NEW", newService });

        notification({ message: "Service created successfully.", type: "success", duration: 3000 });
        
        if (newService.getPropertyValue("startup") !== "immediate") {
            await onReloadServices();   
        }
    };

    if (isLoading) 
        return <Spinner size="large" label="Loading Service Configurations.." fullScreen={true} />;          

    return (
        <div id="admin-admin-services" data-testid="admin-admin-services"  style={{ display: "flex", flexDirection: "column", height: "100%" }}>
            <div style={{ display: "flex", alignItems: "center", paddingBottom: theme.spacing(2) }}>
                <div style={{ flex: 1 }}>
                    <Breadcrumb items={breadcrumbItems} />  
                </div>                 
                <ContextualHelpButton showHelp={() => dispatch({ type: "HELP_OPEN" })} />                
            </div>
            {
                spinnerMessage && <Spinner size="large" label={spinnerMessage} fullScreen={true} />
            }
            {
                serviceConfigError && 
                    <Message status="error" message={serviceConfigError.message} />
            }
            <div style={{ flex: 1, overflow: "auto" }}>
                <Switch>
                    <Route exact path="/Admin/Services/" render={(props) => 
                        <div style={{ display: "flex", flexDirection: "column", height: "100%" }}>                        
                            <div style={{ paddingBottom: theme.spacing(0.5) }}>
                                <Toolbar
                                    contains={[
                                        <Button 
                                            data-testid="ServiceNewBtn"
                                            variant="text" 
                                            color="primary"
                                            key="new" 
                                            icon="add" 
                                            label="New" 
                                            onClick={() => dispatch({ type: "SERVICE_NEW_CLICK" })}
                                            disabled={!environment} 
                                        />,
                                        <Button 
                                            data-testid="ServiceEditBtn"
                                            variant="text" 
                                            key="new" 
                                            icon="edit" 
                                            label="Edit" 
                                            onClick={() => history.push("/Admin/Services/" + serviceId + "#edit")} 
                                            disabled={!selectedService || !selectedService.getPropertyValue("uriServiceConf")} 
                                        />,
                                        <Button 
                                            data-testid="ServiceDeleteBtn"
                                            variant="text" 
                                            key="delete" 
                                            icon="delete" 
                                            label="Delete" 
                                            onClick={() => dispatch({ type: "CONFIRM_DELETE" })}
                                            disabled={!selectedService} 
                                        />,
                                        <Button 
                                            data-testid="ServiceReloadBtn"
                                            variant="text" 
                                            icon="refresh" 
                                            label="Refresh"
                                            onClick={onReloadServices} 
                                        />
                                    ]}
                                />                                
                            </div>
                            <div style={{ marginLeft: theme.spacing(1), display: "flex" }}>
                                <div style={{ display: "flex", marginRight: theme.spacing(1) }}>
                                    <div style={{ marginRight: theme.spacing(1) }}>
                                        <Toggle 
                                            label="Adapters" 
                                            value={showAdapters} 
                                            onChanged={showAdapters => dispatch({ type: "TOGGLE_ADAPTERS", showAdapters })}
                                        />
                                    </div>
                                    <div style={{ marginRight: theme.spacing(1) }}>
                                        <Toggle 
                                            label="Logs" 
                                            value={showLogs} 
                                            onChanged={showLogs => dispatch({ type: "TOGGLE_LOGS", showLogs })}
                                        />
                                    </div>
                                    <div style={{ marginRight: theme.spacing(1) }}>
                                        <Toggle 
                                            label="Auth" 
                                            value={showAuthHandlers} 
                                            onChanged={showAuthHandlers => dispatch({ type: "TOGGLE_AUTH_HANDLERS", showAuthHandlers })}
                                        />
                                    </div>
                                </div>
                                <div>
                                    <TextField 
                                        value={filter}
                                        onChanged={filter => dispatch({ type: "FILTER_CHANGED", filter })}
                                        placeholder="Filter"
                                    />
                                </div>
                            </div>
                            <div style={{ flex: 1 }}>
                                <div style={{ display: "flex", flexDirection: "column", height: "100%" }}>
                                    <div style={{ flex: 1, overflow: "auto" }}>
                                        <Table 
                                            rows={filteredServices}
                                            columns={[
                                                { 
                                                    key: "id", name: "Service Id", isResizable: true, minWidth: 150, maxWidth: 150,
                                                    style: { wordWrap: "break-word" },
                                                    onRender: (service) => {
                                                        const {confId, serviceId, uriServiceConf} = service.getPropertyValues();                                                        
                                                        let url = props.match.url + "/" + serviceId;                                                        
                                
                                                        return (                            
                                                            uriServiceConf ?
                                                                <Link style={{ display: "inline-block" }} url={url} text={confId} /> :
                                                                <span>{confId}</span>                            
                                                        );
                                                    }
                                                },
                                                { 
                                                    key: "type", name: "Type", isResizable: true, minWidth: 150, maxWidth: 150,
                                                    style: { wordWrap: "break-word" },
                                                    onRender: (item) => {
                                                        const {serviceType, confSubtype} = item.getPropertyValues();                                                                                                              
                                                        
                                                        if (serviceType)
                                                            return <span>{serviceType + "/" + confSubtype}</span>;
                                                        else
                                                            return <span>Unknown</span>;
                                                    }
                                                },                
                                                { 
                                                    key: "status", name: "Status", isResizable: true, minWidth: 100, maxWidth: 250, isMultiline: true,
                                                    style: { wordWrap: "break-word" },
                                                    onRender: (item) => {
                                                        const {status, serviceType, uriRelated} = item.getPropertyValues();    

                                                        const id = uriRelated ? uriRelated.split("/")[3] : undefined;

                                                        if (id && status === "running") {
                                                            if (serviceType === "adapter")
                                                                return <Link url={"/Admin/Adapters/" + encodeURIComponent(id)} text={status}/>;
                                                            else if (serviceType === "log")
                                                                return <Link url={"/Admin/Logs/" + encodeURIComponent(id)} text={status}/>;
                                                            else if (serviceType === "authorizationHandler")
                                                                return <Link url={"/Admin/AuthHandlers/" + encodeURIComponent(id)} text={status}/>;
                                                        }

                                                        return status;
                                                    }
                                                },
                                                {
                                                    key: "description", name: "Service Description", isResizable: true, minWidth: 350, maxWidth: 400, isMultiline: true,
                                                    style: { wordWrap: "break-word" },
                                                    onRender: (service) => {
                                                        const {uriServiceConf} = service.getPropertyValues();    
                                                        
                                                        /* find the serviceConfig that matches it */
                                                        const matchingServiceConf = serviceConfigs ? serviceConfigs.filter(serviceConf => serviceConf.getPath() === uriServiceConf) : "";
                                
                                                        return (                            
                                                            matchingServiceConf[0] ? matchingServiceConf[0].getPropertyValue([ "conf", "description" ]) : ""
                                                        );
                                                    }
                                                }
                                            ]}
                                            selectionMode="single"
                                            onSelectionChanged={onSelectionChanged}    
                                            onRowInvoked={(selectedService) => dispatch({ type: "SERVICE_ROW_INVOKED", selectedService })}
                                            compact={true}
                                        />                                
                                    </div>
                                    {
                                        showServiceNew &&
                                            <ServiceNew
                                                open={showServiceNew}
                                                onDismiss={() => dispatch({ type: "SERVICE_NEW_DISMISS" })}
                                                onSave={onNewService}
                                            />         
                                    }               
                                    <Dialog 
                                        open={showConfirmDeleteDialog}
                                        onDismiss={() => dispatch({ type: "CONFIRM_DELETE_DISMISS" })}
                                        title="Delete Service"
                                        subText="Delete the selected service?"
                                        footer={
                                            <div style={{ display: "flex", justifyContent: "flex-end", padding: theme.spacing(0.5) }}>
                                                <Button 
                                                    onClick={() => dispatch({ type: "CONFIRM_DELETE_DISMISS" })}
                                                    label="Cancel" 
                                                    size="small"
                                                />
                                                <Button 
                                                    style={{ marginLeft: theme.spacing(1) }}
                                                    color="primary"
                                                    variant="contained"
                                                    onClick={onConfirmDelete} 
                                                    label="Delete" 
                                                    aria-label="Confirm Delete"
                                                    size="small"
                                                />                                                
                                            </div>
                                        }
                                    />              
                                    <div style={{ padding: theme.spacing(3) }}>
                                        { selectedService && 
                                        <div style={{ display: "flex" }}>
                                            <div>
                                                <Button
                                                    color="primary" 
                                                    variant="contained"
                                                    label="Start"
                                                    disabled={!canStart}
                                                    onClick={() => startService(selectedService)}
                                                />
                                            </div>
                                            <div style={{ marginLeft: theme.spacing(1) }}>
                                                <Button                            
                                                    label="Stop"
                                                    disabled={!canStop}
                                                    onClick={() => stopService(selectedService)}
                                                />
                                            </div>
                                            <div style={{ marginLeft: theme.spacing(1) }}>
                                                <Button                               
                                                    label="Restart"
                                                    disabled={!canRestart}
                                                    onClick={() => restartService(selectedService)}
                                                />
                                            </div>
                                        </div>                            
                                        }
                                    </div>
                                </div>                                
                            </div>                            
                        </div>
                    } />
                    <Route path="/Admin/Services/:serviceId" render={(props) => {                    
                        let selectedService;                        

                        filteredServices.forEach((service) => {
                            if (service.getPropertyValue("serviceId") === decodeURIComponent(props.match.params.serviceId))
                                selectedService = service;
                        });
                        
                        /* select the appropriate service layout */
                        return (
                            <div style={{ display: "flex", height: "100%", flexDirection: "column" }}>
                                <div style={{ flex: 1, overflow: "auto" }}>
                                    <ServiceEditor 
                                        service={selectedService}
                                        startService={startService}
                                        restartService={restartService}
                                        stopService={stopService}
                                        reloadServices={reloadServices}
                                    />
                                </div>
                            </div>
                        );
                        
                    }} />
                </Switch>    
                <Dialog                     
                    open={serviceDetailsOpen}
                    blocking={false}
                    showClose={true}
                    maxWidth="md"
                    contains={
                        <div style={{ padding: theme.spacing(2) }}>
                            <ServiceDetails 
                                service={selectedService} 
                                onDismiss={() => dispatch({ type: "SERVICE_DETAILS_DISMISS" })}
                            />
                        </div>
                    }
                    onDismiss={() => dispatch({ type: "SERVICE_DETAILS_DISMISS" })}
                />      
                <Dialog 
                    open={showStartService}
                    blocking={true}
                    maxWidth="sm"
                    title="Start Service"
                    contains={
                        <Typography text="Start service now?" />
                    }                    
                    footer={
                        <div style={{ display: "flex", width: "100%", padding: theme.spacing(2) }}>     
                            <Button 
                                style={{ marginRight: theme.spacing(1) }}
                                label="Yes"
                                color="primary"
                                onClick={() => startService(newService)}
                            />                       
                            <Button 
                                style={{ marginRight: theme.spacing(1) }}
                                label="No"
                                onClick={() => dispatch({ type: "SERVICE_START_DISMISS" })}
                            />                            
                        </div>
                    }
                />    
            </div> 
            <ContextualHelp 
                open={showHelp}
                onClose={() => dispatch({ type: "HELP_DISMISS" })}
                routes={ContextualHelpRoutes}
            /> 
        </div> 
    );
};

export default Services;
