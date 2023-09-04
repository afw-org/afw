// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Responsive
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Responsive.propTypes";
import fallback from "./Responsive.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Responsive
 * Category          : containers
 * 
 * A layout container that renders content in a grid format, with breakpoints
 * defined for a variety of screen sizes.
 * 
 * This container component defines a Row/Column grid of components to display
 * content horizontally and vertically, depending on the screen size. For
 * larger displays, you may assign breakpoints that allow for all content to
 * fit, while on smaller mobile devices, content will be divided up into
 * chunks.
 * 
 */
export const Responsive = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Responsive")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Responsive",
                parameters: props
            }}
        />
    );
};


Responsive.propTypes = propTypes;
Responsive.defaultProps = defaultProps;
Responsive.displayName = "Responsive";

export default memo(Responsive);
