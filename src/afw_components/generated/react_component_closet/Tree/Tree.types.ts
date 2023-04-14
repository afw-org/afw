
/**
 * Typescript interface definition for propTypes
 */
export interface ITreeProps {
    /**
     * children
     * Data Type: (list, object _AdaptiveLayoutComponentType_Tree_Node)
     * 
     * The children of this Tree.
     */
    children:                           any[];
    /**
     * dense
     * Data Type: (boolean)
     * 
     * The tree nodes should be condensed in space.
     */
    dense?:                             boolean;
    /**
     * expandedKeys
     * Data Type: (list, string)
     * 
     * The keys of nodes in this tree that are expanded.
     */
    expandedKeys?:                      string[];
    /**
     * selectedKeys
     * Data Type: (list, string)
     * 
     * The keys of nodes in this tree that are selected.
     */
    selectedKeys?:                      string[];
}

