// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect, useCallback, useRef} from "react";

import {
    AdaptorDropdown,
    Button,
    Checkbox,
    Dialog,
    ObjectTypeDropdown,
    Spinner,
    TextField,
    useModel,
} from "@afw/react";

import ObjectEditorLayout from "./ObjectEditorLayout";
import {ErrorDialog} from "../common/ErrorForm";

import {useApplication, useTheme} from "../hooks";

/*
 * ObjectNew
 *
 * This layout presents the user with a way to create a new object from the 
 * Objects route.  It accomplishes this by presenting a two-step Modal popup
 * that allows the user to select the adaptorId and Object Type Id, along wit
 * an optional Object Id and followed by the required and optional properties.
 */
const ObjectNew = (props) => {
    
    const [adaptorId, setAdaptorId] = useState(props.adaptorId);
    const [objectTypeId, setObjectTypeId] = useState(props.objectTypeId);
    const [editNewObjectModal, setEditNewObjectModal] = useState(false);
    const [objectId, setObjectId] = useState();
    const [object, setObject] = useState();
    const [isLoading, setIsLoading] = useState(false);
    const [isInitializing, setIsInitializing] = useState(false);
    const [objectTypeObject, setObjectTypeObject] = useState();
    const [canNext, setCanNext] = useState(false);    
    const [, setObjectChanged] = useState();
    const [error, setError] = useState();    
    const [showError, setShowError] = useState(false);
    const [createAgain, setCreateAgain] = useState(false);
    const [sourceChanged, setSourceChanged] = useState(false);
    const layoutRef = useRef();

    const {open} = props;
    const theme = useTheme();
    const model = useModel();
    const {notification} = useApplication();
    
    useEffect(() => {
        setAdaptorId(props.adaptorId);
        setObjectTypeId(props.objectTypeId);
    }, [props.adaptorId, props.objectTypeId]);

    /**
     * canNext()
     * 
     * Returns true/false if the "Next" button should be enabled.  For example, if
     * the user enters an objectId that already exists, then we shouldn't be able to.
     */
    useEffect(() => {
        let canNext = true;

        /* "afw" adaptor is always read-only */
        if (adaptorId === "afw")
            canNext = false;

        if (!objectTypeId)
            canNext = false;

        if (objectTypeObject && objectTypeObject.getPropertyValue("allowAdd") === false)
            canNext = false;

        setCanNext(canNext);
    }, [adaptorId, objectTypeId, objectTypeObject]);

    /**
     * onSourceChanged()
     * 
     * The object may not have changed, but the source has, if the user has used the CodeEditor.
     */
    const onSourceChanged = useCallback(() => {        
        setSourceChanged(true);
    }, [setSourceChanged]);
    
    /*
     * onDismiss()
     *
     * If the user chooses to cancel the operation, the Modal is simply thrown away and the
     * state is cleared for next time.
     */
    const onDismiss = () => {
        setObjectId();
        setEditNewObjectModal(false);
        setShowError(false);
        setError();
    };

    /*
     * onNext()
     *
     * After the user chooses an Adaptor Id, Object Type Id and (optionally) Object Id, they may 
     * click the "Next" button, which prompts us to prepare a new object which may be edited.
     * The new object is set in the state and a ObjectEditorLayout modal is presented to fill out
     * the remaining properties.
     */
    const onNext = async () => {

        const object = model.newObject({ adaptorId, objectTypeId });
        if (objectId) {
            object.setObjectId(objectId);
            object.setPath("/" + adaptorId + "/" + objectTypeId + "/" + objectId);
        }
        setIsInitializing(true);
        await object.initialize();
        setIsInitializing(false);

        setObject(object);
        setEditNewObjectModal(true);
    };

    /*
     * onCreate()
     *
     * After the user finishes editing the new object, and clicks "Create" this routine simply funnels
     * the operation back up to its parent to be created.  Additionally, it removes the ObjectEditorLayout 
     * modal.
     */
    const onCreate = async () => {
        try {
            setIsLoading(true);
            await props.onAddObject(object, createAgain);
            setIsLoading(false);

            if (createAgain)
                await onNext();
            else 
                setEditNewObjectModal(false);
        } catch (error) {
            notification({message: error, type: "error", snackbar: false });
            setIsLoading(false);
            setError(error);            
            setShowError(true);
        }
    };

    /**
     * onSelectObjectType()
     * 
     * This function fires after the user has selected an Object Type from the dropdown menu.
     */
    const onSelectObjectType = (objectTypeId, objectTypeObject) => {        
        setObjectTypeId(objectTypeId);
        setObjectTypeObject(objectTypeObject);
    };

    /**
     * onObjectIdChanged()
     * 
     * This function fires when the objectId in the TextField is changing.
     */
    const onObjectIdChanged = (objectId) => {
        setObjectId(objectId);
    };


    if (showError) {
        return (
            <ErrorDialog
                error={error}
                onDismiss={onDismiss}
            />
        );
    }

    /*
     * This component displays two separate Modals in two steps.  
     *
     *   First, we setup the adaptor/objectType/objectId.  
     *   Next, we display the Object Editor Layout to begin editing properties.
     * 
     */
    if (!open)
        return null;

    if (isInitializing)
        return <Spinner label="Initializing Object..." fullScreen={true} />;

    if (isLoading)
        return <Spinner label="Creating Object..." fullScreen={true} />;
    

    if (editNewObjectModal) {
        return (
            <Dialog 
                open={open ? open : false}
                onDismiss={onDismiss}
                isBlocking={true}
                title={"New /" + adaptorId + "/" + objectTypeId + "/" + (objectId ? objectId : " Object")}
                maxWidth="xl"
                contains={    
                    <div style={{ height: "calc(100vh - 300px)" }}>                        
                        <ObjectEditorLayout
                            ref={layoutRef} 
                            object={object}
                            addMode={true}
                            editMode={true}                            
                            onChanged={() => setObjectChanged(true)}
                            sourceChanged={sourceChanged}
                            onSourceChanged={onSourceChanged}
                        />                        
                    </div>  
                }
                footer={
                    <div style={{ display: "flex", justifyContent: "space-between", padding: theme.spacing(2), width: "100%" }}>
                        <Checkbox 
                            style={{ display: "inline-block" }}
                            label="Create Another of this Object Type."
                            value={createAgain}
                            onChanged={setCreateAgain}
                        />
                        <div>
                            <div style={{ display: "inline-block", paddingRight: theme.spacing(1) }}>
                                <Button                         
                                    label="Cancel"
                                    onClick={onDismiss}
                                />
                            </div>
                            <div style={{ display: "inline-block" }}>
                                <Button 
                                    color="primary"
                                    variant="contained"
                                    label="Create"                                        
                                    onClick={onCreate}
                                />                                
                            </div>
                        </div>
                    </div>
                }
            />    
        );
    } else {
        return (
            <Dialog 
                open={open ? open : false}
                onDismiss={props.onDismiss}
                isBlocking={false}        
                showClose={true}
                title="New Object"                               
                maxWidth="md"
                contains={
                    <div style={{ padding: theme.spacing(2) }}>          
                        <div style={{ paddingTop: theme.spacing(1) }}>  
                            <AdaptorDropdown 
                                id="objects-new-adaptor-dropdown"
                                label="Adaptor" 
                                description="Enter the Adaptor Id to store this new Object."
                                value={adaptorId}
                                onChanged={adaptorId => setAdaptorId(adaptorId)}
                            />                                     
                        </div>
                        <div style={{ marginTop: theme.spacing(1) }}>   
                            <ObjectTypeDropdown 
                                id="objects-new-objectType-dropdown"
                                adaptorId={adaptorId}
                                label="Object Type"
                                description="Select an Object Type for this new Object."                                
                                value={objectTypeId}
                                onChanged={onSelectObjectType}        
                                requireEntity={true}                                                            
                            /> 
                        </div>
                        <div style={{ marginTop: theme.spacing(1) }}>   
                            <TextField
                                label="Object Id" 
                                description="Enter an optional, suggested Object Id.  If omitted, one will be assigned by the target adaptor."
                                onChanged={onObjectIdChanged}
                                value={objectId}
                            />
                        </div>                        
                    </div>                
                }
                footer={
                    <div style={{ display: "flex", justifyContent: "flex-end", width: "100%", padding: theme.spacing(2) }}>
                        <div>
                            <div style={{ display: "inline-block", paddingRight: theme.spacing(1) }}>
                                <Button                         
                                    label="Cancel"
                                    onClick={props.onDismiss}                                    
                                    color="inherit"
                                />  
                            </div>
                            <div style={{ display: "inline-block" }}>
                                <Button 
                                    color="primary"
                                    variant="contained"
                                    label="Next"
                                    disabled={!canNext}
                                    onClick={onNext}
                                />                                
                            </div>
                        </div>
                    </div>
                }
            />
        );
    }  
};

export default ObjectNew;
