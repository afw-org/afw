// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for ToggleButtons
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./ToggleButtons.propTypes";
import fallback from "./ToggleButtons.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : ToggleButtons
 * Category          : inputs
 * 
 * A component to display a selectable choice of options, presented as buttons
 * that can be toggled on or off.
 * 
 * This component provides a list of options, organized as buttons, that may
 * be toggled on or off. They may be inclusive or exclusive.
 * 
 */
export const ToggleButtons = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "ToggleButtons")}
            fallback={fallback}
            layoutComponent={{
                componentType: "ToggleButtons",
                parameters: props
            }}
        />
    );
};


ToggleButtons.propTypes = propTypes;
ToggleButtons.defaultProps = defaultProps;
ToggleButtons.displayName = "ToggleButtons";

export default memo(ToggleButtons);
