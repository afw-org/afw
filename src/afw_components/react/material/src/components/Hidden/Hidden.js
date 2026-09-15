// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Hidden
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import PropTypes from "prop-types";

import Box from "@mui/material/Box";

import {AdaptiveComponent} from "@afw/react";

/*
 * MUI's own <Hidden> component was removed in v7 (replaced by the `sx` prop
 * or useMediaQuery, per its own migration guide) - this rebuilds the same
 * xsDown/xsUp/.../only API on top of a breakpoint-keyed `sx.display`, since
 * that's the public prop contract other AFW layouts already depend on.
 */
const breakpointOrder = ["xs", "sm", "md", "lg", "xl"];

const computeHiddenBreakpoints = ({
    xsDown, smDown, mdDown, lgDown, xlDown,
    xsUp, smUp, mdUp, lgUp, xlUp,
    only,
}) => {
    if (only) {
        const onlyBreakpoints = Array.isArray(only) ? only : [only];
        return new Set(onlyBreakpoints);
    }

    const downMap = {xs: xsDown, sm: smDown, md: mdDown, lg: lgDown, xl: xlDown};
    const upMap = {xs: xsUp, sm: smUp, md: mdUp, lg: lgUp, xl: xlUp};
    const hidden = new Set();

    breakpointOrder.forEach((bp, index) => {
        breakpointOrder.forEach((downBp, downIndex) => {
            if (downMap[downBp] && index <= downIndex)
                hidden.add(bp);
        });
        breakpointOrder.forEach((upBp, upIndex) => {
            if (upMap[upBp] && index >= upIndex)
                hidden.add(bp);
        });
    });

    return hidden;
};

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

    const hiddenBreakpoints = computeHiddenBreakpoints({
        xsDown, smDown, mdDown, lgDown, xlDown,
        xsUp, smUp, mdUp, lgUp, xlUp,
        only,
    });

    const display = {};
    breakpointOrder.forEach(bp => {
        display[bp] = hiddenBreakpoints.has(bp) ? "none" : "block";
    });

    return (
        <Box
            className={className}
            sx={{ display }}
        >
            {
                contains ?
                    <AdaptiveComponent layoutComponent={contains} /> : null
            }
        </Box>
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
