
/**
 * Typescript interface definition for propTypes
 */
export interface ITableProps {
    /**
     * columns
     * Data Type: (array, object _AdaptiveLayoutComponentType_Table_Column)
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
     * Data Type: (array)
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

