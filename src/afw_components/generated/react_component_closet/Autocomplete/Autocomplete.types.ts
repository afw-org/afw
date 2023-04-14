
/**
 * Typescript interface definition for propTypes
 */
export interface IAutocompleteProps {
    /**
     * allowWrite
     * Data Type: (boolean)
     * 
     * Specifies whether the Autocomplete component can be changed.
     */
    allowWrite?:                        boolean;
    /**
     * disabled
     * Data Type: (boolean)
     * 
     * Specifies whether the Autocomplete component is disabled.
     */
    disabled?:                          boolean;
    /**
     * isLoading
     * Data Type: (boolean)
     * 
     * Specifies whether the Autocomplete component is currently loading.
     */
    isLoading?:                         boolean;
    /**
     * label
     * Data Type: (string)
     * 
     * The label for the Autocomplete component.
     */
    label?:                             string;
    /**
     * options
     * Data Type: (list, object _AdaptiveLayoutComponentType_Autocomplete_Option)
     * 
     * The available options for the Autocomplete component.
     */
    options?:                           any[];
    /**
     * placeholder
     * Data Type: (string)
     * 
     * Specifies the placeholder text displayed when no value is present.
     */
    placeholder?:                       string;
    /**
     * required
     * Data Type: (boolean)
     * 
     * Specifies whether this Autocomplete component is required.
     */
    required?:                          boolean;
    /**
     * value
     * Data Type: (object, _AdaptiveLayoutComponentType_Autocomplete_Option)
     * 
     * The option value for the Autocomplete component.
     */
    value?:                             any;
}

