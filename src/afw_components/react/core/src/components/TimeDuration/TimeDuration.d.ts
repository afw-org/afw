// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface ITimeDurationProps {
    /**
     * dayTimeDuration
     * Data Type: (string)
     * 
     * The value of the TimeDuration component expressed as a dayTime.
     */
    dayTimeDuration?:                   string;
    /**
     * description
     * Data Type: (string)
     * 
     * The description for the TimeDuration component that describes this
     * input.
     */
    description?:                       string;
    /**
     * disabled
     * Data Type: (boolean)
     * 
     * Specifies whether the the TimeDuration is disabled.
     */
    disabled?:                          boolean;
    /**
     * label
     * Data Type: (string)
     * 
     * The label for the TimeDuration component.
     */
    label?:                             string;
    /**
     * required
     * Data Type: (boolean)
     * 
     * Specifies whether the the TimeDuration is required.
     */
    required?:                          boolean;
    /**
     * yearMonthDuration
     * Data Type: (string)
     * 
     * The value of the TimeDuration component expressed as a yearMonth.
     */
    yearMonthDuration?:                 string;
}

/**
 *
 * A component that renders a time duration.
 * 
 * This component understands a time duration data type and renders a
 * convenient way to select a duration of time.
 * 
 */
export default function TimeDuration(props: ITimeDurationProps): JSX.Element;
