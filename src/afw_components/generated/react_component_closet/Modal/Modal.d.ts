// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";


/**
 * Typescript interface definition for propTypes
 */
export interface IModalProps {
    /**
     * contains
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * The component to be rendered inside the Modal.
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
 * This component container is visible when opened, displaying its content in
 * a popup that's centered over the rest of the page. It can optionally be
 * marked blocking, so the user must take an action before it can be
 * dismissed.
 * 
 */
export default function Modal(props: IModalProps): JSX.Element;
