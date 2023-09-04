// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Snackbar
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Snackbar.propTypes";
import fallback from "./Snackbar.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Snackbar
 * Category          : utilities
 * 
 * A component that renders a Snackbar to be displayed.
 * 
 * This component pops up a small bar at the bottom of the screen to alert the
 * user of a recent activity, such as a success or error status, typically
 * following an action that was taken by the user.
 * 
 */
export const Snackbar = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Snackbar")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Snackbar",
                parameters: props
            }}
        />
    );
};


Snackbar.propTypes = propTypes;
Snackbar.defaultProps = defaultProps;
Snackbar.displayName = "Snackbar";

export default memo(Snackbar);
