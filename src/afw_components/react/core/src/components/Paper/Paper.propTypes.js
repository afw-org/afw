// See the 'COPYING' file in the project root for licensing information.

import PropTypes from "prop-types";

/**
 * PropTypes generated by _AdaptiveLayoutComponentType_Paper
 */
export const propTypes = {
    /**
     * contains
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * Contained component on this Paper layout.
     */
    contains:                           PropTypes.oneOfType([ PropTypes.element, PropTypes.object ]),
    /**
     * elevation
     * Data Type: (integer)
     * 
     * Depth elevation appearance of the Paper surface.
     */
    elevation:                          PropTypes.number,
    /**
     * square
     * Data Type: (boolean)
     * 
     * Use square corners instead of rounded.
     */
    square:                             PropTypes.bool,
};

export const defaultProps = {
    square:                             false,
};

export default propTypes;
