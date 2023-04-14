/**
 * Typescript interface definition for propTypes
 */
export interface IToggleButtonsProps {
    /**
     * exclusive
     * Data Type: (boolean)
     * 
     * Specifies whether the choices are mutually exclusive.
     */
    exclusive?:                         boolean;
    /**
     * label
     * Data Type: (string)
     * 
     * The label for the ToggleButtons component.
     */
    label?:                             string;
    /**
     * options
     * Data Type: (list, object)
     * 
     * The available options for the ToggleButtons component.
     */
    options:                            any[];
    /**
     * required
     * Data Type: (boolean)
     * 
     * Specifies whether this ToggleButtons component is required.
     */
    required?:                          boolean;
}

