// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
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

/**
 *
 * A component to display a selectable choice of options, presented as
 * buttons that can be toggled on or off.
 * 
 * This component provides a list of options, organized as buttons, that may
 * be toggled on or off.  They may be inclusive or exclusive.
 * 
 */
export default function ToggleButtons(props: IToggleButtonsProps): JSX.Element;
