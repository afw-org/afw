// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Breadcrumb
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Breadcrumb.propTypes";
import {IBreadcrumbProps} from "./Breadcrumb.types";
import fallback from "./Breadcrumb.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Breadcrumb
 * Category          : containers
 * 
 * A component that renders a Breadcrumb path with links
 * 
 * This component renders a path of url links that the user can click to
 * navigate at any point within an application.
 * 
 */
export const Breadcrumb : React.FunctionComponent<IBreadcrumbProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Breadcrumb")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Breadcrumb",
                parameters: props
            }}
        />
    );
};


Breadcrumb.propTypes = propTypes;
Breadcrumb.defaultProps = defaultProps;
Breadcrumb.displayName = "Breadcrumb";

export default React.memo(Breadcrumb);
