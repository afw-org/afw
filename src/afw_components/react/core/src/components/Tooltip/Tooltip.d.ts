// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface ITooltipProps {
    /**
     * contains
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * Content that can be rendered inside this tooltip display.
     */
    contains?:                          any;
    /**
     * content
     * Data Type: (string)
     * 
     * The content to display in the Tooltip.
     */
    content?:                           string;
    /**
     * delay
     * Data Type: (integer)
     * 
     * The time delay, in milliseconds, before displaying tooltip.
     */
    delay?:                             number;
    /**
     * target
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * The target to wrap inside this tooltip.
     */
    target?:                            any;
}

/**
 *
 * A component that surrounds content with a tooltip.
 * 
 * This component renders a small popup with text when the user hovers over
 * an element for a specific amount of time.  This may provide helpful hints
 * to explain to the user what they are looking at.
 * 
 */
export default function Tooltip(props: ITooltipProps): JSX.Element;
