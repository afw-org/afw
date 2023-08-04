// See the 'COPYING' file in the project root for licensing information.
import HybridBuilder from "../HybridBuilder";
import {ExpressionTupleGroup} from "./ExpressionTupleGroup";

import {
    Typography
} from "@afw/react";

const ExpressionTupleOperationProperty = (props) => {
    
    const {description, label, onChanged, tuple} = props;

    return (
        <ExpressionTupleGroup 
            tuple={tuple}
            contains={
                <table style={{ width: "100%" }}>
                    <tbody>
                        <tr>
                            <td style={{ whiteSpace: "nowrap" }}>
                                <Typography 
                                    color="textSecondary"
                                    size="5"
                                    text="Property Name"                                    
                                />
                            </td>
                            <td style={{ width: "99%" }}>
                                <HybridBuilder 
                                    dataType="string" 
                                    hybrid={tuple[2]}
                                    onChanged={(propertyName) => {
                                        let t = [...tuple];
                                        t[2] = propertyName;
                                        onChanged(t);
                                    }}
                                />
                            </td>
                        </tr>
                        <tr>
                            <td style={{ whiteSpace: "nowrap" }}>
                                <Typography 
                                    color="textSecondary"
                                    size="5"
                                    text="Object"                                    
                                />
                            </td>
                            <td style={{ width: "99%" }}>
                                <HybridBuilder 
                                    dataType="object" 
                                    hybrid={tuple[3]}
                                    onChanged={(propertyName) => {
                                        let t = [...tuple];
                                        t[3] = propertyName;
                                        onChanged(t);
                                    }}
                                />
                            </td>
                        </tr>
                    </tbody>                        
                </table>
            }                
            groupHeaderText={label}
            groupHeaderDescription={description}
            onChanged={onChanged}
        />
    );
};

ExpressionTupleOperationProperty.propTypes = {
};

export default ExpressionTupleOperationProperty;
