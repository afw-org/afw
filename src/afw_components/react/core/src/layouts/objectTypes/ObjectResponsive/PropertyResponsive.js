// See the 'COPYING' file in the project root for licensing information.
import {cloneElement, useState, useEffect, useCallback} from "react";
import PropTypes from "prop-types";

import {
    Button,
    Dialog,
    Responsive,
    TextField,
    Tooltip,
    Typography,
    useOperational, 
    useEventId, 
    useAfwProperty, 
    useLayoutParameters,  
    useTheme,
    PropertyLayout,
} from "@afw/react";

const Breakpoints = {
    small:                  PropTypes.oneOf([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
    medium:                 PropTypes.oneOf([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
    large:                  PropTypes.oneOf([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
    xl:                     PropTypes.oneOf([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
    xxl:                    PropTypes.oneOf([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
    xxxl:                   PropTypes.oneOf([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
};

const propTypes = {
    embedded:               PropTypes.oneOfType([PropTypes.bool, PropTypes.object]),    
    breakpoints:            PropTypes.shape(Breakpoints),
    componentProps:         PropTypes.object,
    tooltip:                PropTypes.bool,    
    showLabels:             PropTypes.bool,
    showDescriptions:       PropTypes.bool,   
};

const defaultProps = {
    showLabels:             true,
    showDescriptions:       true,
    tooltip:                false,
};

/**
 * LabeledValue is a component that displays an AfwProperty in a 
 * read-only (non editable) way by laying the property label/name and value
 * out in a responsive column/column row.
 * 
 * @param {AfwProperty} property
 * @param {boolean} showLabels
 * @param {boolean} requireValue 
 * @param {*} valueContains 
 * 
 */
const LabeledValue = (props) => {

    const property = useAfwProperty(props);
    const {showLabels, readOnlyOrientation} = useLayoutParameters(props, propTypes, defaultProps);
    const theme = useTheme();
    const {requireValue, valueContains} = props;

    if (!property)
        return null;

    if (requireValue && (property.getValue() === undefined))
        return null;

    const label = property.getLabel() ? property.getLabel() : property.getName() ? property.getName() : "";
    const name = property.getName() ? property.getName() : "";

    let contains = (
        <Typography 
            size="5"
            color="textSecondary"
            component="span"
            text={showLabels === false ? name : label} 
        />
    );

    if (readOnlyOrientation === "vertical") {
        return (
            <div>
                { contains }
                <div style={{ marginLeft: theme.spacing(8) }}>
                    { cloneElement(valueContains, { "aria-label": label ? label : name })}
                </div>
            </div>
        );
    }

    /* 
     * \fixme using these breakpoints as a heuristic.  
     *
     * The label is generally shorter than the value, so on larger displays 
     * the label only consumes 25%-33%.  On smaller displays, the label
     * consumes 50%.
     */

    /* Return the property in two parts, which are declared responsively */
    return (
        <Responsive.Row
            spacing={1}
            columns={[
                {
                    style: { overflow: "auto" },
                    breakpoints: {
                        small: 6,
                        medium: 4,
                        large: 3
                    },
                    contains                        
                },
                {
                    breakpoints: {
                        small: 6,
                        medium: 8,
                        large: 9
                    },
                    contains: 
                        cloneElement(
                            valueContains, 
                            { "aria-label": label ? label : name }
                        )
                }
            ]} />   
    );
};

/**
 * PropertyResponsiveDelete
 * 
 * Removes a property, but confirms the action first.
 */
export const PropertyResponsiveDelete = ({ 
    object, onDismiss, onDelete, propertyName, 
    subText = "Are you sure you want to delete this property?", 
    title = "Delete Property" 
}) => {

    const theme = useTheme();

    return (
        <Dialog 
            open={true}
            isBlocking={true}
            title={title}
            maxWidth="sm"
            onDismiss={onDismiss}
            subText={subText}
            footer={
                <div style={{ display: "flex", textAlign: "right" }}>
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button 
                            label="Yes"
                            color="primary"
                            variant="contained"
                            onClick={() => {
                                object.removeProperty(propertyName);
                                onDelete();
                            }}
                        />
                    </div>
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button 
                            label="No"
                            onClick={onDismiss}
                        />
                    </div>                            
                </div>
            }
        />
    );
};

/**
 * PropertyResponsiveRename
 * 
 * Renames a property in an object.
 */
export const PropertyResponsiveRename = ({ 
    description = "Choose a new name for this Property.", 
    label = "Property Name", 
    open = true, onDismiss, onRename, property, 
    title = "Rename Property"
}) => {
    
    const [newName, setNewName] = useState();
    const [savable, setSavable] = useState();
    const theme = useTheme();

    const object = property ? property.getEmbeddingObject() : undefined;

    useEffect(() => {
        if (property)
            setNewName(property.getName());
    }, [property]);

    const onPropertyValidate = (newName) => {

        /* if the new property name is already taken, return false */
        if (object.hasProperty(newName) && property?.getName() !== newName)
            return false;        

        /* all good! */
        return true;
    };

    const onPropertyNameChanged = (newName) => {
        let savable = true;
        
        if (!newName)
            savable = false;

        else if (object.hasProperty(newName))
            savable = false;

        setNewName(newName);
        setSavable(savable);
    };

    const onSave = () => {
        if (property)
            property.setName(newName);

        onRename();
    };


    return (
        <Dialog 
            open={open && Boolean(property)}
            isBlocking={false}
            title={title}
            maxWidth="sm"
            contains={
                <>
                    <TextField 
                        // eslint-disable-next-line jsx-a11y/no-autofocus
                        autoFocus={true}
                        label={label}       
                        required={true}                         
                        description={description}
                        value={newName}
                        onValidate={onPropertyValidate}
                        onChanged={onPropertyNameChanged}
                    /> 
                    <div style={{ height: theme.spacing(6) }} />
                </>
            }
            footer={
                <div style={{ display: "flex", textAlign: "right" }}>
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button 
                            label="Save"
                            color="primary"
                            variant="contained"
                            onClick={onSave}
                            disabled={!savable}
                        />
                    </div>
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button 
                            label="Cancel"
                            onClick={() => {
                                onDismiss();
                            }}
                        />
                    </div>                            
                </div>
            }
        />
    );
};


/**
 * PropertyResponsiveAdd
 * 
 * This component takes an object and prompts for a new Property to be
 * added by inputting the new name.  It handles validating that the property
 * is not already taken and has the appropriate syntax.  If the user clicks 
 * Save, the property is added to the object, otherwise, it's discarded.
 */
export const PropertyResponsiveAdd = (props) => {

    const [newName, setNewName] = useState();
    const [savable, setSavable] = useState();
    const theme = useTheme();

    const {open, object, title, label, description} = props;

    const onDismiss = () => {
        if (props.onDismiss)
            props.onDismiss();

        setSavable();
    };

    const onPropertyValidate = (newName) => {

        /* if the new property name is already taken, return false */
        if (object && object.hasProperty(newName))
            return false;        

        /* all good! */
        return true;
    };

    const onPropertyNameChanged = (newName) => {
        let savable = true;
        
        if (!newName)
            savable = false;

        else if (object && object.hasProperty(newName))
            savable = false;

        setNewName(newName);
        setSavable(savable);
    };

    const onSave = () => {
        /* Since the property didn't have a name to start with, it doesn't exist internally yet */
        const property = object ? object.newProperty(newName) : undefined;
        
        props.onSave(property, newName);

        setSavable();
    };


    return (
        <Dialog 
            open={Boolean(open)}
            isBlocking={true}
            onDismiss={onDismiss}
            title={title}
            maxWidth="md"
            contains={
                <>
                    <TextField 
                        // eslint-disable-next-line jsx-a11y/no-autofocus
                        autoFocus={true}
                        label={label}       
                        required={true}                         
                        description={description}
                        value={newName}
                        onValidate={onPropertyValidate}
                        onChanged={onPropertyNameChanged}
                    /> 
                    <div style={{ height: theme.spacing(6) }} />
                </>
            }
            footer={
                <div style={{ display: "flex", textAlign: "right" }}>
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button 
                            label="Save"
                            color="primary"
                            variant="contained"
                            onClick={onSave}
                            disabled={!savable}
                        />
                    </div>
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button 
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
 * PropertyResponsive
 *
 * The PropertyResponsive component is a layout that maps an AfwProperty instance to
 * the appropriate component, based on layout information and/or property metadata.  It
 * also understands some of the underlying component props and maps them from the property 
 * as appropriate.
 * 
 * When the property is not editable, as directed by props, then it will often display the
 * property in a non-editable LabeledValue component.
 */
export const PropertyResponsive = (props) => {

    const {editable, add} = useOperational(props);    
    const property = useAfwProperty(props);    
    const {breakpoints, showDescriptions, tooltip, showLabels} = useLayoutParameters(props, propTypes, defaultProps);    
    const theme = useTheme();

    const dataTestId = props["data-testid"];

    /* track changes to the property and re-render */
    useEventId({ property, eventId: "onDiscardChanges" }); 
    useEventId({ property, eventId: "onChanged" });

    /* Capture changes by the component and save them in the property value */
    const onChanged = useCallback((newValue) => {        
        const allowWrite = property.getAllowWrite();
        const required = property.getRequired();

        if (editable !== false && (allowWrite !== false || (required && add))) {            
            property.setValue(newValue);
        }
    }, [property, editable, add]);

    const onValidate = useCallback(() => {        
        return property.validate();
    }, [property]);


    /* Nothing we can do here. */
    if (!property)
        return null;
    
    /* merge the two, and let property meta override */           
    let valueMeta = {...props, brief: undefined, label: undefined, description: undefined, ...property.getMeta()};

    valueMeta.name = property.getName();
    valueMeta.value = (property.getValue() !== undefined) ? property.getValue() : property.getDefaultValue();    
    valueMeta.description = (showDescriptions === false) ? undefined : (valueMeta.brief ? valueMeta.brief : valueMeta.description);
    valueMeta.label = showLabels === false ? undefined : (valueMeta.label ? valueMeta.label : valueMeta.name);  
    
    let id = "PropertyResponsive";
    if (property.getPath())
        id += "-" + property.getPath().split("/").join("-");    
    
    let component;

    /* Use a registered property layout */  
    component = (
        <PropertyLayout 
            {...valueMeta}
            property={property}         
            value={property.getValue()}
            valueMeta={valueMeta}                
            data-component-type="PropertyResponsive" 
            data-testid={dataTestId}
            id={props["id"] ? props["id"] + id : id}     
            onChanged={onChanged}
            onValidate={onValidate}
        />
    );    

    if (!editable && property.getDataType() !== "object") {
        component = <LabeledValue {...props} valueContains={component} />;
    }

    /* if a tooltip is required, then wrap it inside the Tooltip component */        
    if (tooltip && (property.getDataType() !== "object")) {
        let tooltipContent = property.getName() + ": " + property.getDataType();
        if (property.getDataTypeParameter())
            tooltipContent += " " + property.getDataTypeParameter();

        const brief = property.getBrief();
        const description = property.getDescription();
        
        component = (
            <Tooltip
                contains={
                    <>
                        <Typography size="2" text={tooltipContent} />
                        <div style={{ height: theme.spacing(3) }} />
                        <Typography size="1" text={brief} />
                        <div style={{ height: theme.spacing(2) }} />
                        <Typography size="1" text={description} />
                    </>
                }
                delay={2000}
                target={ <div>{ component }</div> }
            />
        );            
    }

    if (breakpoints) {
        return <Responsive.Col breakpoints={breakpoints} contains={component} />;
    } else {
        return (component);
    }   
};

PropertyResponsive.displayName = "PropertyResponsive";
PropertyResponsive.Add = PropertyResponsiveAdd;
PropertyResponsive.Delete = PropertyResponsiveDelete;
PropertyResponsive.Rename = PropertyResponsiveRename;

export default PropertyResponsive;
