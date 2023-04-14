// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";


/**
 *
 */
export interface ITabsProps {
    /**
     * gapSpace
     * Data Type: (integer)
     * 
     * The amount of space between the tabs and tabbed content.
     */
    gapSpace?:                          number;
    /**
     * tabs
     * Data Type: (list, object _AdaptiveLayoutComponentType_Tabs_Tab)
     * 
     * The tabs that are defined for this Tabs component.
     */
    tabs?:                              any[];
}

/**
 *
 * A layout container that renders a series of tabs.
 * 
 * This component displays a list of components, one at a time, by containing
 * the content inside of their individual tabs.  A tab has a clickable link,
 * which when clicked, will switch the content to the desired component.
 * 
 */
export default function Tabs(props: ITabsProps): JSX.Element;
