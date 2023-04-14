// See the 'COPYING' file in the project root for licensing information.
import {useState, useMemo} from "react";
import {Route, Switch} from "react-router";

import {
    Button,
    Link,
    Message,
    Spinner,
    Table,
    Typography,
    useRetrieveObjects,
    objectOptions_objectId_objectType_path,
} from "@afw/react";

import {useTheme} from "../../hooks";
import {ObjectType} from "./ObjectType";

/**
 * Displayed when an error occurs loading object types.
 */
const ObjectTypesError = ({ error }) => 
    <>
        <Typography color="textSecondary" text="There was an error loading Object Types." />
        <Typography text={error.message} />
    </>;

/**
 * Displayed when the selected adaptorId has no Object Type definitions that can be stored.
 */
const ObjectTypesNone = () => {

    const theme = useTheme();

    return (
        <div style={{ padding: theme.spacing(1) }}>
            <Message                                         
                contains={
                    <div>
                        <Typography     
                            color="textSecondary"                          
                            text="This adaptor contains no Object Types and user-defined Object Types cannot be created and stored directly in this adaptor type." 
                        />
                        <div style={{ height: theme.spacing(2) }} />
                        <Typography 
                            style={{ display: "inline-block" }}
                            color="textSecondary"
                            text="You must create a" 
                        />
                        <Link 
                            style={{ 
                                display: "inline-block", 
                                marginLeft: theme.spacing(0.5),
                                marginRight: theme.spacing(0.5)
                            }} 
                            text="Model" url="/Admin/Models/" />
                        <Typography 
                            style={{ display: "inline-block" }}
                            color="textSecondary"
                            text="and map it onto this adaptor." 
                        />
                    </div>
                }
            />                
        </div>
    );
};

/*
 * ObjectTypes
 *
 * This component displays a table of Object Type objects for the
 * user to select by clicking on the generated URL link.
 */
export const ObjectTypes = (props) => {

    const theme = useTheme();

    const [selected, setSelected] = useState([]);
    const [allowAdd, setAllowAdd] = useState();
    const [allowChange, setAllowChange] = useState();
    const [allowDelete, setAllowDelete] = useState();

    const {adaptorId} = props;
    
    /* use the useRetrieveObjects() hook to retrieve all Object Types from this adaptor */
    const {objects, isLoading, error} = useRetrieveObjects({ 
        adaptorId, 
        objectTypeId: "_AdaptiveObjectType_", 
        objectOptions: objectOptions_objectId_objectType_path
    });

    /* 
     * Once the Object Type objects are retrieved, read the _AdaptiveObjectType_ object
     * to determine if Object Types can be created or deleted in this adaptor.
     */
    const objectTypeObjects = useMemo(() => {
        /* filter out the _AdaptiveObjectType_ object type */
        if (objects) {
            const objectTypeObjects = objects.filter(o => {
                if (o.getObjectId() === "_AdaptiveObjectType_") {
                    const {allowAdd, allowChange, allowDelete} = o.getPropertyValues();

                    setAllowAdd(allowAdd);
                    setAllowChange(allowChange);
                    setAllowDelete(allowDelete);

                    return false;
                } else return true;
            }).sort((A, B) => {
                return (A.getObjectId().toLowerCase().localeCompare(B.getObjectId().toLowerCase()));
            });

            return objectTypeObjects;
        } else return [];
    }, [objects]);

    /* Return a progress indicator while the fetch is loading */
    if (isLoading)
        return <Spinner size="large" label="Retrieve Object Types..." fullScreen={true} />;

    /* Return an error, if it failed to retrieve Object Types */
    if (error) {
        return <ObjectTypesError error={error} />;
    }

    /* Some adaptors do not have Object Types, or even allow new ones to be created */
    if ((allowAdd === false) && (allowDelete === false) && objectTypeObjects.length === 0) {
        return <ObjectTypesNone />;
    }

    /* Return the Table with routable links to manage these Object Types */
    return (
        <Switch>
            <Route exact path="/Admin/Schema/:adaptorId" render={() => 
                <div style={{ display: "flex", flexDirection: "column", padding: theme.spacing(1), height: "100%" }}>
                    { (allowAdd || allowDelete) ? (
                        <div>
                            <div style={{ display: "inline-block" }}>
                                <Button 
                                    label="New"
                                    variant="text"
                                    color="primary"
                                    size="small"
                                    icon="add"
                                    disabled={!allowAdd}
                                />
                            </div>
                            <div style={{ display: "inline-block", marginLeft: theme.spacing(1) }}>
                                <Button 
                                    label="Remove"
                                    variant="text"
                                    size="small"
                                    icon="remove"
                                    disabled={selected.length === 0 || !allowDelete}
                                />
                            </div>
                            <div style={{ height: theme.spacing(2) }} />
                        </div>
                    ) : (
                        <div>
                            <Message 
                                message="Object Types cannot be managed directly through this adaptor." 
                            />
                            <div style={{ height: theme.spacing(2) }} />
                        </div>
                    )}
                    <div style={{ flex: 1, overflow: "auto" }}>
                        <Table 
                            rows={objectTypeObjects}
                            columns={[
                                {
                                    key: "objectTypeId", 
                                    name: "Object Type", 
                                    isResizable: true, 
                                    minWidth: 120, 
                                    maxWidth: 200,
                                    width: "20%", 
                                    style: { wordWrap: "break-word" },
                                    onRender: objectTypeObject => {
                                        const objectTypeId = objectTypeObject.getObjectId();

                                        return (
                                            <Link 
                                                text={objectTypeId}
                                                uriComponents={["Admin", "Schema", adaptorId, objectTypeId]}
                                            />
                                        );
                                    }
                                },
                                {
                                    key: "description", 
                                    name: "Description", 
                                    isResizable: true, 
                                    minWidth: 100, 
                                    maxWidth: 400,
                                    isMultiline: true,
                                    width: "80%", 
                                    style: { wordWrap: "break-word" },
                                    onRender: objectTypeObject => objectTypeObject.getPropertyValue("description")?.replaceAll("\\n", "\n")
                                }
                            ]}
                            selectionMode="multiple"
                            onSelectionChanged={selected => setSelected(selected)}
                        />
                    </div>
                    <Typography size="1" color="textSecondary" text={objectTypeObjects.length + " Object Types found."} />
                </div>
            } />
            <Route path="/Admin/Schema/:adaptorId/:objectTypeId" render={(routeProps) => {
                let objectTypeObject;
                const objectTypeId = routeProps.match.params.objectTypeId;

                if (objectTypeId && objectTypeObjects) {
                    objectTypeObjects.forEach(o => {
                        if (o.getObjectId() === objectTypeId)
                            objectTypeObject = o;
                    });

                    if (!objectTypeObject)
                        return <Typography text="Object Type not found." />;    

                    return (
                        <ObjectType 
                            {...props}
                            objectTypeId={objectTypeId}
                            objectTypeObject={objectTypeObject}
                            allowChange={allowChange}
                        />
                    );
                } else return null;
            }} />
        </Switch>
    );
};

export default ObjectTypes;
