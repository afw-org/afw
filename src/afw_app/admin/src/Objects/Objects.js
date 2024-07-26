// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";
import {Switch, Route, useHistory, useLocation} from "react-router";
import Container from "../common/Container";

import {
    AdapterDropdown,
    Breadcrumb,
    Button,
    Checkbox,
    ObjectTypeDropdown,
    Responsive,
    TextField,
    Tooltip,
} from "@afw/react";

import {useApplication, useAppCore, useBreadcrumbs, useTheme} from "../hooks";

import ObjectsQueryBuilder from "./ObjectsQueryBuilder";
import ObjectNew from "./ObjectNew";
import ObjectsTable from "./ObjectsTable";
import ContextualHelpRoutes from "./ContextualHelp";
import {ContextualHelpButton, ContextualHelp} from "../common/ContextualHelp";

const breadcrumbsRoot = { text: "Objects", link: "/Objects" };

/**
 * The Objects layout provides a generic way to create, edit, and delete objects 
 * in any chosen adapter.  It uses the application and adapters to create pickers 
 * that the user may select from in order to Retrieve or Create new objects of 
 * a particular Object Type.   
 * 
 * When an object is selected, it may be edited in order to add or remove property 
 * values.  Finally, the object can be saved or changes discarded at any time.
 */
const Objects = () => {

    const [operation, setOperation] = useState("retrieve");
    const [queryCriteria, setQueryCriteria] = useState("");
    const [objectId, setObjectId] = useState();        
    const [selectedObjectTypeId, setSelectedObjectTypeId] = useState();
    const [selectedObjectTypeObject, setSelectedObjectTypeObject] = useState();
    const [showNewObject, setShowNewObject] = useState(false);
    const [buildQueryCriteria, setBuildQueryCriteria] = useState(false);
    const [showHelp, setShowHelp] = useState(false);        
    const [selectedObject, setSelectedObject] = useState(); 
    const [selectedAdapter, setSelectedAdapter] = useState();
    const [selectedAdapterId, setSelectedAdapterId] = useState();

    const breadcrumbItems = useBreadcrumbs(breadcrumbsRoot);
    const {application} = useAppCore();
    const {notification, marginHeight} = useApplication();
    const history = useHistory();
    const {pathname} = useLocation();
    const theme = useTheme();
    


    /**
     * parseApplication()
     *
     * This routine parses out the application object and uses the configured defaultAdapterId
     * as the default chosen adapterId.
     */
    useEffect(() => {        
        if (pathname && pathname.split("/").length > 2) 
            setSelectedAdapterId(pathname.split("/")[2]);
        else if (application)
            setSelectedAdapterId(application.getPropertyValue("defaultAdapterId"));
        else 
            // default to afw, if path and application do not specify
            setSelectedAdapterId("afw");
    }, [pathname, application]);

    /**
     * Callbacks fired when the user selects an adapterId or an objectType.  For now, we
     * simply save them away in our state, but we may want to do additional things later 
     * on.
     */
    const onSelectAdapterId = (selectedAdapterId, selectedAdapter) => {
        setSelectedAdapterId(selectedAdapterId);
        setSelectedAdapter(selectedAdapter);
    };

    const onSelectObjectType = (selectedObjectTypeId, selectedObjectTypeObject) => {
        setSelectedObjectTypeId(selectedObjectTypeId);
        setSelectedObjectTypeObject(selectedObjectTypeObject);
    };    

    /**
     * onRetrieve
     *
     * When the "Retrieve" button is clicked, this will push the appropriate URL into history, and 
     * the React Router code will be leveraged to retrieve the list of objects.
     */
    const onRetrieve = () => {
        history.push("/Objects/" + selectedAdapterId + "/" + selectedObjectTypeId + "?" + queryCriteria);
    };
    
    /**
     * onGet
     *
     * When the "Get" button is clicked, this will push the appropriate URL into history, and 
     * the React Router code will be leveraged to get the object to be edited.
     */
    const onGet = () => {
        history.push("/Objects/" + selectedAdapterId + "/" + selectedObjectTypeId + "/" + objectId);
    };
    
    /**
     * onAddObject
     *
     * This is fired when the "Create" button is clicked on the NewObject Modal, after the user has
     * filled out all of the requirements for a new object.  We save the changes by calling the object's
     * add() method and waiting for a response.
     */
    const onAddObject = async (object, again) => {
        if (!again)
            setShowNewObject(false);
        
        try {
            const response = object.add();
            const result = await response.result();            

            notification({ message: "Object " + result["objectId"] + " created.", type: "success", response });   
        } catch (error) {      
            notification({ message: error, type: "error" });
        }     
    };

    const canAdd = (selectedObjectTypeObject && (selectedObjectTypeObject.getPropertyValue("allowAdd") !== false));

    /*
     * This renders the main "Routing" table for all Object operations.  Depending on the url, the user
     * is routed to the appropriate component.  Valid url paths include:
     * 
     *      /Objects/:adapterId/:objectTypeId/:objectId
     * 
     *          This links directly to an object.
     * 
     *      /Objects/:adapterId/:objectTypeId?queryString
     * 
     *          This links to all of the objects for a given objectType with an appropriate queryString.
     * 
     *      /Objects/
     * 
     *          This is the default route, which allows the user to select an adapterId and objectType
     *          and perform a search or get operation.
     */
    return (
        <Container maxWidth="xl" style={{ height: "100%", overflow: "auto" }}>   
            <div id="admin-objects" style={{ display: "flex", flexDirection: "column", height: "calc(100vh - " + marginHeight + ")" }}>
                <div style={{ display: "flex", paddingBottom: theme.spacing(2), alignItems: "center" }}>
                    <div style={{ flex: 1 }}>
                        { (breadcrumbItems.length > 1) &&
                            <Breadcrumb items={breadcrumbItems} />   
                        }
                    </div>
                    <ContextualHelpButton showHelp={setShowHelp} />                                         
                </div>
                <div style={{ flex: 1, overflow: "auto" }}>
                    <Switch>                       
                        <Route path={"/Objects/:adapterId/:objectTypeId"} render={(props) => {                            
                            let {adapterId, objectTypeId} = props.match.params;

                            adapterId = decodeURIComponent(adapterId);
                            objectTypeId = decodeURIComponent(objectTypeId);

                            return (
                                <ObjectsTable
                                    adapterId={adapterId}
                                    objectTypeId={objectTypeId}     
                                    objectTypeObject={selectedObjectTypeObject}     
                                    onSelectObject={setSelectedObject}                  
                                />
                            );
                        }} />
                        <Route render={() => {                            
                            return (
                                <div style={{ padding: theme.spacing(1) }}>       
                                    <Responsive 
                                        items={[
                                            {       
                                                breakpoints: {
                                                    small: 12,
                                                    medium: 9,
                                                    large: 6,
                                                    xl: 4,
                                                    xxl: 3
                                                },                                  
                                                contains: 
                                                    <Tooltip 
                                                        content={selectedAdapter?.properties?.description || ""}
                                                        delay={2000}
                                                        target={
                                                            <div>
                                                                <AdapterDropdown 
                                                                    id="admin-objects-adapter-dropdown"
                                                                    value={selectedAdapterId}
                                                                    onChanged={onSelectAdapterId}
                                                                />                                                                
                                                            </div>
                                                        }
                                                    />  
                                            }
                                        ]}
                                    />  
                                    <Responsive 
                                        items={[
                                            {       
                                                breakpoints: {
                                                    small: 12,
                                                    medium: 9,
                                                    large: 6,
                                                    xl: 4,
                                                    xxl: 3
                                                },                                  
                                                contains: 
                                                    <Tooltip 
                                                        content={selectedObjectTypeObject?.getPropertyValue("description") || ""}
                                                        delay={2000}
                                                        target={
                                                            <div>
                                                                <ObjectTypeDropdown      
                                                                    id="admin-objects-objectType-dropdown"                                                                 
                                                                    adapterId={selectedAdapterId}
                                                                    label="Object Type"
                                                                    description="Select an Object Type Id."                                
                                                                    value={selectedObjectTypeId}
                                                                    onChanged={onSelectObjectType}        
                                                                    requireEntity={true}     
                                                                    onError={(error) => notification({ message: error, type: "error" })}    
                                                                    defaultFirstNonAdaptive={(selectedAdapterId !== "afw")}                                                   
                                                                />                                                                
                                                            </div>
                                                        }
                                                    />   
                                            }
                                        ]}
                                    />                                    
                                    <Responsive 
                                        items={[
                                            {
                                                breakpoints: {
                                                    small: 12,
                                                    medium: 9,
                                                    large: 8,
                                                    xl: 6,
                                                    xxl: 4,
                                                },
                                                contains:
                                                    operation === "retrieve" ? (     
                                                        <div style={{ width: "calc(100% - 16px)", display: "flex", alignItems: "center" }}>                                                                                                       
                                                            <TextField             
                                                                label="Query Criteria"
                                                                placeholder="property=value"
                                                                description="Enter the Query Criteria."
                                                                value={queryCriteria}
                                                                onChanged={setQueryCriteria}
                                                                onEnter={onRetrieve}                                                            
                                                            />
                                                            <Button   
                                                                label="Build Query Criteria"            
                                                                tooltip="Build Query Criteria"
                                                                type="icon"
                                                                icon="code"
                                                                onClick={() => setBuildQueryCriteria(true)}
                                                            />
                                                            <Checkbox                                                                                                
                                                                label="Retrieve"
                                                                value={operation === "retrieve" ? true : false}
                                                                onChanged={checked => setOperation(checked ? "retrieve" : "get")}
                                                            />
                                                        </div>                                                    
                                                    ) :
                                                        <div style={{ display: "flex", alignItems: "center" }}>
                                                            <TextField 
                                                                style={{ flex: 1 }}                                             
                                                                label="Object Id"
                                                                description="Enter the Object Id of the object to be returned."
                                                                value={objectId}                                                            
                                                                onChanged={setObjectId}
                                                            />
                                                            <Checkbox             
                                                                style={{ marginLeft: theme.spacing(2) }}                                                                                      
                                                                label="Retrieve"
                                                                value={operation === "retrieve" ? true : false}
                                                                onChanged={checked => setOperation(checked ? "retrieve" : "get")}
                                                            />
                                                        </div>
                                            }
                                        ]}
                                    />
                                    <Responsive 
                                        items={[
                                            {
                                                breakpoints: {
                                                    small: 12,
                                                    medium: 6,
                                                    large: 6,
                                                    xl: 4,
                                                    xxl: 3,
                                                },
                                                contains: 
                                                    <div style={{ display: "flex", marginTop: theme.spacing(4) }}>                                                        
                                                        <div>                                                            
                                                            {
                                                                operation === "retrieve" ?                                                                                                                                                    
                                                                    <Button                                          
                                                                        color="primary"
                                                                        variant="contained"
                                                                        label="Retrieve"
                                                                        aria-label="Retrieve Objects"
                                                                        icon="search"
                                                                        size="small"
                                                                        onClick={onRetrieve}
                                                                        disabled={!(selectedAdapterId && selectedObjectTypeId)}
                                                                        tooltip="Retrieve Objects"
                                                                    /> :
                                                                    <Button                                             
                                                                        color="primary"
                                                                        variant="contained"
                                                                        label="Get"
                                                                        aria-label="Get Object"
                                                                        icon="search"
                                                                        size="small"
                                                                        onClick={onGet}     
                                                                        disabled={!(selectedAdapterId && selectedObjectTypeId && objectId)}       
                                                                        tooltip="Get Object"                                                            
                                                                    />
                                                            }                                                            
                                                        </div>
                                                        <div style={{ marginLeft: theme.spacing(2) }}>         
                                                            <Button                                                                                              
                                                                label="New Object"
                                                                variant="contained"
                                                                icon="add"
                                                                size="small"
                                                                onClick={() => setShowNewObject(true)}        
                                                                tooltip={canAdd ? "Create New Object" : "Can't Create These Objects"}
                                                            />                                                                                                                                                                                                                                        
                                                        </div>
                                                    </div>
                                            }  
                                        ]}
                                    />
                                    <ObjectsQueryBuilder 
                                        open={buildQueryCriteria}
                                        disabled={selectedObjectTypeObject ? false : true}
                                        objectTypeObject={selectedObjectTypeObject}
                                        onDismiss={() => setBuildQueryCriteria(false)}                             
                                        adapterId={selectedAdapterId}
                                        objectTypeId={selectedObjectTypeId ? selectedObjectTypeId : ""}
                                        queryCriteria={queryCriteria}
                                        onApply={(queryCriteria) => {
                                            setQueryCriteria(queryCriteria);
                                            setBuildQueryCriteria(false);
                                        }}
                                    />
                                    <ObjectNew 
                                        open={showNewObject}
                                        adapterId={selectedAdapterId}
                                        objectTypeId={selectedObjectTypeId ? selectedObjectTypeId : ""}
                                        onAddObject={onAddObject}
                                        onDismiss={() => setShowNewObject(false)}
                                    />  
                                </div>
                            );
                        }} />
                    </Switch>
                </div>
                <ContextualHelp 
                    open={showHelp}
                    onClose={() => setShowHelp(false)}
                    routes={ContextualHelpRoutes}
                    selectedAdapterId={selectedAdapterId}
                    selectedObjectTypeId={selectedObjectTypeId}
                    selectedObject={selectedObject} 
                />     
            </div>
        </Container>
    );
};

export default Objects;
