/**
 * Typescript interface definition for propTypes
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

