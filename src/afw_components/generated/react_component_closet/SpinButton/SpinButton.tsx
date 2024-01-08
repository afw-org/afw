// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for SpinButton
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import React from "react";
import {propTypes, defaultProps} from "./SpinButton.propTypes";
import {ISpinButtonProps} from "./SpinButton.types";
import fallback from "./SpinButton.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";


/**
 * Implementation Id : SpinButton
 * Category          : inputs
 * 
 * A component that renders a numeric field that an be incremented or
 * decremented.
 * 
 * A component that renders a numeric field that an be incremented or
 * decremented.
 * 
 */
export const SpinButton : React.FunctionComponent<ISpinButtonProps> = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "SpinButton")}
            fallback={fallback}
            layoutComponent={{
                componentType: "SpinButton",
                parameters: props
            }}
        />
    );
};


SpinButton.propTypes = propTypes;
SpinButton.defaultProps = defaultProps;
SpinButton.displayName = "SpinButton";

export default React.memo(SpinButton);
