// See the 'COPYING' file in the project root for licensing information.
import {memo} from "react";
import MuiModal from "@mui/material/Modal";

import {AdaptiveComponent} from "@afw/react";

export const Modal = ({ 
    id, "data-testid": dataTestId, "data-component-type": dataComponentType, 
    contains, open, isBlocking, onDismiss, ...rest 
}) => {  

    return (
        <MuiModal 
            id={id}
            data-testid={dataTestId}
            data-component-type={dataComponentType}
            open={open}
            onClose={isBlocking ? () => false : onDismiss}                                     
        >                
            <div style={{
                backgroundColor: "white",
                position: "absolute", 
                top: "50%",
                left: "50%",
                transform: "translate(-50%, -50%)",
            }}>
                <AdaptiveComponent {...rest} layoutComponent={contains} />
            </div>
        </MuiModal>
    );    
};

export default memo(Modal);
