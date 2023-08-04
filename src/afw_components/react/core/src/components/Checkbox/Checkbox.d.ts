// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface ICheckboxProps {
    /**
     * boxSide
     * Data Type: (string)
     * 
     * Specifies which side the checkbox is on (start or end).
     */
    boxSide?:                           string;
    /**
     * description
     * Data Type: (string)
     * 
     * The description for the Checkbox component.
     */
    description?:                       string;
    /**
     * disabled
     * Data Type: (boolean)
     * 
     * Specifies whether the Checkbox component is disabled.
     */
    disabled?:                          boolean;
    /**
     * label
     * Data Type: (string)
     * 
     * The label for the Checkbox component.
     */
    label?:                             string;
    /**
     * value
     * Data Type: (boolean)
     * 
     * The value for the Checkbox component.
     */
    value?:                             boolean;
}

/**
 *
 * A component that displays a checkbox.
 * 
 * This component is simply a checkbox that allows the user to click to
 * enable or disable its value.
 * 
 */
export default function Checkbox(props: ICheckboxProps): JSX.Element;
