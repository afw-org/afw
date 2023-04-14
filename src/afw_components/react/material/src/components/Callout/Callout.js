// See the 'COPYING' file in the project root for licensing information.
import Popover from "@mui/material/Popover";

import {AdaptiveComponent} from "@afw/react";

export const Callout = ({ target, contains, open, onDismiss, targetPosition, id, "data-testid": dataTestId, "data-component-type": dataComponentType }) => {    

    return (            
        <Popover              
            id={id}
            data-testid={dataTestId}
            data-component-type={dataComponentType}
            anchorReference={targetPosition ? "anchorPosition" : "anchorEl"}          
            open={open}
            anchorEl={target}
            anchorPosition={targetPosition}
            onClose={onDismiss}
        >
            <AdaptiveComponent layoutComponent={contains} />
        </Popover>            
    );   
};

export default Callout;
