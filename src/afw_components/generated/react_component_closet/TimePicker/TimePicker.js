// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for TimePicker
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./TimePicker.propTypes";
import fallback from "./TimePicker.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : TimePicker
 * Category          : pickers
 * 
 * A component that allows a time to be chosen.
 * 
 * This component allows a time to be chosen, typically from a clock or spin
 * buttons.
 * 
 */
export const TimePicker = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "TimePicker")}
            fallback={fallback}
            layoutComponent={{
                componentType: "TimePicker",
                parameters: props
            }}
        />
    );
};


TimePicker.propTypes = propTypes;
TimePicker.defaultProps = defaultProps;
TimePicker.displayName = "TimePicker";

export default memo(TimePicker);
