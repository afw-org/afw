// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";


/**
 *
 */
export interface IPickerProps {
    /**
     * description
     * Data Type: (string)
     * 
     * The text description for this Picker component.
     */
    description?:                       string;
    /**
     * items
     * Data Type: (list, object _AdaptiveLayoutComponentType_Picker_Item)
     * 
     * Items to be displayed by this Picker layout.
     */
    items?:                             any[];
    /**
     * label
     * Data Type: (string)
     * 
     * The text label for this Picker component.
     */
    label?:                             string;
}

/**
 *
 * A component that allows one or more items to be picked from a list.
 * 
 * This component allows for zero or more items to be selected from a list.
 * When an item is selected, it appears as a tag in the selection, which may
 * also be later removed.
 * 
 */
export default function Picker(props: IPickerProps): JSX.Element;
