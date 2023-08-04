// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface IModalProps {
    /**
     * contains
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * Specifies the placeholder text displayed when no value is present.
     */
    contains?:                          any;
    /**
     * isBlocking
     * Data Type: (boolean)
     * 
     * Specifies whether this Modal component is blocking or can lightly be
     * dismissed.
     */
    isBlocking?:                        boolean;
    /**
     * open
     * Data Type: (boolean)
     * 
     * Specifies whether this Modal component is open.
     */
    open:                               boolean;
}

/**
 *
 * A layout container that displays content inside a popup.
 * 
 * This component container is visible when a target element is clicked.
 * It's used to quickly show or grab input from the user in a popup area and
 * can be dismissed.
 * 
 */
export default function Modal(props: IModalProps): JSX.Element;
