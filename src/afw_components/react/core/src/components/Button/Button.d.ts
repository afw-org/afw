// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface IButtonProps {
    /**
     * color
     * Data Type: (string)
     * 
     * Specifies an optional color for this Button (primary, secondary,
     * error).
     */
    color?:                             string;
    /**
     * disabled
     * Data Type: (boolean)
     * 
     * Specifies whether this Button component is disabled.
     */
    disabled?:                          boolean;
    /**
     * dropdown
     * Data Type: (boolean)
     * 
     * Specifies an optional dropdown icon for this Button.
     */
    dropdown?:                          boolean;
    /**
     * icon
     * Data Type: (string)
     * 
     * Specifies an optional icon for this Button.
     */
    icon?:                              string;
    /**
     * label
     * Data Type: (string)
     * 
     * Specifies the text on the Button.
     */
    label?:                             string;
    /**
     * secondaryText
     * Data Type: (string)
     * 
     * Specifies optional text to be added below the main text label.
     */
    secondaryText?:                     string;
    /**
     * size
     * Data Type: (string)
     * 
     * Specifies a size for the button (small)
     */
    size?:                              string;
    /**
     * type
     * Data Type: (string)
     * 
     * The type of Button (icon, split, compound, default).
     */
    type?:                              string;
}

/**
 *
 * Component to display a Button.
 * 
 * This component encapsulates a wide range of Button features, including
 * labels, icons and colors.  The Button component may even allow for a
 * dropdown of selected actions to perform when clicked.
 * 
 */
export default function Button(props: IButtonProps): JSX.Element;
