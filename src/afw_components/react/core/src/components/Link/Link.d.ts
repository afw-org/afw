// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface ILinkProps {
    /**
     * disabled
     * Data Type: (boolean)
     * 
     * Specifies whether the the Link is disabled.
     */
    disabled?:                          boolean;
    /**
     * text
     * Data Type: (string)
     * 
     * The text value displayed for the Link component.
     */
    text?:                              string;
    /**
     * url
     * Data Type: (string)
     * 
     * The URL for the link.
     */
    url?:                               string;
}

/**
 *
 * A component that renders a url link.
 * 
 * This component understands how to render a link with the appropriate
 * framework style, and handles the React Routing, if the link references
 * another component within this same application.
 * 
 */
export default function Link(props: ILinkProps): JSX.Element;
