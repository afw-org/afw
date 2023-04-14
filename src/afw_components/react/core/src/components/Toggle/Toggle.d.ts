// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface IToggleProps {
    /**
     * disabled
     * Data Type: (boolean)
     * 
     * Specifies whether the Toggle component is disabled.
     */
    disabled?:                          boolean;
    /**
     * label
     * Data Type: (string)
     * 
     * The label for the Toggle component.
     */
    label?:                             string;
    /**
     * offText
     * Data Type: (string)
     * 
     * The text displayed when the Toggle component is off.
     */
    offText?:                           string;
    /**
     * onText
     * Data Type: (string)
     * 
     * The text displayed when the Toggle component is on.
     */
    onText?:                            string;
    /**
     * value
     * Data Type: (boolean)
     * 
     * The value for the Toggle component.
     */
    value?:                             boolean;
}

/**
 *
 * A component that renders a boolean value as a toggleable switch.
 * 
 * This component takes a boolean value and displays it as a switch that can
 * be toggled on or off to represent true or false.  It may also have text
 * labels to interpret its meaning.
 * 
 */
export default function Toggle(props: IToggleProps): JSX.Element;
