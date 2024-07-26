// See the 'COPYING' file in the project root for licensing information.
import {
    CustomVariables,
    ObjectResponsive,
    Tabs,
    Typography,
    useOperational
} from "@afw/react";

import {useTheme} from "../../hooks";

import {
    ServiceEditorGeneral, 
    ServiceEditorRuntime
} from "./ServiceEditor";

/**
 * ServiceNewAdapterLdap
 * 
 * This is a common component, used by the ServiceNew stepper.  It provides a
 * layout for the General configuration for type=adapter, subtype=lmdb services.
 */
export const ServiceNewAdapterLdap = (props) => {    
    const {editable} = useOperational();

    const {confObject} = props;
    if (!confObject)
        return null;             

    return (
        <ObjectResponsive 
            object={confObject}  
            filterOptions={{
                filterExcludeProperties: [
                    "adapterId", 
                    "adapterType", 
                    "authorizationHandlerId", 
                    "custom",
                    "description",
                    "journalAdapterId",
                    "sourceLocation",
                    "title",
                    "type"                      
                ],
                filterValuedProperties: editable ? false : true
            }}                  
        />  
    );
};


/**
 * ServiceEditorAdapterLdap
 * 
 * Main component for displaying the editor for a LDAP Service.
 */
export const ServiceEditorAdapterLdap = (props) => {

    const {editable} = useOperational();
    const theme = useTheme();

    const {service, serviceConf, serviceUriRelated, confTypes} = props;        
    if (!service || !serviceConf || !confTypes)
        return null;

    const conf = serviceConf.getPropertyValue("conf");
    if (!conf)
        return null;

    const type = conf.getPropertyValue("type");
    const {idPropertyName, subtypePropertyName} = confTypes[type];

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
                            <div>
                                <Typography size="7" text="General Configuration" />
                                <div style={{ height: theme.spacing(2) }} />
                                <ObjectResponsive 
                                    object={conf}
                                    filterOptions={{
                                        filterProperties: [
                                            "description",
                                            "authorizationHandlerId",
                                            "journalAdapterId"
                                        ],
                                        filterValuedProperties: editable ? false : true
                                    }}                                   
                                />  
                                <div style={{ height: theme.spacing(5) }} />
                                <Typography size="7" text="LDAP Configuration" />
                                <div style={{ height: theme.spacing(2) }} />
                                <ServiceNewAdapterLdap 
                                    confObject={conf} 
                                />                     
                            </div>
                    },                        
                    ...runtime,
                    {
                        ...(
                            conf.getProperty("custom") ? 
                                {
                                    text: "Custom",
                                    contains:
                                        <CustomVariables 
                                            custom={conf.getProperty("custom")}
                                        />
                                } : {}
                        )
                    }
                ]}
            />                
        </div>                            
    );    
};

export default ServiceEditorAdapterLdap;
