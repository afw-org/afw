// See the 'COPYING' file in the project root for licensing information.
import {useState, useReducer, useEffect, useImperativeHandle, forwardRef} from "react";
import {useLocation, useHistory} from "react-router";
import PropTypes from "prop-types";

import {
    Button,
    Dialog,
    Divider,        
    Menu,
    Message,
    ObjectFlattened,
    ObjectTree,
    ObjectLayout,
    ObjectResponsive,
    PropertyResponsive,    
    Typography,
    CodeEditor,
    Spinner,
    ToggleButtons,
    useIsMounted,
    useEventId,
    useModel,
    useOperational,
    useValues,            
    OperationalContext,
    AdaptiveLayoutContext,
    ObjectEditorContext,
} from "@afw/react";

import {AfwObject, AfwProperty} from "@afw/client";
import {useApplication, useTheme, useClasses} from "../hooks";

import {ObjectEditorLayoutParameters, defaultLayoutParameters} from "./ObjectEditorLayoutParameters";
import {LayoutViewContextMenu} from "./ObjectEditorLayoutViewContextMenu";

import useMediaQuery from "@mui/material/useMediaQuery";



/*
 * ObjectError
 *
 * A component that indicates this object has an error and allows the user to explore
 * the contents of the error.
 */
const ObjectError = ({ object }) => {

    const theme = useTheme();
    const [details, setDetails] = useState(false);

    return (
        <>
            <div style={{ padding: theme.spacing(1) }}>
                <Message
                    status="warning"
                    contains={
                        <div>
                            <Typography
                                style={{ display: "inline-block", paddingRight: theme.spacing(1) }}
                                text="This object contains errors."
                            />
                            <button
                                style={{ display: "inline-block", background: "inherit", color: "inherit", border: "none", padding: "0", textDecoration: "underline", cursor: "pointer" }}
                                onClick={() => setDetails(true)}
                            ><Typography text="See details."/></button>

                        </div>
                    }
                    message={object.getErrors().join("\n")}
                />
            </div>
            <Dialog
                open={details}
                isBlocking={false}
                showClose={true}
                onDismiss={() => setDetails(false)}
                maxWidth="lg"
                title={"Object Error Details for " + object.getPath()}
                contains={
                    <div style={{ padding: theme.spacing(2) }}>
                        <div style={{ display: "flex", justifyContent: "space-around" }}>
                            <div style={{ paddingRight: theme.spacing(2) }}>
                                <Typography color="textSecondary" text="Property Name" />
                            </div>
                            <Typography color="textSecondary" text="Error(s)" />
                        </div>    
                        <div>
                            <div style={{ height: theme.spacing(2) }} />
                        </div>  
                        <Divider />
                        <div style={{ height: theme.spacing(4) }} />
                        <span style={{ whiteSpace: "pre-line" }}>
                            {
                                object.getErrors().map((error, index) =>
                                    <div key={index}>
                                        <div style={{ display: "flex", justifyContent: "space-around" }}>
                                            <div style={{ paddingRight: theme.spacing(2) }}>
                                                <Typography text={error.propertyName} />
                                            </div>
                                            <Typography text={error.errors} />
                                        </div>               
                                        <div>
                                            <div style={{ height: theme.spacing(2) }} />
                                        </div>                                                                 
                                        <Divider />
                                        <div style={{ height: theme.spacing(2) }} />
                                    </div>
                                )
                            }
                        </span>
                    </div>
                }
            />
        </>
    );
};

ObjectError.propTypes = {
    object:         PropTypes.instanceOf(AfwObject),
};

/**
 * PropertyEditor
 * 
 * When the user uses the context menu to edit a property, this is
 * rendered in a Dialog box for quick property editing, without having
 * to render the entire object in edit mode. 
 * 
 * \fixme this doesn't allow you to save anywhere yet.
 */
const PropertyEditor = ({ open, property, onDismiss, onSave }) => {

    const theme = useTheme();
    if (!open || !property)
        return null;

    return (
        <Dialog
            open={open}
            isBlocking={true}
            onDismiss={onDismiss}
            showClose={true}
            title={"Edit Property"}
            maxWidth="md"
            contains={
                <div style={{ padding: theme.spacing(2) }}>
                    <PropertyResponsive
                        //eslint-disable-next-line
                        autoFocus={true}
                        property={property}
                        editable={true}
                    />
                </div>
            }
            footer={
                <div style={{ padding: theme.spacing(2), textAlign: "right" }}>
                    <div style={{ display: "inline-block", paddingRight: theme.spacing(1) }}>
                        <Button
                            label="Ok"
                            color="primary"
                            variant="contained"
                            onClick={() => onSave()}
                        />
                    </div>
                    <div style={{ display: "inline-block", paddingRight: theme.spacing(1) }}>
                        <Button
                            label="Cancel"
                            onClick={() => {
                                property.discardChanges();
                                onDismiss();
                            }}
                        />
                    </div>
                </div>
            }
        />
    );
};

PropertyEditor.propTypes = {
    open:           PropTypes.bool,
    property:       PropTypes.instanceOf(AfwProperty), 
    onDismiss:      PropTypes.func, 
    onSave:         PropTypes.func,
};

/**
 * PropertyMetadata
 * 
 * This renders a dialog that displays the meta for a property. 
 */
const PropertyMetadata = ({ open, property, onDismiss }) => {

    const theme = useTheme();
    const model = useModel();
    const isMounted = useIsMounted();
    const [metadata, setMetadata] = useState();

    useEffect(() => {
        const fetchMeta = async() => {
            const metadata = model.newObject({
                model,
                adaptorId: property.getEmbeddingObject().getAdaptorId(),
                objectTypeId: "_AdaptiveValueMeta_",
                object: property.getMeta()
            });

            await metadata.initialize();  
            if (isMounted())          
                setMetadata(metadata);
        };

        if (open && property)
            fetchMeta();

    }, [open, property, model, isMounted]);

    if (!open || !property || !metadata)
        return null;

    return (
        <Dialog
            open={open}
            isBlocking={false}
            onDismiss={onDismiss}
            showClose={true}
            title={"Property " + property.getName()}
            maxWidth="md"
            contains={
                <div style={{ padding: theme.spacing(2) }}>
                    <ObjectResponsive
                        object={metadata}
                        editable={false}
                        filterOptions={{
                            filterValuedProperties: true
                        }}
                    />
                </div>
            }
        />
    );
};

PropertyMetadata.propTypes = {
    open:           PropTypes.bool,
    property:       PropTypes.instanceOf(AfwProperty), 
    onDismiss:      PropTypes.func, 
};

/**
 * ObjectMetadata
 * 
 * This displays the object's metadata in a Dialog, when the 
 * user selects it from the right-click context menu.
 */
const ObjectMetadata = ({ open, object, onDismiss }) => {

    const theme = useTheme();
    if (!open || !object)
        return null;

    return (
        <Dialog
            open={open}
            isBlocking={false}
            onDismiss={onDismiss}
            showClose={true}
            title={"Object Type " + object.getObjectTypeId()}
            maxWidth="xl"
            contains={
                <div style={{ height: "80vh", overflow: "auto", padding: theme.spacing(2) }}>
                    <ObjectResponsive
                        object={object.getObjectTypeObject()}
                        editable={false}
                        filterOptions={{
                            filterValuedProperties: true
                        }}
                    />
                </div>
            }
        />
    );
};

ObjectMetadata.propTypes = {
    object:         PropTypes.instanceOf(AfwObject),
    open:           PropTypes.bool,
    onDismiss:      PropTypes.func,
};


/*
 * onRenderToggleEdit()
 *
 * This routine renders the buttons for entering or exiting "edit" mode.
 */
const ToggleEdit = ({ refreshing, object, onEditObject }) => {

    const {editable} = useOperational();
    const theme = useTheme();
    const isSmall = !useMediaQuery(theme.breakpoints.up("sm"));

    useEffect(() => {
        
        const onKeyUp = (e) => {            

            if (e.srcElement.nodeName === "BODY" && (e.key === "e" || e.key === "E")) {
                onEditObject(true);
            } else if (e.srcElement.nodeName === "BODY" && (e.code === "Escape" || e.key === "Esc"))
                onEditObject(false);
        };

        window.addEventListener("keyup", onKeyUp);

        return () => { window.removeEventListener("keyup", onKeyUp); };

    }, [onEditObject]);

    if (refreshing)
        return <Spinner size="small"/>;

    if (editable)
        return null;    

    return (
        <div style={{ display: "flex", alignItems: "center" }}>
            { !isSmall && ((object.getAllowChange() !== false) ?
                <>
                    <Typography style={{ textDecoration: "underline" }} size="3" text="E" />
                    <Typography style={{ marginRight: theme.spacing(0.5) }} size="3" text="dit Object" />
                </>
                :
                <Typography style={{ marginRight: theme.spacing(0.5) }} size="3" text="Object is read-only" />)
            }
            <Button
                label="Edit Object"
                tooltip="Edit Object"
                type="icon"
                icon="lock"
                color="primary"
                onClick={() => onEditObject(true)}
                disabled={object.getAllowChange() === false}
            />
        </div>
    );
};

ToggleEdit.propTypes = {
    refreshing:         PropTypes.bool,
    object:             PropTypes.instanceOf(AfwObject),
    onEditObject:       PropTypes.func,
};

/* Default object options when entering "editing" mode. */
const initialObjectOptions = {
    objectId: true,
    objectType: true,
    path: true,
    normalize: true,
    composite: true,
    resolvedParentPaths: true,
    inheritedFrom: true,
    reconcilable: true,
};


const styles = theme => ({
    ObjectResponsiveBodyClass: {
        padding: theme.spacing(2),
        flexGrow: 1,
        overflow: "auto",
    }
});

/**
 * OperationButtons
 * 
 * This component displays the Save (and Replace/Reconcile/Discard) Button, along with the Cancel 
 * Button.  It will enable/disable based upon whether the source has changed, or the object has.
 */
const OperationButtons = ({ object, sourceChanged, onSave, onReplace, onReconcile, onReview, onDiscard }) => {

    const theme = useTheme();
    const {editable} = useOperational();
    const savable = useEventId({ object, eventId: "onSavable" });

    if (!editable)
        return null;

    return (
        <>
            <Button
                label="Save"
                size="small"
                onClick={onSave}
                disabled={!savable && !sourceChanged}
                color="primary"
                variant="contained"
                type="split"
                items={[
                    {
                        key: "replace",
                        label: "Replace Object",
                        onClick: onReplace,
                    },
                    {
                        key: "reconcile",
                        label: "Reconcile Object",
                        onClick: onReconcile
                    },
                    {
                        key: "reconcileTest",
                        label: "Review Changes",
                        onClick: onReview,
                    },
                    {
                        key: "discardChanges",
                        label: "Discard Changes",
                        onClick: onDiscard,
                    }
                ]}
            />                          
            <div style={{ marginLeft: theme.spacing(1), marginRight: theme.spacing(1) }}>
                <Button
                    size="small"
                    label="Cancel"
                    onClick={onDiscard}
                />
            </div>
        </>
    );
};

/** 
 * Based on the layout chosen, return the appropriate component.
 * 
 * The Flattened and Tree layouts are only conditionally rendered 
 * in order to keep memory usage from blowing up with really large 
 * objects.
 */
const RenderedLayout = (props) => {
    
    const {
        object, layout, onRenderHeader, disableContextualMenu, setContextMenu,        
        objectJSON, objectJSONSchema, setObjectJSON, onSourceChanged, sourceLayoutDisplayed,
    } = props;

    const theme = useTheme();
    const {editable} = useOperational();
    const classes = useClasses(styles);
    

    /*
     * onPropertyContextMenu()
     *
     * Fired off when the user right-clicks on a property.
     */
    const onPropertyContextMenu = (event, property) => {
        if (event.target && event.target.nodeName === "A") {
            /* don't interrupt right-clicks for links */
            return;
        }

        event.preventDefault();
        event.stopPropagation();               

        setContextMenu(property, event);
    };

    /*
     * onRenderProperty()
     *
     * The ObjectResponsive component allows for a callback when rendering properties of objects.
     * We use this callback to add some padding and a divider between properties, as well as
     * a contextual menu (right-click) in order to drill down and view metadata.
     */
    const onRenderProperty = (propertyProps, property, renderedProperty, index, count) => {

        if (editable === true) {
            return (
                <>
                    {renderedProperty}
                    {(index < count - 1) ?                        
                        <Divider style={{ width: "98%" }} />
                        : null
                    }
                </>
            );
        } else {
            if (disableContextualMenu !== true) {
                return (
                    <div
                        onContextMenu={(event) => onPropertyContextMenu(event, property)}>
                        {renderedProperty}
                    </div>
                );
            } else {
                return (
                    <div>
                        {renderedProperty}
                    </div>
                );
            }
        }
    };

    return (
        <>
            <div style={{ display: (layout === "responsive") ? "block" : "none", height: "100%" }}>    
                <ObjectEditorContext.Provider value={{ onRenderProperty, onRenderHeader }}>
                    <ObjectResponsive
                        style={{ display: "flex", flexDirection: "column", height: "100%" }}        
                        //eslint-disable-next-line                    
                        autoFocus={true}
                        bodyClassName={classes.ObjectResponsiveBodyClass}    
                        headerOptions="all"                        
                    >
                        <ObjectLayout />
                    </ObjectResponsive>
                </ObjectEditorContext.Provider>
            </div>
            <div style={{
                display: (layout === "source") ? "block" : "none",
                overflow: "auto", height: "100%",
            }}
            >
                {
                    sourceLayoutDisplayed &&
                    <CodeEditor
                        source={objectJSON}
                        readOnly={!(editable === true)}
                        schema={objectJSONSchema}
                        showLineNumbers={true}
                        onChanged={newJSON => {
                            if (objectJSON !== newJSON) {
                                setObjectJSON(newJSON);                            
                                onSourceChanged(object);
                            }                            
                        }}
                    />
                }
            </div>                
            {
                (layout === "flattened") &&
                <div 
                    style={{
                        display: (layout === "flattened") ? "block" : "none",
                        overflow: "auto", height: "100%"
                    }}
                >
                    <ObjectFlattened />
                </div>
            }
            {
                (layout === "tree") &&
                <div 
                    style={{
                        display: (layout === "tree") ? "block" : "none",
                        overflow: "auto", height: "100%",
                        padding: theme.spacing(2)
                    }}
                >
                    <ObjectTree object={object} />
                </div>
            }
        </>
    );
};   


const initialState = ({ object }) => ({

    showLayoutParameters:       false,
    showContextMenu:            false,
    contextMenuClientX:         0,
    contextMenuClientY:         0,
    showPropertyEditor:         false,
    showPropertyMetadata:       false,
    showObjectMetadata:         false,
    refreshing:                 false,
    layoutParametersJSON:       {},
    editLayoutParametersJSON:   {},
    objectOptions:              initialObjectOptions,    
    disableContextualMenu:      false,
    layout:                     "responsive",
    objectJSON:                 "",
    objectJSONSchema:           object?.getJSONSchema(),

});

const reducer = (state, action) => {

    switch (action.type) {

    case "INIT_LAYOUT_PARAMS":
        return {
            ...state,
            layoutParameters: action.layoutParameters,
            layoutParametersJSON: action.layoutParameters.toJSON(),
        };

    case "LAYOUT_PARAMS_ADVANCED":
        return {
            ...state,
            showLayoutParameters: true,
        };

    case "LAYOUT_PARAMS_SAVE":
        return {
            ...state,
            layoutParametersJSON: state.layoutParameters.toJSON(),
            showLayoutParameters: false,
        };

    case "LAYOUT_PARAMS_DISMISS":
        return {
            ...state,
            showLayoutParameters: false,
        };

    case "INIT_EDIT_LAYOUT_PARAMETERS":
        return {
            ...state,
            editLayoutParameters: action.editLayoutParameters,
            editLayoutParametersJSON: action.editLayoutParameters.toJSON(),
        };

    case "CONTEXT_MENU":
        return {
            ...state,
            showContextMenu: true,
            contextMenuClientX: action.event.clientX,
            contextMenuClientY: action.event.clientY,
            contextMenuProperty: action.property,
        };    

    case "CLOSE_CONTEXT_MENU":
        return {
            ...state,
            showContextMenu: false,
        };

    case "DISABLE_CONTEXT_MENU":
        return {
            ...state,
            disableContextualMenu: true,
        };

    case "COPY_PROPERTY":
        return {
            ...state,
            showContextMenu: false,            
        };

    case "EDIT_PROPERTY":
        return {
            ...state,
            showContextMenu: false,
            showPropertyEditor: true
        };

    case "CLOSE_EDIT_PROPERTY":
        return {
            ...state,
            showPropertyEditor: false,
        };

    case "SHOW_PROPERTY_META":
        return {
            ...state,
            showContextMenu: false,
            showPropertyMetadata: true,
        };

    case "HIDE_PROPERTY_META":
        return {
            ...state,
            showPropertyMetadata: false,
        };

    case "EDIT_OBJECT":
        return {
            ...state,
            showContextMenu: false,
        };

    case "SHOW_OBJECT_META":
        return {
            ...state,
            showContextMenu: false,
            showObjectMetadata: true,
        };

    case "HIDE_OBJECT_META":
        return {
            ...state,
            showContextMenu: false,
            showObjectMetadata: false,
        };

    case "REFRESHING":
        return {
            ...state,
            refreshing: true,
        };
        
    case "REFRESHED":
        return {
            ...state,
            refreshing: false,
        };

    case "OBJECT_OPTIONS":
        return {
            ...state,
            objectOptions: action.options,
        };

    case "OBJECT_STRINGIFY":
        return {
            ...state,
            objectJSON: action.object.stringify(),            
        };

    case "OBJECT_JSON":
        return {
            ...state,
            objectJSON: action.json,
        };

    case "OBJECT_JSON_SCHEMA":
        return {
            ...state,
            objectJSONSchema: action.object.getJSONSchema(),
        };

    case "LAYOUT_CHANGE":        
        return {
            ...state,
            layout: action.layout,
            objectJSON: (action.layout === "source") ? action.object?.stringify() : state.objectJSON,
            sourceLayoutDisplayed: (state.sourceLayoutDisplayed || action.layout === "source"),
        };

    default:
        return state;
    }
};

/*
 * ObjectEditorLayout
 *
 * The ObjectEditorLayout wraps an adaptive ObjectResponsive component with additional buttons and menus
 * in order to switch layouts and/or specify layout properties.  Additionally, it provides an
 * "Edit Object" checkbox that toggles the Object in and out of an "editable" state.
 *
 * In an editable state, the object properties can be added and/or removed.  Components inside
 * the layout may also respond to the "editable" state by providing more useful ways to display
 * properties, while in a non-editable state, the properties may simply be read-only labels.
 */
const ObjectEditorLayout = (props, ref) => {

    const [state, dispatch] = useReducer(reducer, props, initialState);

    const {
        showLayoutParameters,
        layoutParameters,
        editLayoutParameters,
        showContextMenu,
        contextMenuProperty,
        contextMenuClientX,
        contextMenuClientY,
        showPropertyEditor,
        showPropertyMetadata,
        showObjectMetadata,
        refreshing,
        disableContextualMenu,
        objectOptions,
        layout,
        objectJSON,
        objectJSONSchema,
        sourceLayoutDisplayed,
    } = state;               
        
    useValues(layoutParameters);
    useValues(editLayoutParameters);    

    const {
        object, 
        editMode, 
        onEditMode, 
        addMode,
        onNextObject, 
        onPreviousObject,
        sourceChanged,
        onSourceChanged,
    } = props;

    const isMounted = useIsMounted();
    const model = useModel();
    const {onCopy, notification, isMobile} = useApplication();
    const theme = useTheme();
    const {pathname, hash} = useLocation();
    const history = useHistory();

    const changesDiscarded = useEventId({ object, eventId: "onDiscardChanges" });

    useEffect(() => {
        if (hash)
            dispatch({ type: "LAYOUT_CHANGE", layout: hash.substring(1), object });
    }, [hash, object]);

    /*
     * onFinalizeObject()
     *
     * This is a public callback, so that our parent component can notify us that the object we are displaying
     * is about to be saved.  In case we are in JSON source mode, we can decide to re-parse the JSON back into
     * the object, so that it's up to date.  Without doing this, we would have to re-parse the JSON way too often
     * and the component would be sluggish.
     */
    useImperativeHandle(ref, () => ({
        onFinalizeObject: () => {
            if (layout === "source" && sourceChanged) {                
                object.fromJSON(objectJSON);
            }
        }
    }));
  
    /**
     * When the object has its changes discarded, we register to get notified, so we can reconstruct the JSON and 
     * JSON schema, if necessary.
     */
    useEffect(() => {
        if (changesDiscarded && object && layout === "source") {       
            dispatch({ type: "OBJECT_STRINGIFY", object });            
        }
    }, [changesDiscarded, object, layout]);    
   

    /*
     * When this component is first mounted, set the initial default layout
     * parameters, that can be changed later on when the user chooses to
     * do so.
     */
    useEffect(() => {

        const setInitialLayoutParameters = async () => {

            const layoutParameters = model.newObject({
                adaptorId: "afw",
                objectTypeId: "_AdaptiveLayout_ObjectResponsive",
                object: defaultLayoutParameters(false, addMode, isMobile)
            });
            await layoutParameters.initialize();

            if (isMounted()) {
                dispatch({ type: "INIT_LAYOUT_PARAMS", layoutParameters });                
            }
        };

        const setInitialEditLayoutParameters = async () => {

            const editLayoutParameters = model.newObject({
                adaptorId: "afw",
                objectTypeId: "_AdaptiveLayout_ObjectResponsive",
                object: defaultLayoutParameters(true, addMode, isMobile)
            });
            await editLayoutParameters.initialize();

            if (isMounted()) {
                dispatch({ type: "INIT_EDIT_LAYOUT_PARAMETERS", editLayoutParameters });                
            }
        };

        setInitialLayoutParameters();
        setInitialEditLayoutParameters();
        
    }, [model, addMode, isMounted, isMobile]);

    useEffect(() => {
        if (object)
            dispatch({ type: "OBJECT_JSON_SCHEMA", object });
    }, [object]);

    /*
     * onEditObject()
     *
     * When the "Edit Object" checkbox is checked, this triggers an object refresh() so that
     * we get the latest copy of the object in reconcilable mode.
     */
    const onEditObject = async (editMode) => {
        
        if (editMode) {
            try {
                dispatch({ type: "REFRESHING" });
                await props.onRefresh(objectOptions);                            
            } catch (error) {
                if (isMounted())
                    notification({ type: "error", error });
            }
        }

        if (isMounted()) {
            onEditMode(editMode);
            dispatch({ type: "REFRESHED" });
        }
    };

    /*
     * onRefreshObject()
     *
     * When the user selects new Object Options, we refresh the object with the new options.
     */
    const onRefreshObject = async (objectOptions) => {
        dispatch({ type: "REFRESHING" });

        try {
            await props.onRefresh(objectOptions);            
        } catch (error) {
            notification({message: error, type: "error"});
        }

        if (isMounted())
            dispatch({ type: "REFRESHED" });
    };
  

    /*
     * onRenderHeader()
     *
     * The ObjectResponsive component allows for a callback when rendering headers of objects and embedded
     * objects.  We use this callback to supplement the header contents with our own custom contextual
     * menu, which includes layout options.  The top-level object also contains the "Edit Object"
     * checkbox, used to put the object in and out of edit mode.
     */
    const onRenderHeader = (headerProps, renderedHeader) => {

        /* let the default header render on embedded objects */
        if (headerProps && headerProps.nestingDepth !== 0) {
            return renderedHeader;
        }

        /* The top-level object renders a special header with context menus and buttons/options */
        return (
            <>
                <div data-testid="ObjectEditorLayout-Toolbar" style={{ display: "flex", alignItems: "center", width: "100%" }}>
                    <div style={{  marginRight: theme.spacing(1) }}>                    
                        <LayoutViewContextMenu
                            object={object}
                            layout={layout}
                            layoutParameters={editMode ? editLayoutParameters : layoutParameters}
                            onLayoutParamsAdvanced={() => dispatch({ type: "LAYOUT_PARAMS_ADVANCED" })}
                            onChanged={() => {                               
                                return null;
                            }}
                            onEditObject={onEditObject}
                            onSave={props.onSave}
                            onReplace={props.onReplace}
                            onReconcile={props.onReconcile}
                            onDiscard={props.onDiscard}
                            onReview={props.onReview}
                            onRefresh={() => onRefreshObject(objectOptions)}
                            objectOptions={objectOptions}
                            setObjectOptions={(options) => {
                                dispatch({ type: "OBJECT_OPTIONS", options });
                                onRefreshObject(options);
                            }}
                        />
                    </div>
                    <div style={{ flex: 1 }}>
                        <div style={{ display: "inline-block", verticalAlign: "middle" }}>
                            <ToggleButtons
                                exclusive={true}
                                value={layout}
                                onChanged={(newLayout) => {
                                    if (!newLayout)
                                        return;
                                        
                                    let json = objectJSON;
                                    if (newLayout === "source") 
                                    {                                
                                        json = object.stringify();                                        
                                    }

                                    else if ((layout === "source" && newLayout !== "source") && sourceChanged) 
                                    {
                                        try {                            
                                            object.fromJSON(json);
                                        } catch (e) {
                                            // \fixme no need for global notification here.  alert user, but don't record it in notifications?
                                            notification({ message: "Unable to parse JSON.", type: "error" });
                                            return;
                                        }
                                    }
  
                                    history.push(pathname + "#" + newLayout);
                                    dispatch({ type: "LAYOUT_CHANGE", layout: newLayout });
                                    dispatch({ type: "OBJECT_JSON", json });
                                }}
                                options={[
                                    {
                                        value: "responsive",
                                        icon: "view_quilt",
                                        tooltip: "View object in Responsive View",
                                        "aria-label": "Responsive View"
                                    },
                                    {
                                        value: "flattened",
                                        icon: "format_list_bulleted",
                                        tooltip: "View object in Flattened View",
                                        "aria-label": "Flattened View"
                                    },
                                    {
                                        value: "tree",
                                        icon: "share",
                                        tooltip: "View object in Tree View",
                                        "aria-label": "Tree View"
                                    },
                                    {
                                        value: "source",
                                        icon: "code",
                                        tooltip: "View object source",
                                        "aria-label": "Source View"
                                    }
                                ]}
                            />
                        </div>
                        { renderedHeader &&
                            <div style={{ display: "inline-block", verticalAlign: "middle" }}>
                                {renderedHeader}
                            </div>
                        }
                    </div>
                    <div>
                        <ToggleEdit
                            object={object}
                            refreshing={refreshing}
                            onEditObject={onEditObject}
                        />
                    </div>                    
                </div>
                {
                    object.hasErrors() && (
                        <ObjectError object={object} />
                    )
                }
            </>
        );        
    };
   

    /* make sure we wait until we have an object and layout parameters */
    if (!object || !layoutParameters || !editLayoutParameters)
        return null;

    const layoutJSON = (editMode ? editLayoutParameters.toJSON() : layoutParameters.toJSON());       
    
    return (
        <OperationalContext.Provider value={{ editable: (editMode || addMode), add: addMode }}>
            <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters: { ...layoutJSON } }}>
                <div style={{ height: "100%", display: "flex", flexDirection: "column" }}>
                    {
                        // Responsive layouts don't need a header, because it already has one
                        (layout !== "responsive" && onRenderHeader())
                    }
                    <div style={{ flexGrow: 1, overflow: "auto" }}>
                        <RenderedLayout 
                            object={object}
                            layout={layout} 
                            onRenderHeader={onRenderHeader}              
                            objectJSON={objectJSON} 
                            objectJSONSchema={objectJSONSchema}
                            setObjectJSON={(json) => dispatch({ type: "OBJECT_JSON", json })}
                            onSourceChanged={onSourceChanged}
                            disableContextualMenu={disableContextualMenu}
                            setContextMenu={(property, event) => dispatch({ type: "CONTEXT_MENU", property, event })}   
                            sourceLayoutDisplayed={sourceLayoutDisplayed}                         
                        />
                    </div>
                    {
                        (!addMode) &&                        
                            <div style={{
                                display: "flex",
                                justifyContent: "space-between",
                                paddingTop: theme.spacing(0.5), 
                                paddingBottom: theme.spacing(0.5),
                                borderTop: "solid thin #999"
                            }}>      
                                <Button 
                                    type="icon"
                                    icon="chevron_left"
                                    onClick={onPreviousObject}   
                                    disabled={!onPreviousObject}     
                                    label="Previous Object"                        
                                    tooltip="Previous Object"                        
                                />                                                  
                                <div style={{ display: "flex", justifyContent: "flex-end", alignItems: "center" }}>                                    
                                    <OperationButtons 
                                        object={object}
                                        sourceChanged={sourceChanged}
                                        onSave={props.onSave}
                                        onReplace={props.onReplace}
                                        onReconcile={props.onReconcile}
                                        onReview={props.onReview}
                                        onDiscard={props.onDiscard}
                                    />                                     
                                    <Button 
                                        type="icon"
                                        icon="chevron_right"
                                        onClick={onNextObject}
                                        disabled={!onNextObject}
                                        tooltip="Next Object"
                                        label="Next Object"
                                    />    
                                </div>                                                   
                            </div>
                    }
                </div>
            </AdaptiveLayoutContext.Provider>
            <ObjectEditorLayoutParameters
                open={Boolean(showLayoutParameters)}
                layoutParameters={editMode ? editLayoutParameters : layoutParameters}
                onSave={() => dispatch({ type: "LAYOUT_PARAMS_SAVE", layoutParameters })}
                onDismiss={() => dispatch({ type: "LAYOUT_PARAMS_DISMISS" })}
            />
            <Menu
                open={(disableContextualMenu !== true) && showContextMenu}
                anchorPosition={{ left: contextMenuClientX, top: contextMenuClientY }}
                onClose={() => dispatch({ type: "CLOSE_CONTEXT_MENU" })}
                items={[
                    {
                        key: "Copy",
                        label: contextMenuProperty ? ("Copy \"" + (
                            contextMenuProperty.getLabel() ? contextMenuProperty.getLabel() : contextMenuProperty.getName()) + "\""
                        ) : "Copy Property",
                        onClick: () => {
                            onCopy(contextMenuProperty);          
                            dispatch({ type: "COPY_PROPERTY" });
                        }
                    },
                    {
                        key: "EditProperty",
                        label: contextMenuProperty ? ("Edit \"" + (
                            contextMenuProperty.getLabel() ? contextMenuProperty.getLabel() : contextMenuProperty.getName()) + "\""
                        ) : "Edit Property",
                        disabled: contextMenuProperty && (object.getAllowChange() === false || contextMenuProperty.getAllowWrite() === false),
                        onClick: () => {
                            dispatch({ type: "EDIT_PROPERTY" });
                        }
                    },
                    {
                        key: "ViewMetadata",
                        label: "View Property Metadata",
                        onClick: () => {                            
                            dispatch({ type: "SHOW_PROPERTY_META" });
                        }
                    },
                    {
                        key: "divider",
                        type: "divider"
                    },
                    {
                        key: "EditObject",
                        label: "Edit Object",
                        disabled: contextMenuProperty && (object.getAllowChange() === false),
                        onClick: () => {
                            onEditObject(true);
                            dispatch({ type: "EDIT_OBJECT" });
                        }
                    },
                    {
                        key: "ViewObjectMetadata",
                        label: "View Object Metadata",
                        onClick: () => {
                            dispatch({ type: "SHOW_OBJECT_META" });                            
                        }
                    },
                    {
                        key: "divider2",
                        type: "divider",                        
                    },
                    {
                        key: "disableContextualMenu",
                        label: "Disable this Menu",
                        onClick: () => dispatch({ type: "DISABLE_CONTEXT_MENU" })
                    }
                ]}
            />
            <PropertyEditor
                open={showPropertyEditor}
                property={contextMenuProperty}
                onSave={() => dispatch({ type: "CLOSE_EDIT_PROPERTY" })}
                onDismiss={() => dispatch({ type: "CLOSE_EDIT_PROPERTY" })}
            />
            <PropertyMetadata
                open={showPropertyMetadata}
                onDismiss={() => dispatch({ type: "HIDE_PROPERTY_META" })}
                property={contextMenuProperty}
            />
            <ObjectMetadata 
                open={showObjectMetadata}
                onDismiss={() => dispatch({ type: "HIDE_OBJECT_META" })}
                object={object}
            />
        </OperationalContext.Provider>
    );
};

export default forwardRef(ObjectEditorLayout);
