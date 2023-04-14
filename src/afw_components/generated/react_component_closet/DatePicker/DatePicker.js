// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for DatePicker
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./DatePicker.propTypes";
import fallback from "./DatePicker.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : DatePicker
 * Category          : pickers
 * 
 * A component that allows you to select a date from a calendar.
 * 
 * This component understands date values and allows the user to select a
 * date from a calendar, or enter it in the appropriate format.
 * 
 */
export const DatePicker = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "DatePicker")}
            fallback={fallback}
            layoutComponent={{
                componentType: "DatePicker",
                parameters: props
            }}
        />
    );
};


DatePicker.propTypes = propTypes;
DatePicker.defaultProps = defaultProps;
DatePicker.displayName = "DatePicker";

export default memo(DatePicker);
