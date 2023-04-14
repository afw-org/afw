// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";
import PropTypes from "prop-types";

import PropertyResponsive from "./PropertyResponsive";
import {DataTypeDropdown, ObjectTypeDropdown} from "../../propertyTypes";

import {
    Button,
    Dialog,
    TextField,
    useTheme,
} from "@afw/react";

import {AfwProperty} from "@afw/client";


/*
 * ObjectResponsivePropertyOther is a component that displays the property name 
 * and other metadata necessary to create a new property that doesn't have 
 * a known propertyType.
 */
export const ObjectResponsivePropertyOther = (props) => {

    const [dataType, setDataType] = useState();
    const [dataTypeParameter, setDataTypeParameter] = useState();
    const [name, setName] = useState();    
    const theme = useTheme();

    const {open, adaptorId, propertyType, onDismiss, onSave} = props;    
    
    useEffect(() => {
        if (open) {
            /* reset state values when re-opened */
            setName();
            setDataType(propertyType ? propertyType.dataType : undefined);
            setDataTypeParameter(propertyType ? propertyType.dataTypeParameter : undefined);
        }
    }, [open, propertyType]);

    const onSaveProperty = () => {
        onSave(name, dataType, dataTypeParameter);
    };

    
    
    /* 
        \fixme need to allow dataType/dataTypeParameter be recursive, as lists can create n-dimensions
        \fixme also, dataTypeParameter may be used for dataType's other than object/list (string mediaType, for example)
     */
    return (
        <Dialog 
            open={open}
            isBlocking={false}
            onDismiss={onDismiss}
            maxWidth="md"
            title="New Property"
            contains={
                <div>                
                    <TextField 
                        id="NewPropertyName"
                        // eslint-disable-next-line jsx-a11y/no-autofocus
                        autoFocus={true}
                        label="Property Name"
                        description="Select a name for the new property."
                        value={name}
                        onChanged={setName}
                        onEnter={(event) => {
                            event.stopPropagation();
                            event.preventDefault();
                            if (name) 
                                onSaveProperty();
                        }}
                    />                
                    <div style={{ height: theme.spacing(2) }} />
                    <DataTypeDropdown 
                        id="NewPropertyDataType"
                        label="Data Type"
                        description="Select a Data Type for the new property."
                        value={dataType}
                        onChanged={setDataType}
                        disabled={(propertyType && propertyType.dataType) ? true : false}
                    />
                    {
                        (dataType === "object") && (
                            <>
                                <div style={{ height: theme.spacing(2) }} />
                                <ObjectTypeDropdown 
                                    id="NewPropertyObjectTypeDropdown"
                                    adaptorId={adaptorId}
                                    label="Object Type"
                                    description="Select an Object Type to describe this object."                                
                                    value={dataTypeParameter}
                                    onChanged={setDataTypeParameter}
                                    disabled={(propertyType && propertyType.dataTypeParameter) ? true : false}
                                />
                            </>
                        )
                    }      
                    {
                        (dataType === "list") && (
                            <DataTypeDropdown 
                                id="NewPropertyListDataType"
                                label="Data Type"
                                description="Select a Data Type for List entries."
                                value={dataTypeParameter}
                                onChanged={setDataTypeParameter}
                                disabled={(propertyType && propertyType.dataTypeParameter) ? true : false}
                            />
                        )
                    }                   
                </div>
            }
            footer={
                <div style={{ display: "flex", width: "100%", padding: theme.spacing(1), textAlign: "right" }}>                            
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button 
                            label="Ok"
                            buttonType="submit"
                            color="primary"
                            variant="contained"
                            onClick={onSaveProperty}
                            disabled={!name}
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

ObjectResponsivePropertyOther.propTypes = {
    otherPropertiesType:                PropTypes.object,
    property:                           PropTypes.instanceOf(AfwProperty),
};



/**
 * ObjectResponsiveProperty is a component that prompts for property information 
 * such as property name and dataType and value and returns it to 
 * the caller.  It's intended to be used with ObjectResponsiveToolbar when a user
 * clicks on a new property to be added.
 * 
 * Some new properties are "otherProperties", which do not have a property name
 * and instead is supplied at runtime by the user modifying the object.  Furthermore,
 * some propertyType information may not specify dataType, objectType and
 * those need to be prompted by the user as well, and stored back into the property
 * value as metadata.
 * 
 * To make things even more complex, the value of the property can be anything from 
 * a single string to a list of list of object, so this component has to accommodate 
 * that.
 * 
 */
export const ObjectResponsiveProperty = (props) => {

    const {open, onDismiss, onSave, property} = props;
    const theme = useTheme();

    useEffect(() => {
        if (property)
            property.detach();
    }, [property]);

    if (!property)
        return null;

    const maxWidth = (property.getDataType() === "object") ? "xl" : "md";

    return (
        <Dialog 
            open={open}
            isBlocking={false}
            onDismiss={onDismiss}
            maxWidth={maxWidth}
            title="Add Property"
            contains={
                <div>                                
                    <PropertyResponsive 
                        // eslint-disable-next-line jsx-a11y/no-autofocus
                        autoFocus={true}
                        property={property}
                        headerOptions="all"
                    />                                              
                </div>
            }
            footer={
                <div style={{ display: "flex", width: "100%", padding: theme.spacing(1), textAlign: "right" }}>            
                    <div style={{ marginRight: theme.spacing(1) }}>                        
                        <Button 
                            label="Ok"
                            color="primary"
                            variant="contained"
                            buttonType="submit"
                            onClick={() => {
                                property.attach();
                                onSave();
                            }}
                            disabled={false}
                        />
                    </div>                
                    <div>
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

export default ObjectResponsiveProperty;
