
/**
 * Typescript interface definition for propTypes
 */
export interface ICalloutProps {
    /**
     * contains
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * The component to be rendered inside the Callout.
     */
    contains?:                          any;
    /**
     * gapSpace
     * Data Type: (integer)
     * 
     * The gap space between target and the Callout component.
     */
    gapSpace?:                          number;
    /**
     * open
     * Data Type: (boolean)
     * 
     * Specifies whether this Callout component is open.
     */
    open:                               boolean;
    /**
     * targetPosition
     * Data Type: (object, _AdaptiveLayoutComponentType_Callout_TargetPosition)
     * 
     * Specifies the position of the callout, relative to the target
     * component.
     */
    targetPosition?:                    any;
}

