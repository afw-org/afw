// See the 'COPYING' file in the project root for licensing information.
import PropTypes from "prop-types";

import {Button} from "@afw/react";
import {Template} from "../../Template";

import {useOperational} from "@afw/react";

const ExpressionTupleOperationTemplate = (props) => {
    
    const {onChanged, tuple} = props;
    const {editable} = useOperational(props);

    return (
        <div style={{ display: "flex", height: "100%" }}>
            { editable &&
                <div>
                    <Button 
                        style={{ color: "#f44336" }}
                        type="icon"           
                        label="Remove Template"             
                        icon="close"                  
                        onClick={() => onChanged()}
                        disabled={editable === false}
                    />
                </div>
            }
            <div style={{ flex: 1 }}>
                <Template
                    {...props}
                    value={tuple[2] ? tuple[2] : ""}  
                    onChanged={(template) => {
                        let t = [...tuple];
                        t[2] = template;

                        onChanged(t);
                    }}                      
                />
            </div>
        </div>
    );
};

ExpressionTupleOperationTemplate.propTypes = {
    allowChange:        PropTypes.bool,
    onChanged:          PropTypes.func,
    tuple:              PropTypes.array,
};

export default ExpressionTupleOperationTemplate;
