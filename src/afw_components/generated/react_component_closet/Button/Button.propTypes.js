import PropTypes from "prop-types";

/**
 * PropTypes generated by _AdaptiveLayoutComponentType_Button
 */
export const propTypes = {
    /**
     * color
     * Data Type: (string)
     * 
     * Specifies an optional color for this Button (primary, secondary,
     * error).
     */
    color:                              PropTypes.oneOf(["default", "inherit", "primary", "secondary"]),
    /**
     * disabled
     * Data Type: (boolean)
     * 
     * Specifies whether this Button component is disabled.
     */
    disabled:                           PropTypes.bool,
    /**
     * dropdown
     * Data Type: (boolean)
     * 
     * Specifies an optional dropdown icon for this Button.
     */
    dropdown:                           PropTypes.bool,
    /**
     * icon
     * Data Type: (string)
     * 
     * Specifies an optional icon for this Button.
     */
    icon:                               PropTypes.string,
    /**
     * label
     * Data Type: (string)
     * 
     * Specifies the text on the Button.
     */
    label:                              PropTypes.string,
    /**
     * secondaryText
     * Data Type: (string)
     * 
     * Specifies optional text to be added below the main text label.
     */
    secondaryText:                      PropTypes.string,
    /**
     * size
     * Data Type: (string)
     * 
     * Specifies a size for the button (small)
     */
    size:                               PropTypes.oneOf(["small", "medium", "large"]),
    /**
     * type
     * Data Type: (string)
     * 
     * The type of Button (icon, split, compound, default).
     */
    type:                               PropTypes.oneOf(["icon", "split", "compound", "default"]),
};

export const defaultProps = {
};

export default propTypes;
