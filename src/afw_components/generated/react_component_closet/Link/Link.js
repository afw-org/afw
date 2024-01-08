// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Link
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Link.propTypes";
import fallback from "./Link.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


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
export const Link = (props) => {

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
Link.defaultProps = defaultProps;
Link.displayName = "Link";

export default memo(Link);
