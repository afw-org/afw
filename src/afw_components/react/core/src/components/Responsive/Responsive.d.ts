// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";


/**
 *
 */
export interface IResponsiveProps {
    /**
     * alignContent
     * Data Type: (string)
     * 
     * Align content.
     */
    alignContent?:                      string;
    /**
     * alignItems
     * Data Type: (string)
     * 
     * Align items.
     */
    alignItems?:                        string;
    /**
     * justify
     * Data Type: (string)
     * 
     * Justify content.
     */
    justify?:                           string;
    /**
     * rows
     * Data Type: (list, object _AdaptiveLayoutComponentType_ResponsiveRow)
     * 
     * The layout row components that are contained by this Responsive.
     */
    rows:                               any[];
    /**
     * spacing
     * Data Type: (integer)
     * 
     * Spacing between rows.
     */
    spacing?:                           number;
}

/**
 *
 * A layout container that renders content in a grid format, with breakpoints
 * defined for a variety of screen sizes.
 * 
 * This container component defines a Row/Column grid of components to
 * display content horizontally and vertically, depending on the screen size.
 * For larger displays, you may assign breakpoints that allow for all
 * content to fit, while on smaller mobile devices, content will be divided
 * up into chunks.
 * 
 */
export default function Responsive(props: IResponsiveProps): JSX.Element;
