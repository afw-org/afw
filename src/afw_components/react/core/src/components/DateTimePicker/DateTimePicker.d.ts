// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface IDateTimePickerProps {
    /**
     * description
     * Data Type: (string)
     * 
     * The description for the DateTimePicker component.
     */
    description?:                       string;
    /**
     * disabled
     * Data Type: (boolean)
     * 
     * Specifies whether the DateTimePicker component is disabled.
     */
    disabled?:                          boolean;
    /**
     * label
     * Data Type: (string)
     * 
     * The label for the DateTimePicker component.
     */
    label?:                             string;
    /**
     * required
     * Data Type: (boolean)
     * 
     * Specifies whether the Date Picker value is required.
     */
    required?:                          boolean;
    /**
     * value
     * Data Type: (dateTime)
     * 
     * The value for the DateTimePicker component.
     */
    value?:                             string;
}

/**
 *
 * A component that allows you to select a date and time.
 * 
 * This component understands both dates and time formats, which allows the
 * user to select a date and time from a calendar and clock for higher date
 * precision.
 * 
 */
export default function DateTimePicker(props: IDateTimePickerProps): JSX.Element;
