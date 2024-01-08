// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Autocomplete
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Autocomplete.propTypes";
import fallback from "./Autocomplete.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Autocomplete
 * Category          : inputs
 * 
 * Component to display a list of options that may be selected or can be
 * autocompleted by typing.
 * 
 * This component provides a dropdown picker with possible values to quickly
 * select from.  By typing, it will narrow down the selection to make the
 * list even shorter to choose a selection from.  Additionally, it will offer
 * the user the ability to enter a new value, if allowed.
 * 
 */
export const Autocomplete = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Autocomplete")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Autocomplete",
                parameters: props
            }}
        />         
    );
};


Autocomplete.propTypes = propTypes;
Autocomplete.defaultProps = defaultProps;
Autocomplete.displayName = "Autocomplete";

export default memo(Autocomplete);
