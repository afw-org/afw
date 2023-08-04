// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Paper
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */
import {memo} from "react";
import MuiPaper from "@mui/material/Paper";

import {AdaptiveComponent} from "@afw/react";

export const Paper = ({ contains, ...rest }) => {
    
    return (            
        <MuiPaper {...rest}>
            <AdaptiveComponent layoutComponent={contains} />            
        </MuiPaper>            
    );
};

export default memo(Paper);
