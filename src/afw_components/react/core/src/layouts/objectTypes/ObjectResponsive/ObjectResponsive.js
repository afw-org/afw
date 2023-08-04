// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for ObjectResponsive
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {useEffect, useMemo, useReducer} from "react";

import {propTypes, defaultProps} from "./ObjectResponsive.propTypes";
import PropertyResponsive from "./PropertyResponsive";
import {ObjectResponsiveHeader} from "./ObjectResponsiveHeader";

import {AfwObject} from "@afw/client";

/* common components in react-core */
import {    
    Checkbox,
    Collapsible,
    Divider,
    Link,
    Paper,
    Responsive,
    Typography
} from "@afw/react";

/* hooks from react-core */
import {
    useEventId,
    useModel,
    useOperational,
    useAdaptiveLayout,    
    useRouteBasePath,
    useObjectEditor,
    AdaptiveLayoutContext,    
} from "@afw/react";

import {OperationalContext, OperationalMode} from "../../../context";
import {useTheme, useClasses} from "../../../hooks";


/* styling props for ObjectResponsive */
const styles = theme => ({
    PropertyCheckboxContainer: {
        width: theme.spacing(6), 
        textAlign: "center", 
        verticalAlign: "top"
    },
    PropertyValueContainer: {
        flex: 1, 
        verticalAlign: "top"
    }
});

/**
 * filterProperties(property, properties)
 *
 * This filter returns the property only if the name
 *   matches one of the property names in the properties array.
 */
const filterProperties = (property, properties) => {
    return (properties.includes(property.getName()));
};

/**
 * filterExcludeProperties(property, properties)
 *
 * This filter returns the property only if the name
 *   does not match one of the property names in the properties array.
 */
const filterExcludeProperties = (property, properties) => {
    return !filterProperties(property, properties);
};

/**
 * filterRequiredProperties(property, properties)
 *
 * This filter returns the property only if the property
 *   is a required one.
 */
const filterRequiredProperties = (property) => {
    return (property.getRequired());
};

/**
 * filterValuedProperties(property, properties)
 *
 * This filter returns the property only if the property
 *   contains a value.
 */
const filterValuedProperties = (property) => {
    return (property.getValue() !== undefined);
};

/**
 * filterExcludeEmbeddedProperties(property, properties)
 *
 * This filter returns the property only if it's not an
 *   embedded property (dataType of "object").
 */
const filterExcludeEmbeddedProperties = (property) => {
    return (property.getDataType() !== "object");
};

/**
 * filterCondAndProperties(property, properties)
 *
 * This filter returns the property if it passes all
 *   conditions in the conditions array.
 */
const filterCondAndProperties = (property, conditions) => {
    for (const condition of conditions) {
        if (condition === "required" && property.getRequired() === false)
            return false;
        else if (condition === "optional" && property.getRequired() === true)
            return false;
        else if (condition === "valued" && property.getValue() === undefined)
            return false;
        else if (condition === "unvalued" && property.getValue() !== undefined)
            return false;
    }

    return true;
};

/**
 * filterCondOrProperties(property, properties)
 *
 * This filter returns the property if it passes any of the
 *   conditions in the conditions array.
 */
const filterCondOrProperties = (property, conditions) => {
    for (const condition of conditions) {
        if (condition === "required" && property.getRequired() === true)
            return true;
        else if (condition === "optional" && property.getRequired() === false)
            return true;
        else if (condition === "valued" && property.getValue() !== undefined)
            return true;
        else if (condition === "unvalued" && property.getValue() === undefined)
            return true;
    }

    return false;
};

/**
 * This routine sorts two properties by their property name (case-ignore)
 */
const sortByPropertyName = (property1, property2) => {
    return (property1.getName().toLowerCase().localeCompare(
        property2.getName().toLowerCase()));
};

/**
 * This routine sorts two properties by their dataType.
 */
const sortByDataType = (property1, property2) => {
    if (!property1.getDataType())
        return 1;
    else if (!property2.getDataType())
        return -1;
    else
        return (property1.getDataType().localeCompare(property2.getDataType()));
};

/**
 * This routine sorts properties so "required" properties are ordered first.
 */
const sortByRequired = (property1, property2) => {
    if (property1.getRequired() && !property2.getRequired())
        return -1;
    else if (!property1.getRequired() && property2.getRequired())
        return 1;
    else
        return 0;
};

/**
 *  The filter routine simply filters a list of properties to those that are applicable through prop configured filter rules.  
 */
const filter = (object, filterOptions) => {
    let properties = object.getProperties();

    if (!filterOptions)
        return properties;

    if (filterOptions.filterProperties) {
        /* First make sure the filterProperties are valid */
        properties = [];
        filterOptions.filterProperties.forEach(propertyName => {
            if (object.getProperty(propertyName))
                properties.push(object.getProperty(propertyName));
        });
    }

    if (filterOptions.filterExcludeProperties) {
        properties = properties.filter(filterProperty =>
            filterExcludeProperties(filterProperty, filterOptions.filterExcludeProperties));
    }

    if (filterOptions.filterRequiredProperties === true)
        properties = properties.filter(filterProperty =>
            filterRequiredProperties(filterProperty));

    if (filterOptions.filterValuedProperties === true)
        properties = properties.filter(filterProperty =>
            filterValuedProperties(filterProperty));

    if (filterOptions.filterExcludeEmbeddedProperties === true)
        properties = properties.filter(filterProperty =>
            filterExcludeEmbeddedProperties(filterProperty));

    if (filterOptions.filterCondOrProperties) {
        properties = properties.filter(filterProperty =>
            filterCondOrProperties(filterProperty, filterOptions.filterCondOrProperties));
    }

    if (filterOptions.filterCondAndProperties) {
        properties = properties.filter(filterProperty =>
            filterCondAndProperties(filterProperty, filterOptions.filterCondAndProperties));
    }

    /* finally, check the onFilter callback */
    if (filterOptions.onFilter)
        properties = properties.filter((filterProperty, index) =>
            filterOptions.onFilter(filterProperty, index));

    return properties;
};


/**
 * The sort routine sorts the properties into an arrangement configured by sorting rules 
 */
const sort = (properties, sortOptions, filterOptions) => {
    if (!sortOptions)
        return properties;

    /* if properties were specified, use the order specified */
    if (filterOptions && filterOptions.filterProperties) {
        return properties;
    }

    if (sortOptions.sortByPropertyName === true)
        properties = properties.sort((a, b) => sortByPropertyName(a, b));

    if (sortOptions.sortByDataType === true)
        properties = properties.sort((a, b) => sortByDataType(a, b));

    if (sortOptions.sortByRequired === true)
        properties = properties.sort((a, b) => sortByRequired(a, b));

    return properties;
};

/**
 * EmbeddedLayout
 * 
 * This determines how to render embedded objects.  It will use the layoutParameters 
 * to choose the appropriate container; otherwise, it will default to a Collapsible
 * container.  The following embedded components are valid:
 * 
 * Collapsible - a collapsible area (accordian), which can expand when the user clicks
 * on the expand icon.  This is useful for hiding it, until the user wishes to dive 
 * into the embedded object.
 * 
 * Paper - a simple Paper layout, containing a contrasting-color background, determined 
 * by the theme to help separate the embedded object from the parent.
 * 
 * Link - a link that the user can click to traverse the embedded object path.  Useful if 
 * you only want to render one object at a time.
 * 
 * Box - an area, outlined with a border, to separate the embedded object from the rest of
 * the parent's content.  Useful if you have just a few properties.
 * 
 * None - Similar to box, but with no border outline.
 */
const EmbeddedLayout = (props) => {

    const {
        layout, 
        isCollapsed, 
        property, 
        nestingDepth, 
        embedded, 
        onRenderHeader, 
        ariaLabel, 
        style
    } = props;

    const theme = useTheme();
    const routeBasePath = useRouteBasePath();    

    let collapsed = isCollapsed;

    if (collapsed === undefined)
        collapsed = ((property && property.getRequired()) ? false : true);

    /* alternate background colors to distinguish nesting levels */
    const containerStyle = {
        backgroundColor: (nestingDepth % 2 === 0) ? theme.palette.background.default : theme.palette.background.paper,
        borderRadius: theme.shape.borderRadius
    };

    /* The default embedded object layout is a Collapsible */
    if (!embedded.component || (embedded.component === "Collapsible")) 
    {
        return (
            <Collapsible                    
                style={containerStyle}
                aria-label={ariaLabel}
                contains={
                    <div style={{ ...style }}>
                        <Divider />
                        <div style={{ height: theme.spacing(1) }} />
                        {layout}
                    </div>
                }
                isCollapsed={collapsed}                    
                onRenderHeader={onRenderHeader}
                nestingDepth={nestingDepth + 1}
            />
        );
    } 
    
    if (embedded.component === "Paper") 
    {
        return (
            <Paper
                style={containerStyle}
                aria-label={ariaLabel}
                elevation={3}
                contains={
                    <div style={{ padding: theme.spacing(2) }}>
                        <div style={{ width: "100%" }}>
                            { onRenderHeader() }
                        </div>
                        <Divider />
                        <div style={{ height: theme.spacing(1) }} />
                        {layout}
                    </div>
                }
            />
        );
    } 
    
    if (embedded.component === "Link" && property) 
    {
        return (
            <Responsive.Row
                columns={[
                    {
                        breakpoints: {
                            small: 6,
                            medium: 4,
                            large: 3
                        },
                        contains:
                            <Link 
                                url={routeBasePath + property.getPath()} 
                                text={property.getLabel() ? property.getLabel() : property.getName()} 
                            />
                    },
                    {
                        breakpoints: {
                            small: 6,
                            medium: 8,
                            large: 9
                        },
                        contains: 
                            <Typography size="5" component="span" text={property.getDescription()} />
                    }
                ]} />  
        );
    } 
    
    if (embedded.component === "Box") 
    {
        return (
            <div 
                style={{ 
                    ...style, 
                    border: "solid thin", 
                    borderColor: theme.palette.primary.light,
                    padding: theme.spacing(2) 
                }} 
                aria-label={ariaLabel}
            >
                <div style={{ width: "100%", marginBottom: theme.spacing(1) }}>
                    { onRenderHeader() }
                </div>
                <Divider />
                <div style={{ height: theme.spacing(1) }} />
                <div>
                    {layout}
                </div>
            </div>
        );           
    } 
        
    /* None */
    return (
        <div style={style} aria-label={ariaLabel}>
            <div style={{ width: "100%", marginBottom: theme.spacing(1) }}>
                { onRenderHeader() }
            </div>
            <Divider />
            <div style={{ height: theme.spacing(1) }} />
            <div style={{ marginLeft: theme.spacing(2) }}>
                {layout}
            </div>
        </div>
    );
    
};

const initialState = {
    visibleProperties:      [],
    selectedProperties:     [],
    addedProperties:        [],    
};


const getVisibleProperties = (object, filterOptions, sortOptions, addedProperties) => {

    let visibleProperties = [];

    if (object) {
        /* apply any necessary filters specified by the layout props */
        let properties = filter(object, filterOptions);

        /* add in any added properties that may not pass filter options yet */
        addedProperties.forEach(p => {
            if (!properties.some(p2 => p2.getName() === p.getName()))
                properties.push(p);
        });

        /* Apply any sorting props */
        if (sortOptions && sortOptions.onSort)
            visibleProperties = properties.sort(sortOptions.onSort);
        else
            visibleProperties = sort(properties, sortOptions, filterOptions);
    }

    return visibleProperties;
};


const reducer = (state, action) => {

    switch (action.type) {

    case "OBJECT": { 
    
        const visibleProperties = getVisibleProperties(
            action.object, state.filterOptions, 
            state.sortOptions, []
        );

        return {
            ...state,
            object: action.object,
            addedProperties: [],
            selectedProperties: [],      
            visibleProperties,      
        };
    }

    case "EDITABLE":
        if (action.editable) {
            return {
                ...state,
                selectedProperties: [],
            };        
        } else return state;

    case "PROPERTY_SELECTED": {
        const {selectedProperties} = state;
        const {property} = action;
        let newSelectedProperties;

        if (selectedProperties.some(p => p.getName() === action.property.getName()))
            newSelectedProperties = selectedProperties.filter(p => p.getName() !== property.getName());
        else
            newSelectedProperties = [ ...selectedProperties, property ];
        
        return {
            ...state,
            selectedProperties: newSelectedProperties,
        };        
    }

    case "PROPERTY_ADDED": {
        const addedProperties = [ ...state.addedProperties, action.property ];
        const visibleProperties = getVisibleProperties(
            state.object, state.filterOptions, state.sortOptions,
            addedProperties
        );

        return {
            ...state,
            addedProperties,
            lastAddedProperty: action.property,
            visibleProperties,
        };  
    }
        
    case "PROPERTIES_DELETE": {

        state.selectedProperties.forEach(selectedProperty => {
            if (selectedProperty.getAllowWrite() !== false)
                selectedProperty.remove();
        });

        const visibleProperties = state.visibleProperties.filter( p => {
            for (const selectedProperty of state.selectedProperties) {
                /* can't remove properties that don't allow write */
                if (selectedProperty.getAllowWrite() === false)
                    return true;

                if (selectedProperty.getName() === p.getName())
                    return false;
            }

            return true;
        });

        const addedProperties = state.addedProperties.filter( p => {
            for (const addedProperty of state.addedProperties) {
                if (addedProperty.getName() === p.getName())
                    return false;
            }

            return true;
        });

        return {
            ...state,
            selectedProperties: [],
            visibleProperties,
            addedProperties,
        };
    }

    case "PROPERTIES_CHANGED": {
        const visibleProperties = getVisibleProperties(
            state.object, state.filterOptions, 
            state.sortOptions, state.addedProperties
        );

        return {
            ...state,            
            visibleProperties,
        };
    }

    case "CHANGES_DISCARDED": {
        const visibleProperties = getVisibleProperties(
            state.object, state.filterOptions, 
            state.sortOptions, []
        );

        return {
            ...state,            
            addedProperties: [],
            visibleProperties,
        };
    }   

    case "FILTER":
    case "SORT": {
        const filterOptions = action.filterOptions || state.filterOptions;
        const sortOptions = action.sortOptions || state.sortOptions;

        const visibleProperties = getVisibleProperties(
            state.object, filterOptions, sortOptions, state.addedProperties
        );        

        return {
            ...state,
            filterOptions,
            sortOptions,
            visibleProperties,
        };        
    }

    default:
        return state;
    }
};

/**
 * ObjectResponsive 
 *
 * A layout that displays an adaptive object with a responsive layout.
 *
 * ObjectResponsive layout renders an object and its properties with some props that influence
 * how they are displayed in an HTML Form UI.  Each property is laid out in a
 * Responsive Layout, each property to a row, with optional columns.
 *
 * The properties are individually rendered using the PropertyResponsive layouts, which
 * best determine the component to use based upon the property's dataType
 * metadata.  These can also be influenced by props.
 *
 */
export const ObjectResponsive = (props) => {

    const [state, dispatch] = useReducer(reducer, initialState);     
    const {
        visibleProperties,
        selectedProperties,
        addedProperties,
        lastAddedProperty,
        object
    } = state;    

    const model = useModel();
    const {editable} = useOperational(props);
    const afwLayout = useAdaptiveLayout(props, propTypes, defaultProps);
    const {onRenderProperty, onRenderHeader, onSelectProperty} = useObjectEditor(props);
    
    const classes = useClasses(styles);
    const theme = useTheme();
    const propertiesChanged = useEventId({ object, eventId: "onPropertiesChanged" });
    const changesDiscarded = useEventId({ object, eventId: "onDiscardChanges" });

    const {
        autoFocus,
        propertyStyle,
        style,
        "aria-label": ariaLabel,
        "data-testid": dataTestId,
    } = props;
    
    const {
        layoutParameters,         
        data: { object: obj, property }
    } = afwLayout;    

    const {        
        filterOptions,
        sortOptions,
        preferredComponents,
        showMeta,
        bodyClassName,
        bodyStyle,
        breakpoints,
        columns,
        embedded,
        headerOptions,
        isCollapsed,
        nestingDepth,
        selectable,
        spacing,      
    } = layoutParameters;


    /**
     * If a regular object is passed in, turn it into an AfwObject.
     */
    useEffect(() => {
        const initObject = async (obj) => {
            const object = model.newObject({ object: obj });
            await object.initialize();

            dispatch({ type: "OBJECT", object });               
        };

        if (obj && model) {
            if (obj instanceof AfwObject) {
                dispatch({ type: "OBJECT", object: obj });                
            } else {
                /* We got a plain old object in, need to initialize it. */
                initObject(obj);
            }
        }
    }, [model, obj]);


    /**
     * When the object, filter or sorting options change, update the
     * layout to present only those properties that should be visible.
     */
    useEffect(() => dispatch({ type: "FILTER", filterOptions }), [filterOptions]);    

    useEffect(() => dispatch({ type: "SORT", sortOptions }), [sortOptions]);

    useEffect(() => dispatch({ type: "PROPERTIES_CHANGED" }), [propertiesChanged]);
    
    useEffect(() => dispatch({ type: "CHANGES_DISCARDED" }), [changesDiscarded]);

    /* editable state has changed */
    useEffect(() => dispatch({ type: "EDITABLE", editable }) , [editable]);    


    /* Determines if the trash can icon is clickable.  If there are no valid properties that can be removed, then don't allow it to be clicked. */
    const canDeleteProperties = useMemo(() => {

        if (selectedProperties.length === 0)
            return false;

        /* Don't allow delete if a property selected is required */
        for (const property of selectedProperties) {
            if (property.getAllowWrite() === false)
                return false;

            if (property.getRequired())
                return false;
        }

        return true;
    }, [selectedProperties]);

    /* determine whether or not to display a header above the properties */
    const isHeaderVisible = 
            (nestingDepth > 0 && (headerOptions === "all" || headerOptions === "embedded")) ||
            (nestingDepth === 0 && (headerOptions === "all" || headerOptions === "base")); 
            
    /*
        This determines if the properties being displayed have been filtered.  This
        is used to determine whether to allow properties to be selected and removed.  For example,
        if we wish to display all Required properties, even those without value, then we don't
        want to allow them to select one and remove it.
     */
    const isFiltered =                 
        filterOptions.filterProperties || filterOptions.filterRequiredProperties ||
        filterOptions.filterCondOrProperties || filterOptions.filterCondAndProperties ||
        filterOptions.onFilter;

   
    /*
        Use the preferredComponents prop to determine which componentType to use in this property layout.  When
        using the preferredComponents, the user can apply a specific componentType layout based on any or all of
        the following specifiers:  dataType and propertyName.

        This allows you to change all boolean dataTypes to Toggles instead of Checkbox, for example.  Or use
        TextFields for a specific property.
     */
    const getPreferredComponent = (property) => {
        let preferredComponent;
        let preferredComponentProps;
        
        if (preferredComponents) {
            preferredComponents.forEach(pref => {
                if (pref.propertyName) {
                    if (pref.propertyName && (pref.propertyName === property.getName()))
                        preferredComponent = pref.component;
                } else {
                    if (pref.dataType && (pref.dataType === property.getDataType())) {
                        preferredComponent = pref.component;
                        preferredComponentProps = pref.componentProps;
                    } else if (!pref.dataType) {
                        /* nothing to match on, so just use the componentProps */
                        preferredComponentProps = pref.componentProps;
                    }
                }
            });
        }

        return { preferredComponent, preferredComponentProps };
    };

    /*
        This routine determines if a property has been selected, so that it can be removed with the delete icon.
        It displays the checkbox and selected div in a shaded background to illustrate it's been selected.
     */
    const isPropertySelected = (property) => {
        return selectedProperties.some(p => p.getName() === property.getName());
    };        
        

    /*
        Decides whether a property can be selected.  Shouldn't be able to remove required properties, or
        those that have already had a special filter applied.
     */
    const canSelectProperty = (property) => {
        /* if it was added, this is ok to select for removal */
        if (addedProperties.includes(property))
            return true;

        if (isFiltered)
            return false;        

        return true;
    };

    /*
        This routine is called when the user selects a property by clicking on the div that surrounds the property.
        When it's selected, we add it ot the selectedProperties[] array, which is later used to remove properties.
     */
    const _onSelectProperty = (event, property) => {        

        /* first stop the event propagation so parent divs aren't also selected by accident. */
        if (event) {
            event.preventDefault();
            event.stopPropagation();
        }       

        dispatch({ type: "PROPERTY_SELECTED", property });

        if (!isHeaderVisible && onSelectProperty) {
            onSelectProperty(event, property);
        } 
    };   

    /**
     * _onRenderProperty(property, index, count)
     *
     * This is a callback routine that is called when we have a property that needs
     * to be rendered.  It provides the property, along with the index of the property
     * among the list of properties, and the count of total properties in the list.
     *
     * The default behavior is to use the PropertyResponsive layout to render an individual 
     * property unless its overridden by the preferredComponents prop and also by the callers
     * onRenderProperty callback, which can completely change its behavior.  The callers
     * onRenderProperty prop also gets a second parameter, which is the renderedComponent, in 
     * case it would need to wrap the default rendered property.     
     */
    const _onRenderProperty = (property, index, count) => {                
        const propertySelected = isPropertySelected(property);

        let autoFocusProperty = false;
        if (autoFocus) {
            if (lastAddedProperty === property)
                autoFocusProperty = true;
            else if ((lastAddedProperty === undefined) && (index === 0))
                autoFocusProperty = true;
        }

        /* determine the preferredComponent and its props that may need to be passed along */
        const {preferredComponent, preferredComponentProps} = getPreferredComponent(property);

        /* pass along recursive options, but exclude filterProperties, since they'll rarely be recursive. */
        const {filterProperties, ...embeddedFilterOptions} = filterOptions;  // eslint-disable-line no-unused-vars

        let mergedLayoutParameters = { 
            ...layoutParameters, 
            breakpoints: undefined,    
            filterOptions: (filterOptions && (filterOptions.recurse !== false)) ? embeddedFilterOptions : undefined,
            sortOptions: (sortOptions && !(sortOptions.recurse === false)) ? sortOptions : undefined,    
            preferredComponent, 
            ...preferredComponentProps,                            
            nestingDepth: nestingDepth + 1,
        };
        
        const propertyResponsive = (
            <AdaptiveLayoutContext.Provider value={{ 
                layoutParameters: { ...mergedLayoutParameters }, 
                data: { object: undefined, property } 
            }}>
                <PropertyResponsive                        
                    // eslint-disable-next-line jsx-a11y/no-autofocus
                    autoFocus={autoFocusProperty}        
                />                             
            </AdaptiveLayoutContext.Provider>
        );

        const style = {  
            marginBottom: spacing, 
            marginTop: editable ? spacing : 0,
            padding: editable ? theme.spacing(1) : 0, 
            ...propertyStyle, 
            width: "100%" 
        };

        if (selectable && editable && (isHeaderVisible || onSelectProperty) && canSelectProperty(property)) {
            if (propertySelected) {                
                style.borderRadius = theme.shape.borderRadius;
                style.border = "dashed thin " + theme.palette.primary.main;
            }
        }

        /* Set up a div-wrapped PropertyResponsive, so we can apply some common style to each one, and allow it to be selectable. */
        const renderedProperty = (
            <div
                id={"ObjectResponsiveProperty-" + nestingDepth + "-" + property.getName() + "-" + index}
                style={style}
                onClick={(event) => event.target.id?.indexOf("ObjectResponsiveProperty-") === 0 && _onSelectProperty(event, property)}
                onKeyPress={(event) => event.target.id?.indexOf("ObjectResponsiveProperty-") === 0 && _onSelectProperty(event, property)}
                role="button"
                tabIndex={(editable && selectable && canSelectProperty(property)) ? 0 : undefined}
            >
                {
                    /* Only display selectable divs if the layout allows it and we have a visible header to work with. */
                    (selectable && editable && (isHeaderVisible || onSelectProperty) && canSelectProperty(property)) ?  (
                        <div style={{ display: "flex" }}>
                            <div
                                style={{ width: "64px" }}
                                onClick={(event) => _onSelectProperty(event, property)}
                                onKeyPress={event => _onSelectProperty(event, property)}
                                role="button"
                                tabIndex={-1}                                                            
                            >
                                <Checkbox                          
                                    value={isPropertySelected(property)} 
                                    aria-label={"Select Property"}
                                    tooltip={"Select Property " + property.getName()}
                                    icon="check_circle_outline"
                                    checkedIcon="check_circle"
                                />
                            </div>
                            <div className={classes.PropertyValueContainer}>
                                { propertyResponsive }
                            </div>
                        </div>
                    ) : propertyResponsive                    
                }
            </div>
        );

        /* if the caller needs to render the property on its own, call it with our renderedProperty */
        if (onRenderProperty)
            return onRenderProperty(props, property, renderedProperty, index, count);
        else
            return renderedProperty;
    };

    /*
        This routine is called when the user adds a new property to the object.
     */
    const onAddProperty = (property) => {
        dispatch({ type: "PROPERTY_ADDED", property });        
    };

    /*
        This routine is called when the user chooses to delete properties that are selected by clicking the
        Delete icon.  We need to go through each of our selected properties and remove them one by one.
     */
    const onDeleteSelectedProperties = () => {
        dispatch({ type: "PROPERTIES_DELETE" });        
    };

    /**
     * _onRenderHeader()
     *
     * When a header is being applied, we describe the object with its Label, Description along with
     * Edit buttons where properties can be added or removed.
     */
    const _onRenderHeader = () => 
        <ObjectResponsiveHeader      
            tooltip={props.tooltip}                     
            label={props.label}
            description={props.description}
            isHeaderVisible={isHeaderVisible}
            canDeleteProperties={canDeleteProperties}
            onAddProperty={onAddProperty}
            onDeleteSelectedProperties={onDeleteSelectedProperties}
            onRenderHeader={onRenderHeader}
            nestingDepth={nestingDepth}
        />;
        

    /* make sure we have an object */
    if (!object)
        return null;

    /* layout the visibleProperties into responsive rows and columns */    
    let rows = [];    
    if (object) {            
        for (let i = 0; i < visibleProperties.length; i += columns) {
            let cols = [];
            for (let j = 0; j < columns && (i + j < visibleProperties.length); j++) {
                const prop = visibleProperties[i + j];

                cols.push({
                    breakpoints,
                    contains: _onRenderProperty(prop, i + j, visibleProperties.length)
                });
            }
            rows.push({ columns: cols });
        }
    }


    /* The default object layout is a Responsive one, where each property is its own row.  */    
    if (nestingDepth) {
        /* For embedded objects, there are a variety of ways to represent it in the UI layout. */
        return (
            <EmbeddedLayout                 
                layout={<Responsive rows={rows} />}
                isCollapsed={isCollapsed}
                property={property}
                nestingDepth={nestingDepth}
                embedded={embedded}
                onRenderHeader={_onRenderHeader}
                ariaLabel={ariaLabel}
                style={style}
            />
        );
    } 

    /* All non-embedded objects have an optional header, followed by the layout */
    return (
        <div style={style} data-component-type="ObjectResponsive" data-testid={dataTestId}>
            <div style={{ width: "100%" }}>
                { isHeaderVisible && _onRenderHeader() }
            </div>                
            <div className={bodyClassName} style={bodyStyle}>
                { showMeta && (object.getObjectTypeId() !== "_AdaptiveMeta_") &&
                    /**                                          
                     * Render an optional metadata section, to display the object's
                     * id, object type, path and parent paths, etc.
                     */
                    <>
                        <OperationalContext.Provider value={OperationalMode.NotEditable}>
                            <ObjectResponsive
                                object={object.getMetaObject()}
                                aria-label="Object Metadata"
                                label="Object Metadata"
                                description="Object's meta information"
                                nestingDepth={nestingDepth + 1}
                                showMeta={false}
                                filterOptions={{ filterValuedProperties: true }}
                            />
                        </OperationalContext.Provider>
                        <div style={{ height: theme.spacing(2) }} />
                    </>
                }                 
                <Responsive rows={rows} />                                
            </div>
        </div>
    );
    
};

ObjectResponsive.displayName = "ObjectResponsive";

export default ObjectResponsive;
