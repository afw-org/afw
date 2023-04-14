// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect, useCallback} from "react";

import {    
    AdaptorDropdown,
    CustomVariables,
    Dropdown,    
    ObjectResponsive,
    Tabs,    
    Typography,
    useModel,
    useForceUpdate,
    useOperational
} from "@afw/react";

import {useApplication, useTheme} from "../../hooks";

import {
    ServiceEditorGeneral, 
    ServiceEditorRuntime
} from "./ServiceEditor";


/**
 * ServiceNewAdaptorModel
 * 
 * This is a common component, used by the ServiceNew stepper.  It provides a
 * layout for the General configuration for type=adaptor, subtype=file services.
 */
export const ServiceNewAdaptorModel = (props) => {

    const theme = useTheme();
    const [modelOptions, setModelOptions] = useState([]);
    const [fetchingModels, setFetchingModels] = useState(false);

    const {notification} = useApplication();
    const model = useModel();
    const forceUpdate = useForceUpdate();

    const {confObject} = props;
    const {editable} = useOperational(props);

    const onModelAdaptorChanged = useCallback(async modelAdaptor => {            
        let modelLocationAdaptorId = confObject.getProperty("modelLocationAdaptorId");

        if (modelAdaptor !== modelLocationAdaptorId.getValue())
            modelLocationAdaptorId.setValue(modelAdaptor);

        try {            
            /* now lookup any models that can be selected */
            setFetchingModels(true);
            const {objects} = model.retrieveObjects({
                objectTypeId: "_AdaptiveModel_", 
                adaptorId: modelAdaptor
            });              
            const models = await objects;          
        
            const modelOptions = [];            
            models.forEach(model => {
                const {modelId, description} = model.getPropertyValues();

                modelOptions.push({
                    key: modelId,
                    text: modelId,
                    description,
                });
            });

            setModelOptions(modelOptions);            
        } catch(error) {
            notification({message: error, type: "error"});
        }
        setFetchingModels(false);
    }, [confObject, model, notification]);

    useEffect(() => {        
        let isMounted = true;

        const fetchModels = async () => {
            const modelAdaptorId = confObject.getPropertyValue("modelLocationAdaptorId");
            if (modelAdaptorId) {
                let modelLocationAdaptorId = confObject.getProperty("modelLocationAdaptorId");

                if (modelAdaptorId !== modelLocationAdaptorId.getValue())
                    modelLocationAdaptorId.setValue(modelAdaptorId);

                try {            
                    /* now lookup any models that can be selected */
                    setFetchingModels(true);
                    const {objects} = model.retrieveObjects({
                        objectTypeId: "_AdaptiveModel_", 
                        adaptorId: modelAdaptorId
                    });               
                    const models = await objects;         
                
                    const modelOptions = [];            
                    models.forEach(model => {
                        const {modelId, description} = model.getPropertyValues();

                        modelOptions.push({
                            key: modelId,
                            text: modelId,
                            description,
                        });
                    });

                    if (isMounted)
                        setModelOptions(modelOptions);            
                } catch(error) {
                    notification({ message: error, type: "error" });
                }
                if (isMounted)
                    setFetchingModels(false);
            }
        };

        if (confObject) {            
            fetchModels(confObject);
        }

        return () => { isMounted = false; };

    }, [model, notification, confObject, onModelAdaptorChanged]);
        
    if (!confObject)
        return null;        

    const mappedAdaptorId = confObject.getProperty("mappedAdaptorId");
    const modelLocationAdaptorId = confObject.getProperty("modelLocationAdaptorId");
    const modelId = confObject.getProperty("modelId");
    
    if (!editable) {
        return (
            <ObjectResponsive 
                object={confObject}
                filterOptions={{
                    filterExcludeProperties: [
                        "adaptorId",
                        "adaptorType",
                        "custom",
                        "variables",
                        "description",
                        "authorizationHandlerId",
                        "journalAdaptorId",
                        "title",
                        "type"
                    ],
                    filterValuedProperties: editable ? false : true
                }}
            />
        );
    }
    
    return (
        <div>                
            <AdaptorDropdown 
                id="model-location-adaptor-dropdown"
                data-testid="model-location-adaptor-dropdown"
                disabled={editable === false}
                isModelLocation={true}
                required={modelLocationAdaptorId.getRequired()}
                label={modelLocationAdaptorId.getLabel()}
                aria-label={modelLocationAdaptorId.getLabel()}
                description={modelLocationAdaptorId.getDescription()}                    
                value={modelLocationAdaptorId.getValue()}
                onChanged={onModelAdaptorChanged}
            />
            <div style={{ height: theme.spacing(5) }} />
            <Dropdown 
                id="model-dropdown"
                data-testid="model-dropdown"
                isLoading={fetchingModels}
                disabled={editable === false}
                label={modelId.getLabel()}
                aria-label={modelId.getLabel()}
                description={modelId.getDescription()}
                required={modelId.getRequired()}
                options={modelOptions}
                selectedKey={modelId.getValue()}
                onChanged={value => {
                    modelId.setValue(value.key);
                    forceUpdate();
                }}
            />            
            <div style={{ height: theme.spacing(5) }} />
            <AdaptorDropdown 
                id="mapped-adaptor-dropdown"
                data-testid="mapped-adaptor-dropdown"
                disabled={editable === false}
                required={mappedAdaptorId.getRequired()}
                label={mappedAdaptorId.getLabel()}
                aria-label={mappedAdaptorId.getLabel()}
                description={mappedAdaptorId.getDescription()}                    
                value={mappedAdaptorId.getValue()}
                onChanged={value => {
                    mappedAdaptorId.setValue(value);
                    forceUpdate();
                }}
            />  
            <div style={{ height: theme.spacing(5) }} />
        </div>            
    );    
};



/**
 * ServiceEditorAdaptorModel
 * 
 * Main component for displaying the editor for a Model Service.
 */
export const ServiceEditorAdaptorModel = (props) => {

    const {editable} = useOperational();
    const theme = useTheme();
    
    const {service, serviceConf, serviceUriRelated, confTypes} = props;        
    if (!service || !serviceConf || !confTypes)
        return null;

    const conf = serviceConf.getPropertyValue("conf");
    if (!conf)
        return null;

    const type = conf.getPropertyValue("type");
    const idPropertyName = confTypes[type].idPropertyName;
    const subtypePropertyName = confTypes[type].subtypePropertyName;

    let qualifiers;

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
        <div style={{ paddingRight: theme.spacing(5) }}>
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
                                <Typography size="7" text="Model Configuration" />
                                <div style={{ height: theme.spacing(2) }} />
                                <ServiceNewAdaptorModel 
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
                                            qualifiers={qualifiers}
                                        />
                                } : {}
                        )
                    }
                ]}
            />                
        </div>                            
    );
};

export default ServiceEditorAdaptorModel;
