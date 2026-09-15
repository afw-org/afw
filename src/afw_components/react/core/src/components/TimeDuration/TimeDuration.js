// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for TimeDuration
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./TimeDuration.propTypes";
import fallback from "./TimeDuration.fallback";

import {AdaptiveComponent} from "../AdaptiveComponent";
import {ctx} from "../../utils/utils";


/**
 * Implementation Id : TimeDuration
 * Category          : inputs
 * 
 * A component that renders a time duration.
 * 
 * This component understands a time duration data type and renders a
 * convenient way to select a duration of time.
 * 
 */
export const TimeDuration = (props) => {

    props = {...defaultProps, ...props};

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "TimeDuration")}
            fallback={fallback}
            layoutComponent={{
                componentType: "TimeDuration",
                parameters: props
            }}
        />
    );
};


TimeDuration.propTypes = propTypes;
TimeDuration.displayName = "TimeDuration";

export default memo(TimeDuration);
