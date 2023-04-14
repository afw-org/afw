/**
 * Typescript interface definition for propTypes
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

