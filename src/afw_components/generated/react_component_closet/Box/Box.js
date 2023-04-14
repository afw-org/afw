// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Box
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Box.propTypes";
import fallback from "./Box.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Box
 * Category          : containers
 * 
 * A component that renders a flexible Box of content.
 * 
 * This component is a very powerful container, which implements a box
 * allowing items to be added in a flexible way.
 * 
 */
export const Box = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Box")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Box",
                parameters: props
            }}
        />
    );
};


Box.propTypes = propTypes;
Box.defaultProps = defaultProps;
Box.displayName = "Box";

export default memo(Box);
