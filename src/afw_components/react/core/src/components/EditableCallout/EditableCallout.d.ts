// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface IEditableCalloutProps {
    /**
     * allowWrite
     * Data Type: (boolean)
     * 
     * Specifies whether the EditableCallout component is writable.
     */
    allowWrite?:                        boolean;
    /**
     * disabled
     * Data Type: (boolean)
     * 
     * Specifies whether the EditableCallout component is disabled.
     */
    disabled?:                          boolean;
    /**
     * open
     * Data Type: (boolean)
     * 
     * Specifies whether the EditableCallout component is visible.
     */
    open:                               boolean;
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
     * The value for the EditableCallout component.
     */
    value?:                             string;
}

/**
 *
 * A component that calls out with an editable multiline text area.
 * 
 * This component wraps an editable text field inside a Callout component.
 * This is useful when a label needs to be quickly edited by popping up an
 * editor, rather than navigating them to a new page.  The EditableLabel
 * component that it displays may be a multiline text field.
 * 
 */
export default function EditableCallout(props: IEditableCalloutProps): JSX.Element;
