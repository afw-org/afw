/**
 * Typescript interface definition for propTypes
 */
export interface IModalProps {
    /**
     * contains
     * Data Type: (object, _AdaptiveLayoutComponentType_)
     * 
     * The component to be rendered inside the Modal.
     */
    contains?:                          any;
    /**
     * isBlocking
     * Data Type: (boolean)
     * 
     * Specifies whether this Modal component is blocking or can lightly be
     * dismissed.
     */
    isBlocking?:                        boolean;
    /**
     * open
     * Data Type: (boolean)
     * 
     * Specifies whether this Modal component is open.
     */
    open:                               boolean;
}

