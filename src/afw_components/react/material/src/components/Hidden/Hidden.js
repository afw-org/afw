// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Hidden
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {memo} from "react";
import PropTypes from "prop-types";

import MuiHidden from "@mui/material/Hidden";

import {AdaptiveComponent} from "@afw/react";

/**
 * Implementation Id : Hidden
 * Category          : utilities
 * 
 * A component that hides content under screen breakpoint conditions.
 * 
 * This is a utility component that gives a layout the ability to hide
 * content under screen size conditions.  This is necessary for responsive
 * layouts to provide both complex or simply designs that are accessible and
 * usable from a variety of screen devices.
 * 
 */
export const Hidden = ({ 
    className, xsDown, xsUp, smDown, smUp, mdDown, mdUp, lgDown, lgUp, 
    xlDown, xlUp, only, contains
}) => {

    return (
        <MuiHidden            
            className={className}                
            xsDown={xsDown}
            xsUp={xsUp}
            smDown={smDown}
            smUp={smUp}
            mdDown={mdDown}
            mdUp={mdUp}
            lgDown={lgDown}
            lgUp={lgUp}
            xlDown={xlDown}
            xlUp={xlUp}
            only={only}
        >
            { 
                contains ? 
                    <AdaptiveComponent layoutComponent={contains} /> : null
            }
        </MuiHidden>
    );
};

Hidden.propTypes = {
    contains:           PropTypes.oneOfType([ PropTypes.element, PropTypes.object ]),
    xsDown:             PropTypes.bool,
    smDown:             PropTypes.bool,
    mdDown:             PropTypes.bool,
    lgDown:             PropTypes.bool,
    xlDown:             PropTypes.bool,
    only:               PropTypes.oneOfType([ PropTypes.string, PropTypes.arrayOf(PropTypes.string)]),
    for:                PropTypes.oneOf(["xs", "sm", "md", "lg", "xl"]),
};

export default memo(Hidden);
