// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";


/**
 *
 */
export interface IListProps {
    /**
     * contains
     * Data Type: (list, object _AdaptiveLayoutComponentType_)
     * 
     * The layout components that are contained by this List component.
     */
    contains?:                          any[];
    /**
     * selectable
     * Data Type: (boolean)
     * 
     * Can the List items be selected?
     */
    selectable?:                        boolean;
}

/**
 *
 * A layout container that renders a list of components.
 * 
 * This component is a simple container component that renders its contents
 * in a sequential, scrollable list.
 * 
 */
export default function List(props: IListProps): JSX.Element;
