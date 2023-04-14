// See the 'COPYING' file in the project root for licensing information.
/* eslint-disable jsx-a11y/click-events-have-key-events */
/* eslint-disable jsx-a11y/no-static-element-interactions */
import {useEffect, useState} from "react";

import {ExpressionTupleGroup} from "./ExpressionTupleGroup";
import HybridBuilder from "../HybridBuilder";

import {
    Button,    
    Table,
    Typography,
    useOperational
} from "@afw/react";

const ExpressionTupleOperationCall = (props) => {

    const [parameters, setParameters] = useState([]);
    const [selectedParameters, setSelectedParameters] = useState();
    const {editable} = useOperational(props);

    const {description, label, onChanged, tuple} = props;

    useEffect(() => {
        let parameters = [];

        if (tuple && tuple.length > 2) {
            for (let key = 3; key < tuple.length; key++)
                parameters.push({ key });            
        }

        setParameters(parameters);
    }, [tuple]);    

    const onRemoveParameter = () => {
        const selectedParameter = selectedParameters[0];
        let t = [...tuple];
        
        t.splice(selectedParameter.key, 1);
            
        props.onChanged(tuple);
        setSelectedParameters([]);        
    };

    const onAddParameter = () => {
        let t = [...tuple];
        t.push(null);

        props.onChanged(t);
    };

    const onRenderParameters = () => {

        return (
            <tr>                                
                <td colSpan="2">
                    <div style={{ display: "flex" }}>
                        <Typography 
                            color="textSecondary"
                            size="5"
                            text="Parameters"
                        />
                        <Button 
                            style={{ color: "#4caf50" }}
                            type="icon"
                            label="Add Parameter"
                            icon="add"                            
                            onClick={onAddParameter}
                            tooltip="Add Parameter"
                        />
                        <Button 
                            style={{ color: (selectedParameters && selectedParameters.length > 0) ? "#f44336" : undefined }}
                            type="icon"
                            label="Remove Parameter"
                            icon="remove"                            
                            onClick={onRemoveParameter}
                            tooltip="Remove Selected Parameter"
                        />
                    </div>                    
                    <Table 
                        compact={true}
                        columns={[
                            {
                                key: "Parameter",
                                name: "Parameter",
                                onRender: (item) => 
                                    <div onClick={e => e.stopPropagation()}>
                                        <HybridBuilder                                        
                                            hybrid={tuple[item.key]}
                                            onChanged={hybrid => {
                                                let t = [...tuple];
                                                t[item.key] = hybrid;
                                                props.onChanged(t);
                                            }}                                    
                                        />
                                    </div>
                            }
                        ]}
                        rows={parameters}
                        selectionMode={editable ? "single" : "none"}
                        onSelectionChanged={(selectedParameters) => setSelectedParameters(selectedParameters)}
                    />                    
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
                        <tr>
                            <td style={{ whiteSpace: "nowrap" }}>
                                <Typography 
                                    color="textSecondary"
                                    size="5"
                                    text="Function"
                                />
                            </td>
                            <td style={{ width: "99%" }}>
                                <HybridBuilder 
                                    hybrid={tuple[2]}
                                    dataType="string"
                                    onChanged={name => {
                                        let t = [...tuple];
                                        t[2] = name;
                                        props.onChanged(t);
                                    }}
                                />
                            </td>
                        </tr>                            
                        {onRenderParameters()}
                    </tbody>
                </table>
            }                
            groupHeaderText={label}
            groupHeaderDescription={description}
        />
    );
};

ExpressionTupleOperationCall.propTypes = {
};

export default ExpressionTupleOperationCall;
