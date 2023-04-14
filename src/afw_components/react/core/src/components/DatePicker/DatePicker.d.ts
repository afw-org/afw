// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface IDatePickerProps {
    /**
     * description
     * Data Type: (string)
     * 
     * The description for the DatePicker component.
     */
    description?:                       string;
    /**
     * disabled
     * Data Type: (boolean)
     * 
     * Specifies whether the DatePicker component is disabled.
     */
    disabled?:                          boolean;
    /**
     * label
     * Data Type: (string)
     * 
     * The label for the DatePicker component.
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
     * The value for the DatePicker component.
     */
    value?:                             string;
}

/**
 *
 * A component that allows you to select a date from a calendar.
 * 
 * This component understands date values and allows the user to select a
 * date from a calendar, or enter it in the appropriate format.
 * 
 */
export default function DatePicker(props: IDatePickerProps): JSX.Element;
