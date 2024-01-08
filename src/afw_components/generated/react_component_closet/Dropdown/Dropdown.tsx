// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Dropdown
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Dropdown.propTypes";
import {IDropdownProps} from "./Dropdown.types";
import fallback from "./Dropdown.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Dropdown
 * Category          : pickers
 * 
 * A component that displays a set of options that can be selected from a
 * dropdown.
 * 
 * This component, given a set of options, provides the user with the ability
 * to select one of the values by clicking on the dropdown icon and selecting
 * the appropriate value from a list.
 * 
 */
export const Dropdown : React.FunctionComponent<IDropdownProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Dropdown")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Dropdown",
                parameters: props
            }}
        />
    );
};


Dropdown.propTypes = propTypes;
Dropdown.defaultProps = defaultProps;
Dropdown.displayName = "Dropdown";

export default React.memo(Dropdown);
