// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";

import {
    Button,
    ChoiceGroup,
    Dialog,
    Divider,
    ObjectResponsive,
    PropertyResponsive,
    ObjectSelector,
    Responsive,
    CodeEditor,
    Stepper,
    TextField,
    Typography,
    useModel,
    useEventId,
    useValues,
    useForceUpdate,
    useIsMounted,
    OperationalContext,
    OperationalMode
} from "@afw/react";

import {AfwObject} from "@afw/client";

import {ServiceNewAdapter} from "./ServiceEditorAdapter";
import {ServiceNewAdapterFile} from "./ServiceEditorAdapterFile";
import {ServiceNewAdapterLmdb} from "./ServiceEditorAdapterLmdb";
import {ServiceNewAdapterModel} from "./ServiceEditorAdapterModel";
import {ServiceNewAdapterLdap} from "./ServiceEditorAdapterLdap";

import {ServiceNewLog} from "./ServiceEditorLog";
import {ServiceNewLogSyslog} from "./ServiceEditorLogSyslog";
import {ServiceNewLogStandard} from "./ServiceEditorLogStandard";

import {ServiceNewAuthorizationHandler} from "./ServiceEditorAuthorizationHandler";

import {ErrorDialog} from "../../common/ErrorForm";
import {useAppCore, useTheme} from "../../hooks";


const decamelize = (str, separator = " ") => {

    let newStr = str
        .replace(/([a-z\d])([A-Z])/g, "$1" + separator + "$2")
        .replace(/([A-Z]+)([A-Z][a-z\d]+)/g, "$1" + separator + "$2");        
    
    return newStr.charAt(0).toUpperCase() + newStr.slice(1);
};

/*
 * ServiceStepSelect
 * 
 * This component is the first "step" in the walk through for creating
 * a new Service.  It allows the user to select a service type and 
 * subtype from a list of available service types.
 * 
 */
export const ServiceStepSelect = (props) => {

    const [selected, setSelected] = useState([]);
    const [objectSelectors, setObjectSelectors] = useState([]);
    const [selectedGroupIndex, setSelectedGroupIndex] = useState();

    const theme = useTheme();
    const {environment} = useAppCore();       
    const model = useModel();
    const isMounted = useIsMounted();
     
    const serviceTypeObjects = environment.service_type;

    const onSelectServiceType = (serviceTypeObject, serviceSubtypeObject, index, groupIndex) => {
        
        if (isMounted()) {
            setSelected([index]);
            setSelectedGroupIndex(groupIndex);
            
            props.onSelectServiceType(serviceTypeObject, serviceSubtypeObject);
        }
    };
       
    useEffect(() => {                

        const loadObjectSelectors = async () => {
            let objectSelectors = [];
            for (let serviceTypeObject of Object.values(serviceTypeObjects)) {        
                const {serviceType, confType} = serviceTypeObject;
                const {subtypeRegistryType, subtypePropertyName} = confType;          
                
                /* get all of the conf subtypes for this service type */
                const subtypeObjectProperties = environment[subtypeRegistryType];
                        
                let serviceObjects = [];
                for (let [subtypeObjectPropertyName, subtypeObjectProperty] of Object.entries(subtypeObjectProperties)) {
                    let aObj = new AfwObject({ model, object: subtypeObjectProperty, propertyName: subtypeObjectPropertyName });

                    await aObj.initialize();
                    if (isMounted())
                        serviceObjects.push(aObj);
                }      
                
                objectSelectors.push({ serviceType, subtypePropertyName, serviceObjects, serviceTypeObject });                
            }

            if (isMounted())
                setObjectSelectors(objectSelectors);
        };

        if (serviceTypeObjects)
            loadObjectSelectors(serviceTypeObjects);       
    
    }, [model, environment, serviceTypeObjects, isMounted]);        
    
    return (            
        <div style={{ maxHeight: "50vh", overflowX: "hidden", overflowY: "auto" }}>             
            {
                objectSelectors.map( ({ serviceType, subtypePropertyName, serviceObjects, serviceTypeObject }, groupIndex) =>
                    <div key={groupIndex}>
                        <ObjectSelector
                            data-testid={"object-selector-" + serviceType}
                            key={serviceType} 
                            title={decamelize(serviceType) + " Types"}
                            selectedIndexes={(groupIndex === selectedGroupIndex) ? selected : []}
                            onSelect={(serviceSubtypeObject, index) => onSelectServiceType(serviceTypeObject, serviceSubtypeObject, index, groupIndex)}
                            objects={serviceObjects}
                            cols={4}
                            titleProperty={subtypePropertyName}
                            descriptionProperty="description"
                        />
                        <div key={"divider-" + serviceType}>
                            <div style={{ height: theme.spacing(2) }} />
                            <Divider />
                        </div>
                    </div>
                )
            }
        </div>       
    );   

};

/*
 * ServiceStepDescription
 * 
 * This component is the first "step" in the walk through for creating
 * a new Service.  It prompts for Service type and subtype, along with
 * a description and startup options.
 * 
 */
export const ServiceStepDescription = (props) => {     
              
    const {idProperty, confObject, startupKey} = props;
    const theme = useTheme();
    const forceUpdate = useForceUpdate();

    return (
        <div style={{ marginTop: theme.spacing(6) }}>
            <Responsive 
                spacing={1}
                items={[
                    {
                        breakpoints: {
                            small: 12,
                            medium: 6,
                        },
                        contains:
                            <TextField                                     
                                id="service-new-id"
                                //eslint-disable-next-line
                                autoFocus={true}
                                data-testid="ServiceNewIdField"
                                label={confObject.getProperty(idProperty).getLabel()}
                                required={true}
                                value={confObject.getPropertyValue(idProperty)}
                                description={confObject.getProperty(idProperty).getBrief()}
                                onChanged={(idPropertyValue) => {
                                    confObject.setPropertyValue(idProperty, idPropertyValue);
                                    forceUpdate();
                                }}
                            />
                    },                      
                    {
                        breakpoints: {
                            small: 12,
                            medium: 6,
                        },
                        contains: 
                            <ChoiceGroup 
                                data-testid="ServiceNewStartupCondition"
                                aria-label="Startup Condition"
                                label="Startup Condition"
                                options={[
                                    { key: "immediate", text: "Immediate" },
                                    { key: "manual", text: "Manual" },
                                    { key: "disabled", text: "Disabled" },
                                    { key: "permanent", text: "Permanent",  disabled: true  },
                                ]}
                                selectedKey={startupKey}
                                onChanged={(startupCondition) => props.onStartupConditionChanged(startupCondition)}
                            />
                    }
                ]}
            />
            {
                confObject && (
                    <PropertyResponsive 
                        data-testid="ServiceNewDescriptionField"
                        property={confObject.getProperty("description")}
                    />
                )
            }
        </div>
    );

};


/**
 * ServiceStepGeneral
 * 
 * This component is the second step in configuring a new service.
 * It displays general configuration properties that are common to
 * all Service types that the user has selected (i.e., adapter, log).
 * 
 */
export const ServiceStepGeneral = (props) => {
    
    const confObject = props.confObject;
    if (!confObject)
        return null;

    const serviceType = confObject.getPropertyValue("type");

    if (serviceType === "adapter")
        return <ServiceNewAdapter confObject={confObject} />;

    else if (serviceType === "log")
        return <ServiceNewLog confObject={confObject} />;

    else if (serviceType === "authorizationHandler")
        return <ServiceNewAuthorizationHandler confObject={confObject} />;

    return <ObjectResponsive object={confObject} />;
    
};


/**
 * ServiceStepDetailed
 * 
 * This component is the third step in configuring a new service.
 * It displays detailed configuration properties that are unique to
 * a Service subtype that the user has selected (i.e., LMDB, LDAP, etc.).
 * 
 */
export const ServiceStepDetailed = (props) => {

    const {objectTypeObjects, confObject, serviceSubtype} = props;
    if (!confObject)
        return null;

    const serviceType = confObject.getPropertyValue("type");

    if (serviceType === "log") {

        if (serviceSubtype === "syslog")
            return <ServiceNewLogSyslog confObject={confObject} />;

        else if (serviceSubtype === "standard")
            return <ServiceNewLogStandard confObject={confObject} />;

    } else if (serviceType === "adapter") {

        if (serviceSubtype === "lmdb") 
            return <ServiceNewAdapterLmdb confObject={confObject} />;

        else if (serviceSubtype === "model")
            return <ServiceNewAdapterModel confObject={confObject} />;

        else if (serviceSubtype === "file") 
            return <ServiceNewAdapterFile confObject={confObject} />;

        else if (serviceSubtype === "ldap")
            return <ServiceNewAdapterLdap confObject={confObject} />;

    } else if (serviceType === "authorizationHandler") {
        //eslint-disable-next-line
    }

    /*
        In order to figure out the adapter subtype-specific properties to display, we do a quick "diff" of the 
        type Object Type and subtype Object and only display the subtype-specific properties.
     */
    const typeObjectTypeConf = "/afw/_AdaptiveObjectType_/_AdaptiveConf_" + serviceType;
    const typeObjectType = objectTypeObjects[typeObjectTypeConf];
    const subtypeObjectType = objectTypeObjects[typeObjectTypeConf + "_" + serviceSubtype];

    if (!subtypeObjectType) {
        return (
            <div style={{ textAlign: "center" }}>
                <Typography text={"Cannot locate Object Type " + typeObjectTypeConf + "_" + serviceSubtype} />
                <Typography text="You may need to reload the application to refresh the Object Type definitions" />
            </div>
        );
    }

    let filterProperties = [];
    subtypeObjectType.getPropertyValue("propertyTypes").getProperties().forEach(propertyType => {
        let propertyName = propertyType.getName();
        if (!typeObjectType.getPropertyValue(["propertyTypes", propertyName]))
            filterProperties.push(propertyName);
    });

    return (
        <ObjectResponsive 
            object={confObject}
            filterOptions={{
                filterProperties
            }}
        />
    );    
};

const CreateButton = ({ className, style, id, confObject, onCreateService }) => {

    const [canCreate, setCanCreate] = useState(false);
    const changed = useEventId({ object: confObject, eventId: "onChildChanged" });    

    useEffect(() => {
        if (confObject) {
            const errors = confObject.validate();
            if (!errors || errors.length === 0) {                
                setCanCreate(true);                                   
            }
        }
    }, [changed, confObject]);

    return (
        <Button 
            id={id}
            className={className}
            style={style}
            data-testid="ServiceNewCreateBtn"            
            color="primary"
            variant="contained"
            label="Create" 
            disabled={!canCreate}    
            onClick={onCreateService}                                                                                       
        />
    );
};

/**
 * ServiceNew
 * 
 * This is the main component for stepping through the process of 
 * creating a new service.
 */
export const ServiceNew = (props) => {

    const [stepsText] = useState([
        "Select",
        "Describe",
        "Settings",
        "Details"
    ]);
    const [activeStep, setActiveStep] = useState(0);
    const [startupKey, setStartupKey] = useState("manual");
    const [serviceTypeObject, setServiceTypeObject] = useState();
    const [serviceSubtypeObject, setServiceSubtypeObject] = useState();
    const [serviceSubtype, setServiceSubtype] = useState();
    const [canNext, setCanNext] = useState(false);    
    const [confObject, setConfObject] = useState();
    const [idProperty, setIdProperty] = useState();
    const [error, setError] = useState();
    const [editSource, setEditSource] = useState();

    const theme = useTheme();
    const {application, objectTypeObjects} = useAppCore();
    const model = useModel();
    const changed = useEventId({ object: confObject, eventId: "onSavable" });
    const values = useValues(confObject);

    useEffect(() => {        
        if (confObject && changed && idProperty && values && values[idProperty]) {                      
            setCanNext(true);
        } else setCanNext(false);
    }, [changed, confObject, idProperty, values]);
   
    const onSelectServiceType = (serviceTypeObject, serviceSubtypeObject) => {
        let canNext = false;

        if (serviceTypeObject && serviceSubtypeObject) 
            canNext = true;

        setServiceTypeObject(serviceTypeObject);
        setServiceSubtypeObject(serviceSubtypeObject);
        setCanNext(canNext);
    };

    const onDismiss = () => {
        setCanNext(false);
        setActiveStep(0);
        setStartupKey("manual");

        props.onDismiss();
    };

    const onCreateService = async () => {
        const confAdapterId = application.getPropertyValue("confAdapterId");
        const idPropertyValue = confObject.getPropertyValue(idProperty);
        
        /* create a new service configuration object */
        const newService = model.newObject({
            adapterId: confAdapterId,
            objectTypeId: "_AdaptiveServiceConf_",            
        });
        await newService.initialize();
                
        /* assign the objectId for the new service */
        const objectId = serviceTypeObject.serviceType + "-" + idPropertyValue;

        newService.setPropertyValue("serviceId", objectId);        
        newService.setPropertyValue("startup", startupKey);
        newService.setPropertyValue("conf", confObject);
        newService.setPath("/" + confAdapterId + "/_AdaptiveServiceConf_/" + objectId);

        try {
            const response = newService.add();
            await response.result();
            
            setConfObject();
            setActiveStep(0);

            props.onSave(newService, response);
        } catch (error) {
            setError(error);
        }
    };

    const onNext = async () => {     

        if (activeStep === 0) {
            /* when user clicks Next on first step, then, go ahead and create our conf object */            
            const serviceSubtype = serviceSubtypeObject.getEmbeddingProperty();

            const {serviceType, confType} = serviceTypeObject;
            const {subtypePropertyName, idPropertyName} = confType;

            const confObjectTypeId = "_AdaptiveConf_" + serviceType + "_" + serviceSubtype;            

            try {
                const confObject = model.newObject({
                    adapterId: "afw", 
                    objectTypeId: confObjectTypeId
                }); 

                await confObject.initialize();           

                confObject.setPropertyValue("type", serviceType);
                confObject.setPropertyValue(subtypePropertyName, serviceSubtype);            

                setIdProperty(idPropertyName);
                setConfObject(confObject);            
                setCanNext(false);
                setServiceSubtype(serviceSubtype);

                setActiveStep(activeStep => activeStep + 1);
            } catch (error) {
                setError(error);
            }
        } else {
            setActiveStep(activeStep => activeStep + 1);
        }
    };     

    const steps = [            
        <ServiceStepSelect 
            {...props}
            onSelectServiceType={onSelectServiceType}
        />,
        <ServiceStepDescription                 
            startupKey={startupKey}
            confObject={confObject}
            idProperty={idProperty}
            onStartupConditionChanged={(startupCondition) => setStartupKey(startupCondition.key)}                                        
        />,
        <ServiceStepGeneral 
            confObject={confObject}
            serviceSubtype={serviceSubtype}
        />,
        <ServiceStepDetailed
            objectTypeObjects={objectTypeObjects}                
            confObject={confObject} 
            serviceSubtype={serviceSubtype}
        />
    ];

    return (
        <Dialog             
            open={props.open}            
            onDismiss={onDismiss}
            blocking={true}
            showClose={true}
            title="New Service"
            maxWidth="lg"
            contains={
                <div data-testid="service-new" style={{ minHeight: "50vh" }}>
                    <OperationalContext.Provider value={OperationalMode.Editable}>
                        {
                            editSource ?                                 
                                <CodeEditor 
                                    source={confObject ? confObject.stringify() : "{}"}
                                    style={{ height: "50vh" }}
                                    onChanged={(source) => {
                                        if (confObject) {
                                            confObject.fromJSON(source);
                                        }
                                    }}
                                /> :
                                <>
                                    <Stepper 
                                        steps={stepsText}
                                        activeStep={activeStep}
                                    />                                                
                                    { steps[activeStep] }               
                                    {
                                        error &&
                                        <ErrorDialog
                                            error={error}
                                            onDismiss={() => setError()}
                                        />                            
                                    }       
                                </>
                        }  
                    </OperationalContext.Provider>
                </div>
            }
            footer={
                <div style={{ display: "flex", padding: theme.spacing(2), width: "100%", justifyContent: "space-between" }}>
                    <div>
                        <Button 
                            data-testid="ServicePreviousBtn"                            
                            label="Previous"
                            variant="contained"
                            disabled={activeStep === 0}
                            onClick={() => setActiveStep(activeStep - 1)}
                            startIcon="chevron_left"
                        />                        
                    </div>
                    <div style={{ display: "flex" }}>
                        {
                            editSource ?
                                <Button 
                                    style={{ marginRight: theme.spacing(1) }}
                                    label="Steps"
                                    onClick={() => setEditSource(false)}
                                /> :
                                <Button 
                                    style={{ marginRight: theme.spacing(1) }}
                                    label="Source"
                                    onClick={() => setEditSource(true)}
                                    disabled={true}
                                />
                        }
                        <Button 
                            data-testid="ServiceCancelBtn"                                                        
                            label="Cancel"                                        
                            onClick={onDismiss}
                        />
                        {
                            (activeStep < 3) ?
                                <Button 
                                    data-testid="ServiceNextBtn"
                                    style={{ marginLeft: theme.spacing(1) }}
                                    color="primary"      
                                    variant="contained"                                          
                                    label="Next"
                                    disabled={!canNext}
                                    onClick={onNext}
                                    endIcon="chevron_right"
                                /> :
                                <CreateButton                                     
                                    style={{ marginLeft: theme.spacing(1) }}  
                                    confObject={confObject}                                                                      
                                    onCreateService={onCreateService}                                                                                       
                                />
                        }
                    </div>
                </div>
            }
        />
    );
};

export default ServiceNew;
