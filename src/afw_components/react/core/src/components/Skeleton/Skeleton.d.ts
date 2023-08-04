// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface ISkeletonProps {
    /**
     * height
     * Data Type: (None)
     * 
     * The CSS height of the skeleton.
     */
    height?:                            any;
    /**
     * type
     * Data Type: (string)
     * 
     * The shape of the skeleton to display (line or circle).
     */
    type?:                              string;
    /**
     * width
     * Data Type: (None)
     * 
     * The CSS width of the skeleton.
     */
    width?:                             any;
}

/**
 *
 * A component that displays a skeleton, or shape, of a component that may
 * shimmer to indicate that an actual component is waiting to be loaded.
 * 
 * The Skeleton is a feedback utility component, similar to the Spinner, but
 * it takes on a shape that gives the user the perspective that data is
 * nearly ready and the component is attempting to render.  It's a component
 * placeholder, which often animates or shimmers.
 * 
 */
export default function Skeleton(props: ISkeletonProps): JSX.Element;
