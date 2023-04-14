// See the 'COPYING' file in the project root for licensing information.
import MuiTooltip from "@mui/material/Tooltip";

import {AdaptiveComponent} from "@afw/react";

export const Tooltip = ({ 
    contains, content, delay, leaveDelay, target,
    id, "data-testid": dataTestId, "data-component-type": dataComponentType
}) => {

    if (!target)
        return null;

    return (                
        <MuiTooltip    
            id={id}
            data-testid={dataTestId}
            data-component-type={dataComponentType}                                     
            title={
                contains ?
                    <AdaptiveComponent layoutComponent={contains} /> :                    
                    content ? 
                        content : ""
            } 
            placement="top-start"     
            enterDelay={delay}
            leaveDelay={leaveDelay}
        >
            <AdaptiveComponent layoutComponent={target} />              
        </MuiTooltip>
    );
};

export default Tooltip;
