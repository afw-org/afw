// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Box
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import MuiDrawer from "@mui/material/Drawer";
import {AdaptiveComponent} from "@afw/react";

export const Drawer = ({ contains, open, anchor, variant }) => {       

    return (
        <MuiDrawer open={open} anchor={anchor} variant={variant}>
            {
                contains && <AdaptiveComponent layoutComponent={contains} />                
            }
        </MuiDrawer>                                    
    );
};

export default Drawer;
