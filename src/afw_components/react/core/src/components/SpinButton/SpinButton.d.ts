// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface ISpinButtonProps {
    /**
     * disabled
     * Data Type: (boolean)
     * 
     * Specifies whether the SpinButton component is disabled.
     */
    disabled?:                          boolean;
    /**
     * label
     * Data Type: (string)
     * 
     * The label for the SpinButton component.
     */
    label?:                             string;
    /**
     * max
     * Data Type: (integer)
     * 
     * The maximum value for the SpinButton component.
     */
    max?:                               number;
    /**
     * min
     * Data Type: (integer)
     * 
     * The minimum value for the SpinButton component.
     */
    min?:                               number;
    /**
     * step
     * Data Type: (integer)
     * 
     * The incremental step value for the SpinButton component.
     */
    step?:                              number;
    /**
     * value
     * Data Type: (integer)
     * 
     * The value for the SpinButton component.
     */
    value?:                             number;
}

/**
 *
 * A component that renders a numeric field that an be incremented or
 * decremented.
 * 
 * A component that renders a numeric field that an be incremented or
 * decremented.
 * 
 */
export default function SpinButton(props: ISpinButtonProps): JSX.Element;
