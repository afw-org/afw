/**
 * Typescript interface definition for propTypes
 */
export interface ICollapsibleProps {
    /**
     * contains
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * Specifies the component contained within this Collapsible Body.
     */
    contains?:                          any;
    /**
     * headerItems
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * Specifies the component contained within this Collapsible Header.
     */
    headerItems?:                       any;
    /**
     * isCollapsed
     * Data Type: (boolean)
     * 
     * Specifies whether the Collapsible content is collapsed or not.
     */
    isCollapsed?:                       boolean;
    /**
     * showCollapse
     * Data Type: (boolean)
     * 
     * Specifies whether the Collapsible should render the collapse feature.
     */
    showCollapse?:                      boolean;
}

