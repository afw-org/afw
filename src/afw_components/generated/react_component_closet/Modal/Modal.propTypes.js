import PropTypes from "prop-types";

/**
 * PropTypes generated by _AdaptiveLayoutComponentType_Modal
 */
export const propTypes = {
    /**
     * contains
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * Specifies the placeholder text displayed when no value is present.
     */
    contains:                           PropTypes.oneOfType([ PropTypes.element, PropTypes.object ]),
    /**
     * isBlocking
     * Data Type: (boolean)
     * 
     * Specifies whether this Modal component is blocking or can lightly be
     * dismissed.
     */
    isBlocking:                         PropTypes.bool,
    /**
     * open
     * Data Type: (boolean)
     * 
     * Specifies whether this Modal component is open.
     */
    open:                               PropTypes.bool.isRequired,
};

export const defaultProps = {
};

export default propTypes;