
/**
 * Typescript interface definition for propTypes
 */
export interface IChoiceGroupProps {
    /**
     * label
     * Data Type: (string)
     * 
     * The label for the ChoiceGroup component.
     */
    label?:                             string;
    /**
     * options
     * Data Type: (array, object _AdaptiveLayoutComponentType_ChoiceGroup_Option)
     * 
     * The available options for the ChoiceGroup component.
     */
    options:                            any[];
    /**
     * orientation
     * Data Type: (string)
     * 
     * The orientation of the Choice Group (horizontal or vertical).
     */
    orientation?:                       string;
    /**
     * required
     * Data Type: (boolean)
     * 
     * Specifies whether this ChoiceGroup component is required.
     */
    required?:                          boolean;
    /**
     * selectedKey
     * Data Type: (string)
     * 
     * The value of the selected key for the ChoiceGroup options.
     */
    selectedKey?:                       string;
}

