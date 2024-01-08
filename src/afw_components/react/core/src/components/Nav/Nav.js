// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Nav
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Nav.propTypes";
import fallback from "./Nav.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Nav
 * Category          : navigation
 * 
 * A navigational component that displays a list of links.
 * 
 * This component, often referred to as a Side Nav, displays a list of
 * clickable links into a horizontal panel.
 * 
 */
export const Nav = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Nav")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Nav",
                parameters: props
            }}
        />
    );
};


Nav.propTypes = propTypes;
Nav.defaultProps = defaultProps;
Nav.displayName = "Nav";

export default memo(Nav);
