// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface INativeProps {
    /**
     * tag
     * Data Type: (string)
     * 
     * The HTML Tag name.
     */
    tag?:                               string;
}

/**
 *
 * A component that displays a native HTML element.
 * 
 * This component represents any additional native HTML node that is required
 * by the end user, which is not covered by other components within this
 * library.
 * 
 */
export default function Native(props: INativeProps): JSX.Element;
