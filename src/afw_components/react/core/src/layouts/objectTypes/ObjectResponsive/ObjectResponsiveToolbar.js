// See the 'COPYING' file in the project root for licensing information.
import {useState, useMemo} from "react";

import {
    Button,
    Menu
} from "@afw/react";

import {useOperational, useAdaptiveLayout} from "@afw/react";
import {ObjectResponsiveProperty, ObjectResponsivePropertyOther} from "./ObjectResponsiveProperty";


/**
 * ObjectResponsiveToolbar displays a toolbar with "add/remove" actions to allow the
 * user to add or remove properties from an object.  The add button invokes a
 * dropdown menu to allow the user to quickly select with property to add.
 */
export const ObjectResponsiveToolbar = (props) => {

    const [addMenuTarget, setAddMenuTarget] = useState();    
    const [selectedProperty, setSelectedProperty] = useState();
    const [selectedPropertyType, setSelectedPropertyType] = useState();
    const [ObjectResponsivePropertyOpen, setObjectResponsivePropertyOpen] = useState(false);
    const [ObjectResponsivePropertyTypeOpen, setObjectResponsivePropertyTypeOpen] = useState(false);
    

    const {object, canDelete, onDelete, onAdd} = props;

    const {editable} = useOperational(props);
    const {data: { property }} = useAdaptiveLayout();

    /**
     * Invoked when user selects a property from the "add" dropdown menu.
     *
     * @param {AfwProperty} property
     */
    const onSelectProperty = (property) => {
        if (property.getDefaultValue())
            property.setValue( property.getDefaultValue() );

        /* for objects, create an empty object, {} */
        else if (property.getDataType() === "object")
            property.setValue({});

        // \fixme useReducer?
        setAddMenuTarget();
        setSelectedPropertyType();
        setSelectedProperty(property);
        setObjectResponsivePropertyOpen(true);
        setObjectResponsivePropertyTypeOpen(false);
    };

    /**
     * Invoked when the user selects "New Property" from the "add" dropdown menu.
     *
     * @param {object} otherPropertyType
     */
    const onSelectOtherProperty = (otherPropertyType) => {
        // \fixme useReducer?
        setSelectedPropertyType(otherPropertyType);
        setSelectedProperty();
        setAddMenuTarget();
        setObjectResponsivePropertyOpen(false);
        setObjectResponsivePropertyTypeOpen(true);
    };


    const [onlyOtherProperties, propertyMenuItems] = useMemo(() => {

        let propertyMenuItems = [];
        let onlyOtherProperties;

        if (object) {
            /* sort the properties */
            const properties = object.getProperties().sort((a, b) => {
                const A = a.getLabel() ? a.getLabel() : a.getName();
                const B = b.getLabel() ? b.getLabel() : b.getName();

                return A.toLowerCase().localeCompare(B.toLowerCase());
            });

            /* build the menu items.  \fixme there may be more to exclude by ObjectResponsive (if layout displays more than just "valued" for example) */
            properties.forEach(property => {
                /* only display undefined properties that aren't already handled */
                if (property.getValue() === undefined) {
                    propertyMenuItems.push({
                        key: property.getName(),
                        label: property.getLabel() ? property.getLabel() : property.getName(),
                        disabled: property.getAllowWrite() === false,
                        onClick: () => onSelectProperty(property)
                    });
                }
            });

            /* check for "otherProperties" */
            const otherPropertyType = object.getPropertyType();
            if (otherPropertyType !== undefined) {
                if (propertyMenuItems.length > 0)
                    propertyMenuItems.push({ key: "divider", type: "divider" });
                else
                    onlyOtherProperties = true;

                propertyMenuItems.push({
                    key: "other",
                    label: "New Property",
                    onClick: () => onSelectOtherProperty(otherPropertyType)
                });
            }            
        }

        return [onlyOtherProperties, propertyMenuItems];
    }, [object]);

    /**
     * When a user clicks the "Cancel" button while editing a new Property.
     */
    const onDismiss = () => {
        selectedProperty.discardChanges();

        setObjectResponsivePropertyOpen(false);
    };

    const onSaveOtherProperty = (name, dataType, dataTypeParameter) => {
        const property = object.newProperty(name);

        if (property.getDefaultValue())
            property.setValue( property.getDefaultValue() );
        else if (property.getDataType() === "object")
            property.setValue({});

        property.setDataType(dataType);
        property.setDataTypeParameter(dataTypeParameter);

        onAdd(property);
        setObjectResponsivePropertyTypeOpen(false);
    };

    const onAddProperty = () => {
        onAdd(selectedProperty);

        setObjectResponsivePropertyOpen(false);
    };

    /* When not editable, we don't want to allow the user to click Add/Remove properties */
    if (editable !== true)
        return <div />;

    const propertyLabel = property ? (property.getLabel() ?? property.getName()) : undefined;
    return (
        <div 
            style={{ display: "flex" }}
            role="button"
            tabIndex={-1}
            onClick={(event) => event.stopPropagation()}
            onKeyPress={(event) => event.stopPropagation()}
        >
            <Button                
                className="addPropertyButton"
                type="icon"
                icon="add"
                label={property ? ("Add New " + propertyLabel + " Property") : "Add New Property"}                
                tooltip="Add New Property"
                color="primary"
                disabled={propertyMenuItems.length === 0}
                onClick={event => {
                    if (onlyOtherProperties)
                        onSelectOtherProperty(object.getPropertyType());
                    else
                        setAddMenuTarget(event.target);
                }}
            />
            <Button                
                className="deletePropertyButton"
                type="icon"
                icon="remove"
                label={property ? ("Remove " + propertyLabel + " Property") : "Remove Property"}
                tooltip="Remove Property"
                color="primary"
                disabled={!canDelete}
                onClick={event => {
                    onDelete(event);
                }}
            />
            <Menu
                open={Boolean(addMenuTarget)}
                target={addMenuTarget}
                onClose={() => setAddMenuTarget()}
                color="primary"
                items={propertyMenuItems}
            />
            <ObjectResponsivePropertyOther
                open={ObjectResponsivePropertyTypeOpen}
                propertyType={selectedPropertyType}
                adaptorId={object ? object.getAdaptorId() : undefined}
                onDismiss={() => setObjectResponsivePropertyTypeOpen(false)}
                onSave={onSaveOtherProperty}
            />
            <ObjectResponsiveProperty
                open={ObjectResponsivePropertyOpen}
                property={selectedProperty}
                onDismiss={onDismiss}
                onSave={onAddProperty}
            />
        </div>
    );
};

export default ObjectResponsiveToolbar;
