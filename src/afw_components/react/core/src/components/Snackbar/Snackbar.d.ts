// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface ISnackbarProps {
    /**
     * duration
     * Data Type: (integer)
     * 
     * The amount of time, in milliseconds to display the Snackbar.
     */
    duration?:                          number;
    /**
     * message
     * Data Type: (string)
     * 
     * The message to be displayed in the Snackbar component.
     */
    message?:                           string;
    /**
     * open
     * Data Type: (boolean)
     * 
     * Specifies whether the Snackbar is open or not.
     */
    open?:                              boolean;
    /**
     * type
     * Data Type: (string)
     * 
     * The type of message for the Snackbar component (error, warning, info,
     * success).
     */
    type?:                              string;
}

/**
 *
 * A component that renders a Snackbar to be displayed.
 * 
 * This component pops up a small bar at the bottom of the screen to alert
 * the user of a recent activity, such as a success or error status,
 * typically following an action that was taken by the user.
 * 
 */
export default function Snackbar(props: ISnackbarProps): JSX.Element;
