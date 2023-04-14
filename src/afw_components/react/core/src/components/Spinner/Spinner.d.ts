// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface ISpinnerProps {
    /**
     * label
     * Data Type: (string)
     * 
     * The label for the Spinner component.
     */
    label?:                             string;
    /**
     * size
     * Data Type: (string)
     * 
     * The size of the Spinner component.
     */
    size?:                              string;
}

/**
 *
 * A component that displays a spinner progress indicator.
 * 
 * This utility component displays a spinning circle to indicate to the user
 * to wait for processing to complete.
 * 
 */
export default function Spinner(props: ISpinnerProps): JSX.Element;
