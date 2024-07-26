// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect, useCallback} from "react";

import {    
    AdapterDropdown,
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
 * ServiceNewAdapterModel
 * 
 * This is a common component, used by the ServiceNew stepper.  It provides a
 * layout for the General configuration for type=adapter, subtype=file services.
 */
export const ServiceNewAdapterModel = (props) => {

    const theme = useTheme();
    const [modelOptions, setModelOptions] = useState([]);
    const [fetchingModels, setFetchingModels] = useState(false);

    const {notification} = useApplication();
    const model = useModel();
    const forceUpdate = useForceUpdate();

    const {confObject} = props;
    const {editable} = useOperational(props);

    const onModelAdapterChanged = useCallback(async modelAdapter => {            
        let modelLocationAdapterId = confObject.getProperty("modelLocationAdapterId");

        if (modelAdapter !== modelLocationAdapterId.getValue())
            modelLocationAdapterId.setValue(modelAdapter);

        try {            
            /* now lookup any models that can be selected */
            setFetchingModels(true);
            const {objects} = model.retrieveObjects({
                objectTypeId: "_AdaptiveModel_", 
                adapterId: modelAdapter
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
            const modelAdapterId = confObject.getPropertyValue("modelLocationAdapterId");
            if (modelAdapterId) {
                let modelLocationAdapterId = confObject.getProperty("modelLocationAdapterId");

                if (modelAdapterId !== modelLocationAdapterId.getValue())
                    modelLocationAdapterId.setValue(modelAdapterId);

                try {            
                    /* now lookup any models that can be selected */
                    setFetchingModels(true);
                    const {objects} = model.retrieveObjects({
                        objectTypeId: "_AdaptiveModel_", 
                        adapterId: modelAdapterId
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

    }, [model, notification, confObject, onModelAdapterChanged]);
        
    if (!confObject)
        return null;        

    const mappedAdapterId = confObject.getProperty("mappedAdapterId");
    const modelLocationAdapterId = confObject.getProperty("modelLocationAdapterId");
    const modelId = confObject.getProperty("modelId");
    
    if (!editable) {
        return (
            <ObjectResponsive 
                object={confObject}
                filterOptions={{
                    filterExcludeProperties: [
                        "adapterId",
                        "adapterType",
                        "custom",
                        "variables",
                        "description",
                        "authorizationHandlerId",
                        "journalAdapterId",
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
            <AdapterDropdown 
                id="model-location-adapter-dropdown"
                data-testid="model-location-adapter-dropdown"
                disabled={editable === false}
                isModelLocation={true}
                required={modelLocationAdapterId.getRequired()}
                label={modelLocationAdapterId.getLabel()}
                aria-label={modelLocationAdapterId.getLabel()}
                description={modelLocationAdapterId.getDescription()}                    
                value={modelLocationAdapterId.getValue()}
                onChanged={onModelAdapterChanged}
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
            <AdapterDropdown 
                id="mapped-adapter-dropdown"
                data-testid="mapped-adapter-dropdown"
                disabled={editable === false}
                required={mappedAdapterId.getRequired()}
                label={mappedAdapterId.getLabel()}
                aria-label={mappedAdapterId.getLabel()}
                description={mappedAdapterId.getDescription()}                    
                value={mappedAdapterId.getValue()}
                onChanged={value => {
                    mappedAdapterId.setValue(value);
                    forceUpdate();
                }}
            />  
            <div style={{ height: theme.spacing(5) }} />
        </div>            
    );    
};



/**
 * ServiceEditorAdapterModel
 * 
 * Main component for displaying the editor for a Model Service.
 */
export const ServiceEditorAdapterModel = (props) => {

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
                                            "journalAdapterId"
                                        ],
                                        filterValuedProperties: editable ? false : true
                                    }}                                    
                                />    
                                <div style={{ height: theme.spacing(5) }} />
                                <Typography size="7" text="Model Configuration" />
                                <div style={{ height: theme.spacing(2) }} />
                                <ServiceNewAdapterModel 
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

export default ServiceEditorAdapterModel;
