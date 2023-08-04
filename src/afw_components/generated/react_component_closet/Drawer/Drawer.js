// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Drawer
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Drawer.propTypes";
import fallback from "./Drawer.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Drawer
 * Category          : containers
 * 
 * A component that renders content inside a side sheet anchored to one of
 * the edges.
 * 
 * This component, which contains other content, is anchored to one of the
 * edges of the main window. It's often animated to slide into view when an
 * action occurs, but can be optionally docked, or permanent.
 * 
 */
export const Drawer = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Drawer")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Drawer",
                parameters: props
            }}
        />
    );
};


Drawer.propTypes = propTypes;
Drawer.defaultProps = defaultProps;
Drawer.displayName = "Drawer";

export default memo(Drawer);
