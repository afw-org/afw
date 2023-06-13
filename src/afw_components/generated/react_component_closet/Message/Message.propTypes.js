import PropTypes from "prop-types";

/**
 * PropTypes generated by _AdaptiveLayoutComponentType_Message
 */
export const propTypes = {
    /**
     * canDismiss
     * Data Type: (boolean)
     * 
     * Specifies whether the Message can be dismissed.
     */
    canDismiss:                         PropTypes.bool,
    /**
     * contains
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * Optional components to be displayed for the Message component.
     */
    contains:                           PropTypes.oneOfType([ PropTypes.element, PropTypes.object ]),
    /**
     * isMultiline
     * Data Type: (boolean)
     * 
     * Specifies whether the Message message is multilined.
     */
    isMultiline:                        PropTypes.bool,
    /**
     * message
     * Data Type: (string)
     * 
     * Optional message to be displayed for the Message component.
     */
    message:                            PropTypes.string,
    /**
     * status
     * Data Type: (string)
     * 
     * Status of message (error, info, warning).
     */
    status:                             PropTypes.oneOf(["error", "info", "warning"]),
};

export const defaultProps = {
};

export default propTypes;