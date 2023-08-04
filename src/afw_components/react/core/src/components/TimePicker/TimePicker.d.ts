// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface ITimePickerProps {
    /**
     * description
     * Data Type: (string)
     * 
     * The text description for this TimePicker component.
     */
    description?:                       string;
    /**
     * label
     * Data Type: (string)
     * 
     * The text label for this TimePicker component.
     */
    label?:                             string;
}

/**
 *
 * A component that allows a time to be chosen.
 * 
 * This component allows a time to be chosen, typically from a clock or spin
 * buttons.
 * 
 */
export default function TimePicker(props: ITimePickerProps): JSX.Element;
