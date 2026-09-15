// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Snackbar
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Snackbar.propTypes";
import {ISnackbarProps} from "./Snackbar.types";
import fallback from "./Snackbar.fallback";

import {AdaptiveComponent} from "../AdaptiveComponent";
import {ctx} from "../../utils/utils";


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
export const Snackbar : React.FunctionComponent<ISnackbarProps> = (props) => {

    props = {...defaultProps, ...props};

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
Snackbar.displayName = "Snackbar";

export default React.memo(Snackbar);
