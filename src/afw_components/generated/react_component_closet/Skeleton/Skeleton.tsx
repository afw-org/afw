// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Skeleton
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Skeleton.propTypes";
import {ISkeletonProps} from "./Skeleton.types";
import fallback from "./Skeleton.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Skeleton
 * Category          : utilities
 * 
 * A component that displays a skeleton, or shape, of a component that may
 * shimmer to indicate that an actual component is waiting to be loaded.
 * 
 * The Skeleton is a feedback utility component, similar to the Spinner, but
 * it takes on a shape that gives the user the perspective that data is
 * nearly ready and the component is attempting to render. It's a component
 * placeholder, which often animates or shimmers.
 * 
 */
export const Skeleton : React.FunctionComponent<ISkeletonProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Skeleton")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Skeleton",
                parameters: props
            }}
        />
    );
};


Skeleton.propTypes = propTypes;
Skeleton.defaultProps = defaultProps;
Skeleton.displayName = "Skeleton";

export default React.memo(Skeleton);
