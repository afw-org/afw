// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface IMessageProps {
    /**
     * canDismiss
     * Data Type: (boolean)
     * 
     * Specifies whether the Message can be dismissed.
     */
    canDismiss?:                        boolean;
    /**
     * contains
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * Optional components to be displayed for the Message component.
     */
    contains?:                          any;
    /**
     * isMultiline
     * Data Type: (boolean)
     * 
     * Specifies whether the Message message is multilined.
     */
    isMultiline?:                       boolean;
    /**
     * message
     * Data Type: (string)
     * 
     * Optional message to be displayed for the Message component.
     */
    message?:                           string;
    /**
     * status
     * Data Type: (string)
     * 
     * Status of message (error, info, warning).
     */
    status?:                            string;
}

/**
 *
 * A component that renders a message to be displayed.
 * 
 * This component displays a text message within a surface that may alert the
 * user to additional information, errors or warnings.
 * 
 */
export default function Message(props: IMessageProps): JSX.Element;
