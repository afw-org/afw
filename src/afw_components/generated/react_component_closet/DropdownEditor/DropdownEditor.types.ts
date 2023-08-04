
/**
 * Typescript interface definition for propTypes
 */
export interface IDropdownEditorProps {
    /**
     * description
     * Data Type: (string)
     * 
     * The description for this dropdown editor.
     */
    description?:                       string;
    /**
     * editable
     * Data Type: (boolean)
     * 
     * Whether the options managed by this Dropdown Editor are editable.
     */
    editable?:                          boolean;
    /**
     * label
     * Data Type: (string)
     * 
     * The label for the Dropdown component.
     */
    label?:                             string;
    /**
     * multiselect
     * Data Type: (boolean)
     * 
     * Whether this Dropdown allows for multiple selections.
     */
    multiselect?:                       boolean;
    /**
     * options
     * Data Type: (list, object _AdaptiveLayoutComponentType_Dropdown_Option)
     * 
     * Specifies the available options for the dropdown.
     */
    options?:                           any[];
    /**
     * selectedKeys
     * Data Type: (list, string)
     * 
     * The key(s) that are selected for the Dropdown component.
     */
    selectedKeys?:                      string[];
}

