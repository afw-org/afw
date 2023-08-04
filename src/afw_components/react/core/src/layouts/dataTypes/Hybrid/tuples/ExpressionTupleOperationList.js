// See the 'COPYING' file in the project root for licensing information.
/* eslint-disable jsx-a11y/click-events-have-key-events */
/* eslint-disable jsx-a11y/no-static-element-interactions */
import {ExpressionTupleGroup} from "./ExpressionTupleGroup";
import HybridBuilder from "../HybridBuilder";

import {
    Button,
    Typography,
    Table
} from "@afw/react";
//import {useOperational} from "@afw/react";

const ExpressionTupleOperationList = (props) => {
 
    const {description, label, onChanged, tuple} = props;
    //const {editable} = useOperational(props);

    const onAddParameter = () => {
        let t = [...tuple];

        t.push(null);        
        props.onChanged(t);
    };

    /*
    const onParameterNameChanged = (index, value) => {
        let t = [...tuple];

        t[index+2] = value;
        props.onChanged(t);
    };
     */

    const onParameterValueChanged = (index, value) => {
        let t = [...tuple];

        t[index+2] = value;
        props.onChanged(t);
    };

    const onRemoveParameter = () => {
        /* \fixme */
    };

    const onRenderParameters = () => {
        let rows = [];

        /* The tuple should look like [annotation, "list", "item1", "item2", ...] */
        let tupleParams = tuple.slice(2);       

        for (let i = 0; i < tupleParams.length; i ++) {
            rows.push({ key: i });
        }

        return (
            <div style={{ width: "100%" }}>
                <div style={{ display: "flex", alignItems: "center" }}>
                    <Typography 
                        color="textSecondary" 
                        size="5" 
                        text="Items"
                    />
                    <Button
                        style={{ color: "#4caf50" }}
                        type="icon"
                        label="Add Parameter"
                        icon="add"
                        color="primary"
                        onClick={onAddParameter}
                        tooltip="Add List Item"
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
                </div>
                <Table 
                    dense={true}
                    columns={[                        
                        {
                            key: "Item",
                            name: "Item",
                            onRender: (item) => 
                                <div onClick={(e) => e.stopPropagation()}>
                                    <HybridBuilder 
                                        onChanged={() => onParameterValueChanged(item.key)}
                                        hybrid={tupleParams[item.key]}
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


export default ExpressionTupleOperationList;
