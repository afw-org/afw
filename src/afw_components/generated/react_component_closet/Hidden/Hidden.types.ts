/**
 * Typescript interface definition for propTypes
 */
export interface IHiddenProps {
    /**
     * contains
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * The contained component that will be hidden.
     */
    contains?:                          any;
    /**
     * for
     * Data Type: (list, string)
     * 
     * The breakpoint conditions for which the contained component will be
     * hidden (xs, sm, md, lg, xl).
     */
    for?:                               string[];
    /**
     * lgDown
     * Data Type: (boolean)
     * 
     * This content will only be displayed on large displays or smaller.
     */
    lgDown?:                            boolean;
    /**
     * mdDown
     * Data Type: (boolean)
     * 
     * This content will only be displayed on medium displays or smaller.
     */
    mdDown?:                            boolean;
    /**
     * smDown
     * Data Type: (boolean)
     * 
     * This content will only be displayed on small displays or smaller.
     */
    smDown?:                            boolean;
    /**
     * xlDown
     * Data Type: (boolean)
     * 
     * This content will only be displayed on extra large displays or
     * smaller.
     */
    xlDown?:                            boolean;
    /**
     * xsDown
     * Data Type: (boolean)
     * 
     * This content will only be displayed on extra small displays.
     */
    xsDown?:                            boolean;
}

