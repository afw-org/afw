// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for ObjectEditor
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


import {useState, useEffect, useReducer, useMemo} from "react";
import PropTypes from "prop-types";
import {useHistory} from "react-router";
import {Prompt} from "react-router-dom";

import useMediaQuery from "@mui/material/useMediaQuery";

import {
    Button,
    Dialog,
    Menu,
    CodeEditor,
    Spinner,
    ToggleButtons,
    Typography,
    ObjectDifferences,
    OperationalContext,
    AdaptiveLayoutMergeProviders,
    useEventId,
    useModel,
    useIsMounted,
    useTheme,
} from "@afw/react";

import {AfwObject, objectGet} from "@afw/client";

/**
 * ObjectEditorHeader displays the header above an Object that provides 
 * a toolbar to view and edit the object.
 */
export const ObjectEditorHeader = (props) => {

    const {
        object,
        allowChange,
        displayMoreButton,
        editable,
        savable,
        showSource,
        sourceState,
        dispatchSource,
        onEditable, 
        objectErrors,
        onSave,
        onReview,
        onDismiss,
        showMeta,
    } = props;

    const [moreOptionsTarget, setMoreOptionsTarget] = useState();

    const history = useHistory();
    const theme = useTheme();

    useEffect(() => {
        
        const onKeyUp = (e) => {            

            if (e.srcElement.nodeName === "BODY" && (e.key === "e" || e.key === "E")) {
                onEditable(true);
            } else if (e.srcElement.nodeName === "BODY" && (e.code === "Escape" || e.key === "Esc"))
                onEditable(false);
        };

        window.addEventListener("keyup", onKeyUp);

        return () => { window.removeEventListener("keyup", onKeyUp); };

    }, [onEditable]);

    return (
        <div style={{
            display: "flex",
            alignItems: "center",
            backgroundColor: theme.palette.background.paper, 
            borderRadius: theme.spacing(0.5),
            minHeight: theme.spacing(6),
        }}>
            <div style={{ flex: 1, display: "flex", alignItems: "center", marginLeft: theme.spacing(1), marginRight: theme.spacing(1)}}>
                {
                    (displayMoreButton !== false) &&
                        <Button
                            type="icon"
                            icon="more_vert"
                            tooltip="More Options"
                            label="More Options"
                            onClick={e => setMoreOptionsTarget(e.target)}
                        />
                }
                <ToggleButtons 
                    exclusive={true}
                    value={showSource ? "Source": "Layout"}
                    options={[
                        {
                            value: "Layout",
                            label: "Show Object in Layout",
                            icon: "view_quilt",
                            tooltip: "Show Object in Layout"
                        },
                        {
                            value: "Source",
                            icon: "code",
                            label: "Show Object Source",
                            tooltip: "Show Object Source"
                        },
                    ]}
                    onChanged={(newLayout) => {
                        if (newLayout)
                            dispatchSource({ type: "TOGGLE_VIEW", showSource: (newLayout === "Source"), object, showMeta });
                    }}
                />
                {
                    (objectErrors && objectErrors.length > 0) &&
                        <Button
                            type="icon"
                            icon="warning"
                            label="This object contains errors"
                            tooltip="This object contains errors"
                        />
                }
                <Menu
                    key="MoreOptionsMenu"
                    target={moreOptionsTarget}
                    open={Boolean(moreOptionsTarget)}
                    onClose={() => setMoreOptionsTarget()}
                    items={[
                        {
                            key: "Edit",
                            label: "Edit Object",
                            icon: "edit",
                            color: "primary",
                            disabled: editable,
                            onClick: () => {
                                onEditable(true);
                                setMoreOptionsTarget();
                            }
                        },
                        {
                            key: "divider",
                            type: "divider"
                        },
                        {
                            key: "Save",
                            label: "Save Changes",
                            icon: "save",
                            color: "primary",
                            disabled: !savable && !sourceState.sourceChanged,
                            onClick: () => {
                                onSave();
                                setMoreOptionsTarget();
                            }
                        },
                        {
                            key: "Review",
                            label: "Review Changes",
                            icon: "find_in_page",
                            color: "primary",
                            disabled: !savable && !sourceState.sourceChanged,
                            onClick: () => {
                                onReview();
                                setMoreOptionsTarget();
                            }
                        },
                        {
                            key: "Discard",
                            label: "Discard Changes",
                            icon: "undo",
                            color: "primary",
                            disabled: !savable && !sourceState.sourceChanged,
                            onClick: () => {
                                onDismiss();
                                setMoreOptionsTarget();
                            }
                        },
                        {
                            key: "divider2",
                            type: "divider",
                        },
                        {
                            key: "ViewObjects",
                            label: "Open in Object Viewer",
                            icon: "open_in_new",
                            color: "primary",
                            onClick: () => {
                                setMoreOptionsTarget();

                                const path = object.getPath();
                                history.push("/Objects" + path);
                            }
                        }
                    ]}
                />
            </div>            
            {
                ((allowChange !== false) && (editable === false)) &&
                <div style={{ display: "flex", alignItems: "center" }}>
                    <Typography 
                        style={{ textDecoration: "underline" }} 
                        size="3" text="E"
                    />
                    <Typography size="3" text="dit Object" />
                    <Button
                        color="primary"
                        type="icon"
                        icon="lock"   
                        label="Edit Object"                     
                        tooltip="Edit Object"
                        onClick={() => onEditable(true)}
                    />
                </div>
            }
        </div>
    );
};

const ignoreMeta = (key, val) => {
    if (key !== "_meta_")
        return val;
};

/*
 * The Source Editor interaction can create a complicated state, because we have to
 * track the toggle between Form and Source, as well as track changes from the Source
 * Editor itself.  Using a reducer aims to simplify this and avoid cyclical useEffect 
 * logic.
 */
const sourceReducer = (state, action) => {
    switch(action.type) {
    // when the user changes anything in the source editor
    case "SOURCE_CHANGED":
        return { 
            ...state, 
            sourceChanged: true, 
            objectSource: action.objectSource,                    
        };            

    case "DISCARD_CHANGES":            
        return {
            ...state, 
            objectSource: action.object ? action.object.stringify( !action.showMeta ? ignoreMeta : null ) : "",         
            sourceChanged: false 
        };            

    case "TOGGLE_VIEW":
        // when the user toggles the Source/Form button            
        if (action.showSource) {
            // about to show source view
            const objectSource = action.object ? action.object.stringify( !action.showMeta ? ignoreMeta : null ) : "";            
            let schema = state.schema;
            if (action.object && !schema)
                schema = action.object.getJSONSchema();

            return {
                ...state, 
                showSource: action.showSource,
                objectSource,          
                schema,          
            };
        } else {
            // about to show Form view
            if (state.sourceChanged) {
                //! \fixme, if it's not parsable, we can't do this yet                
                action.object.fromJSON(state.objectSource);
            }

            return {
                ...state, 
                showSource: action.showSource,
            };
        }     

    default:
        return state;
    }
};

/**
 * ObjectError
 * 
 * Small dialog to display an error.
 */
const ObjectError = ({ error, onDismiss }) => {

    let errorString;
    if (error) {
        if (objectGet(error, ["response", "data", "message"]))        
            errorString = error.response.data.message;
        else if (objectGet(error, ["response", "statusText"]))
            errorString = error.response.statusText;
        else if (error.message)
            errorString = error.message;
        else
            errorString = String(error);
    } else
        errorString = "An Error occurred.";

    return (
        <Dialog 
            open={Boolean(error)}
            onDismiss={onDismiss}
            isBlocking={false}
            title="Error"
            maxWidth="sm"
            showClose={true}
            contains={
                <Typography component="span" text={errorString} />
            }
        />
    );
};


/**
 * ObjectEditor
 * 
 * This is the main component that renders a tool for editing an object, 
 * including its header, view, save/cancel buttons and a context menu.
 */
export const ObjectEditor = (props) => {

    const [editable, setEditable] = useState(false);
    const [showConfirmDismiss, setShowConfirmDismiss] = useState(false);
    const [spinnerText, setSpinnerText] = useState();
    const [showReview, setShowReview] = useState(false);
    const [differences, setDifferences] = useState();
    const [error, setError] = useState();    
    const [object, setObject] = useState();
    const isMounted = useIsMounted();

    const {
        displaySaveButton, 
        displayCancelButton, 
        displayMoreButton,
        displayHeader,
        displaySourceButton,
        layout,
        showMeta = false,
        warnNavigateAway
    } = props;

    const model = useModel();
    const theme = useTheme();
    const isSmall = !useMediaQuery(theme.breakpoints.up("sm"));

    useEffect(() => {

        const initObject = async (obj) => {
            const object = model.newObject({ object: obj });
            await object.initialize();
            if (isMounted())
                setObject(object);
        };

        if (props.object && props.object instanceof AfwObject) {
            setObject(props.object);
        } else if (props.object && model)
            initObject(props.object);


    }, [props.object, model, isMounted]);

    const objectErrors = useMemo(() => object?.getErrors(), [object]);        

    const savable = useEventId({ object, eventId: "onSavable" });

    const initialSourceState = {
        sourceChanged: false,
        objectSource: "",
        showSource: false,
        schema: object?.getJSONSchema()
    };
   
    const [sourceState, dispatchSource] = useReducer(sourceReducer, initialSourceState);
  
    if (!object)
        return null;    

    const allowChange = object.getAllowChange();
    
    const onSave = async () => {
        try {
            if (!(savable && savable.true) && sourceState.sourceChanged) {
                /* first, we must parse out the source changes */
                object.fromJSON(sourceState.objectSource);
            }

            setSpinnerText("Saving Changes...");

            await object.reconcile().result();
            if (isMounted()) {
                setEditable(false);
                setShowReview(false);
                    
                props.onEditable && props.onEditable(false);
                props.onSaved && props.onSaved();
            }
        } catch (error) {
            setError(error);
        }

        if (isMounted())
            setSpinnerText();        
    };

    const onConfirmDismiss = () => {        
        if (savable && savable.value)
            object.discardChanges();

        setEditable(false);            

        if (props.onEditable)
            props.onEditable(false);
        
        dispatchSource({ type: "DISCARD_CHANGES", object });
    };

    const onDismiss = () => {
        if (savable && savable.value)
            setShowConfirmDismiss(true);
        else
            onConfirmDismiss();
    };

    const onReview = async () => {
        try {
            if (!(savable && savable.value) && sourceState.sourceChanged) {
                /* first, we must parse out the source changes */
                object.fromJSON(sourceState.objectSource);
            }

            setSpinnerText("Reviewing...");

            const differences = await object.reconcile(true).result();            
            if (isMounted()) {
                setDifferences(differences);
                setShowReview(true);
            }

        } catch (error) {
            setError(error);
        }

        if (isMounted())
            setSpinnerText();
    };

    const onEditable = (editable) => {
        if (props.onEditable)
            props.onEditable(editable);

        setEditable(editable);
    };

    if (spinnerText) {
        return <Spinner size="large" text={spinnerText} />;        
    }

    const headerProps = {
        object,
        allowChange,
        displayCancelButton,
        displayMoreButton,
        displaySourceButton,
        displaySaveButton,
        editable,
        showSource: sourceState.showSource,
        showMeta,
        sourceState,
        dispatchSource,
        onEditable,
        onDismiss,
        onReview,
        onSave,
        savable,
        objectErrors,
    };

    return (
        <AdaptiveLayoutMergeProviders data={{ object }}>            
            <OperationalContext.Provider value={{ editable }}>
                <div style={{ display: "flex", flexDirection: "column", height: "100%" }}>
                    { (displayHeader !== false) &&
                        <ObjectEditorHeader {...headerProps} />
                    }
                    <div style={{
                        flex: 1, width: "100%", height: "100%", 
                        overflow: "auto", display: "inline-block"
                    }}>
                        {
                            sourceState.showSource ? 
                                <CodeEditor 
                                    // eslint-disable-next-line jsx-a11y/no-autofocus
                                    autoFocus={true}
                                    showLineNumbers={true}
                                    source={sourceState.objectSource}
                                    showMinimap={isSmall ? false : true}
                                    style={{ height: "100%", /* account for our header */ }}
                                    readOnly={!editable}             
                                    schema={sourceState.schema}
                                    onChanged={objectSource => {
                                        if (objectSource !== sourceState.objectSource)
                                            dispatchSource({ type: "SOURCE_CHANGED", objectSource, object });
                                    }}
                                />
                                : layout
                        }
                    </div>   
                    {
                        (editable === true) &&
                            <div style={{ 
                                display: "flex", 
                                justifyContent: "flex-end", 
                                alignItems: "center",
                                width: "100%", 
                                padding: theme.spacing(2), 
                                borderTop: "solid thin #999" }}
                            >
                                {
                                    (displaySaveButton !== false) &&
                                        <div style={{ marginRight: theme.spacing(1) }}>
                                            <Button                            
                                                size="small"                                                        
                                                color="primary"
                                                variant="contained"
                                                icon="save"
                                                disabled={!(savable && savable.value) && !sourceState.sourceChanged}
                                                label="Save"
                                                onClick={onSave}
                                                type="split"
                                                items={[                                                    
                                                    {
                                                        key: "Save",
                                                        label: "Save Changes",
                                                        icon: "save",
                                                        color: "primary",
                                                        disabled: !(savable && savable.value) && !sourceState.sourceChanged,
                                                        onClick: onSave
                                                    },
                                                    {
                                                        key: "Review",
                                                        label: "Review Changes",
                                                        icon: "find_in_page",
                                                        color: "primary",
                                                        disabled: !(savable && savable.value) && !sourceState.sourceChanged,
                                                        onClick: onReview
                                                    },
                                                    {
                                                        key: "Discard",
                                                        label: "Discard Changes",
                                                        icon: "undo",
                                                        color: "primary",
                                                        disabled: !(savable && savable.value) && !sourceState.sourceChanged,
                                                        onClick: onDismiss
                                                    }
                                                ]}
                                            />
                                        </div>
                                }
                                {
                                    (displayCancelButton !== false) &&
                                        <div style={{ marginRight: theme.spacing(1) }}>
                                            <Button
                                                size="small"
                                                icon="cancel"
                                                label="Cancel"
                                                onClick={onDismiss}
                                                disabled={!editable}
                                            />
                                        </div>
                                }
                            </div>
                    }
                    <Dialog 
                        open={showReview}
                        onDismiss={() => setShowReview(false)}
                        isBlocking={false}
                        title="Review Changes"   
                        showClose={true}
                        maxWidth="xl"
                        contains={
                            <ObjectDifferences 
                                object={object}
                                objectDifferences={differences}
                            />
                        }
                        footer={
                            <div style={{ display: "flex", padding: theme.spacing(2), width: "100%", textAlign: "right" }}>
                                <div style={{ marginRight: theme.spacing(1) }}>
                                    <Button
                                        variant="contained"
                                        color="primary"                                        
                                        label="Apply Changes"
                                        onClick={onSave}                            
                                    />        
                                </div>
                                <div>
                                    <Button
                                        style={{ height: theme.spacing(4), marginLeft: theme.spacing(1) }}
                                        label="Back"
                                        onClick={() => setShowReview(false)}
                                    />
                                </div>
                            </div>
                        }
                    />
                    <Dialog 
                        open={showConfirmDismiss}
                        onDismiss={() => setShowConfirmDismiss(false)}
                        isBlocking={false}
                        maxWidth="sm"
                        title="Discard Changes"
                        showClose={true}
                        contains={
                            <Typography component="span" text="This Object has changed.  Discard the changes?" />
                        }
                        footer={
                            <div style={{ display: "flex", justifyContent: "flex-end", padding: theme.spacing(2), width: "100%" }}>
                                <div style={{ marginRight: theme.spacing(1) }}>
                                    <Button
                                        label="Yes"
                                        color="primary"
                                        variant="contained"
                                        onClick={() => {
                                            setShowConfirmDismiss(false);
                                            onConfirmDismiss();
                                        }}
                                    />
                                </div>
                                <div>                                    
                                    <Button
                                        label="No"
                                        onClick={() => setShowConfirmDismiss(false)}
                                    />
                                </div>
                            </div>
                        }
                    />
                    <ObjectError 
                        error={error}
                        onDismiss={() => setError()}
                    />                    
                    {
                        (warnNavigateAway !== false) &&
                        <Prompt 
                            when={(savable && savable.value) ? true : false}
                            message={() => "You have unsaved changes.  Are you sure you want to leave?"}
                        />
                    }
                </div>
            </OperationalContext.Provider>            
        </AdaptiveLayoutMergeProviders>
    );
};

/**
 * PropTypes generated by _AdaptiveLayoutComponentType_ObjectEditor
 */
ObjectEditor.propTypes = {
    // (boolean)
    displayCancelButton:                PropTypes.bool,
    // (boolean)
    displaySaveButton:                  PropTypes.bool,
    // (object, _AdaptiveLayoutComponentType_)
    layout:                             PropTypes.oneOfType([ PropTypes.element, PropTypes.object ]),
    // (boolean)
    warnNavigateAway:                   PropTypes.bool,
};

ObjectEditor.defaultProps = {
    displayMoreButton:                  true,
    displayCancelButton:                true,
    displaySaveButton:                  true,
    warnNavigateAway:                   true,
};

ObjectEditor.displayName = "ObjectEditor";

export default ObjectEditor;
