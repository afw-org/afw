// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for String
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {useState} from "react";

import {
    Typography, 
    DataTypeDropdown, 
    ObjectTypeDropdown,
    useOperational,
    useAfwProperty,
} from "@afw/react";

/**
 * Handles unknown dataTypes.
 */
export const Undefined = (props) => {
    
    const property = useAfwProperty(props);    
    const {value, valueMeta = {}, onChanged} = props;

    const label = valueMeta.label ? valueMeta.label : (property ? property.getName() : undefined);    

    const {editable} = useOperational(props);
    const [dataType, setDataType] = useState(property ? property.getDataType() : undefined);
    const [dataTypeParameter, setDataTypeParameter] = useState(
        property ? (property.getDataTypeParameter() ? property.getDataTypeParameter().split(" ") : []) : undefined
    );

    const onSetDataType = (dataType) => {
        property.setDataType( dataType );

        if (dataType === "array") {
            property.setDataTypeParameter();
            setDataTypeParameter([ undefined ]);
        }

        else if (dataType === "object") {
            property.setDataTypeParameter();
            setDataTypeParameter([ undefined ]);
        }

        setDataType(dataType);
        onChanged();
    };

    const onSetDataTypeParameter = (parameter, index) => {
        let newDataTypeParameter = [ ...dataTypeParameter ];
        if (newDataTypeParameter.length < index)
            newDataTypeParameter.push( parameter );
        else
            newDataTypeParameter[index] = parameter;

        property.setDataTypeParameter( newDataTypeParameter.join(" ") );
        setDataTypeParameter( newDataTypeParameter );

        onChanged();
    };

    if (editable) {
        return (
            <div aria-label={label}>
                <DataTypeDropdown
                    id="UndefinedDataType"
                    key="dataType"
                    label={label + " Data Type"}
                    description={"Select a Data Type for " + label}
                    value={dataType}
                    onChanged={onSetDataType}
                />
                {
                    dataTypeParameter && dataTypeParameter.map( (parameter, index) => {
                        let lastDataType = dataType;
                        if (index > 0)
                            lastDataType = dataTypeParameter[ index - 1 ];

                        if (lastDataType === "object") {
                            return (
                                <ObjectTypeDropdown                                    
                                    key={index}
                                    adapterId={property.getEmbeddingObject().getAdapterId()}
                                    label="Object Type"
                                    description={"Select an Object Type for " + label}
                                    value={parameter}
                                    onChanged={(objectTypeId) => onSetDataTypeParameter(objectTypeId, index)}
                                    autoSelect={false}
                                />
                            );
                        }

                        else {
                            return (
                                <DataTypeDropdown
                                    key={index}
                                    label="Entry Data Type"
                                    description="Select a Data Type for this entry."
                                    value={parameter}
                                    onChanged={(dataType) => onSetDataTypeParameter(dataType, index)}
                                />
                            );
                        }
                    })
                }
            </div>
        );
    } else {
        return (
            <Typography
                style={{ whiteSpace: "pre-line" }}
                size="5"
                text={value || ""}
            />
        );
    }
};

export default Undefined;
