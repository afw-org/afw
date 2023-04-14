// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for TimeDuration
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./TimeDuration.propTypes";
import fallback from "./TimeDuration.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


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
TimeDuration.defaultProps = defaultProps;
TimeDuration.displayName = "TimeDuration";

export default memo(TimeDuration);
