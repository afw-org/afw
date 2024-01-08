// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Icon
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Icon.propTypes";
import {IIconProps} from "./Icon.types";
import fallback from "./Icon.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Icon
 * Category          : content
 * 
 * A component that displays an icon.
 * 
 * This component displays an icon, which references a common dictionary of
 * icon names, each of which may map to the appropriate icon library depending
 * on the framework that is being used. The icon itself may be clickable.
 * 
 */
export const Icon : React.FunctionComponent<IIconProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Icon")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Icon",
                parameters: props
            }}
        />
    );
};


Icon.propTypes = propTypes;
Icon.defaultProps = defaultProps;
Icon.displayName = "Icon";

export default React.memo(Icon);
