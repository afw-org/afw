// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface IEditableLabelProps {
    /**
     * multiline
     * Data Type: (boolean)
     * 
     * Specifies whether the TextField for the EditableLabel needs to be
     * multilined.
     */
    multiline?:                         boolean;
    /**
     * placeholder
     * Data Type: (string)
     * 
     * The placeholder value for the text field when no value exists.
     */
    placeholder?:                       string;
    /**
     * value
     * Data Type: (string)
     * 
     * The value for the EditableLabel component.
     */
    value?:                             string;
}

/**
 *
 * A component that renders an label, which can be edited when clicked.
 * 
 * This component provides a very quick way for a label to become "editable"
 * simply by clicking on the text label.  After clicking on the label, the
 * component changes from a static label to an editable TextField component.
 * 
 */
export default function EditableLabel(props: IEditableLabelProps): JSX.Element;
