// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Native
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Native.propTypes";
import {INativeProps} from "./Native.types";
import fallback from "./Native.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Native
 * Category          : content
 * 
 * A component that displays a native HTML element.
 * 
 * This component represents any additional native HTML node that is required
 * by the end user, which is not covered by other components within this
 * library.
 * 
 */
export const Native : React.FunctionComponent<INativeProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Native")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Native",
                parameters: props
            }}
        />
    );
};


Native.propTypes = propTypes;
Native.defaultProps = defaultProps;
Native.displayName = "Native";

export default React.memo(Native);
