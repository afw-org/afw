/**
 * Typescript interface definition for propTypes
 */
export interface IDatePickerProps {
    /**
     * description
     * Data Type: (string)
     * 
     * The description for the DatePicker component.
     */
    description?:                       string;
    /**
     * disabled
     * Data Type: (boolean)
     * 
     * Specifies whether the DatePicker component is disabled.
     */
    disabled?:                          boolean;
    /**
     * label
     * Data Type: (string)
     * 
     * The label for the DatePicker component.
     */
    label?:                             string;
    /**
     * required
     * Data Type: (boolean)
     * 
     * Specifies whether the Date Picker value is required.
     */
    required?:                          boolean;
    /**
     * value
     * Data Type: (dateTime)
     * 
     * The value for the DatePicker component.
     */
    value?:                             string;
}

