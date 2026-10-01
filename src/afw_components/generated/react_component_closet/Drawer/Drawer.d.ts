// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";



/**
 * Typescript interface definition for propTypes
 */
export interface IDrawerProps {
    /**
     * anchor
     * Data Type: (string)
     * 
     * Side that this drawer is anchored to
     * 
     * This property describes which side of the page, the drawer should be
     * anchored to. It may be one of 'bottom', 'top', 'left' or 'right'.
     */
    anchor?:                            string;
    /**
     * contains
     * Data Type: (array, object _AdaptiveLayoutComponentType_DrawerItem)
     * 
     * Items inside of the Drawer
     * 
     * This declares a list of items to be rendered inside the Drawer.
     */
    contains?:                          any[];
    /**
     * open
     * Data Type: (boolean)
     * 
     * Specifies whether this Drawer component is open.
     */
    open:                               boolean;
    /**
     * variant
     * Data Type: (string)
     * 
     * The variant to use
     * 
     * This property describes whether the drawer should be anchored
     * temporary, persistent or permanent. If temporary, the drawer will be
     * dismissed when the user unblocks its focus. If persistent, the drawer
     * will be displayed until the user manually closes it. If permanent, the
     * drawer will always remain open.
     */
    variant?:                           string;
}

/**
 *
 * A component that renders content inside a side sheet anchored to one of the
 * edges.
 * 
 * This component, which contains other content, is anchored to one of the
 * edges of the main window. It's often animated to slide into view when an
 * action occurs, but can be optionally docked, or permanent.
 * 
 */
export default function Drawer(props: IDrawerProps): JSX.Element;
