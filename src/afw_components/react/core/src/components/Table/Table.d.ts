// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";


/**
 *
 */
export interface ITableProps {
    /**
     * columns
     * Data Type: (list, object _AdaptiveLayoutComponentType_Table_Column)
     * 
     * The columns that are defined for this Table component.
     */
    columns:                            any[];
    /**
     * compact
     * Data Type: (boolean)
     * 
     * Indicates whether this Table compact appears compact.
     */
    compact?:                           boolean;
    /**
     * isHeaderVisible
     * Data Type: (boolean)
     * 
     * Indicates whether this Table displays column headers.
     */
    isHeaderVisible?:                   boolean;
    /**
     * rows
     * Data Type: (list)
     * 
     * The rows that are defined for this Table component.
     */
    rows:                               any[];
    /**
     * selectionMode
     * Data Type: (string)
     * 
     * The selection mode for this table (none, single, multiple).
     */
    selectionMode?:                     string;
}

/**
 *
 * A component that displays data in a table format.
 * 
 * This component arranges data into rows and columns, which can be
 * optionally sorted, selected and edited through actions.
 * 
 */
export default function Table(props: ITableProps): JSX.Element;
