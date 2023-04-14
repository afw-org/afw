// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";


/**
 *
 */
export interface IToolbarProps {
    /**
     * alignContent
     * Data Type: (string)
     * 
     * Defines the align-content style property.
     */
    alignContent?:                      string;
    /**
     * alignItems
     * Data Type: (string)
     * 
     * Defines the align-items style property.
     */
    alignItems?:                        string;
    /**
     * contains
     * Data Type: (list, object _AdaptiveLayoutComponentType_)
     * 
     * A list of components to be contained by this Toolbar.
     */
    contains:                           any[];
    /**
     * dense
     * Data Type: (boolean)
     * 
     * Specifies whether the Toolbar should be condensed as much as possible.
     */
    dense?:                             boolean;
    /**
     * justify
     * Data Type: (string)
     * 
     * Defines the justify-content style property.
     */
    justify?:                           string;
    /**
     * spacing
     * Data Type: (integer)
     * 
     * The spacing between items in the Toolbar.
     */
    spacing?:                           number;
}

/**
 *
 * A container component that arranges a series of buttons or other minor
 * content in a horizontal bar.
 * 
 * This component is used to provide actions, via buttons, labels or other
 * small components, condensed into a common location so the user may use it
 * to view or change data that is presented in the layout below.
 * 
 */
export default function Toolbar(props: IToolbarProps): JSX.Element;
