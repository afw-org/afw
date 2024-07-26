// See the 'COPYING' file in the project root for licensing information.
import {useState, useMemo} from "react";
import {useLocation} from "react-router-dom";

import {
    Button,
    Checkbox,
    ChoiceGroup,
    Dialog,
    Dropdown,
    Link,
    Spinner,
    Table,
    TextField,
    Typography,
    useModel,
    useEventId
} from "@afw/react";

import {useAppCore, useTheme} from "../../../hooks";

/**
 * ModelObjectTypeChoice
 * 
 * Prompts for how to create a new Object Type definition:
 * 
 *  (a) Manually
 *  (b) Copy From Model
 *  (c) Map To Adapter
 */
export const ModelObjectTypeChoice = (props) => {

    const [createOption, setCreateOption] = useState("manual");
    const theme = useTheme();

    const {open, onDismiss, availableModels} = props;

    const onSelectCreateOption = (createOption) => {
        setCreateOption(createOption.key);
    };

    const onNext = () => {
        props.onNext(createOption);
    };

    return (
        <Dialog 
            open={open}
            onDismiss={onDismiss}
            title="Add Object Types"
            maxWidth="md"
            contains={                    
                <div style={{ padding: theme.spacing(2) }}>
                    <ChoiceGroup 
                        label="Choose how you would like to add new Object Type(s):"
                        options={[
                            { key: "manual", text: "Create Manually" },
                            { key: "copy", text: "Copy From Another Model", disabled: !availableModels || availableModels.length === 0 },
                            { key: "map", text: "Map To Another Adapter" }
                        ]}
                        selectedKey={createOption}
                        onChanged={onSelectCreateOption}
                    />
                </div>       
            }
            footer={
                <div style={{ display: "flex", padding: theme.spacing(2), width: "100%", justifyContent: "space-between" }}>
                    <Button 
                        size="small"
                        label="Cancel"
                        onClick={onDismiss}
                    />
                    <Button 
                        size="small"
                        color="primary"
                        variant="contained"
                        label="Next"     
                        endIcon="chevron_right"
                        onClick={onNext}
                    />
                </div>
            }
        />
    );
};

/**
 * ModelObjectTypeManualAdd
 * 
 * Modal to gather ObjectTypeId and Description for creating 
 * a new Object Type manually.
 */
export const ModelObjectTypeManualAdd = (props) => {

    const [objectTypeId, setObjectTypeId] = useState();
    const [description, setDescription] = useState();
    const theme = useTheme();

    const onCreate = () => {
        props.onCreate(objectTypeId, description);
    };

    const onDismiss = () => {
        /* reset state for next time */
        setObjectTypeId();
        setDescription();
        
        props.onDismiss();
    };

    return (
        <Dialog 
            open={props.open}
            onDismiss={onDismiss}
            blocking={true}                
            title="New Object Type"
            maxWidth="md"
            contains={
                <div style={{ padding: theme.spacing(2) }}>
                    <TextField 
                        id="new-object-type-object-type-id"
                        //eslint-disable-next-line
                        autoFocus={true}
                        label="Object Type Id"     
                        value={objectTypeId}
                        onChanged={setObjectTypeId}                           
                    />
                    <TextField 
                        id="new-object-type-description"
                        label="Description"
                        multiline={true}
                        rows={4}
                        value={description}
                        onChanged={setDescription}  
                    />
                </div>   
            }
            footer={
                <div style={{ width: "100%", display: "flex", padding: theme.spacing(2), justifyContent: "flex-end" }}>
                    <div style={{ paddingRight: theme.spacing(1) }}>
                        <Button 
                            size="small"
                            color="primary"
                            variant="contained"
                            label="Create"  
                            onClick={onCreate} 
                            disabled={!objectTypeId}
                        />
                    </div>
                    <div>
                        <Button 
                            size="small"
                            label="Cancel"
                            onClick={onDismiss}
                        />
                    </div>
                </div>
            }
        />
    );
};

/**
 * ModelObjectTypeCopy
 * 
 * When the user requests to copy or inherit Object Types from 
 * another model, this component will step them through the process
 * to select the Model and Object Type(s) they wish to copy.
 */
export const ModelObjectTypeCopy = (props) => {

    const [selectedObjectTypes, setSelectedObjectTypes] = useState({});
    const [objectTypeObjects, setObjectTypeObjects] = useState();
    const [selectedModel, setSelectedModel] = useState();
    const [canCopy, setCanCopy] = useState(false);
    const theme = useTheme();

    const {models, availableModels} = props;

    const onDismiss = () => {
        props.onDismiss();
    };

    /* When a Model is selected, display all of the available objectTypeObjects. */
    const onSelectModel = (selectedModel) => {    
        const model = selectedModel.model;    

        /* Load Object Types from this selected model*/
        const objectTypeObjects = model.getPropertyValue("objectTypes");

        setSelectedModel(selectedModel);
        setObjectTypeObjects(objectTypeObjects);
    };

    /* When an object type is checked */
    const onSelectObjectTypeId = (objectTypeId, checked) => {
        let selected = selectedObjectTypes;

        if (checked)
            selected[objectTypeId] = checked;
        else
            delete selected[objectTypeId];

        setSelectedObjectTypes(selected);
        setCanCopy(Object.keys(selected).length > 0 );
    };

    /* checks to make sure there isn't already one with the same name */
    const validObjectTypeId = (objectTypeId) => {
        const model = props.model;

        if (model.getPropertyValue("objectTypes").hasProperty(objectTypeId))
            return false;

        return true;
    };

    /* When the user is finished selecting object types and hits "Copy" */
    const onCopy = () => {

        let copiedObjectTypes= [];
        for (let objectType of Object.keys(selectedObjectTypes)) {
            let copiedObjectType = selectedModel.model.getProperty(["objectTypes", objectType]);
            copiedObjectTypes.push(copiedObjectType);
        }

        props.onCopy(copiedObjectTypes);
    };

    if (!models)
        return (
            <Spinner 
                size="large"
                text="Loading Models.."
                fullScreen={true}
            />
        );

    return (
        <Dialog 
            open={props.open}
            onDismiss={onDismiss}
            blocking={true}
            title="Copy Object Types"
            maxWidth="lg"
            contains={
                <div style={{ padding: theme.spacing(2) }}>
                    <Dropdown 
                        label="Select From a Model"
                        description="Select the Model you wish to copy Object Types from."
                        options={availableModels.map(model => ({
                            key: model.getPropertyValue("modelId"),
                            text: model.getPropertyValue("modelId"),
                            model
                        }))}
                        onChanged={onSelectModel}
                        selectedKey={selectedModel ? selectedModel.key : undefined}
                    />
                    {
                        (objectTypeObjects) && 
                        <div style={{ paddingTop: theme.spacing(2), height: "50vh", overflow: "auto" }}>
                            <Typography text="Select the Object Types to Copy:" />
                            <ul style={{ listStyleType: "none" }}>
                                {
                                    objectTypeObjects.getProperties().map(property => property.getName()).map(objectTypeId => 
                                        <li key={objectTypeId}>
                                            <Checkbox 
                                                value={selectedObjectTypes[objectTypeId]}
                                                label={objectTypeId}                                                        
                                                onChanged={(checked) => onSelectObjectTypeId(objectTypeId, checked)}
                                                disabled={!validObjectTypeId(objectTypeId)}
                                                description={objectTypeObjects.getPropertyValue([objectTypeId, "description"])}
                                            />                                                    
                                        </li>
                                    )
                                }
                            </ul>
                        </div>
                    }
                </div>                                            
            }
            footer={
                <div style={{ width: "100%", display: "flex", justifyContent: "flex-end", padding: theme.spacing(2) }}>
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button 
                            size="small"
                            color="primary"
                            variant="contained"
                            label="Copy" 
                            disabled={!canCopy}             
                            onClick={onCopy}                        
                        />
                    </div>
                    <div>
                        <Button 
                            size="small"
                            label="Cancel"
                            onClick={onDismiss}
                        />
                    </div>
                </div>
            }
        />
    );

};

/**
 * ModelObjectTypeMap
 * 
 * When the user requests to map  Object Types from 
 * another adapter, this component will step them through the process
 * to select the adapter and Object Type(s) they wish to map.
 */
export const ModelObjectTypeMap = (props) => {

    const [selectedObjectTypes, setSelectedObjectTypes] = useState({});
    const [canMap, setCanMap] = useState(false);
    const [isLoading, setIsLoading] = useState(false);
    const [objectTypeObjects, setObjectTypeObjects] = useState();
    const [selectedAdapter, setSelectedAdapter] = useState();

    const theme = useTheme();
    const model = useModel();

    const onDismiss = () => {
        props.onDismiss();
    };

    /* When a Model is selected, display all of the available objectTypeObjects. */
    const onSelectAdapter = async (selectedAdapter) => {    
        const adapterId = selectedAdapter.key;    

        /* Load Object Types from this selected model*/
        setIsLoading(true);
        const {objects} = model.retrieveObjects({
            objectTypeId: "_AdaptiveObjectType_", 
            adapterId, 
            objectOptions: {path: true, objectId: true, objectType: true},            
        });
        let objectTypeObjects = await objects;
        
        // \fixme this is built into modelOptions now
        await Promise.all(objectTypeObjects.map(o => o.initialize()));
        objectTypeObjects = objectTypeObjects.sort((A, B) => A.getObjectId().toLowerCase().localeCompare(B.getObjectId().toLowerCase()));

        setIsLoading(false);
        setSelectedAdapter(selectedAdapter);
        setObjectTypeObjects(objectTypeObjects);
    };

    /* When an object type is checked */
    const onSelectObjectTypeId = (objectTypeId, checked) => {
        let selected = selectedObjectTypes;

        if (checked)
            selected[objectTypeId] = checked;
        else
            delete selected[objectTypeId];

        setSelectedObjectTypes(selected);
        setCanMap(Object.keys(selected).length > 0);
    };

    /* checks to make sure there isn't already one with the same name */
    const validObjectTypeId = (objectTypeId) => {
        if (props.model.getPropertyValue("objectTypes").hasProperty(objectTypeId))
            return false;

        return true;
    };

    /* When the user is finished selecting object types and hits "Map" */
    const onMap = () => {
        
        let mappedObjectTypes= [];
        for (let objectType of Object.keys(selectedObjectTypes)) {
            let mappedObjectType = objectTypeObjects.filter(o => o.getObjectId() === objectType)[0];
            mappedObjectTypes.push(mappedObjectType);
        }

        props.onMap(mappedObjectTypes);
    };

    const models = props.models;
    const {adapters} = useAppCore();

    if (!models)
        return (
            <Spinner 
                size="large"
                label="Loading Models.."
                fullScreen={true}
            />
        );

    return (
        <Dialog 
            open={props.open}
            onDismiss={onDismiss}
            blocking={true}
            title="Map Object Types"
            maxWidth="md"
            contains={                    
                <div style={{ padding: theme.spacing(2) }}>
                    <Dropdown 
                        label="Select an Adapter"
                        description="Select the Adapter you wish to Map Object Types from."
                        options={adapters.map(adapter => ({
                            key: adapter.adapterId,
                            text: adapter.adapterId,
                            adapter
                        }))}
                        onChanged={onSelectAdapter}
                        selectedKey={selectedAdapter ? selectedAdapter.key : undefined}
                    />
                    {
                        isLoading && <Spinner size="large" label="Loading Object Types..." />
                    }
                    {
                        (objectTypeObjects) && 
                        <div style={{ paddingTop: theme.spacing(2), height: "50vh", overflow: "auto" }}>
                            <Typography text="Select the Object Types to Map:" />
                            <div style={{ height: theme.spacing(2) }} />
                            <ul style={{ listStyleType: "none" }}>
                                {
                                    objectTypeObjects.map(objectType => 
                                        <li key={objectType.getObjectId()}>
                                            <Checkbox 
                                                value={selectedObjectTypes[objectType.getObjectId()]}
                                                label={objectType.getObjectId()}                                                        
                                                onChanged={(checked) => onSelectObjectTypeId(objectType.getObjectId(), checked)}
                                                disabled={!validObjectTypeId(objectType.getObjectId())}
                                                description={objectType.getPropertyValue("description")}
                                            />                                                    
                                        </li>
                                    )
                                }
                            </ul>
                        </div>
                    }
                </div>   
            }
            footer={
                <div style={{ width: "100%", display: "flex", justifyContent: "flex-end", padding: theme.spacing(2) }}>
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button 
                            size="small"
                            color="primary"
                            variant="contained"
                            label="Map" 
                            disabled={!canMap}             
                            onClick={onMap}                        
                        />
                    </div>
                    <div>
                        <Button 
                            size="small"
                            label="Cancel"
                            onClick={onDismiss}
                        />
                    </div>
                </div>
            }
        />
    );
};

/**
 * ObjectTypesTable
 * 
 * A simple Table editor for quickly viewing Object Type
 * definitions within the model.  It provides a link to the 
 * Object Type for more details, along with a description.
 * 
 * Additionally, add/remove buttons to manage them.
 */
export const ModelObjectTypesTable = (props) => {

    const [selectedObjectTypes, setSelectedObjectTypes] = useState([]);
    const [showNewObjectType, setShowNewObjectType] = useState(false);
    const [showNewManualObjectType, setShowNewManualObjectType] = useState(false);
    const [showCopyObjectTypes, setShowCopyObjectTypes] = useState(false);
    const [showMapObjectTypes, setShowMapObjectTypes] = useState(false);
    const [createOption, setCreateOption] = useState();

    const {adapterId, model, objectTypes, models} = props;
    const modelId = model.getPropertyValue("modelId");   

    const changed = useEventId({ object: objectTypes, eventId: "onChildChanged" });
    const {hash} = useLocation();
    const theme = useTheme();

    const objectTypePropertyNames = useMemo(() => {
        let objectTypePropertyNames = [];        
            
        if (objectTypes && (changed || !changed)) {
            objectTypePropertyNames = objectTypes.getProperties().map(property => property.getName()).sort((A, B) => {
                const a = A.toLowerCase();
                const b = B.toLowerCase();

                return (a.localeCompare(b));
            });
        }

        return objectTypePropertyNames;        

    }, [objectTypes, changed]);

    /**
     * onCreateObjectType()
     * 
     * When an objectTypeId and description are gathered, this routine will
     * go ahead and create the objectType definition in the working model.
     */
    const onCreateObjectType = (objectTypeId, description) => {
        let modelObjectTypeObjects = model.getPropertyValue("objectTypes");

        if (!modelObjectTypeObjects) {
            /* We do not have objectTypes defined in this model yet */
            modelObjectTypeObjects = model.getProperty("objectTypes");
            modelObjectTypeObjects.setValue({});            
        }

        const newModelObjectTypeObject = modelObjectTypeObjects.newProperty(objectTypeId);        
        newModelObjectTypeObject.setValue({ description });

        setShowNewManualObjectType(false);
        setShowNewObjectType(false);
    };

    /**
     * onRemoveObjectType()
     * 
     * When one or more object types are selected, and the user clicks the 
     * "remove" button, this removes them from the objectTypes and table.
     */
    const onRemoveObjectType = () => {       
        const modelObjectTypeObjects = model.getPropertyValue("objectTypes");

        selectedObjectTypes.forEach( selectedObjectType => {
            modelObjectTypeObjects.getProperty(selectedObjectType).remove();
        });

        setSelectedObjectTypes([]);
    };

    /**
     * onCopyObjectTypes()
     * 
     * When the user selects one or more Object Types from which to copy,
     * this routine will add them to the existing model.
     */
    const onCopyObjectTypes = (copiedObjectTypes) => {
        const modelObjectTypes = model.getPropertyValue("objectTypes");
        
        copiedObjectTypes.forEach(copiedObjectType => {            
            const name = copiedObjectType.getName();
            const value = copiedObjectType.getValue().toJSON();

            modelObjectTypes.setPropertyValue(name, value);
        });

        setShowCopyObjectTypes(false);        
        setShowNewObjectType(false);
    };

    /**
     * onMapObjectTypes()
     * 
     */
    const onMapObjectTypes = (mappedObjectTypes) => {
        const modelObjectTypes = model.getPropertyValue("objectTypes");

        mappedObjectTypes.forEach(mappedObjectType => { 
            modelObjectTypes.setPropertyValue(mappedObjectType.getObjectId(), mappedObjectType.toJSON());
        });

        setShowMapObjectTypes(false);
        setShowNewObjectType(false);
    };


    if (!objectTypes)
        return null;

    const availableModels = models.filter( 
        m => m.getPropertyValue("modelId") !== modelId
    );

    return (
        <div>
            <div style={{ display: "flex" }}>                    
                <div>
                    <Button                                                                        
                        type="icon"
                        label="Add New Object Type"
                        icon="add"
                        tooltip="Add New Object Type"
                        color="primary"        
                        onClick={() => setShowNewObjectType(true)}               
                    />        
                </div>
                <div style={{ marginRight: theme.spacing(1) }}>
                    <Button 
                        type="icon"
                        label="Remove Object Type"
                        icon="remove"                       
                        disabled={selectedObjectTypes.length === 0}
                        tooltip="Remove Object Type"       
                        color="primary"            
                        onClick={onRemoveObjectType}         
                    />                        
                </div>                    
            </div>
            <Table 
                aria-label="Object Types"
                selectionMode="multiple"
                compact={true}
                rows={objectTypePropertyNames}
                onSelectionChanged={setSelectedObjectTypes}
                columns={[
                    {
                        key: "ObjectType",
                        name: "Object Type",
                        minWidth: 100,
                        maxWidth: 200,
                        width: "20%",
                        isResizable: true,
                        onRender: (objectType) => {                            
                            return (
                                <Link 
                                    uriComponents={[
                                        "Admin", "Models", adapterId, modelId, "objectTypes", 
                                        objectType, hash
                                    ]}                                                                           
                                    text={objectType}
                                />
                            );
                        }
                    },
                    {
                        key: "Description",
                        name: "Description",
                        minWidth: 200,
                        maxWidth: 400,
                        width: "80%",
                        isResizable: true,
                        isMultiline: true,
                        onRender: (objectType) => {
                            const objectTypeObject = objectTypes.getPropertyValue(objectType);
                            return (
                                <Typography 
                                    size="2"
                                    text={objectTypeObject.getPropertyValue("description")}
                                />
                            );
                        }
                    }
                ]}        
            />                
            <ModelObjectTypeChoice 
                {...props}
                availableModels={availableModels}
                open={showNewObjectType}
                onDismiss={() => setShowNewObjectType(false)}
                onNext={(createOption) => {
                    setShowNewManualObjectType(false);
                    setCreateOption(createOption);

                    if (createOption === "manual")
                        setShowNewManualObjectType(true);
                    else if (createOption === "copy")
                        setShowCopyObjectTypes(true);
                    else 
                        setShowMapObjectTypes(true);
                }}
            />
            <ModelObjectTypeManualAdd 
                {...props}
                open={showNewManualObjectType}
                onDismiss={() => setShowNewManualObjectType(false)}
                onCreate={onCreateObjectType}
            />
            <ModelObjectTypeCopy
                {...props} 
                availableModels={availableModels}
                open={showCopyObjectTypes}
                createOption={createOption}
                onDismiss={() => setShowCopyObjectTypes(false)}
                onCopy={onCopyObjectTypes}
            />
            <ModelObjectTypeMap 
                {...props}                                             
                open={showMapObjectTypes}
                createOption={createOption}
                onDismiss={() => setShowMapObjectTypes(false)}
                onMap={onMapObjectTypes}
            />
        </div>
    );
};

export default ModelObjectTypesTable;
