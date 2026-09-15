// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Checkbox
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Checkbox.propTypes";
import {ICheckboxProps} from "./Checkbox.types";
import fallback from "./Checkbox.fallback";

import {AdaptiveComponent} from "../AdaptiveComponent";
import {ctx} from "../../utils/utils";


/**
 * Implementation Id : Checkbox
 * Category          : inputs
 * 
 * A component that displays a checkbox.
 * 
 * This component is simply a checkbox that allows the user to click to enable
 * or disable its value.
 * 
 */
export const Checkbox : React.FunctionComponent<ICheckboxProps> = (props) => {

    props = {...defaultProps, ...props};

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Checkbox")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Checkbox",
                parameters: props
            }}
        />
    );
};


Checkbox.propTypes = propTypes;
Checkbox.displayName = "Checkbox";

export default React.memo(Checkbox);
