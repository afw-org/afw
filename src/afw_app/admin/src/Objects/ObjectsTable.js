// See the 'COPYING' file in the project root for licensing information.
import {useEffect, useCallback, useRef, useReducer} from "react";
import {Switch, Route, useHistory, useLocation, useRouteMatch} from "react-router";

import {    
    Link,
    Table,    
    Spinner,        
    Button,
    Dialog,
    Typography,
    ValueLayout,
    useGetObject,
    useModel,
    OperationalContext,
    OperationalMode
} from "@afw/react";
import {AfwClient, isArray, AfwArray, AfwObject} from "@afw/client";
import {afwRetrieveObjectsToResponse} from "@afw/core";

import ObjectNew from "./ObjectNew";
import ObjectUploadModal from "./ObjectsUpload";
import ObjectsTableToolbar from "./ObjectsTableToolbar";
import ObjectsBatchModifyModal from "./ObjectsBatchEdit";
import ObjectEditor from "./ObjectEditor";
import {ErrorDialog} from "../common/ErrorForm";

import {useApplication, useTheme} from "../hooks";

const client = new AfwClient({
    url: "/afw",
    accept: "application/x-afw"
});


/**
 * onRetrieveObjects()
 *
 * When this component is loaded, or when the user clicks the "Refresh" button, this routine actually 
 * performs the work of retrieve objects and updating the state and notifications, as appropriate.
 */
const onRetrieveObjects = async ({ 
    model, 
    adaptorId, 
    objectTypeId, 
    queryCriteria, 
    onStatus, 
    onFinish, 
    onError, 
    onReturn, 
    onResponse 
}) => {    
    
    try {
        onStatus("Retrieving objects...");

        /* the query criteria comes from the route url, which will include the ? operator */
        if (queryCriteria)
            queryCriteria = queryCriteria.substring(1);
                
        const response = afwRetrieveObjectsToResponse(
            client, objectTypeId, adaptorId, 
            { urlEncodedRQLString: queryCriteria }, 
            { objectId: true }
        );
        onResponse(response);

        const streams = await response.streams();        

        let objects = [];
        let status;

        /* if we don't get an array back, then we may only have a result */
        if (isArray(streams.response)) {
            objects = streams.response.filter(response => response.intermediate).map(response =>
                new AfwObject({ model, object: response.result, objectTypeId, adaptorId })                    
            );        
            status = streams.response[ streams.response.length - 1];
        } else if (streams.response)
            status = streams.response;

        if (status && status.status === "error")
            throw status.error;

        onStatus("Initializing objects...");
        for (const o of objects)
            await o.initialize();                
        
        /* sort them by objectId */
        objects = objects.sort((A, B) => {
            let a = A.getObjectId()?.toLowerCase() || "";
            let b = B.getObjectId()?.toLowerCase() || "";

            return (a.localeCompare(b));
        });
        
        onReturn(objects);
        onStatus();
    } catch (error) {      
        onStatus();
        if (error && !(error.id === "not_found"))
            onError(error);
    }

    onFinish();    
};


const initialState = {
    objects:                    [],
    showConfirmDeleteDialog:    false,
    showNewObject:              false,
    selectedObjects:            [],
    showBatchEdit:              false,
    showUpload:                 false,
    retrieved:                  false,
    columns:                    [],
};

const reducer = (state, action) => {

    switch (action.type) {

    case "OBJECTS":
        return {
            ...state,
            objects: action.objects,
            selectedObject: [],
            spinnerText: undefined,
        };

    case "DELETE":
        return {
            ...state,
            showConfirmDeleteDialog: true,
        };

    case "CONFIRM_DELETE":
        return {
            ...state,
            showConfirmDeleteDialog: false,
            spinnerText: undefined,
            selectedObjects: [],
            objects: action.remainingObjects,
        };

    case "CANCEL_DELETE":
        return {
            ...state,
            showConfirmDeleteDialog: false,
        };

    case "ADD_OBJECT":
        return {
            ...state,
            showNewObject: true,
        };

    case "ADDING_OBJECT":
        return {            
            ...state,
            showNewObject: false,
        };

    case "CANCEL_ADD":
        return {
            ...state,
            showNewObject: false,
        };

    case "SELECT_OBJECTS":
        return {
            ...state,
            selectedObjects: (action.selectedObjects.length > 0) ? action.selectedObjects : [],
        };

    case "BATCH_EDIT":
        return {
            ...state,
            showBatchEdit: true,
        };

    case "CANCEL_BATCH_EDIT":
        return {
            ...state,
            showBatchEdit: false,
        };

    case "UPLOAD":
        return {
            ...state,
            showUpload: true,
        };

    case "CANCEL_UPLOAD":
        return {
            ...state,
            showUpload: false,
        };

    case "COLUMNS":
        return {
            ...state,
            columns: action.columns,
        };

    case "STATUS":
        return {
            ...state,
            spinnerText: action.spinnerText,
        };

    case "RETRIEVING":
        return {
            ...state,
            adaptorId: action.adaptorId,
            objectTypeId: action.objectTypeId,
            queryCriteria: action.queryCriteria,
        };

    case "RETRIEVED":
        return {
            ...state,
            retrieved: true,
        };

    case "ERROR":
        return {
            ...state,
            spinnerText: undefined,
            error: action.error,
            selectedObjects: [],
            showConfirmDeleteDialog: false,
        };

    case "CLEAR_ERROR":
        return {
            ...state,
            error: undefined,
        };


    default:
        return state;
    }
};

/*
 * ObjectsTable
 *
 * This layout provides a way to navigate a list of objects by presenting them inside a Table component
 * with buttons to Refresh, Delete, Edit or Add new ones.  There are two columns that are presented for
 * each object:  The Object Id and its description.
 */
const ObjectsTable = ({ onSelectObject }) => {

    const [state, dispatch] = useReducer(reducer, initialState);
    const {
        objects,
        showConfirmDeleteDialog,
        showNewObject,
        selectedObjects,
        showBatchEdit,
        showUpload,
        spinnerText,
        adaptorId,
        objectTypeId,
        queryCriteria,
        columns,
        retrieved,
        error,
    } = state;
            
    const theme = useTheme();
    const match = useRouteMatch();
    const history = useHistory();
    const location = useLocation();
    const model = useModel();
    const {notification} = useApplication();    
    const {object: objectTypeObject, isLoading: objectTypeLoading} = useGetObject({ 
        adaptorId, objectTypeId: "_AdaptiveObjectType_", objectId: objectTypeId 
    });

    /* abort controller for retrieve */
    const controller = useRef(null);

    const hasAborted = () => controller.current && controller.current.signal.aborted;

    /* when this component unmounts, make sure to abort any retrieve request that may be active */
    useEffect(() => {
        return () => { if (controller.current) controller.current.abort(); };
    }, []);

    /* when the component initially mounts up, retrieve the needed objects to display. */
    useEffect(() => {
        const retrieve = async (adaptorId, objectTypeId, queryCriteria) => {
            const onReturn = (objects) => {
                if (!hasAborted())
                    dispatch({ type: "OBJECTS", objects });
            };

            const onStatus = (spinnerText) => {
                if (!hasAborted()) 
                    dispatch({ type: "STATUS", spinnerText });
            };

            const onError = (error) => {
                notification({ message: error, type: "error" });                
            };

            const onResponse = (response) => { controller.current = response.controller; };

            const onFinish = () => {
                if (!hasAborted()) 
                    dispatch({ type: "RETRIEVED" });
            };

            await onRetrieveObjects({
                model, 
                adaptorId, 
                objectTypeId, 
                queryCriteria, 
                onStatus,
                onFinish,
                onError, 
                onReturn, 
                onResponse,
            });
        };

        /* only retrieve objects, if we're on the URI that requests them.  otherwise, we're looking for a specific object */
        if (model && match.isExact && !retrieved) {     
            let adaptorId = match.params.adaptorId;
            let objectTypeId = match.params.objectTypeId;            
            let queryCriteria = location.search;

            adaptorId = decodeURIComponent(adaptorId);
            objectTypeId = decodeURIComponent(objectTypeId);

            dispatch({ type: "RETRIEVING", adaptorId, objectTypeId, queryCriteria });            

            retrieve(adaptorId, objectTypeId, queryCriteria);            
        }
    }, [model, location.search, match.isExact, match.params.adaptorId, match.params.objectTypeId, notification, retrieved]);

    let sortableProperties = [];
    if (objectTypeObject) {
        sortableProperties = objectTypeObject.getProperties().filter(p => (p.getDataType() !== "object" && p.getDataType() !== "array"));
    }


    /**
     * getDescription()
     *
     * This routine tries to make sense of some sort of object description to be displayed in the table of objects for each row.
     * It firsts looks at the instance object's description and uses that, if available.  Otherwise, it looks at the meta 
     * description, provided by the object type definition.
     */
    const getDescription = (object) => {
        let description;
           
        /* \fixme decide on something here */
        description = object.getPropertyValue("description");

        return description;
    };

    /**
     * onRenderProperty()
     * 
     * When the table column is rendered for a particular row, this routine is called to determine how the value should be 
     * displayed.
     * 
     * @param {*} object 
     * @param {*} propertyName 
     */
    const onRenderProperty = (object, propertyName) => {
        const property = object.getProperty(propertyName);
        return (
            <ValueLayout value={property} />
        );
    };

    /**
     * onRenderDescription()
     *
     * This routine renders the Description field for objects in the table.
     */
    const onRenderDescription = (object) => {                
        const description = getDescription(object);

        if (description && description instanceof AfwArray) {                                   
            return (
                <ul style={{ listStyleType: "none", paddingInlineStart: "0px", marginBlockEnd: "0px", marginBlockStart: "0px" }}>
                    {                        
                        description.map((descr, index) => {
                            return (
                                <li key={index}>
                                    <Typography text={descr.getValue()} />
                                </li>
                            );
                        })
                    }
                </ul>
            );
        }

        return <Typography text={description ? description : ""} />;
    };

    /**
     * getValue() 
     *
     * This routine is used by the Table component for sorting purposes.  Rather than use the actual value in the row cell, which 
     * may be rendered a variety of ways, the getValue() returns a canonical representation, useful for sorting, when the user 
     * clicks on a column header to sort.
     */
    const getValue = (propertyName, object) => {
        let value = object.getPropertyValue(propertyName);

        /* \fixme kludge */
        if (value && value.constructor === Array)
            value = value[0].getValue();

        if (value)
            return value.toLowerCase();

        return value;
    };

    /**
     * onSelectionChanged()
     *
     * When a user selects one or more objects in the table, this routine is invoked. The selection
     * is saved away in the state, so operations can later use them.
     */
    const onSelectionChanged = useCallback((selectedObjects) => {      
        dispatch({ type: "SELECT_OBJECTS", selectedObjects });
    }, []);

    /**
     * onObjectInvoked()
     *
     * When an item is double-clicked in the table, we can invoke the object editor.
     */
    const onObjectInvoked = useCallback((item) => {
        const objectId = item.getObjectId();        

        history.push("/Objects/" + adaptorId + "/" + objectTypeId + "/" + objectId);
    }, [adaptorId, history, objectTypeId]);

    /**
     * onDelete()
     *
     * This is the first step of the delete object process.  Pop up a dialog to confirm action.
     */
    const onDelete = () => {
        dispatch({ type: "DELETE" });
    };

    /**
     * onConfirmDelete()
     *
     * Once the use clicks "Delete" again to confirm, then actually remove all objects that are 
     * selected in the table.
     */
    const onConfirmDelete = async () => {        

        try {            
            for (let i = 0; i < selectedObjects.length; i++) {
                dispatch({ type: "STATUS", spinnerText: "Deleting objects " + i + " of " + selectedObjects.length + "..." });
                
                const selectedObject = selectedObjects[i];
                const response = selectedObject.delete();
                await response.result();
            }            
            
            notification({ message: selectedObjects.length + " Object(s) deleted successfully.", type: "success" });

            /* now remove them from the objects array */
            let remainingObjects = objects.filter( obj => {
                for (const selectedObject of selectedObjects) {
                    if (selectedObject.getObjectId() === obj.getObjectId())
                        return false;
                }
                return true;
            });
            
            dispatch({ type: "CONFIRM_DELETE", remainingObjects });            
        } catch (error) {
            dispatch({ type: "ERROR", error });
            notification({ message: error, type: "error", snackbar: false });
        }
    };

    /**
     * onRefresh()
     *
     * This routine is invoked when the user clicks the "Refresh" button to refresh the objects listed
     * in the table.
     */
    const onRefresh = async () => {
        const onReturn = (objects) => {
            if (!hasAborted())
                dispatch({ type: "OBJECTS", objects });
        };

        const onStatus = (spinnerText) => {
            if (!hasAborted()) 
                dispatch({ type: "STATUS", spinnerText });
        };

        const onError = (error) => notification({ message: error, type: "error" });

        const onResponse = (response) => { controller.current = response.controller; };

        const onFinish = () => {
            if (!hasAborted()) 
                dispatch({ type: "RETRIEVED" });
        };

        await onRetrieveObjects({
            model, 
            adaptorId, 
            objectTypeId, 
            queryCriteria, 
            onStatus,
            onFinish,
            onError, 
            onReturn, 
            onResponse,
        });
    };    

    /**
     * onAdd()
     *
     * This routine is invoked when the user has created a new object and its appropriate properties 
     * and clicked the "Create" button to save the new object.
     */
    const onAdd = async (object) => {
        /* hide the modal */
        dispatch({ type: "ADDING_OBJECT" });

        try {
            const response = await object.add().result();

            notification({ message: "Object " + response["objectId"] + " created.", type: "success", response });

            /* new object was created, so refresh the table */
            onRefresh();
        } catch (error) {
            dispatch({ type: "ERROR", error });
            notification({ message: error, type: "error", snackbar: false });
        }
    };
   
    /**
     * onDownload()
     * 
     * When the "Download" icon is clicked, we stringify the selected objects to make them available for
     * a single downloadable file.
     */
    const onDownload = () => {
        let content = JSON.stringify(selectedObjects, null, 4);

        /* Now, create an anchor to simulate a click for download */
        let element = document.createElement("a");
        element.setAttribute("href", "data:application/json;charset=utf-8," + encodeURI(content));
        element.setAttribute("download", "objects.json");

        /* make it invisible */
        element.style.display = "none";
        document.body.appendChild(element);

        /* Now simulate the click to initiate download */
        element.click();

        /* Finally, remove it */
        document.body.removeChild(element);
    };

    /**
     * onSortBy()
     *
     * When the user chooses to sort the objects in the table.
     */
    const onSortBy = (property) => {
        const name = property.getName();

        const sorted = objects.sort((A, B) => {
            let a = A.getPropertyValue(name);
            let b = B.getPropertyValue(name);

            if (a && b)
                return (a.toLowerCase().localeCompare(b.toLowerCase()));
            else return 0;
        });

        dispatch({ type: "OBJECTS", objects: sorted });        
    };

    /**
     * onNextObject()
     * 
     * When the user wishes to navigate to the next available object.
     */
    const onNextObject = () => {
        if (objects) {
            let [, , , ,objectId] = location.pathname.split("/");

            /* select the next object in the array and push it onto history */
            for (let i = 0; i < objects.length; i++) {                
                if (objects[i].getObjectId() === objectId) {                    
                    let nextObject = objects[(i + 1) % objects.length];
                    history.push("/Objects/" + adaptorId + "/" + objectTypeId + "/" + nextObject.getObjectId());
                }
            }
        }
    };

    /**
     * onPreviousObject()
     * 
     * When the user wishes to navigate to the previously available object.
     */
    const onPreviousObject = () => {
        if (objects) {
            let [,,,,objectId] = location.pathname.split("/");

            /* select the previous object in the array and push it onto history */
            for (let i = 0; i < objects.length; i++) {                
                if (objects[i].getObjectId() === objectId) {
                    let prev = (i - 1) < 0 ? objects.length - 1 : (i - 1);
                    let nextObject = objects[prev];
                    history.push("/Objects/" + adaptorId + "/" + objectTypeId + "/" + nextObject.getObjectId());
                }
            }

        }
    };

    /**
     * viewPropertyMenu()
     * 
     * Builds the "View" menu options for adding additional columns to the table.
     */
    const viewPropertyMenu = () => {
        const propertyTypes =  objectTypeObject ? objectTypeObject.getPropertyValue("propertyTypes") : undefined;
        const properties = propertyTypes ? propertyTypes.getProperties().sort((A, B) => {
            return (A.getName().toLowerCase().localeCompare(B.getName().toLowerCase()));
        }) : [];

        return (
            {
                key: "view",
                label: "View",
                disabled: !objectTypeObject,
                subMenu: {
                    items: properties.map(p => {
                        const name = p.getName();
                        const value = p.getValue();
                        const label = (value && value.getPropertyValue("label")) ? value.getPropertyValue("label") : name;

                        return ({
                            key: name,
                            label,
                            canCheck: true,
                            checked: columns.some(c => c.key === name),
                            onClick: () => {
                                let cols;

                                if (columns.some(c => c.key === name)) {
                                    /* remove this column from the view */
                                    cols = columns.filter(c => c.key !== name);
                                } else {
                                    /* add this column to the view */
                                    cols = [...columns, {
                                        key: name,
                                        name: label,
                                        minWidth: 100,
                                        maxWidth: 200,
                                        isResizable: true,
                                        isMultiline: (p.getValue().getPropertyValue("dataType") === "array"),
                                        getValue: (object) => String(object.getPropertyValue(name)),
                                        onRender: (object) => onRenderProperty(object, name)
                                    }];
                                }
                                
                                dispatch({ type: "COLUMNS", columns: cols });
                                // Don't set optionsMenuTarget(), because they may want to set a bunch at once
                            }
                        });
                    })
                }
            }
        );
    };

    if (spinnerText)
        return <Spinner size="large" label={spinnerText} fullScreen={true} />;        

    if (objectTypeLoading) 
        return <Spinner size="large" label="Loading Object Type" fullScreen={true} />;
        
    return (
        <OperationalContext.Provider value={OperationalMode.NotEditable}>
            <Switch>
                <Route path="/Objects/:adaptorId/:objectTypeId/:objectId" render={routerProps => {
                    let {adaptorId, objectTypeId, objectId} = routerProps.match.params;
                    
                    adaptorId = decodeURIComponent(adaptorId);
                    objectTypeId = decodeURIComponent(objectTypeId);
                    objectId = decodeURIComponent(objectId);

                    return (
                        <ObjectEditor 
                            adaptorId={adaptorId}
                            objectTypeId={objectTypeId}
                            objectId={objectId}
                            onNextObject={objects?.length ? onNextObject : undefined}
                            onPreviousObject={objects?.length ? onPreviousObject : undefined}
                            onSelectObject={onSelectObject}
                        />
                    );
                }} />
                <Route path="/Objects/:adaptorId/:objectTypeId" render={() => {
                    return (
                        <div style={{ display: "flex", flexDirection: "column", height: "100%", marginRight: theme.spacing(1) }}>
                            <ObjectsTableToolbar 
                                sortableProperties={sortableProperties}  
                                setShowNewObject={() => dispatch({ type: "ADD_OBJECT" })}
                                onBatchEdit={() => dispatch({ type: "BATCH_EDIT" })}
                                onDelete={onDelete}
                                onUpload={() => dispatch({ type: "UPLOAD" })}
                                onDownload={onDownload}
                                onRefresh={onRefresh}
                                onSortBy={onSortBy}
                                objectTypeObject={objectTypeObject}
                                selectedObjects={selectedObjects}
                                viewPropertyMenu={viewPropertyMenu}                                
                            />
                            {
                                (objects?.length > 0) ?
                                    <div style={{ flex: 1, width: "100%", overflow: "auto" }}>
                                        <Table 
                                            rows={objects}
                                            columns={[
                                                {
                                                    key: "objectId",
                                                    name: "Object Id",
                                                    minWidth: 100,
                                                    maxWidth: 250,
                                                    isResizable: true,
                                                    getValue: (item) => item.getObjectId() ? item.getObjectId().toLowerCase() : "",
                                                    onRender: (item) => {
                                                        return (
                                                            <Link 
                                                                uriComponents={[ "Objects", item.getAdaptorId(), item.getObjectTypeId(), item.getObjectId() ]}                            
                                                                text={item.getObjectId()}
                                                            />
                                                        );
                                                    }
                                                },
                                                ...((columns.length > 0) ? columns : [({
                                                    key: "description",
                                                    name: "Description",
                                                    isMultiline: true,
                                                    minWidth: 100,
                                                    maxWidth: 400,
                                                    isResizable: true, 
                                                    getValue: () => getValue("description"),                                    
                                                    onRender: onRenderDescription,                   
                                                })])
                                            ]}
                                            onSelectionChanged={onSelectionChanged}
                                            onRowInvoked={onObjectInvoked}
                                            pagination={false}
                                        />                    
                                    </div> :
                                    <div>
                                        <div style={{ height: "10vh" }} />
                                        <div style={{ textAlign: "center" }}>
                                            <Typography size="5" text="No Objects Returned" />
                                        </div>
                                    </div>
                            }                
                            <div style={{ marginTop: theme.spacing(1), marginBottom: theme.spacing(1) }} >
                                { (objects?.length > 0) &&
                                    <Typography 
                                        size="1"
                                        text={
                                            selectedObjects.length > 0 ? 
                                                selectedObjects.length + " of " + objects.length + " objects selected." : 
                                                objects.length + " objects returned."
                                        }
                                    />
                                }
                            </div>
                            <ObjectNew                         
                                open={showNewObject}
                                adaptorId={adaptorId}
                                objectTypeId={objectTypeId}
                                objectTypeObject={objectTypeObject}
                                onAddObject={onAdd}
                                onDismiss={() => dispatch({ type: "CANCEL_ADD" })}
                            />  
                            <ObjectUploadModal 
                                adaptorId={adaptorId}
                                objectTypeId={objectTypeId}
                                objectTypeObject={objectTypeObject}                    
                                open={showUpload}
                                onDismiss={() => dispatch({ type: "CANCEL_UPLOAD" })}
                            />
                            <ObjectsBatchModifyModal 
                                open={showBatchEdit}
                                onDismiss={() => dispatch({ type: "CANCEL_BATCH_EDIT" })}
                                objects={selectedObjects}
                                objectTypeObject={objectTypeObject}
                            />
                            <Dialog 
                                open={showConfirmDeleteDialog}
                                title="Delete Objects"
                                subText="Are you sure you want to delete the selected object(s)?"
                                isBlocking={true}
                                footer={
                                    <div style={{ display: "flex", justifyContent: "flex-end", width: "100%" }}>
                                        <Button                                       
                                            style={{ marginRight: theme.spacing(1) }}
                                            label="Cancel"
                                            onClick={() => dispatch({ type: "CANCEL_DELETE" })}
                                        />
                                        <Button 
                                            color="primary"
                                            variant="contained"
                                            label="Delete"                                        
                                            onClick={onConfirmDelete}
                                        />                                        
                                    </div>
                                }
                            />
                            <ErrorDialog 
                                error={error}
                                onDismiss={() => dispatch({ type: "ERROR" })}
                            />                        
                        </div>
                    );
                }} />
            </Switch>
        </OperationalContext.Provider>
    );  
};

export default ObjectsTable;
