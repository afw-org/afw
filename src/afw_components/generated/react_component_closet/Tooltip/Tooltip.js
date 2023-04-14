// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Tooltip
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Tooltip.propTypes";
import fallback from "./Tooltip.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Tooltip
 * Category          : utilities
 * 
 * A component that surrounds content with a tooltip.
 * 
 * This component renders a small popup with text when the user hovers over
 * an element for a specific amount of time. This may provide helpful hints
 * to explain to the user what they are looking at.
 * 
 */
export const Tooltip = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Tooltip")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Tooltip",
                parameters: props
            }}
        />
    );
};


Tooltip.propTypes = propTypes;
Tooltip.defaultProps = defaultProps;
Tooltip.displayName = "Tooltip";

export default memo(Tooltip);
