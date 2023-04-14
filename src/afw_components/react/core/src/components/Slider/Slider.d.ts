// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface ISliderProps {
    /**
     * disabled
     * Data Type: (boolean)
     * 
     * Specifies whether the Slider component is disabled.
     */
    disabled?:                          boolean;
    /**
     * label
     * Data Type: (string)
     * 
     * The label for the Slider component.
     */
    label?:                             string;
    /**
     * max
     * Data Type: (integer)
     * 
     * The maximum value for the Slider component.
     */
    max?:                               number;
    /**
     * min
     * Data Type: (integer)
     * 
     * The minimum value for the Slider component.
     */
    min?:                               number;
    /**
     * showNumber
     * Data Type: (boolean)
     * 
     * Indicates whether the value of the Slider should be visible.
     */
    showNumber?:                        boolean;
    /**
     * step
     * Data Type: (integer)
     * 
     * The incremental step value for the Slider component.
     */
    step?:                              number;
    /**
     * value
     * Data Type: (integer)
     * 
     * The value for the Slider component.
     */
    value?:                             number;
    /**
     * vertical
     * Data Type: (boolean)
     * 
     * Indicates whether the Slider component is vertical oriented.
     */
    vertical?:                          boolean;
}

/**
 *
 * A component that displays numeric values in a slider control.
 * 
 * This component is used for numeric values, and allows the user to slide a
 * controller to increase or decrease its value without having to enter data
 * manually.
 * 
 */
export default function Slider(props: ISliderProps): JSX.Element;
