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
    useEventId
} from "@afw/react";

import {useTheme} from "../../../hooks";

/**
 * ModelPropertyTypeChoice
 * 
 * Prompts for how to create a new Property Type definition:
 * 
 *  (a) Manually
 *  (b) Copy From Model
 *  (c) Inherit From Modal
 */
export const ModelPropertyTypeChoice = (props) => {

    const theme = useTheme();
    const [createOption, setCreateOption] = useState("manual");
    
    const onSelectCreateOption = (createOption) => {
        setCreateOption(createOption.key);
    };

    const onDismiss = () => {
        props.onDismiss();
    };

    const onNext = () => {
        props.onNext(createOption);
    };

    return (
        <Dialog 
            open={props.open}
            onDismiss={props.onDismiss}
            title="Add Model Properties"
            maxWidth="md"
            contains={
                <div style={{ padding: theme.spacing(2) }}>
                    <ChoiceGroup 
                        label="Select the method of adding new Property Type(s):"
                        options={[
                            { key: "manual", text: "Create Manually" },
                            { key: "copy", text: "Copy From Another Model" },
                            { key: "inherit", text: "Inherit From Another Model" }
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
 * ModelPropertyTypeManualAdd
 * 
 * Modal to gather Property Name and Description for creating 
 * a new Property Type manually.
 */
export const ModelPropertyTypeManualAdd = (props) => {

    const [propertyName, setPropertyName] = useState();
    const [description, setDescription] = useState();
    const theme = useTheme();

    const onCreate = () => {
        props.onCreate(propertyName, description);
    };

    const onDismiss = () => {
        /* reset state for next time */
        setPropertyName();
        setDescription();

        props.onDismiss();
    };

    return (
        <Dialog 
            open={props.open}
            onDismiss={onDismiss}
            blocking={true}
            title="New Property Type"
            maxWidth="md"
            contains={
                <div style={{ padding: theme.spacing(2) }}>
                    <TextField 
                        id="new-property-type-property-name"
                        //eslint-disable-next-line
                        autoFocus={true}
                        label="Property Name"     
                        value={propertyName}
                        onChanged={setPropertyName}                           
                    />
                    <TextField 
                        id="new-property-type-description"
                        label="Description"
                        multiline={true}
                        rows={4}
                        value={description}
                        onChanged={setDescription}  
                    />
                </div>
            }
            footer={
                <div style={{ display: "flex", justifyContent: "flex-end", padding: theme.spacing(2), width: "100%" }}>
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button 
                            size="small"
                            color="primary"
                            variant="contained"
                            label="Create"  
                            onClick={onCreate} 
                            disabled={!propertyName}
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
 * ModelPropertyTypeCopy
 * 
 * When the user requests to copy or inherit Property Types from 
 * another model, this component will step them through the process
 * to select the Model and Property Type(s) they wish to copy.
 */
export const ModelPropertyTypeCopy = (props) => {

    const [selectedPropertyTypes, setSelectedPropertyTypes] = useState({});
    const [selectedObjectType, setSelectedObjectType] = useState();
    const [canCopy, setCanCopy] = useState(false);    
    const [selectedModel, setSelectedModel] = useState();
    const [objectTypeObjects, setObjectTypeObjects] = useState();
    const theme = useTheme();

    const {objectTypeObject, model, models} = props;

    const onDismiss = () => {
        props.onDismiss();
    };

    /* When a Model is selected, display all of the available objectTypeProperties. */
    const onSelectModel = (selectedModel) => {    
        const model = selectedModel.model;    

        /* Load Object Types from this selected model*/
        const objectTypeObjects = model.getPropertyValue("objectTypes");

        setSelectedModel(selectedModel);
        setObjectTypeObjects(objectTypeObjects);
    };

    const onSelectProperty = (propertyName, checked) => {
        let selected = selectedPropertyTypes;

        if (checked)
            selected[propertyName] = true;
        else 
            delete selected[propertyName];

        setSelectedPropertyTypes(selected);
        setCanCopy(Object.keys(selectedPropertyTypes).length > 0);
    };

    /* checks to make sure there isn't already one with the same name */
    const validPropertyName = (propertyName) => {
        if (objectTypeObject.getPropertyValue("propertyTypes").hasProperty(propertyName))
            return false;

        return true;
    };

    /* When the user is finished selecting object types and hits "Copy" */
    const onCopy = () => {

        let copiedPropertyTypes= [];
        for (let propertyName of Object.keys(selectedPropertyTypes)) {
            let copiedPropertyType = selectedObjectType.object.getProperty(["propertyTypes", propertyName]);
            copiedPropertyTypes.push(copiedPropertyType);
        }

        props.onCopy(copiedPropertyTypes);
    };

    if (!models)
        return (
            <Spinner 
                size="large"
                text="Loading Models.."
                fullScreen={true}
            />
        );

    const availableModels = models.filter( 
        m => m.getPropertyValue("modelId") !== model.getPropertyValue("modelId")
    );

    return (
        <Dialog 
            open={props.open}
            onDismiss={onDismiss}
            blocking={true}
            title="Copy Property Types"
            maxWidth="lg"
            contains={
                <div style={{ padding: theme.spacing(2) }}>
                    <Dropdown 
                        label="Select From a Model"
                        description="Select the Model you wish to copy Property Types from."
                        options={availableModels.map(model => ({
                            key: model.getPropertyValue("modelId"),
                            text: model.getPropertyValue("modelId"),
                            model
                        }))}
                        onChanged={onSelectModel}
                        selectedKey={selectedModel ? selectedModel.key : undefined}
                    />
                    {
                        selectedModel && selectedModel.model.getPropertyValue("objectTypes") && (
                            <div style={{ paddingTop: theme.spacing(2) }}>
                                <Dropdown 
                                    label="Select an Object Type"
                                    description="Select the Object Type you wish to copy Property Types from."
                                    options={selectedModel.model.getPropertyValue("objectTypes").getProperties().map(property => property.getName()).map(objectTypeId => ({
                                        key: objectTypeId,
                                        text: objectTypeId,  
                                        object: objectTypeObjects.getPropertyValue(objectTypeId)
                                    }))}
                                    onChanged={setSelectedObjectType}
                                    selectedKey={selectedObjectType ? selectedObjectType.key : undefined}
                                />
                            </div>
                        )
                    }
                    {
                        selectedObjectType && selectedObjectType.object.getProperty("propertyTypes") && (
                            <div style={{ paddingTop: theme.spacing(2), height: "50vh", overflow: "auto" }}>
                                <Typography text="Select the Property Types to Copy:" />
                                <ul style={{ listStyleType: "none" }}>
                                    {
                                        selectedObjectType.object.getPropertyValue("propertyTypes").getProperties().map(property => property.getName()).map(propertyName => 
                                            <li key={propertyName}>
                                                <Checkbox 
                                                    value={selectedPropertyTypes[propertyName]}
                                                    label={propertyName}
                                                    description={selectedObjectType.object.getPropertyValue(["propertyTypes", propertyName, "description"])}
                                                    onChanged={(checked) => onSelectProperty(propertyName, checked)}
                                                    disabled={!validPropertyName(propertyName)}
                                                />
                                            </li>
                                        )
                                    }
                                </ul>
                            </div>
                        )
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
 * PropertyTypesTable
 * 
 * A simple Table editor for quickly viewing Property Type
 * definitions within the model.  It provides a link to the 
 * Property Type for more details, along with a description.
 * 
 * Additionally, add/remove buttons to manage them.
 */
export const ModelPropertyTypesTable = (props) => {

    const {hash} = useLocation();
    const [selectedPropertyTypes, setSelectedPropertyTypes] = useState([]);
    const [createOption, setCreateOption] = useState();
    const [showNewPropertyType, setShowNewPropertyType] = useState(false);
    const [showNewManualPropertyType, setShowNewManualPropertyType] = useState(false);
    const [showCopyPropertyTypes, setShowCopyPropertyTypes] = useState(false);
    const theme = useTheme();

    const {objectTypeObject, propertyTypes, model} = props;
    const modelId = model.getPropertyValue("modelId");
    const adapterId = props.adapterId;
    const objectType = props.objectType;

    const changed = useEventId({ object: objectTypeObject, eventId: "onChildChanged" });

    const propertyTypePropertyNames = useMemo(() => {
            
        let propertyTypePropertyNames = [];
        if (propertyTypes && (changed || !changed)) {             
            propertyTypePropertyNames = propertyTypes.getProperties().map(property => property.getName()).sort((A, B) => {
                const a = A.toLowerCase();
                const b = B.toLowerCase();

                return (a.localeCompare(b));
            });
        }
    
        return propertyTypePropertyNames;
        
    }, [propertyTypes, changed]);


    /**
     * onCreatePropertyType()
     * 
     * When the user has chosen to manually create a Property Type, by 
     * specifying a property name and description, this routine is invoked
     * to add it to our propertyTypes.
     */
    const onCreatePropertyType = (propertyName, description) => {
        const objectTypeObject = props.objectTypeObject;
        let modelPropertyTypeObjects = objectTypeObject.getPropertyValue("propertyTypes");

        if (!modelPropertyTypeObjects) {
            /* We do not have propertyTypes defined in this objectType yet */
            modelPropertyTypeObjects = objectTypeObject.getProperty("propertyTypes");
            modelPropertyTypeObjects.setValue({});

            /* now get it again */
            modelPropertyTypeObjects = objectTypeObject.getPropertyValue("propertyTypes");
        }

        const newModelPropertyTypeObject = modelPropertyTypeObjects.newProperty(propertyName);        
        newModelPropertyTypeObject.setValue({ description });

        setShowNewPropertyType(false);
        setShowNewManualPropertyType(false);
    };

    /**
     * onRemovePropertyType()
     * 
     * When one or more property types are selected, and the user clicks the 
     * "remove" button, this removes them from the propertyTypes and table.
     */
    const onRemovePropertyType = () => {
        const modelPropertyTypeObjects = objectTypeObject.getPropertyValue("propertyTypes");

        selectedPropertyTypes.forEach(selectedPropertyType => {
            modelPropertyTypeObjects.getProperty(selectedPropertyType).remove();
        });

        setSelectedPropertyTypes([]);
    };

    /**
     * onCopyPropertyTypes()
     * 
     * When the user selects one or more Property Types from which to copy,
     * this routine will add them to the existing model.
     */
    const onCopyPropertyTypes = (copiedPropertyTypes) => {
        const objectTypeObject = props.objectTypeObject;
        let modelPropertyTypes = objectTypeObject.getPropertyValue("propertyTypes");

        copiedPropertyTypes.forEach(copiedPropertyType => {
            
            const name = copiedPropertyType.getName();
            const value = copiedPropertyType.getValue().toJSON();

            modelPropertyTypes.setPropertyValue(name, value);
        });

        setShowCopyPropertyTypes(false);
    };
   
    /*! \fixme
        If propertyTypes are null, and we go ahead and create some, then the parent will 
        have to detect notification and re-render us.
     */

    return (
        <div>
            <div style={{ display: "flex" }}>
                <div>                    
                    <Button                                                                        
                        type="icon"
                        icon="add"
                        tooltip="Add New Property Type"
                        label="Add New Property Type"
                        color="primary"             
                        onClick={() => setShowNewPropertyType(true)}               
                    />        
                </div>
                <div style={{ marginRight: theme.spacing(1) }}>
                    <Button 
                        type="icon"
                        icon="remove"                        
                        disabled={selectedPropertyTypes.length === 0}
                        tooltip="Remove Property Type"    
                        label="Remove Property Type"   
                        color="primary"        
                        onClick={onRemovePropertyType}             
                    />
                </div>
            </div>                                    
            <Table 
                aria-label="Property Types"
                selectionMode="multiple"
                compact={true}
                rows={propertyTypePropertyNames}
                onSelectionChanged={setSelectedPropertyTypes}
                columns={[
                    {
                        key: "PropertyType",
                        name: "Property Type",
                        minWidth: 100,
                        maxWidth: 200,
                        isResizable: true,
                        onRender: (propertyType) => {
                            return (
                                <Link      
                                    uriComponents={[
                                        "Admin", "Models", adapterId, modelId, "objectTypes", 
                                        objectType, "propertyTypes", propertyType, hash
                                    ]}                             
                                    text={propertyType}
                                />
                            );
                        }
                    },
                    {
                        key: "Description",
                        name: "Description",
                        minWidth: 200,
                        maxWidth: 400,
                        isResizable: true,
                        isMultiline: true,
                        onRender: (propertyType) => {                            
                            const propertyTypeObject = propertyTypes.getPropertyValue(propertyType);
                            return (
                                <Typography 
                                    size="2"
                                    text={propertyTypeObject ? propertyTypeObject.getPropertyValue("description") : ""}
                                />
                            );
                        }
                    }
                ]}        
            />
            <ModelPropertyTypeChoice 
                {...props}
                open={showNewPropertyType}
                onDismiss={() => setShowNewPropertyType(false)}
                onNext={(createOption) => {
                    setShowNewPropertyType(false);
                    setCreateOption(createOption);

                    if (createOption === "manual")
                        setShowNewManualPropertyType(true);
                    else 
                        setShowCopyPropertyTypes(true);
                }}
            />
            <ModelPropertyTypeManualAdd 
                {...props}
                open={showNewManualPropertyType}
                onDismiss={() => setShowNewManualPropertyType(false)}
                onCreate={onCreatePropertyType}
            />
            <ModelPropertyTypeCopy 
                {...props}
                open={showCopyPropertyTypes}
                createOption={createOption}
                onDismiss={() => setShowCopyPropertyTypes(false)}
                onCopy={onCopyPropertyTypes}
            />
        </div>
    );
};

export default ModelPropertyTypesTable;
