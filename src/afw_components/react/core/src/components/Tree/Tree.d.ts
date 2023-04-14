// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";


/**
 *
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

/**
 *
 * A component that renders data in a Tree structure.
 * 
 * This component helps to organize content into a Tree structure, where
 * nodes can be selected, expanded, or collapsed to allow the user to
 * navigate more complex nested data or component structures.
 * 
 */
export default function Tree(props: ITreeProps): JSX.Element;
