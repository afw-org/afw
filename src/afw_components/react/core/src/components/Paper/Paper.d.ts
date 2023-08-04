// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface IPaperProps {
    /**
     * contains
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * Contained component on this Paper layout.
     */
    contains?:                          any;
    /**
     * elevation
     * Data Type: (integer)
     * 
     * Depth elevation appearance of the Paper surface.
     */
    elevation?:                         number;
    /**
     * square
     * Data Type: (boolean)
     * 
     * Use square corners instead of rounded.
     */
    square?:                            boolean;
}

/**
 *
 * A component that allows contents to be displayed on a Paper like surface.
 * 
 * This component allows data on its surface to stand out by providing an
 * edged, or raised outline, with shadow depth and optional corner radius
 * style.  This component is another useful container to help separate a
 * section of data from its surrounding contents.
 * 
 */
export default function Paper(props: IPaperProps): JSX.Element;
