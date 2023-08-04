// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";


/**
 *
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

/**
 *
 * Component to display a list of options that may be selected or can be
 * autocompleted by typing.
 * 
 * This component provides a dropdown picker with possible values to quickly
 * select from.  By typing, it will narrow down the selection to make the
 * list even shorter to choose a selection from.  Additionally, it will offer
 * the user the ability to enter a new value, if allowed.
 * 
 */
export default function Autocomplete(props: IAutocompleteProps): JSX.Element;
