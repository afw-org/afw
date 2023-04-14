// See the 'COPYING' file in the project root for licensing information.
/* eslint-disable jsx-a11y/click-events-have-key-events */
/* eslint-disable jsx-a11y/no-static-element-interactions */
import {useState, useEffect} from "react";

import {ExpressionTupleGroup} from "./ExpressionTupleGroup";
import HybridBuilder from "../HybridBuilder";

import {useOperational, /*useQualifiers*/} from "@afw/react";

import {
    Button,
    EditableLabel,
    Table,
    Typography
} from "@afw/react";

const ExpressionTupleOperationExpression = (props) => {

    const [/*variables*/, setVariables] = useState({});
    const [selectedVariables, setSelectedVariables] = useState([]);
    const [variableRows, setVariableRows] = useState([]);

    //const qualifiers = useQualifiers();
    const editable = useOperational(props);

    const {description, label, tuple} = props;

    useEffect(() => {
        if (tuple) {
            let vars = tuple[2];

            let rows = [];
            if (vars) {
                for (let i = 0; i < vars.length; i += 2) {
                    rows.push({ key: i });
                }
            }

            setVariableRows(rows);
        }
    }, [tuple]);
    

    const onChanged = (tuple) => {
        if (tuple && tuple.length >= 2) {
            let variables = [];
            
            if (tuple[2]) {
                for (let i = 0; i < tuple[2].length; i+=2) {
                    variables.push(tuple[2][i]);                
                }
            }
            setVariables(variables);
        }

        props.onChanged(tuple);
    };    

    const onAddVariable = () => {
        let t = [...tuple];
        if (!t[2])
            t[2] = [];

        t[2].push(null);
        t[2].push(null);

        props.onChanged(t);
    };

    const onRemoveVariable = () => {
        
        let t = [...tuple];
        selectedVariables.forEach(selectedVariable => {
            for (let i = 0; t[2] && (i < t[2].length); i += 2) {
                if (i === selectedVariable.key)
                    t[2].splice(i, 2);
            }
        });               
        
        props.onChanged(t);
        setSelectedVariables([]);
    };

    const onRenderVariables = () => {
        const variables = tuple[2];

        return (
            <tr>
                <td colSpan="2">
                    <div style={{ display: "flex" }}>
                        <Typography 
                            color="textSecondary"
                            size="5"
                            text="Variable Declarations"
                        />
                        <Button
                            style={{ color: "#4caf50" }}
                            type="icon"
                            label="Add Variable"
                            icon="add"
                            color="primary"
                            onClick={onAddVariable}
                            tooltip="Add Variable"
                        />
                        <Button
                            style={{ color: selectedVariables.length > 0 ? "#f44336" : undefined }}
                            type="icon"
                            label="Remove Selected Variable"
                            icon="remove"
                            color="primary"
                            onClick={onRemoveVariable}
                            disabled={selectedVariables.length === 0}
                            tooltip="Remove Selected Variable"
                        />
                    </div>
                    <Table 
                        compact={true}
                        columns={[
                            { 
                                key: "Name", 
                                name: "Variable",
                                minWidth: 100,
                                maxWidth: 200,
                                isResizable: true,
                                onRender: (item) => 
                                    <div onClick={(e) => e.stopPropagation()}>
                                        <EditableLabel 
                                            placeholder="Variable name..."                        
                                            value={variables[item.key]}
                                            onChanged={(variableName) => {
                                                let t = [...tuple];
                                                t[2][item.key] = variableName;                                                
                                                props.onChanged(t);
                                            }}
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
                                            hybrid={variables[item.key+1]}
                                            label="Value"
                                            onChanged={(dataType) => { 
                                                let t = [...tuple];
                                                t[2][item.key+1] = dataType;                           
                                                props.onChanged(t);
                                            }}
                                        />
                                    </div>
                            }
                        ]}
                        rows={variableRows}
                        selectionMode={editable ? "single" : "none"}
                        onSelectionChanged={(selectedVariables) => setSelectedVariables(selectedVariables)}
                    />
                </td>
            </tr>
        );
    };

    const onRenderExpression = () => {

        return (
            <tr>
                <td style={{ display: "flex", whiteSpace: "nowrap" }}>
                    <Typography 
                        color="textSecondary" 
                        size="5" 
                        text="Expression Body"
                    />
                </td>
                <td style={{ width: "99%" }}>
                    <div style={{ width: "100%" }}>
                        <HybridBuilder 
                            hybrid={tuple[3]}
                            onChanged={(hybrid) => {
                                let t = [...tuple];
                                t[3] = hybrid;

                                props.onChanged(t);
                            }}
                        />
                    </div>
                </td>
            </tr>
        );
    };    
            
    return (
        <ExpressionTupleGroup 
            tuple={tuple}
            onChanged={onChanged}         
            groupHeaderText={label}
            groupHeaderDescription={description} 
            contains={
                <table style={{ width: "100%" }}>
                    <tbody>
                        { onRenderVariables() }
                        { onRenderExpression() }
                    </tbody>
                </table>
            }                                       
        />
    );
};

ExpressionTupleOperationExpression.propTypes = {
};

export default ExpressionTupleOperationExpression;
