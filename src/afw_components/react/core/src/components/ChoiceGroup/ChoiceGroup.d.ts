// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";


/**
 *
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
     * Data Type: (list, object _AdaptiveLayoutComponentType_ChoiceGroup_Option)
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

/**
 *
 * A component to display a selectable choice of options.
 * 
 * This component provides a mutually exclusive list of options, organized as
 * radio buttons, that may be selected.
 * 
 */
export default function ChoiceGroup(props: IChoiceGroupProps): JSX.Element;
