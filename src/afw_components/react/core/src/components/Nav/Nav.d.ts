// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";


/**
 *
 */
export interface INavProps {
    /**
     * items
     * Data Type: (list, object _AdaptiveLayoutComponentType_Nav_Item)
     * 
     * Item to be displayed by this Nav layout.
     */
    items?:                             any[];
}

/**
 *
 * A navigational component that displays a list of links.
 * 
 * This component, often referred to as a Side Nav, displays a list of
 * clickable links into a horizontal panel.
 * 
 */
export default function Nav(props: INavProps): JSX.Element;
