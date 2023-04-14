// See the 'COPYING' file in the project root for licensing information.
import {
    CustomVariables,
    Dropdown,
    ObjectResponsive,
    Tabs,
    Typography,
    useForceUpdate,
    useOperational
} from "@afw/react";

import {useTheme} from "../../hooks";

import {
    ServiceEditorGeneral, 
    ServiceEditorRuntime
} from "./ServiceEditor";

const ContentType = (props) => {

    const {confObject} = props;

    const {editable} = useOperational(props);
    const forceUpdate = useForceUpdate();

    /*! \fixme maybe use possibleValues/allowedValues? */
    const contentTypeOptions = [
        {
            key: "json",
            text: "JSON",
        },
        {
            key: "xml",
            text: "XML",
        },
        {
            key: "yaml",
            text: "YAML"
        },
        {
            key: "ubjson",
            text: "UBJSON"
        }
    ];

    return (
        <Dropdown 
            id="service-new-contentType-dropdown"
            data-testid="ServiceNewContentTypeDropdown"
            required={confObject.getProperty("contentType").getRequired()}
            label={confObject.getProperty("contentType").getLabel()}
            aria-label={confObject.getProperty("contentType").getLabel()}
            description={confObject.getProperty("contentType").getDescription()}
            options={contentTypeOptions}
            selectedKey={confObject.getProperty("contentType").getValue()}
            onChanged={value => {
                confObject.setPropertyValue("contentType", value.key);
                forceUpdate();
            }}
            disabled={editable === false}
        />  
    );            
};

/**
 * ServiceNewAdaptorFile
 * 
 * This is a common component, used by the ServiceNew stepper.  It provides a
 * layout for the General configuration for type=adaptor, subtype=file services.
 */
export const ServiceNewAdaptorFile = (props) => {

    const theme = useTheme();
    const {editable} = useOperational(props);
    const {confObject} = props;

    if (!confObject)
        return null;

    return (
        <div>
            { 
                editable && <ContentType confObject={confObject} /> 
            }
            <div style={{ height: theme.spacing(2) }} />
            <ObjectResponsive                 
                object={confObject}  
                filterOptions={{
                    filterProperties: [    
                        ...(!editable ? ["contentType"] : []),                    
                        "isDevelopmentInput",
                        "isModelLocation",
                        "isPolicyLocation",                      
                        "filenameSuffix",
                        "root"
                    ],
                    filterValuedProperties: editable ? false : true
                }}                                     
            />
        </div>
    );
};



/**
 * ServiceEditorAdaptorFile
 * 
 * Main component for displaying the editor for a LMDB Service.
 */
export const ServiceEditorAdaptorFile = (props) => {

    const {editable} = useOperational();
    const theme = useTheme();

    const {service, serviceConf, serviceUriRelated, confTypes} = props;        
    if (!service || !serviceConf || !confTypes)
        return null;

    const conf = serviceConf.getPropertyValue("conf");
    if (!conf)
        return (
            <>
                <div style={{ height: "10vh" }} />
                <div style={{ textAlign: "center" }}>
                    <Typography text="This service definition has no configuration data." />
                </div>
            </>    
        );
        
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
                            <div>
                                <Typography size="7" text="General Configuration" />
                                <div style={{ height: theme.spacing(2) }} />
                                <ObjectResponsive 
                                    object={conf}
                                    filterOptions={{
                                        filterProperties: [
                                            "description",
                                            "authorizationHandlerId",
                                            "journalAdaptorId"
                                        ],
                                        filterValuedProperties: editable ? false : true
                                    }}                                  
                                />      
                                <div style={{ height: theme.spacing(5) }} />
                                <Typography size="7" text="File Configuration" />
                                <div style={{ height: theme.spacing(2) }} />
                                <ServiceNewAdaptorFile 
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

export default ServiceEditorAdaptorFile;
