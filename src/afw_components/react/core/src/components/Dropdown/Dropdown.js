// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Dropdown
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Dropdown.propTypes";
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
export const Dropdown = (props) => {

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

export default memo(Dropdown);
