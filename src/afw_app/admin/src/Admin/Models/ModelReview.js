// See the 'COPYING' file in the project root for licensing information.
import {
    Button,
    Dialog,
    ObjectDifferences
} from "@afw/react";

import {useTheme} from "../../hooks";

export const ModelReview = (props) => {

    const theme = useTheme();
    const {open, onDismiss, onSave, model, modelDifferences} = props;

    return (            
        <Dialog
            open={open}
            onDismiss={onDismiss}
            isBlocking={false}
            showClose={true}
            title="Review Model Changes"
            maxWidth="xl"
            contains={
                <div style={{ padding: theme.spacing(2) }}>
                    <ObjectDifferences 
                        object={model} 
                        objectDifferences={modelDifferences}
                    />                    
                </div>
            }
            footer={
                <div style={{ display: "flex", padding: theme.spacing(2) }}>   
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button
                            variant="contained"
                            color="primary"
                            variant="contained"
                            label="Apply Changes"
                            onClick={onSave}                            
                        />        
                    </div>
                    <div>
                        <Button
                            variant="contained"
                            style={{ marginLeft: theme.spacing(1) }}
                            label="Close"
                            onClick={props.onDismiss}
                        />
                    </div>
                </div>
            }
        />               
    );
};

export default ModelReview;
