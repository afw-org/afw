// See the 'COPYING' file in the project root for licensing information.

import PropTypes from "prop-types";

/**
 * PropTypes generated by _AdaptiveLayoutComponentType_Hidden
 */
export const propTypes = {
    /**
     * contains
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * The contained component that will be hidden.
     */
    contains:                           PropTypes.oneOfType([ PropTypes.element, PropTypes.object ]),
    /**
     * for
     * Data Type: (list, string)
     * 
     * The breakpoint conditions for which the contained component will be
     * hidden (xs, sm, md, lg, xl).
     */
    for:                                PropTypes.arrayOf(PropTypes.string),
    /**
     * lgDown
     * Data Type: (boolean)
     * 
     * This content will only be displayed on large displays or smaller.
     */
    lgDown:                             PropTypes.bool,
    /**
     * mdDown
     * Data Type: (boolean)
     * 
     * This content will only be displayed on medium displays or smaller.
     */
    mdDown:                             PropTypes.bool,
    /**
     * smDown
     * Data Type: (boolean)
     * 
     * This content will only be displayed on small displays or smaller.
     */
    smDown:                             PropTypes.bool,
    /**
     * xlDown
     * Data Type: (boolean)
     * 
     * This content will only be displayed on extra large displays or
     * smaller.
     */
    xlDown:                             PropTypes.bool,
    /**
     * xsDown
     * Data Type: (boolean)
     * 
     * This content will only be displayed on extra small displays.
     */
    xsDown:                             PropTypes.bool,
};

export const defaultProps = {
};

export default propTypes;