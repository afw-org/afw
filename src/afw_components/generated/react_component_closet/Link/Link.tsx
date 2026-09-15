// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Link
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Link.propTypes";
import {ILinkProps} from "./Link.types";
import fallback from "./Link.fallback";

import {AdaptiveComponent} from "../AdaptiveComponent";
import {ctx} from "../../utils/utils";


/**
 * Implementation Id : Link
 * Category          : navigation
 * 
 * A component that renders a url link.
 * 
 * This component understands how to render a link with the appropriate
 * framework style, and handles the React Routing, if the link references
 * another component within this same application.
 * 
 */
export const Link : React.FunctionComponent<ILinkProps> = (props) => {

    props = {...defaultProps, ...props};

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Link")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Link",
                parameters: props
            }}
        />
    );
};


Link.propTypes = propTypes;
Link.displayName = "Link";

export default React.memo(Link);
