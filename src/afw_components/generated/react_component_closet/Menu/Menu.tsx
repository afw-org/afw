// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Menu
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Menu.propTypes";
import {IMenuProps} from "./Menu.types";
import fallback from "./Menu.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Menu
 * Category          : navigation
 * 
 * A component that renders a Menu to be displayed.
 * 
 * This component pops up a Menu of items, each of which take an appropriate
 * action when clicked. Menu items may also open up new submenus in order to
 * organize or filter a set of items in a way that's easy for the end user to
 * understand and select from.
 * 
 */
export const Menu : React.FunctionComponent<IMenuProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Menu")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Menu",
                parameters: props
            }}
        />
    );
};


Menu.propTypes = propTypes;
Menu.defaultProps = defaultProps;
Menu.displayName = "Menu";

export default React.memo(Menu);
