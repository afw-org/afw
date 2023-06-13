// See the 'COPYING' file in the project root for licensing information.

import PropTypes from "prop-types";

/* Generated objectType propType definition for property text */
const ListEditor_Item = {
    /**
     * Property: secondary
     * Data Type: (string)
     * 
     * The secondary text to display.
     */
    secondary:                          PropTypes.string,
    /**
     * Property: text
     * Data Type: (string)
     * 
     * The primary text to display.
     */
    text:                               PropTypes.string,
};

/**
 * PropTypes generated by _AdaptiveLayoutComponentType_ListEditor
 */
export const propTypes = {
    /**
     * editable
     * Data Type: (boolean)
     * 
     * Specifies whether this list is currently editable.
     */
    editable:                           PropTypes.bool,
    /**
     * items
     * Data Type: (list, object _AdaptiveLayoutComponentType_ListEditor_Item)
     * 
     * The list of items to be managed.
     */
    items:                              PropTypes.arrayOf(PropTypes.shape(ListEditor_Item)),
};

export const defaultProps = {
};

export default propTypes;