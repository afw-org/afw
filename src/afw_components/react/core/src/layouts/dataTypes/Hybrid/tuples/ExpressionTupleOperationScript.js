// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";
import PropTypes from "prop-types";

import {
    Button,
    Dialog,
    CodeEditor,    
    Typography
} from "@afw/react";
import {useOperational} from "@afw/react";

const ExpressionTupleOperationScript = (props) => {
        
    const [warn, setWarn] = useState(false);
    const {onChanged, tuple} = props;
    const {editable} = useOperational(props);

    return (
        <>
            <div style={{ display: "flex", height: "100%" }}>
                { editable &&
                    <div>
                        <Button 
                            style={{ color: "#f44336" }}
                            type="icon"           
                            label="Remove Script"             
                            tooltip="Remove this Script"
                            icon="close"                  
                            onClick={() => setWarn(true)}
                        />
                    </div>                
                }
                <div style={{ flex: 1, height: "100%" }}>
                    <CodeEditor 
                        style={{ width: "100%", height: "100%" }}
                        language="afw"
                        readOnly={editable === false}
                        showLineNumbers={true}
                        source={tuple[2] ? tuple[2] : ""}
                        onChanged={source => {
                            let t = [...tuple];
                            t[2] = source;
    
                            onChanged(t);
                        }}
                    />
                </div>
            </div>
            <Dialog 
                open={Boolean(warn)}
                onDismiss={() => setWarn(false)}
                isBlocking={false}
                maxWidth="sm"
                title="Remove Script"
                showClose={true}
                contains={
                    <Typography text="Remove Adaptive Script?" />
                }
                footer={
                    <div style={{ display: "flex", justifyContent: "flex-end", padding: "20px", width: "100%" }}>  
                        <div style={{ marginRight: "5px" }}>
                            <Button
                                label="No"
                                onClick={() => setWarn(false)}
                            />
                        </div>                      
                        <Button
                            label="Yes"
                            color="primary"
                            onClick={() => {
                                onChanged();
                                setWarn(false);
                            }}
                        />                        
                    </div>
                }
            />
        </>
    );
};

ExpressionTupleOperationScript.propTypes = {
    allowChange:        PropTypes.bool,
    onChanged:          PropTypes.func,
    tuple:              PropTypes.array,
};

export default ExpressionTupleOperationScript;
