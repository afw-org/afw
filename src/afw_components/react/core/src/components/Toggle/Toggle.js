// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Toggle
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Toggle.propTypes";
import fallback from "./Toggle.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Toggle
 * Category          : inputs
 * 
 * A component that renders a boolean value as a toggleable switch.
 * 
 * This component takes a boolean value and displays it as a switch that can
 * be toggled on or off to represent true or false.  It may also have text
 * labels to interpret its meaning.
 * 
 */
export const Toggle = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Toggle")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Toggle",
                parameters: props
            }}
        />
    );
};


Toggle.propTypes = propTypes;
Toggle.defaultProps = defaultProps;
Toggle.displayName = "Toggle";

export default memo(Toggle);
