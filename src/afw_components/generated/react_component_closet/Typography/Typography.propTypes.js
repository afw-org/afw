import PropTypes from "prop-types";

/**
 * PropTypes generated by _AdaptiveLayoutComponentType_Typography
 */
export const propTypes = {
    /**
     * color
     * Data Type: (string)
     * 
     * The color of the text
     * The color of the text. This may be a theme definition, or an actual
     * color.
     */
    color:                              PropTypes.oneOf(["default", "error", "inherit", "primary", "secondary", "textPrimary", "textSecondary"]),
    /**
     * size
     * Data Type: (string)
     * 
     * The size of the text
     * The size of the text (1-11).
     */
    size:                               PropTypes.oneOf(["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"]),
    /**
     * tag
     * Data Type: (string)
     * 
     * The html tag to produce
     * This allows you to explicitly set the underlying HTML tag in order to
     * get the semantics correct.
     */
    tag:                                PropTypes.oneOf(["h1", "h2", "h3", "h4", "h5", "h6", "body", "span", "div"]),
    /**
     * text
     * Data Type: (string)
     * 
     * The text string to be displayed
     * The actual text content to be rendered by this component.
     */
    text:                               PropTypes.string.isRequired,
};

export const defaultProps = {
};

export default propTypes;
