// See the 'COPYING' file in the project root for licensing information.
import {useReducer, useState} from "react";

import {
    Button,
    Dialog,
    Dropdown,
    TextField,
    Typography,
    useModel,
} from "@afw/react";

import {useAppCore, useTheme} from "../../hooks";

import {ModelObjectTypeSelector} from "./ModelObjectTypeSelector";
import {ModelMapping} from "./Mappings/ModelMapping";

const initialState = {
    step: 0,
    mappedObjectTypes: [],
    selectedTypes: [],
};

const reducer = (state, action) => {

    switch (action.type) {

    case "ON_NEXT":
        return {
            ...state,
            step: state.step + 1,
        };

    case "ON_PREV":
        return {
            ...state,
            step: state.step - 1,
        };

    case "DESCRIPTION":
        return {
            ...state,
            description: action.description
        };

    case "MODEL_ID":
        return {
            ...state,
            modelId: action.modelId
        };

    case "SELECT_OBJECT_TYPES":
        return {
            ...state,
            mappedObjectTypes: action.mappedObjectTypes,
            selectedTypes: action.selectedTypes,
        };

    case "ON_MAP":
        return {
            ...state,
            step: state.step + 1,
            model: action.model,
        };

    case "DISMISS":
        return initialState;

    default:
        return state;
    }
};

const Step1Contains = ({ modelId, description, dispatch }) => {

    return (
        <div>                
            <div>                            
                <TextField                     
                    id="models-new-model-id"
                    data-testid="models-new-model-id"
                    //eslint-disable-next-line
                    autoFocus={true}
                    label="Model Id"     
                    description="Enter the Model Id for the new Model."
                    value={modelId}
                    onChanged={modelId => dispatch({ type: "MODEL_ID", modelId })}                                                                   
                />
                <TextField 
                    id="models-new-model-description"
                    data-testid="models-new-model-description"
                    label="Description"
                    description="Enter a description for this new Model."
                    value={description}
                    multiline={true}
                    rows={4}
                    onChanged={description => dispatch({ type: "DESCRIPTION", description })}
                />
            </div>                
        </div>
    );
};

const Step1Footer = ({ modelId, onCreate, onNext, onDismiss }) => {

    const theme = useTheme();

    return (
        <div style={{ display: "flex", width: "100%", padding: theme.spacing(2), justifyContent: "space-between" }}>
            <div style={{ display: "flex" }}>
                <Button 
                    style={{ marginRight: theme.spacing(1) }}
                    label="Cancel"                    
                    onClick={onDismiss}
                />
                <Button 
                    label="<< Previous" 
                    aria-label="Previous"                       
                    disabled={true}                                                                  
                />
            </div>
            <div style={{ display: "flex" }}>
                <Button 
                    style={{ marginRight: theme.spacing(1) }}                                                                                                            
                    label="Next >>"     
                    aria-label="Next"                       
                    onClick={onNext}
                    disabled={!modelId}
                />
                <Button 
                    color="primary"           
                    variant="contained"                         
                    label="Create"
                    disabled={!modelId}
                    onClick={onCreate}                             
                />
            </div>
        </div>
    );
};

const Step2Contains = ({ dispatch }) => {

    const [selectedAdapter, setSelectedAdapter] = useState();
    const {adapters} = useAppCore();
    const theme = useTheme();

    let mappedOptions = [];
    if (adapters)
        mappedOptions = adapters.map(adapter => 
            ({ 
                key: adapter.adapterId,
                text: adapter.adapterId,                
                description: adapter.description,
                adapter
            })
        );

    return (
        <div>
            <div> 
                <Dropdown 
                    id="AdapterMapDropdown"
                    label="Select Adapter to Map"
                    description="Select an Adapter to map the Object and Property Types to.  If this Model is intended to be used with a back end adapter that has no schema, select None."
                    options={[
                        { key: "None", text: "None (No Mapping)" },
                        { key: "divider2", type: "divider" },
                        ...mappedOptions,                            
                    ]}
                    selectedKey={selectedAdapter?.key || "None"}
                    onChanged={setSelectedAdapter}
                /> 
                <div style={{ height: theme.spacing(5) }} />
                <div style={{ height: "30vh", minHeight: "400px", overflow: "auto" }}>
                    { (selectedAdapter && selectedAdapter.key !== "None") && (
                        <>
                            <Typography size="4" text="Select the Object and Property Types to Map:" />
                            <ModelObjectTypeSelector                         
                                adapterId={selectedAdapter ? selectedAdapter.key : undefined}
                                onSelect={(mappedObjectTypes, selectedTypes) =>{
                                    dispatch({ type: "SELECT_OBJECT_TYPES", mappedObjectTypes, selectedTypes });                                
                                }}
                            />  
                        </>
                    )}
                </div>
            </div>                
        </div>
    );   
};

const Step2Footer = ({ modelId, selectedTypes, onCreate, onPrevious, onMap, onDismiss }) => {

    const theme = useTheme();

    return (
        <div style={{ display: "flex", width: "100%", padding: theme.spacing(2), justifyContent: "space-between" }}>          
            <div style={{ display: "flex" }}>
                <Button 
                    style={{ marginRight: theme.spacing(1) }}
                    label="Cancel"
                    onClick={onDismiss}
                />
                <Button 
                    label="<< Previous"   
                    aria-label="Previous"                       
                    onClick={onPrevious}                                                                
                />
            </div>
            <div style={{ display: "flex" }}>
                <Button 
                    style={{ marginRight: theme.spacing(1) }}                                                                                                            
                    label="Next >>"
                    aria-label="Next"                       
                    onClick={onMap}
                    disabled={(selectedTypes.length === 0)}
                />
                <Button 
                    color="primary"         
                    variant="contained"                           
                    label="Create"
                    disabled={!modelId}
                    onClick={onCreate}                             
                />
            </div>
        </div>
    );
};

const Step3Contains = ({ model }) => {

    return (
        <div style={{ height: "30vh", minHeight: "400px", overflow: "auto" }}>
            <ModelMapping 
                modelObjectTypeObjects={model ? model.getPropertyValue("objectTypes") : undefined}
                editable={true}
            />
        </div>
    );
};

const Step3Footer = ({ modelId, onCreate, onPrevious, onDismiss }) => {

    const theme = useTheme();

    return (
        <div style={{ display: "flex", width: "100%", padding: theme.spacing(2), justifyContent: "space-between" }}>          
            <div style={{ display: "flex" }}>
                <Button 
                    style={{ marginRight: theme.spacing(1) }}
                    label="Cancel"
                    onClick={onDismiss}
                />
                <Button 
                    label="<< Previous"
                    aria-label="Previous"                          
                    onClick={onPrevious}                                                                
                />
            </div>            
            <div style={{ display: "flex" }}>                    
                <Button 
                    color="primary"       
                    variant="contained"                             
                    label="Create"
                    disabled={!modelId}
                    onClick={onCreate}                             
                />
            </div>
        </div>
    );
};

export const ModelsNew = ({ open, adapterId, onCreate, onDismiss }) => {

    const [state, dispatch] = useReducer(reducer, initialState);
    const dataModel = useModel();

    const onCreateModel = async () => {

        const {modelId, description, model} = state;
        const objectTypes = model ? model.getPropertyValue("objectTypes").toJSON() : {};

        onCreate(modelId, description, objectTypes);
    };

    /**
     * When creating a new model that maps to another model, they can choose 
     * which Object Types and Property Types to map to/from.
     */
    const onMap = async () => {

        let objectTypes = {};

        /* filter out only those object types and property types that were selected */
        let types = state.mappedObjectTypes.filter(objectType => {
            /* if the entire objectType was selected, return it from this filter */
            if (state.selectedTypes.includes(objectType.getPath()))
                return true;

            /* check each property type to see if individual ones were selected */
            let propertyTypes= objectType.getPropertyValue("propertyTypes");
            if (propertyTypes) {                
                propertyTypes.getProperties().forEach(propertyType => {
                    if (!state.selectedTypes.includes(propertyType.getValue().getPath())) {
                        /* they didn't selected this property type, so remove it*/
                        propertyType.remove();
                    }
                });

                if (propertyTypes.getProperties() && propertyTypes.getProperties().length > 0)
                    return true;
            }

            return false;
        });

        /* setup the default mapped names */
        types.forEach(mappedObjectType => {  
            const modelObjectType = {};   

            modelObjectType["mappedObjectType"] = mappedObjectType.getObjectId();
            if (mappedObjectType.getPropertyValue("propertyTypes")) {
                modelObjectType["propertyTypes"] = {};
                /* set default mapped names, which can be changed later */
                mappedObjectType.getPropertyValue("propertyTypes").getProperties().forEach(propertyType => {
                    modelObjectType["propertyTypes"][propertyType.getName()] = {
                        mappedPropertyName: propertyType.getName()
                    };
                });
            }

            objectTypes[mappedObjectType.getObjectId()] = modelObjectType;            
        });

        const model = dataModel.newObject({
            adapterId,
            objectTypeId: "_AdaptiveModel_",
        });
        await model.initialize();

        model.setObjectId(state.modelId);
        model.setPropertyValue("modelId", state.modelId);
        model.setPropertyValue("description", state.description);
        model.setPropertyValue("objectTypes", objectTypes);

        dispatch({ type: "ON_MAP", model });
    };

    const StepContains = [
        <Step1Contains 
            {...state}
            dispatch={dispatch}
        />,
        <Step2Contains        
            {...state}     
            dispatch={dispatch}
        />,
        <Step3Contains 
            {...state}
            dispatch={dispatch}
        />
    ];

    const StepFooters = [
        <Step1Footer
            {...state}
            onNext={() => dispatch({ type: "ON_NEXT" })}
            onPrevious={() => dispatch({ type: "ON_PREV" })}
            onCreate={onCreateModel}
            onDismiss={() => {
                dispatch({ type: "DISMISS" });
                onDismiss();
            }}
            onMap={onMap}
        />,
        <Step2Footer
            {...state}
            onNext={() => dispatch({ type: "ON_NEXT" })}
            onPrevious={() => dispatch({ type: "ON_PREV" })}
            onCreate={onCreateModel}
            onDismiss={() => {
                dispatch({ type: "DISMISS" });
                onDismiss();
            }}
            onMap={onMap}
        />,
        <Step3Footer
            {...state}
            onNext={() => dispatch({ type: "ON_NEXT" })}
            onPrevious={() => dispatch({ type: "ON_PREV" })}
            onCreate={onCreateModel}
            onDismiss={() => {
                dispatch({ type: "DISMISS" });
                onDismiss();
            }}
            onMap={onMap}
        />
    ];    

    

    return (
        <Dialog 
            open={open}            
            onDismiss={() => {
                dispatch({ type: "DISMISS" });
                onDismiss();
            }}
            showClose
            isBlocking            
            maxWidth="md"
            title="New Model"
            contains={StepContains[state.step]}
            footer={StepFooters[state.step]}
        />
    );
};

export default ModelsNew;
