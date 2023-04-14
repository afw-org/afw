// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";


/**
 *
 */
export interface IMenuProps {
    /**
     * items
     * Data Type: (list, object _AdaptiveLayoutComponentType_Menu_Item)
     * 
     * Item to display under this Menu.
     */
    items?:                             any[];
    /**
     * open
     * Data Type: (boolean)
     * 
     * Specifies whether this menu is open.
     */
    open?:                              boolean;
}

/**
 *
 * A component that renders a Menu to be displayed.
 * 
 * This component pops up a Menu of items, each of which take an appropriate
 * action when clicked.  Menu items may also open up new submenus in order to
 * organize or filter a set of items in a way that's easy for the end user to
 * understand and select from.
 * 
 */
export default function Menu(props: IMenuProps): JSX.Element;
