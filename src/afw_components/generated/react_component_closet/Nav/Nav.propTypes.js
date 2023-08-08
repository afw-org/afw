import PropTypes from "prop-types";

/* Generated objectType propType definition for property url */
const Nav_Item = {
    /**
     * Property: icon
     * Data Type: (string)
     * 
     * Icon for this Nav item.
     */
    icon:                               PropTypes.string,
    /**
     * Property: isExpanded
     * Data Type: (boolean)
     * 
     * Is the current Nav link expanded.
     */
    isExpanded:                         PropTypes.bool,
    /**
     * Property: label
     * Data Type: (string)
     * 
     * Label for this Nav item.
     */
    label:                              PropTypes.string,
    /**
     * Property: selected
     * Data Type: (boolean)
     * 
     * Is the current Nav link selected.
     */
    selected:                           PropTypes.bool,
    /**
     * Property: url
     * Data Type: (string)
     * 
     * Url for this Nav item.
     */
    url:                                PropTypes.string,
};

/**
 * PropTypes generated by _AdaptiveLayoutComponentType_Nav
 */
export const propTypes = {
    /**
     * items
     * Data Type: (array, object _AdaptiveLayoutComponentType_Nav_Item)
     * 
     * Item to be displayed by this Nav layout.
     */
    items:                              PropTypes.arrayOf(PropTypes.shape(Nav_Item)),
};

export const defaultProps = {
};

export default propTypes;
