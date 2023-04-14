
/**
 * Typescript interface definition for propTypes
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

