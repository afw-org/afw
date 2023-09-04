// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Dialog
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./Dialog.propTypes";
import {IDialogProps} from "./Dialog.types";
import fallback from "./Dialog.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : Dialog
 * Category          : containers
 * 
 * A layout container that displays content inside a popup, with a
 * confirmation response.
 * 
 * This component container is visible when a target element is clicked. It's
 * used to quickly show or grab input from the user in a popup area and can be
 * dismissed by confirming or cancelling via buttons.
 * 
 */
export const Dialog : React.FunctionComponent<IDialogProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Dialog")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Dialog",
                parameters: props
            }}
        />
    );
};


Dialog.propTypes = propTypes;
Dialog.defaultProps = defaultProps;
Dialog.displayName = "Dialog";

export default React.memo(Dialog);
