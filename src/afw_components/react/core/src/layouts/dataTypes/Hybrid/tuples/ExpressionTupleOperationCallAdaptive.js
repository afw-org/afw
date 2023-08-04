// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";

import {ExpressionTupleGroup} from "./ExpressionTupleGroup";
import HybridBuilder from "../HybridBuilder";

import {
    Button,
    Tooltip,
    Typography    
} from "@afw/react";

import {useOperational, useFunctions} from "@afw/react";

/*
 * The Call Adaptive Function expression tuple has the form:
 * 
 *     [ annotation, "callA", functionId, ...parameters ]
 * 
 * The functionId parameter may be a string value, or an expression tuple 
 *   that resolves to string dataType, which is the name of the adaptive
 *   function.
 */
export const ExpressionTupleOperationCallAdaptive = (props) => {

    const [func, setFunc] = useState();
    const [/*isMulti*/, setIsMulti] = useState();
    const [description, setDescription] = useState();

    const {functions} = useFunctions();
    const {tuple} = props;
    const {editable} = useOperational(props);

    useEffect(() => {
        if (tuple && functions) {
            const functionLabel = tuple[2];

            for (const f of functions) {
                if (f.functionLabel === functionLabel || f.functionId === functionLabel) {
                    const params = f.parameters;
                    let isMulti = false;
                    const description = f.description;
                    if (params && params.length > 0) {
                        const minArgs = params[params.length - 1].minArgs;
                        if (minArgs > -1)
                            isMulti = true;
                    }

                    setFunc(f);
                    setIsMulti(isMulti);
                    setDescription(description);

                    break;
                }
            }
        }
    }, [tuple, functions]);

    const onParameterChanged = (param, paramIndex, tupleIndex, value) => {
        let tuple = props.tuple;

        /*! \fixme this won't always work, depending on the param (inline vs not inline) */
        tuple[tupleIndex + 3] = value;

        /*! \fixme if last parameter was removed, adjust the tuple to completely remove it. */
        props.onChanged(tuple);
    };

    const onAddParameter = (/*param, paramIndex, tupleIndex*/) => {
        let tuple = props.tuple;

        tuple.push(null);
        
        props.onChanged(tuple);
    };

    const onRenderParameters = () => {
        let params;

        if (func) {
            params = func.parameters;
        }

        /* The tuple should look like [annotation, "callA", "functionId", ...tupleParams] */
        let tupleParams = tuple.slice(3);         
        let tupleIndex = 0;           

        if (params) {            
            let rows = [];
            params.forEach((param, paramIndex) => {                     
                const {name, description, allowFunction, allowSingle, dataType, optional, dataTypeParameter, minArgs} = param;

                let hybrids = [];
                if (minArgs > -1) {
                    /* last parameter may have multiple args */
                    for (; tupleIndex < tupleParams.length; tupleIndex++) {
                        let tuple_index = tupleIndex;
                        hybrids.push(
                            <HybridBuilder 
                                key={tuple_index}
                                label="Value"
                                dataType={dataType}
                                dataTypeParameter={dataTypeParameter}
                                allowLiterals={allowSingle}
                                allowFunction={allowFunction}         
                                onChanged={(value) => onParameterChanged(param, paramIndex, tuple_index, value)}     
                                hybrid={tupleParams[tuple_index]}              
                            />
                        );
                    }
                } else {
                    let tuple_index = tupleIndex;
                    hybrids.push(
                        <HybridBuilder 
                            key={tuple_index}
                            label="Value"
                            dataType={dataType}
                            dataTypeParameter={dataTypeParameter}
                            allowLiterals={allowSingle}
                            allowFunction={allowFunction}         
                            onChanged={(value) => onParameterChanged(param, paramIndex, tuple_index, value)}     
                            hybrid={tupleParams[tuple_index]}              
                        />
                    );
                    tupleIndex++;
                }

                if (editable || (hybrids.length > 0))
                    rows.push(
                        <tr key={paramIndex}>
                            <td style={{ whiteSpace: "nowrap", verticalAlign: "top" }}>
                                <Tooltip                             
                                    content={description}
                                    target={    
                                        <div style={{ display: "flex" }}>
                                            <Typography 
                                                style={{ marginRight: "5px" }}
                                                color="textSecondary" 
                                                size="5" 
                                                text={name + (optional === false ? " *" : "")} 
                                            />
                                            {
                                                (minArgs > -1) &&
                                                <Button
                                                    style={{ color: "green" }}
                                                    type="icon"
                                                    label="Add Parameter"
                                                    icon="add"
                                                    color="primary"
                                                    onClick={() => onAddParameter(param, paramIndex, tupleIndex)}
                                                />
                                            }
                                        </div>
                                    }
                                    delay={2000}
                                />
                            </td>
                            <td style={{ width: "99%" }}>                            
                                {hybrids} 
                            </td>
                        </tr>
                    );
            });

            return (
                <table>
                    <tbody style={{ width: "100%" }}>
                        {rows}
                    </tbody>
                </table>
            );            

        } else {
            /* without a known adaptive function, we can only let the user define the tuple parameters */
            return (
                <div>
                    {
                        tupleParams.map((value, valueIndex) => {
                            return (
                                <div key={valueIndex}>                                   
                                    <div style={{ overflow: "hidden" }}>
                                        <HybridBuilder 
                                            label="Value" 
                                            hybrid={tuple[3]}
                                            onChanged={(v) => {                                        
                                                tuple[3] = v;
                                                props.onChanged(tuple);
                                            }}     
                                        />
                                    </div>
                                </div>
                            );
                        })
                    }
                </div>
            );
        }
    };

    const onChanged = (tuple) => {
        props.onChanged(tuple);
    };

    return (            
        <ExpressionTupleGroup 
            tuple={tuple}
            onChanged={onChanged}
            contains={onRenderParameters()}                
            groupHeaderText={"Call " + tuple[2] + "()"}
            groupHeaderDescription={description ? description : ""}
        />
    );
};

ExpressionTupleOperationCallAdaptive.propTypes = {
};

export default ExpressionTupleOperationCallAdaptive;
