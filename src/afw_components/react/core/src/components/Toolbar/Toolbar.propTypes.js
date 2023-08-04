// See the 'COPYING' file in the project root for licensing information.

import PropTypes from "prop-types";

/**
 * PropTypes generated by _AdaptiveLayoutComponentType_Toolbar
 */
export const propTypes = {
    /**
     * alignContent
     * Data Type: (string)
     * 
     * Defines the align-content style property.
     */
    alignContent:                       PropTypes.oneOf(["stretch", "center", "flex-start", "flex-end", "space-between", "space-around"]),
    /**
     * alignItems
     * Data Type: (string)
     * 
     * Defines the align-items style property.
     */
    alignItems:                         PropTypes.oneOf(["flex-start", "center", "flex-end", "stretch", "baseline"]),
    /**
     * contains
     * Data Type: (list, object _AdaptiveLayoutComponentType_)
     * 
     * A list of components to be contained by this Toolbar.
     */
    contains:                           PropTypes.arrayOf(PropTypes.oneOfType([ PropTypes.element, PropTypes.object ])).isRequired,
    /**
     * dense
     * Data Type: (boolean)
     * 
     * Specifies whether the Toolbar should be condensed as much as possible.
     */
    dense:                              PropTypes.bool,
    /**
     * justify
     * Data Type: (string)
     * 
     * Defines the justify-content style property.
     */
    justify:                            PropTypes.oneOf(["flex-start", "center", "flex-end", "space-between", "space-around", "space-evenly"]),
    /**
     * spacing
     * Data Type: (integer)
     * 
     * The spacing between items in the Toolbar.
     */
    spacing:                            PropTypes.oneOf([0, 8, 16, 24, 32, 40]),
};

export const defaultProps = {
};

export default propTypes;
