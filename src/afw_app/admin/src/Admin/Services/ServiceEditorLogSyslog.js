// See the 'COPYING' file in the project root for licensing information.
import {    
    CustomVariables,
    Message,    
    ObjectResponsive,    
    Responsive,    
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
 * ServiceNewLogSyslog
 * 
 * This is a common component, used by the ServiceNew stepper.  It provides a
 * layout for the General configuration for type=adapter services.
 */
export const ServiceNewLogSyslog = (props) => {

    const {editable} = useOperational();
    
    const confObject = props.confObject;
    if (!confObject)
        return null;

    return (
        <div>
            <ObjectResponsive 
                object={confObject}
                filterOptions={{
                    filterProperties: [
                        "facility",
                        "ident",
                        "optionLogCons",
                        "optionLogNoDelay",
                        "optionLogNoWait",
                        "optionLogODelay",
                        "optionLogPError",
                        "optionLogPid"
                    ],
                    filterValuedProperties: editable ? false : true
                }}                
            />
        </div>
    );
};


/**
 * ServiceEditorLogSyslog
 * 
 * Main component for displaying the editor for a LMDB Service.
 */
const ServiceEditorLogSyslog = (props) => {

    const theme = useTheme();
    const {editable} = useOperational();    
       
    const {service, serviceConf, serviceUriRelated, confTypes} = props;        
    if (!service || !serviceConf || !confTypes)
        return null;

    const conf = serviceConf.getPropertyValue("conf");
    if (!conf)
        return null;

    const {type, custom} = conf.getPropertyValues();
    const {idPropertyName, subtypePropertyName} = confTypes[type];

    let qualifiers;       
    if (custom) { 
        if (!qualifiers["custom"])
            qualifiers["custom"] = {};           
        custom.getProperties().forEach((property) => {
            qualifiers["custom"][property.getName()] = property.getValue();
        });
    }

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
                        <div>    
                            <Typography size="7" text="General Configuration" />
                            <div style={{ height: theme.spacing(2) }} />
                            <ObjectResponsive 
                                object={conf}                
                                filterOptions={{
                                    filterProperties: [
                                        "description",
                                        "filter",
                                        "format"
                                    ],
                                    filterValuedProperties: editable ? false : true
                                }}
                            />            
                            <ServiceNewLogSyslog confObject={conf} />                                        
                        </div>
                },                        
                {
                    text: "Priorities",
                    contains: 
                        <>
                            <Message message="Select the priority levels to be captured for this logging service." />
                            <Responsive                                     
                                rows={[
                                    {
                                        columns: [
                                            { 
                                                contains:
                                                <Responsive 
                                                    rows={[
                                                        {
                                                            columns: [
                                                                {
                                                                    breakpoints: {
                                                                        small: 12,
                                                                        large: 6,
                                                                    },
                                                                    contains:
                                                                        <ObjectResponsive 
                                                                            object={conf}           
                                                                            filterOptions={{
                                                                                filterProperties: [
                                                                                    "emerg", 
                                                                                    "crit", 
                                                                                    "alert", 
                                                                                    "err", 
                                                                                    "warning", 
                                                                                    "notice", 
                                                                                    "info", 
                                                                                    "debug"
                                                                                ],
                                                                                filterValuedProperties: editable ? false : true
                                                                            }}                                                                                             
                                                                            componentProps={{ description: undefined }}            
                                                                            spacing={theme.spacing(1)}
                                                                        />
                                                                },
                                                                {
                                                                    breakpoints: {
                                                                        small: 12,
                                                                        large: 6,
                                                                    },
                                                                    contains:
                                                                        <ObjectResponsive 
                                                                            object={conf}     
                                                                            filterOptions={{
                                                                                filterProperties: [
                                                                                    "trace1", 
                                                                                    "trace2", 
                                                                                    "trace3", 
                                                                                    "trace4", 
                                                                                    "trace5", 
                                                                                    "trace6", 
                                                                                    "trace7", 
                                                                                    "trace8"
                                                                                ],
                                                                                filterValuedProperties: editable ? false : true
                                                                            }}                                                                                                   
                                                                            componentProps={{ description: undefined }}  
                                                                            spacing={theme.spacing(1)}
                                                                        />
                                                                },
                                                            ]
                                                        }
                                                    ]}
                                                /> 
                                            }, 
                                        ]
                                    }
                                ]}
                            />
                        </>
                },
                {
                    text: "Runtime",
                    contains:
                        <ServiceEditorRuntime 
                            serviceUriRelated={serviceUriRelated}
                        />
                },
                {
                    ...(
                        conf.getProperty("custom") ? 
                            {
                                text: "Custom",
                                contains:
                                    <CustomVariables 
                                        custom={conf.getProperty("custom")}
                                        qualifiers={qualifiers}
                                    />
                            } : {}
                    )
                }
            ]}
        />                 
    );    
};

export default ServiceEditorLogSyslog;
