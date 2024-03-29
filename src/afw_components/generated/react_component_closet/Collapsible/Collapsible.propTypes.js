import PropTypes from "prop-types";

/**
 * PropTypes generated by _AdaptiveLayoutComponentType_Collapsible
 */
export const propTypes = {
    /**
     * contains
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * Specifies the component contained within this Collapsible Body.
     */
    contains:                           PropTypes.oneOfType([ PropTypes.element, PropTypes.object ]),
    /**
     * headerItems
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * Specifies the component contained within this Collapsible Header.
     */
    headerItems:                        PropTypes.oneOfType([ PropTypes.element, PropTypes.object ]),
    /**
     * isCollapsed
     * Data Type: (boolean)
     * 
     * Specifies whether the Collapsible content is collapsed or not.
     */
    isCollapsed:                        PropTypes.bool,
    /**
     * showCollapse
     * Data Type: (boolean)
     * 
     * Specifies whether the Collapsible should render the collapse feature.
     */
    showCollapse:                       PropTypes.bool,
};

export const defaultProps = {
    isCollapsed:                        true,
    showCollapse:                       true,
};

export default propTypes;
