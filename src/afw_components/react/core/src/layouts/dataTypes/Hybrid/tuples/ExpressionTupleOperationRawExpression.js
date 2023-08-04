// See the 'COPYING' file in the project root for licensing information.
import PropTypes from "prop-types";

import {
    Button,
    EditableLabel
} from "@afw/react";

import {useOperational} from "@afw/react";

const ExpressionTupleOperationRawExpression = (props) => {
    const {onChanged, tuple } = props;
    const {editable} = useOperational(props);

    return (
        <div style={{ display: "flex" }}>
            { editable &&
                <div>
                    <Button 
                        type="icon"
                        label="Remove Raw Expression"
                        style={{ color: "#f44336" }}
                        icon="close"             
                        onClick={() => onChanged()}
                    />
                </div>                
            }
            <div>
                <EditableLabel 
                    {...props}
                    multiline={true}
                    placeholder="Enter an expression..."
                    value={tuple[2] ? tuple[2] : undefined}  
                    onChanged={(expression) => {
                        let t = [...tuple];
                        t[2] = expression;
                        onChanged(t);
                    }}                      
                />
            </div>
        </div>
    );    
};

ExpressionTupleOperationRawExpression.propTypes = {
    allowWrite:         PropTypes.bool,
    onChanged:          PropTypes.func,
    tuple:              PropTypes.array,
};

export default ExpressionTupleOperationRawExpression;
