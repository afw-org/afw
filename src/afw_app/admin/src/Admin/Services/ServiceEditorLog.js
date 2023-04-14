// See the 'COPYING' file in the project root for licensing information.
import {    
    CustomVariables,
    ObjectResponsive,    
    PropertyResponsive,
    Responsive,    
    Tabs, 
} from "@afw/react";

import {useTheme} from "../../hooks";

import {
    ServiceEditorGeneral, 
    ServiceEditorRuntime,
    ServiceEditorConfiguration
} from "./ServiceEditor";

/**
 * ServiceNewLog
 * 
 * This is a common component, used by the ServiceNew stepper.  It provides a
 * layout for the General configuration for type=log services.
 */
export const ServiceNewLog = (props) => {
    
    const confObject = props.confObject;
    if (!confObject)
        return null;

    return (
        <div>
            <Responsive 
                rows={[
                    {
                        columns: [
                            {
                                contains: 
                                    <ObjectResponsive 
                                        object={confObject}
                                        filterOptions={{
                                            filterProperties: [
                                                "crit",
                                                "alert",
                                                "emerg",
                                                "err",
                                                "warning",
                                                "notice",
                                                "info",
                                                "debug",
                                            ]      
                                        }}                                      
                                        preferredComponents={[{
                                            componentProps: { showDescriptions: false }
                                        }]}  
                                        spacing="0"
                                    />
                            },
                            {
                                contains: 
                                    <ObjectResponsive 
                                        object={confObject}
                                        filterOptions={{
                                            filterProperties: [
                                                "trace1",
                                                "trace2",
                                                "trace3",
                                                "trace4",
                                                "trace5",
                                                "trace6",
                                                "trace7",
                                                "trace8",
                                            ]
                                        }}
                                        preferredComponents={[{
                                            componentProps: { showDescriptions: false }
                                        }]}  
                                        spacing="0"
                                    />
                            }
                        ]
                    },
                    {
                        columns: [
                            {
                                contains: 
                                    <PropertyResponsive 
                                        property={confObject.getProperty("format")}
                                    />
                            }
                        ]
                    },
                    {
                        columns: [
                            {
                                contains: 
                                    <PropertyResponsive 
                                        property={confObject.getProperty("filter")}
                                    />
                            }
                        ]
                    }
                ]}
            />
        </div>
    );
};


/**
 * ServiceEditorLog
 * 
 * Main component for displaying the editor for a LMDB Service.
 */
export const ServiceEditorLog = (props) => {
   
    const theme = useTheme();
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
                        <ServiceEditorConfiguration 
                            service={service}                                    
                            serviceConf={serviceConf}
                            idPropertyName={idPropertyName}
                            subtypePropertyName={subtypePropertyName}
                        />    
                },                        
                {
                    text: "Priorities",
                    contains: 
                        <Responsive                                     
                            rows={[
                                {
                                    columns: [
                                        {
                                            breakpoints: {
                                                
                                            },
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
                                                                            ]
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
                                                                            ]
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

export default ServiceEditorLog;
