// See the 'COPYING' file in the project root for licensing information.
import {ExpressionTupleGroup} from "./ExpressionTupleGroup";
import HybridBuilder from "../HybridBuilder";

import {
    Typography
} from "@afw/react";

const ExpressionTupleOperationCallLambda = (props) => {
     
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
                                    text="Lambda Function"                                        
                                />
                            </td>
                            <td style={{ width: "99%" }}>
                                <HybridBuilder            
                                    onChanged={(lambdaFunction) => {
                                        let t = [...tuple];
                                        t[2] = lambdaFunction;
                                        props.onChanged(t);
                                    }}
                                />
                            </td>
                        </tr>
                        <tr>
                            <td style={{ whiteSpace: "nowrap" }}>
                                <Typography 
                                    color="textSecondary"
                                    size="5"
                                    text="Parameters"                                        
                                />
                            </td>
                            <td style={{ width: "99%" }}>
                                <HybridBuilder 
                                    onChanged={(parameters) => {
                                        let t = [...tuple];
                                        t[3] = parameters;
                                        props.onChanged(t);
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

ExpressionTupleOperationCallLambda.propTypes = {
};

export default ExpressionTupleOperationCallLambda;
