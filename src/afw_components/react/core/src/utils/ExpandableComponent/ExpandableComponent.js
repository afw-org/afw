// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

import {
    Button,
    Dialog
} from "../../components";

import {useTheme} from "../../hooks";

/**
 * This utility component makes the child expandable to a larger view. 
 */
export const ExpandableComponent = ({ children, expandedChildren }) => {

    const [expanded, setExpanded] = useState(false);    
    const theme = useTheme();

    return (
        <>
            <div style={{ height: "100%", width: "100%", display: "flex", alignItems: "start" }}>
                <div style={{ flex: 1 }}>
                    { children }            
                </div>
                <Button 
                    type="icon" 
                    icon="fullscreen"
                    tooltip="Fullscreen"
                    onClick={() => setExpanded(true)}
                />
            </div>            
            <Dialog 
                open={expanded}
                isBlocking={true}
                contains={
                    <div style={{ height: "100%" }}>
                        <div style={{ float: "right" }}>                        
                            <Button                                 
                                color="primary"                            
                                icon="fullscreen_exit"
                                tooltip="Exit Full Screen"
                                label="Exit Full Screen"
                                onClick={() => setExpanded(false)}                           
                            />
                        </div>
                        <div style={{ height: "100%", padding: theme.spacing(6)}}>
                            { expandedChildren || children }
                        </div>
                    </div>                    
                    
                } 
                onDismiss={() => setExpanded(false)}
                fullScreen                
            />            
        </>
    );
};

export default ExpandableComponent;
