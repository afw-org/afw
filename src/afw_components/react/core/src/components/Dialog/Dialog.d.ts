// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";


/**
 *
 */
export interface IDialogProps {
    /**
     * contains
     * Data Type: (object, _AdaptiveLayoutComponent_)
     * 
     * Any contents that needs to be rendered inside the Dialog.
     */
    contains?:                          any;
    /**
     * footer
     * Data Type: (object, _AdaptiveLayoutComponent_)
     * 
     * Any contents that needs to be rendered inside the Dialog's footer
     * area.
     */
    footer?:                            any;
    /**
     * fullScreen
     * Data Type: (boolean)
     * 
     * Should the Dialog be full screen?
     */
    fullScreen?:                        boolean;
    /**
     * isBlocking
     * Data Type: (boolean)
     * 
     * Determines if the Dialog should block dismissing until the user clicks
     * a button.
     */
    isBlocking?:                        boolean;
    /**
     * maxWidth
     * Data Type: (string)
     * 
     * The maximum width of the Dialog.
     */
    maxWidth?:                          string;
    /**
     * open
     * Data Type: (boolean)
     * 
     * Specifies whether this Dialog component is open.
     */
    open?:                              boolean;
    /**
     * showClose
     * Data Type: (boolean)
     * 
     * Should a close button be rendered, to allow the user to dismiss the
     * Dialog.
     */
    showClose?:                         boolean;
    /**
     * subText
     * Data Type: (string)
     * 
     * The content text to display inside the dialog box, directly under the
     * Title.
     */
    subText?:                           string;
    /**
     * title
     * Data Type: (string)
     * 
     * The title text to display at the top of the dialog box.
     */
    title?:                             string;
}

/**
 *
 * A layout container that displays content inside a popup, with a
 * confirmation response.
 * 
 * This component container is visible when a target element is clicked.
 * It's used to quickly show or grab input from the user in a popup area and
 * can be dismissed by confirming or cancelling via buttons.
 * 
 */
export default function Dialog(props: IDialogProps): JSX.Element;
