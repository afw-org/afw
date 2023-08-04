// See the 'COPYING' file in the project root for licensing information.
import {
    ObjectResponsive,
    Tabs,
    useOperational
} from "@afw/react";

import {
    ServiceEditorGeneral, 
    ServiceEditorRuntime,
    ServiceEditorConfiguration
} from "./ServiceEditor";

/**
 * ServiceNewAuthorizationHandler
 * 
 * This is a common component, used by the ServiceNew stepper.  It provides a
 * layout for the General configuration for type=authorizationHandler services.
 */
export const ServiceNewAuthorizationHandler = (props) => {

    const confObject = props.confObject;
    if (!confObject)
        return null;

    return (
        <ObjectResponsive 
            object={confObject}
            filterOptions={{
                filterProperties: [
                    "allowDenyOverride",
                    "allowPermitOverride",
                    "priority",
                    "required",
                ]
            }}
        />
    );
};


/**
 * ServiceEditorAuthorizationHandler
 * 
 * Main component for displaying the editor for a LMDB Service.
 */
const ServiceEditorAuthorizationHandler = (props) => {
         
    const {service, serviceConf, serviceUriRelated, confTypes} = props;  
    const {editable} = useOperational(props);

    if (!service || !serviceConf || !confTypes)
        return null;

    const conf = serviceConf.getPropertyValue("conf");
    if (!conf)
        return null;

    const type = conf.getPropertyValue("type");
    
    const idPropertyName = confTypes[type].idPropertyName;
    const subtypePropertyName = confTypes[type].subtypePropertyName;

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
                    ...(runtime)
                ]}
            />                                
        </div>                            
    );
};

export default ServiceEditorAuthorizationHandler;
