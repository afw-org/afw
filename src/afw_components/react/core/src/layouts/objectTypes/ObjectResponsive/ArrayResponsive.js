// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for List
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {useState, useEffect} from "react";
import PropTypes from "prop-types";

import {
    Button,
    Dialog,
    Link,
    List,
    ListEditor,
    Typography,
    useRouteBasePath, 
    useOperational, 
    useAfwProperty, 
    useEventId,
    useTheme,
} from "@afw/react";

import ValueLayout from "../../ValueLayout";
import {AdaptiveLayoutMergeProviders} from "../../AdaptiveLayout";
import {AfwProperty} from "@afw/client";


export const ArrayResponsiveReadonly = (props) => {

    const property = useAfwProperty(props);    
    const routeBasePath = useRouteBasePath();

    const { items, "aria-label": ariaLabel } = props;

    return (
        <List
            aria-label={ariaLabel}
            style={{ width: "100%", maxHeight: "30vh", overflow: "auto" }}
            contains={
                items.map((item, index) => {

                    /*! \fixme need to use the metadata handler to return the appropriate string value 
                        return (
                            <ValueLayout                                 
                                id={id + ".ArrayResponsive.Value"}                  
                                value={property.getValue()}        
                                nestingDepth={0}                                 
                                headerOptions="all"
                                valueMeta={{ ...property.getMeta(), ...editValue.getMeta() }}
                            />
                        );
                     */

                    if (property.getDataTypeParameter() === "objectPath") {
                        return (
                            <Link
                                key={index}
                                url={routeBasePath + item.text} 
                                text={item.text}
                            />
                        );
                    } else {
                        return (
                            <Typography 
                                style={{ overflow: "hidden", textOverflow: "ellipsis" }}
                                key={index}
                                size="5"
                                component="span"
                                text={item.text}
                            />
                        );
                    }
                })
            }
        />
    );
};

/**
 * The ArrayResponsive is a component that wraps ListEditor to provide options
 * derived from an AfwProperty of dataType list.  It handles the modals to 
 * prompt for new values and manages the options when new list entries are added
 * and removed.
 */
export const ArrayResponsive = (props) => {

    const [showModal, setShowModal] = useState(false);
    const [modalTitle, setModalTitle] = useState("");
    const [addItem, setAddItem] = useState(false);
    const [items, setItems] = useState([]);
    const [savable, setSavable] = useState();
    const [editValue, setEditValue] = useState();    
    const theme = useTheme();    

    const property = useAfwProperty(props);    
    const {editable} = useOperational(props);

    /* track when the property gets values added */
    const onChanged = useEventId({ property, eventId: "onChanged" });    

    const {id, "aria-label": ariaLabel} = props;        
    const label = property?.getLabel() || property?.getName();
    const description = property?.getBrief() || property?.getDescription();
    
    useEffect(() => {
        const items = [];

        if (property && property.getValue() !== undefined) {           
            const label = property.getLabel() ? property.getLabel() : property.getName();            
            property.getValue().forEach((value, index) => {
                let text;

                // \fixme the heuristic here may be better off requesting the dataType metadata handlers
                //        since they know how to handle the specific dataType.  For example base64Binary
                //        or hexBinary.
                if (value.getValue() !== undefined) {
                    if (value.getDataType() === "object") {
                        const descriptionPropertyName = value.getValue().getDescriptionPropertyName();
                        if (descriptionPropertyName)
                            text = value.getValue().getPropertyValue(descriptionPropertyName);
                    } else if (value.getValue() !== undefined) {
                        // \fixme look at CSS text-overflow to make sure they're not too long.
                        text = String(value.getValue());
                    }

                    if (text === undefined)
                        text = label + " [" + String(index) + "]";

                    items.push({ text, value, index });
                }
            });
        }

        setItems(items);
    }, [property, onChanged]);

    const onAddItem = () => {        
        if (property) {
            /* if this is the first value for this property, then we need to allocate a new AfwArray */
            if (!property.getValue()) {
                property.setValue([]);
            }

            /* create a new value on this list */
            const editValue = property.getValue().newValue();
            editValue.addEventListener("onSavable", setSavable);        

            setEditValue(editValue);
            setModalTitle("Add New Value");
        }
        
        setShowModal(true);
        setAddItem(true);
    };

    const onEditItem = (item) => {
        const editValue = item.value;

        editValue.detach();
        editValue.addEventListener("onSavable", setSavable);

        setEditValue(editValue);
        setModalTitle("Edit Value");
        setShowModal(true);
        setAddItem(false);
    };

    const onDeleteItems = (deleteItems) => {

        let newItems = items.filter(item => {
            for (let i = 0; i < deleteItems.length; i++) {
                let deleteItem = deleteItems[i];
                if (item.index === deleteItem.index)
                    return false;
            }

            return true;
        });

        /* actually remove the values from the property, too */
        deleteItems.forEach(item => item.value.remove());

        setItems(newItems);
    };

    const onSave = () => {
        editValue.attach();

        const items = [];

        if (property && property.getValue()) {
            const label = property.getLabel() ? property.getLabel() : property.getName();
            property.getValue().forEach((value, index) => {
                let text;

                if (value.getDataType() === "object") {
                    const descriptionPropertyName = value.getValue().getDescriptionPropertyName();
                    if (descriptionPropertyName)
                        text = value.getValue().getPropertyValue(descriptionPropertyName);
                } else if (value.getValue() !== undefined)
                    text = String(value.getValue());

                if (!text)
                    text = label + " [" + String(index) + "]";

                items.push({ text, value, index });
            });
        }

        setItems(items);
        setShowModal(false);
    };

    const onCancel = () => {
        if (addItem) 
            editValue.remove();
        else
            editValue.discardChanges();        

        setShowModal(false);
    };

    if (editable !== true) {
        return (
            <ArrayResponsiveReadonly 
                {...props}
                items={items}
            />
        );
    }

    return (
        <>
            <ListEditor 
                aria-label={ariaLabel}
                label={label}
                description={description}
                items={items}
                editable={editable}
                onAddItem={onAddItem}
                onEditItem={onEditItem}
                onDeleteItems={onDeleteItems}
            />
            <Dialog 
                open={showModal}
                title={modalTitle}
                isBlocking={false}
                showClose={true}
                maxWidth={(editValue && editValue.getDataType() === "object") ? "md" : "sm"}
                onDismiss={() => setShowModal(false)}
                contains={
                    <div>    
                        {
                            editValue && 
                                <AdaptiveLayoutMergeProviders layoutParameters={{ nestingDepth: 0, headerOptions: "all" }}>
                                    <ValueLayout 
                                        // eslint-disable-next-line jsx-a11y/no-autofocus
                                        autoFocus={true}                      
                                        id={id + ".ArrayResponsive.ValueLayout"}                  
                                        value={editValue}  
                                        valueMeta={{ ...property.getMeta(), ...editValue.getMeta() }}
                                    />
                                </AdaptiveLayoutMergeProviders>
                        }                           
                    </div>
                }
                footer={
                    <div style={{ display: "flex", padding: theme.spacing(2), textAlign: "right" }}>
                        <Button 
                            color="primary"
                            variant="contained"
                            style={{ marginRight: theme.spacing(1) }}
                            label="Ok"
                            disabled={!savable}
                            onClick={onSave}                                    
                        />
                        <Button 
                            label="Cancel"
                            onClick={onCancel}
                        />
                    </div>
                }
            />
        </>
    );    
};


ArrayResponsive.propTypes = {
    property:           PropTypes.instanceOf(AfwProperty),
};

ArrayResponsive.displayName = "ArrayResponsive";

export default ArrayResponsive;
