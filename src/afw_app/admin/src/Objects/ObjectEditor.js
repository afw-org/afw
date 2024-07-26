// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect, useCallback, useRef} from "react";
import {Prompt, useRouteMatch, useLocation} from "react-router";

import ObjectEditorLayout from "./ObjectEditorLayout";
import ObjectDifferencesModal from "./ObjectDifferencesModal";

import {
    Button,
    Dialog,
    Spinner,
    Typography,
    useGetObject,
    RouteBasePathContext,
    useIsMounted,
} from "@afw/react";

import {useApplication, useTheme} from "../hooks";

/* default Object Options */
const objectOptions = {
    objectId: true,
    objectType: true,
    path: true,
    normalize: true,
    composite: true,
    resolvedParentPaths: true,
    inheritedFrom: true,
};

/*
 * ObjectEditor
 *
 * The Object Editor wraps ObjectEditorLayout and provides it with Back and
 * Save/Discard/Review buttons so the user can save the work that they have
 * been doing.
 */
const ObjectEditor = ({ onNextObject, onPreviousObject, onSelectObject }) => {

    const [sourceChanged, setSourceChanged] = useState(false);
    const [showDismissWarning, setShowDismissWarning] = useState(false);    
    const [objectDifferences, setObjectDifferences] = useState();
    const [showObjectDifferencesModal, setShowObjectDifferencesModal] = useState(false);
    const [editMode, setEditMode] = useState();
    const [adapterId, setAdapterId] = useState();
    const [objectTypeId, setObjectTypeId] = useState();
    const [objectId, setObjectId] = useState();
    const [isProcessing, setIsProcessing] = useState();
    const [embeddedObject, setEmbeddedObject] = useState();
    
    const {object, isLoading, error, savable} = useGetObject({ adapterId, objectTypeId, objectId, objectOptions });
    const {notification} = useApplication();
    const match = useRouteMatch();
    const location = useLocation();
    const theme = useTheme();    
    const isMounted = useIsMounted();
    const layoutRef = useRef();

    useEffect(() => {
        if (match.params) {
            const {adapterId, objectTypeId, objectId} = match.params;

            setAdapterId( decodeURIComponent(adapterId) );
            setObjectTypeId( decodeURIComponent(objectTypeId) );
            setObjectId( decodeURIComponent(objectId) );

            if (!match.isExact) {
                /* we must have requested an embeddedObject */
                const embeddedObject = location.pathname.split("/" + adapterId + "/" + objectTypeId + "/" + objectId + "/")[1];
                setEmbeddedObject(embeddedObject.split("/"));
            } else {
                setEmbeddedObject();
            }
        }
    }, [match, location]);

    useEffect(() => onSelectObject(object), [onSelectObject, object]);

    /**
     * lightDismiss()
     * 
     * Dismisses edit mode without discarding anything.
     */
    const lightDismiss = useCallback( () => {

        setSourceChanged(false);
        setShowDismissWarning(false);
        setEditMode(false);
    }, [setSourceChanged, setShowDismissWarning]);

    /*
     * confirmDismiss()
     *
     * Once the user confirms that they no wish to discard changes, the object changes
     * are actually discarded, the parent is notified, and the warning is dismissed.
     */
    const confirmDismiss = () => {
        setSourceChanged(false);
        setShowDismissWarning(false);

        object.discardChanges();
        
        setEditMode(false);
    };

    /*
     * onSave()
     *
     * When the user clicks on the "Save" mode, this routine will actually save the object back out to the store
     * and notify the parent.
     */
    const onSave = useCallback( async () => {

        /* first let the layout do any last-second preparation for the save, such as parsing */
        layoutRef.current.onFinalizeObject();

        try {
            setIsProcessing(true);
            const response = object.reconcile();
            await response.result();

            if (isMounted()) {
                setSourceChanged(false);
                setShowObjectDifferencesModal(false);
                setEditMode(false);

                notification({ message: "Object " + object.getObjectId() + " saved.", type: "success", response });
            }
        } catch (error) {
            setShowObjectDifferencesModal(false);
            notification({ message: error, type: "error" });
        }

        if (isMounted())
            setIsProcessing(false);
    }, [layoutRef, object, setIsProcessing, setSourceChanged, setShowObjectDifferencesModal, setEditMode, notification, isMounted]);

    /*
     * onReplace()
     *
     * The replace() operation allows the user to ignore the object state in the current adapter and go ahead
     * and replace its contents with the contents of the current object.
     */
    const onReplace = useCallback( async () => {
        /* first let the layout do any last-second preparation for the save, such as parsing */
        layoutRef.current.onFinalizeObject();

        try {
            setIsProcessing(true);
            const response = object.replace();
            await response.result();

            if (isMounted()) {
                setSourceChanged(false);
                setEditMode(false);

                notification({ message: "Object " + object.getObjectId() + " replaced.", type: "success", response });
            }
        } catch (error) {
            notification({ message: error, type: "error" });
        }

        if (isMounted())
            setIsProcessing(false);
    }, [layoutRef, object, setIsProcessing, setSourceChanged, setEditMode, notification, isMounted]);

    /*
     * onReconcile()
     *
     * The reconcile() operation allows the user to leverage the reconcilable metadata and save changes back out
     * to the adapter.  If the object has been updated since it was last retrieved, it undergoes a reconcile process
     * to attempt to merge our changes back in with the others.
     */
    const onReconcile = useCallback( async () => {
        /* first let the layout do any last-second preparation for the save, such as parsing */
        layoutRef.current.onFinalizeObject();

        try {
            setIsProcessing(true);
            const response = object.reconcile(false);
            const result = await response.result();

            if (isMounted()) {
                setSourceChanged(false);
                setEditMode(false);

                notification({ message: "Object reconciled.", type: "success", response: result });
            }
        } catch (error) {
            notification({ message: error, type: "error" });
        }

        if (isMounted())
            setIsProcessing(false);
    }, [layoutRef, object, setIsProcessing, setSourceChanged, setEditMode, notification, isMounted]);

    /*
     * onReview()
     *
     * The user may click to "Review" changes of an object to see the "diff" details and confirm that they
     * definitely want to save the changes in question.  This routine displays the Object Differences
     * Modal for this purpose.
     */
    const onReview = useCallback( async () => {
        /* first let the layout do any last-second preparation for the save, such as parsing */
        layoutRef.current.onFinalizeObject();

        try {
            setIsProcessing(true);
            const response = object.reconcile(true);
            const objectDifferences = await response.result();

            if (isMounted()) {
                setObjectDifferences(objectDifferences);
                setShowObjectDifferencesModal(true);
            }
        } catch (error) {
            notification({ message: error, type: "error" });
        }

        if (isMounted())
            setIsProcessing(false);
    }, [layoutRef, object, setIsProcessing, setObjectDifferences, setShowObjectDifferencesModal, notification, isMounted]);

    /*
     * onDiscard()
     *
     * The user has intentionally selected to discard changes on the object by clicking on the
     * Discard Changes button option.
     */
    const onDiscard = useCallback( () => {

        if (sourceChanged || savable)
            setShowDismissWarning(true);
        else 
            lightDismiss();
    }, [sourceChanged, setShowDismissWarning, lightDismiss, savable]);

    /**
     * onSourceChanged()
     * 
     * The object may not have changed, but the source has, if the user has used the CodeEditor.
     */
    const onSourceChanged = useCallback(() => {        
        setSourceChanged(true);
    }, [setSourceChanged]);

    /*
     * onRefresh()
     *
     * The user has requested new object options and wishes to refresh the object.
     */
    const onRefresh = useCallback( async (objectOptions) => {
        try {
            object.refresh(objectOptions);
        } catch (error) {
            notification({ message: error, type: "error" });
        }
    }, [object, notification]);

    if (isLoading)
        /* spin until the object is loaded */
        return <Spinner size="large" label="Loading Object..." fullScreen={true} />;

    if (error) {
        return (
            <div>
                <div style={{ height: "10vh" }} />
                <div style={{ textAlign: "center" }}>
                    <Typography text={error.message} />
                </div>
            </div>
        );
    }

    if (!object || (embeddedObject && !object.getPropertyValue(embeddedObject))) {
        /* still don't have an object, but also not an error or loading.. */
        return null;
    }

    let obj = object;    
    if (embeddedObject && object.getProperty(embeddedObject) && object.getProperty(embeddedObject).getDataType() !== "object") {
        /* specified an embedded property that isn't an object */
        obj = object.getProperty(embeddedObject).getEmbeddingObject();
    } else if (embeddedObject) {
        obj = object.getPropertyValue(embeddedObject);
    }

    return (
        <>
            <RouteBasePathContext.Provider value="/Objects">
                <ObjectEditorLayout
                    object={obj}
                    ref={layoutRef}
                    sourceChanged={sourceChanged}
                    onSourceChanged={onSourceChanged}
                    editMode={editMode}
                    onEditMode={setEditMode}
                    onSave={onSave}
                    onReplace={onReplace}
                    onReconcile={onReconcile}
                    onReview={onReview}
                    onDiscard={onDiscard}
                    onRefresh={onRefresh}
                    onNextObject={onNextObject}
                    onPreviousObject={onPreviousObject}
                />
            </RouteBasePathContext.Provider>            
            <Dialog 
                open={showDismissWarning}
                title="Discard Changes"
                subText="Are you sure you want to discard changes made to this object?"
                isBlocking={false}  
                showClose={true}  
                onDismiss={() => setShowDismissWarning(false)}
                footer={
                    <div style={{ display: "flex", justifyContent: "flex-end" }}>
                        <div style={{ marginRight: theme.spacing(1) }}>
                            <Button 
                                color="primary"
                                variant="contained"
                                label="Yes"
                                onClick={confirmDismiss}
                            />
                        </div>
                        <div style={{ marginRight: theme.spacing(1) }}>
                            <Button 
                                label="No"
                                onClick={() => setShowDismissWarning(false)}
                            />
                        </div>
                    </div>
                }                
            />
            { showObjectDifferencesModal &&
                <ObjectDifferencesModal
                    open={showObjectDifferencesModal}
                    object={object}
                    objectDifferences={objectDifferences}
                    onDismiss={() => setShowObjectDifferencesModal(false)}
                    onSave={onSave}
                />
            }
            {
                isProcessing &&
                    <Spinner size="large" label="Processing..." fullScreen={true} />
            }
            <Prompt
                when={(savable || sourceChanged) ? true : false}
                message={location => 
                    location.pathname.startsWith("/Objects/" + adapterId + "/" + objectTypeId + "/" + objectId) ? true : 
                        "This Object has unsaved changes.  Are you sure you want to leave?" }
            />            
        </>
    );
};

export default ObjectEditor;
