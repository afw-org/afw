// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Toolbar
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Toolbar.propTypes";
import {IToolbarProps} from "./Toolbar.types";
import fallback from "./Toolbar.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Toolbar
 * Category          : containers
 * 
 * A container component that arranges a series of buttons or other minor
 * content in a horizontal bar.
 * 
 * This component is used to provide actions, via buttons, labels or other
 * small components, condensed into a common location so the user may use it
 * to view or change data that is presented in the layout below.
 * 
 */
export const Toolbar : React.FunctionComponent<IToolbarProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Toolbar")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Toolbar",
                parameters: props
            }}
        />
    );
};


Toolbar.propTypes = propTypes;
Toolbar.defaultProps = defaultProps;
Toolbar.displayName = "Toolbar";

export default React.memo(Toolbar);
