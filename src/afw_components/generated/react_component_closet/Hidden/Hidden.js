// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Hidden
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Hidden.propTypes";
import fallback from "./Hidden.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Hidden
 * Category          : utilities
 * 
 * A component that hides content under screen breakpoint conditions.
 * 
 * This is a utility component that gives a layout the ability to hide content
 * under screen size conditions. This is necessary for responsive layouts to
 * provide both complex or simply designs that are accessible and usable from
 * a variety of screen devices.
 * 
 */
export const Hidden = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Hidden")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Hidden",
                parameters: props
            }}
        />
    );
};


Hidden.propTypes = propTypes;
Hidden.defaultProps = defaultProps;
Hidden.displayName = "Hidden";

export default memo(Hidden);
