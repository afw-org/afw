// See the 'COPYING' file in the project root for licensing information.
import PropTypes from "prop-types";

/* Generated objectType propType definition for property xxxl */
const Responsive_Breakpoints = {
    /**
     * Property: large
     * Data Type: (integer)
     *
     * The breakpoint definition on large resolution screens.
     */
    large:                              PropTypes.oneOf([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
    /**
     * Property: medium
     * Data Type: (integer)
     *
     * The breakpoint definition on medium resolution screens.
     */
    medium:                             PropTypes.oneOf([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
    /**
     * Property: small
     * Data Type: (integer)
     *
     * The breakpoint definition on small resolution screens.
     */
    small:                              PropTypes.oneOf([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
    /**
     * Property: xl
     * Data Type: (integer)
     *
     * The breakpoint definition on extra large resolution screens.
     */
    xl:                                 PropTypes.oneOf([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
    /**
     * Property: xxl
     * Data Type: (integer)
     *
     * The breakpoint definition on extra extra large resolution screens.
     */
    xxl:                                PropTypes.oneOf([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
    /**
     * Property: xxxl
     * Data Type: (integer)
     *
     * The breakpoint definition on extra extra extra large resolution
     * screens.
     */
    xxxl:                               PropTypes.oneOf([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
};

/* Generated objectType propType definition for property propertyName */
const ObjectResponsive_preferredComponent = {
    /**
     * Property: component
     * Data Type: (string)
     *
     * The Preferred Component to apply to this layout rule definition.  For
     * example, single booleans should be Switch components.
     */
    component:                          PropTypes.string,
    /**
     * Property: componentProps
     * Data Type: (object)
     *
     * Additional properties to be passed to each preferred component.
     */
    componentProps:                     PropTypes.any,
    /**
     * Property: dataType
     * Data Type: (string)
     *
     * Any property that matches this dataType will have a Preferred
     * Component applied to its layout.
     */
    dataType:                           PropTypes.string,
    /**
     * Property: propertyName
     * Data Type: (string)
     *
     * Any property name that matches this name will have a Preferred
     * Component applied to its layout.
     */
    propertyName:                       PropTypes.string,
};

const EditOptions = {
    editAdd:                            PropTypes.bool,
    editRemove:                         PropTypes.bool,
};


/* Generated objectType propType definition for property component */
const ObjectResponsive_embedded = {
    /**
     * Property: component
     * Data Type: (string)
     *
     * The Preferred Component to apply to this embedded object.
     */
    component:                          PropTypes.oneOf(["Collapsible", "Link", "Paper", "none"]),
};


/* Generated objectType propType definition for property recurse */
const ObjectResponsive_filterOptions = {
    /**
     * Property: filterCondAndProperties
     * Data Type: (list, string)
     *
     * Specifies exclusive (AND) conditions for properties to display
     * (required, optional, valued, unvalued).
     */
    filterCondAndProperties:            PropTypes.arrayOf(PropTypes.string),
    /**
     * Property: filterCondOrProperties
     * Data Type: (list, string)
     *
     * Specifies inclusive (OR) conditions for properties to display
     * (required, optional, valued, unvalued).
     */
    filterCondOrProperties:             PropTypes.arrayOf(PropTypes.string),
    /**
     * Property: filterExcludeEmbeddedProperties
     * Data Type: (boolean)
     *
     * Exclude embedded properties from display.
     */
    filterExcludeEmbeddedProperties:    PropTypes.bool,
    /**
     * Property: filterExcludeProperties
     * Data Type: (list, string)
     *
     * Do not display the specified properties.
     */
    filterExcludeProperties:            PropTypes.arrayOf(PropTypes.string),
    /**
     * Property: filterProperties
     * Data Type: (list, string)
     *
     * Display the specified properties.
     */
    filterProperties:                   PropTypes.arrayOf(PropTypes.string),
    /**
     * Property: filterRequiredProperties
     * Data Type: (boolean)
     *
     * Display Required Properties.
     */
    filterRequiredProperties:           PropTypes.bool,
    /**
     * Property: filterValuedProperties
     * Data Type: (boolean)
     *
     * Display valued properties.
     */
    filterValuedProperties:             PropTypes.bool,
    /**
     * Property: recurse
     * Data Type: (boolean)
     *
     * Recurse filter options to embedded objects.
     */
    recurse:                            PropTypes.bool,
};

/* Generated objectType propType definition for property sortByRequired */
const ObjectResponsive_sortOptions = {
    /**
     * Property: recurse
     * Data Type: (boolean)
     *
     * Recurse sort options to embedded objects.
     */
    recurse:                            PropTypes.bool,
    /**
     * Property: sortByDataType
     * Data Type: (boolean)
     *
     * The properties should be sorted by their data type.
     */
    sortByDataType:                     PropTypes.bool,
    /**
     * Property: sortByPropertyName
     * Data Type: (boolean)
     *
     * The properties should be sorted by their property name.
     */
    sortByPropertyName:                 PropTypes.bool,
    /**
     * Property: sortByRequired
     * Data Type: (boolean)
     *
     * The required properties should be sorted before optional properties.
     */
    sortByRequired:                     PropTypes.bool,
};

export const propTypes = {
    /**
     */
    bodyStyle:                          PropTypes.object,

    /**
     */
    bodyClassName:                      PropTypes.string,

    /**
     */
    breakpoints:                        PropTypes.shape(Responsive_Breakpoints),

    /**
     */
    columns:                            PropTypes.number,

    /**
     */
    editable:                           PropTypes.oneOfType([PropTypes.bool, PropTypes.shape(EditOptions)]),

    /**
     */
    embedded:                           PropTypes.oneOfType([PropTypes.bool, PropTypes.shape(ObjectResponsive_embedded)]),

    /**
     */
    filterOptions:                      PropTypes.shape(ObjectResponsive_filterOptions),

    /**
     */
    headerOptions:                      PropTypes.oneOf(["none", "base", "embedded", "all"]),

    /**
     */
    nestingDepth:                       PropTypes.number,

    /**
     */
    isCollapsed:                        PropTypes.bool,

    /**
     */
    onSort:                             PropTypes.bool,

    /**
     */
    preferredComponents:                PropTypes.arrayOf(PropTypes.shape(ObjectResponsive_preferredComponent)),

    /**
     */
    showLabels:                         PropTypes.bool,

    /**
     */
    showDescriptions:                   PropTypes.bool,

    /**
     */
    sortOptions:                        PropTypes.shape(ObjectResponsive_sortOptions),

    /**
     */
    spacing:                            PropTypes.string,

    /**
     */
    selectable:                         PropTypes.bool,

    /**     
     */
    showMeta:                           PropTypes.bool,

    /**     
     */
    tooltip:                           PropTypes.bool,
};


export const defaultProps = {
    nestingDepth:                       0,
    headerOptions:                      "embedded",
    embedded:                           { component: "Collapsible" },
    columns:                            1,
    selectable:                         true,
    showMeta:                           false,
    showLabels:                         true,
    showDescriptions:                   true,
    filterOptions:                      { recurse: true },
    sortOptions:                        { sortByPropertyName: true, recurse: true },
    spacing:                            "16px"
};
