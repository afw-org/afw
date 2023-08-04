// See the 'COPYING' file in the project root for licensing information.
import {useEffect, useReducer} from "react";
import PropTypes from "prop-types";
import {Switch, Route, Prompt, useLocation, useHistory} from "react-router";

import {
    Button,
    Dialog,
    ToggleButtons,
    Spinner,
    Typography,    
    OperationalContext,
    useEventId
} from "@afw/react";

import {AfwObject} from "@afw/client";
import {afwServiceRestart} from "@afw/core";

import Model from "./Model";
import ModelReview from "./ModelReview";
import ModelObjectTypes from "./ModelObjectTypes";
import ModelPropertyTypes from "./ModelPropertyTypes";
import ModelDeploy from "./ModelDeploy";
import ModelEditorContextMenu from "./ModelEditorContextMenu";

import NoRoute from "../../common/NoRoute";
import {useApplication, useAppCore, useTheme} from "../../hooks";


const initialState = {
    showModelReview:    false,
    editable:           false,
    perspective:        "overview",
    deploy:             false,
};

const reducer = (state, action) => {

    switch (action.type) {
     
    case "PERSPECTIVE":
        return {
            ...state,
            perspective: action.perspective,
            error: undefined,
            optionsTarget: undefined,
        };

    case "MODEL_SAVED":
        return {
            ...state,
            showModelReview: false,
            editable: false,
            spinnerMessage: undefined,
            optionsTarget: undefined,
        };

    case "MODEL_REVIEW":
        return {
            ...state,
            showModelReview: true,
            modelDifferences: action.modelDifferences,
            error: undefined,
            optionsTarget: undefined,
        };

    case "MODEL_SAVING":
        return {
            ...state,
            spinnerText: "Saving Model...",
            error: undefined,
            optionsTarget: undefined,
        };

    case "MODEL_REVIEWING":
        return {
            ...state,
            spinnerMessage: "Reviewing Changes...",
            error: undefined,
            optionsTarget: undefined,
        };

    case "MODEL_DISCARD":
        return {
            ...state,
            editable: false,
            showConfirm: false,            
            spinnerMessage: undefined,
            error: undefined,
            optionsTarget: undefined,
            reload: [],
        };

    case "MODEL_DISCARD_CONFIRM":
        return {
            ...state,
            showConfirm: true,
            spinnerMessage: undefined,
            error: undefined,
            optionsTarget: undefined,
        };

    case "CONFIRM_DISMISS":
        return {
            ...state,
            showConfirm: false,
            spinnerMessage: undefined,
            error: undefined,
            optionsTarget: undefined,
        };

    case "EDIT":
        return {
            ...state,
            editable: true,
            error: undefined,
            optionsTarget: undefined,
        };

    case "ON_EDIT":
        return {
            ...state,
            editable: action.editable,
            error: undefined,
            optionsTarget: undefined,
        };

    case "DEPLOY":
        return {
            ...state,
            deploy: true,
            optionsTarget: undefined,    
            error: undefined,        
        };

    case "DEPLOY_DISMISS":
        return {
            ...state,
            deploy: false,
            error: undefined,
            optionsTarget: undefined,
        };

    case "REVIEW_DISMISS":
        return {
            ...state,
            showModelReview: false,
            spinnerMessage: undefined,
            error: undefined,
            optionsTarget: undefined,
        };

    case "MODEL_ADAPTOR_RESTARTING":
        return {
            ...state,
            spinnerMessage: "Restarting Model Adaptor...",
            error: undefined,
            optionsTarget: undefined,
        };

    case "MODEL_ADAPTOR_RESTARTED":
        return {
            ...state,
            spinnerMessage: undefined,
            error: undefined,
            optionsTarget: undefined,
        };

    case "SELECT_MAPPED_ADAPTOR":
        return {
            ...state,
            mappedAdaptorId: action.adaptor?.adaptorId,
            optionsTarget: undefined,
            error: undefined,
        };

    case "MORE_OPTIONS":
        return {
            ...state,
            optionsTarget: action.target,
            error: undefined,
        };

    case "NO_OPTIONS":
        return {
            ...state,
            optionsTarget: undefined,
            error: undefined,
        };

    case "ERROR":
        return {
            ...state,
            spinnerMessage: undefined,
            error: action.error,
            optionsTarget: undefined,
        };
    
    default:
        return state;
    }
};

const fullOptions = [
    {
        value: "tree",
        icon: "share",
        label: "Show Tree View",
        tooltip: "Show Tree View",
    },
    {
        value: "overview",
        icon: "dashboard",
        label: "Show Overview",
        tooltip: "Show Overview"
    },    
    {
        value: "spreadsheet",
        icon: "grid_on",
        label: "Show Spreadsheet",
        tooltip: "Show Spreadsheet",
    },
    {
        value: "mappings",
        icon: "swap_horiz",
        label: "Show Data Mappings",
        tooltip: "Show Data Mappings"
    },
    {
        value: "source",
        icon: "code",
        label: "Show Source",
        tooltip: "Show Source"
    },                                    
];

const mobileOptions = [
    {
        value: "overview",
        icon: "dashboard",
        label: "Show Overview",
        tooltip: "Show Overview"
    },    
    {
        value: "spreadsheet",
        icon: "grid_on",
        label: "Show Spreadsheet",
        tooltip: "Show Spreadsheet",
    },
    {
        value: "diagram",
        icon: "pie_chart",
        label: "Show Diagram",
        tooltip: "Show Diagram",
        disabled: true
    },
    {
        value: "mappings",
        icon: "swap_horiz",
        label: "Show Data Mappings",
        tooltip: "Show Data Mappings"
    },
    {
        value: "source",
        icon: "code",
        label: "Show Source",
        tooltip: "Show Source"
    },                                    
];

/**
 * ModelEditor
 * 
 * The main component for editing an individual model within the 
 * Admin Model page.  It lays out the model in an organized way
 * that allows the user to navigate down into each part of the model
 * and select actions to perform on the model for both design and 
 * simulations.
 * 
 * It also attaches event listeners to the model object and tracks it
 * for changes, allowing the Save/Review/Discard buttons to be useful.
 */
export const ModelEditor = (props) => {    

    const [state, dispatch] = useReducer(reducer, initialState);

    const savable = useEventId({ object: props.model, eventId: "onSavable" });
    const {notification, isMobile, client} = useApplication();
    const {adaptors} = useAppCore();
    const {pathname, hash} = useLocation();
    const history = useHistory();
    const theme = useTheme();

    const {
        showModelReview, 
        modelDifferences,
        editable, 
        perspective, 
        deploy,
        spinnerMessage,
        optionsTarget,
        mappedAdaptorId,
        showConfirm,
        error,
        reload,
    } = state;
    
    useEffect(() => {
        if (hash) {
            dispatch({ type: "PERSPECTIVE", perspective: hash.substring(1) });
        } else if (theme && !isMobile) {
            /* on larger displays, set the default perspective to Tree view */              
            dispatch({ type: "PERSPECTIVE", perspective: "tree" });
        }
    }, [theme, hash, isMobile]);

    /* display error notifications */
    useEffect(() => {
        if (notification && error) {
            notification({ type: "error", message: error });
        }
    }, [notification, error]);


    /**
     * onSaveModel()
     * 
     * When the user is ready to save changes, without/after review, we perform
     * a reconcile() and return the results in the snackbar.
     */
    const onSaveModel = async () => {
        const model = props.model;

        try {
            dispatch({ type: "MODEL_SAVING" });

            const response = model.reconcile(false);
            await response.result();
                        
            dispatch({ type: "MODEL_SAVED" });            

            notification({ message: "Model Saved.", type: "success", duration: 1000 });
        } catch (error) {  
            dispatch({ type: "ERROR", error });            
        }
    };

    /**
     * onReviewModel()
     * 
     * After changes are made, the user may "Review" the changes by doing a 
     * reconcile() with the test flag on, so we know the differences without 
     * actually changing anything on the live server.
     */
    const onReviewModel = async () => {
        const model = props.model;

        try {
            dispatch({ type: "MODEL_REVIEWING" });

            const response = model.reconcile(true);
            const modelDifferences = await response.result();
                        
            dispatch({ type: "MODEL_REVIEW", modelDifferences });
             
        } catch (error) {         
            dispatch({ type: "ERROR", error });   
        }
    };

    /**
     * onConfirmDiscardModelChanges()
     * 
     * After the user is sure they want to discard/cancel, do it!
     */
    const onConfirmDiscardModelChanges = () => {
        const model = props.model;

        if (savable?.value)
            model.discardChanges();

        dispatch({ type: "MODEL_DISCARD" });        
    };

    /**
     * onDiscardModelChanges()
     * 
     * Undo any editing the user may have made to the current model.
     */
    const onDiscardModelChanges = () => {
        if (savable?.value)
            dispatch({ type: "MODEL_DISCARD_CONFIRM" });            
        else 
            onConfirmDiscardModelChanges();
    };

    /**
     * onSelectPerspective()
     * 
     * Fired off when the user chooses a different perspective from the 
     * Dropdown button.  The perspective propagates down to every child
     * component that renders a part of the Model.
     */
    const onSelectPerspective = (perspective) => { 
        if (perspective) {       
            dispatch({ type: "PERSPECTIVE", perspective });

            history.push( pathname + "#" + perspective );
        }    
    };

    const onRestartModelAdaptor = async (modelAdaptorId) => {
        try {            
            dispatch({ type: "MODEL_ADAPTOR_RESTARTING" });

            await afwServiceRestart(client, "adaptor-" + modelAdaptorId).result();            
            
            dispatch({ type: "MODEL_ADAPTOR_RESTARTED" });
            notification({ message: "Model Adaptor Service has been restarted.", type: "success", duration: 3000 });
        } catch (error) {
            dispatch({ type: "ERROR", error });
        }      
    };

    const {models, model, adaptorId, objectType, propertyType} = props;
    if (!model)
        return null;
    
    return (
        <OperationalContext.Provider value={{ editable }}>
            <div style={{ display: "flex", flexDirection: "column", width: "100%", height: "100%" }}>
                <div style={{ display: "flex", alignItems: "center" }}>
                    <div>
                        <Button 
                            type="icon"
                            icon="more_vert"
                            tooltip="More Actions"
                            color="primary"                    
                            label="More Options"                                           
                            onClick={(event) => dispatch({ type: "MORE_OPTIONS", target: event.target })}                                           
                        />   
                    </div>                                                                                         
                    <div style={{ flex: 1 }}>    
                        <ToggleButtons 
                            exclusive={true} 
                            value={perspective} 
                            onChanged={(value) => onSelectPerspective(value)} 
                            options={isMobile ? mobileOptions : fullOptions}
                        />                                                     
                    </div>   
                    { !editable &&
                        <div style={{ display: "flex", alignItems: "center" }}>
                            <Typography 
                                size="3" 
                                text="Edit Model" 
                            />
                            <Button
                                type="icon"
                                icon="lock"
                                color="primary"
                                tooltip="Unlock to Edit Model."
                                label="Edit Model"
                                onClick={() => dispatch({ type: "EDIT" })}
                            />
                        </div>      
                    }          
                </div>                      
                <div style={{ flex: 1, overflow: "auto" }}>
                    <Switch>
                        <Route exact path={[ 
                            "/Admin/Models/:adaptorId/:modelId", 
                            "/Admin/Models/:adaptorId/:modelId/custom", 
                            "/Admin/Models/:adaptorId/:modelId/custom/:variable", 
                            "/Admin/Models/:adaptorId/:modelId/objectTypes", 
                            "/Admin/Models/:adaptorId/:modelId/propertyTypes" 
                        ]} render={(props) => 
                            <Model 
                                {...props}                                 
                                perspective={perspective}
                                hash={hash}
                                mappedAdaptorId={mappedAdaptorId}
                                adaptorId={adaptorId}
                                models={models}
                                model={model}
                                reload={reload}
                            />
                        } />

                        <Route exact path={[ 
                            "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType",
                            "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/onFunctions",
                            "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/onFunctions/:onFunctionId",
                            "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/propertyTypes",
                            "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/custom",
                            "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/custom/:variable",
                            "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/methods",                            
                        ]} render={(props) => 
                            <ModelObjectTypes
                                {...props}
                                perspective={perspective}
                                hash={hash}
                                mappedAdaptorId={mappedAdaptorId}
                                adaptorId={adaptorId}
                                models={models}
                                model={model}
                                objectType={objectType}
                                reload={reload}
                            />
                        } />

                        <Route exact path={[
                            "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/propertyTypes/:propertyType",
                            "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/propertyTypes/:propertyType/onFunctions",
                            "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/propertyTypes/:propertyType/onFunctions/:onFunctions",
                            "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/propertyTypes/:propertyType/custom",
                            "/Admin/Models/:adaptorId/:modelId/objectTypes/:objectType/propertyTypes/:propertyType/custom/:variable",
                        ]} render={(props) => 
                            <ModelPropertyTypes 
                                {...props}
                                perspective={perspective}
                                hash={hash}
                                mappedAdaptorId={mappedAdaptorId}
                                adaptorId={adaptorId}
                                models={models}
                                model={model}
                                objectType={objectType}
                                propertyType={propertyType}
                                reload={reload}
                            />
                        } />

                        <Route component={NoRoute} />
                    </Switch>
                </div>   
                {                    
                    editable &&                                    
                        <div style={{ display: "flex", padding: theme.spacing(2), borderTop: "solid thin #999", justifyContent: "flex-end" }}>                                                
                            <div style={{ marginRight: theme.spacing(1) }}>
                                <Button 
                                    key="Save Model"
                                    label="Save"
                                    size="small"
                                    type="split"                            
                                    color="primary"    
                                    variant="contained"
                                    icon="save"   
                                    disabled={!(savable && savable.value)}                                           
                                    onClick={onSaveModel}                            
                                    items={[
                                        {
                                            key: "Save",
                                            label: "Save Changes",
                                            icon: "save",
                                            color: "primary",                                    
                                            disabled: !(savable && savable.value),
                                            onClick: onSaveModel,
                                        },
                                        {
                                            key: "Review",
                                            label: "Review Changes",
                                            icon: "find_in_page",
                                            color: "primary",
                                            disabled: !(savable && savable.value),
                                            onClick: onReviewModel,
                                        },
                                        {
                                            key: "Discard",
                                            label: "Discard Changes",
                                            icon: "undo",
                                            color: "primary",
                                            disabled: !(savable && savable.value),
                                            onClick: onDiscardModelChanges,
                                        }   
                                    ]}
                                />                            
                            </div>   
                            <div>
                                <Button 
                                    size="small"
                                    key="Cancel"
                                    label="Cancel"
                                    onClick={onDiscardModelChanges}
                                />
                            </div>                            
                        </div> 
                        
                }                     
            </div>   
            <ModelEditorContextMenu 
                model={model}
                target={optionsTarget}
                onClose={() => dispatch({ type: "NO_OPTIONS" })}
                adaptors={adaptors}
                mappedAdaptorId={mappedAdaptorId}
                onSelectMappedAdaptorId={adaptor => dispatch({ type: "SELECT_MAPPED_ADAPTOR", adaptor })}
                onDeploy={() => dispatch({ type: "DEPLOY" })}
                onRestartModelAdaptor={() => onRestartModelAdaptor(adaptorId)}
                onReloadView={() => {
                    return null;
                }}
                onRefresh={() => {
                    return null;
                }}
                onEditable={editable => dispatch({ type: "ON_EDIT", editable })}
                onRoute={(/*newRoute*/) => {
                    return null;
                }}
                onSave={() => {
                    return null;
                }}
            />         
            <ModelReview
                open={showModelReview}
                onSave={onSaveModel}
                onDismiss={() => dispatch({ type: "REVIEW_DISMISS" })}
                model={model}
                modelDifferences={modelDifferences}
            />          
            <ModelDeploy                 
                open={deploy}
                mappedAdaptorId={mappedAdaptorId}
                onDismiss={() => dispatch({ type: "DEPLOY_DISMISS" })}
                model={model}
            />
            <Dialog 
                open={showConfirm}
                title="Discard Changes"
                subText="Are you sure you want to discard any changes?"
                isBlocking={false}  
                showClose={true}  
                onDismiss={() => dispatch({ type: "CONFIRM_DISMISS" })}
                footer={
                    <div style={{ display: "flex", justifyContent: "right" }}>
                        <div style={{ marginRight: theme.spacing(1) }}>
                            <Button 
                                color="primary"
                                variant="contained"
                                label="Yes"
                                onClick={onConfirmDiscardModelChanges}
                            />
                        </div>
                        <div style={{ marginRight: theme.spacing(1) }}>
                            <Button 
                                label="Cancel"
                                onClick={() => dispatch({ type: "CONFIRM_DISMISS" })}
                            />
                        </div>
                    </div>
                }                
            />
            {
                spinnerMessage && 
                    <Spinner size="large" label={spinnerMessage} fullScreen={true} />
            }
            <Prompt 
                when={(savable && savable.value) ? true : false}
                message={(location) => {                    
                    return (
                        location.pathname.startsWith("/Admin/Models/" + adaptorId + "/") ? true : 
                            "This Model has unsaved changes.  Are you sure you want to leave?"
                    );
                }}
            />   
        </OperationalContext.Provider>
    );
};

ModelEditor.propTypes = {
    model:      PropTypes.instanceOf(AfwObject).isRequired,
};

export default ModelEditor;
