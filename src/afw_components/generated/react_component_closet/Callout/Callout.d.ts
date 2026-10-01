// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";



/**
 * Typescript interface definition for propTypes
 */
export interface ICalloutProps {
    /**
     * contains
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * The component to be rendered inside the Callout.
     */
    contains?:                          any;
    /**
     * gapSpace
     * Data Type: (integer)
     * 
     * The gap space between target and the Callout component.
     */
    gapSpace?:                          number;
    /**
     * open
     * Data Type: (boolean)
     * 
     * Specifies whether this Callout component is open.
     */
    open:                               boolean;
    /**
     * targetPosition
     * Data Type: (object, _AdaptiveLayoutComponentType_Callout_TargetPosition)
     * 
     * Specifies the position of the callout, relative to the target
     * component.
     */
    targetPosition?:                    any;
}

/**
 *
 * A layout container that displays content inside a popup.
 * 
 * This component container is visible when a target element is clicked. It's
 * used to quickly show or grab input from the user in a popup area and can be
 * dismissed.
 * 
 */
export default function Callout(props: ICalloutProps): React.JSX.Element;
