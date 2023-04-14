// See the 'COPYING' file in the project root for licensing information.
import {
    Button,
    Dialog,
    ObjectDifferences
} from "@afw/react";

import {useTheme} from "../hooks";

const ObjectDifferencesModal = (props) => {

    const theme = useTheme();

    const onSave = () => {
        if (props.onSave)
            props.onSave(props.object);
    };

    return (
        <Dialog
            open={props.open}
            onDismiss={props.onDismiss}
            isBlocking={false}
            title="Object Differences"
            showClose={true}
            maxWidth="xl"
            contains={
                <div style={{ padding: theme.spacing(2) }}>                        
                    <ObjectDifferences {...props} />                        
                </div>
            }
            footer={
                <div style={{ display: "flex", justifyContent: "flex-end", padding: theme.spacing(2), width: "100%" }}>   
                    <Button
                        style={{ marginRight: theme.spacing(1) }}
                        color="primary"
                        variant="contained"
                        label="Apply Changes"
                        onClick={onSave}                            
                    />                     
                    <Button                        
                        label="Close"
                        onClick={props.onDismiss}
                    />
                </div>
            }
        />                                
    );
};

export default ObjectDifferencesModal;
