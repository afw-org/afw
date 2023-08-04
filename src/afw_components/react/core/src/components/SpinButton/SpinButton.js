// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for SpinButton
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./SpinButton.propTypes";
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
export const SpinButton = (props) => {

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

export default memo(SpinButton);
