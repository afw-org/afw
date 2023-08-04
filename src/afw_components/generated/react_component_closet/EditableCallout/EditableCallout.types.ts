/**
 * Typescript interface definition for propTypes
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

