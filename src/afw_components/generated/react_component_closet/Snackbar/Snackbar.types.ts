/**
 * Typescript interface definition for propTypes
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

