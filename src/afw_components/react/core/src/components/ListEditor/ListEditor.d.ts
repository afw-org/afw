// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";


/**
 *
 */
export interface IListEditorProps {
    /**
     * editable
     * Data Type: (boolean)
     * 
     * Specifies whether this list is currently editable.
     */
    editable?:                          boolean;
    /**
     * items
     * Data Type: (list, object _AdaptiveLayoutComponentType_ListEditor_Item)
     * 
     * The list of items to be managed.
     */
    items?:                             any[];
}

/**
 *
 * A component that allows the user to manage a list of text items.
 * 
 * This component renders a list of text and optional secondary text, items,
 * which allows the user to add, edit or delete items from the list.
 * 
 */
export default function ListEditor(props: IListEditorProps): JSX.Element;
