// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Paper
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Paper.propTypes";
import {IPaperProps} from "./Paper.types";
import fallback from "./Paper.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Paper
 * Category          : containers
 * 
 * A component that allows contents to be displayed on a Paper like surface.
 * 
 * This component allows data on its surface to stand out by providing an
 * edged, or raised outline, with shadow depth and optional corner radius
 * style. This component is another useful container to help separate a
 * section of data from its surrounding contents.
 * 
 */
export const Paper : React.FunctionComponent<IPaperProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Paper")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Paper",
                parameters: props
            }}
        />
    );
};


Paper.propTypes = propTypes;
Paper.defaultProps = defaultProps;
Paper.displayName = "Paper";

export default React.memo(Paper);
