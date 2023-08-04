// See the 'COPYING' file in the project root for licensing information.
/* eslint-disable jsx-a11y/click-events-have-key-events */
/* eslint-disable jsx-a11y/no-static-element-interactions */
import {ExpressionTupleGroup} from "./ExpressionTupleGroup";
import HybridBuilder from "../HybridBuilder";

import {
    Button,
    EditableLabel,
    Table,
    Typography
} from "@afw/react";

const ExpressionTupleOperationObject = (props) => {

    const {description, label, onChanged, tuple} = props;
 
    const onAddParameter = () => {
        let t = [...tuple];

        t.push(null);
        t.push(null);

        onChanged(t);
    };

    const onParameterNameChanged = (index, value) => {
        let t = [...tuple];

        t[index+2] = value;
        onChanged(t);
    };

    const onParameterValueChanged = (index, value) => {
        let t = [...tuple];

        t[index+2] = value;
        onChanged(t);
    };

    const onRemoveParameter = () => {
        /* \fixme */
    };

    const onRenderParameters = () => {
        let rows = [];

        /* The tuple should look like [annotation, "obj", "param1", "value1", ...] */
        let tupleParams = tuple.slice(2);       

        for (let i = 0; i < tupleParams.length; i += 2) {
            rows.push({ key: i });
        }

        return (
            <div style={{ width: "100%" }}>
                <div style={{ display: "flex", alignItems: "center" }}>
                    <Typography 
                        color="textSecondary" 
                        size="5" 
                        text="Object Properties"
                    />
                    <Button
                        style={{ color: "#4caf50" }}
                        type="icon"
                        label="Add Object Property"                        
                        icon="add"
                        color="primary"
                        onClick={onAddParameter}
                        tooltip="Add Object Property"
                    />
                    <Button
                        style={{ color: "#f44336" }}
                        type="icon"
                        label="Remove Selected Property"                        
                        icon="remove"
                        color="primary"
                        onClick={onRemoveParameter}
                        tooltip="Remove Selected Property"
                    />
                </div>
                <Table 
                    style={{ marginLeft: "8px" }}
                    compact={true}
                    columns={[
                        {
                            key: "Property",
                            name: "Property",
                            minWidth: 50,
                            maxWidth: 150,
                            isResizable: true,
                            onRender: (item) => 
                                <div onClick={(e) => e.stopPropagation()}>
                                    <EditableLabel 
                                        placeholder="Property Name"
                                        onChanged={name => onParameterNameChanged(item.key, name)}                        
                                        value={tupleParams[item.key]}    
                                    />
                                </div>
                        },
                        {
                            key: "Value",
                            name: "Value",
                            minWidth: 100,
                            maxWidth: 300,
                            isResizable: true,
                            onRender: (item) => 
                                <div onClick={(e) => e.stopPropagation()}>
                                    <HybridBuilder 
                                        onChanged={value => onParameterValueChanged(item.key+1, value)}
                                        hybrid={tupleParams[item.key+1]}
                                    />
                                </div>
                        }
                    ]}
                    rows={rows}
                />
            </div>
        );
    };
 
    return (
        <ExpressionTupleGroup 
            tuple={tuple}
            onChanged={onChanged}
            groupHeaderText={label}
            groupHeaderDescription={description}
            contains={onRenderParameters()}                                
        />
    );
};

ExpressionTupleOperationObject.propTypes = {
};

export default ExpressionTupleOperationObject;
