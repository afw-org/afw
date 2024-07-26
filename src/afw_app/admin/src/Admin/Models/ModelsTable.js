// See the 'COPYING' file in the project root for licensing information.
import {useReducer, useEffect} from "react";

import {
    AdapterDropdown,
    Button,
    Dialog,
    Link,
    Toolbar,
    Spinner,
    Table,
    Typography,
    useModel,
} from "@afw/react";

import {useAppCore, useTheme, useApplication} from "../../hooks";
import ObjectsImport from "../../common/ObjectsImport";

import ModelsNew from "./ModelsNew";


const initialState = {
    selectedModels: [],
    showDeleteModel: false,
    showAddModel: false,
    showImportModel: false,
};

const reducer = (state, action) => {

    switch (action.type) {

    case "MODEL_NEW":
        return {
            ...state,
            showAddModel: true,
        };

    case "MODEL_DELETE":
        return {
            ...state,
            showDeleteModel: true,
        };

    case "MODEL_IMPORT":
        return {
            ...state,
            showImportModel: true,
        };

    case "MODELS_SELECT":
        return {
            ...state,
            selectedModels: action.models,
        };

    case "MODEL_DELETE_CANCEL":
        return {
            ...state,
            showDeleteModel: false,
        };

    case "MODEL_NEW_CANCEL":
        return {
            ...state,
            showAddModel: false,
        };

    case "MODEL_IMPORT_CANCEL":
        return {
            ...state,
            showImportModel: false,
        };
   
    case "CREATING_MODEL": 
        return {
            ...state,
            spinnerText: "Creating Model...",
        };

    case "CREATED_MODEL": 
        return {
            ...state,
            spinnerText: undefined,
            showAddModel: false,
        };

    case "DELETING_MODEL": 
        return {
            ...state,
            spinnerText: "Deleting Model...",
        };

    case "DELETED_MODEL":
        return {
            ...state,
            spinnerText: undefined,
        };

    case "DELETED_MODELS":
        return {
            ...state,
            showDeleteModel: false,
        };

    case "IMPORTING_MODELS":
        return {
            ...state,
            showImportModel: false,
            spinnerText: "Importing Models..",
        };

    case "IMPORTED_MODELS":
        return {
            ...state,
            spinnerText: undefined,
        };

    case "CREATE_MODEL_ERROR":
    case "DELETE_MODEL_ERROR":
    case "IMPORT_MODEL_ERROR":
        return {
            ...state,
            spinnerText: undefined,
            error: action.error,
        };

    default:
        return state;
    }
};

const ModelsTableTable = ({ adapterId, models, onSelectModels }) => {

    return (
        <>
            <Table 
                data-testid="models-table"
                rows={models}
                columns={[
                    {
                        key: "modelId", name: "Model", isResizable: true, minWidth: 150, maxWidth: 300, width: "20%", onRender: model => {
                            const modelId = model.getPropertyValue("modelId");                            
                            return (
                                <Link 
                                    text={modelId}
                                    uriComponents={["Admin", "Models", adapterId, modelId]}
                                />
                            );
                        }
                    },
                    {
                        key: "description", name: "Description", isResizable: true, minWidth: 300, width: "80%", isMultiline: true,
                        onRender: model => model.getPropertyValue("description")
                    }
                ]}
                selectionMode="multiple"
                onSelectionChanged={onSelectModels}
            />
            {
                (models?.length === 0) &&
                <div>
                    <div style={{ height: "5vh" }} />
                    <div style={{ textAlign: "center" }}>
                        <Typography color="textSecondary" text="No models found." />
                    </div>
                </div>
            }
        </>
    );
};

export const ModelsTable = (props) => {

    const {adapters} = useAppCore();
    const {notification} = useApplication();
    const theme = useTheme();
    const dataModel = useModel();

    const [state, dispatch] = useReducer(reducer, initialState);

    const {adapterId, onSelectAdapterId, reloadModels} = props;

    /* When we have an error in our state, use the app notifications to alert them */
    useEffect(() => {
        if (notification && state.error)
            notification({ type: "error", message: state.error });
    }, [notification, state.error]);

    /**
     * onCreateModel()
     * 
     * This function creates the new _AdaptiveModel_ object when the user 
     * completes filling out the required information, including its 
     * modelId, description and any objectTypes declared.     
     */
    const onCreateModel = async (modelId, description, objectTypes) => {

        const model = dataModel.newObject({
            adapterId, 
            objectTypeId: "_AdaptiveModel_"
        });
        await model.initialize();

        model.setObjectId(modelId);
        model.setPropertyValue("modelId", modelId);
        model.setPropertyValue("description", description);
        model.setPropertyValue("objectTypes", objectTypes);

        dispatch({ type: "CREATING_MODEL" });

        try {
            const response = model.add();
            await response.result();
            
            dispatch({ type: "CREATED_MODEL" });
        } catch (error) {
            dispatch({ type: "CREATE_MODEL_ERROR", error });
        }

        reloadModels();            
    };

    /**
     * 
     */
    const onConfirmDelete = async () => {

        for (const selectedModel of state.selectedModels) {
            try {
                dispatch({ type: "DELETING_MODEL", model: selectedModel });

                const response = selectedModel.delete();
                await response.result();

                dispatch({ type: "DELETED_MODEL", model: selectedModel });
            } catch (error) {
                dispatch({ type: "DELETE_MODEL_ERROR", error });
            }
        }

        dispatch({ type: "DELETED_MODELS" });
        reloadModels();
    };

    /**
     *      
     */
    const onImportModel = async (models) => {

        dispatch({ type: "IMPORTING_MODELS" });

        for (const model of models) {
            try {
                await model.initialize();
                await model.add().result();
            } catch (error) {
                dispatch({ type: "IMPORT_MODEL_ERROR", error });
            }
        }
        
        dispatch({ type: "IMPORTED_MODELS" });
        reloadModels();
    };


    /* Only show adapters that can store models (isModelLocation) */
    const modelLocationAdapters = 
        adapters?.filter(adapter => 
            adapter.properties.isModelLocation).map(adapter =>
            ({
                key: adapter.adapterId,
                text: adapter.adapterId,
            })
        ) || [];    

    return (
        <>
            <div>
                <Toolbar 
                    contains={[
                        <Button 
                            key="New"
                            label="New Model"
                            tooltip="Create a New Model"
                            variant="text"
                            color="primary"
                            icon="add"
                            size="small"
                            onClick={() => dispatch({ type: "MODEL_NEW" })}
                        />,
                        <Button 
                            key="Delete"
                            label="Delete Model"
                            tooltip="Delete the selected Model"
                            variant="text"
                            icon="delete"
                            size="small"
                            disabled={(state.selectedModels?.length > 0) ? false : true}
                            onClick={() => dispatch({ type: "MODEL_DELETE" })}
                        />,
                        <Button 
                            key="Import"
                            label="Import Model"
                            tooltip="Import a Model from Local file"
                            variant="text"
                            icon="publish"
                            size="small"
                            onClick={() => dispatch({ type: "MODEL_IMPORT" })}
                        />,
                        <Button 
                            key="Refresh"
                            label="Refresh"
                            tooltip="Refresh models"
                            variant="text"
                            icon="refresh"
                            size="small"
                            onClick={() => reloadModels()}
                        />
                    ]}
                />
            </div>
            {
                (modelLocationAdapters.length > 1) ?
                    <div style={{ maxWidth: "300px", marginBottom: theme.spacing(2) }}>                    
                        <AdapterDropdown 
                            id="model-adapter-dropdown"
                            value={adapterId}
                            isModelLocation={true}
                            onChanged={onSelectAdapterId}                        
                        />
                    </div> : null
            }
            <ModelsTableTable 
                {...props}         
                onSelectModels={(models) => dispatch({ type: "MODELS_SELECT", models })}
            />
            <ModelsNew 
                {...props}
                open={state.showAddModel}
                onDismiss={() => dispatch({ type: "MODEL_NEW_CANCEL" })}
                onCreate={onCreateModel}                    
            />
            <ObjectsImport 
                open={state.showImportModel}
                title="Import Model"
                prompt="Select a file containing the Adaptive Model to import."
                onDismiss={() => dispatch({ type: "MODEL_IMPORT_CANCEL" })}
                adapterId={adapterId}
                objectTypeId="_AdaptiveModel_"
                onImport={onImportModel}
            />
            <Dialog 
                open={state.showDeleteModel}
                title="Delete Model"
                subText="Are you sure you want to delete the selected model(s)?"
                isBlocking={true}    
                footer={
                    <div style={{ display: "flex" }}>
                        <Button 
                            style={{ marginRight: theme.spacing(1) }}
                            color="primary"
                            variant="contained"
                            label="Delete"
                            onClick={onConfirmDelete}
                        />
                        <Button 
                            label="Cancel"
                            onClick={() => dispatch({ type: "MODEL_DELETE_CANCEL" })}
                        />
                    </div>
                }                
            />
            { state.spinnerText &&
                <Spinner 
                    size="large"
                    label={state.spinnerText}
                    fullScreen={true}
                />
            }
        </>
    );
};

export default ModelsTable;
