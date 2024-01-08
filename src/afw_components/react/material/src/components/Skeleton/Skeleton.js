// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Skeleton
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import PropTypes from "prop-types";

import MuiSkeleton from "@mui/material/Skeleton";


/**
 * Implementation Id : Skeleton
 * Category          : utilities
 * 
 * A component that displays a skeleton, or shape, of a component that may
 * shimmer to indicate that an actual component is waiting to be loaded.
 * 
 * The Skeleton is a feedback utility component, similar to the Spinner, but
 * it takes on a shape that gives the user the perspective that data is
 * nearly ready and the component is attempting to render.  It's a component
 * placeholder, which often animates or shimmers.
 * 
 */
export const Skeleton = ({ type, height, width, ...rest }) => {

    let variant = (type === "circle") ? "circle" : "rect";
    return (
        <MuiSkeleton variant={variant} width={width} height={height} {...rest} />
    );
};


/**
 * PropTypes generated by _AdaptiveLayoutComponentType_Skeleton
 */
Skeleton.propTypes = {
    /**
     * height
     * Data Type: (string)
     *
     * The CSS height of the skeleton.
     */
    height:                             PropTypes.oneOfType([ PropTypes.string, PropTypes.number ]),
    /**
     * type
     * Data Type: (string)
     *
     * The shape of the skeleton to display (line or circle).
     */
    type:                               PropTypes.string,
    /**
     * width
     * Data Type: (string)
     *
     * The CSS width of the skeleton.
     */
    width:                              PropTypes.oneOfType([ PropTypes.string, PropTypes.number ]),
};

Skeleton.defaultProps = {
};

Skeleton.displayName = "Skeleton";

export default memo(Skeleton);
