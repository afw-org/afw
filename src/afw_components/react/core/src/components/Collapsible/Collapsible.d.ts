// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface ICollapsibleProps {
    /**
     * contains
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * Specifies the component contained within this Collapsible Body.
     */
    contains?:                          any;
    /**
     * headerItems
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * Specifies the component contained within this Collapsible Header.
     */
    headerItems?:                       any;
    /**
     * isCollapsed
     * Data Type: (boolean)
     * 
     * Specifies whether the Collapsible content is collapsed or not.
     */
    isCollapsed?:                       boolean;
    /**
     * showCollapse
     * Data Type: (boolean)
     * 
     * Specifies whether the Collapsible should render the collapse feature.
     */
    showCollapse?:                      boolean;
}

/**
 *
 * A layout container that displays content that can be hidden by a
 * collapsible control.
 * 
 * This component helps organize data into a group, which can be temporarily
 * concealed or revealed by clicking on the collapse or expand icons.  This
 * may be important for preserving real estate on the screen, or reducing the
 * amount of data that may be present at a given time, to avoid overloading
 * the end user with information.
 * 
 */
export default function Collapsible(props: ICollapsibleProps): JSX.Element;
