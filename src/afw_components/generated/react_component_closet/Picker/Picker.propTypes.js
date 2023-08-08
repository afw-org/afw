import PropTypes from "prop-types";

/* Generated objectType propType definition for property label */
const Picker_Item = {
    /**
     * Property: key
     * Data Type: (string)
     * 
     * Key for this Picker item.
     */
    key:                                PropTypes.string,
    /**
     * Property: label
     * Data Type: (string)
     * 
     * Label for this Picker item.
     */
    label:                              PropTypes.string,
};

/**
 * PropTypes generated by _AdaptiveLayoutComponentType_Picker
 */
export const propTypes = {
    /**
     * description
     * Data Type: (string)
     * 
     * The text description for this Picker component.
     */
    description:                        PropTypes.string,
    /**
     * items
     * Data Type: (array, object _AdaptiveLayoutComponentType_Picker_Item)
     * 
     * Items to be displayed by this Picker layout.
     */
    items:                              PropTypes.arrayOf(PropTypes.shape(Picker_Item)),
    /**
     * label
     * Data Type: (string)
     * 
     * The text label for this Picker component.
     */
    label:                              PropTypes.string,
};

export const defaultProps = {
};

export default propTypes;
