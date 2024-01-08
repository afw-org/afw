// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Box
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import MuiBox from "@mui/material/Box";
import {AdaptiveComponent} from "@afw/react";

export const BoxItem = ({ contains, ...rest }) => {

    return (
        <MuiBox {...rest}>
            {
                contains &&
                <AdaptiveComponent layoutComponent={contains} />
            }
        </MuiBox>
    );
};

export const Box = ({ contains, ...rest }) => {       

    return (
        <MuiBox display="flex" {...rest}>
            {
                contains && contains.map((boxItem, index) => 
                    <BoxItem key={index} {...boxItem} />
                )
            }
        </MuiBox>                                    
    );
};

export default Box;
