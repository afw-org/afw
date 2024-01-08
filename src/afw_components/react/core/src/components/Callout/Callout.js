// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Callout
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Callout.propTypes";
import fallback from "./Callout.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Callout
 * Category          : containers
 * 
 * A layout container that displays content inside a popup.
 * 
 * This component container is visible when a target element is clicked.
 * It's used to quickly show or grab input from the user in a popup area and
 * can be dismissed.
 * 
 */
export const Callout = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Callout")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Callout",
                parameters: props
            }}
        />
    );
};


Callout.propTypes = propTypes;
Callout.defaultProps = defaultProps;
Callout.displayName = "Callout";

export default memo(Callout);
