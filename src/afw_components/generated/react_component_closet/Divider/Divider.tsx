// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Divider
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Divider.propTypes";
import {IDividerProps} from "./Divider.types";
import fallback from "./Divider.fallback";

import {AdaptiveComponent} from "../AdaptiveComponent";
import {ctx} from "../../utils/utils";


/**
 * Implementation Id : Divider
 * Category          : content
 * 
 * A component that displays a Divider line to separate content.
 * 
 * A very simple way to divide content from the remaining content with the use
 * of a dividing line.
 * 
 */
export const Divider : React.FunctionComponent<IDividerProps> = (props) => {

    props = {...defaultProps, ...props};

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Divider")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Divider",
                parameters: props
            }}
        />
    );
};


Divider.propTypes = propTypes;
Divider.displayName = "Divider";

export default React.memo(Divider);
