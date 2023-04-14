// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Responsive
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */
import PropTypes from "prop-types";
import Grid from "@mui/material/Grid";

import {AdaptiveComponent} from "@afw/react";


const Breakpoints = {
    small:              PropTypes.oneOf([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
    medium:             PropTypes.oneOf([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
    large:              PropTypes.oneOf([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
    xl:                 PropTypes.oneOf([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
    xxl:                PropTypes.oneOf([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
    xxxl:               PropTypes.oneOf([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
};

const Visibility = {
    small:              PropTypes.bool,
    medium:             PropTypes.bool,
    large:              PropTypes.bool,
    xl:                 PropTypes.bool,
    xxl:                PropTypes.bool,
    xxxl:               PropTypes.bool,
};

/* simple map to convert human-readable sizes to Grid styles */
const SizeMap = {
    small:    "xs",
    medium:   "sm",
    large:    "md",
    xl:       "lg",
    xxl:      "xl",
};

const getBreakpoints = (breakpoints) => {
    let breaks = {};

    if (!breakpoints)
        return breaks;

    for (let [size, breakpoint] of Object.entries(breakpoints))
        breaks[SizeMap[size]] = breakpoint;

    return breaks;
};

export const ResponsiveCol = (props) => {   
               
    const {style, contains, breakpoints, numCols, ...rest} = props;

    if (!contains)
        return null;
    
    const breakpointsObj = getBreakpoints(breakpoints);
    
    return (
        <Grid style={{ width: String(100 / numCols) + "%", ...style }} item {...breakpointsObj}>
            <AdaptiveComponent {...rest} layoutComponent={contains} />
        </Grid>
    );   
};

ResponsiveCol.propTypes = {
    contains:           PropTypes.oneOfType([ PropTypes.element, PropTypes.object ]),
    breakpoints:        PropTypes.shape(Breakpoints),
    visibility:         PropTypes.shape(Visibility),
};


export const ResponsiveRow = (props) => {

    if (!props.columns)
        return null;

    const {style, columns, spacing, justify, alignItems, alignContent, ...rest} = props;

    return (
        <Grid 
            style={style} 
            container 
            spacing={spacing}
            justifyContent={justify}
            alignItems={alignItems}
            alignContent={alignContent}
        >
            {
                columns.map((child, index) => 
                    <ResponsiveCol 
                        {...child}
                        {...rest}
                        key={index}
                        numCols={columns.length}
                    />
                )
            }
        </Grid>
    );
};

ResponsiveRow.propTypes = {
    columns:            PropTypes.arrayOf(PropTypes.shape(ResponsiveCol.propTypes)),
    style:              PropTypes.object,
    spacing:            PropTypes.oneOf([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]),
    justify:            PropTypes.oneOf(["flex-start", "center", "flex-end", "space-between", "space-around"]),
    alignContent:       PropTypes.oneOf(["stretch", "center", "flex-start", "flex-end", "space-between", "space-around"]),
    alignItems:         PropTypes.oneOf(["flex-start", "center", "flex-end", "stretch", "baseline"]),
};


/**
 * Implementation Id : Responsive
 * Category          : containers
 * 
 * A layout container that renders content in a grid format, with breakpoints
 * defined for a variety of screen sizes.
 * 
 * This container component defines a Row/Column grid of components to
 * display content horizontally and vertically, depending on the screen size.
 * For larger displays, you may assign breakpoints that allow for all
 * content to fit, while on smaller mobile devices, content will be divided
 * up into chunks.
 * 
 */
export const Responsive = (props) => {
      
    if (!props.rows && !props.items)
        return null;

    const {
        id, "data-testid": 
        dataTestId, 
        "data-component-type": dataComponentType, 
        style, className, rows, spacing, justify, align, items,
        ...rest
    } = props;

    return (
        <Grid 
            id={id}
            data-testid={dataTestId}
            data-component-type={dataComponentType}
            className={className}
            style={style} 
            container         
            justifyContent={justify}
            align={align}
            spacing={spacing}
        >
            {
                rows && rows.map((child, index) =>                    
                    <ResponsiveRow key={index} {...child} {...rest} />                    
                )
            }
            {
                items && items.map((item, index) => {

                    const {breakpoints, contains} = item;
                    const muiBreakpoints = getBreakpoints(breakpoints);

                    return (
                        <Grid key={index} item {...muiBreakpoints}>
                            <AdaptiveComponent layoutComponent={contains} />
                        </Grid>
                    );

                })
            }
        </Grid>
    );
};

/**
 * PropTypes generated by _AdaptiveLayoutComponentType_Responsive
 */
Responsive.propTypes = {    
    /**
     * justify
     * Data Type: (string)
     * 
     * Justify content.
     */
    justify:                            PropTypes.oneOf(["flex-start", "center", "flex-end", "space-between", "space-around"]),
    /**
     * rows
     * Data Type: (list, object _AdaptiveLayoutComponentType_ResponsiveRow)
     * 
     * The layout row components that are contained by this Responsive.
     */
    rows:                               PropTypes.arrayOf(PropTypes.shape(ResponsiveRow.propTypes)),
    /**
     * spacing
     * Data Type: (integer)
     * 
     * Spacing between rows.
     */
    spacing:            PropTypes.number,
};

Responsive.defaultProps = {
};

export default Responsive;
