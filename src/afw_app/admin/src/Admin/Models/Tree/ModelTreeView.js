// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";
import {useHistory} from "react-router";
import Container from "../../../common/Container";

import {
    Button,
    Divider,
    ObjectResponsive,
    PropertyResponsive,
    HybridBuilder,
    Link,
    Typography,
    QualifiersProvider,
    useOperational,
    isHybridScript,
    isHybridTuple
} from "@afw/react";


import {ModelPropertyTypeOverview} from "../Overview/ModelPropertyTypeOverview";
import {ModelPropertyTypeManualAdd} from "../Overview/ModelPropertyTypesTable";
import {ModelObjectTypeManualAdd} from "../Overview/ModelObjectTypesTable";

import {CodeEditor} from "../../../common/TabbedCodeEditor";
import {RemovableComponent} from "../../../common/RemovableComponent";
import {useTheme} from "../../../hooks";

const treeHash = "#tree";

const Pad = () => {
    const theme = useTheme();

    return (
        <div style={{ height: theme.spacing(2) }} />
    );
};

const PaddedDivider = () => {    

    return (
        <>
            <Pad />
            <Divider />
            <Pad />
        </>
    );
};

const commentWrapped = (s) => {
    const words = s.split(" ");    
    let ret = " *";
    let length = 2;

    for (const word of words) {
        if (length + word.length + 1 >= 80) {
            /* wrap to next line */
            ret += "\n * " + word;
            length = word.length + 3;
        } else {
            ret += " " + word;
            length += word.length + 1;   
        }
    }

    return ret;
};

/**
 * ObjectTypesMainContent
 * 
 * The right-hand side view of the model, when "Object Types" tree node is selected.
 * It will list the current Object Types that are currently listed in the model and
 * allow the user to add a new one, if they wish.
 */
const ObjectTypesMainContent = ({ model, propertyName, onReloadTree }) => {

    const [showAdd, setShowAdd] = useState();
    const history = useHistory();
    const {editable} = useOperational();

    const objectTypes = model.getPropertyValue(propertyName);
    const adaptorId = model.getAdaptorId();
    const modelId = model.getPropertyValue("modelId");

    return (
        <Container maxWidth="md">
            <Typography size="8" component="h2" text="Object Types" />
            <Pad />
            <Typography 
                size="3" 
                text="Object Types define the shape of objects that are presented by this Model.  
                    Each Object Type may have a unique set of Property Types and Action rules to declare 
                    how objects are retrieve and stored by the underlying adaptor.
                    Expand the tree and select an Object Type from the Menu, or create a new one." />
            <Pad />
            <ul>
                {
                    objectTypes && objectTypes.getProperties().map(o => 
                        <li key={o.getName()}>
                            <Link url={"/Admin/Models/" + adaptorId + "/" + modelId + "/objectTypes/" + o.getName() + treeHash} text={o.getName()} />
                        </li>
                    )
                }
            </ul>       
            <Pad />
            { editable &&
                <Button 
                    icon="add"
                    label="Object Type"
                    size="small"
                    tooltip="Click to Add a new Object Type"
                    onClick={() => setShowAdd(true)}  
                />
            }
            <ModelObjectTypeManualAdd 
                open={showAdd}
                onDismiss={() => setShowAdd(false)}
                onCreate={(objectTypeId, description) => {
                    const newObjectTypeObject = objectTypes.newProperty(objectTypeId);
                    newObjectTypeObject.setValue({ description });

                    setShowAdd(false);
                    onReloadTree();

                    history.push("/Admin/Models/" + adaptorId + "/" + modelId + "/objectTypes/" + objectTypeId + treeHash);
                }}
            />
        </Container>
    );
};


/** 
 * PropertyTypesMainContent
 * 
 * The right-hand side view of the model, when "Property Types" tree node is selected.
 * It will list the current Property Types that are listed under this particular Object Type
 * and allow the user to add a new one, if the wish.
 */
const PropertyTypesMainContent = ({ model, objectType, propertyName, onReloadTree }) => {

    const [showAdd, setShowAdd] = useState();    
    const history = useHistory();
    const {editable} = useOperational();

    const propertyTypes = model.getPropertyValue(propertyName);
    const adaptorId = model.getAdaptorId();
    const modelId = model.getPropertyValue("modelId");

    return (
        <Container maxWidth="md">
            <Typography size="8" component="h2" text="Property Types" />
            <Pad />
            <Typography 
                size="3" 
                text="Property Types define which properties may exist on an Object Type, and how 
                    those properties should look and be constrained.
                    Expand the tree and select an Property Type from the Menu, or create a new one." />
            <Pad />
            <ul>
                {
                    propertyTypes && propertyTypes.getProperties().map(p => 
                        <li key={p.getName()}>
                            <Link url={"/Admin/Models/" + adaptorId + "/" + modelId + (objectType ? ("/objectTypes/" + objectType) : "") + "/propertyTypes/" + p.getName() + treeHash} text={p.getName()} />
                        </li>
                    )
                }
            </ul>       
            <Pad />
            { editable &&
                <Button 
                    icon="add"
                    label="Property Type"
                    tooltip="Click to Add a new Property Type"
                    size="small"
                    onClick={() => setShowAdd(true)}                    
                />
            }
            { showAdd &&
                <ModelPropertyTypeManualAdd
                    open={showAdd}
                    onDismiss={() => setShowAdd(false)}
                    onCreate={(propertyTypeName, description) => {                        
                        if (!model.getPropertyValue(propertyName))
                            model.setPropertyValue(propertyName, {});

                        const propertyTypes = model.getPropertyValue(propertyName);
                        const newPropertyTypeObject = propertyTypes.newProperty(propertyTypeName);
                        newPropertyTypeObject.setValue({ description });

                        setShowAdd(false);
                        onReloadTree();

                        if (objectType)
                            history.push("/Admin/Models/" + adaptorId + "/" + modelId + "/objectTypes/" + objectType + "/propertyTypes/" + propertyTypeName + treeHash);
                        else
                            history.push("/Admin/Models/" + adaptorId + "/" + modelId + "/propertyTypes/" + propertyTypeName + treeHash);
                    }}
                />
            }
        </Container>
    );
};


/**
 * ObjectTypeMainContent
 * 
 * The right-hand side view of the model, when a specific Object Type tree node is selected.
 * It will list the contents of this Object Type definition, including its Property Types,
 * Custom Variables, etc. and allow the user to make changes to anything here.
 */
const ObjectTypeMainContent = ({ property, objectType, model, onReloadTree }) => {

    const {editable} = useOperational();
    const history = useHistory();
    const [showAddProperty, setShowAddProperty] = useState();
    const [, setShowAddCustom] = useState();

    const {propertyTypes, description, custom} = property.getValue().getPropertyValues();

    const modelId = model.getPropertyValue("modelId");
    const adaptorId = model.getAdaptorId();    
    const properties = propertyTypes?.propertyTypes?.getProperties().sort((A, B) => 
        A.getName().toLowerCase().localeCompare(B.getName().toLowerCase())
    ) || [];

    return (
        <Container maxWidth="md">
            <Typography size="8" component="h2" text={objectType} />
            <Pad />
            { editable ?
                <PropertyResponsive property={property.getValue().getProperty("description")} /> :
                <Typography 
                    size="3" 
                    text={description} />
            }
            <Pad />
            <ObjectResponsive 
                object={property.getValue()}
                filterOptions={{
                    filterProperties: [
                        "mappedObjectType",
                        "collectionURIs",
                        "originURI",
                        "objectIdPropertyName",
                        "descriptionPropertyName",
                        "tags",
                    ],
                    filterValuedProperties: (editable ? false : true)
                }}
            />
            <PaddedDivider />
            <Typography size="6" component="h3" text="Property Types" />
            <Pad />
            <Typography 
                size="3" 
                text={`Property Types declare how Adaptive Properties will be presented by the Model through an Object Type.  The following is a list of Property Types for ${objectType}.`} />
            <Pad />
            <ul>
                {
                    properties.map(p => 
                        <li key={p.getName()}>
                            <Link url={"/Admin/Models/" + adaptorId + "/" +  modelId + "/objectTypes/" + objectType + "/propertyTypes/" + p.getName() + treeHash} text={p.getName()} />
                        </li>
                    )
                }
            </ul>
            { editable &&
                <Button 
                    icon="add"
                    label="Property Type"
                    tooltip="Click to Add a new Property Type"
                    size="small"
                    onClick={() => setShowAddProperty(true)}                    
                />
            }
            { showAddProperty &&
                <ModelPropertyTypeManualAdd
                    open={showAddProperty}
                    onDismiss={() => setShowAddProperty(false)}
                    onCreate={(propertyName, description) => {
                        if (!propertyTypes) {
                            property.getValue().setPropertyValue("propertyTypes", {});

                            const newPropertyTypeObject = property.getValue().getPropertyValue("propertyTypes").newProperty(propertyName);
                            newPropertyTypeObject.setValue({ description });
                        } else {
                            const newPropertyTypeObject = propertyTypes.newProperty(propertyName);
                            newPropertyTypeObject.setValue({ description });
                        }                        

                        setShowAddProperty(false);
                        onReloadTree();

                        if (objectType)
                            history.push("/Admin/Models/" + adaptorId + "/" + modelId + "/objectTypes/" + objectType + "/propertyTypes/" + propertyName + treeHash);
                        else
                            history.push("/Admin/Models/" + adaptorId + "/" + modelId + "/propertyTypes/" + propertyName + treeHash);
                    }}
                />
            }
            <PaddedDivider />
            <Typography size="6" component="h3" text="Constraints" />            
            <Pad />
            <Typography 
                size="3" 
                text="Specify the constraints on how these Objects are accessed." />            
            <Pad />
            <ObjectResponsive 
                object={property.getValue()}
                filterOptions={{
                    filterProperties: [
                        "allowAdd",
                        "allowChange",
                        "allowDelete",
                        "allowEntity",
                    ],
                    filterValuedProperties: (editable ? false : true)
                }}
            />
            <PaddedDivider />
            <Typography size="6" component="h3" text="Custom Variables" />
            <Pad />            
            <Typography 
                size="3" 
                text="You can declare Custom Variables here, which may be used by expressions throughout
                    this Object Type context.  These variables may be as simple as a string constant, or much 
                    more complex, such as an expression, lambda function or adaptive script." />
            <Pad />
            <ul>
                {
                    custom?.getProperties().map(c => 
                        <li key={c.getName()}>
                            <Link url={"/Admin/Models/" + adaptorId + "/" +  modelId + "/objectTypes/" + objectType + "/custom/" + c.getName() + treeHash} text={c.getName()} />
                        </li>
                    )
                }
            </ul>
            { editable &&
                <Button 
                    icon="add"
                    label="Custom Variable"
                    tooltip="Click to Add a new Custom Variable"
                    size="small"
                    onClick={() => setShowAddCustom(true)}                    
                />
            }
        </Container>
    );
};


/**
 * PropertyTypeMainContent
 * 
 * The right-hand side view of the model, when a specific Property Type is selected. 
 * This displays the Property Type editor, but excludes the computed properties, which
 * can instead be edited via the Tree.
 */
const PropertyTypeMainContent = ({ property, propertyType }) => {

    return (
        <Container maxWidth="md">
            <ModelPropertyTypeOverview 
                propertyType={propertyType}
                propertyTypeObject={property.getValue()}
                filterExcludeProperties={[
                    "onGetInitialValue",
                    "method",
                    "onGetProperty",
                    "onSetProperty"
                ]}
            />
        </Container>
    );
};


/**
 * OnFunctionsMainContent
 * 
 * The right-hand side view of the model, when "onFunctions" is selected.
 * It explains what each action is, depending on whether they are in an
 * Object Type Actions or Property Type Actions.
 */
const OnFunctionsMainContent = ({ objectType, propertyType }) => {

    const theme = useTheme();
    
    if (propertyType) {
        return (
            <Container maxWidth="md">
                <Typography size="8" component="h2" text={propertyType + " On Functions"} />
                <Pad />
                <Typography 
                    size="3" 
                    text="When an Object or Property value is calculated by the Model Adaptor, you may override the 
                        default behavior by implementing Action handlers." />
                <Pad />
                <div style={{ marginLeft: theme.spacing(2) }}>                
                    <Typography component="h3" size="5" text="On Get Initial Value()" />
                    <Typography 
                        size="3" 
                        text="When an Adaptive Property is created by an adaptor, the value may be
                            automatically generated by this expression." />
                    <Pad />
                    <Typography component="h3" size="5" text="Set Property()" />
                    <Typography 
                        size="3" 
                        text="You may augment or override the default model behavior when new Adaptive Property 
                            of this Property Type is created through the adaptor interface." />
                    <Pad />
                    <Typography component="h3" size="5" text="Get Property()" />
                    <Typography 
                        size="3" 
                        text="You may augment or override the default model behavior when an Adaptive Property 
                        of this Property Type is retrieved through the adaptor's get_object() or retrieve_objects() interfaces." />       
                    <div style={{ height: theme.spacing(5) }} />                                                    
                </div>
                <Typography 
                    size="3" 
                    text="Expand the tree on the left and select an Action to edit any one of these." /> 
            </Container>
        );
    } 
    
    else {
        return (
            <Container maxWidth="md">
                <Typography size="8" component="h2" text={objectType + " On Functions"} />
                <Pad />
                <Typography 
                    size="3" 
                    text="When an Object or Property value is calculated by the Model Adaptor, you may override the 
                        default behavior by implementing Action handlers." />
                <Pad />
                <div style={{ marginLeft: theme.spacing(2) }}>                
                    <Typography component="h3" size="5" text="On Get Initial ObjectId()" />
                    <Typography 
                        size="3" 
                        text="When an Adaptive Object is created by an adaptor, the required objectId may be
                            automatically generated by this Object Type definition." />
                    <Pad />
                    <Typography component="h3" size="5" text="On Add Object()" />
                    <Typography 
                        size="3" 
                        text="You may augment or override the default model behavior when new Adaptive Objects 
                            of this Object Type are created through the adaptor's add_object() interface." />
                    <Pad />
                    <Typography component="h3" size="5" text="On Modify Object()" />
                    <Typography 
                        size="3" 
                        text="You may augment or override the default model behavior when Adaptive Objects 
                        of this Object Type are modified through the adaptor's modify_object() interface." />
                    <Pad />
                    <Typography component="h3" size="5" text="On Replace Object()" />
                    <Typography 
                        size="3" 
                        text="You may augment or override the default model behavior when Adaptive Objects 
                        of this Object Type are replaced through the adaptor's replace_object() interface." />
                    <Pad />
                    <Typography component="h3" size="5" text="On Delete Object()" />
                    <Typography 
                        size="3" 
                        text="You may augment or override the default model behavior when Adaptive Objects 
                        of this Object Type are deleted through the adaptor's delete_object() interface." />
                    <Pad />
                    <Typography component="h3" size="5" text="On Get Object()" />
                    <Typography 
                        size="3" 
                        text="You may augment or override the default model behavior when an Adaptive Object
                        of this Object Type is returned through the adaptor's get_object() interface." />
                    <Pad />
                    <Typography component="h3" size="5" text="On Retrieve Objects()" />
                    <Typography 
                        size="3" 
                        text="You may augment or override the default model behavior when Adaptive Objects 
                        of this Object Type are returned through the adaptor's retrieve_objects() interface." />
                    <div style={{ height: theme.spacing(5) }} />
                </div>
                <Typography 
                    size="3" 
                    text="Expand the tree on the left and select an Action to edit any one of these." /> 
            </Container>
        );
    }
};

/**
 * ActionMainContent
 * 
 * The right-hand side view of the model, when a specific "Action" is selected.
 */
const ActionMainContent = ({ model, propertyName }) => {
    
    const [tab, setTab] = useState();    
    const {editable} = useOperational();
    const theme = useTheme();

    useEffect(() => {
        if (model && propertyName) {
            const property = model.getProperty(propertyName);
            if (property) {
                const value = property.getValue();

                setTab({
                    source: value,
                    key: property.getName(),            
                    format: value ? isHybridScript(value) ? "script" : isHybridTuple(value) ? "tuple" : "template" : undefined,            
                    label: property.getName(),
                });
            }
        }
    }, [model, propertyName]);

    const property = model.getProperty(propertyName);
    if (!property)
        return null;
    
    const propertyType = property.getEmbeddingObject().getParent();
    const dataType = property.getDataType();
    const contextType = property.getContextType();

    let mainContent = null;
    if (tab && tab.format) {
        if (dataType === "hybrid" && tab.format === "tuple") {
            
            mainContent = 
                <div style={{ padding: theme.spacing(2) }}>
                    <QualifiersProvider contextTypes={[ contextType ]}>
                        <HybridBuilder 
                            style={{ height: "100%" }}
                            hybrid={tab.source} 
                            onChanged={hybrid => {                        
                                property.setValue(hybrid);

                                tab.source = hybrid ? hybrid : "";
                                setTab({...tab});
                            }}
                        />
                    </QualifiersProvider>
                </div>;
        }

        else
            mainContent =
                <RemovableComponent 
                    removeTitle="Remove onFunction"
                    removeText="Remove this onFunction?"
                    component={
                        <QualifiersProvider contextTypes={[ contextType ]}>
                            <CodeEditor 
                                tab={tab}
                                editable={editable}                        
                                onSaveSource={() => {
                                    /*! \fixme when a user requests to save the source from a Ctl-S inside the editor */                                    
                                }}
                                onSourceChanged={(newValue) => {    
                                    property.setValue(newValue);

                                    tab.source = newValue ? newValue : "";
                                    setTab({...tab});
                                }}
                            />
                        </QualifiersProvider>
                    }
                    onRemove={() => {
                        property.setValue();
                        tab.source = tab.format = undefined;

                        setTab({ ...tab });
                    }}
                />;
    } 
    
    else 
    {
        /* no value yet */
        mainContent = (
            <Container maxWidth="md">
                <Typography size="8" component="h2" text={propertyType.getName() + ": " + property.getName()} />
                <Pad />                
                <Typography size="3" text={property.getDescription()} />    
                <Pad />                            
                <Typography size="3" text="This Variable currently has no value defined.  Implement this Variable using an Adaptive Script." />
                <div style={{ height: theme.spacing(5) }} />
                { editable && 
                    <div style={{ display: "flex", padding: theme.spacing(2) }}>
                        <Button 
                            style={{ marginRight: theme.spacing(1) }}
                            icon="add"
                            label="New Script"
                            size="small"
                            variant="outlined"
                            onClick={() => {
                                tab.format = "script";
                                tab.source = "#! afw\n";

                                tab.source += "\n/**\n";
                                tab.source += " * " + property.getName();

                                if (property.getSkeleton()) {
                                    tab.source = property.getSkeleton();
                                } else if (property.getDescription()) {     
                                    tab.source += "\n * \n";         
                                    tab.source += commentWrapped(property.getDescription());
                                    tab.source += "\n */\n\n";
                                }                                

                                setTab({...tab});
                            }}
                        />                        
                    </div>
                }
            </Container>
        );
    }
    
    return mainContent;        
};


/**
 * CustomVariablesMainContent 
 *
 * The right-hand side view of the model, when "Custom Variables" is selected.  
 * It will display a list of the current Custom Variables.  This may be viewed
 * from the main model section, and Object Type section, or a Property Type
 * section, so we account for all three.
 */
const CustomVariablesMainContent = ({ model, objectType, propertyType, propertyName, onReloadTree }) => {

    const {editable} = useOperational();
    const [showAddCustom, setShowAddCustom] = useState();
    const [variable, setVariable] = useState();
    const theme = useTheme();
    
    const adaptorId = model.getAdaptorId();
    const modelId = model.getPropertyValue("modelId");
    const custom = model.getPropertyValue(propertyName);

    return (
        <Container maxWidth="md">
            <Typography component="h2" size="8" text="Custom Variables" />
            <Pad />
            <Typography 
                size="3" 
                text="Custom Variables allow you to create user-defined Variables, which may be used 
                    throughout this part of the model by adaptive scripts and expressions.
                    Expand the tree and select a Custom Variable from the Menu, or create a new one." />
            <Pad />
            <ul>
                {
                    custom ? custom.getProperties().map(c => {           
                        if (propertyType)
                            return (
                                <li key={c.getName()}>
                                    <Link 
                                        url={"/Admin/Models/" + adaptorId + "/" +  modelId + "/objectTypes/" + objectType + "/propertyTypes/" + propertyType + "/custom/" + c.getName() + treeHash} 
                                        text={c.getName()} 
                                    />
                                </li>
                            );

                        else if (objectType)
                            return (
                                <li key={c.getName()}>
                                    <Link 
                                        url={"/Admin/Models/" + adaptorId + "/" +  modelId + "/objectTypes/" + objectType + "/custom/" + c.getName() + treeHash} 
                                        text={c.getName()} 
                                    />
                                </li>
                            );
                        
                        else
                            return (
                                <li key={c.getName()}>
                                    <Link 
                                        url={"/Admin/Models/" + adaptorId + "/" +  modelId + "/custom/" + c.getName() + treeHash} 
                                        text={c.getName()} 
                                    />
                                </li>
                            );

                    }) : <Typography component="span" text="(No Custom Variables Defined)" />
                }
            </ul>
            <div style={{ height: theme.spacing(5) }} />
            { editable &&
                <Button 
                    icon="add"
                    label="Custom Variable"
                    size="small"
                    onClick={() => {
                        setVariable( custom.newProperty() );
                        setShowAddCustom(true);
                    }}
                />
            }
            {
                (showAddCustom && variable) && 
                    <PropertyResponsive.Add 
                        open={showAddCustom}
                        title="New Custom Variable"
                        label="Variable Name"
                        description="Enter the name for your new Custom Variable."
                        object={custom}
                        onDismiss={() => {
                            setShowAddCustom();
                        }}
                        onSave={() => {
                            setShowAddCustom();
                            onReloadTree();
                        }}
                    />
            }
        </Container>
    );
};


/**
 * DefaultMainContent
 * 
 * The right-hand side view of the model, when the top-level model node is selected.
 */
const DefaultMainContent = ({ model }) => {

    const theme = useTheme();
    const {editable} = useOperational();
        
    const adaptorId = model.getAdaptorId();
    const {modelId, description, custom, propertyTypes, objectTypes} = model.getPropertyValues();

    const objectTypeProperties = objectTypes?.getProperties().sort((A, B) => 
        A.getName().toLowerCase().localeCompare(B.getName().toLowerCase())
    ) || [];

    return (
        <Container maxWidth="md">
            <Typography size="8" component="h2" text={modelId} />
            <Pad />

            { editable ?
                <PropertyResponsive property={model.getProperty("description")} /> :
                <Typography 
                    size="5" 
                    text={description} />
            }
            <div style={{ height: theme.spacing(3) }} />
            <Typography component="h3" size="6" text="Custom Variables" />
            <Typography 
                size="3" 
                text="You can declare Custom Variables here, which may be used by expressions throughout
                    this model.  These variables may be something simple, like a string constant, or much 
                    more complex, such as an expression, lambda function or adaptive script." />
            <Pad />            
            {
                custom ? 
                    <ul>
                        {
                            custom.getProperties().map(c => 
                                <li key={c.getName()}>
                                    <Link url={"/Admin/Models/" + adaptorId + "/" +  modelId + "/custom/" + c.getName() + treeHash} text={c.getName()} />
                                </li>
                            )
                        }
                    </ul> : <Typography component="span" text="(No Custom Variables Defined)" />
            }            
            <Pad />
            <Typography component="h3" size="6" text="Object Types" />
            <Typography 
                size="3" 
                text="An Adaptive Model is partitioned into Object Types.  Object Types will declare how an 
                    Adaptive Object will look when presented by this model.  Object Types may have their
                    own Custom Variables and use Property Types to further define how Adaptive Properties will
                    be presented by this Model." />
            <Pad />
            <ul>
                {
                    objectTypeProperties.map(o => 
                        <li key={o.getName()}>
                            <Link url={"/Admin/Models/" + adaptorId + "/" +  modelId + "/objectTypes/" + o.getName() + treeHash} text={o.getName()} />
                        </li>
                    )
                }
            </ul>
            <Pad />
            <Typography component="h3" size="6" text="Property Types" />
            <Typography 
                size="3" 
                text=""
            />
            <Pad />
            {
                propertyTypes ?
                    <ul>
                        {
                            propertyTypes.getProperties().map(p => 
                                <li key={p.getName()}>
                                    <Link url={"/Admin/Models/" + adaptorId + "/" +  modelId + "/propertyTypes/" + p.getName() + treeHash} text={p.getName()} />
                                </li>
                            )
                        }
                    </ul> : <Typography component="span" text="(No Property Types Defined)" />
            }
        </Container>
    );
};


/**
 * 
 */
export const ModelTreeView = (props) => {

    const { model, selectedNode, onReloadTree } = props;

    let mainContent = null;

    if (selectedNode) {

        let property;
        if (selectedNode.propertyName && model.hasProperty(selectedNode.propertyName))
            property = model.getProperty(selectedNode.propertyName);

        if (selectedNode.type === "objectTypes")         
            mainContent = <ObjectTypesMainContent {...props} {...selectedNode} property={property} onReloadTree={onReloadTree} />;        

        else if (selectedNode.type === "onFunctions")
            mainContent = <OnFunctionsMainContent {...props} {...selectedNode} property={property} onReloadTree={onReloadTree} />;               

        else if (selectedNode.type === "propertyTypes") 
            mainContent = <PropertyTypesMainContent {...props} {...selectedNode} onReloadTree={onReloadTree} />;        

        else if (selectedNode.type === "customs")
            mainContent = <CustomVariablesMainContent {...props} {...selectedNode} onReloadTree={onReloadTree} />;

        else if (selectedNode.type === "objectType")         
            mainContent = <ObjectTypeMainContent {...props} {...selectedNode} property={property} onReloadTree={onReloadTree} />;        

        else if (selectedNode.type === "propertyType")
            mainContent = <PropertyTypeMainContent {...props} {...selectedNode} property={property} onReloadTree={onReloadTree} />;
        
        else if (selectedNode.propertyName)       
            mainContent = <ActionMainContent {...selectedNode} {...props} onReloadTree={onReloadTree} />;

        else if (selectedNode.type === "model")
            mainContent = <DefaultMainContent {...props} {...selectedNode} onReloadTree={onReloadTree} />;
        
    }

    return mainContent;

};


export default ModelTreeView;
