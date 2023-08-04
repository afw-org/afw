// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface ITextFieldProps {
    /**
     * description
     * Data Type: (string)
     * 
     * The description for the TextField component that describes this input.
     */
    description?:                       string;
    /**
     * disabled
     * Data Type: (boolean)
     * 
     * Specifies whether the the TextField is disabled.
     */
    disabled?:                          boolean;
    /**
     * label
     * Data Type: (string)
     * 
     * The label for the TextField component.
     */
    label?:                             string;
    /**
     * multiline
     * Data Type: (boolean)
     * 
     * Specifies whether this TextField component is multilined.
     */
    multiline?:                         boolean;
    /**
     * placeholder
     * Data Type: (string)
     * 
     * The placeholder text for this component.
     */
    placeholder?:                       string;
    /**
     * required
     * Data Type: (boolean)
     * 
     * Specifies whether this TextField component is required.
     */
    required?:                          boolean;
    /**
     * rows
     * Data Type: (integer)
     * 
     * Specifies the number of rows to display.
     */
    rows?:                              number;
    /**
     * value
     * Data Type: (string)
     * 
     * The value of the TextField component.
     */
    value?:                             string;
}

/**
 *
 * A component that renders a text field with a label and description.
 * 
 * This is a simple component for inputting text.  It may display an outlined
 * or underlined field from which the user can enter text into.  It may also
 * display a label and description to help the user understand its contents.
 * 
 */
export default function TextField(props: ITextFieldProps): JSX.Element;
