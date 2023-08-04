// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";

/**
 *
 */
export interface IBreadcrumbProps {
    /**
     * items
     * Data Type: (list, _AdaptiveLayoutComponentType_BreadcrumbItem)
     * 
     * Breadcrumb items
     * 
     * Breadcrumb items
     */
    items:                              any[];
}

/**
 *
 * A component that renders a Breadcrumb path with links
 * 
 * This component renders a path of url links that the user can click to
 * navigate at any point within an application.
 * 
 */
export default function Breadcrumb(props: IBreadcrumbProps): JSX.Element;
