// See the 'COPYING' file in the project root for licensing information.
import {
    AdaptorDropdown,
    Dropdown,
    Tabs,
} from "@afw/react";

import {useAppCore, useTheme} from "../../hooks";

import {
    ServiceEditorGeneral, 
    ServiceEditorRuntime,
    ServiceEditorConfiguration
} from "./ServiceEditor";

/**
 * ServiceNewAdaptor
 * 
 * This is a common component, used by the ServiceNew stepper.  It provides a
 * layout for the General configuration for type=adaptor services.
 */
export const ServiceNewAdaptor = (props) => {

    const {authHandlers} = useAppCore();
    const theme = useTheme();

    const confObject = props.confObject;
    if (!confObject)
        return null;

    let authorizationHandler = confObject.getProperty("authorizationHandlerId");        
    let journalAdaptor = confObject.getProperty("journalAdaptorId");        

    return (
        <div>
            <Dropdown       
                id="authorization-handler-dropdown"     
                aria-label={authorizationHandler.getLabel()}
                label={authorizationHandler.getLabel()}
                required={authorizationHandler.getRequired()}
                options={
                    authHandlers.map(authHandler => ({
                        key: authHandler.authorizationHandlerId,
                        text: authHandler.authorizationHandlerId,
                    }))    
                }
                onChanged={(id) => authorizationHandler.setValue(id.key)}
                description={authorizationHandler.getDescription()}
                selectedKey={authorizationHandler.getValue()}
                disabled={(!authHandlers || authHandlers.length === 0)}
            />
            <div style={{ height: theme.spacing(5) }} />
            <AdaptorDropdown 
                id="journal-adaptor-dropdown"
                label={journalAdaptor.getLabel()}
                description={journalAdaptor.getDescription()}
                required={journalAdaptor.getRequired()}
                value={journalAdaptor.getValue()}
                onChanged={(journalAdaptorId) => journalAdaptor.setValue(journalAdaptorId)}
                allowNone={true}
            />
            <div style={{ height: theme.spacing(5) }} />
        </div>
    );
};


/**
 * ServiceEditorAdaptor
 * 
 * Main component for displaying the editor for a LMDB Service.
 */
const ServiceEditorAdaptor = (props) => {
         
    const {service, serviceConf, serviceUriRelated, confTypes} = props;        
    if (!service || !serviceConf || !confTypes)
        return null;

    const conf = serviceConf.getPropertyValue("conf");
    if (!conf)
        return null;

    const type = conf.getPropertyValue("type");    
    const idPropertyName = confTypes[type].idPropertyName;
    const subtypePropertyName = confTypes[type].subtypePropertyName;

    return (            
        <div>
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
                                idPropertyName={idPropertyName}
                                subtypePropertyName={subtypePropertyName}
                            />                   
                            
                    },                                                
                    {
                        text: "Runtime",
                        contains:
                            <ServiceEditorRuntime 
                                serviceUriRelated={serviceUriRelated}
                            />
                    }                        
                ]}
            />                                
        </div>                            
    );
};

export default ServiceEditorAdaptor;
