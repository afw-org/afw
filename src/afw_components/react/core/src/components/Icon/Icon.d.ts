// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface IIconProps {
    /**
     * iconName
     * Data Type: (string)
     * 
     * The name of the icon to render.
     */
    iconName?:                          string;
}

/**
 *
 * A component that displays an icon.
 * 
 * This component displays an icon, which references a common dictionary of
 * icon names, each of which may map to the appropriate icon library
 * depending on the framework that is being used.  The icon itself may be
 * clickable.
 * 
 */
export default function Icon(props: IIconProps): JSX.Element;
