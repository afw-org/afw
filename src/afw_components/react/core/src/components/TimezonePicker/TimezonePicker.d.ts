// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface ITimezonePickerProps {
    /**
     * description
     * Data Type: (string)
     * 
     * The text description for this TimezonePicker component.
     */
    description?:                       string;
    /**
     * label
     * Data Type: (string)
     * 
     * The text label for this TimezonePicker component.
     */
    label?:                             string;
}

/**
 *
 * A component that allows a timezone to be chosen.
 * 
 * This component allows a timezone to be chosen, typically from a dropdown
 * of timezones, or from a world map.
 * 
 */
export default function TimezonePicker(props: ITimezonePickerProps): JSX.Element;
