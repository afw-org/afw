// See the 'COPYING' file in the project root for licensing information.
import PropTypes from "prop-types";

export const propTypes = {
    /**
     * alignItems
     * Data Type: (string)
     * 
     * Alignment along the cross axis
     * This defines the default behavior for how flex items are laid out
     * along the cross axis on the current line. Think of it as the "Justify
     * Content" version for the cross axis (perpendicular to the main-axis).
     */
    alignItems:                         PropTypes.string,
    /**
     * direction
     * Data Type: (string)
     * 
     * The direction of items
     * This declares the direction that items are oriented inside the Box.
     */
    direction:                          PropTypes.string,
    /**
     * justifyContent
     * Data Type: (string)
     * 
     * Alignment along the main axis
     * This defines the alignment along the main axis. It helps distribute
     * extra free space leftover when either all the flex items on a line are
     * inflexible, or are flexible but have reached their maximum size. It
     * also exerts some control over the alignment of items when they
     * overflow the line.
     */
    justifyContent:                     PropTypes.string,
    /**
     * wrap
     * Data Type: (string)
     * 
     * Item wrap behavior
     * By default, flex items will all try to fit onto one line. You can
     * change that and allow the items to wrap as needed with this property.
     */
    wrap:                               PropTypes.string,
};

export const defaultProps = {
    alignItems:                         "stretch",
    direction:                          "row",
    justifyContent:                     "flex-start",
    wrap:                               "nowrap",
};

export default propTypes;
