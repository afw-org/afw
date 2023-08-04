// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for DateTimePicker
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./DateTimePicker.propTypes";
import fallback from "./DateTimePicker.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : DateTimePicker
 * Category          : pickers
 * 
 * A component that allows you to select a date and time.
 * 
 * This component understands both dates and time formats, which allows the
 * user to select a date and time from a calendar and clock for higher date
 * precision.
 * 
 */
export const DateTimePicker = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "DateTimePicker")}
            fallback={fallback}
            layoutComponent={{
                componentType: "DateTimePicker",
                parameters: props
            }}
        />
    );
};


DateTimePicker.propTypes = propTypes;
DateTimePicker.defaultProps = defaultProps;
DateTimePicker.displayName = "DateTimePicker";

export default memo(DateTimePicker);
