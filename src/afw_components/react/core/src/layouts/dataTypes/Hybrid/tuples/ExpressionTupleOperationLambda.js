// See the 'COPYING' file in the project root for licensing information.
/* eslint-disable jsx-a11y/click-events-have-key-events */
/* eslint-disable jsx-a11y/no-static-element-interactions */
import {useState} from "react";
import PropTypes from "prop-types";

import {DataTypeDropdown} from "../../../propertyTypes/DataTypeDropdown";
import ExpressionTuple from "./ExpressionTuple";
import {ExpressionTupleGroup} from "./ExpressionTupleGroup";

import {
    Button,
    EditableLabel,
    Table,
    Typography
} from "@afw/react";
import {useOperational, /*useQualifiers*/} from "@afw/react";

/**
 * ExpressionTupleOperationLambda
 * 
 * This component renders and allows the user to define their own Lambda functions.
 * It provides the UI to declare a return dataType, parameter definitions and the 
 * Lambda expression body.
 */
const ExpressionTupleOperationLambda = (props) => {

    const [/*variables*/, setVariables] = useState([]);
    const [/*selectedParameters*/, setSelectedParameters] = useState([]);

    const {description, label, tuple} = props;
    //const qualifiers = useQualifiers();
    const {editable} = useOperational();

    const onChanged = (tuple) => {
        if (tuple && tuple.length >= 4) {
            let variables = [];
            for (let i = 0; i < tuple[3].length; i+=2) {
                variables.push(tuple[3][i+1]);
            }

            setVariables(variables);
        }

        props.onChanged(tuple);
    };

    const onRenderReturnDataType = () => {
        return (
            <tr>
                <td style={{ whiteSpace: "nowrap" }}>
                    <div style={{ display: "inline-block", paddingRight: "20px", verticalAlign: "middle" }}>
                        <Typography 
                            color="textSecondary" 
                            size="5" 
                            text="Return Type"
                        />
                    </div>
                </td>
                <td style={{ width: "99%" }}>
                    <div style={{ display: "inline-block", minWidth: "200px" }}>
                        { editable ?
                            <DataTypeDropdown 
                                id="ReturnDataType"
                                value={tuple[2]}
                                onChanged={(dataType) => { 
                                    let t = [...tuple];
                                    t[2] = dataType;                           
                                    props.onChanged(t);
                                }}
                            /> :
                            <Typography text={tuple[2]} />
                        }
                    </div>
                </td>
            </tr>
        );
    };

    const onRemoveParameter = () => {
        /*! \fixme use selectedParameters and slice the tuple */
        let t = [...tuple];

        props.onChanged(t);
    };

    const onAddParameter = () => {
        let t = [...tuple];

        if (!t[3])
            t[3] = [];

        t[3].push(null);
        t[3].push(null);

        props.onChanged(t);
    };

    const onRenderParameters = () => {
        const parameters = tuple[3];

        let rows = [];
        if (parameters) {
            for (let i = 0; i < parameters.length; i+=2) {
                rows.push({ key: i });
            }
        }

        return (
            <tr>
                <td colSpan="2">
                    <div style={{ display: "flex", alignItems: "center" }}>
                        <Typography 
                            color="textSecondary" 
                            size="5" 
                            text="Parameters"
                        />
                        { editable &&
                            <>
                                <Button
                                    style={{ color: "#4caf50" }}
                                    type="icon"
                                    label="Add Parameter"
                                    icon="add"
                                    color="primary"
                                    onClick={onAddParameter}
                                    tooltip="Add Parameter"
                                />
                                <Button
                                    style={{ color: "#f44336" }}
                                    type="icon"   
                                    label="Remove Selected Parameter"                         
                                    icon="remove"
                                    color="primary"
                                    onClick={onRemoveParameter}
                                    tooltip="Remove Selected Parameter"
                                />
                            </>
                        }
                    </div>
                    { (rows && rows.length) ?
                        <Table 
                            style={{ marginLeft: "8px" }}
                            compact={true}
                            columns={[
                                { 
                                    key: "Parameter Datatype", 
                                    name: "Data Type",
                                    minWidth: 100,
                                    maxWidth: 200,
                                    isResizable: true,
                                    onRender: (item) =>  
                                        <div onClick={(e) => e.stopPropagation()}>
                                            { editable ?
                                                <DataTypeDropdown 
                                                    id="ParameterDataType"
                                                    value={parameters[item.key]}
                                                    onChanged={(dataType) => { 
                                                        let t = [...tuple];
                                                        t[3][item.key] = dataType;                           
                                                        props.onChanged(t);
                                                    }}
                                                /> : <Typography text={parameters[item.key]} />
                                            }
                                        </div>
                                },
                                { 
                                    key: "Name", 
                                    name: "Function Parameter",
                                    minWidth: 100,
                                    maxWidth: 300,
                                    isResizable: true,
                                    onRender: (item) => 
                                        <div onClick={(e) => e.stopPropagation()}>
                                            <EditableLabel 
                                                placeholder="Parameter name..."                        
                                                value={parameters[item.key+1]}
                                                onChanged={(variableName) => {
                                                    let t = [...tuple];
                                                    t[3][item.key+1] = variableName;
                                                
                                                    props.onChanged(t);
                                                }}
                                            />
                                        </div>
                                },
                            ]}
                            rows={rows}
                            selectionMode={editable ? "multiple" : "none"}
                            onSelectionChanged={setSelectedParameters}
                        /> : null
                    }
                </td>
            </tr>
        );
    };

    const onRenderExpressionBody = () => {
        return (
            <tr>
                <td style={{ whiteSpace: "nowrap", verticalAlign: "top" }}>
                    <Typography 
                        style={{ display: "inline-block" }}
                        color="textSecondary" 
                        size="5" 
                        text="Expression Body"
                    />
                </td>
                <td style={{ width: "99%" }}>
                    <div style={{ display: "inline-block", width: "100%" }}>
                        <ExpressionTuple 
                            tuple={tuple[4]}
                            onChanged={(expression) => {
                                let t = [...tuple];
                                t[4] = expression;

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
            contains={
                <table style={{ width: "100%" }}>
                    <tbody>
                        { onRenderReturnDataType() }
                        { onRenderParameters() }
                        { onRenderExpressionBody() }
                    </tbody>
                </table>
            }                
            groupHeaderText={label}
            groupHeaderDescription={description}                    
        />            
    );
};

ExpressionTupleOperationLambda.propTypes = {
    onChanged:          PropTypes.func,
    tuple:              PropTypes.array,
};

export default ExpressionTupleOperationLambda;
