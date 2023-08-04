// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect, useMemo} from "react";

import {    
    Button,
    ChoiceGroup,   
    CustomVariables,
    Dialog,
    ObjectEditor,
    ObjectResponsive,  
    PropertyResponsive,  
    Responsive,    
    Spinner,
    Tabs,    
    Typography,
    useModel,
    useValues,
    useOperational,
} from "@afw/react";

import {afwServiceStart, afwServiceRestart, afwServiceStop} from "@afw/core";

import {useAppCore, useApplication, useTheme} from "../../hooks";

import ServiceEditorAdaptor from "./ServiceEditorAdaptor";
import ServiceEditorAdaptorLmdb from "./ServiceEditorAdaptorLmdb";
import ServiceEditorAdaptorFile from "./ServiceEditorAdaptorFile";
import ServiceEditorAdaptorLdap from "./ServiceEditorAdaptorLdap";
import ServiceEditorAdaptorModel from "./ServiceEditorAdaptorModel";

import ServiceEditorAuthorizationHandler from "./ServiceEditorAuthorizationHandler";

import ServiceEditorLog from "./ServiceEditorLog";
import ServiceEditorLogSyslog from "./ServiceEditorLogSyslog";
import ServiceEditorLogStandard from "./ServiceEditorLogStandard";

/**
 * ServiceEditorGeneral
 * 
 * This component renders the "General" tab content for all Services.
 */
export const ServiceEditorGeneral = (props) => {

    const {service, serviceConf} = props;    

    const {startup} = useValues(serviceConf); 
    const {editable} = useOperational(props);
    const theme = useTheme();

    if (!serviceConf || !service)
        return null;
    
    if (editable) {
        return (
            <div style={{ padding: theme.spacing(2) }}>
                <PropertyResponsive  
                    property={serviceConf.getProperty("serviceId")}
                />  
                <div style={{ height: theme.spacing(5) }} />
                <ChoiceGroup                                
                    selectedKey={startup ? startup : "manual"}
                    options={[
                        { key: "immediate", text: "Immediate" },
                        { key: "manual", text: "Manual" },
                        { key: "disabled", text: "Disabled" },
                        { key: "permanent", text: "Permanent" },
                    ]}                                
                    label="Startup Condition"     
                    aria-label="Startup Condition"
                    onChanged={(startupOption) => {
                        serviceConf.setPropertyValue("startup", startupOption.key);
                    }}     
                    disabled={!editable}     
                    description={serviceConf.getProperty("startup").getBrief()}             
                /> 
            </div>
        );
    } else {
        return (
            <div style={{ padding: theme.spacing(2) }}>
                <ObjectResponsive 
                    object={service}
                    spacing={theme.spacing(2)}
                    filterOptions={{
                        filterValuedProperties: (editable ? false : true),
                    }}
                />
            </div>
        );
    }
};


/**
 * ServiceEditorConfiguration
 * 
 * This component will render the default Tabbed content for a Service's configuration.
 */
export const ServiceEditorConfiguration = (props) => {

    const {service, serviceConf, idPropertyName, subtypePropertyName} = props;
    const {editable} = useOperational(props);

    if (!service || !serviceConf)
        return null;

    const conf = serviceConf.getPropertyValue("conf");        

    return (
        <Responsive 
            spacing={8}
            items={[                                                     
                {
                    contains:
                        <ObjectResponsive 
                            {...props}
                            object={conf}                                                                     
                            embedded={{ component: "Paper" }}
                            preferredComponents={[                                                    
                                {
                                    componentProps: {
                                        breakpoints: {
                                            small: 12,
                                        }
                                    }
                                },
                                {
                                    dataType: "object",
                                    componentProps: {
                                        breakpoints: {
                                            small: 12
                                        }
                                    }
                                },
                            ]}                                        
                            sortOptions={{
                                sortByDataType: true
                            }}
                            filterOptions={{
                                filterProperties: [
                                    "title",
                                    idPropertyName,                                             
                                    "description"
                                ],
                                filterValuedProperties: (editable ? false : true),
                            }}                                                
                        />
                },                                                                                
                {
                    contains:
                        <ObjectResponsive 
                            {...props}
                            object={conf}                                                                     
                            embedded={{ component: "Paper" }}
                            preferredComponents={[                                                    
                                {
                                    componentProps: {
                                        breakpoints: {
                                            small: 12,
                                        }
                                    }
                                },
                                {
                                    dataType: "object",
                                    componentProps: {
                                        breakpoints: {
                                            small: 12
                                        }
                                    }
                                },
                            ]}                                        
                            sortOptions={{
                                sortByDataType: true
                            }}
                            filterOptions={{
                                filterExcludeProperties: [
                                    idPropertyName, 
                                    subtypePropertyName, 
                                    "title", 
                                    "description", 
                                    "type"
                                ],
                                filterValuedProperties: (editable ? false : true),
                            }}                                                
                        />
                }  
            ]}
        />    
    );
};



/**
 * ServiceEditorRuntime
 * 
 * This component will render a default runtime tab content for a Service.
 */
export const ServiceEditorRuntime = (props) => {

    const {serviceUriRelated} = props;
    const theme = useTheme(); 
    
    if (!serviceUriRelated)
        return null;              

    return (
        <div>
            <Typography size="7" text="Properties" />
            <div style={{  padding: theme.spacing(1) }}>
                <ObjectResponsive 
                    object={serviceUriRelated}
                    editable={false}
                    filterOptions={{
                        filterProperties: [
                            "referenceCount", "stopping", "metrics"                            
                        ],
                        filterValuedProperties: true,
                    }}
                    embedded={{
                        component: "None"
                    }}
                    spacing={theme.spacing(1)}
                />
            </div>
                         
        </div>
    );
};


/**
 * When no implementation exists for a service type/subtype, use this as a default.
 */
export const ServiceEditorDefault = (props) => {

    const {service, serviceConf, serviceUriRelated, confTypes} = props;    
    const {editable} = useOperational(props);

    if (!service || !serviceConf || !confTypes)
        return null;

    const conf = serviceConf.getPropertyValue("conf");
    if (!conf)
        return null;

    const type = conf.getPropertyValue("type");        
    const {idPropertyName, subtypePropertyName} = confTypes[type];    
    
    let qualifiers;

    const custom = conf.getProperty("custom") ? 
        ({
            text: "Custom",
            contains:
                <CustomVariables 
                    custom={conf.getProperty("custom")}
                    qualifiers={qualifiers}
                />
        }) : [];

    const runtime = editable ? [] :
        ([{
            text: "Runtime",
            contains: 
                <ServiceEditorRuntime 
                    service={service}
                    serviceUriRelated={serviceUriRelated}
                />
        }]);

    return (
        <Tabs 
            gapSpace={20}
            tabs={[
                {
                    text: "General",
                    contains: 
                        <ServiceEditorGeneral 
                            service={service}                                    
                            serviceConf={serviceConf}
                            idPropertyName={idPropertyName}
                            subtypePropertyName={subtypePropertyName}
                        />
                },
                {
                    text: "Configuration",                                
                    contains:
                        <ServiceEditorConfiguration 
                            service={service}
                            serviceConf={serviceConf}
                        />
                },
                ...(custom),
                ...(runtime),
            ]}
        />
    );
};


/**
 * ServiceEditor
 * 
 * Main component, which handles fetching service editor configuration, runtime
 * information and determining which child component to use for rendering.
 * 
 * It also handles the save/cancel buttons and saving out data back to the 
 * server.
 */
const ServiceEditor = (props) => {

    const [editable, setEditable] = useState(false);
    const [serviceUriRelated, setServiceUriRelated] = useState();
    const [serviceConf, setServiceConf] = useState();
    const [showRestartService, setShowRestartService] = useState(false);
    const [spinnerText, setSpinnerText] = useState();

    const {environment} = useAppCore();
    const model = useModel();
    const {notification, client} = useApplication();    
    const theme = useTheme();

    const {service, reloadServices} = props;
    const {uriServiceConf, uriRelated, serviceId, status, serviceType, confSubtype, canStart, canRestart, canStop} = useValues(service);

    useEffect(() => {
        let response;
        const loadServiceConfig = async () => {
            setSpinnerText("Loading Service Configuration...");
            try {     
                response = model.getObjectWithUri({
                    uri: uriServiceConf,
                    objectOptions: { reconcilable: true }
                });
                const serviceConf = await response.object;

                if (!response.controller.signal.aborted)
                    setServiceConf(serviceConf);
            // eslint-disable-next-line no-empty
            } catch (error) {
                if (response && !response.controller.signal.aborted)
                    notification({ message: error, type: "error" });
            }
            setSpinnerText();
        };

        if (notification && service && model && uriServiceConf)
            loadServiceConfig();

        return () => {
            if (response)
                response.controller.abort();
        };
    }, [service, model, uriServiceConf, notification]);

    const confTypes = useMemo(() => {
        if (environment) {
            let confTypes = {};            
                
            let serviceTypes = Object.values(environment.service_type);
            for (let serviceType of serviceTypes) {
                let confType = serviceType.confType;
                confTypes[serviceType.serviceType] = confType;             
            }

            return confTypes;
        }
    }, [environment]);

    useEffect(() => {
        let response;

        const loadRuntime = async () => {     
            try {                       
                response = model.getObjectWithUri({ uri: uriRelated });
                const serviceUriRelated = await response.object;                    

                if (!response.controller.signal.aborted)
                    setServiceUriRelated(serviceUriRelated);                
            } catch (error) {
                if (response && !response.controller.signal.aborted)
                    notification({ message: error, type: "error" });
            }
        };

        if (notification && service && serviceConf && uriRelated && (status === "running"))            
            loadRuntime();

        return () => { 
            if (response?.controller)
                response.controller.abort();
        };
    }, [serviceConf, service, model, status, uriServiceConf, uriRelated, notification]);

    if (!service)
        return null;

    const startService = async () => {
        try {
            setSpinnerText("Starting Service...");

            const result = await afwServiceStart(client, serviceId).result();            
            const dialogMessage = result["statusDescription"];

            notification({ message: dialogMessage, type: "success", duration: 3000 });
        } catch (error) {                       
            notification({ message: error, type: "error" });         
        }   

        await reloadServices();
        setSpinnerText();        
    };

    const stopService = async () => {        
        try {
            setSpinnerText("Stopping Service...");

            const result = await afwServiceStop(client, serviceId).result();            
            const dialogMessage = result["statusDescription"];

            notification({ message: dialogMessage, type: "success", duration: 3000 });
        } catch (error) {
            notification({message: error, type: "error"});
        }

        await reloadServices();
        setSpinnerText();
    };

    const restartService = async () => {    
        try {
            setSpinnerText("Restarting Service...");

            const result = await afwServiceRestart(client, serviceId).result();            
            const dialogMessage = result["statusDescription"];

            notification({ message: dialogMessage, type: "success", duration: 3000 });
        } catch (error) {
            notification({message: error, type: "error"});
        }
 
        await reloadServices();
        setSpinnerText();
    };   

    if (!spinnerText && !serviceConf) {
        /* If we're not waiting and have no serviceConf, then we don't have as service configuration for the user to show for it */
        return (
            <>
                <div style={{ height: "10vh" }} />
                <div style={{ textAlign: "center" }}>
                    <Typography text="This service has no configuration that can be viewed or edited here." />
                </div>
            </>            
        );
    }

    /* Choose the appropriate layout, based on the serviceType and confSubtype */
    let serviceEditor;
    const serviceProps = {
        service,
        serviceConf,
        serviceUriRelated,
        confTypes
    };
 
    if (serviceType === "adaptor" && confSubtype === "lmdb")
        serviceEditor = <ServiceEditorAdaptorLmdb  {...serviceProps} />;
 
    else if (serviceType === "adaptor" && confSubtype === "file")
        serviceEditor = <ServiceEditorAdaptorFile {...serviceProps} />;
 
    else if (serviceType === "adaptor" && confSubtype === "ldap")
        serviceEditor = <ServiceEditorAdaptorLdap {...serviceProps} />;
 
    else if (serviceType === "adaptor" && confSubtype === "model")
        serviceEditor = <ServiceEditorAdaptorModel {...serviceProps} />;
 
    else if (serviceType === "adaptor")
        serviceEditor = <ServiceEditorAdaptor {...serviceProps} />;
 
    else if (serviceType === "log" && confSubtype === "syslog")
        serviceEditor = <ServiceEditorLogSyslog {...serviceProps} />;
 
    else if (serviceType === "log" && confSubtype === "standard")
        serviceEditor = <ServiceEditorLogStandard {...serviceProps} />;
 
    else if (serviceType === "log")
        serviceEditor = <ServiceEditorLog {...serviceProps} />;
 
    else if (serviceType === "authorizationHandler")
        serviceEditor = <ServiceEditorAuthorizationHandler {...serviceProps} />;
 
    else
        serviceEditor = <ServiceEditorDefault {...serviceProps} />;
    
    return ( 
        <>
            <ObjectEditor 
                object={serviceConf}
                onEditable={setEditable}
                onSaved={() => {
                    notification({ message: "Service Configuration saved.", type: "success" });

                    if (canRestart === true)
                        setShowRestartService(true);
                }}
                layout={
                    <div style={{ display: "flex", flexDirection: "column", width: "100%", height: "100%" }}>
                        <div style={{ flex: 1 }}>
                            {serviceEditor}
                        </div>
                        { !editable &&
                            <div style={{ display: "flex", padding: theme.spacing(2) }}>
                                <div style={{ marginRight: theme.spacing(0.5) }}>
                                    <Button
                                        color="primary" 
                                        variant="contained"
                                        label="Start"
                                        disabled={!canStart}
                                        onClick={() => startService()}
                                    />
                                </div>
                                <div style={{ marginRight: theme.spacing(0.5) }}>
                                    <Button                            
                                        label="Stop"
                                        disabled={!canStop}
                                        onClick={() => stopService()}
                                    />
                                </div>
                                <div>
                                    <Button                               
                                        label="Restart"
                                        disabled={!canRestart}
                                        onClick={() => restartService()}
                                    />
                                </div>
                            </div>  
                        }
                    </div>
                }
            />   
            { spinnerText &&
                <Spinner 
                    fullScreen={true}
                    size="large"
                    label={spinnerText}
                />   
            }       
            <Dialog 
                open={showRestartService}
                blocking={false}
                maxWidth="sm"
                title="Restart Service"
                contains={
                    <Typography text="Restart service now?" />
                }
                footer={
                    <div style={{ display: "flex", width: "100%", padding: theme.spacing(2) }}>     
                        <Button 
                            style={{ marginRight: theme.spacing(1) }}
                            label="Yes"
                            color="primary"
                            variant="contained"
                            onClick={() => {
                                restartService();
                                setShowRestartService(false);                                
                            }}
                        />                       
                        <Button 
                            style={{ marginRight: theme.spacing(1) }}
                            label="No"
                            onClick={() => setShowRestartService(false)}
                        />                            
                    </div>
                }
            />
        </>
    );
};

export default ServiceEditor;
