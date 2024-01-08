// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Slider
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Slider.propTypes";
import fallback from "./Slider.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Slider
 * Category          : inputs
 * 
 * A component that displays numeric values in a slider control.
 * 
 * This component is used for numeric values, and allows the user to slide a
 * controller to increase or decrease its value without having to enter data
 * manually.
 * 
 */
export const Slider = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Slider")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Slider",
                parameters: props
            }}
        />
    );
};


Slider.propTypes = propTypes;
Slider.defaultProps = defaultProps;
Slider.displayName = "Slider";

export default memo(Slider);
